#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <vector>

size_t getHouseNum(size_t input, size_t top, size_t multiplier) {
    std::vector<size_t> houses;
    houses.resize(top);
    for(size_t i = 1; i < top; i++) {
        for(size_t j = i; j < top; j += i) {
            houses[j] += i * multiplier;
        }
    }
    for(size_t i = 0; i < top; i++) {
        if(houses[i] >= input)
            return i;
    }
    return top;
}

size_t getHouseNum50(size_t input, size_t top, size_t multiplier) {
    std::vector<size_t> houses;
    houses.resize(top);
    for(size_t i = 1; i < top; i++) {
        size_t visits = 0;
        for(size_t j = i; j < top; j += i) {
            visits++;
            houses[j] += i * multiplier;
            if(visits >= 50)
                break;
        }
    }
    for(size_t i = 0; i < top; i++) {
        if(houses[i] >= input)
            return i;
    }
    return top;
}

int main() {
    size_t input = 34000000;

    std::cout << "The lowest house number to get " << input << " presents is \033[93;1m" << getHouseNum(input, input/10, 10)
              << "\033[0m" << std::endl;
    std::cout << "The lowest house number to get " << input << " presents (without infinite visits) is \033[93;1m" << getHouseNum50(input, input/11, 11)
              << "\033[0m" << std::endl;
}
