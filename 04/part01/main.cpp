#include <fstream>
#include <iomanip>
#include <iostream>
#include <openssl/md5.h>
#include <sstream>
#include <unordered_map>
#include <vector>

std::string md5( const std::string &input ) {
    MD5_CTX c;
    MD5_Init( &c );
    MD5_Update( &c, input.c_str(), input.length() );
    unsigned char output[16];
    MD5_Final( output, &c );
    std::stringstream ss;
    for ( int i = 0; i < 16; i++ ) {
        ss << std::hex << std::setw( 2 ) << std::setfill( '0' )
           << (int)output[i];
    }
    return ss.str();
}

int findMiningKey( const std::string &input, int zero_count ) {
    size_t number = 0;
    while ( md5( input + std::to_string( number ) ).substr( 0, zero_count ) !=
            std::string( zero_count, '0' ) ) {
        number++;
    }
    return number;
}

int main() {
    std::string input = "yzbqklnj";
    std::cout << "PART1: Santa needs the following mining key: \033[93;1m"
              << findMiningKey( input, 5 ) << "\033[0m" << std::endl;
    std::cout << "PART2: Santa needs the following mining key: \033[93;1m"
              << findMiningKey( input, 6 ) << "\033[0m" << std::endl;
}
