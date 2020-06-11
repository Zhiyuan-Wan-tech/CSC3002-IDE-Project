/*
 * File: exeSLw.h
 * --------------
 * This file exports functions that can execute lw, sw format.
 */

#ifndef EXESLW_H
#define EXESLW_H

#include <iostream>
#include <string>
#include "console.h"
#include "mapInfo.h"
#include "registersMem.h"
#include "virtualMemory.h"
#include "baseConversion.h"

using namespace std;

void exeSLw(string line, RegisterMem & rgsMem, virtualMemory & mainMem) {
    string opk = line.substr(0, 6);
    string rdest = line.substr(11, 5);
    string rs = line.substr(6, 5);      // rs is $k0 by default

    int offset = bitToDcml(line.substr(16, 16));
    int byAddr = rgsMem.array[bitToDcml(rs)];

    int woAddr = byAddr/4 + offset;

    // lw case
    if (opk==dcmlToBit(35, 6)) {
        cout << "))) word at address 0x" << dcmlToHex(byAddr) <<" is fetched" << endl;
        rgsMem.updateRgs(rdest, bitToDcml(mainMem.array[woAddr].value));
    }
    // sw case
    if (opk==dcmlToBit(43, 6)) {
        cout << "))) word at address 0x" << dcmlToHex(byAddr) <<" is updated" << endl;
        mainMem.array[woAddr].value = dcmlToBit(rgsMem.array[bitToDcml(rdest)], 32);
    }
}

#endif // EXESLW_H
