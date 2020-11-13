#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

std::string lookAndSay( const std::string &input ) {
    std::string ret = "";
    char prev = input[0];
    int prev_count = 0;
    for ( const auto &c : input ) {
        if ( c == prev ) {
            prev_count++;
        } else {
            ret += std::to_string( prev_count );
            ret += prev;
            prev = c;
            prev_count = 1;
        }
    }
    ret += std::to_string( prev_count );
    ret += prev;
    return ret;
}

int main() {
    std::string input = "1113122113";
    std::string tmp = input;
    for ( int i = 0; i < 40; i++ ) {
        tmp = lookAndSay( tmp );
    }
    std::cout << "Look and say produced output of length \033[93;1m"
              << tmp.length() << "\033[0m after 40 iterations" << std::endl;
    for ( int i = 0; i < 10; i++ ) {
        tmp = lookAndSay( tmp );
    }
    std::cout << "Look and say produced output of length \033[93;1m"
              << tmp.length() << "\033[0m after 50 iterations" << std::endl;
}
