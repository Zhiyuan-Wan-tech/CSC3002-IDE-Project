#include<string>
#include<iostream>
#include<fstream>
#include"Lexical.h"
#include <unistd.h>

using namespace std;



char lexical:: getChar() {
        char a;
        a = fin.get();
        return a;
    }

void lexical::lexical_analysis() {
    char ch = getChar();
    int cur_line = 1;
    while (ch != EOF) {
        while (ch == ' ' || ch == '\n' || ch == '\t') {
            if (ch == '\n') cur_line++;  //count the line
            ch = getChar();
        }
        string temp = "";
        if (ch == '_' || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) { //ID or reserved words
            temp += ch;
            ch = getChar();
            while (ch == '_' || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
                temp += ch;
                ch = getChar();
            }
            token_list[iter].ID_name = temp;
            //check reserved words (also can implemented by checking a list if the amount of reserved words is large )
            if (temp == "for") token_list[iter].type = "FOR";
            else if (temp == "if") token_list[iter].type = "IF";
            else if (temp == "while") token_list[iter].type = "WHILE";
            else if (temp == "and") token_list[iter].type = "AND";
            else if (temp == "or") token_list[iter].type = "OR";
            else if (temp == "not") token_list[iter].type = "NOT";
            else if (temp == "else") token_list[iter].type = "ELSE";
            else if (temp == "main") token_list[iter].type = "MAIN";
            else if (temp == "int") token_list[iter].type = "INT";
            else if (temp == "char") token_list[iter].type = "CHAR";
            else if (temp == "input") token_list[iter].type = "INPUT";
            else if (temp == "output") token_list[iter].type = "OUTPUT";
            else token_list[iter].type = "ID";
            token_list[iter].line = cur_line;
            iter++;
        }

        else if (ch >= '0' && ch <= '9') { //numbers
            temp += ch;
            ch = getChar();
            while (ch >= '0' && ch <= '9') {
                temp += ch;
                ch = getChar();
            }
            token_list[iter].ID_name = temp;
            token_list[iter].type = "NUM";
            token_list[iter].line = cur_line;
            iter++;
        }
        else if (ch == '(') {
            token_list[iter].ID_name = '(';
            token_list[iter].type = "LPAR";
            token_list[iter].line = cur_line;
            iter++;
            ch = getChar();
        }
        else if (ch == ')') {
            token_list[iter].ID_name = ')';
            token_list[iter].type = "RPAR";
            token_list[iter].line = cur_line;
            iter++;
            ch = getChar();
        }
        else if (ch == '{') {
            token_list[iter].ID_name = '{';
            token_list[iter].type = "LBRA";
            token_list[iter].line = cur_line;
            iter++;
            ch = getChar();
        }
        else if (ch == '}') {
            token_list[iter].ID_name = "}";
            token_list[iter].type = "RBRA";
            token_list[iter].line = cur_line;
            iter++;
            ch = getChar();
        }
        else if (ch == '[') {
            token_list[iter].ID_name = "[";
            token_list[iter].type = "LBRACK";
            token_list[iter].line = cur_line;
            iter++;
            ch = getChar();
        }
        else if (ch == ']') {
            token_list[iter].ID_name = "]";
            token_list[iter].type = "RBRACK";
            token_list[iter].line = cur_line;
            iter++;
            ch = getChar();
        }
        else if (ch == '=') {
            temp += ch;
            ch = getChar();
            if (ch == '=') {
                token_list[iter].ID_name = "==";
                token_list[iter].type = "EQ";
                token_list[iter].line = cur_line;
                ch = getChar();
                iter++;
            }
            else {
                token_list[iter].ID_name = "=";
                token_list[iter].type = "ASSI";
                token_list[iter].line = cur_line;
                iter++;
            }
        }
        else if (ch == '>') {
            ch = getChar();
            if (ch == '=') {
                token_list[iter].ID_name = ">=";
                token_list[iter].type = "GEQ";
                token_list[iter].line = cur_line;
                ch = getChar();
                iter++;
            }
            else {
                token_list[iter].ID_name = ">";
                token_list[iter].type = "GRE";
                token_list[iter].line = cur_line;
                iter++;
            }
        }
        else if (ch == '<') {
            ch = getChar();
            if (ch == '=') {
                token_list[iter].ID_name = "<=";
                token_list[iter].type = "LEQ";
                token_list[iter].line = cur_line;
                ch = getChar();
                iter++;
            }
            else {
                token_list[iter].ID_name = "<";
                token_list[iter].type = "LES";
                token_list[iter].line = cur_line;
                iter++;
            }
        }
        else if (ch == '!') {
        ch = getChar();
        if (ch == '=') {
            token_list[iter].ID_name = "!=";
            token_list[iter].type = "NEQ";
            token_list[iter].line = cur_line;
            ch = getChar();
            iter++;
        }
        else {
            token_list[iter].ID_name = "<";
            token_list[iter].type = "LES";
            token_list[iter].line = cur_line;
            iter++;
        }
        }
        else if (ch == '\'') {
            ch = getChar();
            token_list[iter].ID_name = ch;
            token_list[iter].type = "CHAR";
            token_list[iter].line = cur_line;
            iter++;
            ch = getChar();
            if (ch != '\'') {
                cout << "lack of ' in char def" << endl;
            }
            ch = getChar();
        }
        else if (ch == '\"') {
            string str;
            ch = getChar();
            while (ch != '\"') {
                str += ch;
                ch = getChar();
            }
            token_list[iter].ID_name = str;
            token_list[iter].type = "STR";
            token_list[iter].line = cur_line;
            iter++;
            ch = getChar();
        }
        else if (ch == ';') {
            token_list[iter].ID_name = ';';
            token_list[iter].type = "SCOL";
            token_list[iter].line = cur_line;
            ch = getChar();
            iter++;
        }
        else if (ch == ',') {
            token_list[iter].ID_name = ',';
            token_list[iter].type = "COMMA";
            token_list[iter].line = cur_line;
            ch = getChar();
            iter++;
        }
        else if (ch == '+') {
            token_list[iter].ID_name = '+';
            token_list[iter].type = "PLUS";
            token_list[iter].line = cur_line;
            ch = getChar();
            iter++;
        }
        else if (ch == '-') {
            token_list[iter].ID_name = '-';
            token_list[iter].type = "MINUS";
            token_list[iter].line = cur_line;
            ch = getChar();
            iter++;
        }
        else if (ch == '*') {
            token_list[iter].ID_name = '*';
            token_list[iter].type = "TIMES";
            token_list[iter].line = cur_line;
            ch = getChar();
            iter++;
        }
        else if (ch == '/') {
            token_list[iter].ID_name = '/';
            token_list[iter].type = "DIV";
            token_list[iter].line = cur_line;
            ch = getChar();
            iter++;
        }
        else {
            return; //error
        }
    }
}
void lexical:: init() {
    char* path;
    path = getcwd(NULL, 0);
    string newpath = path;
    newpath = newpath.substr(0,newpath.find("Project")+7);
    free(path);

    string strt = newpath+"/Files/arithemetic" + "T.txt";
        fin.open(strt);
    }



