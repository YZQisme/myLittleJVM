//
// Created by YZQ on 25-7-3.
//
#pragma once
#include <iostream>
#include <cstdint>
#include <stack>
#include "../BaseStruct/Slots.h"
#ifndef STACK_H
#define STACK_H

#endif //STACK_H
#define MAX_STACKFRAME_NUM 2048
#define MAX_allOperandStack_SIZE (2*1024*1024)
#define MAX_allLocalVariableTable_SIZE (2*1024*1024)

struct MetaMethod;
struct OopDesc;
struct Kclass;

struct OperandStack {//操作数栈
    int operandStackSize=0;
    int operandSP=-1;
    uint32_t* operandStack=nullptr;
    OperandStack()= default;
    explicit OperandStack(int operandStackSize) {
        this->operandStackSize=operandStackSize;
        operandStack=(uint32_t*)malloc(operandStackSize<<2);
        operandSP=0;
    }
    ~OperandStack() {
        if (operandStack){free(operandStack);operandStack=nullptr;}
        operandSP=operandStackSize=0;
    }

    void Push_I32(int32_t val) {
        operandStack[operandSP]=*((uint32_t*)(&val));
        ++operandSP;
    }
    void Push_uI32(uint32_t val) {
        operandStack[operandSP]=val;
        ++operandSP;
    }
    void Push_F(float val) {
        operandStack[operandSP]=*((uint32_t*)(&val));
        ++operandSP;
    }
    uint32_t getTop_uI32() const {
        return operandStack[operandSP-1];
    }
    int32_t getTop_I32() const {
        auto* p=(int32_t*)(operandStack+operandSP);
        --p;
        int32_t re=*p;
        return re;
    }
    float getTop_F() const {
        auto* p=(float*)(operandStack+operandSP);
        --p;
        float re=*p;
        return re;
    }
    void Pop_32() {
        --operandSP;
    }

    void Push_I64(int64_t val) {//存放long和double
        auto* p=(uint64_t*)(operandStack+operandSP);
        *p=*((uint64_t*)(&val));
        operandSP+=2;
    }
    void Push_D(double val) {
        auto* p=(uint64_t*)(operandStack+operandSP);
        *p=*((uint64_t*)(&val));
        operandSP+=2;
    }
    int64_t getTop_I64() const {
        auto* p=(int64_t*)(operandStack+operandSP);
        --p;
        int64_t re=*p;
        return re;
    }
    double getTop_D() const {
        auto* p=(double*)(operandStack+operandSP);
        --p;
        double re=*p;
        return re;
    }
    void Pop_64() {
        operandSP-=2;
    }
    void Push_uI64(uint64_t val) {//存放long和double
        auto* p=(uint64_t*)(operandStack+operandSP);
        *p=val;
        operandSP+=2;
    }

    uint64_t getTop_uI64() const {
        auto* p=(uint64_t*)(operandStack+operandSP);
        --p;
        uint64_t re=*p;
        return re;
    }

    void PushuI32n(const uint32_t* pptr,int size) {
        void* d=operandStack+operandSP;
        memcpy(d,pptr,size<<2);
        operandSP+=size;
    }

    void Pop_32n(int size) {
        operandSP-=size;
    }

    void getTop_uI32n(uint32_t* pptr,int size) {
        if (size<=0){return ;}
        const void* s=operandStack+operandSP-size;
        memcpy(pptr,s,size<<2);
    }
};


struct StackFrame {
    int PC=-1;//程序计数器
    bool isNativeMethod=false;
    OperandStack* operand_stack=nullptr;
    Slots* local_variable_table=nullptr;
    const char* code=nullptr;//指向代码
    Kclass* Class=nullptr;
    OopDesc* instance=nullptr;
    const MetaMethod* method=nullptr;
    StackFrame()=default;
    StackFrame(int localVariableTableSize,int operandStackSize,
    const char* code,bool isNative=false,Kclass* Class=nullptr,
    OopDesc* instance=nullptr,const MetaMethod* method=nullptr) {
        PC=0;
        this->isNativeMethod=isNative;
        operand_stack=new OperandStack(operandStackSize);
        local_variable_table=new Slots(localVariableTableSize);
        this->code=code;
        this->Class=Class;
        this->instance=instance;
        this->method=method;
    }
    ~StackFrame() {
        PC=-1;
        if (operand_stack){delete operand_stack;operand_stack=nullptr;}
        if (local_variable_table){delete local_variable_table;local_variable_table=nullptr;}
        code=nullptr;
    }
};

typedef std::stack<StackFrame*> Stack;