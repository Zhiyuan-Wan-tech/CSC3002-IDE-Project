#include "Syntax.h"
#include "MiddleToMips.h"
#include <unistd.h>

const int mmax = 3000;

static int mc = 0,
    labelCnt = 0;

static struct Mipscode Mipscodes[mmax];

vector<string> message,
               mscontent;

int locate(string name)
{
    int i;
    for( i = symbolTable.size()-1; i>=0; i--){
        if(symbolTable[i].name == name){
            return i;
        }
    }
    return 0;
}

string newmessage() {
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
    label = "Message" + label;
    return label;
}

void entermips(string mipsop, string mipsrs, string mipsrt, string mipsrd)
{
    Mipscodes[mc].op = mipsop;
    Mipscodes[mc].rs = mipsrs;
    Mipscodes[mc].rt = mipsrt;
    Mipscodes[mc].rd = mipsrd;
    mc += 1;
}


void gen_mips()
{

    int i,
        tmpt;

    /* entermips("addi", "$s0,", "$sp,", "0"); //initial $sp
    entermips("addi","$sp,","$sp,","-80");
    entermips("sw", "","$sp,","0($sp)");
    */



    for(i = 0; i < int(midcode.size()); i++){

        //entermips("\n#" + midcode[i].op, midcode[i].src1, midcode[i].src2, midcode[i].dst + "\n");

        if (midcode[i].op == "PLUS"){

            if(midcode[i].src1[0]>='0' && midcode[i].src1[0]<='9'){
                entermips("li","$t1,",midcode[i].src1,"");
            }
            else entermips("lw","$t1,",midcode[i].src1+"L","");

            if(midcode[i].src2[0]>='0' && midcode[i].src2[0]<='9'){
                entermips("li","$t2,",midcode[i].src2,"");
            }
            else entermips("lw","$t2,",midcode[i].src2+"L","");

            entermips("add","$t3,","$t2,","$t1");
            entermips("sw","$t3,",midcode[i].dst+"L","");
        }

        else if (midcode[i].op == "MINUS"){
            if(midcode[i].src1[0]>='0' && midcode[i].src1[0]<='9'){
                entermips("li","$t1,",midcode[i].src1,"");
            }
            else entermips("lw","$t1,",midcode[i].src1+"L","");

            if(midcode[i].src2[0]>='0' && midcode[i].src2[0]<='9'){
                entermips("li","$t2,",midcode[i].src2,"");
            }
            else entermips("lw","$t2,",midcode[i].src2+"L","");

            entermips("sub","$t3,","$t1,","$t2");
            entermips("sw","$t3,",midcode[i].dst+"L","");
        }

        else if (midcode[i].op == "MULTI"){
            if(midcode[i].src1[0]>='0' && midcode[i].src1[0]<='9'){
                entermips("li","$t1,",midcode[i].src1,"");
            }
            else entermips("lw","$t1,",midcode[i].src1+"L","");

            if(midcode[i].src2[0]>='0' && midcode[i].src2[0]<='9'){
                entermips("li","$t2,",midcode[i].src2,"");
            }
            else entermips("lw","$t2,",midcode[i].src2+"L","");

            entermips("mul","$t3,","$t2,","$t1");
            entermips("sw","$t3,",midcode[i].dst+"L","");
        }

       else if (midcode[i].op == "DIV"){
            if(midcode[i].src1[0]>='0' && midcode[i].src1[0]<='9'){
                entermips("li","$t1,",midcode[i].src1,"");
            }
            else entermips("lw","$t1,",midcode[i].src1+"L","");

            if(midcode[i].src2[0]>='0' && midcode[i].src2[0]<='9'){
                entermips("li","$t2,",midcode[i].src2,"");
            }
            else entermips("lw","$t2,",midcode[i].src2+"L","");

            entermips("div","$t3,","$t1,","$t2");
            entermips("sw","$t3,",midcode[i].dst+"L","");

        }


        else if(midcode[i].op == "LOAD"){//LOAD, c,5,t2 => t2=c[5]
            tmpt = locate(midcode[i].src1);
            if(midcode[i].src2[0]>='0' && midcode[i].src2[0]<='9'){

                if(symbolTable[tmpt].type == "INT"){
                    entermips("lw","$t0,",midcode[i].src1,"");

                    if(midcode[i].src2[0]>='0' && midcode[i].src2[0]<='9'){ //int
                        entermips("li","$t2,",midcode[i].src2,"");
                }

                else entermips("lw","$t2,",midcode[i].src2+"L",""); //int
                entermips("sw", "$t2,","","("+(string)midcode[i].src2+")"+(string)midcode[i].src1+"L");
                }
                else{ //char

                    entermips("la","$t1,",midcode[i].src1+"L","");

                    entermips("lb","$t0,","","("+(string)midcode[i].src2+")"+"$t1");
                }
            }
            else{
                entermips("lw","$t0,","",midcode[i].src2+"L");
                entermips("la","$t2,","",midcode[i].src1+"L");
                entermips("add","$t1,","$t0,","$t2");
                entermips("lb","$t3,","$t1","");
                entermips("la","$t4,","",midcode[i].dst+"L");
                entermips("sb","$t3,","$t4","");


            }


        }

        else if (midcode[i].op == "BEQ"){

            if(midcode[i].src1[0]>='0' && midcode[i].src1[0]<='9'){
                entermips("li","$t1,",midcode[i].src1,"");
            }
            else entermips("lw","$t1,",midcode[i].src1+"L","");

            if(midcode[i].src2[0]>='0' && midcode[i].src2[0]<='9'){
                entermips("li","$t2,",midcode[i].src2,"");
            }
            else entermips("lw","$t2,",midcode[i].src2+"L","");
            entermips("beq", "$t1,", "$t2,", midcode[i].dst);
        }

        else if(midcode[i].op == "BNE"){
            if(midcode[i].src1[0]>='0' && midcode[i].src1[0]<='9'){
                entermips("li","$t1,",midcode[i].src1,"");
            }
            else entermips("lw","$t1,",midcode[i].src1+"L","");

            if(midcode[i].src2[0]>='0' && midcode[i].src2[0]<='9'){
                entermips("li","$t2,",midcode[i].src2,"");
            }
            else entermips("lw","$t2,",midcode[i].src2+"L","");
            entermips("bne", "$t1,", "$t2,", midcode[i].dst);
        }

        else if(midcode[i].op == "B>"){ //B>
            if(midcode[i].src1[0]>='0' && midcode[i].src1[0]<='9'){
                entermips("li","$t1,",midcode[i].src1,"");
            }
            else entermips("lw","$t1,",midcode[i].src1+"L","");

            if(midcode[i].src2[0]>='0' && midcode[i].src2[0]<='9'){
                entermips("li","$t2,",midcode[i].src2,"");
            }
            else entermips("lw","$t2,",midcode[i].src2+"L","");
            entermips("bgt", "$t1,", "$t2,", midcode[i].dst);
        }

        else if(midcode[i].op ==  "B>="){ //B>=
            if(midcode[i].src1[0]>='0' && midcode[i].src1[0]<='9'){
                entermips("li","$t1,",midcode[i].src1,"");
            }
            else entermips("lw","$t1,",midcode[i].src1+"L","");

            if(midcode[i].src2[0]>='0' && midcode[i].src2[0]<='9'){
                entermips("li","$t2,",midcode[i].src2,"");
            }
            else entermips("lw","$t2,",midcode[i].src2+"L","");
            entermips("bge", "$t1,", "$t2,", midcode[i].dst);
        }

        else if(midcode[i].op == "ASSIGN"){

            tmpt =locate(midcode[i].dst);
            if(symbolTable[tmpt].type == "INT"){
                if(midcode[i].src1[0]>='0' && midcode[i].src1[0]<='9'){
                    entermips("li","$t1,",midcode[i].src1,"");
                }
                else entermips("lw","$t1,",midcode[i].src1+"L","");

                if(stoi(midcode[i].src2) > 1){
                    entermips("sw", "$t1,","","("+(string)midcode[i].src2+")"+(string)midcode[i].dst+"L");
                }
                else{
                entermips("sw","$t1,","",midcode[i].dst+"L");
                }
            }
            else{//assign  char
                entermips("la","$t2,","",midcode[i].dst+"L");
                entermips("sb","$t0,","$t2","");
            }

        }

        else if(midcode[i].op == "PUTCHAR"){
            int tmpt_int = int(midcode[i].src1[0]);
            entermips("li","$t0,",to_string(tmpt_int),"");
            entermips("la","$t1,",midcode[i].dst+"L","");
            entermips("sb","$t0,","("+midcode[i].src2+")$t1","");

        }


        else if(midcode[i].op == "J"){
            entermips("b", "", "", midcode[i].dst);
        }

        else if(midcode[i].op == "L"){  //label
            entermips(midcode[i].dst+":","","","");
        }

        else if(midcode[i].op == "INPUT"){
            tmpt = locate(midcode[i].src1);
            if(symbolTable[tmpt].type == "INT" ){
                entermips("li","$v0,","5","");
                entermips("syscall","","","");
                entermips("move","$t0,","$v0,","");
                entermips("sw","$t0,","",midcode[i].dst+"L");

            }
            else if(symbolTable[tmpt].type == "CHAR"){
                entermips("la","$a0,","",midcode[i].dst+"L");
                entermips("li","$v0,","8","");
                entermips("syscall","","","");
            }

            }
        else if(midcode[i].op == "OUTPUT"){
            if(midcode[i].src2 == "INT"){
                entermips("lw","$a0,","",midcode[i].src1+"L");
                entermips("li","$v0,","1","");//print integer
            }
            else if(midcode[i].src2 == "STR"){
                string tmpt_mes;
                tmpt_mes = newmessage();
                message.push_back(tmpt_mes);
                mscontent.push_back(midcode[i].src1);
                entermips("la","$a0","",tmpt_mes);
                entermips("li","$v0,","4","");//print string
            }
            else if(midcode[i].src2 == "CHAR"){
                //tmpt = locate(midcode[i].src1);
                entermips("la","$a0,","",midcode[i].src1+"L");
                entermips("li","$v0,","4","");
            }


            entermips("syscall","","","");
        }

    }
    entermips("li","$v0,","10","");
    entermips("syscall","","","");
}

void print_mips()
{
    gen_mips();

    char* path;
    path = getcwd(NULL, 0);
    string newpath = path;
    newpath = newpath.substr(0,newpath.find("Project")+7);
    free(path);

    string strt = newpath+"/Files/arithemetic" + ".asm";
    ofstream fout(strt);
    if(fout){
        fout<<".text"<<endl;

        for (int i = 0; i < mc; i++) {
            fout << Mipscodes[i].op << " " << Mipscodes[i].rs << " " << Mipscodes[i].rt << " " << Mipscodes[i].rd << endl;
        }

        fout << ".data"<<endl;

        for(int i = 0; i < int(symbolTable.size()); i++){
            if(symbolTable[i].array_length == 1 && symbolTable[i].type == "INT" ){
                fout<<symbolTable[i].name<<"L: "<<".space\t4"<<endl;
            }
            else if(symbolTable[i].array_length > 1 && symbolTable[i].type != "CHAR"){
                fout<<symbolTable[i].name<<"L: "<<".space\t"<< symbolTable[i].array_length*4 << endl;
            }
            else if(symbolTable[i].type == "CHAR"){
                fout<<symbolTable[i].name<<"L: "<<".byte\t"<< symbolTable[i].array_length<< endl;
            }
            else{
                fout<<"no variable."<<endl;
            }
        }
        for(int i = 0; i < int(message.size()); i ++){
            fout<<message[i]<<": .asciiz\t"<<"\""<<mscontent[i]<<"\""<<endl;
        }

        fout.close();

    }

}
