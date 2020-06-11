/*
 * File: exeLi.h
 * -------------
 * This file exports a function that can executes the li, la or lb
 * MIPS assembly language code.
 */

#ifndef EXELI_H
#define EXELI_H

#include <iostream>
#include <string>
#include "console.h"
#include "mapInfo.h"
#include "registersMem.h"
#include "virtualMemory.h"
#include "baseConversion.h"

using namespace std;

// execution of li, la share the same pattern.

void exeLi(string line, RegisterMem & rgsMem) {
    int newValue = bitToDcml(line.substr(16, 16));
    string rdest = line.substr(11, 5);
    rgsMem.updateRgs(rdest, newValue);
}

// this function executes both lb and sb.

void exeLb(string line, RegisterMem & rgsMem, virtualMemory & mainMem) {

    string rdest = line.substr(11, 5);
    string rs = line.substr(6, 5);
    string imme = line.substr(16, 16);
    int byAddr = rgsMem.array[bitToDcml(rs)] + bitToDcml(imme);

    cout << "))) byte at address 0x" << dcmlToHex(byAddr) <<" is fetched" << endl;

    int woAddr = byAddr/4;
    int remainder = byAddr%4;

    if (remainder==0) {
        // lb case
        if (line.substr(0, 6)==dcmlToBit(32, 6)) {
            rgsMem.updateRgs(rdest, bitToDcml(mainMem.array[woAddr].first()));
        }
        // sb case
        else {
            string str2 = dcmlToBit(rgsMem.array[bitToDcml(rdest)], 8);
            mainMem.array[woAddr].value.replace(0, 8, str2);
        }

    }
    if (remainder==1) {
        // lb case
        if (line.substr(0, 6)==dcmlToBit(32, 6)) {
            rgsMem.updateRgs(rdest, bitToDcml(mainMem.array[woAddr].second()));
        }
        // sb case
        else {
            string str2 = dcmlToBit(rgsMem.array[bitToDcml(rdest)], 8);
            mainMem.array[woAddr].value.replace(8, 8, str2);
        }
    }
    if (remainder==2) {
        // lb case
        if (line.substr(0, 6)==dcmlToBit(32, 6)) {
            rgsMem.updateRgs(rdest, bitToDcml(mainMem.array[woAddr].third()));
        }
        // sb case
        else {
            string str2 = dcmlToBit(rgsMem.array[bitToDcml(rdest)], 8);
            mainMem.array[woAddr].value.replace(16, 8, str2);
        }
    }
    if (remainder==3) {
        // lb case
        if (line.substr(0, 6)==dcmlToBit(32, 6)) {
            rgsMem.updateRgs(rdest, bitToDcml(mainMem.array[woAddr].fourth()));
        }
        // sb case
        else {
            string str2 = dcmlToBit(rgsMem.array[bitToDcml(rdest)], 8);
            mainMem.array[woAddr].value.replace(24, 8, str2);
        }
    }
}

#endif // EXELI_H
