/*
 * File: virtualMemory.h
 * ---------------------
 * This file exports a virtualMemory class which represents hardware main
 * memory, the default size is 10^4 words, or 4 * 10*4 bytes. Some getters
 * are implemented for convinience.
 */

#ifndef VIRTUALMEMORY_H
#define VIRTUALMEMORY_H

#include <string>
#include <fstream>
#include "word.h"
using namespace std;

/* Constructor can take file stream, no method implemented */

class virtualMemory {

public:

    /* Constant */

    static const int CAPACITY = 100000;      // memory

    /* Instance variables */

    Word *array;        // A dynamic array of words
    Word *pc;           // Program counter
    int size;
    int datasize;

    /* Constructor */

    virtualMemory() {
        array = new Word[CAPACITY];
        pc = array;
        size = 0;
        datasize = 0;
    }

    virtualMemory(ifstream & inf) {
        array = new Word[CAPACITY];
        pc = array;
        string line;
        int i = 0;
        while (getline(inf, line)) {
            array[i].fill(line);
            i++;
        }
        size = i;
        datasize = 0;
    }

    /* Destructor */

    ~virtualMemory() {
        delete [] array;
    }

};

#endif // VIRTUALMEMORY_H
