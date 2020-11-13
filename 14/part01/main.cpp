#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <vector>

class Reindeer {
  public:
    Reindeer() = delete;
    Reindeer( const std::string &name, int speed, int endurance, int rest ) {
        _speed = speed;
        _endurance = endurance;
        _rest = rest;
        _name = name;
    }

    int getPossibleDistance( int seconds ) {
        int full_loops = seconds / ( _endurance + _rest );
        int remainder = seconds % ( _endurance + _rest );
        int last_sprint = remainder < _endurance ? remainder : _endurance;
        return ( full_loops * _endurance + last_sprint ) * _speed;
    }

    void reset() {
        _cur_distance = 0;
        _cur_endurance = 0;
        _cur_rest = 0;
        _points = 0;
    }

    void moveOneSecond() {
        if ( _cur_endurance < _endurance ) {
            _cur_distance += _speed;
            _cur_endurance++;
            if ( _cur_endurance == _endurance )
                _cur_rest = 0;
        } else {
            _cur_rest++;
            if ( _cur_rest == _rest )
                _cur_endurance = 0;
        }
    }

    void addPoint() {
        _points++;
    }

    std::string getName() {
        return _name;
    }

    int getDistance() {
        return _cur_distance;
    }

    int getPoints() {
        return _points;
    }

  private:
    int _speed;
    int _endurance;
    int _rest;
    std::string _name;

    int _cur_distance = 0;
    int _cur_endurance = 0;
    int _cur_rest = 0;
    int _points = 0;
};

void getReindeer( std::vector<Reindeer> &reindeers, std::ifstream &file ) {
    std::string str;
    std::string name;
    int speed;
    int endurance;
    int rest;
    while ( std::getline( file, str ) ) {
        std::stringstream ss( str );
        ss >> name;
        for ( int i = 0; i < 2; i++ ) // can fly
            ss >> str;
        ss >> speed;
        for ( int i = 0; i < 2; i++ ) // km/s for
            ss >> str;
        ss >> endurance;
        for ( int i = 0; i < 6; i++ ) // seconds, but then must rest for
            ss >> str;
        ss >> rest;
        reindeers.emplace_back( name, speed, endurance, rest );
    }
}

int main() {
    std::ifstream input_file( "input" );
    int input_seconds = 2503;
    std::vector<Reindeer> reindeers{};
    getReindeer( reindeers, input_file );
    std::pair<std::string, int> best = { "NULL", 0 };
    for ( auto &reindeer : reindeers ) {
        auto distance = reindeer.getPossibleDistance( input_seconds );
        if ( distance > best.second )
            best = { reindeer.getName(), distance };
    }
    std::cout << "Santa's best reindeer is " << best.first
              << ", who can travel \033[93;1m" << best.second
              << "\033[0m km in " << input_seconds << " seconds" << std::endl;
    for ( auto &reindeer : reindeers ) {
        reindeer.reset();
    }
    for ( int i = 0; i < input_seconds; i++ ) {
        int longest_distance = 0;
        for ( auto &reindeer : reindeers ) {
            reindeer.moveOneSecond();
            if ( reindeer.getDistance() > longest_distance )
                longest_distance = reindeer.getDistance();
        }
        for ( auto &reindeer : reindeers ) {
            if ( reindeer.getDistance() == longest_distance )
                reindeer.addPoint();
        }
    }
    std::pair<std::string, int> best2 = { "NULL", 0 };
    for ( auto &reindeer : reindeers ) {
        if ( reindeer.getPoints() > best2.second )
            best2 = { reindeer.getName(), reindeer.getPoints() };
    }
    std::cout << "Santa's best reindeer (according to new criteria) is "
              << best2.first << ", who has \033[93;1m" << best2.second
              << "\033[0m points after " << input_seconds << " seconds"
              << std::endl;
}
