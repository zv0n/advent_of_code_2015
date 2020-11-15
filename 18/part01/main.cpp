#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <vector>

void getLights( std::vector<std::vector<bool>> &lights, std::ifstream &file ) {
    std::string str;
    while ( std::getline( file, str ) ) {
        std::vector<bool> light_row;
        for(auto &c : str) {
            switch(c) {
                case '#':
                    light_row.push_back(true);
                    break;
                case '.':
                    light_row.push_back(false);
                default:
                    break;
            }
        }
        lights.push_back(light_row);
    }
}

int lightsAround(size_t row, size_t col, std::vector<std::vector<bool>> &lights) {
    int lights_around = 0;
    for(size_t i = (row >= 1 ? row - 1 : row); i < (row < lights.size() - 1 ? row + 2 : row + 1); i++) {
        for(size_t j = (col >= 1 ? col - 1 : col); j < (col < lights[i].size() - 1 ? col + 2 : col + 1); j++) {
            if(i == row && j == col)
                continue;
            if(lights[i][j])
                lights_around += 1;
        }
    }
    return lights_around;
}

bool shouldBeOn(size_t row, size_t col, std::vector<std::vector<bool>> &lights) {
    auto lights_around = lightsAround(row, col, lights);
    return lights_around == 3 || (lights[row][col] && (lights_around == 2));
}

void life(std::vector<std::vector<bool>> &lights, int iterations, bool stuck_corners) {
    for(int i = 0; i < iterations; i++) {
        std::vector<std::vector<bool>> new_state{};
        for(size_t i = 0; i < lights.size(); i++) {
            new_state.push_back({});
            for(size_t j = 0; j < lights[i].size(); j++) {
                new_state[i].push_back(shouldBeOn(i, j, lights));
            }
        }
        if(stuck_corners) {
            new_state[0][0] = true;
            new_state[lights.size()-1][0] = true;
            new_state[0][lights[0].size()-1] = true;
            new_state[lights.size()-1][lights[lights.size()-1].size()-1] = true;
        }
        lights = std::move(new_state);
    }
}

size_t getTurnedOnLights(const std::vector<std::vector<bool>> &lights) {
    size_t ret = 0;
    for(const auto &light_row : lights) {
        for(auto light : light_row) {
            if(light)
                ret++;
        }
    }
    return ret;
}

int main() {
    std::ifstream input_file( "input" );
    std::vector<std::vector<bool>> input_lights{};
    getLights( input_lights, input_file );
    int input_steps = 100;

    auto lights = input_lights;
    life(lights, input_steps, false);

    std::cout << "There are \033[93;1m" << getTurnedOnLights(lights)
              << "\033[0m turned on lights" << std::endl;

    lights = input_lights;
    life(lights, input_steps, true);

    std::cout << "There are \033[93;1m" << getTurnedOnLights(lights)
              << "\033[0m turned on lights with 4 stuck corners" << std::endl;
}
