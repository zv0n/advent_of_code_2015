#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

void getStrings( std::vector<std::string> &strings, std::ifstream &file ) {
    std::string str;
    while ( std::getline( file, str ) ) {
        strings.push_back( str );
    }
}

size_t getTotalLength( const std::vector<std::string> &strings ) {
    size_t ret{};
    for ( auto &string : strings ) {
        ret += string.length();
    }
    return ret;
}

size_t getEscapedLength( const std::vector<std::string> &strings ) {
    size_t ret{};
    for ( auto &string : strings ) {
        for ( size_t i = 1; i < string.length() - 1; i++ ) {
            if ( string[i] == '\\' ) {
                switch ( string[i + 1] ) {
                case '\\':
                    i += 1;
                    break;
                case '"':
                    i += 1;
                    break;
                case 'x':
                    i += 3;
                default:
                    break;
                }
            }
            ret += 1;
        }
    }
    return ret;
}

size_t getReencodedLength( const std::vector<std::string> &strings ) {
    size_t ret{};
    for ( auto &string : strings ) {
        ret += 2; // initial and closing "
        for ( auto &c : string ) {
            switch ( c ) {
            case '\\':
                ret += 1;
                break;
            case '"':
                ret += 1;
            default:
                break;
            }
            ret += 1;
        }
    }
    return ret;
}

int main() {
    std::ifstream input_file( "input" );
    std::vector<std::string> strings;
    getStrings( strings, input_file );
    auto total = getTotalLength( strings );
    auto escaped = getEscapedLength( strings );
    auto reencoded = getReencodedLength( strings );
    std::cout << "Total length - length in memory = \033[93;1m"
              << total - escaped << "\033[0m" << std::endl;
    std::cout << "Re-encoded length - total length = \033[93;1m"
              << reencoded - total << "\033[0m" << std::endl;
}
