#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <vector>

class Attacker {
  public:
    Attacker( int health, int damage, int armor, int mana ) {
        _health = health;
        _damage = damage;
        _armor = armor;
        _mana = mana;
    }
    void receiveDamage( int damage ) {
        damage -= _armor;
        damage = damage <= 0 ? 1 : damage;
        _health -= damage;
    }
    int getDamage() const {
        return _damage;
    }
    bool isDead() const {
        return _health <= 0;
    }
    void addDamage( int damage ) {
        _damage += damage;
    }
    void addArmor( int armor ) {
        _armor += armor;
    }
    int getHealth() const {
        return _health;
    }
    void heal( int health ) {
        _health += health;
    }
    void removeArmor( int armor ) {
        _armor -= armor;
    }
    void addMana( int mana ) {
        _mana += mana;
    }
    void removeMana( int mana ) {
        _mana -= mana;
    }
    bool canCast( int spell_cost ) const {
        return _mana >= spell_cost;
    }
    int getMana() {
        return _mana;
    }

  private:
    int _health;
    int _damage;
    int _armor;
    int _mana;
};

void magicMissile( Attacker &target, Attacker & /*unused*/, int &countdown ) {
    if ( countdown == 0 )
        return;
    target.receiveDamage( 4 );
    countdown--;
}

void drain( Attacker &target, Attacker &self, int &countdown ) {
    if ( countdown == 0 )
        return;
    target.receiveDamage( 2 );
    self.heal( 2 );
    countdown--;
}

void shield( Attacker & /*unused*/, Attacker &self, int &countdown ) {
    if ( countdown == 0 )
        return;
    if ( countdown == 6 )
        self.addArmor( 7 );
    if ( countdown == 1 )
        self.removeArmor( 7 );
    countdown--;
}

void poison( Attacker &target, Attacker & /*unused*/, int &countdown ) {
    if ( countdown == 0 )
        return;
    target.receiveDamage( 3 );
    countdown--;
}

void recharge( Attacker & /*unused*/, Attacker &self, int &countdown ) {
    if ( countdown == 0 )
        return;
    self.addMana( 101 );
    countdown--;
}

const std::vector<std::pair<
    std::string,
    std::tuple<int, void ( * )( Attacker &, Attacker &, int & ), int>>>
    magic = {
        { "Magic Missile", { 53, magicMissile, 1 } },
        { "Drain", { 73, drain, 1 } },
        { "Shield", { 113, shield, 6 } },
        { "Poison", { 173, poison, 6 } },
        { "Recharge", { 229, recharge, 5 } },
};

Attacker getBoss( std::ifstream &file ) {
    std::string str;
    int health;
    int damage;
    while ( std::getline( file, str ) ) {
        std::stringstream ss( str );
        ss >> str;
        if ( str == "Hit" ) {
            ss >> str;
            ss >> health;
        } else if ( str == "Damage:" ) {
            ss >> damage;
        }
    }
    return Attacker( health, damage, 0, 0 );
}

size_t getPrice( const std::vector<std::pair<int, int>> &cast ) {
    size_t ret = 0;
    for ( auto &spell : cast ) {
        ret += std::get<0>( magic[spell.first].second );
    }
    return ret;
}

void printVec( const std::vector<std::pair<int, int>> &cast ) {
    for ( auto &spell : cast ) {
        std::cout << "\t{" << spell.first << ", " << spell.second << "}"
                  << std::endl;
    }
}

bool spellValid( const std::vector<std::pair<int, int>> &cast, int new_spell ) {
    if ( new_spell < 2 )
        return true;
    for ( auto &spell : cast ) {
        if ( spell.first == new_spell && spell.second > 0 )
            return false;
    }
    return true;
}

void cheapestRecursion( const std::vector<std::pair<int, int>> &cast,
                        const Attacker &player, const Attacker &boss,
                        size_t &cheapest,
                        std::vector<std::pair<int, int>> &best,
                        bool hard = false ) {
    if ( getPrice( cast ) >= cheapest )
        return;
    std::vector<std::pair<int, int>> base = cast;

    Attacker base_player( player );
    Attacker base_boss( boss );
    // effects before boss' turn
    for ( auto &spell : base ) {
        std::get<1>( magic[spell.first].second )( base_boss, base_player,
                                                  spell.second );
    }
    if ( base_boss.isDead() ) {
        if ( getPrice( base ) < cheapest ) {
            cheapest = getPrice( base );
            best = base;
        }
        return;
    }
    // boss' turn
    base_player.receiveDamage( base_boss.getDamage() );
    // end of boss' turn = start of player's turn
    if ( hard )
        base_player.receiveDamage( 1 );
    if ( base_player.isDead() )
        return;

    // effects before player's turn
    for ( auto &spell : base ) {
        std::get<1>( magic[spell.first].second )( base_boss, base_player,
                                                  spell.second );
    }
    if ( base_boss.isDead() ) {
        if ( getPrice( base ) < cheapest ) {
            cheapest = getPrice( base );
            best = base;
        }
        return;
    }
    for ( int i = 0; i < 5; i++ ) {
        std::vector<std::pair<int, int>> cur = base;
        Attacker cur_boss( base_boss );
        Attacker cur_player( base_player );
        if ( cur_player.canCast( std::get<0>( magic[i].second ) ) &&
             spellValid( cur, i ) ) {
            cur_player.removeMana( std::get<0>( magic[i].second ) );
            cur.emplace_back( i, std::get<2>( magic[i].second ) );
            cheapestRecursion( cur, cur_player, cur_boss, cheapest, best,
                               hard );
        }
    }
}

int main() {
    std::ifstream input_file( "input" );
    auto boss = getBoss( input_file );
    Attacker player( 50, 0, 0, 500 );
    player.heal( boss.getDamage() );
    std::vector<std::pair<int, int>> cast = {};
    size_t cheapest = -1;
    std::vector<std::pair<int, int>> best = {};

    cheapestRecursion( cast, player, boss, cheapest, best );

    std::string spell_str{};
    for ( size_t i = 0; i < best.size() - 1; i++ ) {
        spell_str += magic[best[i].first].first + ", ";
    }
    spell_str += magic[best[best.size() - 1].first].first;

    std::cout << "The cheapest way to defeat the boss costs \033[93;1m"
              << cheapest << "\033[0m mana (you cast \033[91;1m" << spell_str
              << "\033[0m)" << std::endl;

    //-------PART 2------------------------------------------------------------

    cheapest = -1;
    cast = {};
    best = {};

    cheapestRecursion( cast, player, boss, cheapest, best, true );

    spell_str = "";
    for ( size_t i = 0; i < best.size() - 1; i++ ) {
        spell_str += magic[best[i].first].first + ", ";
    }
    spell_str += magic[best[best.size() - 1].first].first;

    std::cout
        << "The cheapest way to defeat the boss (on hard) costs \033[93;1m"
        << cheapest << "\033[0m mana (you cast \033[91;1m" << spell_str
        << "\033[0m)" << std::endl;
}
