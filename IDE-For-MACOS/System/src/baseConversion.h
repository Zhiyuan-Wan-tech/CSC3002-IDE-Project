/*
 * File: baseConversion.h
 * ----------------------
 * This file exports various methods to do base conversion.
 * For example, hex to binray, binary to decimal, etc.
 */

#ifndef BASECONVERSION_H
#define BASECONVERSION_H

#include <string>
#include <cmath>
#include <iostream>
#include <sstream>
using namespace std;

int spower(int exp) {
    int results = 1;
    for (int i=1;i<=exp;i++){
        results *= 2;
    }
    return results;
}

int bitToDcml(string binary) {
    int n = binary.length();
    int results = 0;
    for (int i=0;i<n;i++){
        if (binary[i]=='1') {
            results += spower(n-i-1);
        }
    }
    return results;
}

string dcmlToHex(int n) {
    stringstream s;
    s << hex << n;
    string results;
    s >> results;

    // make the output the desired length by padding zeros in the front
    int pad = 8-results.length();
    results = string(pad, '0') + results;

    return results;
}

string dcmlToBit(int n, int len) {

    // array to store binary number
    int binaryNum[32];

    // counter for binary array
    int i = 0;

    while (n>0) {

        // store remainder in binary array
        binaryNum[i] = n % 2;
        n = n/2;
        i++;
    }

    // form binary string in reverse order
    string results;
    for (int j=0;j<i;j++) {
        results = to_string(binaryNum[j]) + results;
    }

    // make the output the desired length by padding zeros in the front
    int pad = len-results.length();
    results = string(pad, '0') + results;

    return results;
}

#endif // BASECONVERSION_H
