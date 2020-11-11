#include <fstream>
#include <iostream>

int firstBasement( const std::string &input ) {
    int floor = 0;
    int pos = 0;
    for ( auto &x : input ) {
        pos++;
        switch ( x ) {
        case '(':
            floor++;
            break;
        case ')':
            floor--;
        default:
            break;
        }
        if ( floor < 0 )
            return pos;
    }
    return -1;
}

int main() {
    std::ifstream input_file( "input" );
    std::string input;
    std::getline( input_file, input );
    std::cout << "Santa will enter basement at instruction \033[93;1m"
              << firstBasement( input ) << "\033[0m" << std::endl;
}
