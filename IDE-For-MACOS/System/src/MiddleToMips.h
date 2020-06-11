#ifndef MIDDLETOMIPS_H
#define MIDDLETOMIPS_H
#include <iostream>
#include <vector>
#include <fstream>

#include "PathName.h"

using namespace std;


enum Opcode { // Four code option
    INT, CHAR,ASSIGN,PLUS,MINUS, MULTI,BEQ,BNE,L,BGTZ, BGEZ, INPUT,OUTPUT,J
};



struct Mipscode{
    string op;
    string rs;
    string rt;
    string rd;
};
int locate(string name);
void entermips(string mipsop, string mipsrs, string mipsrt, string mipsrd);
void gen_mips();
void print_mips();

//string trans_op_to_sting(enum Opcode enumop);

#endif // MIDDLETOMIPS_H
