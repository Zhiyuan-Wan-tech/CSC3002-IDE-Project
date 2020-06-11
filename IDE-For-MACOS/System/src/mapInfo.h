/*
 * File: mapInfo.h
 * ---------------
 * THis file exports a map that contains the binary representation in 5 bits
 * for all 32 registers.
 */

#ifndef MAPINFO_H
#define MAPINFO_H

#include <map>
#include <string>
using namespace std;

const map<string, string> Rname = {
        {"zero","00000"}, {"at","00001"},{"v0","00010"},{"v1","00011"},{"a0","00100"},{"a1","00101"},{"a2","00110"},{"a3","00111"},
        {"t0","01000"},{"t1","01001"},{"t2","01010"},{"t3","01011"},{"t4","01100"},{"t5","01101"},{"t6","01110"},{"t7","01111"},
        {"s0","10000"},{"s1","10001"},{"s2","10010"},{"s3","10011"},{"s4","10100"},{"s5","10101"},{"s6","10110"},{"s7","10111"},
        {"t8","11000"},{"t9","11001"},{"k0","11010"},{"k1","11011"},{"gp","11100"},{"sp","11101"},{"fp","11110"},{"ra","11111"}
};

/* A method for finding key given value */

string getKeyRgs(string line) {
    for (auto it = Rname.begin(); it != Rname.end(); ++it) {
        if (it->second==line) {
            return it->first;
        }
    }
    return "none";
}

#endif // MAPINFO_H
