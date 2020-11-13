#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

long long getSum( const std::string &input ) {
    long long ret = 0;
    std::string cur_number{};
    char prev = 0;
    for ( auto &c : input ) {
        if ( std::isdigit( c ) || ( c == '-' && !std::isdigit( prev ) ) ) {
            if ( std::isdigit( prev ) || prev == '-' )
                cur_number += c;
            else
                cur_number = c;
        } else if ( std::isdigit( prev ) ) {
            ret += std::stoi( cur_number );
        }
        prev = c;
    }
    return ret;
}

class Redless {
  public:
    Redless() = delete;
    Redless( size_t _parent ) {
        sum = 0;
        parent = _parent;
        red = false;
    }
    long long sum;
    size_t parent;
    bool red;
};

long long getSumRedless( const std::string &input ) {
    std::vector<Redless> redless{};
    std::string cur_number{};
    char prev = 0;
    size_t current = 0;
    bool string = false;
    int red_count = 0;
    int string_len = 0;
    bool string_val = false;
    redless.emplace_back( 0 );
    for ( auto &c : input ) {
        if ( string )
            string_len++;
        if ( c == '"' ) {
            string = !string;
            if ( string ) {
                if ( prev == ':' )
                    string_val = true;
                string_len = 0;
                red_count = 0;
            }
            if ( !string ) {
                if ( string_len == 4 && red_count == 3 && string_val ) {
                    redless[current].red = true;
                    redless[current].sum = 0;
                }
                string_val = false;
            }
        }
        if ( string && c == 'r' ) {
            if ( red_count == 0 && string_len == 1 ) {
                red_count = 1;
            } else {
                red_count = 0;
            }
        }
        if ( string && c == 'e' ) {
            if ( red_count == 1 && string_len == 2 ) {
                red_count = 2;
            } else {
                red_count = 0;
            }
        }
        if ( string && c == 'd' ) {
            if ( red_count == 2 && string_len == 3 ) {
                red_count = 3;
            } else {
                red_count = 0;
            }
        }
        if ( std::isdigit( c ) || ( c == '-' && !std::isdigit( prev ) ) ) {
            if ( std::isdigit( prev ) || prev == '-' )
                cur_number += c;
            else
                cur_number = c;
        } else if ( std::isdigit( prev ) ) {
            if ( !redless[current].red )
                redless[current].sum += std::stoi( cur_number );
            cur_number = "";
        }
        if ( c == '{' ) {
            redless.emplace_back( current );
            current = redless.size() - 1;
        }
        if ( c == '}' ) {
            if ( !redless[redless[current].parent].red )
                redless[redless[current].parent].sum += redless[current].sum;
            current = redless[current].parent;
        }
        prev = c;
    }
    return redless[0].sum;
}

int main() {
    std::ifstream input_file( "input" );
    std::string json{};
    std::getline( input_file, json );
    std::cout << "PART 1: Accountants have calculated \033[93;1m"
              << getSum( json ) << "\033[0m" << std::endl;
    std::cout << "PART 2: Accountants have calculated \033[93;1m"
              << getSumRedless( json ) << "\033[0m in red-less parts"
              << std::endl;
}
