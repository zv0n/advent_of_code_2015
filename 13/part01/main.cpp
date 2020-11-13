#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <vector>

void getGraph( std::map<std::string, std::map<std::string, int>> &graph,
               std::vector<std::string> &people, std::ifstream &file ) {
    std::string str;
    std::string source;
    std::string target;
    int happiness;
    int multiplier;
    std::map<std::string, int> people_map;
    while ( std::getline( file, str ) ) {
        std::stringstream ss( str );
        ss >> source;
        ss >> str; // would
        ss >> str;
        if ( str == "lose" )
            multiplier = -1;
        else
            multiplier = 1;
        ss >> happiness;
        ss >> str; // happiness
        ss >> str; // units
        ss >> str; // by
        ss >> str; // sitting
        ss >> str; // next
        ss >> str; // to
        ss >> target;
        target = target.substr( 0, target.length() - 1 );
        graph[source][target] = happiness * multiplier;
        people_map[source] = 0;
    }
    for ( auto &entry : people_map ) {
        people.push_back( entry.first );
    }
}

bool isThereUnseated( std::vector<bool> &seated ) {
    bool ret = false;
    for ( auto x : seated ) {
        ret |= !x;
    }
    return ret;
}

int findOptimal( size_t prev,
                 std::map<std::string, std::map<std::string, int>> &graph,
                 std::vector<bool> &seated, std::vector<std::string> &people,
                 std::function<bool( int, int )> cmpfunc, int initial_value,
                 int first_sitter ) {
    if ( !isThereUnseated( seated ) )
        return graph[people[prev]][people[first_sitter]] +
               graph[people[first_sitter]][people[prev]];
    int happiness = initial_value;
    for ( size_t i = 0; i < people.size(); i++ ) {
        if ( !seated[i] ) {
            seated[i] = true;
            auto tmp = findOptimal( i, graph, seated, people, cmpfunc,
                                    initial_value, first_sitter );
            tmp = graph[people[prev]][people[i]] +
                  graph[people[i]][people[prev]] + tmp;
            if ( cmpfunc( tmp, happiness ) )
                happiness = tmp;
            seated[i] = false;
        }
    }
    return happiness;
}

int findOptimal( std::map<std::string, std::map<std::string, int>> &graph,
                 std::vector<std::string> &people,
                 std::function<bool( int, int )> cmpfunc, int initial_value ) {
    std::vector<bool> seated;
    seated.resize( people.size(), false );
    int happiness = initial_value;

    seated[0] = true;
    auto tmp =
        findOptimal( 0, graph, seated, people, cmpfunc, initial_value, 0 );
    if ( cmpfunc( tmp, happiness ) )
        happiness = tmp;
    seated[0] = false;

    return happiness;
}

bool lowerThan( int a, int b ) {
    return a < b;
}

bool higherThan( int a, int b ) {
    return a > b;
}

void addYourselfToTheGraph(
    std::map<std::string, std::map<std::string, int>> &graph,
    std::vector<std::string> &people ) {
    for ( auto &person : people ) {
        graph[person]["you"] = 0;
        graph["you"][person] = 0;
    }
    people.push_back( "you" );
}

int main() {
    std::ifstream input_file( "input" );
    std::map<std::string, std::map<std::string, int>> graph;
    std::vector<std::string> people;
    getGraph( graph, people, input_file );
    std::cout << "Optimal seating has \033[93;1m"
              << findOptimal( graph, people, higherThan,
                              std::numeric_limits<int>::min() )
              << "\033[0m happiness" << std::endl;
    addYourselfToTheGraph( graph, people );
    std::cout << "Optimal seating (with you included) has \033[93;1m"
              << findOptimal( graph, people, higherThan,
                              std::numeric_limits<int>::min() )
              << "\033[0m happiness" << std::endl;
}
