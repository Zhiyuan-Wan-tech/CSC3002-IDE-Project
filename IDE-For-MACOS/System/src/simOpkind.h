/*
 * File: simOpkind.h
 * -----------------
 * This file exports a function that scans one line of binary
 * code, and output information that indicates what kind of
 * operation this corresponds to.
 */

#ifndef SIMOPKIND_H
#define SIMOPKIND_H

#include <string>
#include "tokenscanner.h"
#include "mapInfo.h"
#include "baseConversion.h"

using namespace std;

string simOpKind(string line) {
    string str1 = line.substr(0, 6);
    string str2 = line.substr(26, 6);
    if (line=="00000000000000001111111111111111") {
        return "syscall";
    }

    // addi, subi (implemented with addi)
    else if (str1==dcmlToBit(8, 6)) {
        return "Iformat";
    }

    // mul
    else if (str1==dcmlToBit(28, 6)) {
        return "Rformat";
    }

    // add/sub/div or slt
    else if (str1==dcmlToBit(0, 6)) {
        // slt indicates bgt
        if (str2==dcmlToBit(42, 6)) {
            return "pBformat";

        }
        else {
            return "Rformat";
        }
    }

    // lb or sb
    else if (str1==dcmlToBit(32, 6) || str1==dcmlToBit(40, 6)) {
        return "LSBformat";
    }

    // lui (indicates la or li)
    else if (str1==dcmlToBit(15, 6)) {
        return "Lformat";
    }

    // lw, sw
    else if (str1==dcmlToBit(35, 6) || str1==dcmlToBit(43, 6)) {
        return "SLwformat";
    }

    // beq, bne, or blez
    else if (str1==dcmlToBit(4, 6) || str1==dcmlToBit(6, 6) || str1==dcmlToBit(5, 6)) {
        return "Bformat";
    }
    else {
        return ": did not found op in binary code!!";
    }

}


#endif // SIMOPKIND_H
