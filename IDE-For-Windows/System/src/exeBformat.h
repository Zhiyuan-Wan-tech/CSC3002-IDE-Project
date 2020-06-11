/*
 * File: exeBformat.h
 * ------------------
 * This file exports functions that can deal with all
 * types of Jump format instructions.
 */

#ifndef EXEBFORMAT_H
#define EXEBFORMAT_H

#include <iostream>
#include <string>
#include "console.h"
#include "mapInfo.h"
#include "registersMem.h"
#include "virtualMemory.h"
#include "baseConversion.h"

using namespace std;

bool exeB(string line, RegisterMem & rgstMem, int & addr) {
    string subsr = line.substr(0, 6);

    // beq
    if (subsr==dcmlToBit(4, 6)) {
        string str1 = line.substr(6, 5);
        string str2 = line.substr(11, 5);
        int val1 = rgstMem.array[bitToDcml(str1)];
        int val2 = rgstMem.array[bitToDcml(str2)];
        if (val1==val2) {
            string str3 = line.substr(16, 16);
            addr = bitToDcml(str3);
            cout << "*** Branching to 0x" << dcmlToHex(addr) << endl;
            return true;
        }
    }

    // blez
    if (subsr == dcmlToBit(6, 6)) {
        string str1 = line.substr(6, 5);
        int val1 = rgstMem.array[bitToDcml(str1)];
        if (val1<=0) {
            string str3 = line.substr(16, 16);
            addr = bitToDcml(str3);
            cout << "*** Branching to 0x" << dcmlToHex(addr) << endl;
            return true;
        }
    }

    // bne
    if (subsr==dcmlToBit(5, 6)) {
        string firSc = line.substr(6, 5);
        string secSc = line.substr(11, 5);

        int firSci = rgstMem.array[bitToDcml(firSc)];
        int secSci = rgstMem.array[bitToDcml(secSc)];
        if (firSci!=secSci) {
            string str3 = line.substr(16, 16);
            addr = bitToDcml(str3);
            cout << "*** Branching to 0x" << dcmlToHex(addr) << endl;
            return true;
        }
    }
    return false;
}

void exeSlt(string line, RegisterMem & rgstMem) {
    string firSc = line.substr(6, 5);
    string secSc = line.substr(11, 5);
    string rdes = line.substr(16, 5);

    int firSci = rgstMem.array[bitToDcml(firSc)];
    int secSci = rgstMem.array[bitToDcml(secSc)];

    if (firSci<secSci) {
        rgstMem.updateRgs(rdes, 1);
    }
    else {
        rgstMem.updateRgs(rdes, 0);
    }

}

#endif // EXEBFORMAT_H
