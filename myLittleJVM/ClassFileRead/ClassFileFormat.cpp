//
// Created by YZQ on 25-7-7.
//
#include "ClassFileFormat.h"

readfile_ConstantPool::readfile_ConstantPool(const char *dataPtr, int *bias) {
    int tag,bi=0,tbi;
    constant_pool_count=ToLittleEndian_16(*((uint16_t*)(dataPtr+bi)));
    bi+=2;
    pool.resize(constant_pool_count,nullptr);
    for (int i = 1; i < constant_pool_count; ++i) {
        tag=*((uint8_t*)(dataPtr+bi));
        tbi=0;
        switch (tag) {
            case CONSTANT_Utf8: {
                pool[i]=new Utf8_info(dataPtr+bi,&tbi);
                break;
            }
            case CONSTANT_Integer: {
                pool[i]=new Integer_info(dataPtr+bi,&tbi);
                break;
            }
            case CONSTANT_Float: {
                pool[i]=new Float_info(dataPtr+bi,&tbi);
                break;
            }
            case CONSTANT_Long: {
                pool[i]=new Long_info(dataPtr+bi,&tbi);
                ++i;
                break;
            }
            case CONSTANT_Double: {
                pool[i]=new Double_info(dataPtr+bi,&tbi);
                ++i;
                break;
            }
            case CONSTANT_Class: {
                pool[i]=new Class_info(dataPtr+bi,&tbi);
                break;
            }
            case CONSTANT_String: {
                pool[i]=new String_info(dataPtr+bi,&tbi);
                break;
            }
            case CONSTANT_Fieldref:{
                pool[i]=new Fieldref_info(dataPtr+bi,&tbi);
                break;
            }
            case CONSTANT_Methodref:{
                pool[i]=new Methodref_info(dataPtr+bi,&tbi);
                break;
            }
            case CONSTANT_InterfaceMethodref: {
                pool[i]=new InterfaceMethodref_info(dataPtr+bi,&tbi);
                break;
            }
            case CONSTANT_NameAndType: {
                pool[i]=new NameAndType_info(dataPtr+bi,&tbi);
                break;
            }
            case CONSTANT_MethodHandle: {
                pool[i]=new MethodHandle_info(dataPtr+bi,&tbi);
                break;
            }
            case CONSTANT_MethodType: {
                pool[i]=new MethodType_info(dataPtr+bi,&tbi);
                break;
            }
            case CONSTANT_InvokeDynamic: {
                pool[i]=new InvokeDynamic_info(dataPtr+bi,&tbi);
                break;
            }
            default: {
                printf("在读取Class文件中发生了错误，tag=%d\n",tag);
            }
        }
        bi+=tbi;
    }
    *bias=bi;
}

NormalAttribute::NormalAttribute(const char *dataPtr, int *bias) {
    int bi=0;
    attribute_name_index=ToLittleEndian_16(*((uint16_t*)(dataPtr+bi)));
    bi+=2;
    attribute_length=ToLittleEndian_32(*((uint32_t*)(dataPtr+bi)));
    bi+=4;
    info=nullptr;
    if (attribute_length>0) {
        info=(char*)malloc(attribute_length);
        memcpy(info,dataPtr+bi,attribute_length);
    }
    *bias=attribute_length+6;
}

CodeAttribute::CodeAttribute(const char *dataPtr, int *bias) {
    int bi=0,tbi;
    max_stack=ToLittleEndian_16(*((uint16_t*)(dataPtr+bi)));
    bi+=2;
    max_locals=ToLittleEndian_16(*((uint16_t*)(dataPtr+bi)));
    bi+=2;

    tbi=0;
    codes=new Code(dataPtr+bi,&tbi);
    bi+=tbi;

    tbi=0;
    exceptionTable=new ExceptionTable(dataPtr+bi,&tbi);
    bi+=tbi;

    tbi=0;
    attributes=new Attributes(dataPtr+bi,&tbi);
    bi+=tbi;
    *bias=bi;
}

ClassFile::ClassFile(const char *dataPtr, int *bias) {
    int bi=0,tbi;
    magic=ToLittleEndian_32(*((uint32_t*)(dataPtr+bi)));
    bi+=4;
    minor_version=ToLittleEndian_16(*((uint16_t*)(dataPtr+bi)));
    bi+=2;
    major_version=ToLittleEndian_16(*((uint16_t*)(dataPtr+bi)));
    bi+=2;

    tbi=0;
    constantPool=new readfile_ConstantPool(dataPtr+bi,&tbi);
    bi+=tbi;

    access_flags=ToLittleEndian_16(*((uint16_t*)(dataPtr+bi)));
    bi+=2;
    this_class=ToLittleEndian_16(*((uint16_t*)(dataPtr+bi)));
    bi+=2;
    super_class=ToLittleEndian_16(*((uint16_t*)(dataPtr+bi)));
    bi+=2;

    tbi=0;
    interfaces=new Interface(dataPtr+bi,&tbi);
    bi+=tbi;

    tbi=0;
    fields=new Fields(dataPtr+bi,&tbi);
    bi+=tbi;

    tbi=0;
    methods=new Methods(dataPtr+bi,&tbi);
    bi+=tbi;

    tbi=0;
    classAttributes=new Attributes(dataPtr+bi,&tbi);
    bi+=tbi;

    *bias=bi;
}