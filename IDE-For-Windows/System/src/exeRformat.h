/*
 * File: exeRformat.h
 * ------------------
 * This file exports a function that can executes add, sub, div, mul MIPS instruction.
 * Note, move instruction is also assembled as add commmand.
 *
 * Also can executes addi, subi, which are I format instructions.
 */

#ifndef EXERFORMAT_H
#define EXERFORMAT_H

#include <iostream>
#include <string>
#include "console.h"
#include "mapInfo.h"
#include "registersMem.h"
#include "baseConversion.h"

using namespace std;

void exeIformat(string line, RegisterMem & rgsMem) {
    string rdest = line.substr(11, 5);
    string rs = line.substr(6, 5);

    char ind = line[16];
    int imm = bitToDcml(line.substr(17, 16));
    int newvalue;

    // subi case
    if (ind=='1') {
        newvalue = rgsMem.array[bitToDcml(rs)] - imm;
    }

    // addi case
    else {
        newvalue = rgsMem.array[bitToDcml(rs)] + imm;
    }
    rgsMem.updateRgs(rdest, newvalue);
}

void exeRformat(string line, RegisterMem & rgsMem) {

    // div case first
    if (line.substr(26, 6)==dcmlToBit(27, 6)) {
        string src1 = line.substr(6, 5);
        string src2 = line.substr(11, 5);
        string lo = Rname.find("a2")->second;   // $a2 to replace lo
        string hi = Rname.find("a3")->second;   // $a3 to replace hi

        int quotient = rgsMem.array[bitToDcml(src1)] / rgsMem.array[bitToDcml(src2)];
        int remainder = rgsMem.array[bitToDcml(src1)] % rgsMem.array[bitToDcml(src2)];
        rgsMem.updateRgs(lo, quotient);
        rgsMem.updateRgs(hi, remainder);
        return;
    }

    string rdest = line.substr(16, 5);
    string rs = line.substr(6, 5);
    string rt = line.substr(11, 5);

    int newValue;

    // add
    if (line.substr(26, 6)==dcmlToBit(32, 6)) {
        newValue = rgsMem.array[bitToDcml(rs)] + rgsMem.array[bitToDcml(rt)];
    }

    // sub
    else if (line.substr(26, 6)==dcmlToBit(34, 6)) {
        newValue = rgsMem.array[bitToDcml(rs)] - rgsMem.array[bitToDcml(rt)];
    }

    // mul
    else {
        newValue = rgsMem.array[bitToDcml(rs)] * rgsMem.array[bitToDcml(rt)];
    }

    rgsMem.updateRgs(rdest, newValue);
}

#endif // EXERFORMAT_H
