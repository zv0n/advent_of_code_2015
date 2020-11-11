#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

bool niceReq1( const std::string &input ) {
    int count = 0;
    for ( auto &c : input ) {
        if ( c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' )
            count++;
        if ( count >= 3 )
            break;
    }
    return count >= 3;
}

bool niceReq2( const std::string &input ) {
    char prev = 0;
    for ( auto &c : input ) {
        if ( c == prev )
            return true;
        prev = c;
    }
    return false;
}

bool naughtyReq1( const std::string &input ) {
    bool ret = false;
    ret |= input.find( "ab" ) != std::string::npos;
    ret |= input.find( "cd" ) != std::string::npos;
    ret |= input.find( "pq" ) != std::string::npos;
    ret |= input.find( "xy" ) != std::string::npos;
    return ret;
}

bool niceReq3( const std::string &input ) {
    for ( size_t pos = 0; pos < input.length() - 2; pos++ ) {
        auto search = input.substr( pos, 2 );
        if ( input.find( search, pos + 2 ) != std::string::npos )
            return true;
    }
    return false;
}

bool niceReq4( const std::string &input ) {
    char back = 0;
    char middle = 0;
    for ( auto &c : input ) {
        if ( c == back )
            return true;
        back = middle;
        middle = c;
    }
    return false;
}

size_t niceLines(
    const std::vector<std::string> &strings,
    std::vector<std::function<bool( const std::string & )>> &nice_req,
    std::vector<std::function<bool( const std::string & )>> &naughty_req ) {
    size_t ret = 0;
    for ( const auto &string : strings ) {
        size_t nice = 0;
        size_t naughty = 0;
        for ( auto &func : nice_req ) {
            if ( func( string ) )
                nice++;
        }
        for ( auto &func : naughty_req ) {
            if ( func( string ) )
                naughty++;
        }
        if ( nice == nice_req.size() && naughty == 0 )
            ret++;
    }
    return ret;
}

std::vector<std::string> getStrings( std::ifstream &input ) {
    std::vector<std::string> ret{};
    std::string str{};
    while ( std::getline( input, str ) ) {
        ret.push_back( str );
    }
    return ret;
}

int main() {
    std::ifstream input_file( "input" );
    auto strings = getStrings( input_file );
    std::vector<std::function<bool( const std::string & )>> nice_req = {
        niceReq1, niceReq2 };
    std::vector<std::function<bool( const std::string & )>> naughty_req = {
        naughtyReq1 };
    std::cout << "PART1: Santa has \033[93;1m"
              << niceLines( strings, nice_req, naughty_req )
              << "\033[0m nice lines" << std::endl;
    std::vector<std::function<bool( const std::string & )>> nice_req2 = {
        niceReq3, niceReq4 };
    std::vector<std::function<bool( const std::string & )>> naughty_req2 = {};
    std::cout << "PART2: Santa has \033[93;1m"
              << niceLines( strings, nice_req2, naughty_req2 )
              << "\033[0m nice lines" << std::endl;
}
