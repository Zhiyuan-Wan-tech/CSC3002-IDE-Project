/*
 * File: iFormat.h
 * ---------------
 * This file exports a function that can translate one kind of MIPS
 * instruction to its binary equivalents.
 *
 * addi     rd, rs, imm
 *
 * subi     rd, rs, imm     <=>
 * addi     rd, rs, -imm
 *
 * This is a simple task in the sense that it does require any pre-assigned
 * addresses to aid the translation.
 */

#ifndef IFORMAT_H
#define IFORMAT_H

#include <string>
#include "tokenscanner.h"
#include "mapInfo.h"
#include "baseConversion.h"

using namespace std;

string iCode(string instruction) {
    TokenScanner scanner;
    scanner.setInput(instruction);
    scanner.ignoreWhitespace();

    bool flag = false;
    int i = -1;
    string registers[2];
    int imm; // all immediate value are in decimal at the beginning

    string opk = scanner.nextToken();

    while (scanner.hasMoreTokens()) {
        string token = scanner.nextToken();
        if (scanner.getTokenType(token)==scanner.NUMBER) imm=stoi(token);
        if (flag) {
            registers[i] = token;
            flag = false;
        }
        if (token=="$") {
            flag = true;
            i++;
        }
    }

    string rs = Rname.find(registers[1])->second;
    string rd = Rname.find(registers[0])->second;
    string imme = dcmlToBit(imm, 16);

    // first digit 1 indicates negative number.
    if (opk=="subi") {
        imme[0]='1';
    }

    return dcmlToBit(8, 6)+rs+rd+imme;
}

#endif // IFORMAT_H
