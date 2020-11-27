#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <vector>

std::vector<int> getWeights( std::ifstream &file ) {
    std::vector<int> ret;
    std::string str;
    int weight;
    while ( std::getline( file, str ) ) {
        std::stringstream ss( str );
        ss >> weight;
        ret.push_back( weight );
    }
    std::reverse( ret.begin(), ret.end() );
    return ret;
}

void findBest( const std::vector<int> &weights, size_t target,
               std::vector<std::vector<int>> &best ) {
    size_t min_best = -1;
    std::vector<bool> used( weights.size(), false );

    size_t next = 1;
    std::vector<int> cur = { 0 };
    size_t cur_sum = weights[0];
    while ( cur.size() != 0 || next != weights.size() - 1 ) {
        for ( size_t i = next; i < weights.size(); i++ ) {
            if ( weights[i] + cur_sum <= target ) {
                cur_sum += weights[i];
                cur.push_back( i );
            }
            if ( cur_sum == target )
                break;
            if ( cur.size() > min_best )
                break;
        }
        if ( cur_sum == target ) {
            if ( cur.size() < min_best ) {
                best.clear();
                min_best = cur.size();
                best.push_back( cur );
            } else if ( cur.size() == min_best ) {
                best.push_back( cur );
            }
        }
        cur_sum -= weights[cur.back()];
        cur.pop_back();
        cur_sum -= weights[cur.back()];
        next = cur.back() + 1;
        cur.pop_back();
    }
}

size_t computeQuantum( const std::vector<int> &weights,
                       const std::vector<int> &input ) {
    size_t ret = 1;
    for ( auto &index : input )
        ret *= weights[index];
    return ret;
}

size_t bestQuantum( const std::vector<int> &weights,
                    const std::vector<std::vector<int>> &best ) {
    size_t ret = 0;
    for ( auto &config : best ) {
        size_t tmp = computeQuantum( weights, config );
        if ( tmp < ret || ret == 0 )
            ret = tmp;
    }
    return ret;
}

size_t getTargetWeight( std::vector<int> &weights, int divisor = 3 ) {
    size_t sum = 0;
    for ( auto &weight : weights )
        sum += weight;
    return sum / divisor;
}

int main() {
    std::ifstream input_file( "input" );
    auto weights = getWeights( input_file );
    std::vector<std::vector<int>> best{};

    auto target = getTargetWeight( weights );
    findBest( weights, target, best );
    std::cout << "Best configuration has QE of \033[93;1m"
              << bestQuantum( weights, best ) << "\033[0m" << std::endl;

    //-------------PART 2------------------
    best.clear();
    target = getTargetWeight( weights, 4 );
    findBest( weights, target, best );
    std::cout << "Best configuration (trunk included) has QE of \033[93;1m"
              << bestQuantum( weights, best ) << "\033[0m" << std::endl;
}
