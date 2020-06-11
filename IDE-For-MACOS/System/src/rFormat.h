/*
 * File: rFormat.h
 * ---------------
 * This file exports a function that translate two R type
 * assembly instruction, simple one-to-one correspondence.
 *
 * add, rd, rs, rt
 * mul, rs, rt
 */

#ifndef RFORMAT_H
#define RFORMAT_H

#include <map>
#include <string>
#include "tokenscanner.h"
#include "baseConversion.h"
#include "mapInfo.h"

using namespace std;

string rCode(string instruction) {
    TokenScanner scanner;
    scanner.setInput(instruction);
    scanner.ignoreWhitespace();

    string token = scanner.nextToken();
    int id = 0; // 0 means add
    if (token=="mul") id=1;
    if (token=="sub") id=2;

    bool flag = false;
    int i = -1;
    string registers[3];


    while (scanner.hasMoreTokens()) {
        string token = scanner.nextToken();
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
    string rt = Rname.find(registers[2])->second;
    string rd = Rname.find(registers[0])->second;

    string results;

    if (id==1) { // mul
        results = "011100"+rs+rt+rd+string(5, '0')+"000010";

    }
    else if (id==2) { //sub
        results = string(6, '0')+rs+rt+rd+string(5, '0')+dcmlToBit(34, 6);
    }
    else {  // add
        results = string(6, '0')+rs+rt+rd+string(5, '0')+dcmlToBit(32, 6);
    }

    return results;
}

/*
 * Div case.
 *
 * div  src1, src2
 * means put quotients to lo($a2), remainders to hi($a3).
 *
 * div  des, src1, src2     <=>
 * div  src1, src2
 * move des,  lo($a2)
 */

string dCode(string instruction) {
    TokenScanner scanner;
    scanner.setInput(instruction);
    scanner.ignoreWhitespace();

    string token;
    string registers[3];
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
    // div, src1, src2 case.
    if (i==1) {
        results = string(6, '0') + Rname.find(registers[0])->second + Rname.find(registers[1])->second;
        results = results + string(10, '0') + dcmlToBit(27, 6);
    }
    // div, des, src1, src2 case.
    else {
        results = string(6, '0') + Rname.find(registers[1])->second + Rname.find(registers[2])->second;
        results = results + string(10, '0') + dcmlToBit(27, 6);

        string newAdd = "add $" + registers[0] + " $zero " + "$a2";
        results = results + "\n" + rCode(newAdd);
    }

    return results;
}

#endif // RFORMAT_H
