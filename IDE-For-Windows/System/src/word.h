/*
 * File: word.h
 * ------------
 * This file exports a word class that represents 32 bits word
 * in hardware, with convinient methods to extract 4 bytes in this word.
 */

#ifndef WORD_H
#define WORD_H

#include <string>
#include <iostream>

using namespace std;

class Word{

public:

    /* Instance variables */

    string value;

    /*
     * Constructor: Word
     * Usage Word word(value);
     * ----------------
     * Initializes a new empty word.
     */

    Word(){};

    Word(string val){
        value = val;
    }

    string first() {
        return value.substr(0, 8);
    }

    string second() {
        return value.substr(8, 8);
    }

    string third() {
        return value.substr(16, 8);
    }

    string fourth() {
        return value.substr(24, 8);
    }

    void fill(string val) {
//        cout << val << endl;
        value = val;
    }

};

#endif // WORD_H
