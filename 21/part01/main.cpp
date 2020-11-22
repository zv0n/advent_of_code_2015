#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <vector>

const std::vector<std::pair<std::string, std::tuple<int, int, int>>> weapons = {
    { "Dagger", { 8, 4, 0 } },     { "Shortsword", { 10, 5, 0 } },
    { "Warhammer", { 25, 6, 0 } }, { "Longsword", { 40, 7, 0 } },
    { "Greataxe", { 74, 8, 0 } },
};

const std::vector<std::pair<std::string, std::tuple<int, int, int>>> armor = {
    { "Leather", { 13, 0, 1 } },    { "Chainmail", { 31, 0, 2 } },
    { "Splintmail", { 53, 0, 3 } }, { "Bandedmail", { 75, 0, 4 } },
    { "Platemail", { 102, 0, 5 } },
};

const std::vector<std::pair<std::string, std::tuple<int, int, int>>> rings = {
    { "Damage +1", { 25, 1, 0 } },  { "Damage +2", { 50, 2, 0 } },
    { "Damage +3", { 100, 3, 0 } }, { "Defense +1", { 20, 0, 1 } },
    { "Defense +2", { 40, 0, 2 } }, { "Defense +3", { 80, 0, 3 } },
};

class Attacker {
  public:
    Attacker( int health, int damage, int armor ) {
        _health = health;
        _damage = damage;
        _armor = armor;
    }
    void receiveDamage( int damage ) {
        damage -= _armor;
        damage = damage <= 0 ? 1 : damage;
        _health -= damage;
    }
    int getDamage() {
        return _damage;
    }
    bool isDead() {
        return _health <= 0;
    }
    void addDamage( int damage ) {
        _damage += damage;
    }
    void addArmor( int armor ) {
        _armor += armor;
    }
    int getHealth() {
        return _health;
    }

  private:
    int _health;
    int _damage;
    int _armor;
};

Attacker getBoss( std::ifstream &file ) {
    std::string str;
    int health;
    int damage;
    int armor;
    while ( std::getline( file, str ) ) {
        std::stringstream ss( str );
        ss >> str;
        if ( str == "Hit" ) {
            ss >> str;
            ss >> health;
        } else if ( str == "Damage:" ) {
            ss >> damage;
        } else {
            ss >> armor;
        }
    }
    return Attacker( health, damage, armor );
}

void increaseRings( std::vector<int> &vec ) {
    vec[2]++;
    if ( (size_t)vec[2] == rings.size() ) {
        vec[2] = 0;
        vec[3]++;
        if ( vec[3] == vec[2] )
            vec[3]++;
    } else if ( vec[2] == vec[3] ) {
        increaseRings( vec );
    }
}

bool increaseVec( std::vector<int> &vec ) {
    // always increase weapon
    vec[0]++;
    if ( (size_t)vec[0] == weapons.size() ) {
        vec[0] = 0;
        if ( vec[1] == -1 && vec[2] != -1 ) {
            // don't increase armor if increasing rings and armor is -1
            increaseRings( vec );
        } else {
            vec[1]++;
            if ( (size_t)vec[1] == armor.size() ) {
                if ( vec[2] == -1 )
                    vec[1] = -1;
                else
                    vec[1] = 0;
                increaseRings( vec );
            }
        }
    }
    if ( (size_t)vec[3] == rings.size() ) {
        if ( vec[1] == -1 ) {
            vec[1] = 0;
            vec[2] = 0;
            vec[3] = -1;
        } else {
            return false;
        }
    }
    return true;
}

void increaseStats( Attacker &attacker, std::vector<int> &vec ) {
    attacker.addDamage( std::get<1>( weapons[vec[0]].second ) );
    if ( vec[1] != -1 )
        attacker.addArmor( std::get<2>( armor[vec[1]].second ) );
    if ( vec[2] != -1 ) {
        attacker.addDamage( std::get<1>( rings[vec[2]].second ) );
        attacker.addArmor( std::get<2>( rings[vec[2]].second ) );
    }
    if ( vec[3] != -1 ) {
        attacker.addDamage( std::get<1>( rings[vec[3]].second ) );
        attacker.addArmor( std::get<2>( rings[vec[3]].second ) );
    }
}

bool battleWon( Attacker &player, Attacker &boss ) {
    while ( !player.isDead() ) {
        boss.receiveDamage( player.getDamage() );
        if ( boss.isDead() )
            return true;
        player.receiveDamage( boss.getDamage() );
    }
    return false;
}

size_t getPrice( const std::vector<int> &vec ) {
    if ( vec[0] == -1 )
        return -1;
    size_t ret = 0;
    ret += std::get<0>( weapons[vec[0]].second );
    if ( vec[1] != -1 )
        ret += std::get<0>( armor[vec[1]].second );
    if ( vec[2] != -1 )
        ret += std::get<0>( rings[vec[2]].second );
    if ( vec[3] != -1 )
        ret += std::get<0>( rings[vec[3]].second );
    return ret;
}

void findCheapest( std::vector<int> &bought, const Attacker &player,
                   const Attacker &boss ) {
    std::vector<int> ret = bought;
    while ( increaseVec( bought ) ) {
        Attacker cur_player( player );
        Attacker cur_boss( boss );
        increaseStats( cur_player, bought );
        if ( battleWon( cur_player, cur_boss ) &&
             getPrice( bought ) < getPrice( ret ) )
            ret = bought;
    }
    bought = ret;
}

void findMostExpensive( std::vector<int> &bought, const Attacker &player,
                        const Attacker &boss ) {
    std::vector<int> ret = bought;
    while ( increaseVec( bought ) ) {
        Attacker cur_player( player );
        Attacker cur_boss( boss );
        increaseStats( cur_player, bought );
        if ( !battleWon( cur_player, cur_boss ) &&
             ( getPrice( bought ) > getPrice( ret ) || ret[0] == -1 ) )
            ret = bought;
    }
    bought = ret;
}

int main() {
    std::ifstream input_file( "input" );
    auto boss = getBoss( input_file );
    Attacker player( 100, 0, 0 );
    std::vector<int> bought = { -1, -1, -1, -1 };
    findCheapest( bought, player, boss );

    std::cout << "The cheapest way to defeat the boss costs \033[93;1m"
              << getPrice( bought ) << "\033[0m gold (\033[91;1m"
              << weapons[bought[0]].first
              << ( bought[1] == -1 ? "" : ", " + armor[bought[1]].first )
              << ( bought[2] == -1 ? "" : ", " + rings[bought[2]].first )
              << ( bought[3] == -1 ? "" : ", " + rings[bought[3]].first )
              << "\033[0m)" << std::endl;

    bought = { -1, -1, -1, -1 };
    findMostExpensive( bought, player, boss );

    std::cout << "The most expensive way to lose to the boss costs \033[93;1m"
              << getPrice( bought ) << "\033[0m gold (\033[91;1m"
              << weapons[bought[0]].first
              << ( bought[1] == -1 ? "" : ", " + armor[bought[1]].first )
              << ( bought[2] == -1 ? "" : ", " + rings[bought[2]].first )
              << ( bought[3] == -1 ? "" : ", " + rings[bought[3]].first )
              << "\033[0m)" << std::endl;
}
