#include "console.h"
#include <iostream>
#include <fstream>
#include <map>
#include <unistd.h>

// headers for compiler
#include"Syntax.h"
#include"MiddleToMips.h"

// headers for assembler
#include "rFormat.h"
#include "iFormat.h"
#include "bFormat.h"
#include "lFormat.h"
#include "tokenscanner.h"
#include "mFormat.h"
#include "addressLabel.h"
#include "opkind.h"
#include "pbFormat.h"

#include "slwFormat.h"
#include "baseConversion.h"

// headers for simulator
#include "virtualMemory.h"
#include "registersMem.h"
#include "word.h"
#include "exeSyscall.h"
#include "exeLi.h"
#include "exeRformat.h"
#include "simOpkind.h"
#include "fillData.h"
#include "exeBformat.h"
#include "exeSLw.h"
using namespace std;

/* Function Prototypes */

void clearComment(string name);

void print_mid();

void writeFile(string name);

void simulate(string name);

/* Main program */

int main() {

    char* path;
    path = getcwd(NULL, 0);
    string newpath = path;
    newpath = newpath.substr(0,newpath.find("Project_Win")+11);
    free(path);

    string filename = newpath+"\\Files\\arithemetic";

    clearComment(filename);

    // compiler
    test();
    print_mid();
    print_mips();

    // assembler & simulator
    writeFile(filename);
    simulate(filename);

    return 0;
}

void clearComment(string name) {
    ifstream inf;
    ofstream outf;

    string ifname = name + ".txt";
    string ofname = name + "T.txt";

    inf.open(ifname);
    outf.open(ofname);

    TokenScanner scanner;
    scanner.setInput(inf);
    scanner.ignoreComments();
    string token;

    while (scanner.hasMoreTokens()) {
        token = scanner.nextToken();
        outf << token;
    }

    inf.close();
    outf.close();
}

void print_mid() {
    for (int i = 0; i < int(midcode.size()); i++) {
        cout << i << ": op=" << midcode[i].op << "  src1=" << midcode[i].src1 << "  src2=" << midcode[i].src2 << "  dst= " << midcode[i].dst << endl;
    }
}

void writeFile(string name) {
    ifstream inf;
    ofstream outf;



    string ifname = name+".asm";
    string ofname = name+"BINARY.txt";

    inf.open(ifname);
    outf.open(ofname);
    map<string, string> SymbolTable;

    createTable(SymbolTable, inf);

//    for (auto const& x : SymbolTable)
//    {
//        cout << x.first  // string (key)
//                  << ':'
//                  << bitToDcml(x.second)
//                  << endl ;
//    }

    string line;
    set<string> opSet;
    createOpSet(opSet);

    inf.close();
    inf.open(ifname.c_str());

    while (getline(inf, line)) {
        string token = opkind(line, opSet);
        if (token!="none"){
            if (token=="li" || token=="la" || token=="lb" || token=="sb") {
                outf << lCode(line, SymbolTable) << endl;
            }
            if (token=="lw" || token=="sw") {
                outf << slwCode(line, SymbolTable) << endl;
            }
            if (token=="beq" || token=="blez" || token=="bne") {
                outf << bCode(line, SymbolTable) << endl;
            }
            if (token=="b" || token=="bgt" || token=="beqz" || token=="bge") {
                outf << pbCode(line, SymbolTable) << endl;
            }
            if (token=="mul" || token=="add" || token=="sub") {
                outf << rCode(line) << endl;
            }
            if (token=="div" || token=="divu") {
                outf << dCode(line) << endl;
            }
            if (token=="move") {
                outf << mCode(line) << endl;
            }
            if (token=="addi" || token=="subi") {
                outf << iCode(line) << endl;
            }
            if (token=="syscall") {
                outf << "00000000000000001111111111111111" << endl;
            }
        }
    }

    inf.close();
    outf.close();
}

void simulate(string name) {

    string iname = name + "BINARY.txt";

    ifstream inf;
    inf.open(iname);

    virtualMemory mainMem(inf);
    inf.close();
    RegisterMem rgsMem;

    fillData(name, mainMem);

    Word cInstr;
    string opkd;
    for (int i=0;i<mainMem.size;i++) {

        // i plays the role of program counter.

        cInstr = mainMem.array[i];

        string line = cInstr.value;
        opkd = simOpKind(line);


        cout << "// instruction at address 0x" << dcmlToHex(i*4) << " is being executed ... " << endl;
        if (opkd=="syscall") {
            exeSyscall(rgsMem, mainMem);
        }
        else if (opkd=="Iformat") {
            exeIformat(line, rgsMem);
        }
        else if (opkd=="Lformat") {
            i++;
            exeLi(mainMem.array[i].value, rgsMem);
        }
        else if (opkd=="LSBformat") {
            exeLb(line, rgsMem, mainMem);
        }
        else if (opkd=="SLwformat") {
            exeSLw(line, rgsMem, mainMem);
        }
        else if (opkd=="Rformat") {
            exeRformat(line, rgsMem);
        }
        else if (opkd=="Bformat") {
            int addr = 0;
            if (exeB(line, rgsMem, addr)) {
                i = addr/4-1;
                continue;
            }
        }
        else if (opkd=="pBformat") {
            exeSlt(line, rgsMem);
        }
    }
}
