#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <unordered_map>
#include <vector>

void getMolecules(
    std::unordered_map<std::string, std::vector<std::string>> &molecules,
    std::string &input_molecule, std::ifstream &file ) {
    std::string str;
    std::string molecule;
    std::string transformation;
    bool nextismol = false;
    while ( std::getline( file, str ) ) {
        if ( str == "" ) {
            nextismol = true;
            continue;
        }
        if ( nextismol ) {
            input_molecule = str;
            return;
        }
        std::stringstream ss( str );
        ss >> molecule;
        ss >> str;
        ss >> transformation;
        molecules[molecule].push_back( transformation );
    }
}

size_t molLength( const std::string molecule ) {
    size_t ret = 0;
    for ( auto &c : molecule ) {
        if ( c >= 65 && c <= 90 )
            ret++;
    }
    return ret;
}

std::vector<std::pair<std::string, std::string>>
reverseMapping( const std::unordered_map<std::string, std::vector<std::string>>
                    &molecules ) {
    std::vector<std::pair<std::string, std::string>> ret;
    size_t longest = 0;
    for ( auto &x : molecules ) {
        for ( auto &y : x.second ) {
            if ( molLength( y ) > longest )
                longest = molLength( y );
        }
    }
    // inefficient, I don't care
    while ( longest > 0 ) {
        for ( auto &mapping : molecules ) {
            for ( auto &mol : mapping.second ) {
                if ( molLength( mol ) == longest ) {
                    ret.emplace_back( mol, mapping.first );
                }
            }
        }
        longest--;
    }
    return ret;
}

std::string nextMolecule( const std::string &molecule, size_t pos ) {
    std::string ret = "";
    ret += molecule[pos];
    pos++;
    while ( molecule[pos] >= 'a' && molecule[pos] <= 'z' ) {
        ret += molecule[pos];
        pos++;
    }
    return ret;
}

size_t
possibilities( const std::unordered_map<std::string, std::vector<std::string>>
                   &molecule_map,
               const std::string &input_molecule ) {
    size_t pos = 0;
    std::unordered_map<std::string, int> found_molecules;
    while ( pos < input_molecule.length() ) {
        auto next = nextMolecule( input_molecule, pos );
        auto og_pos = pos;
        pos += next.length();
        if ( molecule_map.find( next ) == molecule_map.end() )
            continue;
        const auto &vec = molecule_map.at( next );
        for ( const auto &transformation : vec ) {
            std::string newmol = input_molecule;
            newmol.replace( og_pos, next.length(), transformation );
            found_molecules[newmol] = 1;
        }
    }
    return found_molecules.size();
}

int shortestWay(
    const std::vector<std::pair<std::string, std::string>> &mappings,
    std::string molecule ) {
    int ret = 0;
    while ( molecule != "e" ) {
        for ( auto &mapping : mappings ) {
            size_t pos = 0;
            if ( ( pos = molecule.find( mapping.first ) ) !=
                 std::string::npos ) {
                molecule.replace( pos, mapping.first.length(), mapping.second );
                ret++;
                // start searching again after molecule change
                break;
            }
        }
    }
    return ret;
}

int main() {
    std::ifstream input_file( "input" );
    std::unordered_map<std::string, std::vector<std::string>> molecules;
    std::string molecule;
    getMolecules( molecules, molecule, input_file );

    std::cout << "There are \033[93;1m" << possibilities( molecules, molecule )
              << "\033[0m possible new molecules" << std::endl;
    // we assume that no 2 molecules create the same result for this to work
    auto reverse = reverseMapping( molecules );
    std::cout << "The shortest way to get the cure is \033[93;1m"
              << shortestWay( reverse, molecule ) << "\033[0m transformations"
              << std::endl;
}
