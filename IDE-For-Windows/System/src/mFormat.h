/*
 * File: mFormat.h
 * ---------------
 * This file exports a function that can translate move operation.
 *
 * move     rdes,   rsrc    // <=>
 *
 * add      rdes,   $zero,  rsrc
 */

#ifndef MFORMAT_H
#define MFORMAT_H

#include <string>
#include "tokenscanner.h"
#include "mapInfo.h"
#include "baseConversion.h"
#include "rFormat.h"
using namespace std;

string mCode(string instruction) {
    TokenScanner scanner;
    scanner.setInput(instruction);
    scanner.ignoreWhitespace();

    string registers[2];
    bool flag = false;
    int i = -1;

    while (scanner.hasMoreTokens()) {
        string token = scanner.nextToken();
        if (flag) {
            registers[i] = "$"+token;
            flag = false;
        }
        if (token=="$") {
            flag = true;
            i++;
        }
    }

    // pseduo instruction move is implemented by add.

    string newAdd = "add " + registers[0] + " $zero " + registers[1];
    return rCode(newAdd);
}

#endif // MFORMAT_H
