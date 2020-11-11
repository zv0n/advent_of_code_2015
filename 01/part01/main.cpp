#include <fstream>
#include <iostream>

int getFloor( const std::string &input ) {
    int ret = 0;
    for ( auto &x : input ) {
        switch ( x ) {
        case '(':
            ret++;
            break;
        case ')':
            ret--;
        default:
            break;
        }
    }
    return ret;
}

int main() {
    std::ifstream input_file( "input" );
    std::string input;
    std::getline( input_file, input );
    std::cout << "Santa needs to go to floor number \033[93;1m"
              << getFloor( input ) << "\033[0m" << std::endl;
}
