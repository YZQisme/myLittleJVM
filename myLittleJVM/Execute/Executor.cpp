//
// Created by YZQ on 25-7-28.
//
#include "Executor.h"
#include "Instructions.h"
#include "../MetaSpace/Kclass.h"
#include "../MetaSpace/ClassLoader.h"
#include "NativeMethod.h"
int Execute::initialStack(const std::string &className, const std::string &MethodName,
    const std::string& describe) {
    instanceClass* runClass=(instanceClass*)my_class_loader.getInstanceClass(className);
    if (runClass==nullptr){return -2;}
    MetaMethod* method=nullptr;
    for (auto m: runClass->methods) {
        if (m->name==MethodName && m->descriptor==describe) {
            method=m;
            break;
        }
    }
    if (method==nullptr){return -1;}
    StackFrame* mainS=new StackFrame(method->maxLocalVTSize,
        method->maxOpStackSize,method->code->code,
        false,runClass,nullptr,method);
    stack.emplace(mainS);
    stack_frame=mainS;
    code=mainS->code;
    runState=RUN_ABLE;
    return 0;
}

int Execute::NativeStackPop(uint64_t* ret) {
    int i=this->stack_frame->method->descriptor.find(')');
    char j=this->stack_frame->method->descriptor[i+1];
    stack.pop();
    delete stack_frame;stack_frame=nullptr;code=nullptr;
    if (stack.empty()) {
        return RUN_FINISH;
    }
    stack_frame=stack.top();
    code=stack_frame->code;
    if (j=='D'||j=='J') {
        stack_frame->operand_stack->Push_uI64(*ret);
    }else if (j=='V') {}
    else {
        stack_frame->operand_stack->Push_uI32(uint32_t(*ret));
    }
    return RUN_CONTINUE;
}


void Execute::run() {
    uint8_t op;
    if (runState==RUN_ABLE){runState=RUN_CONTINUE;}
    while (runState==RUN_CONTINUE) {
        while (runState==RUN_CONTINUE && !stack.empty()) {
            op=*((uint8_t*)(code+stack_frame->PC));
            runState=InstructionFuncPtrArr[op](this);
        }
        if (stack.empty()) {
            runState=RUN_FINISH;
            break;
        }
        std::string MethodName=this->stack_frame->Class->name+"."
            +this->stack_frame->method->name+":"
            +this->stack_frame->method->descriptor;
        if (runState==RUN_NATIVE) {
            //run native
            auto nf=nativeMethodNameToFuncPtr.find(MethodName);
            if (nf==nativeMethodNameToFuncPtr.end()) {
                std::cerr<<"Native方法: "<<MethodName<<"未实现"<<std::endl;
                runState=RUN_INSTRUCTION_UNDO;
                break;
            }else {
                //运行Native方法
                uint64_t ret=0;
                runState=(*nf).second(this,&ret);
                if (runState!=RUN_CONTINUE) {
                    std::cerr<<"在运行Native方法: "<<MethodName<<"中发生了错误"<<std::endl;
                    break;
                }else {
                    runState=NativeStackPop(&ret);
                }
            }
        }else{
            std::cerr<<"在运行方法: "<<MethodName<<"中发生了错误"<<std::endl;
            break;
        }
    }
    if (runState!=RUN_FINISH){
        std::cerr<<"Executor运行异常退出"<<std::endl;
    }
}
