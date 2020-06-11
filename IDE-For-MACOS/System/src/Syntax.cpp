#include<iostream>
#include<string>
#include<vector>
#include"Lexical.h"
#include"Syntax.h"
using namespace std;

lexical lex;
int iter = -1;
int cur_line;
string cur_type, cur_IDname;
int cur_offset;
string last_temp_var;

IR temp_midcode;
vector<IR> midcode;
vector<Symbol> symbolTable;



struct SemInfo {
    string name;
    string type;
    string true_out;
    string false_out;
}temp_info;

//
void add_symbol(string name, string type, int add, int value, int array_len = 1);
string newTempVar(string type = "INT");
//
void variable_declaration();
void multi_var_declaration(string var_type);
//
SemInfo expression();
SemInfo term();
SemInfo factor();
//
void assign(string var_name, string var_type);
//
void blockSentence();
void sentence();
//
void ifSentence();
void orLogic(string Bt, string Bf);
void andLogic(string Bt, string Bf);
//
void whileSentence(string Bf);
void forSentence();
void inputSentence();
void outputSentence();
//
void error(int id, int line) {
    if (id == -1) cout << "an error in line:" << line << endl;
    if (id == 1) cout << "lack of } in line:" << line << endl;
    if (id == 2) cout << "lack of ) in line:" << line << endl;
    if (id == 3) cout << "lack of ] in line:" << line << endl;
    if (id == 4) cout << "the variable has been defined ,line:" << line << endl;
    if (id == 5) cout << "out of boundary in line:" << line << endl;
    if (id == 6) cout << "variable undefined in line:" << line << endl;
    if (id == 7) cout << "unmatched variable type in line:" << line << endl;
    if (id == 8) cout << "lack of ; in line:" << line << endl;
    if (id == 9) cout << "lack of ( in while , line" << line << endl;
}

int temp_var_cnt = 1;

string newTempVar(string type) {
    string var = "";
    int temp;
    temp = temp_var_cnt;
    temp_var_cnt++;
    while (temp != 0) {
        char c;
        c = (temp % 10) + '0';
        temp /= 10;
        var = c + var;
    }
    var = 't' + var;
    last_temp_var = var;
    add_symbol(var, type, cur_offset, 0);
    cur_offset += 4;
    return var;
}

int labelCnt = 0;
string last_label;
string newLabel() {
    string label = "";
    int temp;
    labelCnt++;
    temp = labelCnt;
    while (temp != 0) {
        char c;
        c = (temp % 10) + '0';
        temp /= 10;
        label = c + label;
    }
    label = 'L' + label;
    last_label = label;
    return label;
}

int temp_mes_cnt = 1;
string newMes() {
    string mes = "";
    int temp;
    temp_mes_cnt++;
    temp = temp_mes_cnt;
    while (temp != 0) {
        char c;
        c = (temp % 10) + '0';
        temp /= 10;
        mes = c + mes;
    }
    mes = "MES" + mes;
    return mes;
}

void add_symbol(string name, string type, int add, int value, int array_len) {
    Symbol temp_sym;
    temp_sym.name = name;
    temp_sym.type = type;
    temp_sym.address = add;
    temp_sym.value = value;
    temp_sym.array_length = array_len;
    bool flag = true;
    for (int i = 0; i < int(symbolTable.size()); i++) {
        if (symbolTable[i].name == name) {
            error(4, cur_line);
            flag = false;
        }
    }
    if (flag) symbolTable.push_back(temp_sym);
}

bool check_var(string name, string &type, int array_len = 1) {
    bool flag = false;
    for (int i = 0; i < int(symbolTable.size()); i++) {
        if (symbolTable[i].name == name) {
            if (symbolTable[i].array_length > 1 && array_len > symbolTable[i].array_length) { // it is an array and out of boundary
                error(5, cur_line);
            }
            flag = true;
            type = symbolTable[i].type;
        }
    }
    return flag;
}

int find_var_len(string name,string modi = "") {
    int length = -1;
    for (int i = 0; i < int(symbolTable.size()); i++) {
        if (symbolTable[i].name == name) {
            length = symbolTable[i].array_length;
            symbolTable[i].mes = modi;
            break;
        }
    }
    return length;
}

bool eat(string type, bool withoutCheck = false) {
    iter++;
    cur_type = lex.token_list[iter].type;
    cur_IDname = lex.token_list[iter].ID_name;
    cur_line = lex.token_list[iter].line;
    if (cur_type != type && (!withoutCheck)) {
        iter--;
        return false;
    }
    else return true;
}


void addMidcode(string op, string src1, string src2, string dst) {
    temp_midcode.op = op;
    temp_midcode.src1 = src1;
    temp_midcode.src2 = src2;
    temp_midcode.dst = dst;
    midcode.push_back(temp_midcode);
}

//variable declaration part

void variable_declaration() {
    int temp_iter = iter;
    if (eat("INT") || eat("CHAR")) {
        string var_type = cur_type;
        string var_name;
        int var_len = 1;
        int temp_num = 0;
        if (iter - temp_iter == 2) error(-1, cur_line);
        if (!eat("ID")) error(-1, cur_line);
        var_name = cur_IDname;
        if (eat("LBRACK")) { //it's an array
            if (!eat("NUM")) error(-1, cur_line);
            temp_num = stoi(cur_IDname);
            var_len = temp_num;
            if (!eat("RBRACK")) error(3, cur_line);
        }
        add_symbol(var_name, var_type, cur_offset, 0, var_len);
        cur_offset += 4 * var_len;
        //add midcode (int/char,array_length, ,var_name)
        addMidcode(var_type, to_string(var_len), "", var_name);

        multi_var_declaration(var_type);
    }
    else error(-1, cur_line);

}

void multi_var_declaration(string var_type) {
    if (eat("SCOL")) return;
    else if (eat("COMMA")) {
        if (eat("ID")) {
            string var_name = cur_IDname;
            int var_length = 1;
            if (eat("LBRACK")) { //it's an array
                if (!eat("NUM")) error(-1, cur_line);

                var_length = stoi(cur_IDname);
                if (!eat("RBRACK")) error(3, cur_line);
            }
            add_symbol(var_name, var_type, cur_offset, 0, var_length);
            cur_offset += 4 * var_length;
            //add midcode (int/char,array_length, ,var_name)

            addMidcode(var_type, to_string(var_length), "", var_name);
            multi_var_declaration(var_type);
        }
        else error(-1, cur_line);
    }
}


SemInfo expression() {
    SemInfo term_info, exp_info;
    if (eat("PLUS") || eat("MINUS")) {
        if (cur_type == "MINUS") {
            term_info = term();
            temp_midcode.op = "MINUS";
            temp_midcode.src1 = "0";
            temp_midcode.src2 = term_info.name;
            temp_midcode.dst = newTempVar();
            exp_info.name = temp_midcode.dst;
            midcode.push_back(temp_midcode);
        }
    }
    term_info = term();
    exp_info.name = term_info.name;
    exp_info.type = term_info.type;

    while (eat("PLUS") || eat("MINUS")) {
        if (cur_type == "PLUS") {
            SemInfo new_term;
            new_term = term();
            temp_midcode.op = "PLUS";
            temp_midcode.src1 = exp_info.name;
            temp_midcode.src2 = new_term.name;
            exp_info.name = newTempVar();
            temp_midcode.dst = exp_info.name;
            midcode.push_back(temp_midcode);
            continue;
        }
        if (cur_type == "MINUS") {
            SemInfo new_term;
            new_term = term();
            temp_midcode.op = "MINUS";
            temp_midcode.src1 = exp_info.name;
            temp_midcode.src2 = new_term.name;
            exp_info.name = newTempVar();
            temp_midcode.dst = exp_info.name;
            midcode.push_back(temp_midcode);
            continue;
        }
    }
    return exp_info;
}

SemInfo term() {
    SemInfo factorInfo;
    factorInfo = factor();
    while (eat("TIMES") || eat("DIV")) {
        if (cur_type == "TIMES") {
            SemInfo factorInfo2;
            string temp_var;
            factorInfo2 = factor();
            temp_var = newTempVar();
            temp_midcode.op = "MULTI";
            temp_midcode.src1 = factorInfo.name;
            temp_midcode.src2 = factorInfo2.name;
            temp_midcode.dst = temp_var;
            factorInfo.name = temp_midcode.dst;
            midcode.push_back(temp_midcode);
        }
        if (cur_type == "DIV") {
            SemInfo factorInfo2;
            string temp_var;
            factorInfo2 = factor();
            temp_var = newTempVar();
            temp_midcode.op = "DIV";
            temp_midcode.src1 = factorInfo.name;
            temp_midcode.src2 = factorInfo2.name;
            temp_midcode.dst = temp_var;
            factorInfo.name = temp_midcode.dst;
            midcode.push_back(temp_midcode);
        }
    }

    return factorInfo;

}

SemInfo factor() {
    string factor_type;
    if (eat("ID")) {
        string var_name = cur_IDname;
        string new_var_name;
        //if(eat("LPAR")   hold for function call
        if (eat("LBRACK")) { //it is an array
            SemInfo array_info;
            array_info = expression();
            if (!check_var(var_name, factor_type)) error(6, cur_line); //need array boundary check
            new_var_name = newTempVar();
            addMidcode("LOAD", var_name, array_info.name, new_var_name);
            var_name = new_var_name;
            if (!eat("RBRACK")) error(3, cur_line);
        }
        else { // it is not an array
            if (!check_var(var_name, factor_type)) error(6, cur_line);
        }
        temp_info.name = var_name;
        temp_info.type = factor_type;
        return temp_info;
    }
    if (eat("LPAR")) { // (<expression>)
        SemInfo temp_exp;
        temp_exp = expression();
        if (!eat("RPAR")) error(2, cur_line);
        return temp_exp;
    }

    if (eat("NUM")) {
        temp_info.name = cur_IDname;
        temp_info.type = "INT";
        return temp_info;
    }

    if (eat("CHAR")) {
        temp_info.name = cur_IDname;
        temp_info.type = "CHAR";
        return temp_info;
    }

    temp_info.name = "NULL";
    return temp_info;

}

void assign(string var_name, string var_type) {
    SemInfo index_info, exp_info;
    string Ltype = var_type, assign_len = "0";
    bool strAssi = false;//whether it's c = "XXX"
    if (eat("LBRACK")) { // L-value is an array
        index_info = expression();
        if (index_info.name[index_info.name.size() - 1] >= '0' && index_info.name[index_info.name.size() - 1] <= '9') {
            check_var(var_name, Ltype, stoi(index_info.name) + 1);
            //if (Ltype != var_type) error(7, cur_line);
        }
        if (!eat("RBRACK")) error(3, cur_line);
        assign_len = index_info.name;
    }
    check_var(var_name, Ltype);
    if (eat("ASSI")) {
        if (Ltype == "CHAR" && find_var_len(var_name) > 1) {//string assign
            if (!eat("STR")) error(-1, cur_line);
            int str_offset = 0,assi_len;
            string temp_mes = newMes();
            addMidcode("STRING", cur_IDname, "", temp_mes);
            assi_len = find_var_len(var_name, temp_mes);
            if (assi_len > int(cur_IDname.length())) assi_len = cur_IDname.length();
            for (int i = 0; i < assi_len; i++) {
                string cur_char = "",cur_str_offset = "";
                int temp_i = i;
                cur_char = cur_IDname[i];
                if (temp_i == 0) cur_str_offset = "0";
                while (temp_i != 0) {
                    char dig;
                    dig = temp_i % 10 + '0';
                    cur_str_offset = dig + cur_str_offset;
                    temp_i /= 10;
                }
                addMidcode("PUTCHAR", cur_char, cur_str_offset, var_name);
            }

        }
        else{
            exp_info = expression();
            if (Ltype == "CHAR") {
                addMidcode("ASSIGN", "'" + exp_info.name + "'", assign_len, var_name);
            }
            else {
                addMidcode("ASSIGN", exp_info.name, assign_len, var_name);
            }
        }
    }

}


void blockSentence() {
    while (eat("ID") || eat("IF") || eat("CHAR") || eat("INT") || eat("WHILE") || eat("FOR") || eat("INPUT") || eat("OUTPUT")) {
        sentence();
    }
}

void sentence() {
    if (cur_type == "ID") { //assignment sentence
        assign(cur_IDname, cur_type);
        if (!eat("SCOL")) error(8, cur_line);
        return;
    }
    if (cur_type == "IF") {// if sentence
        ifSentence();
        return;
    }
    if (cur_type == "CHAR" || cur_type == "INT") {
        iter--;
        variable_declaration();
        return;
    }
    if (cur_type == "WHILE") {
        string Bf = newLabel();
        whileSentence(Bf);
        addMidcode("L", "", "", Bf);
        return;
    }
    if (cur_type == "FOR") {
        forSentence();
        return;
    }
    if (cur_type == "INPUT") {
        inputSentence();
        return;
    }
    if (cur_type == "OUTPUT") {
        outputSentence();
        return;
    }
}


void andLogic(string Bt, string Bf) {
    SemInfo e1, e2;
    string B1true;
    bool LogicDone = false;
    int rel;

    if (eat("NOT")) {
        andLogic(Bf, Bt);
        B1true = Bf;
        LogicDone = true;
    }
    if (eat("LPAR")) {
        orLogic(Bt, Bf);
        if (!eat("RPAR")) error(2, cur_line);
        LogicDone = true;
        B1true = Bt;
    }
    if (!LogicDone) { //current logic is not complex logic, simply A rel B
        e1 = expression();
        if (eat("EQ")) { // ==
            rel = 1;
        }
        else if (eat("GEQ")) {// >=
            rel = 2;
        }
        else if (eat("GRE")) { //>
            rel = 3;
        }
        else if (eat("LEQ")) { //<=
            rel = 4;
        }
        else if (eat("LES")) { // <
            rel = 5;
        }
        else if (eat("NEQ")) {//!=
            rel = 6;
        }
        e2 = expression();
    }
    if (eat("AND")) { //connected with and : B1 && B2
        if (!LogicDone) { // B1 ==  A rel B
            B1true = newLabel();
            if (rel == 1) {
                addMidcode("BEQ", e1.name, e2.name, B1true);
            }
            else if (rel == 2) {
                addMidcode("B>=", e1.name, e2.name, B1true);
            }
            else if (rel == 3) {
                addMidcode("B>", e1.name, e2.name, B1true);
            }
            else if (rel == 4) {
                addMidcode("B>=", e2.name, e1.name, B1true);
            }
            else if (rel == 5) {
                addMidcode("B>", e2.name, e1.name, B1true);
            }
            else if (rel == 6) {
                addMidcode("BNE", e2.name, e1.name, B1true);
            }
            addMidcode("J", "", "", Bf);
            addMidcode("L", "", "", B1true); //set label
        }
        else { // B1 is ( logic) or  not B
            addMidcode("L", "", "", B1true);
        }
        andLogic(Bt, Bf); // compute B2
    }
    else {//single  and condition
        if (!LogicDone) {

            if (rel == 1) {
                addMidcode("BEQ", e1.name, e2.name, Bt);
            }
            else if (rel == 2) {
                addMidcode("B>=", e1.name, e2.name, Bt);
            }
            else if (rel == 3) {
                addMidcode("B>", e1.name, e2.name, Bt);
            }
            else if (rel == 4) {
                addMidcode("B>=", e2.name, e1.name, Bt);
            }
            else if (rel == 5) {
                addMidcode("B>", e2.name, e1.name, Bt);
            }
            else if (rel == 6) {
                addMidcode("BNE", e2.name, e1.name, Bt);
            }

            addMidcode("J", "", "", Bf);
        }
    }
}

void orLogic(string Bt, string Bf) {
    string B1t, B1f, B2t, B2f;
    int cur_mid = midcode.size() - 1;
    B1t = Bt;
    B1f = newLabel();
    andLogic(B1t, B1f);
    if (eat("OR")) { //connected with or
        addMidcode("L", "", "", B1f);
        B2t = Bt;
        B2f = Bf;
        orLogic(B2t, B2f);
    }
    else {
        for (int i = cur_mid; i <= int(midcode.size()) - 1; i++) {
            if (midcode[i].dst == B1f) midcode[i].dst = Bf;
        }
    }
}

void ifSentence() {
    string Bt = newLabel();
    string Bf = newLabel();
    string Bexit = newLabel();
    if (eat("LPAR")) {
        orLogic(Bt, Bf);
        if (!eat("RPAR")) error(2, cur_line);
        if (eat("LBRA")) { // if (condition) {}
            addMidcode("L", "", "", Bt); //set the true out
            blockSentence();
            addMidcode("J", "", "", Bexit);
            if (!eat("RBRA")) error(1, cur_line);
        }
        else { //single sentence
            addMidcode("L", "", "", Bt);
            eat("", true);
            sentence();
            addMidcode("J", "", "", Bexit);
        }
        if (eat("ELSE")) {
            if (eat("LBRA")) { // else {}
                addMidcode("L", "", "", Bf); //set the false out
                blockSentence();
                if (!eat("RBRA")) error(1, cur_line);
            }
            else { //single sentence
                addMidcode("L", "", "", Bf);
                eat("", true);
                sentence();
            }
        }
        else { //no else sentence
            addMidcode("L", "", "", Bf);
        }
        addMidcode("L", "", "", Bexit);
    }
}

void whileSentence(string Bf) {
    string Lbegin = newLabel();
    string Bt;
    Bt = newLabel();
    if (eat("LPAR")) {
        addMidcode("L", "", "", Lbegin);
        orLogic(Bt, Bf);
        addMidcode("L", "", "", Bt);
        if (!eat("RPAR")) error(2, cur_line);
        if (eat("LBRA")) { //while(condition){...}
            blockSentence();
            if (!eat("RBRA")) error(1, cur_line);
        }
        else {
            eat("", true);
            sentence();
        }
        addMidcode("J", "", "", Lbegin);
    }
    else {
        error(9, cur_line);
    }
}

void forSentence() {
    if (eat("LPAR")) {
        string var_name, var_type;
        SemInfo init, step;
        string true_out = newLabel(), false_out = newLabel(), begin_label = newLabel();
        //initialize
        if (!eat("ID")) error(-1, cur_line);
        var_name = cur_IDname;
        check_var(var_name, var_type);
        if (var_type != "INT") error(-1, cur_line);
        if (!eat("ASSI")) error(-1, cur_line);
        init = expression();
        if (init.type != "INT") error(-1, cur_line);
        addMidcode("ASSIGN", init.name, "0", var_name);
        if (!eat("SCOL")) error(-1, cur_line);
        //condition
        addMidcode("L", "", "", begin_label);
        orLogic(true_out, false_out);
        if (!eat("SCOL")) error(-1, cur_line);
        addMidcode("L", "", "", true_out);
        //step
        int step_dir = 1; //1 for +, 0 for -
        string step_len;
        if (!eat("ID")) error(-1, cur_line);
        var_name = cur_IDname;
        check_var(var_name, var_type);
        if (var_type != "INT") error(-1, cur_line);
        if (!eat("ASSI")) error(-1, cur_line);
        if (!eat("ID")) error(-1, cur_line);
        if (eat("PLUS") || eat("MINUS")) {
            if (cur_type == "PLUS") step_dir = 1;
            else step_dir = 0;
        }
        else error(-1, cur_line);
        if (eat("NUM")) {
            step_len = cur_IDname;
        }
        else error(-1, cur_line);
        if (!eat("RPAR")) error(2, cur_line);


        // sentence in for loop
        if (eat("LBRA")) { //block
            blockSentence();
            if (!eat("RBRA")) error(1, cur_line);
        }
        else {
            eat("", true);
            sentence();
        }
        string iter_temp = newTempVar();
        if (step_dir == 1) addMidcode("PLUS", var_name, step_len, iter_temp);
        else addMidcode("MINUS", var_name, step_len, iter_temp);
        addMidcode("ASSIGN", iter_temp, "1", var_name);
        addMidcode("J", "", "", begin_label);
        addMidcode("L", "", "", false_out);
    }
    else {
        error(9, cur_line);
    }
}

void inputSentence() {
    string var_name, var_type;
    if (!eat("LPAR")) error(9, cur_line);
    if (!eat("ID")) error(-1, cur_line);
    var_name = cur_IDname;
    if (!check_var(var_name, var_type)) error(6, cur_line);
    addMidcode("INPUT", "", "", var_name);
    while (eat("COMMA")) {
        if (!eat("ID")) error(-1, cur_line);
        var_name = cur_IDname;
        if (!check_var(var_name, var_type)) error(6, cur_line);
        addMidcode("INPUT", "", "", var_name);
    }
    if (!eat("RPAR")) error(2, cur_line);
    if (!eat("SCOL")) error(8, cur_line);
}

void outputSentence() {
    string var_name, var_type;
    if (!eat("LPAR")) error(9, cur_line);
    if (eat("ID")) {
        var_name = cur_IDname;
        if (!check_var(var_name, var_type)) error(6, cur_line);
        addMidcode("OUTPUT", var_name, var_type, "");
    }
    if (eat("STR")) {
        addMidcode("OUTPUT", cur_IDname, "STR", "");
    }
    if (!eat("RPAR")) error(2, cur_line);
    if (!eat("SCOL")) error(8, cur_line);
}


void whole() {
    if (eat("MAIN")) {
        eat("LBRA");
        blockSentence();
        if (!eat("RBRA")) error(1, cur_line);
    }
}

void test() {
    lex.init();
    lex.lexical_analysis();
    whole();
}

void print_token() {
    for (int i = 0; i <= 29; i++) {
        cout << lex.token_list[i].type << "   ";
    }
}
