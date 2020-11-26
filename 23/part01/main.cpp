#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <vector>

enum AssemblyInstruction {
    half,
    triple,
    increase,
    jump,
    jump_even,
    jump_one,
};

AssemblyInstruction strToInst( const std::string &inst ) {
    if ( inst == "hlf" )
        return half;
    if ( inst == "tpl" )
        return triple;
    if ( inst == "inc" )
        return increase;
    if ( inst == "jmp" )
        return jump;
    if ( inst == "jie" )
        return jump_even;
    if ( inst == "jio" )
        return jump_one;
    throw "Invalid instruction! " + inst;
}

class Instruction {
  public:
    Instruction( const std::string &inst, const std::string &reg,
                 const std::string &offset ) {
        _inst = strToInst( inst );
        if ( reg.empty() ) {
            _reg = 0;
        } else {
            if ( reg[0] == 'a' )
                _reg = 0;
            else
                _reg = 1;
        }
        if ( !offset.empty() )
            _offset = std::stoi( offset );
        else
            _offset = 1;
    }

    AssemblyInstruction getIns() const {
        return _inst;
    }

    int getReg() const {
        return _reg;
    }

    int getOffset() const {
        return _offset;
    }

  private:
    AssemblyInstruction _inst;
    int _reg;
    int _offset;
};

std::vector<Instruction> getInstructions( std::ifstream &file ) {
    std::vector<Instruction> ret;
    std::string str;
    std::string inst;
    std::string reg;
    std::string offset;
    while ( std::getline( file, str ) ) {
        std::stringstream ss( str );
        ss >> inst;
        ss >> reg;
        if ( inst[0] == 'j' ) {
            if ( inst == "jmp" ) {
                ret.emplace_back( inst, "", reg );
            } else {
                ss >> offset;
                ret.emplace_back( inst, reg, offset );
            }
        } else {
            ret.emplace_back( inst, reg, "" );
        }
    }
    return ret;
}

void executeInstruction( Instruction &instruction, uint64_t &reg_a,
                         uint64_t &reg_b, size_t &pc ) {
    uint64_t &work_reg = instruction.getReg() == 0 ? reg_a : reg_b;
    switch ( instruction.getIns() ) {
    case half:
        work_reg /= 2;
        break;
    case triple:
        work_reg *= 3;
        break;
    case increase:
        work_reg++;
        break;
    case jump:
        pc += instruction.getOffset() - 1;
        break;
    case jump_even:
        if ( work_reg % 2 == 0 )
            pc += instruction.getOffset() - 1;
        break;
    case jump_one:
        if ( work_reg == 1 )
            pc += instruction.getOffset() - 1;
    default:
        break;
    }
}

void executeProgram( std::vector<Instruction> &instructions, uint64_t &reg_a,
                     uint64_t &reg_b ) {
    size_t program_counter = 0;
    while ( program_counter < instructions.size() ) {
        executeInstruction( instructions[program_counter], reg_a, reg_b,
                            program_counter );
        program_counter++;
    }
}

int main() {
    std::ifstream input_file( "input" );
    auto instructions = getInstructions( input_file );
    uint64_t reg_a{}, reg_b{};

    executeProgram( instructions, reg_a, reg_b );

    std::cout << "Value in register B is \033[93;1m" << reg_b
              << "\033[0m (value in A is \033[91;1m" << reg_a << "\033[0m)"
              << std::endl;
    //----------PART 2-----------
    reg_a = 1;
    reg_b = 0;

    executeProgram( instructions, reg_a, reg_b );

    std::cout << "Starting with A=1, value in register B is \033[93;1m" << reg_b
              << "\033[0m (value in A is \033[91;1m" << reg_a << "\033[0m)"
              << std::endl;
}
