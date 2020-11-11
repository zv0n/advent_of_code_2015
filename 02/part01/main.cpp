#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

class Box {
  public:
    Box( int x, int y, int z ) {
        _x = x;
        _y = y;
        _z = z;
    }

    int getSurface() {
        return 2 * _x * _y + 2 * _y * _z + 2 * _x * _z;
    }

    int smallestSide() {
        int side_a = _x * _y;
        int side_b = _y * _z;
        int side_c = _x * _z;
        if ( side_a < side_b ) {
            if ( side_a < side_c )
                return side_a;
            return side_c;
        } else {
            if ( side_b < side_c )
                return side_b;
            return side_c;
        }
    }

  private:
    int _x;
    int _y;
    int _z;
};

void getBoxes( std::vector<Box> &boxes, std::ifstream &file ) {
    std::string input;
    while ( std::getline( file, input ) ) {
        std::stringstream ss( input );
        int x, y, z;
        char tmp;
        ss >> x;
        ss >> tmp;
        ss >> y;
        ss >> tmp;
        ss >> z;
        boxes.emplace_back( x, y, z );
    }
}

size_t getWrappingPaper( std::vector<Box> &boxes ) {
    size_t ret = 0;
    for ( auto &box : boxes ) {
        ret += box.getSurface() + box.smallestSide();
    }
    return ret;
}

int main() {
    std::ifstream input_file( "input" );
    std::vector<Box> boxes;
    getBoxes( boxes, input_file );
    std::cout << "Elves need to get \033[93;1m" << getWrappingPaper( boxes )
              << "\033[0m square feet of wrapping paper" << std::endl;
}
