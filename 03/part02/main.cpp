#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

void visitHomes(
    std::unordered_map<int, std::unordered_map<int, int>> &visited_houses,
    std::ifstream &file ) {
    std::pair<int, int> position_a = { 0, 0 };
    std::pair<int, int> position_b = { 0, 0 };
    visited_houses[position_a.first][position_b.second] += 1;
    std::pair<int, int> *position = &position_b;
    std::string input;
    while ( std::getline( file, input ) ) {
        for ( auto &c : input ) {
            if ( position == &position_b )
                position = &position_a;
            else
                position = &position_b;
            switch ( c ) {
            case '^':
                position->second += 1;
                break;
            case 'v':
                position->second -= 1;
                break;
            case '<':
                position->first -= 1;
                break;
            case '>':
                position->first += 1;
            default:
                break;
            }
            visited_houses[position->first][position->second] += 1;
        }
    }
}

size_t getHousesWithPresents(
    std::unordered_map<int, std::unordered_map<int, int>> &visited_houses ) {
    int ret = 0;
    for ( auto &entry : visited_houses ) {
        ret += entry.second.size();
    }
    return ret;
}

int main() {
    std::ifstream input_file( "input" );
    std::unordered_map<int, std::unordered_map<int, int>> visited_houses;
    visitHomes( visited_houses, input_file );
    std::cout << "Santa delivers at least 1 present to \033[93;1m"
              << getHousesWithPresents( visited_houses ) << "\033[0m homes"
              << std::endl;
}
