#ifndef _lexical_h
#define _lexical_h

#include "PathName.h"
#include<fstream>
#include<string>
using namespace std;
const int MAXLEN = 1000;

struct token {
    string ID_name;
    string type;
    int line;
};

class lexical {
public:
    token token_list[MAXLEN];
    int iter = 0;
    char getChar();
    void lexical_analysis();
    void init();
private:
    ifstream fin;
};

#endif
