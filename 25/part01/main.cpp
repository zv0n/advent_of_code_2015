#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <vector>

std::pair<int,int> getCoordinates( std::ifstream &file ) {
    int row{};
    int col{};
    std::string str;
    while ( std::getline( file, str ) ) {
        str = str.substr(80);
        std::stringstream ss( str );
        ss >> row;
        ss >> str;
        ss >> str;
        ss >> col;
    }
    return {row, col};
}

size_t getCodePos(int row, int col) {
    size_t base_row = (row + col) - 1;
    size_t before = (base_row - 1) * base_row / 2;
    return before + col - 1; // - 1 because indexing from 0
}

size_t closestLog2(size_t input) {
    size_t ret = 0;
    while(input > 1) {
        input /= 2;
        ret++;
    }
    return ret;
}

size_t pow2(size_t input) {
    size_t ret = 1;
    for(size_t i = 0; i < input; i++) {
        ret *= 2;
    }
    return ret;
}

void fillRemainders(std::vector<size_t> &remainders, const size_t &divisor) {
    for(size_t i = 1; i < remainders.size(); i++)
        remainders[i] = (remainders[i-1] * remainders[i-1]) % divisor;
}

int main() {
    std::ifstream input_file( "input" );
    auto coords = getCoordinates(input_file);

    int row = coords.first;
    int column = coords.second;
    size_t multiplier = 252533;
    size_t divisor = 33554393;
    size_t base = 20151125;

    auto pos = getCodePos(row, column);
    auto max2 = closestLog2(pos);

    std::vector<size_t> remainders{};
    remainders.resize(max2+1);
    remainders[0] = multiplier;
    fillRemainders(remainders, divisor);

    size_t cur_multiplier = 1;
    while(pos != 0) {
        auto closest = closestLog2(pos);
        cur_multiplier *= remainders[closest];
        cur_multiplier %= divisor;
        pos -= pow2(closest);
    }

    std::cout << "Santa's code is \033[93;1m"
              << (cur_multiplier * base) % divisor << "\033[0m" << std::endl;
}
