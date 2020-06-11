/*
 * File: slwFormat.h
 * -----------------
 * This file exports functions that translate sw, lw instructions.
 *
 * lw/sw, rt, label <=>
 *
 * la, rs, label
 * 0x23/2b, rs, rt, offset
 */

#ifndef SLWFORMAT_H
#define SLWFORMAT_H

#include <string>
#include "lFormat.h"
#include "tokenscanner.h"
#include "mapInfo.h"
#include "baseConversion.h"

using namespace std;

string slwCode(string instruction, map<string, string> SymbolTable) {

    TokenScanner scanner;
    scanner.setInput(instruction);
    scanner.ignoreWhitespace();

    string opKind = scanner.nextToken();
    bool flag = false;
    string token;
    int offset = 0;     // default no offset

    string rt;  // destination register

    while (scanner.hasMoreTokens()) {
        token = scanner.nextToken();
        if (flag) {
            rt = token;
            flag = false;
        }
        if (token=="(") {
            // there is offset
            offset = stoi(scanner.nextToken());
        }
        if (token=="$") {
            flag = true;
        }
    }

    string results;
    string laInstru = "la $k0, " + token;

    string lapart = lCode(laInstru, SymbolTable);

    // lw, note, the last 16 bits are offset. Usually need offset for arrays.
    // note, register rs contain address of label.

    if (opKind=="lw") {
        results = dcmlToBit(35, 6) + Rname.find("k0")->second + Rname.find(rt)->second + dcmlToBit(offset, 16);
    }
    // sw case
    if (opKind=="sw") {
        results = dcmlToBit(43, 6) + Rname.find("k0")->second + Rname.find(rt)->second + dcmlToBit(offset, 16);
    }

    return lapart + "\n" + results;

}

#endif // SLWFORMAT_H
