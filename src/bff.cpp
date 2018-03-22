#include <iostream>
#include <fstream>
#include <list>
#include <stack>
#include <cassert>

enum OPCODE { OPEN, CLOSE, PLUS, MINUS, INCR, DECR, OUT, IN, NOP };

struct Instr {
        Instr(OPCODE op) : opcode(op) {};
        Instr(OPCODE op, std::list<Instr>::iterator it)
                : opcode(op), jump(it) {};
        
        OPCODE opcode;
        std::list<Instr>::iterator jump;
};

std::list<Instr> parse(std::istream &is)
{
        std::list<Instr> op_list;
        std::stack<std::list<Instr>::iterator> jump_stack;

        char op;

        while (is >> op) {
                switch (op) {
                        case '+' : op_list.emplace_back(PLUS);  break;
                        case '-' : op_list.emplace_back(MINUS); break;
                        case '>' : op_list.emplace_back(INCR);  break;
                        case '<' : op_list.emplace_back(DECR);  break;
                        case '.' : op_list.emplace_back(OUT);   break;
                        case ',' : op_list.emplace_back(IN);    break;
                        case '[' : op_list.emplace_back(OPEN);
                                jump_stack.push(std::prev(op_list.end())); break;
                        case ']' : assert(!jump_stack.empty());
                        {
                                auto match = jump_stack.top();
                                jump_stack.pop();
                                op_list.emplace_back(CLOSE, match);
                                match -> jump = std::prev(op_list.end()); break;
                        }
                        default : break;
                }
        }
        assert(jump_stack.empty());
        op_list.emplace_back(NOP);
        
        return op_list;
}

void execute(std::list<Instr> op_list)
{
        char *space = new char[60000]();
        char *ptr   = &space[30000];
        
        for (std::list<Instr>::iterator it = op_list.begin();; ++it) {
                switch (it -> opcode) {
                        case PLUS  : ++ *ptr;  break;
                        case MINUS : -- *ptr;  break;
                        case INCR  : ++  ptr;  break;                        
                        case DECR  : --  ptr;  break;
                        case OUT   : std::cout << *ptr;     break;
                        case IN    : std::cin  >> *ptr;     break;
                        case OPEN  : if (!*ptr) it = it -> jump; break;
                        case CLOSE : if ( *ptr) it = it -> jump; break;
                        case NOP   : delete [] space; return;
                        default : assert(false);
                }
        }
}

int main(int argc, char *argv[])
{
        assert (argc > 1);
        std::ifstream inf(argv[1]);
        
        assert(inf.is_open());
        std::list<Instr> op_list = parse(inf);
        
        execute(op_list);

        return 0;
}

