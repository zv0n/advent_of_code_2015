#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <vector>

class Ingredient {
  public:
    Ingredient() = delete;
    Ingredient( const std::string &name, int capacity, int durability,
                int flavor, int texture, int calories ) {
        _name = name;
        _capacity = capacity;
        _durability = durability;
        _flavor = flavor;
        _texture = texture;
        _calories = calories;
    }

    void setSpoons( int spoons ) {
        _spoons = spoons;
    }

    int getCapacity() {
        return _spoons * _capacity;
    }
    int getDurability() {
        return _spoons * _durability;
    }
    int getFlavor() {
        return _spoons * _flavor;
    }
    int getTexture() {
        return _spoons * _texture;
    }
    int getCalories() {
        return _spoons * _calories;
    }
    std::string getName() {
        return _name;
    }

  private:
    std::string _name;
    int _capacity;
    int _durability;
    int _flavor;
    int _texture;
    int _calories;
    int _spoons;
};

void getIngredients( std::vector<Ingredient> &ingredients,
                     std::ifstream &file ) {
    std::string str;
    std::string name;
    int info[5];
    while ( std::getline( file, str ) ) {
        std::stringstream ss( str );
        ss >> name;
        name = name.substr( 0, name.length() - 1 );
        ss >> str;
        ss >> info[0];
        for ( int i = 0; i < 4; i++ ) {
            ss >> str;
            ss >> str;
            ss >> info[i + 1];
        }
        ingredients.emplace_back( name, info[0], info[1], info[2], info[3],
                                  info[4] );
    }
}

int getCookieScore( std::vector<Ingredient> &ingredients ) {
    int info[5] = { 0, 0, 0, 0, 0 };
    for ( auto &ingredient : ingredients ) {
        info[0] += ingredient.getCapacity();
        info[1] += ingredient.getDurability();
        info[2] += ingredient.getFlavor();
        info[3] += ingredient.getTexture();
        info[4] += ingredient.getCalories();
    }
    int ret = 1;
    for ( int i = 0; i < 4; i++ ) { // we don't want callories yet
        if ( info[i] < 0 )
            info[i] = 0;
        ret *= info[i];
    }
    return ret;
}

int getCookieScore500( std::vector<Ingredient> &ingredients ) {
    int info[5] = { 0, 0, 0, 0, 0 };
    for ( auto &ingredient : ingredients ) {
        info[0] += ingredient.getCapacity();
        info[1] += ingredient.getDurability();
        info[2] += ingredient.getFlavor();
        info[3] += ingredient.getTexture();
        info[4] += ingredient.getCalories();
    }
    if ( info[4] != 500 )
        return -1;
    int ret = 1;
    for ( int i = 0; i < 4; i++ ) {
        if ( info[i] < 0 )
            info[i] = 0;
        ret *= info[i];
    }
    return ret;
}

int main() {
    std::ifstream input_file( "input" );
    std::vector<Ingredient> ingredients{};
    getIngredients( ingredients, input_file );
    std::pair<std::vector<int>, int> best = { {}, 0 };
    for ( int i = 0; i <= 100; i++ ) { // hardcoding this is lame, but I'm lazy
        for ( int j = 0; j <= 100 - i; j++ ) {
            for ( int k = 0; k <= 100 - i - j; k++ ) {
                ingredients[0].setSpoons( i );
                ingredients[1].setSpoons( j );
                ingredients[2].setSpoons( k );
                ingredients[3].setSpoons( 100 - i - j - k );
                auto score = getCookieScore( ingredients );
                if ( score > best.second )
                    best = { { i, j, k, 100 - i - j - k }, score };
            }
        }
    }
    auto spoons = best.first;
    auto score = best.second;
    std::cout << "Best cookie recipe consists of " << spoons[0] << " spoons of "
              << ingredients[0].getName() << ", " << spoons[1] << " spoons of "
              << ingredients[1].getName() << ", " << spoons[2] << " spoons of "
              << ingredients[2].getName() << " and " << spoons[3]
              << " spoons of " << ingredients[3].getName()
              << ". This recipe produces a cookie with score \033[93;1m"
              << score << "\033[0m" << std::endl;

    best.first = {};
    best.second = 0;
    for ( int i = 0; i <= 100; i++ ) {
        for ( int j = 0; j <= 100 - i; j++ ) {
            for ( int k = 0; k <= 100 - i - j; k++ ) {
                ingredients[0].setSpoons( i );
                ingredients[1].setSpoons( j );
                ingredients[2].setSpoons( k );
                ingredients[3].setSpoons( 100 - i - j - k );
                auto score = getCookieScore500( ingredients );
                if ( score > best.second )
                    best = { { i, j, k, 100 - i - j - k }, score };
            }
        }
    }
    spoons = best.first;
    score = best.second;
    std::cout << "Best cookie recipe (which produces 500 calorie cookies) "
                 "consists of "
              << spoons[0] << " spoons of " << ingredients[0].getName() << ", "
              << spoons[1] << " spoons of " << ingredients[1].getName() << ", "
              << spoons[2] << " spoons of " << ingredients[2].getName()
              << " and " << spoons[3] << " spoons of "
              << ingredients[3].getName()
              << ". This recipe produces a cookie with score \033[93;1m"
              << score << "\033[0m" << std::endl;
}
