/*
 * File: registersMem.h
 * --------------------
 * This file exports a registerMem class that represents
 * 32 general purpose registers, the essential data structure
 * is a dynamiclly managed array, array index represents
 * register number.
 */

#ifndef REGISTERSMEM_H
#define REGISTERSMEM_H

#include "mapInfo.h"
#include "baseConversion.h"

/* Essentially a dynamic array of integers */

class RegisterMem {

public:

    /* Instance variables */

    int *array;

    /* Constructor & destructor */

    RegisterMem() {
        array = new int[32];
        for (int i=0;i<32;i++) {
            array[i] = 0;
        }
    }

    ~RegisterMem() {
        delete [] array;
    }

    /* Method for updating the value of registers */
    void updateRgs(string value, int newvalue) {
        int index = bitToDcml(value);
        array[index] = newvalue;
        cout << "/* $" << getKeyRgs(value) << " is updated with new value: " << newvalue << " */";
        cout << endl;
    }

};

#endif // REGISTERSMEM_H
