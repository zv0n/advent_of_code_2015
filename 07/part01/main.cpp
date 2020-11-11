#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>

enum WireActions {
    ASSIGN,
    AND,
    OR,
    NOT,
    LSHIFT,
    RSHIFT,
};

class Wire {
  public:
    Wire() {
        wire_a = nullptr;
        wire_b = nullptr;
        number_a = -1;
        number_b = -1;
        _name = "NULL";
    }
    Wire( WireActions action, Wire *operand_a, Wire *operand_b,
          std::string name ) {
        setVals( action, operand_a, operand_b );
        _name = name;
    }
    Wire( WireActions action, Wire *operand_a, uint16_t operand_b,
          std::string name ) {
        setVals( action, operand_a, operand_b );
        _name = name;
    }
    Wire( WireActions action, Wire *operand_a, std::string name ) {
        setVals( action, operand_a );
        _name = name;
    }
    Wire( WireActions action, uint16_t operand_a, std::string name ) {
        setVals( action, operand_a );
        _name = name;
    }

    void setVals( WireActions action, Wire *operand_a, Wire *operand_b ) {
        _action = action;
        wire_a = operand_a;
        wire_b = operand_b;
    }
    void setVals( WireActions action, Wire *operand_a, uint16_t operand_b ) {
        _action = action;
        wire_a = operand_a;
        number_b = operand_b;
    }
    void setVals( WireActions action, Wire *operand_a ) {
        _action = action;
        wire_a = operand_a;
    }
    void setVals( WireActions action, uint16_t operand_a ) {
        _action = action;
        number_a = operand_a;
    }

    void setVals( WireActions action, uint16_t operand_a, uint16_t operand_b ) {
        _action = action;
        number_a = operand_a;
        number_b = operand_b;
    }

    void setVals( WireActions action, uint16_t operand_a, Wire *operand_b ) {
        _action = action;
        number_a = operand_a;
        wire_b = operand_b;
    }

    uint16_t evaluate() {
        if ( found_result )
            return result;
        uint16_t ret;
        switch ( _action ) {
        case ASSIGN:
            if ( wire_a != nullptr ) {
                ret = wire_a->evaluate();
                result = ret;
                found_result = true;
                return ret;
            }
            result = number_a;
            found_result = true;
            return number_a;
        case AND:
            if ( wire_a == nullptr && wire_b == nullptr ) {
                ret = number_a & number_b;
            } else if ( wire_a == nullptr ) {
                ret = number_a & wire_b->evaluate();
            } else if ( wire_b == nullptr ) {
                ret = wire_a->evaluate() & number_b;
            } else {
                ret = wire_a->evaluate() & wire_b->evaluate();
            }
            result = ret;
            found_result = true;
            return ret;
        case OR:
            if ( wire_a == nullptr && wire_b == nullptr ) {
                ret = number_a | number_b;
            } else if ( wire_a == nullptr ) {
                ret = number_a | wire_b->evaluate();
            } else if ( wire_b == nullptr ) {
                ret = wire_a->evaluate() | number_b;
            } else {
                ret = wire_a->evaluate() | wire_b->evaluate();
            }
            result = ret;
            found_result = true;
            return ret;
        case NOT:
            if ( wire_a == nullptr ) {
                ret = ~number_a;
            } else {
                ret = ~wire_a->evaluate();
            }
            result = ret;
            found_result = true;
            return ret;
        case LSHIFT:
            if ( wire_b == nullptr ) {
                ret = wire_a->evaluate() << number_b;
            } else {
                ret = wire_a->evaluate() << wire_b->evaluate();
            }
            result = ret;
            found_result = true;
            return ret;
        case RSHIFT:
            if ( wire_b == nullptr ) {
                ret = wire_a->evaluate() >> number_b;
            } else {
                ret = wire_a->evaluate() >> wire_b->evaluate();
            }
            result = ret;
            found_result = true;
            return ret;
        }
        return -1;
    }

    void reset() {
        found_result = false;
        result = 0;
    }

    std::string _name;
    uint16_t result;
    bool found_result = false;

  private:
    WireActions _action;
    Wire *wire_a;
    Wire *wire_b;
    uint16_t number_a;
    uint16_t number_b;
};

void getWires( std::unordered_map<std::string, Wire> &wires,
               std::ifstream &file ) {
    std::string str;
    while ( std::getline( file, str ) ) {
        std::stringstream ss( str );
        ss >> str;
        std::string input_a;
        std::string input_b;
        std::string target;
        WireActions operand;
        if ( str == "NOT" ) {
            operand = NOT;
            ss >> input_a;
            ss >> str;
            ss >> target;
        } else {
            input_a = str;
            ss >> str;
            if ( str == "->" ) {
                operand = ASSIGN;
            }
            if ( str == "AND" ) {
                operand = AND;
            }
            if ( str == "OR" ) {
                operand = OR;
            }
            if ( str == "NOT" ) {
                operand = NOT;
            }
            if ( str == "LSHIFT" ) {
                operand = LSHIFT;
            }
            if ( str == "RSHIFT" ) {
                operand = RSHIFT;
            }
            if ( operand != ASSIGN ) {
                ss >> input_b;
                ss >> str;
                ss >> target;
            } else {
                ss >> target;
            }
        }

        switch ( operand ) {
        case ASSIGN:
            wires[target]._name = target;
            if ( std::isdigit( input_a[0] ) ) {
                wires[target].setVals( operand, std::stoi( input_a ) );
                break;
            }
            wires[target].setVals( operand, &wires[input_a] );
            break;
        case AND:
        case OR:
        case NOT:
        case LSHIFT:
        case RSHIFT:
            wires[target]._name = target;
            if ( std::isdigit( input_a[0] ) ) {
                if ( std::isdigit( input_b[0] ) ) {
                    wires[target].setVals( operand, std::stoi( input_a ),
                                           std::stoi( input_b ) );
                    break;
                } else {
                    wires[target].setVals( operand, std::stoi( input_a ),
                                           &wires[input_b] );
                    break;
                }
            } else if ( std::isdigit( input_b[0] ) ) {
                wires[target].setVals( operand, &wires[input_a],
                                       std::stoi( input_b ) );
                break;
            }
            wires[target].setVals( operand, &wires[input_a], &wires[input_b] );
            break;
        default:
            break;
        }
    }
}

void resetWires( std::unordered_map<std::string, Wire> &wires ) {
    for ( auto &entry : wires ) {
        entry.second.reset();
    }
}

int main() {
    std::ifstream input_file( "input" );
    std::unordered_map<std::string, Wire> wires;
    getWires( wires, input_file );
    auto a_res = wires["a"].evaluate();
    std::cout << "Wire 'a' has value \033[93;1m" << a_res << "\033[0m"
              << std::endl;
    resetWires( wires );
    wires["b"].found_result = true;
    wires["b"].result = a_res;
    std::cout << "After re-wiring 'a' has value \033[93;1m"
              << wires["a"].evaluate() << "\033[0m" << std::endl;
}
