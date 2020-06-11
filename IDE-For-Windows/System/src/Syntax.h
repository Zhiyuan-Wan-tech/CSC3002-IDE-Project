#ifndef _syntax_h
#define _syntax_h
#include<string>
#include<vector>
using namespace std;


struct IR {
    string op;
    string src1;
    string src2;
    string dst;
};


struct Symbol {
    string name;
    string type;
    string mes;
    int address;
    int value;
    int array_length = 1;
};

extern vector<IR> midcode;
extern vector<Symbol> symbolTable;


void variable_declaration();
void multi_var_declaration(string var_type);
void test();
void whole();

void print_token();

#endif
