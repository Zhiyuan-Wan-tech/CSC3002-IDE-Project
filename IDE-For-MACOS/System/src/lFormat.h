/*
 * File: lFormat.h
 * ---------------
 * This file exports functions that help to translate
 * two load instructions to their binary equivalents.
 */

#ifndef LFORMAT_H
#define LFORMAT_H

#include <string>
#include "tokenscanner.h"
#include "mapInfo.h"
#include "baseConversion.h"

using namespace std;

/*
 * This function takes one line of MIPS instruction as input, also
 * required is the SymbolTable due to the la operation. Both are
 * pseduo-instruction, as explained below.
 *
 * la       rdest,  address             // <=>
 * lui      $s0,    0                   // all addresses are 16 bits for simplicity
 * ori      rdest,  $s0,  address       // $s0 is the intermediate register.
 *
 * li       rdest,  imme                // <=>
 * lui      $s0,  0                     // upper bits are all zeros.
 * ori      rdest,  $s0, imme
 *
 * lb       rdest,  (imme)rsrc
 */

string lCode(string instruction, map<string, string> SymbolTable) {

    TokenScanner scanner;
    scanner.setInput(instruction);
    scanner.ignoreWhitespace();

    string opKind = scanner.nextToken();
    bool flag = false;
    string token;

    // used to distinguish lb from la and li.
    string registers[2];
    int i=-1;
    int offset = 0;

    while (scanner.hasMoreTokens()) {
        token = scanner.nextToken();
        if (flag) {
            registers[i] = token;
            flag = false;
        }
        if (token=="$") {
            i++;
            flag = true;
        }
        if (token=="(") {
            // there is offset
            offset = stoi(scanner.nextToken());
        }
    }

    // pseduo instruction li, la are implemented by ori and lui.


    // lb/sb, note, the immediate value is assumed zero, so do not support offset yet.
    if (i==1) {
        string results = Rname.find(registers[1])->second + Rname.find(registers[0])->second;
        if (opKind=="lb") {
            return dcmlToBit(32, 6) + results + dcmlToBit(offset, 16);
        }
        // sb case
        return dcmlToBit(40, 6) + results + dcmlToBit(offset, 16);
    }

    string lui = dcmlToBit(15, 6) + "00000" + Rname.find("s0")->second + string(16, '0');
    string imme;

    if (opKind=="li") {
        imme = dcmlToBit(stoi(token), 16);
    }
    else {
        imme = SymbolTable.find(token)->second;
    }
    string ori = dcmlToBit(13, 6) + Rname.find("s0")->second + Rname.find(registers[0])->second + imme;
    return lui + "\n" + ori;

}

#endif // LFORMAT_H
