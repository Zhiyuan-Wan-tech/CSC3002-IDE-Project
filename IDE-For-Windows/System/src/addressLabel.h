/*
 * File: adressLabel.h
 * -------------------
 * This file exports functions that will give label a virtual address in
 * the simulated memory, more specifically, give every instruction an
 * address in the text segment of the data linearly.
 */

#ifndef ADDRESSLABEL_H
#define ADDRESSLABEL_H


#include <map>
#include <string>
#include <fstream>
#include "tokenscanner.h"
#include "baseConversion.h"
#include "opkind.h"

#include "fillData.h"
using namespace std;

/*
 * The following function scans one line, and determines if it contains any label.
 * If it does, then it is passed by reference to the variable label.
 */

bool findLabel(string instruction, string & label) {

    TokenScanner scanner;
    scanner.setInput(instruction);
    scanner.ignoreWhitespace();
    string token;

    while (scanner.hasMoreTokens()) {
        token = scanner.nextToken();
        if (token==":") {
            return true;
        }
        label = token;
    }

    return false;
}

// findDaMark locates the .data directive

void findDaMark(string line, bool & flag) {
    TokenScanner scanner;
    scanner.setInput(line);
    scanner.ignoreWhitespace();
    string token;
    string ntoken;

    while (scanner.hasMoreTokens()) {
        token = scanner.nextToken();
        if (token==".") {
            ntoken = scanner.nextToken();
            if (ntoken=="data") {
                flag = true;
            }
        }
    }

}


/*
 * The following function scans a complete assembly language file, find all the
 * labels by using the findLabel function, then assigns them an address.
 */

map<string, string> & createTable(map<string, string> & table, ifstream & inf) {

    // make label in text segment have the right address
    // label in data segment address starting from const dataStart

    int address1 = 0;    // base 10 for now
    int address2 = dataStart;    // base 10
    string line;
    string label;

    set<string> opSet;
    createOpSet(opSet);

    bool flag = false;
    while (getline(inf, line)) {
        string opera = opkind(line, opSet);
        if (opera!="none") {
            address1++;
            if (opera=="bgt" || opera=="la" || opera=="li" || opera=="bge" || opera=="div") {
                address1++;
            }
            if (opera=="lw" || opera=="sw") {
                address1 += 2;
            }
        }
        if (!flag) {
            findDaMark(line, flag);
            if (findLabel(line, label)) {
                table.insert({label, dcmlToBit(address1*4, 16)});
            }
        }

        if (flag) {

            if (findLabel(line, label)) {
                string content;

                int indicator = findData(line, content);
                table.insert({label, dcmlToBit(address2*4, 16)});

                // .asciiz / .byte
                if (indicator==0 || indicator==2) {
                    int remainder = (content.length()-2) % 4;
                    address2 += (content.length()-2)/4 + 1;
                    if (remainder!=0) {
                        address2++;
                    }
                }

                // .space
                if (indicator==1) {
                    // string case, allocated bytes default to 400
                    if (stoi(content)==400) {
                        address2 += 100;
                    }
                    // int case, 1 word by default
                    else if (stoi(content)==4) {
                        address2++;
                    }
                    // int array case
                    else {
                        address2 += stoi(content)/4;
                    }
                }
            }

        }
    }
    return table;
}

#endif // ADDRESSLABEL_H
