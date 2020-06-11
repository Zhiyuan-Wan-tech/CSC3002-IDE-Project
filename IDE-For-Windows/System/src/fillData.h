/*
 * File: fillData.h
 * ----------------
 * This file is updated.
 *
 * Old version: one word in virtual memory data segment can
 * store a string of any length.
 *
 * New version: one word in virtual memory data segment can
 * only contain 4 bytes of information, i.e 4 characters if
 * storing string.
 */

#ifndef FILLDATA_H
#define FILLDATA_H

#include <string>
#include <fstream>
#include <tokenscanner.h>
#include <baseConversion.h>
#include "virtualMemory.h"

using namespace std;

/* Constants */

static const int dataStart = 1024;

/* findData function find data in the data directives.
 * Stores the data in paramter content.
 */


int findData(string line, string & content) {

    TokenScanner scanner;
    scanner.setInput(line);
    scanner.ignoreWhitespace();
    scanner.scanStrings();
    string token;
    bool flag = false;

    while (scanner.hasMoreTokens()) {
        token = scanner.nextToken();

        // require .space or .asciiz followed directly by content
        if (flag) {
            if (token=="asciiz") {
                content = scanner.nextToken();
                return 0;

                // 0 means .asciiz
            }

            if (token=="space") {
                content = scanner.nextToken();
                return 1;

                // 1 means .space
            }
            if (token=="byte") {
                content = scanner.nextToken();
                content = "\""+string(stoi(content), char(0))+"\"";
                return 2;

                // 2 means char arrays
            }
        }
        if (token==".") {
            flag = true;
        }
    }

    return 3;

    // 3 means no data
}

void fillData(string name, virtualMemory & mainMem) {
    int i = dataStart;
    string dname = name + ".asm";
    ifstream inf;
    inf.open(dname.c_str());

    string line;
    string content;

    int indicator;

    while (getline(inf, line)) {

        indicator = findData(line, content);

        if (indicator==0 || indicator==2) {

            // char array dealt similar as string, all entires default to none.
            int countf = 0;
            string myWord = "";
            for (int j=1;j<int(content.length()-1);j++) {

                if (countf==4) {
                    mainMem.array[i++].fill(myWord);
                    countf = 0;
                    myWord = "";
                }
                if (countf<4) {
                    countf++;
                    myWord = myWord + dcmlToBit(int(content[j]), 8);
                }
            }
            myWord = myWord + string((4-countf)*8, '0');
            mainMem.array[i++].fill(myWord);

            // 0 byte value indicate the end of an ASCII string or char array
            string endSign = string(32, '0');
            mainMem.array[i++].fill(endSign);

        }

        // only need to allocate stoi(content) number of bytes.
        if (indicator==1) {
            // string case, 400 bytes space by default
            if (stoi(content)==400) {
                i += 100;
            }

            // int case, 1 word by default, initialized to zero
            else if (stoi(content)==4) {
                mainMem.array[i++].fill(string(32, '0'));
            }

            // int array case, n words, where n is in content, all entries default zero.
            else {
                for (int ti=0;ti<stoi(content)/4;ti++) {
                    mainMem.array[i++].fill(string(32, '0'));
                }
            }

        }

        mainMem.datasize = i - dataStart;
    }

    inf.close();
}

#endif // FILLDATA_H
