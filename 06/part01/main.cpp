#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

#define TOGGLE_FLAG 0x01
#define ON_FLAG 0x02
#define OFF_FLAG 0x04

void parseString( std::string &str, int &flags, int &startx, int &starty,
                  int &endx, int &endy ) {
    std::stringstream ss( str );
    char tmp;
    ss >> str;
    if ( str == "toggle" ) {
        flags = TOGGLE_FLAG;
    } else {
        ss >> str;
        if ( str == "on" )
            flags = ON_FLAG;
        else
            flags = OFF_FLAG;
    }
    ss >> startx;
    ss >> tmp;
    ss >> starty;
    ss >> str;
    ss >> endx;
    ss >> tmp;
    ss >> endy;
    if ( startx > endx ) {
        startx += endx;
        endx = startx - endx;
        startx -= endx;
    }
    if ( starty > endy ) {
        starty += endy;
        endy = starty - endy;
        starty -= endy;
    }
}

bool onFlagFunc( bool /*UNUSED*/ ) {
    return true;
}

bool offFlagFunc( bool /*UNUSED*/ ) {
    return false;
}

bool toggleFlagFunc( bool input ) {
    return !input;
}

void lightLights( std::vector<std::vector<bool>> &lights,
                  std::ifstream &file ) {
    std::string str;
    while ( std::getline( file, str ) ) {
        int flags;
        int startx, starty, endx, endy;
        parseString( str, flags, startx, starty, endx, endy );
        std::function<bool( bool )> operation = nullptr;
        switch ( flags ) {
        case TOGGLE_FLAG:
            operation = toggleFlagFunc;
            break;
        case ON_FLAG:
            operation = onFlagFunc;
            break;
        case OFF_FLAG:
            operation = offFlagFunc;
        default:
            break;
        }
        for ( int i = startx; i <= endx; i++ ) {
            for ( int j = starty; j <= endy; j++ ) {
                lights[i][j] = operation( lights[i][j] );
            }
        }
    }
}

size_t numberOfLights( std::vector<std::vector<bool>> &lights ) {
    size_t ret = 0;
    for ( auto &row : lights ) {
        for ( auto light : row ) {
            if ( light )
                ret++;
        }
    }
    return ret;
}

int main() {
    std::ifstream input_file( "input" );
    std::vector<std::vector<bool>> lights{};
    lights.resize( 1000 );
    for ( auto &x : lights ) {
        x.resize( 1000, false );
    }
    lightLights( lights, input_file );
    std::cout << "You've lit up \033[93;1m" << numberOfLights( lights )
              << "\033[0m lights" << std::endl;
}
