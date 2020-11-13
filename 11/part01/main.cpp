#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

bool increasingStraight( const std::string &input ) {
    size_t straight_len = 0;
    char prev = 0;
    for ( auto &c : input ) {
        if ( c == prev + 1 )
            straight_len++;
        else
            straight_len = 1;
        if ( straight_len == 3 )
            return true;
        prev = c;
    }
    return false;
}

bool confusingLetters( const std::string &input ) {
    for ( auto &c : input ) {
        if ( c == 'i' || c == 'o' || c == 'l' )
            return false;
    }
    return true;
}

bool twoPair( const std::string &input ) {
    size_t pair_count = 0;
    for ( size_t i = 1; i < input.length(); i++ ) {
        if ( input[i] == input[i - 1] ) {
            pair_count++;
            i++;
        }
    }
    return pair_count >= 2;
}

bool passwordValid(
    const std::string &password,
    std::vector<std::function<bool( const std::string & )>> &requirements ) {
    bool ret = true;
    for ( auto &func : requirements ) {
        ret &= func( password );
    }
    return ret;
}

void increasePassword( std::string &prev_pass ) {
    size_t index = prev_pass.length() - 1;
    while ( prev_pass[index] == 'z' && index != (size_t)-1 ) {
        prev_pass[index] = 'a';
        index--;
    }
    if ( index == (size_t)-1 )
        return;
    prev_pass[index]++;
}

int main() {
    std::string input = "hepxcrrq";
    std::vector<std::function<bool( const std::string & )>> requirements = {
        increasingStraight, confusingLetters, twoPair };
    do {
        increasePassword( input );
    } while ( !passwordValid( input, requirements ) );
    std::cout << "Santa's password should be \033[93;1m" << input << "\033[0m"
              << std::endl;
    do {
        increasePassword( input );
    } while ( !passwordValid( input, requirements ) );
    std::cout << "Santa's password after that should be \033[93;1m" << input
              << "\033[0m" << std::endl;
}
