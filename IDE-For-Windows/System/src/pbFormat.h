/*
 * File: pbFormat.h
 * ----------------
 * This file deals with two pseduo-instruction.
 * SymbolTable is passed  by reference as function parameter since
 * the addresses of label is needed in the translation process.
 *
 * b    label   // <=>
 * beq  $zero,  $zero,  label
 *
 * beqz src1,   label   // <=>
 * beq  src1,   $zero,  label
 *
 * bgt  rsrc1,  rsrc2,  label   // <=>
 * slt  $at,    rsrc2,  rsrc1   // $at is 1 if rsrc1 > rsrc2, 0 otherwise.
 * bne  $at,    $zero,  label   // If $at is not 0, then branch.
 *
 * bge  rsrc1,  rsrc2,  label   // <=>
 * slt  $at,    rsrc1,  rsrc2   // $at is 1 if rsrc1 <  rsrc2, 0 otherwise.
 * beq  $at,    $zero,  label   // if $at is 0, then branch.
 */

#ifndef PBFORMAT_H
#define PBFORMAT_H

#include <string>
#include "tokenscanner.h"
#include "mapInfo.h"
#include "baseConversion.h"
#include "bFormat.h"

using namespace std;

string pbCode(string instruction, map<string, string> & SymbolTable) {
    TokenScanner scanner;
    scanner.setInput(instruction);
    scanner.ignoreWhitespace();

    string opKind = scanner.nextToken();

    // require the instruction must end with label

    string label;
    while (scanner.hasMoreTokens()) {
        label = scanner.nextToken();
    }

    if (opKind=="b") {
        string eqlv = "beq $zero, $zero, " + label;
        return bCode(eqlv, SymbolTable);
    }

    else if (opKind=="beqz") {
        TokenScanner nscanner;
        nscanner.setInput(instruction);
        nscanner.ignoreWhitespace();

        string onlySrc1;
        bool flag = false;

        while (nscanner.hasMoreTokens()) {
            string token = nscanner.nextToken();
            if (flag) {
                onlySrc1 = token;
                break;
            }
            if (token=="$") {
                flag = true;
            }
        }

        string eqlv = "beq $" + onlySrc1 + ", $zero, " + label;
        return bCode(eqlv, SymbolTable);
    }

    else {
        // slt $at, ...
        bool flag = false;
        int i = -1;
        string registers[2];

        TokenScanner nscanner;
        nscanner.setInput(instruction);
        nscanner.ignoreWhitespace();

        while (nscanner.hasMoreTokens()) {
            string token = nscanner.nextToken();
            if (flag) {
                registers[i] = token;
                flag = false;
            }
            if (token=="$") {
                flag = true;
                i++;
            }
        }

        string rd = Rname.find("at")->second;
        string rs = Rname.find(registers[0])->second;
        string rt = Rname.find(registers[1])->second;

        if (opKind=="bgt") {
            string sltpart = string(6, '0')+rt+rs+rd+string(5, '0')+dcmlToBit(42, 6);
            string bnepart = dcmlToBit(5, 6) + rd + Rname.find("zero")->second + SymbolTable.find(label)->second;
            return sltpart + "\n" + bnepart;
        }

        string sltpart = string(6, '0')+rs+rt+rd+string(5, '0')+dcmlToBit(42, 6);
        string beqpart = dcmlToBit(4, 6) + rd + Rname.find("zero")->second + SymbolTable.find(label)->second;
        return sltpart + "\n" + beqpart;
    }
}

#endif // PBFORMAT_H
