/*
 * File: opKind.h
 * --------------
 * This file helps the eventual translation of a complete
 * assembly file to binary files.
 */

#ifndef OPKIND_H
#define OPKIND_H

#include <string>
#include <set>
#include "tokenscanner.h"
#include "mapInfo.h"
#include "baseConversion.h"

using namespace std;

/*
 * This function offers a set that contains all supported operation.
 */

void createOpSet(set<string> & opSet) {
    opSet.clear();
    opSet.insert("add");
    opSet.insert("sub");
    opSet.insert("mul");
    opSet.insert("addi");
    opSet.insert("b");
    opSet.insert("beq");
    opSet.insert("blez");
    opSet.insert("bgt");
    opSet.insert("la");
    opSet.insert("li");
    opSet.insert("move");
    opSet.insert("syscall");
    opSet.insert("beqz");
    opSet.insert("bge");
    opSet.insert("bne");
    opSet.insert("subi");

    opSet.insert("lb");
    opSet.insert("sb");

    opSet.insert("lw");
    opSet.insert("sw");
    opSet.insert("div");
    opSet.insert("divu");
}

/*
 * This function scans one line of MIPS assembly language file,
 * if it is an instruction, i.e. contain an operation in the opSet,
 * then the operation kind is returned, if it is not an instruction,
 * "none" is returned, as a code name for no instruction.
 */

string opkind(string instruction, set<string> & opSet) {
    TokenScanner scanner;
    scanner.setInput(instruction);
    scanner.ignoreWhitespace();

    while (scanner.hasMoreTokens()) {
        string token = scanner.nextToken();
        if (opSet.find(token) != opSet.end()) {
            return token;
        }
    }

    return "none";
}

#endif // OPKIND_H
