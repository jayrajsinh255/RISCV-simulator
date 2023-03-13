/* 

The project is developed as part of Computer Architecture class
Project Name: Functional Simulator for subset of RISCV Processor

Developer's Name:
Developer's Email id:
Date: 

*/


/* myRISCVSim.cpp
   Purpose of this file: implementation file for myRISCVSim
*/
#include "builtin_funcs.hpp"
#include "self_defined_funcs.hpp"
#include "myRISCVSim.hpp"

void fetch();
void decode();
void mA();
void write_back();

void run_riscvsim() {
    EXIT=false;
    while(1) {
        fetch();
        decode();
        if(EXIT){
            EXIT=false;
            printf("ORIGINAL\n");
            unsigned int addr=0x10001000;
            for(int i=0;i<10;i++){
                printf("%d. %d\n",i+1,(unsigned int)memory_read(addr,4));
                addr+=4;
            }
            printf("SORTED\n");
            addr=0x10002000;
            for(int i=0;i<10;i++){
                printf("%d. %d\n",i+1,(unsigned int)memory_read(addr,4));
                addr+=4;
            }
            return;
        }
        execute();
        mA();
        write_back();
    }
}

// it is used to set the reset values
//reset all registers and memory content to 0
void reset_proc() {

}

//load_program_memory reads the input memory, and pupulates the instruction 
// memory
void load_program_memory(char *file_name) {

}

// //reads from the instruction memory and updates the instruction register
void fetch()
{
    if (mycontrol_unit.isBranchTaken)
    {
        PC = branchPC;
    }
    else
    {
        PC = nextPC;
    }
    nextPC = PC + 4;
    // printf("PC=%x\n",PC);
    unsigned int instruct_dec = (unsigned int)memory_read((unsigned int)PC, 4);
    // printf("%x ##\n",instruct_dec);//
    string instruction = dec2bin(instruct_dec);
    if_de_rest.instruction = instruction;
    // cout<<if_de_rest.instruction<<" ##"<<endl;////
}
// //reads the instruction register, reads operand1, operand2 fromo register file, decides the operation to be performed in execute stage
void decode(){

}


// //executes the ALU operation based on ALUop
void execute(){
    long long int alu_result;
    alu_result=alu_unit(mycontrol_unit.aluSignal);
    // printf("%d alu_result\n",alu_result);//
    if(mycontrol_unit.branchSelect==0){
        //not jalr type
        branchPC=de_ex_rest.branch_target+PC;
    }
    else if(mycontrol_unit.branchSelect==1){
        //if jalr then pc
        branchPC=alu_result;
    }
    if(mycontrol_unit.branchSignal=="nbr"){
        mycontrol_unit.setIsBranchTaken(false);
    }
    else if(mycontrol_unit.branchSignal=="ubr"){
        mycontrol_unit.setIsBranchTaken(true);
    }
    else{
        if(mycontrol_unit.branchSignal=="beq"){
            if(alu_result==0){
                mycontrol_unit.setIsBranchTaken(true);
            }
            else{
                mycontrol_unit.setIsBranchTaken(false); 
            }   
        }
        else if(mycontrol_unit.branchSignal=="bne"){
            if(alu_result!=0){
                mycontrol_unit.setIsBranchTaken(true);
            }
            else{
                mycontrol_unit.setIsBranchTaken(false); 
            }   
        }
        else if(mycontrol_unit.branchSignal=="blt"){
            if(alu_result<0){
                mycontrol_unit.setIsBranchTaken(true);
            }
            else{
                mycontrol_unit.setIsBranchTaken(false); 
            }   
        }
        else if(mycontrol_unit.branchSignal=="bge"){
            printf("Bge ##");//
            if(alu_result>=0){
                mycontrol_unit.setIsBranchTaken(true);
            }
            else{
                mycontrol_unit.setIsBranchTaken(false); 
            }   
        }    
    }
    ex_ma_rest.alu_result=alu_result;
    ex_ma_rest.op2=(unsigned int) de_ex_rest.op2;
    ex_ma_rest.rd=(unsigned int) de_ex_rest.rd;
     printf("alu result :%u \n",ex_ma_rest.alu_result);//
     printf("op2 : %u\n",ex_ma_rest.op2);//
     printf("rd :%u\n",ex_ma_rest.rd);//
}

// //perform the memory operation
void mA() {
    unsigned int ldResult=0;
    char my_char;
    short int my_short_int;
    int my_int;

    //load operation
    if(mycontrol_unit.isLd){
        if(mycontrol_unit.nBytes==1){
            my_char=(char)memory_read((unsigned int)ex_ma_rest.alu_result,1);
            my_int=(int)my_char;
            ldResult=(unsigned int)my_int;
        }
        else if(mycontrol_unit.nBytes==2){
            my_short_int=(short int)memory_read((unsigned int)ex_ma_rest.alu_result,2);
            my_int=(int)my_short_int;
            ldResult=(unsigned int)my_int;
        }
        else if(mycontrol_unit.nBytes==4){
            ldResult=(int)memory_read((unsigned int)ex_ma_rest.alu_result,4);
        }
        else{
            cout<<"nBytes is "<<mycontrol_unit.nBytes<<"not supported"<<endl;
        }
    }
    else{
        ldResult=0;
    }

    //store operation
    if(mycontrol_unit.isSt){
        if(mycontrol_unit.nBytes==1){
            memory_write((unsigned int)ex_ma_rest.alu_result,(unsigned long long int) ex_ma_rest.op2,1);
        }
        else if(mycontrol_unit.nBytes==2){
            memory_write((unsigned int)ex_ma_rest.alu_result,(unsigned long long int) ex_ma_rest.op2,2);
        }
        else if(mycontrol_unit.nBytes==4){
            memory_write((unsigned int)ex_ma_rest.alu_result,(unsigned long long int) ex_ma_rest.op2,4);
        }
        else{
            cout<<"nBytes is "<<mycontrol_unit.nBytes<<"not supported"<<endl;
        }   
    }
    ma_wb_rest.alu_result=ex_ma_rest.alu_result;
    ma_wb_rest.ld_result=ldResult;
    ma_wb_rest.rd=ex_ma_rest.rd;
}

// //writes the results back to register file
void write_back() {

}