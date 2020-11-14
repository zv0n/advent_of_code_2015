#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <vector>

void getContainers( std::vector<int> &containers, std::ifstream &file ) {
    std::string str;
    int capacity;
    while ( std::getline( file, str ) ) {
        std::stringstream ss( str );
        ss >> capacity;
        containers.push_back( capacity );
    }
}

int numberOfOnes( std::vector<short> &used ) {
    int ones = 0;
    for ( auto &i : used ) {
        if ( i == 1 )
            ones++;
    }
    return ones;
}

bool increaseUsed( std::vector<short> &used ) {
    size_t index = 0;
    for ( auto &i : used ) {
        index++;
        if ( i == 0 ) {
            i = 1;
            break;
        } else {
            i = 0;
        }
    }
    if ( numberOfOnes( used ) == 0 )
        return false;
    return true;
}

bool validContainers( std::vector<short> &used, std::vector<int> &containers,
                      int required ) {
    int capacity = 0;
    for ( size_t i = 0; i < used.size(); i++ ) {
        if ( used[i] == 1 )
            capacity += containers[i];
    }
    return capacity == required;
}

int main() {
    std::ifstream input_file( "input" );
    std::vector<int> containers{};
    getContainers( containers, input_file );
    std::vector<short> used;
    used.resize( containers.size(), 0 );

    int input_eggnog = 150;
    int valid_containers = 0;
    int min_amount = containers.size();

    while ( increaseUsed( used ) ) {
        if ( validContainers( used, containers, input_eggnog ) ) {
            valid_containers++;
            if ( numberOfOnes( used ) < min_amount )
                min_amount = numberOfOnes( used );
        }
    }

    std::cout << "There are \033[93;1m" << valid_containers
              << "\033[0m valid containers" << std::endl;

    valid_containers = 0;

    while ( increaseUsed( used ) ) {
        if ( validContainers( used, containers, input_eggnog ) &&
             numberOfOnes( used ) == min_amount ) {
            valid_containers++;
        }
    }

    std::cout << "There are \033[93;1m" << valid_containers
              << "\033[0m ways of filling up " << min_amount
              << " containers to achieve " << input_eggnog
              << " liters of eggnog" << std::endl;
}
