/*
 * File: exeSyscall.h
 * ------------------
 * This executes syscall.
 */

#ifndef EXESYSCALL_H
#define EXESYSCALL_H

#include <ios>
#include <limits>
#include <iostream>
#include <string>
#include "console.h"
#include "mapInfo.h"
#include "registersMem.h"
#include "virtualMemory.h"
#include "baseConversion.h"
#include "fillData.h"

using namespace std;

void printstring(int addr, virtualMemory & mainMem) {
    addr = addr/4;
    char c1, c2, c3, c4;
    int i = 0;
    while (mainMem.array[addr+i].value!=string(32, '0')) {
        c1 = bitToDcml(mainMem.array[addr+i].first());
        c2 = bitToDcml(mainMem.array[addr+i].second());
        c3 = bitToDcml(mainMem.array[addr+i].third());
        c4 = bitToDcml(mainMem.array[addr+i].fourth());
        cout << c1 << c2 << c3 << c4;
        i++;
    }
}

void getstring(int addr, virtualMemory & mainMem) {
    addr = addr/4;
    string results;
    cout << "Enter a string: " << endl;
    getline(cin, results);

    int countf = 0;
    string myWord = "";

    for (int i=0;i<int(results.length());i++) {
        if (countf==4) {
            mainMem.array[addr++].fill(myWord);
            countf = 0;
            myWord = "";
        }
        if (countf<4) {
            countf++;
            myWord = myWord + dcmlToBit(int(results[i]), 8);
        }
    }
    myWord = myWord + string((4-countf)*8, '0');
    mainMem.array[addr++].fill(myWord);

    // 0 byte value indicate the end of an ASCII string
    string endSign = string(32, '0');
    mainMem.array[addr++].fill(endSign);

}

void exeSyscall(RegisterMem & rgsMem, virtualMemory & mainMem) {
    int code = rgsMem.array[2];
    // exit
    if (code==10) {
        cout << "/* reached the end of the program, exit now */" << endl;
    }

    // print integer
    else if (code==1) {
        int index = bitToDcml(Rname.find("a0")->second);
        cout << rgsMem.array[index] << endl;
    }

    // print character
    else if (code==3) {
        int index = bitToDcml(Rname.find("a0")->second);
        cout << char(rgsMem.array[index]) << endl;
    }

    // print string at address $a0
    else if (code==4) {
        int idxRgst = bitToDcml(Rname.find("a0")->second);
        int address = rgsMem.array[idxRgst];
        cout << "data at address 0x" << dcmlToHex(address) << " is being fetched" << endl;
        printstring(address, mainMem);
    }

    // read integer/ascii of char to $v0
    else if (code==5){
        int newvalue;
        cout << "enter a number: ";
        cin >> newvalue;
//        cin.ignore(numeric_limits<streamsize>::max(),'\n');

        rgsMem.updateRgs(Rname.find("v0")->second, newvalue);
    }

    // read string from console to address $a0, string length at $a1.
    else {
        int idxRgst = bitToDcml(Rname.find("a0")->second);
        int address = rgsMem.array[idxRgst];

        getstring(address, mainMem);
        cout << "string read from console is stored at address 0x" << dcmlToHex(address) << endl;
    }

}

#endif // EXESYSCALL_H
