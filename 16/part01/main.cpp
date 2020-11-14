#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <vector>

struct Sue {
    int children = -1;
    int cats = -1;
    int samoyeds = -1;
    int pomeranians = -1;
    int akitas = -1;
    int vizslas = -1;
    int goldfish = -1;
    int trees = -1;
    int cars = -1;
    int perfumes = -1;
};

void addProperty( struct Sue &sue, const std::string &name, int value) {
    if(name == "children:") {
        sue.children = value;
    } else if( name == "cats:" ) {
        sue.cats = value;
    } else if( name == "samoyeds:" ) {
        sue.samoyeds = value;
    } else if( name == "pomeranians:" ) {
        sue.pomeranians = value;
    } else if( name == "akitas:" ) {
        sue.akitas = value;
    } else if( name == "vizslas:" ) {
        sue.vizslas = value;
    } else if( name == "goldfish:" ) {
        sue.goldfish = value;
    } else if( name == "trees:" ) {
        sue.trees = value;
    } else if( name == "cars:" ) {
        sue.cars = value;
    } else if( name == "perfumes:" ) {
        sue.perfumes = value;
    } else {
        std::cout << "UNKNOWN NAME: " << name << std::endl;
    }
}

void getSues( std::vector<Sue> &sues, std::ifstream &file ) {
    std::string str;
    std::string name;
    int index = -1;
    int property;
    while ( std::getline( file, str ) ) {
        index++;
        sues.emplace_back();
        std::stringstream ss( str );
        ss >> str;
        ss >> str;
        ss >> str; // what
        ss >> property;
        addProperty(sues[index], str, property);
        ss >> str; // comma
        ss >> str; // what
        ss >> property;
        addProperty(sues[index], str, property);
        ss >> str; // comma
        ss >> str; // what
        ss >> property;
        addProperty(sues[index], str, property);
    }
}

bool isSueValid(const struct Sue &comp_sue, const struct Sue &input_sue) {
    if(comp_sue.children != input_sue.children && comp_sue.children != -1)
        return false;
    if(comp_sue.cats != input_sue.cats && comp_sue.cats != -1)
        return false;
    if(comp_sue.samoyeds != input_sue.samoyeds && comp_sue.samoyeds != -1)
        return false;
    if(comp_sue.pomeranians != input_sue.pomeranians && comp_sue.pomeranians != -1)
        return false;
    if(comp_sue.akitas != input_sue.akitas && comp_sue.akitas != -1)
        return false;
    if(comp_sue.vizslas != input_sue.vizslas && comp_sue.vizslas != -1)
        return false;
    if(comp_sue.goldfish != input_sue.goldfish && comp_sue.goldfish != -1)
        return false;
    if(comp_sue.trees != input_sue.trees && comp_sue.trees != -1)
        return false;
    if(comp_sue.cars != input_sue.cars && comp_sue.cars != -1)
        return false;
    if(comp_sue.perfumes != input_sue.perfumes && comp_sue.perfumes != -1)
        return false;
    return true;
}

bool isSueValidV2(const struct Sue &comp_sue, const struct Sue &input_sue) {
    if(comp_sue.children != input_sue.children && comp_sue.children != -1)
        return false;
    if(comp_sue.cats <= input_sue.cats && comp_sue.cats != -1)
        return false;
    if(comp_sue.samoyeds != input_sue.samoyeds && comp_sue.samoyeds != -1)
        return false;
    if(comp_sue.pomeranians >= input_sue.pomeranians && comp_sue.pomeranians != -1)
        return false;
    if(comp_sue.akitas != input_sue.akitas && comp_sue.akitas != -1)
        return false;
    if(comp_sue.vizslas != input_sue.vizslas && comp_sue.vizslas != -1)
        return false;
    if(comp_sue.goldfish >= input_sue.goldfish && comp_sue.goldfish != -1)
        return false;
    if(comp_sue.trees <= input_sue.trees && comp_sue.trees != -1)
        return false;
    if(comp_sue.cars != input_sue.cars && comp_sue.cars != -1)
        return false;
    if(comp_sue.perfumes != input_sue.perfumes && comp_sue.perfumes != -1)
        return false;
    return true;
}

int main() {
    std::ifstream input_file( "input" );
    std::vector<Sue> sues{};
    getSues( sues, input_file );

    struct Sue inputSue;
    inputSue.children = 3;
    inputSue.cats = 7;
    inputSue.samoyeds = 2;
    inputSue.pomeranians = 3;
    inputSue.akitas = 0;
    inputSue.vizslas = 0;
    inputSue.goldfish = 5;
    inputSue.trees = 3;
    inputSue.cars = 2;
    inputSue.perfumes = 1;

    std::vector<int> valid{};
    for(size_t i = 0; i < sues.size(); i++) {
        if(isSueValid(sues[i], inputSue)) {
            valid.push_back(i+1);
        }
    }

    std::cout << "You should thank Aunt Sue #\033[93;1m"
              << valid[0] << "\033[0m" << std::endl;

    valid = {};
    for(size_t i = 0; i < sues.size(); i++) {
        if(isSueValidV2(sues[i], inputSue)) {
            valid.push_back(i+1);
        }
    }

    std::cout << "You should actually thank Aunt Sue #\033[93;1m"
              << valid[0] << "\033[0m" << std::endl;
}
