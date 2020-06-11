/*
 * File: bFormat.h
 * ---------------
 * This file exports a function that can translate two types of assembly
 * instruction to its numeric equivalents.
 *
 * beq      rs, rt, label.
 * blez     rs, label.
 * bne      rs, rt, label.
 *
 * They are both MIPS native instructions, so translation is a mere one-to-one
 * correspondance. Since they both contain label, SymbolTable need to be passed
 * by reference to the function to know their addresses.
 */

#ifndef BFORMAT_H
#define BFORMAT_H

#include <string>
#include "tokenscanner.h"
#include "mapInfo.h"
#include "baseConversion.h"
using namespace std;

string bCode(string instruction, map<string, string> & SymbolTable) {
    TokenScanner scanner;
    scanner.setInput(instruction);
    scanner.ignoreWhitespace();

    string opKind = scanner.nextToken();
    string token;
    string registers[2];
    string results;
    bool flag = false;
    int i = -1;             // i will enventually denotes the register index

    while (scanner.hasMoreTokens()) {
        token = scanner.nextToken();
        if (flag) {
            registers[i] = token;
            flag = false;
        }
        if (token=="$") {
            flag = true;
            i++;
        }
    }

    // use i to distingiuish bewteen two types of instructions
    if (i==1) {
        if (opKind=="beq") {
            return dcmlToBit(4, 6) + Rname.find(registers[0])->second +
                    Rname.find(registers[1])->second + SymbolTable.find(token)->second;
        }
        // case "bne"
        return dcmlToBit(5, 6) + Rname.find(registers[0])->second +
                Rname.find(registers[1])->second + SymbolTable.find(token)->second;
    }

    // blez
    else {
        return dcmlToBit(6, 6) + Rname.find(registers[0])->second +
                "00000" + SymbolTable.find(token)->second;
    }
}

#endif // BFORMAT_H
