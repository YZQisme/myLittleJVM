//
// Created by YZQ on 25-8-5.
//
#include "Instructions.h"
#include "../MetaSpace/Oop.h"
#include "../Heap/heap.h"
#include "../MetaSpace/ClassLoader.h"
#include <cmath>
#include <queue>

int I_nop(Execute* execute) {
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_aconst_null(Execute* execute) {
    execute->stack_frame->operand_stack->Push_uI32(0);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_iconst_m1(Execute* execute) {
    execute->stack_frame->operand_stack->Push_I32(-1);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_iconst_0(Execute* execute) {
    execute->stack_frame->operand_stack->Push_I32(0);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_iconst_1(Execute* execute) {
    execute->stack_frame->operand_stack->Push_I32(1);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_iconst_2(Execute* execute) {
    execute->stack_frame->operand_stack->Push_I32(2);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_iconst_3(Execute* execute) {
    execute->stack_frame->operand_stack->Push_I32(3);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_iconst_4(Execute* execute) {
    execute->stack_frame->operand_stack->Push_I32(4);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_iconst_5(Execute* execute) {
    execute->stack_frame->operand_stack->Push_I32(5);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_lconst_0(Execute* execute) {
    execute->stack_frame->operand_stack->Push_I64(0);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_lconst_1(Execute* execute) {
    execute->stack_frame->operand_stack->Push_I64(1);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_fconst_0(Execute* execute) {
    execute->stack_frame->operand_stack->Push_F(0);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_fconst_1(Execute* execute){
    execute->stack_frame->operand_stack->Push_F(1);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_fconst_2(Execute* execute){
    execute->stack_frame->operand_stack->Push_F(2);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_dconst_0(Execute* execute){
    execute->stack_frame->operand_stack->Push_D(0);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_dconst_1(Execute* execute){
    execute->stack_frame->operand_stack->Push_D(1);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_bipush(Execute* execute){//byte int8
    int b=execute->stack_frame->PC+1;
    int32_t t=*((int8_t*)(execute->code+b));
    execute->stack_frame->operand_stack->Push_I32(t);
    execute->stack_frame->PC+=2;
    return RUN_CONTINUE;
}

int I_sipush(Execute* execute){//int16
    int b=execute->stack_frame->PC+1;
    uint16_t t1=*((uint8_t*)(execute->code+b)),
    t2=*((uint8_t*)(execute->code+b+1));
    t1=(t1<<8)|t2;
    int16_t t=*((int16_t*)(&t1));
    execute->stack_frame->operand_stack->Push_I32(t);
    execute->stack_frame->PC+=3;
    return RUN_CONTINUE;
}

int I_ldc(Execute* execute) {
    int b=execute->stack_frame->PC+1;
    uint8_t t1=*((uint8_t*)(execute->code+b));
    instanceClass* Cla=(instanceClass*)execute->stack_frame->Class;
    MetaConstantPoolItem* mcpi=Cla->constantPool->pool[t1];
    if (mcpi->tag==CONSTANT_Integer) {
        auto p=(MCP_Integer*)mcpi;
        execute->stack_frame->operand_stack->Push_I32(p->value);
    }else if (mcpi->tag==CONSTANT_Float) {
        auto p=(MCP_Float*)mcpi;
        execute->stack_frame->operand_stack->Push_F(p->value);
    }else if (mcpi->tag==CONSTANT_String) {
        if (baseClassLoaded==0){std::cerr<<"ldc指令提前执行"<<std::endl;}
        auto p=(MCP_String*)mcpi;
        execute->stack_frame->operand_stack->Push_uI32(p->stringPoolPtr);
    }else if (mcpi->tag==CONSTANT_Class){
        if (baseClassLoaded==0){std::cerr<<"ldc指令提前执行"<<std::endl;}
        auto p=(MCP_Class*)mcpi;
        uint32_t yy=my_class_loader.getInstanceClass(p->name)->ClassInstancePtr;
        execute->stack_frame->operand_stack->Push_uI32(yy);
    }else {
        std::cerr<<"ldc指令部分未实现"<<std::endl;
        return RUN_INSTRUCTION_UNDO;
    }
    execute->stack_frame->PC+=2;
    return RUN_CONTINUE;
};

int I_ldc_w(Execute* execute) {
    int b=execute->stack_frame->PC+1;
    uint16_t t1=*((uint8_t*)(execute->code+b)),
    t2=*((uint8_t*)(execute->code+b+1));
    t1=(t1<<8)|t2;
    uint16_t t=*((int16_t*)(&t1));
    instanceClass* Cla=(instanceClass*)execute->stack_frame->Class;
    MetaConstantPoolItem* mcpi=Cla->constantPool->pool[t];
    if (mcpi->tag==CONSTANT_Integer) {
        auto p=(MCP_Integer*)mcpi;
        execute->stack_frame->operand_stack->Push_I32(p->value);
    }else if (mcpi->tag==CONSTANT_Float) {
        auto p=(MCP_Float*)mcpi;
        execute->stack_frame->operand_stack->Push_F(p->value);
    }else if (mcpi->tag==CONSTANT_String) {
        if (baseClassLoaded==0){std::cerr<<"ldc_w指令提前执行"<<std::endl;}
        auto p=(MCP_String*)mcpi;
        execute->stack_frame->operand_stack->Push_uI32(p->stringPoolPtr);
    }else if (mcpi->tag==CONSTANT_Class){
        if (baseClassLoaded==0){std::cerr<<"ldc_w指令提前执行"<<std::endl;}
        auto p=(MCP_Class*)mcpi;
        uint32_t yy=my_class_loader.getInstanceClass(p->name)->ClassInstancePtr;
        execute->stack_frame->operand_stack->Push_uI32(yy);
    }else{
        std::cerr<<"ldc_w指令部分未实现"<<std::endl;
    }
    execute->stack_frame->PC+=3;
    return RUN_CONTINUE;
};

int I_ldc2_w(Execute* execute) {
    int b=execute->stack_frame->PC+1;
    uint16_t t1=*((uint8_t*)(execute->code+b)),
    t2=*((uint8_t*)(execute->code+b+1));
    t1=(t1<<8)|t2;
    uint16_t t=*((int16_t*)(&t1));
    instanceClass* Cla=(instanceClass*)execute->stack_frame->Class;
    MetaConstantPoolItem* mcpi=Cla->constantPool->pool[t];
    if (mcpi->tag==CONSTANT_Long) {
        auto p=(MCP_Long*)mcpi;
        execute->stack_frame->operand_stack->Push_I64(p->value);
    }else if (mcpi->tag==CONSTANT_Double) {
        auto p=(MCP_Double*)mcpi;
        execute->stack_frame->operand_stack->Push_D(p->value);
    }else{
        std::cerr<<"ldc2_w指令部分未实现"<<std::endl;
    }
    execute->stack_frame->PC+=3;
    return RUN_CONTINUE;
};

int I_iload(Execute* execute) {
    int b=execute->stack_frame->PC+1;
    int i=*((uint8_t*)(execute->code+b));
    int32_t t=execute->stack_frame->local_variable_table->getI32(i);
    execute->stack_frame->operand_stack->Push_I32(t);
    execute->stack_frame->PC+=2;
    return RUN_CONTINUE;
}

int I_lload(Execute* execute) {
    int b=execute->stack_frame->PC+1;
    int i=*((uint8_t*)(execute->code+b));
    int64_t t=execute->stack_frame->local_variable_table->getI64(i);
    execute->stack_frame->operand_stack->Push_I64(t);
    execute->stack_frame->PC+=2;
    return RUN_CONTINUE;
}

int I_fload(Execute* execute) {
    int b=execute->stack_frame->PC+1;
    int i=*((uint8_t*)(execute->code+b));
    float t=execute->stack_frame->local_variable_table->getF(i);
    execute->stack_frame->operand_stack->Push_F(t);
    execute->stack_frame->PC+=2;
    return RUN_CONTINUE;
}

int I_dload(Execute* execute) {
    int b=execute->stack_frame->PC+1;
    int i=*((uint8_t*)(execute->code+b));
    double t=execute->stack_frame->local_variable_table->getD(i);
    execute->stack_frame->operand_stack->Push_D(t);
    execute->stack_frame->PC+=2;
    return RUN_CONTINUE;
}

int I_aload(Execute* execute) {
    int b=execute->stack_frame->PC+1;
    int i=*((uint8_t*)(execute->code+b));
    uint32_t t=execute->stack_frame->local_variable_table->getuI32(i);
    execute->stack_frame->operand_stack->Push_uI32(t);
    execute->stack_frame->PC+=2;
    return RUN_CONTINUE;
}

int I_iload_0(Execute* execute) {
    int32_t t=execute->stack_frame->local_variable_table->getI32(0);
    execute->stack_frame->operand_stack->Push_I32(t);
    execute->stack_frame->PC+=1;
    return RUN_CONTINUE;
}

int I_iload_1(Execute* execute) {
    int32_t t=execute->stack_frame->local_variable_table->getI32(1);
    execute->stack_frame->operand_stack->Push_I32(t);
    execute->stack_frame->PC+=1;
    return RUN_CONTINUE;
}

int I_iload_2(Execute* execute) {
    int32_t t=execute->stack_frame->local_variable_table->getI32(2);
    execute->stack_frame->operand_stack->Push_I32(t);
    execute->stack_frame->PC+=1;
    return RUN_CONTINUE;
}

int I_iload_3(Execute* execute) {
    int32_t t=execute->stack_frame->local_variable_table->getI32(3);
    execute->stack_frame->operand_stack->Push_I32(t);
    execute->stack_frame->PC+=1;
    return RUN_CONTINUE;
}

int I_lload_0(Execute* execute) {
    int64_t t=execute->stack_frame->local_variable_table->getI64(0);
    execute->stack_frame->operand_stack->Push_I64(t);
    execute->stack_frame->PC+=1;
    return RUN_CONTINUE;
}

int I_lload_1(Execute* execute){
    int64_t t=execute->stack_frame->local_variable_table->getI64(1);
    execute->stack_frame->operand_stack->Push_I64(t);
    execute->stack_frame->PC+=1;
    return RUN_CONTINUE;
}

int I_lload_2(Execute* execute){
    int64_t t=execute->stack_frame->local_variable_table->getI64(2);
    execute->stack_frame->operand_stack->Push_I64(t);
    execute->stack_frame->PC+=1;
    return RUN_CONTINUE;
}

int I_lload_3(Execute* execute){
    int64_t t=execute->stack_frame->local_variable_table->getI64(3);
    execute->stack_frame->operand_stack->Push_I64(t);
    execute->stack_frame->PC+=1;
    return RUN_CONTINUE;
}

int I_fload_0(Execute* execute) {
    float t=execute->stack_frame->local_variable_table->getF(0);
    execute->stack_frame->operand_stack->Push_F(t);
    execute->stack_frame->PC+=1;
    return RUN_CONTINUE;
}

int I_fload_1(Execute* execute) {
    float t=execute->stack_frame->local_variable_table->getF(1);
    execute->stack_frame->operand_stack->Push_F(t);
    execute->stack_frame->PC+=1;
    return RUN_CONTINUE;
}

int I_fload_2(Execute* execute) {
    float t=execute->stack_frame->local_variable_table->getF(2);
    execute->stack_frame->operand_stack->Push_F(t);
    execute->stack_frame->PC+=1;
    return RUN_CONTINUE;
}

int I_fload_3(Execute* execute) {
    float t=execute->stack_frame->local_variable_table->getF(3);
    execute->stack_frame->operand_stack->Push_F(t);
    execute->stack_frame->PC+=1;
    return RUN_CONTINUE;
}

int I_dload_0(Execute* execute) {
    double t=execute->stack_frame->local_variable_table->getD(0);
    execute->stack_frame->operand_stack->Push_D(t);
    execute->stack_frame->PC+=1;
    return RUN_CONTINUE;
}

int I_dload_1(Execute* execute) {
    double t=execute->stack_frame->local_variable_table->getD(1);
    execute->stack_frame->operand_stack->Push_D(t);
    execute->stack_frame->PC+=1;
    return RUN_CONTINUE;
}

int I_dload_2(Execute* execute) {
    double t=execute->stack_frame->local_variable_table->getD(2);
    execute->stack_frame->operand_stack->Push_D(t);
    execute->stack_frame->PC+=1;
    return RUN_CONTINUE;
}

int I_dload_3(Execute* execute) {
    double t=execute->stack_frame->local_variable_table->getD(3);
    execute->stack_frame->operand_stack->Push_D(t);
    execute->stack_frame->PC+=1;
    return RUN_CONTINUE;
}

int I_aload_0(Execute* execute) {
    uint32_t t=execute->stack_frame->local_variable_table->getuI32(0);
    execute->stack_frame->operand_stack->Push_uI32(t);
    execute->stack_frame->PC+=1;
    return RUN_CONTINUE;
}

int I_aload_1(Execute* execute) {
    uint32_t t=execute->stack_frame->local_variable_table->getuI32(1);
    execute->stack_frame->operand_stack->Push_uI32(t);
    execute->stack_frame->PC+=1;
    return RUN_CONTINUE;
}

int I_aload_2(Execute* execute) {
    uint32_t t=execute->stack_frame->local_variable_table->getuI32(2);
    execute->stack_frame->operand_stack->Push_uI32(t);
    execute->stack_frame->PC+=1;
    return RUN_CONTINUE;
}

int I_aload_3(Execute* execute) {
    uint32_t t=execute->stack_frame->local_variable_table->getuI32(3);
    execute->stack_frame->operand_stack->Push_uI32(t);
    execute->stack_frame->PC+=1;
    return RUN_CONTINUE;
}

int I_iaload(Execute* execute) {
    int32_t index=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    uint32_t aref=execute->stack_frame->operand_stack->getTop_uI32();
    execute->stack_frame->operand_stack->Pop_32();
    if (aref==0) {
        std::cerr<<"iaload发生空指针错误"<<std::endl;
        return RUN_ERROR;
    }
    ArrayOop* ap=(ArrayOop*)(heap.getRowPointer(aref));
    if (ap->eleCount<=index || index<0 ) {
        std::cerr<<"iaload发生数组越界"<<std::endl;
        return RUN_ERROR;
    }
    execute->stack_frame->operand_stack->Push_I32(
        ap->elements.get<int32_t>(index));
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_laload(Execute* execute) {
    int32_t index=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    uint32_t aref=execute->stack_frame->operand_stack->getTop_uI32();
    execute->stack_frame->operand_stack->Pop_32();
    if (aref==0) {
        std::cerr<<"laload发生空指针错误"<<std::endl;
        return RUN_ERROR;
    }
    ArrayOop* ap=(ArrayOop*)(heap.getRowPointer(aref));
    if (ap->eleCount<=index || index<0 ) {
        std::cerr<<"laload发生数组越界"<<std::endl;
        return RUN_ERROR;
    }
    execute->stack_frame->operand_stack->Push_I64(
        ap->elements.get<int64_t>(index));
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_faload(Execute* execute) {
    int32_t index=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    uint32_t aref=execute->stack_frame->operand_stack->getTop_uI32();
    execute->stack_frame->operand_stack->Pop_32();
    if (aref==0) {
        std::cerr<<"faload发生空指针错误"<<std::endl;
        return RUN_ERROR;
    }
    ArrayOop* ap=(ArrayOop*)(ArrayOop*)(heap.getRowPointer(aref));
    if (ap->eleCount<=index || index<0 ) {
        std::cerr<<"faload发生数组越界"<<std::endl;
        return RUN_ERROR;
    }
    execute->stack_frame->operand_stack->Push_F(
        ap->elements.get<float>(index));
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_daload(Execute* execute){
    int32_t index=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    uint32_t aref=execute->stack_frame->operand_stack->getTop_uI32();
    execute->stack_frame->operand_stack->Pop_32();
    if (aref==0) {
        std::cerr<<"daload发生空指针错误"<<std::endl;
        return RUN_ERROR;
    }
    ArrayOop* ap=(ArrayOop*)(heap.getRowPointer(aref));
    if (ap->eleCount<=index || index<0 ) {
        std::cerr<<"daload发生数组越界"<<std::endl;
        return RUN_ERROR;
    }
    execute->stack_frame->operand_stack->Push_D(
        ap->elements.get<double>(index));
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_aaload(Execute* execute){
    int32_t index=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    uint32_t aref=execute->stack_frame->operand_stack->getTop_uI32();
    execute->stack_frame->operand_stack->Pop_32();
    if (aref==0) {
        std::cerr<<"aaload发生空指针错误"<<std::endl;
        return RUN_ERROR;
    }
    ArrayOop* ap=(ArrayOop*)(heap.getRowPointer(aref));
    if (ap->eleCount<=index || index<0 ) {
        std::cerr<<"aaload发生数组越界"<<std::endl;
        return RUN_ERROR;
    }
    execute->stack_frame->operand_stack->Push_uI32(
        ap->elements.get<uint32_t>(index));
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_baload(Execute* execute){
    int32_t index=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    uint32_t aref=execute->stack_frame->operand_stack->getTop_uI32();
    execute->stack_frame->operand_stack->Pop_32();
    if (aref==0) {
        std::cerr<<"baload发生空指针错误"<<std::endl;
        return RUN_ERROR;
    }
    ArrayOop* ap=(ArrayOop*)(heap.getRowPointer(aref));
    if (ap->eleCount<=index || index<0 ) {
        std::cerr<<"baload发生数组越界"<<std::endl;
        return RUN_ERROR;
    }
    execute->stack_frame->operand_stack->Push_I32(
        ap->elements.get<int8_t>(index));
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_caload(Execute* execute) {
    int32_t index=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    uint32_t aref=execute->stack_frame->operand_stack->getTop_uI32();
    execute->stack_frame->operand_stack->Pop_32();
    if (aref==0) {
        std::cerr<<"caload发生空指针错误"<<std::endl;
        return RUN_ERROR;
    }
    ArrayOop* ap=(ArrayOop*)(heap.getRowPointer(aref));
    if (ap->eleCount<=index || index<0 ) {
        std::cerr<<"caload发生数组越界"<<std::endl;
        return RUN_ERROR;
    }
    execute->stack_frame->operand_stack->Push_I32(
        ap->elements.get<uint16_t>(index));
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_saload(Execute* execute){
    int32_t index=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    uint32_t aref=execute->stack_frame->operand_stack->getTop_uI32();
    execute->stack_frame->operand_stack->Pop_32();
    if (aref==0) {
        std::cerr<<"saload发生空指针错误"<<std::endl;
        return RUN_ERROR;
    }
    ArrayOop* ap=(ArrayOop*)(heap.getRowPointer(aref));
    if (ap->eleCount<=index || index<0 ) {
        std::cerr<<"saload发生数组越界"<<std::endl;
        return RUN_ERROR;
    }
    execute->stack_frame->operand_stack->Push_I32(
        ap->elements.get<int16_t>(index));
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_istore(Execute* execute) {
    int b=execute->stack_frame->PC+1;
    int i=*((uint8_t*)(execute->code+b));
    int32_t t=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    execute->stack_frame->local_variable_table->putI32(t,i);
    execute->stack_frame->PC+=2;
    return RUN_CONTINUE;
}

int I_lstore(Execute* execute) {
    int b=execute->stack_frame->PC+1;
    int i=*((uint8_t*)(execute->code+b));
    int64_t t=execute->stack_frame->operand_stack->getTop_I64();
    execute->stack_frame->operand_stack->Pop_64();
    execute->stack_frame->local_variable_table->putI64(t,i);
    execute->stack_frame->PC+=2;
    return RUN_CONTINUE;
}

int I_fstore(Execute* execute) {
    int b=execute->stack_frame->PC+1;
    int i=*((uint8_t*)(execute->code+b));
    float t=execute->stack_frame->operand_stack->getTop_F();
    execute->stack_frame->operand_stack->Pop_32();
    execute->stack_frame->local_variable_table->putF(t,i);
    execute->stack_frame->PC+=2;
    return RUN_CONTINUE;
}

int I_dstore(Execute* execute) {
    int b=execute->stack_frame->PC+1;
    int i=*((uint8_t*)(execute->code+b));
    double t=execute->stack_frame->operand_stack->getTop_D();
    execute->stack_frame->operand_stack->Pop_64();
    execute->stack_frame->local_variable_table->putD(t,i);
    execute->stack_frame->PC+=2;
    return RUN_CONTINUE;
}

int I_astore(Execute* execute) {
    int b=execute->stack_frame->PC+1;
    int i=*((uint8_t*)(execute->code+b));
    uint32_t t=execute->stack_frame->operand_stack->getTop_uI32();
    execute->stack_frame->operand_stack->Pop_32();
    execute->stack_frame->local_variable_table->putuI32(t,i);
    execute->stack_frame->PC+=2;
    return RUN_CONTINUE;
}

int I_istore_0(Execute* execute) {
    int32_t t=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    execute->stack_frame->local_variable_table->putI32(t,0);
    execute->stack_frame->PC+=1;
    return RUN_CONTINUE;
}

int I_istore_1(Execute* execute){
    int32_t t=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    execute->stack_frame->local_variable_table->putI32(t,1);
    execute->stack_frame->PC+=1;
    return RUN_CONTINUE;
}

int I_istore_2(Execute* execute){
    int32_t t=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    execute->stack_frame->local_variable_table->putI32(t,2);
    execute->stack_frame->PC+=1;
    return RUN_CONTINUE;
}

int I_istore_3(Execute* execute){
    int32_t t=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    execute->stack_frame->local_variable_table->putI32(t,3);
    execute->stack_frame->PC+=1;
    return RUN_CONTINUE;
}

int I_lstore_0(Execute* execute) {
    int64_t t=execute->stack_frame->operand_stack->getTop_I64();
    execute->stack_frame->operand_stack->Pop_64();
    execute->stack_frame->local_variable_table->putI64(t,0);
    execute->stack_frame->PC+=1;
    return RUN_CONTINUE;
}

int I_lstore_1(Execute* execute) {
    int64_t t=execute->stack_frame->operand_stack->getTop_I64();
    execute->stack_frame->operand_stack->Pop_64();
    execute->stack_frame->local_variable_table->putI64(t,1);
    execute->stack_frame->PC+=1;
    return RUN_CONTINUE;
}

int I_lstore_2(Execute* execute) {
    int64_t t=execute->stack_frame->operand_stack->getTop_I64();
    execute->stack_frame->operand_stack->Pop_64();
    execute->stack_frame->local_variable_table->putI64(t,2);
    execute->stack_frame->PC+=1;
    return RUN_CONTINUE;
}

int I_lstore_3(Execute* execute) {
    int64_t t=execute->stack_frame->operand_stack->getTop_I64();
    execute->stack_frame->operand_stack->Pop_64();
    execute->stack_frame->local_variable_table->putI64(t,3);
    execute->stack_frame->PC+=1;
    return RUN_CONTINUE;
}

int I_fstore_0(Execute* execute) {
    float t=execute->stack_frame->operand_stack->getTop_F();
    execute->stack_frame->operand_stack->Pop_32();
    execute->stack_frame->local_variable_table->putF(t,0);
    execute->stack_frame->PC+=1;
    return RUN_CONTINUE;
}

int I_fstore_1(Execute* execute) {
    float t=execute->stack_frame->operand_stack->getTop_F();
    execute->stack_frame->operand_stack->Pop_32();
    execute->stack_frame->local_variable_table->putF(t,1);
    execute->stack_frame->PC+=1;
    return RUN_CONTINUE;
}

int I_fstore_2(Execute* execute) {
    float t=execute->stack_frame->operand_stack->getTop_F();
    execute->stack_frame->operand_stack->Pop_32();
    execute->stack_frame->local_variable_table->putF(t,2);
    execute->stack_frame->PC+=1;
    return RUN_CONTINUE;
}

int I_fstore_3(Execute* execute) {
    float t=execute->stack_frame->operand_stack->getTop_F();
    execute->stack_frame->operand_stack->Pop_32();
    execute->stack_frame->local_variable_table->putF(t,3);
    execute->stack_frame->PC+=1;
    return RUN_CONTINUE;
}

int I_dstore_0(Execute* execute) {
    double t=execute->stack_frame->operand_stack->getTop_D();
    execute->stack_frame->operand_stack->Pop_64();
    execute->stack_frame->local_variable_table->putD(t,0);
    execute->stack_frame->PC+=1;
    return RUN_CONTINUE;
}

int I_dstore_1(Execute* execute) {
    double t=execute->stack_frame->operand_stack->getTop_D();
    execute->stack_frame->operand_stack->Pop_64();
    execute->stack_frame->local_variable_table->putD(t,1);
    execute->stack_frame->PC+=1;
    return RUN_CONTINUE;
}

int I_dstore_2(Execute* execute) {
    double t=execute->stack_frame->operand_stack->getTop_D();
    execute->stack_frame->operand_stack->Pop_64();
    execute->stack_frame->local_variable_table->putD(t,2);
    execute->stack_frame->PC+=1;
    return RUN_CONTINUE;
}

int I_dstore_3(Execute* execute) {
    double t=execute->stack_frame->operand_stack->getTop_D();
    execute->stack_frame->operand_stack->Pop_64();
    execute->stack_frame->local_variable_table->putD(t,3);
    execute->stack_frame->PC+=1;
    return RUN_CONTINUE;
}

int I_astore_0(Execute* execute) {
    uint32_t t=execute->stack_frame->operand_stack->getTop_uI32();
    execute->stack_frame->operand_stack->Pop_32();
    execute->stack_frame->local_variable_table->putuI32(t,0);
    execute->stack_frame->PC+=1;
    return RUN_CONTINUE;
}

int I_astore_1(Execute* execute){
    uint32_t t=execute->stack_frame->operand_stack->getTop_uI32();
    execute->stack_frame->operand_stack->Pop_32();
    execute->stack_frame->local_variable_table->putuI32(t,1);
    execute->stack_frame->PC+=1;
    return RUN_CONTINUE;
}

int I_astore_2(Execute* execute){
    uint32_t t=execute->stack_frame->operand_stack->getTop_uI32();
    execute->stack_frame->operand_stack->Pop_32();
    execute->stack_frame->local_variable_table->putuI32(t,2);
    execute->stack_frame->PC+=1;
    return RUN_CONTINUE;
}

int I_astore_3(Execute* execute){
    uint32_t t=execute->stack_frame->operand_stack->getTop_uI32();
    execute->stack_frame->operand_stack->Pop_32();
    execute->stack_frame->local_variable_table->putuI32(t,3);
    execute->stack_frame->PC+=1;
    return RUN_CONTINUE;
}

int I_iastore(Execute* execute){
    int32_t value=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    int32_t index=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    uint32_t aref=execute->stack_frame->operand_stack->getTop_uI32();
    execute->stack_frame->operand_stack->Pop_32();
    if (aref==0 || index<0) {
        std::cerr<<"iastore发生错误"<<std::endl;
        return RUN_ERROR;
    }
    ArrayOop* ap=(ArrayOop*)(heap.getRowPointer(aref));
    if (index>=ap->eleCount) {
        std::cerr<<"iastore发生错误"<<std::endl;
        return RUN_ERROR;
    }
    ap->elements.put<int32_t>(value,index);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_lastore(Execute* execute){
    int64_t value=execute->stack_frame->operand_stack->getTop_I64();
    execute->stack_frame->operand_stack->Pop_64();
    int32_t index=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    uint32_t aref=execute->stack_frame->operand_stack->getTop_uI32();
    execute->stack_frame->operand_stack->Pop_32();
    if (aref==0 || index<0) {
        std::cerr<<"lastore发生错误"<<std::endl;
        return RUN_ERROR;
    }
    ArrayOop* ap=(ArrayOop*)(heap.getRowPointer(aref));
    if (index>=ap->eleCount) {
        std::cerr<<"lastore发生错误"<<std::endl;
        return RUN_ERROR;
    }
    ap->elements.put<int64_t>(value,index);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_fastore(Execute* execute){
    float value=execute->stack_frame->operand_stack->getTop_F();
    execute->stack_frame->operand_stack->Pop_32();
    int32_t index=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    uint32_t aref=execute->stack_frame->operand_stack->getTop_uI32();
    execute->stack_frame->operand_stack->Pop_32();
    if (aref==0 || index<0) {
        std::cerr<<"fastore发生错误"<<std::endl;
        return RUN_ERROR;
    }
    ArrayOop* ap=(ArrayOop*)(heap.getRowPointer(aref));
    if (index>=ap->eleCount) {
        std::cerr<<"fastore发生错误"<<std::endl;
        return RUN_ERROR;
    }
    ap->elements.put<float>(value,index);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_dastore(Execute* execute){
    double value=execute->stack_frame->operand_stack->getTop_D();
    execute->stack_frame->operand_stack->Pop_64();
    int32_t index=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    uint32_t aref=execute->stack_frame->operand_stack->getTop_uI32();
    execute->stack_frame->operand_stack->Pop_32();
    if (aref==0 || index<0) {
        std::cerr<<"dastore发生错误"<<std::endl;
        return RUN_ERROR;
    }
    ArrayOop* ap=(ArrayOop*)(heap.getRowPointer(aref));
    if (index>=ap->eleCount) {
        std::cerr<<"dastore发生错误"<<std::endl;
        return RUN_ERROR;
    }
    ap->elements.put<double>(value,index);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_aastore(Execute* execute){
    uint32_t value=execute->stack_frame->operand_stack->getTop_uI32();
    execute->stack_frame->operand_stack->Pop_32();
    int32_t index=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    uint32_t aref=execute->stack_frame->operand_stack->getTop_uI32();
    execute->stack_frame->operand_stack->Pop_32();
    if (aref==0 || index<0) {
        std::cerr<<"aastore发生错误"<<std::endl;
        return RUN_ERROR;
    }
    ArrayOop* ap=(ArrayOop*)(heap.getRowPointer(aref));
    if (index>=ap->eleCount) {
        std::cerr<<"aastore发生错误"<<std::endl;
        return RUN_ERROR;
    }
    ap->elements.put<uint32_t>(value,index);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_bastore(Execute* execute){
    int32_t value=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    int32_t index=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    uint32_t aref=execute->stack_frame->operand_stack->getTop_uI32();
    execute->stack_frame->operand_stack->Pop_32();
    if (aref==0 || index<0) {
        std::cerr<<"bastore发生错误"<<std::endl;
        return RUN_ERROR;
    }
    ArrayOop* ap=(ArrayOop*)(heap.getRowPointer(aref));
    if (index>=ap->eleCount) {
        std::cerr<<"bastore发生错误"<<std::endl;
        return RUN_ERROR;
    }
    ap->elements.put<int8_t>((int8_t)value,index);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_castore(Execute* execute){
    uint32_t value=execute->stack_frame->operand_stack->getTop_uI32();
    execute->stack_frame->operand_stack->Pop_32();
    int32_t index=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    uint32_t aref=execute->stack_frame->operand_stack->getTop_uI32();
    execute->stack_frame->operand_stack->Pop_32();
    if (aref==0 || index<0) {
        std::cerr<<"castore发生错误"<<std::endl;
        return RUN_ERROR;
    }
    ArrayOop* ap=(ArrayOop*)(heap.getRowPointer(aref));
    if (index>=ap->eleCount) {
        std::cerr<<"castore发生错误"<<std::endl;
        return RUN_ERROR;
    }
    ap->elements.put<uint16_t>(value,index);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_sastore(Execute* execute){
    int32_t value=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    int32_t index=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    uint32_t aref=execute->stack_frame->operand_stack->getTop_uI32();
    execute->stack_frame->operand_stack->Pop_32();
    if (aref==0 || index<0) {
        std::cerr<<"sastore发生错误"<<std::endl;
        return RUN_ERROR;
    }
    ArrayOop* ap=(ArrayOop*)(heap.getRowPointer(aref));
    if (index>=ap->eleCount) {
        std::cerr<<"sastore发生错误"<<std::endl;
        return RUN_ERROR;
    }
    ap->elements.put<int16_t>((int16_t)value,index);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_pop(Execute* execute) {
    execute->stack_frame->operand_stack->Pop_32();
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_pop2(Execute* execute) {
    execute->stack_frame->operand_stack->Pop_64();
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_dup(Execute* execute) {//对栈顶I32进行复制一份
    int32_t t=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Push_I32(t);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_dup_x1(Execute* execute) {
    int32_t t1=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    int32_t t2=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    execute->stack_frame->operand_stack->Push_I32(t1);
    execute->stack_frame->operand_stack->Push_I32(t2);
    execute->stack_frame->operand_stack->Push_I32(t1);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_dup_x2(Execute* execute) {
    int32_t t1=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    int64_t t2=execute->stack_frame->operand_stack->getTop_I64();
    execute->stack_frame->operand_stack->Pop_64();
    execute->stack_frame->operand_stack->Push_I32(t1);
    execute->stack_frame->operand_stack->Push_I64(t2);
    execute->stack_frame->operand_stack->Push_I32(t1);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_dup2(Execute* execute) {
    int64_t t=execute->stack_frame->operand_stack->getTop_I64();
    execute->stack_frame->operand_stack->Push_I64(t);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_dup2_x1(Execute* execute) {
    int64_t t1=execute->stack_frame->operand_stack->getTop_I64();
    execute->stack_frame->operand_stack->Pop_64();
    int32_t t2=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    execute->stack_frame->operand_stack->Push_I64(t1);
    execute->stack_frame->operand_stack->Push_I32(t2);
    execute->stack_frame->operand_stack->Push_I64(t1);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_dup2_x2(Execute* execute) {
    int64_t t1=execute->stack_frame->operand_stack->getTop_I64();
    execute->stack_frame->operand_stack->Pop_64();
    int64_t t2=execute->stack_frame->operand_stack->getTop_I64();
    execute->stack_frame->operand_stack->Pop_64();
    execute->stack_frame->operand_stack->Push_I64(t1);
    execute->stack_frame->operand_stack->Push_I64(t2);
    execute->stack_frame->operand_stack->Push_I64(t1);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_swap(Execute* execute) {
    int32_t t1=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    int32_t t2=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    execute->stack_frame->operand_stack->Push_I32(t1);
    execute->stack_frame->operand_stack->Push_I32(t2);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_iadd(Execute* execute) {
    int32_t t1=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    int32_t t2=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    execute->stack_frame->operand_stack->Push_I32(t1+t2);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_ladd(Execute* execute){
    int64_t t1=execute->stack_frame->operand_stack->getTop_I64();
    execute->stack_frame->operand_stack->Pop_64();
    int64_t t2=execute->stack_frame->operand_stack->getTop_I64();
    execute->stack_frame->operand_stack->Pop_64();
    execute->stack_frame->operand_stack->Push_I64(t1+t2);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_fadd(Execute* execute){
    float t1=execute->stack_frame->operand_stack->getTop_F();
    execute->stack_frame->operand_stack->Pop_32();
    float t2=execute->stack_frame->operand_stack->getTop_F();
    execute->stack_frame->operand_stack->Pop_32();
    execute->stack_frame->operand_stack->Push_F(t1+t2);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_dadd(Execute* execute){
    double t1=execute->stack_frame->operand_stack->getTop_D();
    execute->stack_frame->operand_stack->Pop_64();
    double t2=execute->stack_frame->operand_stack->getTop_D();
    execute->stack_frame->operand_stack->Pop_64();
    execute->stack_frame->operand_stack->Push_D(t1+t2);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_isub(Execute* execute){
    int32_t t1=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    int32_t t2=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    execute->stack_frame->operand_stack->Push_I32(t2-t1);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_lsub(Execute* execute){
    int64_t t1=execute->stack_frame->operand_stack->getTop_I64();
    execute->stack_frame->operand_stack->Pop_64();
    int64_t t2=execute->stack_frame->operand_stack->getTop_I64();
    execute->stack_frame->operand_stack->Pop_64();
    execute->stack_frame->operand_stack->Push_I64(t2-t1);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_fsub(Execute* execute){
    float t1=execute->stack_frame->operand_stack->getTop_F();
    execute->stack_frame->operand_stack->Pop_32();
    float t2=execute->stack_frame->operand_stack->getTop_F();
    execute->stack_frame->operand_stack->Pop_32();
    execute->stack_frame->operand_stack->Push_F(t2-t1);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_dsub(Execute* execute){
    double t1=execute->stack_frame->operand_stack->getTop_D();
    execute->stack_frame->operand_stack->Pop_64();
    double t2=execute->stack_frame->operand_stack->getTop_D();
    execute->stack_frame->operand_stack->Pop_64();
    execute->stack_frame->operand_stack->Push_D(t2-t1);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_imul(Execute* execute){
    int32_t t1=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    int32_t t2=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    execute->stack_frame->operand_stack->Push_I32(t1*t2);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_lmul(Execute* execute){
    int64_t t1=execute->stack_frame->operand_stack->getTop_I64();
    execute->stack_frame->operand_stack->Pop_64();
    int64_t t2=execute->stack_frame->operand_stack->getTop_I64();
    execute->stack_frame->operand_stack->Pop_64();
    execute->stack_frame->operand_stack->Push_I64(t1*t2);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_fmul(Execute* execute){
    float t1=execute->stack_frame->operand_stack->getTop_F();
    execute->stack_frame->operand_stack->Pop_32();
    float t2=execute->stack_frame->operand_stack->getTop_F();
    execute->stack_frame->operand_stack->Pop_32();
    execute->stack_frame->operand_stack->Push_F(t1*t2);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_dmul(Execute* execute){
    double t1=execute->stack_frame->operand_stack->getTop_D();
    execute->stack_frame->operand_stack->Pop_64();
    double t2=execute->stack_frame->operand_stack->getTop_D();
    execute->stack_frame->operand_stack->Pop_64();
    execute->stack_frame->operand_stack->Push_D(t1*t2);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_idiv(Execute* execute){
    int32_t t1=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    int32_t t2=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    if (t1==0) {
        std::cerr<<"错误：发生除0错误"<<std::endl;
        //execute->stack_frame->PC++;
        return RUN_ERROR;
    }else {
        execute->stack_frame->operand_stack->Push_I32(t2/t1);
        execute->stack_frame->PC++;
        return RUN_CONTINUE;
    }
}

int I_ldiv(Execute* execute){
    int64_t t1=execute->stack_frame->operand_stack->getTop_I64();
    execute->stack_frame->operand_stack->Pop_64();
    int64_t t2=execute->stack_frame->operand_stack->getTop_I64();
    execute->stack_frame->operand_stack->Pop_64();
    if (t1==0) {
        std::cerr<<"错误：发生除0错误"<<std::endl;
        execute->stack_frame->PC++;
        return RUN_ERROR;
    }else {
        execute->stack_frame->operand_stack->Push_I64(t2/t1);
        execute->stack_frame->PC++;
        return RUN_CONTINUE;
    }
}

int I_fdiv(Execute* execute){
    float t1=execute->stack_frame->operand_stack->getTop_F();
    execute->stack_frame->operand_stack->Pop_32();
    float t2=execute->stack_frame->operand_stack->getTop_F();
    execute->stack_frame->operand_stack->Pop_32();
    execute->stack_frame->operand_stack->Push_F(t2/t1);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_ddiv(Execute* execute){
    double t1=execute->stack_frame->operand_stack->getTop_D();
    execute->stack_frame->operand_stack->Pop_64();
    double t2=execute->stack_frame->operand_stack->getTop_D();
    execute->stack_frame->operand_stack->Pop_64();
    execute->stack_frame->operand_stack->Push_D(t2/t1);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_irem(Execute* execute) {
    int32_t t1=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    int32_t t2=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    if (t1==0) {
        std::cerr<<"错误：发生除0错误"<<std::endl;
        //execute->stack_frame->PC++;
        return RUN_ERROR;
    }else {
        execute->stack_frame->operand_stack->Push_I32(t2%t1);
        execute->stack_frame->PC++;
        return RUN_CONTINUE;
    }
}

int I_lrem(Execute* execute){
    int64_t t1=execute->stack_frame->operand_stack->getTop_I64();
    execute->stack_frame->operand_stack->Pop_64();
    int64_t t2=execute->stack_frame->operand_stack->getTop_I64();
    execute->stack_frame->operand_stack->Pop_64();
    if (t1==0) {
        std::cerr<<"错误：发生除0错误"<<std::endl;
        //execute->stack_frame->PC++;
        return RUN_ERROR;
    }else {
        execute->stack_frame->operand_stack->Push_I64(t2%t1);
        execute->stack_frame->PC++;
        return RUN_CONTINUE;
    }
}

int I_frem(Execute* execute){
    float t1=execute->stack_frame->operand_stack->getTop_F();
    execute->stack_frame->operand_stack->Pop_32();
    float t2=execute->stack_frame->operand_stack->getTop_F();
    execute->stack_frame->operand_stack->Pop_32();
    execute->stack_frame->operand_stack->Push_F(fmodf(t2,t1));
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_drem(Execute* execute){
    double t1=execute->stack_frame->operand_stack->getTop_D();
    execute->stack_frame->operand_stack->Pop_64();
    double t2=execute->stack_frame->operand_stack->getTop_D();
    execute->stack_frame->operand_stack->Pop_64();
    execute->stack_frame->operand_stack->Push_D(fmod(t2,t1));
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_ineg(Execute* execute) {
    int32_t t=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    execute->stack_frame->operand_stack->Push_I32(-t);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_lneg(Execute* execute) {
    int64_t t=execute->stack_frame->operand_stack->getTop_I64();
    execute->stack_frame->operand_stack->Pop_64();
    execute->stack_frame->operand_stack->Push_I64(-t);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_fneg(Execute* execute) {
    float t=execute->stack_frame->operand_stack->getTop_F();
    execute->stack_frame->operand_stack->Pop_32();
    execute->stack_frame->operand_stack->Push_F(-t);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_dneg(Execute* execute) {
    double t=execute->stack_frame->operand_stack->getTop_D();
    execute->stack_frame->operand_stack->Pop_64();
    execute->stack_frame->operand_stack->Push_D(-t);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_ishl(Execute* execute) {
    int32_t t2=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    int32_t t1=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    execute->stack_frame->operand_stack->Push_I32(t1<<(t2&0x1f));
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_lshl(Execute* execute) {
    int32_t t2=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    int64_t t1=execute->stack_frame->operand_stack->getTop_I64();
    execute->stack_frame->operand_stack->Pop_64();
    execute->stack_frame->operand_stack->Push_I64(t1<<(t2&0x3f));
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_ishr(Execute* execute) {
    int32_t t2=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    int32_t t1=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    execute->stack_frame->operand_stack->Push_I32(t1>>(t2&0x1f));
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_lshr(Execute* execute) {
    int32_t t2=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    int64_t t1=execute->stack_frame->operand_stack->getTop_I64();
    execute->stack_frame->operand_stack->Pop_64();
    execute->stack_frame->operand_stack->Push_I64(t1>>(t2&0x3f));
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_iushr(Execute* execute) {
    int32_t t2=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    uint32_t t1=execute->stack_frame->operand_stack->getTop_uI32();
    execute->stack_frame->operand_stack->Pop_32();
    execute->stack_frame->operand_stack->Push_uI32(t1>>(t2&0x1f));
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_lushr(Execute* execute) {
    int32_t t2=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    uint64_t t1=execute->stack_frame->operand_stack->getTop_uI64();
    execute->stack_frame->operand_stack->Pop_64();
    execute->stack_frame->operand_stack->Push_uI64(t1>>(t2&0x3f));
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_iand(Execute* execute) {
    int32_t t2=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    int32_t t1=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    execute->stack_frame->operand_stack->Push_I32(t1&t2);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_land(Execute* execute) {
    int64_t t1=execute->stack_frame->operand_stack->getTop_I64();
    execute->stack_frame->operand_stack->Pop_64();
    int64_t t2=execute->stack_frame->operand_stack->getTop_I64();
    execute->stack_frame->operand_stack->Pop_64();
    execute->stack_frame->operand_stack->Push_I64(t2&t1);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_ior(Execute* execute) {
    int32_t t2=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    int32_t t1=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    execute->stack_frame->operand_stack->Push_I32(t1|t2);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_lor(Execute* execute) {
    int64_t t1=execute->stack_frame->operand_stack->getTop_I64();
    execute->stack_frame->operand_stack->Pop_64();
    int64_t t2=execute->stack_frame->operand_stack->getTop_I64();
    execute->stack_frame->operand_stack->Pop_64();
    execute->stack_frame->operand_stack->Push_I64(t2|t1);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_ixor(Execute* execute) {
    int32_t t2=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    int32_t t1=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    execute->stack_frame->operand_stack->Push_I32(t1^t2);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_lxor(Execute* execute) {
    int64_t t1=execute->stack_frame->operand_stack->getTop_I64();
    execute->stack_frame->operand_stack->Pop_64();
    int64_t t2=execute->stack_frame->operand_stack->getTop_I64();
    execute->stack_frame->operand_stack->Pop_64();
    execute->stack_frame->operand_stack->Push_I64(t2^t1);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_iinc(Execute* execute) {
    int b=execute->stack_frame->PC+1;
    int i=*((uint8_t*)(execute->code+b));
    int32_t c=int32_t(*((int8_t*)(execute->code+b+1)));
    int32_t t=execute->stack_frame->local_variable_table->getI32(i);
    execute->stack_frame->local_variable_table->putI32(t+c,i);
    execute->stack_frame->PC+=3;
    return RUN_CONTINUE;
}

int I_i2l(Execute* execute) {
    int32_t t=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    int64_t r=t;
    execute->stack_frame->operand_stack->Push_I64(r);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_i2f(Execute* execute) {
    int32_t t=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    float r=float(t);
    execute->stack_frame->operand_stack->Push_F(r);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_i2d(Execute* execute) {
    int32_t t=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    double r=double(t);
    execute->stack_frame->operand_stack->Push_D(r);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_l2i(Execute* execute) {
    int64_t t=execute->stack_frame->operand_stack->getTop_I64();
    execute->stack_frame->operand_stack->Pop_64();
    int32_t r=(int32_t)t;
    execute->stack_frame->operand_stack->Push_I32(r);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_l2f(Execute* execute) {
    int64_t t=execute->stack_frame->operand_stack->getTop_I64();
    execute->stack_frame->operand_stack->Pop_64();
    float r=(float)t;
    execute->stack_frame->operand_stack->Push_F(r);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_l2d(Execute* execute) {
    int64_t t=execute->stack_frame->operand_stack->getTop_I64();
    execute->stack_frame->operand_stack->Pop_64();
    double r=(double)t;
    execute->stack_frame->operand_stack->Push_D(r);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_f2i(Execute* execute) {
    float t=execute->stack_frame->operand_stack->getTop_F();
    execute->stack_frame->operand_stack->Pop_32();
    int32_t r=(int32_t)t;
    execute->stack_frame->operand_stack->Push_I32(r);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_f2l(Execute* execute) {
    float t=execute->stack_frame->operand_stack->getTop_F();
    execute->stack_frame->operand_stack->Pop_32();
    int64_t r=t;
    execute->stack_frame->operand_stack->Push_I64(r);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_f2d(Execute* execute) {
    float t=execute->stack_frame->operand_stack->getTop_F();
    execute->stack_frame->operand_stack->Pop_32();
    double r=t;
    execute->stack_frame->operand_stack->Push_D(r);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_d2i(Execute* execute) {
    double t=execute->stack_frame->operand_stack->getTop_D();
    execute->stack_frame->operand_stack->Pop_64();
    int32_t r=(int32_t)t;
    execute->stack_frame->operand_stack->Push_I32(r);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_d2l(Execute* execute) {
    double t=execute->stack_frame->operand_stack->getTop_D();
    execute->stack_frame->operand_stack->Pop_64();
    int64_t r=(int64_t)t;
    execute->stack_frame->operand_stack->Push_I64(r);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_d2f(Execute* execute) {
    double t=execute->stack_frame->operand_stack->getTop_D();
    execute->stack_frame->operand_stack->Pop_64();
    float r=(float)t;
    execute->stack_frame->operand_stack->Push_F(r);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_i2b(Execute* execute) {
    int32_t t=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    int8_t r=(int8_t)t;
    execute->stack_frame->operand_stack->Push_I32(r);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_i2c(Execute* execute) {
    int32_t t=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    uint16_t r=(uint16_t)t;
    execute->stack_frame->operand_stack->Push_I32(r);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_i2s(Execute* execute) {
    int32_t t=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    int16_t r=(int16_t)t;
    execute->stack_frame->operand_stack->Push_I32(r);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_lcmp(Execute* execute) {
    int64_t t2=execute->stack_frame->operand_stack->getTop_I64();
    execute->stack_frame->operand_stack->Pop_64();
    int64_t t1=execute->stack_frame->operand_stack->getTop_I64();
    execute->stack_frame->operand_stack->Pop_64();
    int32_t r=-1;
    if (t1>t2){r=1;}else if (t1==t2){r=0;}
    execute->stack_frame->operand_stack->Push_I32(r);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_fcmpl(Execute* execute) {
    float t2=execute->stack_frame->operand_stack->getTop_F();
    execute->stack_frame->operand_stack->Pop_32();
    float t1=execute->stack_frame->operand_stack->getTop_F();
    execute->stack_frame->operand_stack->Pop_32();
    int32_t r=-1;
    if (_isnanf(t1) || _isnanf(t2)) {
        r=-1;
    }else {
        if (t1>t2){r=1;}else if (t1==t2){r=0;}
    }
    execute->stack_frame->operand_stack->Push_I32(r);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_fcmpg(Execute* execute) {
    float t2=execute->stack_frame->operand_stack->getTop_F();
    execute->stack_frame->operand_stack->Pop_32();
    float t1=execute->stack_frame->operand_stack->getTop_F();
    execute->stack_frame->operand_stack->Pop_32();
    int32_t r=-1;
    if (_isnanf(t1) || _isnanf(t2)) {
        r=1;
    }else {
        if (t1>t2){r=1;}else if (t1==t2){r=0;}
    }
    execute->stack_frame->operand_stack->Push_I32(r);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_dcmpl(Execute* execute) {
    double t2=execute->stack_frame->operand_stack->getTop_D();
    execute->stack_frame->operand_stack->Pop_64();
    double t1=execute->stack_frame->operand_stack->getTop_D();
    execute->stack_frame->operand_stack->Pop_64();
    int32_t r=-1;
    if (_isnan(t1) || _isnan(t2)) {
        r=-1;
    }else {
        if (t1>t2){r=1;}else if (t1==t2){r=0;}
    }
    execute->stack_frame->operand_stack->Push_I32(r);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_dcmpg(Execute* execute) {
    double t2=execute->stack_frame->operand_stack->getTop_D();
    execute->stack_frame->operand_stack->Pop_64();
    double t1=execute->stack_frame->operand_stack->getTop_D();
    execute->stack_frame->operand_stack->Pop_64();
    int32_t r=-1;
    if (_isnan(t1) || _isnan(t2)) {
        r=1;
    }else {
        if (t1>t2){r=1;}else if (t1==t2){r=0;}
    }
    execute->stack_frame->operand_stack->Push_I32(r);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_ifeq(Execute* execute) {
    int b=execute->stack_frame->PC+1;
    uint16_t t1=*((uint8_t*)(execute->code+b)),
    t2=*((uint8_t*)(execute->code+b+1));
    t1=(t1<<8)|t2;
    int32_t v=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    if (v==0) {
        execute->stack_frame->PC+=(int16_t)t1;
    }else {
        execute->stack_frame->PC+=3;
    }
    return RUN_CONTINUE;
}

int I_ifne(Execute* execute) {
    int b=execute->stack_frame->PC+1;
    uint16_t t1=*((uint8_t*)(execute->code+b)),
    t2=*((uint8_t*)(execute->code+b+1));
    t1=(t1<<8)|t2;
    int32_t v=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    if (v!=0) {
        execute->stack_frame->PC+=(int16_t)t1;
    }else {
        execute->stack_frame->PC+=3;
    }
    return RUN_CONTINUE;
}

int I_iflt(Execute* execute) {
    int b=execute->stack_frame->PC+1;
    uint16_t t1=*((uint8_t*)(execute->code+b)),
    t2=*((uint8_t*)(execute->code+b+1));
    t1=(t1<<8)|t2;
    int32_t v=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    if (v<0) {
        execute->stack_frame->PC+=(int16_t)t1;
    }else {
        execute->stack_frame->PC+=3;
    }
    return RUN_CONTINUE;
}

int I_ifge(Execute* execute) {
    int b=execute->stack_frame->PC+1;
    uint16_t t1=*((uint8_t*)(execute->code+b)),
    t2=*((uint8_t*)(execute->code+b+1));
    t1=(t1<<8)|t2;
    int32_t v=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    if (v>=0) {
        execute->stack_frame->PC+=(int16_t)t1;
    }else {
        execute->stack_frame->PC+=3;
    }
    return RUN_CONTINUE;
}

int I_ifgt(Execute* execute) {
    int b=execute->stack_frame->PC+1;
    uint16_t t1=*((uint8_t*)(execute->code+b)),
    t2=*((uint8_t*)(execute->code+b+1));
    t1=(t1<<8)|t2;
    int32_t v=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    if (v>0) {
        execute->stack_frame->PC+=(int16_t)t1;
    }else {
        execute->stack_frame->PC+=3;
    }
    return RUN_CONTINUE;
}

int I_ifle(Execute* execute) {
    int b=execute->stack_frame->PC+1;
    uint16_t t1=*((uint8_t*)(execute->code+b)),
    t2=*((uint8_t*)(execute->code+b+1));
    t1=(t1<<8)|t2;
    int32_t v=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    if (v<=0) {
        execute->stack_frame->PC+=(int16_t)t1;
    }else {
        execute->stack_frame->PC+=3;
    }
    return RUN_CONTINUE;
}

int I_if_icmpeq(Execute* execute) {
    int b=execute->stack_frame->PC+1;
    uint16_t t1=*((uint8_t*)(execute->code+b)),
    t2=*((uint8_t*)(execute->code+b+1));
    t1=(t1<<8)|t2;
    int32_t v2=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    int32_t v1=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    if (v1==v2) {
        execute->stack_frame->PC+=(int16_t)t1;
    }else {
        execute->stack_frame->PC+=3;
    }
    return RUN_CONTINUE;
}

int I_if_icmpne(Execute* execute) {
    int b=execute->stack_frame->PC+1;
    uint16_t t1=*((uint8_t*)(execute->code+b)),
    t2=*((uint8_t*)(execute->code+b+1));
    t1=(t1<<8)|t2;
    int32_t v2=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    int32_t v1=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    if (v1!=v2) {
        execute->stack_frame->PC+=(int16_t)t1;
    }else {
        execute->stack_frame->PC+=3;
    }
    return RUN_CONTINUE;
}

int I_if_icmplt(Execute* execute) {
    int b=execute->stack_frame->PC+1;
    uint16_t t1=*((uint8_t*)(execute->code+b)),
    t2=*((uint8_t*)(execute->code+b+1));
    t1=(t1<<8)|t2;
    int32_t v2=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    int32_t v1=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    if (v1<v2) {
        execute->stack_frame->PC+=(int16_t)t1;
    }else {
        execute->stack_frame->PC+=3;
    }
    return RUN_CONTINUE;
}

int I_if_icmpge(Execute* execute) {
    int b=execute->stack_frame->PC+1;
    uint16_t t1=*((uint8_t*)(execute->code+b)),
    t2=*((uint8_t*)(execute->code+b+1));
    t1=(t1<<8)|t2;
    int32_t v2=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    int32_t v1=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    if (v1>=v2) {
        execute->stack_frame->PC+=(int16_t)t1;
    }else {
        execute->stack_frame->PC+=3;
    }
    return RUN_CONTINUE;
}

int I_if_icmpgt(Execute* execute) {
    int b=execute->stack_frame->PC+1;
    uint16_t t1=*((uint8_t*)(execute->code+b)),
    t2=*((uint8_t*)(execute->code+b+1));
    t1=(t1<<8)|t2;
    int32_t v2=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    int32_t v1=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    if (v1>v2) {
        execute->stack_frame->PC+=(int16_t)t1;
    }else {
        execute->stack_frame->PC+=3;
    }
    return RUN_CONTINUE;
}

int I_if_icmple(Execute* execute) {
    int b=execute->stack_frame->PC+1;
    uint16_t t1=*((uint8_t*)(execute->code+b)),
    t2=*((uint8_t*)(execute->code+b+1));
    t1=(t1<<8)|t2;
    int32_t v2=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    int32_t v1=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    if (v1<=v2) {
        execute->stack_frame->PC+=(int16_t)t1;
    }else {
        execute->stack_frame->PC+=3;
    }
    return RUN_CONTINUE;
}

int I_if_acmpeq(Execute* execute) {
    int b=execute->stack_frame->PC+1;
    uint16_t t1=*((uint8_t*)(execute->code+b)),
    t2=*((uint8_t*)(execute->code+b+1));
    t1=(t1<<8)|t2;
    uint32_t v2=execute->stack_frame->operand_stack->getTop_uI32();
    execute->stack_frame->operand_stack->Pop_32();
    uint32_t v1=execute->stack_frame->operand_stack->getTop_uI32();
    execute->stack_frame->operand_stack->Pop_32();
    if (v1==v2) {
        execute->stack_frame->PC+=(int16_t)t1;
    }else {
        execute->stack_frame->PC+=3;
    }
    return RUN_CONTINUE;
}

int I_if_acmpne(Execute* execute) {
    int b=execute->stack_frame->PC+1;
    uint16_t t1=*((uint8_t*)(execute->code+b)),
    t2=*((uint8_t*)(execute->code+b+1));
    t1=(t1<<8)|t2;
    uint32_t v2=execute->stack_frame->operand_stack->getTop_uI32();
    execute->stack_frame->operand_stack->Pop_32();
    uint32_t v1=execute->stack_frame->operand_stack->getTop_uI32();
    execute->stack_frame->operand_stack->Pop_32();
    if (v1!=v2) {
        execute->stack_frame->PC+=(int16_t)t1;
    }else {
        execute->stack_frame->PC+=3;
    }
    return RUN_CONTINUE;
}

int I_goto(Execute* execute) {
    int b=execute->stack_frame->PC+1;
    uint16_t t1=*((uint8_t*)(execute->code+b)),
    t2=*((uint8_t*)(execute->code+b+1));
    t1=(t1<<8)|t2;
    execute->stack_frame->PC+=(int16_t)t1;
    return RUN_CONTINUE;
}

int I_jsr(Execute* execute) {
    int b=execute->stack_frame->PC+1;
    uint16_t t1=*((uint8_t*)(execute->code+b)),
    t2=*((uint8_t*)(execute->code+b+1));
    t1=(t1<<8)|t2;
    execute->stack_frame->operand_stack->Push_I32(execute->stack_frame->PC+3);
    execute->stack_frame->PC+=(int16_t)t1;
    return RUN_CONTINUE;
}

int I_ret(Execute* execute) {
    int b=execute->stack_frame->PC+1;
    uint8_t i=*((uint8_t*)(execute->code+b));
    int32_t t=execute->stack_frame->local_variable_table->getI32(i);
    execute->stack_frame->PC=t;
    return RUN_CONTINUE;
}

int32_t I_tableswitch_read4(const char* ptr) {
    uint32_t t1=*((uint8_t*)ptr),t2=*((uint8_t*)(ptr+1)),
    t3=*((uint8_t*)(ptr+2)),t4=*((uint8_t*)(ptr+3));
    uint32_t t=(t1<<24)|(t2<<16)|(t3<<8)|(t4);
    return int32_t(t);
}
int I_tableswitch(Execute* execute) {
    int b=execute->stack_frame->PC;
    if (b%4!=0){b=((b>>2)+1)<<2;}
    const char* ptr=execute->code+b;
    int32_t defaults=I_tableswitch_read4(ptr),
    low=I_tableswitch_read4(ptr+4),
    high=I_tableswitch_read4(ptr+8);
    int32_t bias=0,
    i=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    if (i<low || i>high){bias=defaults;}
    else {
        int j=i-low;
        bias=I_tableswitch_read4(ptr+12+4*j);
    }
    execute->stack_frame->PC+=bias;
    return RUN_CONTINUE;
}

int I_lookupswitch(Execute* execute) {
    int b=execute->stack_frame->PC+1;
    if (b%4!=0){b=((b>>2)+1)<<2;}
    const char* ptr=execute->code+b;
    int32_t defaults=I_tableswitch_read4(ptr),
    np=I_tableswitch_read4(ptr+4);
    int32_t bias=0,
    key=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    {//二分查找，[ptr+8------ptr+8+8*np]
        int i=0,j=np-1,mid=0,find=0;
        int32_t tp;
        while (i<=j) {
            mid=(i+j)>>1;
            tp=I_tableswitch_read4(ptr+8+(mid<<3));
            if (key<tp) {
                j=mid-1;
            }else if (key>tp) {
                i=mid+1;
            }else {
                find=1;
                break;
            }
        }
        if (find) {
            bias=I_tableswitch_read4(ptr+12+(mid<<3));
        }else {
            bias=defaults;
        }
    }
    execute->stack_frame->PC+=bias;
    return RUN_CONTINUE;
}

int I_ireturn(Execute* execute) {
    int32_t re=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack.pop();
    delete execute->stack_frame;execute->stack_frame=nullptr;execute->code=nullptr;
    if (!(execute->stack.empty())) {
        execute->stack_frame=execute->stack.top();
        execute->code=execute->stack_frame->code;
        execute->stack_frame->operand_stack->Push_I32(re);
    }else {
        auto* rp=(int32_t*)(&(execute->lastResult));
        *rp=re;
    }
    return RUN_CONTINUE;
}

int I_lreturn(Execute* execute) {
    int64_t re=execute->stack_frame->operand_stack->getTop_I64();
    execute->stack.pop();
    delete execute->stack_frame;execute->stack_frame=nullptr;execute->code=nullptr;
    if (!(execute->stack.empty())) {
        execute->stack_frame=execute->stack.top();
        execute->code=execute->stack_frame->code;
        execute->stack_frame->operand_stack->Push_I64(re);
    }else {
        auto* rp=(int64_t*)(&(execute->lastResult));
        *rp=re;
    }
    return RUN_CONTINUE;
}

int I_freturn(Execute* execute) {
    float re=execute->stack_frame->operand_stack->getTop_F();
    execute->stack.pop();
    delete execute->stack_frame;execute->stack_frame=nullptr;execute->code=nullptr;
    if (!(execute->stack.empty())) {
        execute->stack_frame=execute->stack.top();
        execute->code=execute->stack_frame->code;
        execute->stack_frame->operand_stack->Push_F(re);
    }else {
        auto* rp=(float*)(&(execute->lastResult));
        *rp=re;
    }
    return RUN_CONTINUE;
}

int I_dreturn(Execute* execute) {
    double re=execute->stack_frame->operand_stack->getTop_D();
    execute->stack.pop();
    delete execute->stack_frame;execute->stack_frame=nullptr;execute->code=nullptr;
    if (!(execute->stack.empty())) {
        execute->stack_frame=execute->stack.top();
        execute->code=execute->stack_frame->code;
        execute->stack_frame->operand_stack->Push_D(re);
    }else {
        auto* rp=(double*)(&(execute->lastResult));
        *rp=re;
    }
    return RUN_CONTINUE;
}

int I_areturn(Execute* execute) {
    uint32_t re=execute->stack_frame->operand_stack->getTop_uI32();
    execute->stack.pop();
    delete execute->stack_frame;execute->stack_frame=nullptr;execute->code=nullptr;
    if (!(execute->stack.empty())) {
        execute->stack_frame=execute->stack.top();
        execute->code=execute->stack_frame->code;
        execute->stack_frame->operand_stack->Push_uI32(re);
    }else {
        auto* rp=(uint32_t*)(&(execute->lastResult));
        *rp=re;
    }
    return RUN_CONTINUE;
}

int I_return(Execute* execute) {
    execute->stack.pop();
    delete execute->stack_frame;
    execute->stack_frame=nullptr;
    execute->code=nullptr;
    if (!(execute->stack.empty())) {
        execute->stack_frame=execute->stack.top();
        execute->code=execute->stack_frame->code;
    }
    return RUN_CONTINUE;
}

int I_getstatic(Execute* execute) {
    instanceClass* cla=(instanceClass*)(execute->stack_frame->Class);
    int b=execute->stack_frame->PC+1;
    uint16_t t1=*((uint8_t*)(execute->code+b)),
    t2=*((uint8_t*)(execute->code+b+1));
    t1=(t1<<8)|t2;
    MCP_Fieldref* fr=(MCP_Fieldref*)((cla->constantPool->pool)[t1]);
    instanceClass* instance_class=(instanceClass*)my_class_loader.getInstanceClass(
        fr->className);
    if (instance_class==nullptr) {
        std::cerr<<"getstatic找不到类"<<fr->className<<std::endl;
        return RUN_ERROR;
    }

    Kclass* FindField_ClassPtr=instance_class;
    auto sf=instance_class->staticSlotMap.end();
    while (FindField_ClassPtr) {
        std::string key=FindField_ClassPtr->name+"."+fr->name+":"+fr->type;
        sf=instance_class->staticSlotMap.find(key);
        if (sf==instance_class->staticSlotMap.end()) {
            FindField_ClassPtr=FindField_ClassPtr->father;
        }else {
            break;
        }
    }
    if (sf==instance_class->staticSlotMap.end()) {
        std::cerr<<"在getstatic中"<<fr->className<<"."<<fr->name<<"不存在"<<std::endl;
        return RUN_ERROR;
    }

    int si=(*sf).second;
    int ty=TypeMapToSlotSize[(fr->type)[0]];
    if (ty==1) {
        execute->stack_frame->operand_stack->Push_uI32(
            instance_class->staticSlots->getuI32(si));
    }else {
        execute->stack_frame->operand_stack->Push_uI64(
            instance_class->staticSlots->getuI64(si));
    }
    execute->stack_frame->PC+=3;
    return RUN_CONTINUE;
}

int I_putstatic(Execute* execute) {
    instanceClass* cla=(instanceClass*)(execute->stack_frame->Class);
    int b=execute->stack_frame->PC+1;
    uint16_t t1=*((uint8_t*)(execute->code+b)),
    t2=*((uint8_t*)(execute->code+b+1));
    t1=(t1<<8)|t2;
    MCP_Fieldref* fr=(MCP_Fieldref*)((cla->constantPool->pool)[t1]);
    instanceClass* instance_class=(instanceClass*)my_class_loader.getInstanceClass(
        fr->className);
    if (instance_class==nullptr) {
        std::cerr<<"putstatic找不到类"<<fr->className<<std::endl;
        return RUN_ERROR;
    }

    Kclass* FindField_ClassPtr=instance_class;
    auto sf=instance_class->staticSlotMap.end();
    while (FindField_ClassPtr) {
        std::string key=FindField_ClassPtr->name+"."+fr->name+":"+fr->type;
        sf=instance_class->staticSlotMap.find(key);
        if (sf==instance_class->staticSlotMap.end()) {
            FindField_ClassPtr=FindField_ClassPtr->father;
        }else {
            break;
        }
    }
    if (sf==instance_class->staticSlotMap.end()) {
        std::cerr<<"在putstatic中"<<fr->className<<"."<<fr->name<<"不存在"<<std::endl;
        return RUN_ERROR;
    }

    int si=(*sf).second;
    int ty=TypeMapToSlotSize[(fr->type)[0]];
    if (ty==1) {
        instance_class->staticSlots->putuI32(
            execute->stack_frame->operand_stack->getTop_uI32(),si);
        execute->stack_frame->operand_stack->Pop_32();
    }else {
        instance_class->staticSlots->putuI64(
            execute->stack_frame->operand_stack->getTop_uI64(),si);
        execute->stack_frame->operand_stack->Pop_64();
    }
    execute->stack_frame->PC+=3;
    return RUN_CONTINUE;
}

int I_getfield(Execute* execute) {
    uint32_t ref=execute->stack_frame->operand_stack->getTop_uI32();
    execute->stack_frame->operand_stack->Pop_32();
    if (ref<=0) {
        std::cerr<<"getfield空指针错误"<<std::endl;
        return RUN_ERROR;
    }
    instanceOopDesc* ins=(instanceOopDesc*)(heap.getRowPointer(ref));
    instanceClass* cla=(instanceClass*)(execute->stack_frame->Class),
    *insClass=(instanceClass*)(ins->kclass);
    int b=execute->stack_frame->PC+1;
    uint16_t t1=*((uint8_t*)(execute->code+b)),
    t2=*((uint8_t*)(execute->code+b+1));
    t1=(t1<<8)|t2;
    MCP_Fieldref* fr=(MCP_Fieldref*)((cla->constantPool->pool)[t1]);

    Kclass* FindField_ClassPtr=insClass;
    auto fii=insClass->instanceSlotMap.end();
    while (FindField_ClassPtr) {
        std::string key=FindField_ClassPtr->name+"."+fr->name+":"+fr->type;
        fii=insClass->instanceSlotMap.find(key);
        if (fii==insClass->instanceSlotMap.end()) {
            FindField_ClassPtr=FindField_ClassPtr->father;
        }else {
            break;
        }
    }
    if (fii==insClass->instanceSlotMap.end()) {
        std::cerr<<"在getfield中"<<fr->className<<"."<<fr->name<<"不存在"<<std::endl;
        return RUN_ERROR;
    }
    int ii=(*fii).second;
    int ty=TypeMapToSlotSize[(fr->type)[0]];
    if (ty==1) {
        execute->stack_frame->operand_stack->Push_uI32(ins->instanceSlots->getuI32(ii));
    }else {
        execute->stack_frame->operand_stack->Push_uI64(ins->instanceSlots->getuI64(ii));
    }
    execute->stack_frame->PC+=3;
    return RUN_CONTINUE;
}

int I_putfield(Execute* execute) {
    instanceClass* cla=(instanceClass*)(execute->stack_frame->Class);
    int b=execute->stack_frame->PC+1;
    uint16_t t1=*((uint8_t*)(execute->code+b)),
    t2=*((uint8_t*)(execute->code+b+1));
    t1=(t1<<8)|t2;
    MCP_Fieldref* fr=(MCP_Fieldref*)((cla->constantPool->pool)[t1]);
    int ty=TypeMapToSlotSize[(fr->type)[0]];
    uint32_t v32=0;
    uint64_t v64=0;
    if (ty==1) {
        v32=execute->stack_frame->operand_stack->getTop_uI32();
        execute->stack_frame->operand_stack->Pop_32();
    }else {
        v64=execute->stack_frame->operand_stack->getTop_uI64();
        execute->stack_frame->operand_stack->Pop_64();
    }
    uint32_t ref=execute->stack_frame->operand_stack->getTop_uI32();
    execute->stack_frame->operand_stack->Pop_32();
    if (ref<=0) {
        std::cerr<<"putfield空指针异常"<<std::endl;
        return RUN_ERROR;
    }
    instanceOopDesc* ins=(instanceOopDesc*)(heap.getRowPointer(ref));
    instanceClass* insClass=(instanceClass*)(ins->kclass);
    Kclass* FindField_ClassPtr=insClass;
    auto fii=insClass->instanceSlotMap.end();
    while (FindField_ClassPtr) {
        std::string key=FindField_ClassPtr->name+"."+fr->name+":"+fr->type;
        fii=insClass->instanceSlotMap.find(key);
        if (fii==insClass->instanceSlotMap.end()) {
            FindField_ClassPtr=FindField_ClassPtr->father;
        }else {
            break;
        }
    }
    if (fii==insClass->instanceSlotMap.end()) {
        std::cerr<<"在getfield中"<<fr->className<<"."<<fr->name<<"不存在"<<std::endl;
        return RUN_ERROR;
    }
    int ii=(*fii).second;
    if (ty==1) {
        ins->instanceSlots->putuI32(v32,ii);
    }else {
        ins->instanceSlots->putuI64(v64,ii);
    }
    execute->stack_frame->PC+=3;
    return RUN_CONTINUE;
}

int invokeAid_getTypesCountFromStr(const std::string& typeStr) {
    int i=0,j=0;
    for (i = 0; i < typeStr.size(); ++i) {
        if (typeStr[i]==')'){j=i;break;}
    }
    int re=0;
    i=1;--j;
    //{'B',1},{'C',1},{'F',1},{'I',1},{'S',1},{'Z',1}, {'D',2},{'J',2} ,{'L',1},{'[',1}
    int k=0;
    while (i<=j) {
        k=i;
        ++re;
        if (typeStr[k]=='D'||typeStr[k]=='J'){++re;}
        if (typeStr[k]=='L') {
            while (typeStr[k]!=';')++k;
        }
        if (typeStr[k]=='[') {
            while (typeStr[k]=='[')++k;
            if (typeStr[k]=='L') {
                while (typeStr[k]!=';')++k;
            }
        }
        i=k+1;
    }
    return re;
}
int I_invokevirtual(Execute* execute) {
    //O obj=new F();
    //obj.func();--> invokevirtual <O.func:()V>
    //  O->A->B->C->D->E->F
    //              ^     ^
    //            目标方法  |
    //                  实际对象
    //从F开始进行查找，到D就成功了
    instanceClass* cla=(instanceClass*)(execute->stack_frame->Class);
    int b=execute->stack_frame->PC+1;
    uint16_t t1=*((uint8_t*)(execute->code+b)),
    t2=*((uint8_t*)(execute->code+b+1));
    t1=(t1<<8)|t2;
    MCP_Methodref* fr=(MCP_Methodref*)((cla->constantPool->pool)[t1]);
    int slotC=invokeAid_getTypesCountFromStr(fr->type);
    uint32_t* temp=(uint32_t*)malloc(slotC<<2);
    execute->stack_frame->operand_stack->getTop_uI32n(temp,slotC);
    execute->stack_frame->operand_stack->Pop_32n(slotC);
    uint32_t objRef=execute->stack_frame->operand_stack->getTop_uI32();
    execute->stack_frame->operand_stack->Pop_32();
    if (objRef==0) {
        std::cerr<<"invokevirtual空指针错误"<<std::endl;
        return RUN_ERROR;
    }
    OopDesc* OopDptr=(OopDesc*)(heap.getRowPointer(objRef));
    instanceClass* instance_class;
    if (OopDptr->kclass->name[0]=='[') {
        //数组
        instance_class=(instanceClass*)(OopDptr->kclass->father);
    }else {
        instance_class=(instanceClass*)(OopDptr->kclass);
    }
    const MetaMethod* targetM=nullptr;
    while (instance_class) {
        for (const auto i:instance_class->methods) {
            if (i->name==fr->name && i->descriptor==fr->type) {
                targetM=i;
                break;
            }
        }
        if (targetM)break;
        instance_class=(instanceClass*)instance_class->father;
    }
    if (targetM) {
        execute->stack_frame->PC+=3;
        bool isN=((targetM->accessFlags & 0x0100)?true:false);
        StackFrame* newS=nullptr;
        if (isN) {
            newS=new StackFrame(slotC+1,
                0,nullptr,isN,
                instance_class,nullptr,targetM);
            execute->code=nullptr;
        }else {
            newS=new StackFrame(targetM->maxLocalVTSize,
                targetM->maxOpStackSize,targetM->code->code,isN,
                instance_class,nullptr,targetM);
            execute->code=targetM->code->code;
        }
        execute->stack.emplace(newS);
        execute->stack_frame=newS;
        execute->stack_frame->local_variable_table->putuI32(objRef,0);
        execute->stack_frame->local_variable_table->putuI32n(temp,slotC,1);
        free(temp);
        return (isN?RUN_NATIVE:RUN_CONTINUE);
    }else {
        free(temp);
        std::cerr<<"方法"<<fr->className<<"."<<fr->name<<":"<<fr->type<<"找不到"<<std::endl;
        execute->stack_frame->PC+=3;
        return RUN_ERROR;
    }
};

int I_invokespecial(Execute* execute) {
    //super.func();--> invokespecial <E.func:()V>
    //  O->A->B->C->D->E->F
    //                    ^
    //                  实际对象
    //从F的父亲E开始进行查找，到D就成功了
    instanceClass* cla=(instanceClass*)(execute->stack_frame->Class);
    int b=execute->stack_frame->PC+1;
    uint16_t t1=*((uint8_t*)(execute->code+b)),
    t2=*((uint8_t*)(execute->code+b+1));
    t1=(t1<<8)|t2;
    MCP_Methodref* fr=(MCP_Methodref*)((cla->constantPool->pool)[t1]);
    int slotC=invokeAid_getTypesCountFromStr(fr->type);
    uint32_t* temp=(uint32_t*)malloc(slotC<<2);
    execute->stack_frame->operand_stack->getTop_uI32n(temp,slotC);
    execute->stack_frame->operand_stack->Pop_32n(slotC);
    uint32_t objRef=execute->stack_frame->operand_stack->getTop_uI32();
    execute->stack_frame->operand_stack->Pop_32();
    if (objRef<=0) {
        std::cerr<<"invokespecial空指针错误"<<std::endl;
        return RUN_ERROR;
    }
    instanceClass* instance_class=(instanceClass*)
        my_class_loader.getInstanceClass(fr->className);
    if (instance_class==nullptr) {
        std::cerr<<"invokespecial找不到"<<fr->className<<std::endl;
        return RUN_ERROR;
    }
    const MetaMethod* targetM=nullptr;
    std::stack<const instanceClass*> sta;
    sta.emplace(instance_class);
    while (!sta.empty()) {
        const instanceClass* top=sta.top();sta.pop();
        for (const auto i:top->methods) {
            if (i->name==fr->name && i->descriptor==fr->type) {
                targetM=i;
                break;
            }
        }
        if (targetM!=nullptr)break;
        for (const auto i:instance_class->interfaces) {
            if (i)sta.emplace(i);
        }
        if (instance_class->father)sta.emplace((instanceClass*)(instance_class));
    }
    if (targetM) {
        execute->stack_frame->PC+=3;
        bool isN=((targetM->accessFlags & 0x0100)?true:false);
        OopDesc* OopDptr=(OopDesc*)(heap.getRowPointer(objRef));
        StackFrame* newS=nullptr;
        if (isN) {
            newS=new StackFrame(slotC+1,
                0,nullptr,isN,
                instance_class,nullptr,targetM);
            execute->code=nullptr;
        }else {
            newS=new StackFrame(targetM->maxLocalVTSize,
                targetM->maxOpStackSize,targetM->code->code,isN,
                instance_class,nullptr,targetM);
            execute->code=targetM->code->code;
        }
        execute->stack.emplace(newS);
        execute->stack_frame=newS;
        execute->stack_frame->local_variable_table->putuI32(objRef,0);
        execute->stack_frame->local_variable_table->putuI32n(temp,slotC,1);
        free(temp);
        return (isN?RUN_NATIVE:RUN_CONTINUE);
    }else {
        free(temp);
        std::cerr<<"方法"<<fr->className<<"."<<fr->name<<":"<<fr->type<<"找不到"<<std::endl;
        execute->stack_frame->PC+=3;
        return RUN_ERROR;
    }
};

int I_invokestatic(Execute* execute) {
    instanceClass* cla=(instanceClass*)(execute->stack_frame->Class);
    int b=execute->stack_frame->PC+1;
    uint16_t t1=*((uint8_t*)(execute->code+b)),
    t2=*((uint8_t*)(execute->code+b+1));
    t1=(t1<<8)|t2;
    MCP_Methodref* fr=(MCP_Methodref*)((cla->constantPool->pool)[t1]);
    int slotC=invokeAid_getTypesCountFromStr(fr->type);
    uint32_t* temp=(uint32_t*)malloc(slotC<<2);
    execute->stack_frame->operand_stack->getTop_uI32n(temp,slotC);
    execute->stack_frame->operand_stack->Pop_32n(slotC);
    instanceClass* instance_class=(instanceClass*)
        my_class_loader.getInstanceClass(fr->className);
    const MetaMethod* targetM=nullptr;
    while (instance_class) {
        int f=0;
        for (const auto i:instance_class->methods) {
            if (i->name==fr->name && i->descriptor==fr->type) {
                targetM=i;f=1;
                break;
            }
        }
        if (f)break;
        instance_class=(instanceClass*)instance_class->father;
    }
    if (targetM) {
        execute->stack_frame->PC+=3;
        bool isN=((targetM->accessFlags & 0x0100)?true:false);
        StackFrame* newS=nullptr;
        if (isN) {
            newS=new StackFrame(slotC,
                0,nullptr,isN,
                instance_class,nullptr,targetM);
            execute->code=nullptr;
        }else {
            newS=new StackFrame(targetM->maxLocalVTSize,
                targetM->maxOpStackSize,targetM->code->code,isN,
                instance_class,nullptr,targetM);
            execute->code=targetM->code->code;
        }
        execute->stack.emplace(newS);
        execute->stack_frame=newS;
        execute->stack_frame->local_variable_table->putuI32n(temp,slotC,0);
        free(temp);
        return (isN?RUN_NATIVE:RUN_CONTINUE);
    }else {
        free(temp);
        std::cerr<<"方法"<<fr->className<<"."<<fr->name<<":"<<fr->type<<"找不到"<<std::endl;
        execute->stack_frame->PC+=3;
        return RUN_ERROR;
    }
};

const MetaMethod* findInterfaceFunc(const instanceClass* instance_class,
    const std::string& name,
    const std::string& describe,
    instanceClass** tarClass) {
    //先找找自己
    const MetaMethod* re=nullptr;
    std::stack<const instanceClass*> sta;
    if (instance_class)sta.emplace(instance_class);
    while (!sta.empty()) {
        const instanceClass* top=sta.top();sta.pop();
        for (const auto i:top->methods) {
            if (i->name==name
                && i->descriptor==describe
                && (i->accessFlags&0x0400)==0) {
                re=i;*tarClass=(instanceClass*)top;
            }
        }
        if (re)break;
        for (const auto i:top->interfaces) {
            if (i)sta.emplace(i);
        }
        if ((top->accessFlag & 0x0600) != 0x0600) {
            //top不是接口interface
            if (top->father)sta.emplace((instanceClass*)(top->father));
        }
    }
    return re;
}
int I_invokeinterface(Execute* execute) {
    instanceClass* cla=(instanceClass*)(execute->stack_frame->Class);
    int b=execute->stack_frame->PC+1;
    uint16_t t1=*((uint8_t*)(execute->code+b)),
    t2=*((uint8_t*)(execute->code+b+1));
    t1=(t1<<8)|t2;
    MCP_Methodref* fr=(MCP_Methodref*)((cla->constantPool->pool)[t1]);
    int slotC=invokeAid_getTypesCountFromStr(fr->type);
    uint32_t* temp=(uint32_t*)malloc(slotC<<2);
    execute->stack_frame->operand_stack->getTop_uI32n(temp,slotC);
    execute->stack_frame->operand_stack->Pop_32n(slotC);
    uint32_t objRef=execute->stack_frame->operand_stack->getTop_uI32();
    execute->stack_frame->operand_stack->Pop_32();
    if (objRef<=0) {
        std::cerr<<"invokeinterface空指针错误"<<std::endl;
        return RUN_ERROR;
    }
    OopDesc* OopDptr=(OopDesc*)(heap.getRowPointer(objRef));
    instanceClass* instance_class=(instanceClass*)(OopDptr->kclass),*tarClass;
    const MetaMethod* targetM=findInterfaceFunc(instance_class,fr->name,fr->type,&tarClass);
    if (targetM) {
        execute->stack_frame->PC+=5;
        bool isN=((targetM->accessFlags & 0x0100)?true:false);
        StackFrame* newS=new StackFrame(targetM->maxLocalVTSize,
            targetM->maxOpStackSize,targetM->code->code,isN,
            tarClass,OopDptr,targetM);
        execute->stack.emplace(newS);
        execute->stack_frame=newS;
        execute->code=targetM->code->code;
        execute->stack_frame->local_variable_table->putuI32(objRef,0);
        execute->stack_frame->local_variable_table->putuI32n(temp,slotC,1);
        if (isN){execute->runState=RUN_NATIVE;}
        free(temp);
        return (isN?RUN_NATIVE:RUN_CONTINUE);
    }else {
        free(temp);
        std::cerr<<"方法"<<fr->className<<"."<<fr->name<<":"<<fr->type<<"找不到"<<std::endl;
        execute->stack_frame->PC+=5;
        return RUN_ERROR;
    }
};

int I_invokedynamic(Execute* execute) {
    std::cerr<<"invokedynamic指令未实现"<<std::endl;
    return RUN_INSTRUCTION_UNDO;
};

int I_new(Execute* execute) {
    instanceClass* Ccla=(instanceClass*)(execute->stack_frame->Class);
    int b=execute->stack_frame->PC+1;
    uint16_t t1=*((uint8_t*)(execute->code+b)),
    t2=*((uint8_t*)(execute->code+b+1));
    t1=(t1<<8)|t2;
    MCP_Class* cl=(MCP_Class*)((Ccla->constantPool->pool)[t1]);
    instanceClass* insClass=(instanceClass*)(
        my_class_loader.getInstanceClass(cl->name));
    if (insClass==nullptr) {
        std::cerr<<"找不到类"<<cl->name<<std::endl;
        return RUN_ERROR;
    }
    uint32_t rp=heap.NewObj_Instance(
        sizeof(instanceOopDesc)+((insClass->instanceSlotSize)<<2));
    if (rp<=0) {
        std::cerr<<"new申请空间失败"<<std::endl;
        return RUN_ERROR;
    }
    execute->stack_frame->operand_stack->Push_uI32(rp);
    instanceOopDesc* insOop=(instanceOopDesc*)(heap.getRowPointer(rp));
    insOop->kclass=insClass;
    insOop->runTimeInfo=0;
    insOop->instanceSlots=new Slots(insClass->instanceSlotSize,
        ((char*)insOop)+sizeof(instanceOopDesc));
    execute->stack_frame->PC+=3;
    return RUN_CONTINUE;
}

int I_newarray(Execute* execute) {
    int b=execute->stack_frame->PC+1;
    uint8_t atype=*((uint8_t*)(execute->code+b));
    int32_t count=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    int eleS=1;//一个元素占多少个字节
    std::string type;
    switch (atype) {
        //{'B',1},{'C',2},{'D',8},{'F',4},{'I',4},{'J',8},{'L',4},{'S',2},{'Z',1},{'[',4}
        //B byte    Z bool    J long
        case 4://bool
            eleS=1;
            type="Z";
            break;
        case 5:
            eleS=2;
            type="C";
            break;
        case 6:
            eleS=4;
            type="F";
            break;
        case 7:
            eleS=8;
            type="D";
            break;
        case 8:
            eleS=1;
            type="B";
            break;
        case 9:
            eleS=2;
            type="S";
            break;
        case 10:
            eleS=4;
            type="I";
            break;
        case 11:
            eleS=8;
            type="J";
            break;
        default:
            break;
    };
    uint32_t rp=heap.NewObj_Array(sizeof(ArrayOop)+eleS*count);
    if (rp<=0) {
        std::cerr<<"newarray申请空间失败"<<std::endl;
        return RUN_ERROR;
    }
    ArrayOop* aoop=(ArrayOop*)(heap.getRowPointer(rp));
    aoop->runTimeInfo=0;
    //aoop->kclass=my_class_loader.loadedClass["["];
    aoop->kclass=my_class_loader.getInstanceClass("["+type);
    if (aoop->kclass==nullptr) {
        std::cerr<<"newarray找不到"<<("["+type)<<std::endl;
        return RUN_ERROR;
    }
    aoop->eleCount=count;
    aoop->elements.ptr=(char*)aoop+sizeof(ArrayOop);
    execute->stack_frame->operand_stack->Push_uI32(rp);
    execute->stack_frame->PC+=2;
    return RUN_CONTINUE;
}

int I_anewarray(Execute* execute) {
    instanceClass* Ccla=(instanceClass*)(execute->stack_frame->Class);
    int b=execute->stack_frame->PC+1;
    uint16_t t1=*((uint8_t*)(execute->code+b)),
    t2=*((uint8_t*)(execute->code+b+1));
    t1=(t1<<8)|t2;
    MCP_Class* cl=(MCP_Class*)((Ccla->constantPool->pool)[t1]);
    int32_t count=execute->stack_frame->operand_stack->getTop_I32();
    execute->stack_frame->operand_stack->Pop_32();
    if (count<0) {
        std::cerr<<"anewarray元素个数错误"<<std::endl;
        return RUN_ERROR;
    }
    uint32_t rp=heap.NewObj_Array(sizeof(ArrayOop)+(count<<2));
    if (rp<=0) {
        std::cerr<<"anewarray申请空间失败"<<std::endl;
        return RUN_ERROR;
    }
    std::string typeStr=cl->name;
    if (cl->name[cl->name.size()-1]!=';') {
        typeStr='L'+typeStr+';';
    }
    ArrayOop* aoop=(ArrayOop*)(heap.getRowPointer(rp));
    aoop->runTimeInfo=0;
    aoop->kclass=my_class_loader.getInstanceClass("["+typeStr);
    if (aoop->kclass==nullptr) {
        std::cerr<<"newarray找不到"<<('['+typeStr)<<std::endl;
        return RUN_ERROR;
    }
    aoop->eleCount=count;
    aoop->elements.ptr=(char*)aoop+sizeof(ArrayOop);
    execute->stack_frame->operand_stack->Push_uI32(rp);
    execute->stack_frame->PC+=3;
    return RUN_CONTINUE;
}

int I_arraylength(Execute* execute) {
    uint32_t ar=execute->stack_frame->operand_stack->getTop_uI32();
    execute->stack_frame->operand_stack->Pop_32();
    if (ar<=0){return RUN_ERROR;}
    ArrayOop* ap=(ArrayOop*)(heap.getRowPointer(ar));
    execute->stack_frame->operand_stack->Push_I32(ap->eleCount);
    execute->stack_frame->PC++;
    return RUN_CONTINUE;
}

int I_athrow(Execute* execute) {
    std::cerr<<"athrow指令未实现，不支持异常处理"<<std::endl;
    return RUN_INSTRUCTION_UNDO;
};

int isSon_Instance(const Kclass* s,const Kclass* f) {
    const Kclass* ss=s;
    while (ss!=nullptr && ss!=f) {
        ss=ss->father;
    }
    if(ss==f){return 1;}
    std::queue<const instanceClass*> que;
    auto* inC=(instanceClass*)s;
    que.emplace(inC);
    while (!que.empty()) {
        const instanceClass* tem=que.front();que.pop();
        if (tem==f){return 1;}
        if (tem->father){que.emplace((const instanceClass*)(tem->father));}
        for (const auto i:tem->interfaces) {
            if (i)que.emplace(i);
        }
    }
    return 0;
}
void myArrTypeStrSplit(const std::string& arrTypeStr,int& dim,std::string& re) {
    //I 0 I
    //[[LA; 2 LA;
    int i=0;
    for (; i < arrTypeStr.size(); ++i) {
        if (arrTypeStr[i]!='[') {
            break;
        }
    }
    dim=i;
    re=arrTypeStr.substr(i);
}
int isSon_Array(const std::string& s,const std::string& f) {
    //s [[LA;  [[I
    //f [[LB; [I
    int si=0,fi=0;
    std::string ff,ss;
    myArrTypeStrSplit(s,si,ss);
    myArrTypeStrSplit(f,fi,ff);
    if (ff[0]!='L') {
        if (ff==ss && si==fi){return 1;}else{return 0;}
    }
    if (ff=="Ljava/lang/Object;") {
        if (fi<si){return 1;}
        if (fi==si) {
            if (ss[0]=='L'){return 1;}
            return 0;
        }
        return 0;
    }
    if (si==fi) {
        if (ss[0]!='L'){return 0;}
        ff=ff.substr(1,ff.size()-2);
        ss=ss.substr(1,ss.size()-2);
        Kclass* fc=my_class_loader.getInstanceClass(ff);
        Kclass* sc=my_class_loader.getInstanceClass(ss);
        if (fc==nullptr || sc==nullptr) {
            std::cerr<<"isSon_Array空指针错误"<<std::endl;
            return -100;
        }
        return isSon_Instance(sc,fc);
    }
    return 0;
}
int I_checkcast(Execute* execute) {
    int b=execute->stack_frame->PC+1;
    uint16_t t1=*((uint8_t*)(execute->code+b)),
    t2=*((uint8_t*)(execute->code+b+1));
    t1=(t1<<8)|t2;
    instanceClass* instance_class=(instanceClass*)execute->stack_frame->Class;
    MCP_Class* classType=(MCP_Class*)(instance_class->constantPool->pool[t1]);
    uint32_t objRef=execute->stack_frame->operand_stack->getTop_uI32();
    if (objRef==0) {
        //std::cerr<<"checkcast空指针是允许的"<<std::endl;
        execute->stack_frame->PC+=3;
        return RUN_CONTINUE;
    }
    OopDesc* ooD=(OopDesc*)heap.getRowPointer(objRef);
    int re=0;
    if (ooD->kclass->name[0]=='[') {//数组
        std::string sn=ooD->kclass->name;//[I or [LA;
        std::string fn;
        //classType->name为类，可能名为 I、L、java/lang/Object、[[I、[[LA;
        //全部统一为[[[...[+(基本类型/Lxxxx;)
        if (classType->name[0]!='[' &&
            classType->name[classType->name.size()-1]!=';') {
            fn="L"+classType->name+";";//LI;
        }else {
            fn=classType->name;
        }
        re=isSon_Array(sn,fn);
    }else {
        //此时classType->name为 I、L、java/lang/Object、...
        // std::string fn;
        // if (classType->name[0]=='L' && classType->name[classType->name.size()-1]==';') {
        //     fn=classType->name.substr(1,classType->name.size()-2);
        // }else {
        //     fn=classType->name;
        // }
        // Kclass* f=my_class_loader.getInstanceClass(fn);
        Kclass* f=my_class_loader.getInstanceClass(classType->name);
        if (f==nullptr) {
            std::cerr<<"checkcast找不到类"<<classType->name<<std::endl;
            return RUN_ERROR;
        }
        re=isSon_Instance(ooD->kclass,f);
    }
    execute->stack_frame->PC+=3;
    if (re<1) {
        std::cerr<<"checkcast不满足"<<std::endl;
        return RUN_ERROR;
    }
    return RUN_CONTINUE;
};

int I_instanceof(Execute* execute) {
    int b=execute->stack_frame->PC+1;
    uint16_t t1=*((uint8_t*)(execute->code+b)),
    t2=*((uint8_t*)(execute->code+b+1));
    t1=(t1<<8)|t2;
    instanceClass* instance_class=(instanceClass*)execute->stack_frame->Class;
    MCP_Class* classType=(MCP_Class*)(instance_class->constantPool->pool[t1]);
    uint32_t objRef=execute->stack_frame->operand_stack->getTop_uI32();
    execute->stack_frame->operand_stack->Pop_32();
    if (objRef==0) {
        execute->stack_frame->operand_stack->Push_I32(0);
        execute->stack_frame->PC+=3;
        return RUN_CONTINUE;
    }
    OopDesc* ooD=(OopDesc*)heap.getRowPointer(objRef);
    int re=0;
    if (ooD->kclass->name[0]=='[') {//数组
        std::string sn=ooD->kclass->name;//[I or [LA;
        std::string fn;
        if (classType->name[0]!='[' &&
            classType->name[classType->name.size()-1]!=';') {
            fn="L"+classType->name+";";//LI;
            }else {
                fn=classType->name;
            }
        re=isSon_Array(sn,fn);
    }else {
        //std::string fn;
        // if (classType->name[0]=='L' && classType->name[classType->name.size()-1]==';') {
        //     fn=classType->name.substr(1,classType->name.size()-2);
        // }else {
        //     fn=classType->name;
        // }
        //Kclass* f=my_class_loader.getInstanceClass(fn);
        Kclass* f=my_class_loader.getInstanceClass(classType->name);
        if (f==nullptr) {
            std::cerr<<"instanceof找不到类"<<classType->name<<std::endl;
            return RUN_ERROR;
        }
        re=isSon_Instance(ooD->kclass,f);
    }
    execute->stack_frame->PC+=3;
    if (re==0) {
        execute->stack_frame->operand_stack->Push_I32(0);
        return RUN_CONTINUE;
    }
    if (re==1) {
        execute->stack_frame->operand_stack->Push_I32(1);
        return RUN_CONTINUE;
    }
    std::cerr<<"instanceof不满足"<<std::endl;
    return RUN_ERROR;
};

int I_monitorenter(Execute* execute) {
    std::clog<<"monitorenter指令未实现，不过程序依旧进行，因为仅支持用户单线程"<<std::endl;
    execute->stack_frame->operand_stack->Pop_32();
    execute->stack_frame->PC+=1;
    return RUN_CONTINUE;
};

int I_monitorexit(Execute* execute) {
    std::clog<<"monitorexit指令未实现，不过程序依旧进行，因为仅支持用户单线程"<<std::endl;
    execute->stack_frame->operand_stack->Pop_32();
    execute->stack_frame->PC+=1;
    return RUN_CONTINUE;
};

int I_wide(Execute* execute) {
    std::cerr<<"wide指令未实现"<<std::endl;
    return RUN_INSTRUCTION_UNDO;
};

uint32_t multiArrCreate(const std::string& type,
    const std::vector<int32_t>& counts,int dim) {
    std::string elementType=type.substr(1);
    int count=counts[dim];
    auto elemSF=TypeToBtyeSize.find(elementType[0]);
    if (elemSF==TypeToBtyeSize.end()) {
        std::cerr<<"multiArrCreate数组类型错误"<<std::endl;
        return -1;
    }
    int elemS=(*elemSF).second;
    uint32_t rRef=heap.NewObj_Array(sizeof(ArrayOop)+elemS*count);
    if (rRef<=0) {
        std::cerr<<"multiArrCreate申请空间失败"<<std::endl;
        return -1;
    }
    ArrayOop* aoop=(ArrayOop*)(heap.getRowPointer(rRef));
    aoop->runTimeInfo=0;
    Kclass* tff=my_class_loader.getInstanceClass(type);
    if (tff==nullptr) {
        std::cerr<<"multiArrCreate找不到"<<type<<std::endl;
        return -1;
    }
    aoop->kclass=tff;
    aoop->eleCount=count;
    aoop->elements.ptr=(char*)aoop+sizeof(ArrayOop);
    uint32_t te;
    if (dim<counts.size()-1) {
        for (int i = 0; i < count; ++i) {
            te=multiArrCreate(elementType,counts,dim+1);
            if (te<=0) {
                std::cerr<<"multiArrCreate申请空间失败"<<std::endl;
                return -1;
            }
            aoop->elements.put<uint32_t>(te,i);
        }
    }
    return rRef;
}
int I_multianewarray(Execute* execute) {
    int b=execute->stack_frame->PC+1;
    uint16_t t1=*((uint8_t*)(execute->code+b)),
    t2=*((uint8_t*)(execute->code+b+1));
    t1=(t1<<8)|t2;
    instanceClass* instance_class=(instanceClass*)execute->stack_frame->Class;
    MCP_Class* type=(MCP_Class*)(instance_class->constantPool->pool[t1]);
    int dimension=*((uint8_t*)(execute->code+b+2));
    std::vector<int32_t> counts(dimension,0);
    for (int i=dimension-1;i>=0;--i) {
        counts[i]=execute->stack_frame->operand_stack->getTop_I32();
        execute->stack_frame->operand_stack->Pop_32();
        if (counts[i]<=0){return RUN_ERROR;}
    }
    //type->name可能为 [[[LA; [[I
    uint32_t maRef=multiArrCreate(type->name,counts,0);
    if (maRef<=0) {
        std::cerr<<"multianewarray建立失败"<<std::endl;
        return RUN_ERROR;
    }
    execute->stack_frame->PC+=4;
    execute->stack_frame->operand_stack->Push_uI32(maRef);
    return RUN_CONTINUE;
};

int I_ifnull(Execute* execute){
    int b=execute->stack_frame->PC+1;
    uint16_t t1=*((uint8_t*)(execute->code+b)),
    t2=*((uint8_t*)(execute->code+b+1));
    t1=(t1<<8)|t2;
    uint32_t v=execute->stack_frame->operand_stack->getTop_uI32();
    execute->stack_frame->operand_stack->Pop_32();
    if (v==0) {
        execute->stack_frame->PC+=(int16_t)t1;
    }else {
        execute->stack_frame->PC+=3;
    }
    return RUN_CONTINUE;
};

int I_ifnonnull(Execute* execute){
    int b=execute->stack_frame->PC+1;
    uint16_t t1=*((uint8_t*)(execute->code+b)),
    t2=*((uint8_t*)(execute->code+b+1));
    t1=(t1<<8)|t2;
    uint32_t v=execute->stack_frame->operand_stack->getTop_uI32();
    execute->stack_frame->operand_stack->Pop_32();
    if (v>0) {
        execute->stack_frame->PC+=(int16_t)t1;
    }else {
        execute->stack_frame->PC+=3;
    }
    return RUN_CONTINUE;
};

int I_goto_w(Execute* execute) {
    std::cerr<<"goto_w指令未实现"<<std::endl;
    return RUN_INSTRUCTION_UNDO;
};

int I_jsr_w(Execute* execute) {
    std::cerr<<"jsr_w指令未实现"<<std::endl;
    return RUN_INSTRUCTION_UNDO;
};

int (*InstructionFuncPtrArr[256])(Execute*)={
//0-19
 I_nop,I_aconst_null,I_iconst_m1,I_iconst_0,I_iconst_1,
 I_iconst_2,I_iconst_3,I_iconst_4,I_iconst_5,I_lconst_0,
 I_lconst_1,I_fconst_0,I_fconst_1,I_fconst_2,I_dconst_0,
 I_dconst_1,I_bipush,I_sipush,I_ldc,I_ldc_w,

 //20-39
 I_ldc2_w,I_iload,I_lload,I_fload,I_dload,
 I_aload,I_iload_0,I_iload_1,I_iload_2,I_iload_3,
 I_lload_0,I_lload_1,I_lload_2,I_lload_3,I_fload_0,
 I_fload_1,I_fload_2,I_fload_3,I_dload_0,I_dload_1,

 //40-59
 I_dload_2,I_dload_3,I_aload_0,I_aload_1,I_aload_2,
 I_aload_3,I_iaload,I_laload,I_faload,I_daload,
 I_aaload,I_baload,I_caload,I_saload,I_istore,
 I_lstore,I_fstore,I_dstore,I_astore,I_istore_0,

 //60-79
 I_istore_1,I_istore_2,I_istore_3,I_lstore_0,I_lstore_1,
 I_lstore_2,I_lstore_3,I_fstore_0,I_fstore_1,I_fstore_2,
 I_fstore_3,I_dstore_0,I_dstore_1,I_dstore_2,I_dstore_3,
 I_astore_0,I_astore_1,I_astore_2,I_astore_3,I_iastore,

 //80-99
 I_lastore,I_fastore,I_dastore,I_aastore,I_bastore,
 I_castore,I_sastore,I_pop,I_pop2,I_dup,
 I_dup_x1,I_dup_x2,I_dup2,I_dup2_x1,I_dup2_x2,
 I_swap,I_iadd,I_ladd,I_fadd,I_dadd,

 //100-119
 I_isub,I_lsub,I_fsub,I_dsub,I_imul,
 I_lmul,I_fmul,I_dmul,I_idiv,I_ldiv,
 I_fdiv,I_ddiv,I_irem,I_lrem,I_frem,
 I_drem,I_ineg,I_lneg,I_fneg,I_dneg,

 //120-140
 I_ishl,I_lshl,I_ishr,I_lshr,I_iushr,I_lushr,
 I_iand,I_land,I_ior,I_lor,I_ixor,
 I_lxor,I_iinc,I_i2l,I_i2f,I_i2d,
 I_l2i,I_l2f,I_l2d,I_f2i,I_f2l,

 //141-160
 I_f2d, I_d2i, I_d2l, I_d2f, I_i2b,
 I_i2c, I_i2s, I_lcmp, I_fcmpl, I_fcmpg,
 I_dcmpl, I_dcmpg, I_ifeq, I_ifne, I_iflt,
 I_ifge, I_ifgt, I_ifle, I_if_icmpeq, I_if_icmpne,

//161-180
 I_if_icmplt,I_if_icmpge,I_if_icmpgt,I_if_icmple,I_if_acmpeq,
 I_if_acmpne,I_goto,I_jsr,I_ret,I_tableswitch,
 I_lookupswitch,I_ireturn,I_lreturn,I_freturn,I_dreturn,
 I_areturn,I_return,I_getstatic,I_putstatic,I_getfield,

 //181-200
 I_putfield,I_invokevirtual,I_invokespecial,I_invokestatic,I_invokeinterface,
 I_invokedynamic,I_new,I_newarray,I_anewarray,I_arraylength,
 I_athrow,I_checkcast,I_instanceof,I_monitorenter,I_monitorexit,
 I_wide,I_multianewarray,I_ifnull,I_ifnonnull,I_goto_w,

 //201
 I_jsr_w,nullptr,nullptr,nullptr,nullptr,
 nullptr,nullptr,nullptr,nullptr,nullptr,
 nullptr,nullptr,nullptr,nullptr,nullptr,
 nullptr,nullptr,nullptr,nullptr,nullptr,
 // Reserved
 // 202 (0xca)    breakpoint
 // 254 (0xfe)    impdep1
 // 255 (0xff)    impdep2
 nullptr,nullptr,nullptr,nullptr,nullptr,
 nullptr,nullptr,nullptr,nullptr,nullptr,
 nullptr,nullptr,nullptr,nullptr,nullptr,
 nullptr,nullptr,nullptr,nullptr,nullptr,

 nullptr,nullptr,nullptr,nullptr,nullptr,
 nullptr,nullptr,nullptr,nullptr,nullptr,
 nullptr,nullptr,nullptr,nullptr,nullptr
};

