//
// Created by YZQ on 25-7-7.
//
#ifndef CLASSFILEFORMAT_H
#define CLASSFILEFORMAT_H
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

//大端序 ：高位字节存储在低地址，字节码文件就是这样
//小端序 ：低位字节存储在低地址，x86/x86-64 (Intel/AMD)以及大部分的ARM
#if defined(__GNUC__) || defined(__clang__)
    #define ToLittleEndian_16(x) (__builtin_bswap16(x))
    #define ToLittleEndian_32(x) (__builtin_bswap32(x))
    #define ToLittleEndian_64(x) (__builtin_bswap64(x))
#endif
/*
ClassFile {
    u4             magic;
    u2             minor_version;
    u2             major_version;//+8

    u2             constant_pool_count;
    cp_info        constant_pool[constant_pool_count-1];

    u2             access_flags;
    u2             this_class;
    u2             super_class;

    u2             interfaces_count;
    u2             interfaces[interfaces_count];

    u2             fields_count;
    field_info     fields[fields_count];

    u2             methods_count;
    method_info    methods[methods_count];
                        u2             attributes_count;
                        attribute_info attributes[attributes_count];
}
*/

#define CONSTANT_Utf8 1
#define CONSTANT_Integer 3
#define CONSTANT_Float 4
#define CONSTANT_Long 5
#define CONSTANT_Double 6
#define CONSTANT_Class 7
#define CONSTANT_String 8
#define CONSTANT_Fieldref 9
#define CONSTANT_Methodref 10
#define CONSTANT_InterfaceMethodref 11
#define CONSTANT_NameAndType 12
#define CONSTANT_MethodHandle 15
#define CONSTANT_MethodType 16
#define CONSTANT_InvokeDynamic 18
struct readfile_constant_item_info {//读取Class文件的ConstantPool结构体
    int tag;
    readfile_constant_item_info():tag(0){}
    virtual ~readfile_constant_item_info() = default;
    virtual void print(){printf("");}
};

struct Class_info:readfile_constant_item_info {
    int name_index;//u2, to CONSTANT_Utf8_info
    Class_info():name_index(0){}
    Class_info(const char* dataPtr,int* bias) {
        tag=7;
        name_index=ToLittleEndian_16(*((uint16_t*)(dataPtr+1)));
        *bias=3;
    }
    void print() override {printf("%-20s%3d\n","Class",name_index);}
};

struct Fieldref_info:readfile_constant_item_info {
    int class_index;//u2 //to CONSTANT_Class_info
    int name_and_type_index;//u2 //to CONSTANT_NameAndType_info
    Fieldref_info():class_index(0),name_and_type_index(0){}
    Fieldref_info(const char* dataPtr,int* bias) {
        tag=9;
        class_index=ToLittleEndian_16(*((uint16_t*)(dataPtr+1)));
        name_and_type_index=ToLittleEndian_16(*((uint16_t*)(dataPtr+3)));
        *bias=5;
    }
    void print() override{
        printf("%-20s%3d%3d\n","Fieldref",class_index,name_and_type_index);
    }
};

struct InterfaceMethodref_info:readfile_constant_item_info {
    int class_index;//u2 //to CONSTANT_Class_info
    int name_and_type_index;//u2 //to CONSTANT_NameAndType_info
    InterfaceMethodref_info():class_index(0),name_and_type_index(0){}
    InterfaceMethodref_info(const char* dataPtr,int* bias) {
        tag=11;
        class_index=ToLittleEndian_16(*((uint16_t*)(dataPtr+1)));
        name_and_type_index=ToLittleEndian_16(*((uint16_t*)(dataPtr+3)));
        *bias=5;
    }
    void print() override{
        printf("%-20s%3d%3d\n","Fieldref",class_index,name_and_type_index);
    }
};

struct Methodref_info:readfile_constant_item_info {
    int class_index;//u2 //to CONSTANT_Class_info
    int name_and_type_index;//u2 //to CONSTANT_NameAndType_info
    Methodref_info():class_index(0),name_and_type_index(0){}
    Methodref_info(const char* dataPtr,int* bias) {
        tag=10;
        class_index=ToLittleEndian_16(*((uint16_t*)(dataPtr+1)));
        name_and_type_index=ToLittleEndian_16(*((uint16_t*)(dataPtr+3)));
        *bias=5;
    }
    void print() override{
        printf("%-20s%3d%3d\n","Fieldref",class_index,name_and_type_index);
    }
};

struct String_info:readfile_constant_item_info {
    int string_index;//u2 //to CONSTANT_Utf8_info
    String_info():string_index(0){}
    String_info(const char* dataPtr,int* bias) {
        tag=8;
        string_index=ToLittleEndian_16(*((uint16_t*)(dataPtr+1)));
        *bias=3;
    }
    void print() override {
        printf("%-20s%3d\n","String",string_index);
    }
};

struct Integer_info:readfile_constant_item_info {
    //int bytes;//u4
    int value;
    Integer_info():value(0){};
    Integer_info(const char* dataPtr,int* bias) {
        tag=3;
        value=int(ToLittleEndian_32(*((uint32_t*)(dataPtr+1))));
        *bias=5;
    }
    void print() override {printf("%-20s%3d\n","Integer",value);}
};

struct Float_info:readfile_constant_item_info {
    //int bytes;//u4
    float value;
    Float_info():value(0) {}
    Float_info(const char* dataPtr,int* bias) {
        tag=4;
        uint32_t t=ToLittleEndian_32(*((uint32_t*)(dataPtr+1)));
        value=*((float*)(&t));
        *bias=5;
    }
    void print() override{printf("%-20s%.3f\n","Float",value);}
};

struct Long_info:readfile_constant_item_info {
    //int high_bytes;//u4
    //int low_bytes;//u4
    int64_t value;
    Long_info():value(0) {}
    Long_info(const char* dataPtr,int* bias) {
        tag=5;
        value=ToLittleEndian_64(*((uint64_t*)(dataPtr+1)));
        *bias=9;
    }
    void print() override {printf("%-20s%lld\n","Long",value);}
};

struct Double_info:readfile_constant_item_info {
    //int high_bytes;//u4
    //int low_bytes;//u4
    double value;
    Double_info():value(0) {}
    Double_info(const char* dataPtr,int* bias) {
        tag=6;
        uint64_t t=ToLittleEndian_64(*((uint64_t*)(dataPtr+1)));
        value=*((double*)(&t));
        *bias=9;
    }
    void print() override {printf("%-20s%.3f\n","Double",value);}
};

struct NameAndType_info:readfile_constant_item_info {
    int name_index;//u2 //to CONSTANT_Utf8_info
    int descriptor_index;//u2 //to CONSTANT_Utf8_info
    NameAndType_info():name_index(0),descriptor_index(0){}
    NameAndType_info(const char* dataPtr,int* bias) {
        tag=12;
        name_index=int(ToLittleEndian_16(*((uint16_t*)(dataPtr+1))));
        descriptor_index=int(ToLittleEndian_16(*((uint16_t*)(dataPtr+3))));
        *bias= 5;
    }
    void print() override {
        printf("%-20s%3d%3d\n","NameAndType",name_index,descriptor_index);
    }
};

struct Utf8_info:readfile_constant_item_info {
    // int length=0;//u2
    // //char* bytes;
    std::string str;
    Utf8_info()= default;
    Utf8_info(const char* dataPtr,int* bias) {
        tag=1;
        int length=int(ToLittleEndian_16(*((uint16_t*)(dataPtr+1))));
        if (length>0) {
            str.assign((const char*)(dataPtr+3),length);
        }
        *bias=3+length;
    }
    void print() override {printf("%-20s%s\n","Utf8",str.c_str());}
};

struct MethodHandle_info:readfile_constant_item_info {
    int reference_kind;//u1
    int reference_index;//u2
    MethodHandle_info():reference_kind(0),reference_index(0){}
    MethodHandle_info(const char* dataPtr,int* bias) {
        tag=15;
        reference_kind =*((uint8_t*)(dataPtr+1));
        reference_index=int(ToLittleEndian_16(*((uint16_t*)(dataPtr+2))));
        *bias=4;
    }
    void print() override {
        printf("%-20s%3d%3d\n","MethodHandle",reference_index,reference_kind);
    }
};

struct MethodType_info:readfile_constant_item_info {
    int descriptor_index;//u2
    MethodType_info():descriptor_index(0){}
    MethodType_info(const char* dataPtr,int* bias) {
        tag=16;
        descriptor_index=int(ToLittleEndian_16(*((uint16_t*)(dataPtr+1))));
        *bias=3;
    }
    void print() override {
        printf("%-20s%3d\n","MethodType",descriptor_index);
    }
};

struct InvokeDynamic_info:readfile_constant_item_info {
    int bootstrap_method_attr_index;//u2
    int name_and_type_index;//u2
    InvokeDynamic_info():bootstrap_method_attr_index(0),name_and_type_index(0){}
    InvokeDynamic_info(const char* dataPtr,int* bias) {
        tag=18;
        bootstrap_method_attr_index=int(ToLittleEndian_16(*((uint16_t*)(dataPtr+1))));
        name_and_type_index=int(ToLittleEndian_16(*((uint16_t*)(dataPtr+3))));
        *bias=5;
    }
    void print() override {
        printf("%-20s%3d%3d\n","InvokeDynamic",bootstrap_method_attr_index,
            name_and_type_index);
    }
};

struct readfile_ConstantPool {
    int constant_pool_count=0;
    std::vector<readfile_constant_item_info*> pool;
    ~readfile_ConstantPool() {
        for (auto& i:pool) {
            if (i){delete i;i=nullptr;}
        }
        constant_pool_count=0;
        pool.clear();
    }
    readfile_ConstantPool()=default;
    readfile_ConstantPool(const char* dataPtr,int* bias);
};

/*
attribute_info {
    u2 attribute_name_index;
    u4 attribute_length;
    u1 info[attribute_length];
 }
 */
struct Attribute {
    int attribute_name_index=0;//u2
    int attribute_length=0;//u4
    Attribute()= default;
    virtual ~Attribute()=default;
};

struct NormalAttribute:Attribute {
    //Normal表示info没有展开
    char* info=nullptr;
    NormalAttribute()=default;
    NormalAttribute(const char* dataPtr,int* bias);
    ~NormalAttribute() override {
        if (info){free(info);info=nullptr;}
    }
};

// /*ConstantValue_attribute {
//          u2 attribute_name_index;
//          u4 attribute_length;
//     u2 constantvalue_index;
// }
struct ConstantValueAttribute:Attribute {
    int constantvalue_index=0;//u2
    ConstantValueAttribute()=default;
    ConstantValueAttribute(const char* dataPtr,int* bias) {
        constantvalue_index=ToLittleEndian_16(*((uint16_t*)dataPtr));
        *bias=2;
    }
    ~ConstantValueAttribute() override =default;
};

// Code_attribute {
//          u2 attribute_name_index;
//          u4 attribute_length;
//     u2 max_stack;
//     u2 max_locals;
//     u4 code_length;
//     u1 code[code_length];
//     u2 exception_table_length;
//         {   u2 start_pc;
//         u2 end_pc;
//         u2 handler_pc;
//         u2 catch_type;
//         } exception_table[exception_table_length];
//     u2 attributes_count;
//     attribute_info attributes[attributes_count];
// }
struct Exception {
    int start_pc=-1;//u2
    int end_pc=-1;//u2
    int handler_pc=-1;//u2
    int catch_type=-1;//u2
    Exception() = default;
    Exception(const char* dataPtr,int* bias) {
        int bi=0;
        start_pc=ToLittleEndian_16(*((uint16_t*)(dataPtr+bi)));
        bi+=2;
        end_pc=ToLittleEndian_16(*((uint16_t*)(dataPtr+bi)));
        bi+=2;
        handler_pc=ToLittleEndian_16(*((uint16_t*)(dataPtr+bi)));
        bi+=2;
        catch_type=ToLittleEndian_16(*((uint16_t*)(dataPtr+bi)));
        bi+=2;
        *bias=bi;
    }
};
struct ExceptionTable {
    int exception_table_length=0;//u2
    std::vector<Exception*> exceptions;
    ExceptionTable()=default;
    ExceptionTable(const char* dataPtr,int* bias) {
        int bi=0,tbi;
        exception_table_length=ToLittleEndian_16(*((uint16_t*)(dataPtr+bi)));
        bi+=2;
        if (exception_table_length>0) {
            exceptions.resize(exception_table_length,nullptr);
            for (int i = 0; i < exception_table_length; ++i) {
                tbi=0;
                exceptions[i]=new Exception(dataPtr+bi,&tbi);
                bi+=tbi;
            }
        }
        *bias=bi;
    }
    ExceptionTable& operator=(ExceptionTable&& o)noexcept {
        exception_table_length=o.exception_table_length;
        exceptions=std::move(o.exceptions);
        o.exception_table_length=0;
        return *this;
    }
    ~ExceptionTable() {
        for (auto& i:exceptions) {
            if (i){delete i;i=nullptr;}
        }
        exception_table_length=0;
        exceptions.clear();
    }
};
struct Code {
    int code_length=0;//u4
    char* code=nullptr;//u1
    Code()=default;
    Code(const char* dataPtr,int* bias) {
        int bi=0;
        code_length=ToLittleEndian_32(*((uint32_t*)(dataPtr+bi)));
        bi+=4;
        code=nullptr;
        if (code_length>0) {
            code=(char*)malloc(code_length);
            memcpy(code,dataPtr+bi,code_length);
        }
        *bias=bi+code_length;
    }
    Code& operator=(Code&& o)  noexcept {//移动复制
        code_length=o.code_length;
        code=o.code;
        o.code=nullptr;
        o.code_length=0;
        return *this;
    }
    ~Code() {
        code_length=0;
        if (code){free(code);code=nullptr;}
    }
};
struct Attributes {
    int attributes_count=0;//u2
    std::vector<Attribute*> attributes;
    Attributes()=default;
    Attributes(const char* dataPtr,int* bias) {
        int bi=0,tbi;
        attributes_count=ToLittleEndian_16(*((uint16_t*)(dataPtr+bi)));
        bi+=2;
        if (attributes_count>0) {
            attributes.resize(attributes_count,nullptr);
            for (int i = 0; i < attributes_count; ++i) {
                tbi=0;
                attributes[i]=new NormalAttribute(dataPtr+bi,&tbi);
                bi+=tbi;
            }
        }
        *bias=bi;
    }
    ~Attributes() {
        for (auto& i:attributes) {
            if (i){delete i;i=nullptr;}
        }
        attributes_count=0;
        attributes.clear();
    }
};

struct CodeAttribute:Attribute {
    int max_stack=0;//u2
    int max_locals=0;//u2
    Code* codes=nullptr;
    ExceptionTable* exceptionTable=nullptr;
    Attributes* attributes=nullptr;
    CodeAttribute()=default;
    CodeAttribute(const char* dataPtr,int* bias);
    ~CodeAttribute() override {
        if (codes){delete codes;codes=nullptr;}
        if (attributes){delete attributes;attributes=nullptr;}
        if (exceptionTable){delete exceptionTable;exceptionTable=nullptr;}
    }
};



/*
u2             interfaces_count;
u2             interfaces[interfaces_count];

u2             fields_count;
field_info     fields[fields_count];

u2             methods_count;
method_info    methods[methods_count];
*/
struct Interface {
    int interfaces_count=0;
    std::vector<int> interface_name_index;
    Interface()=default;
    Interface(const char* dataPtr,int* bias) {
        int bi=0;
        interfaces_count=ToLittleEndian_16(*((uint16_t*)(dataPtr+bi)));
        bi+=2;
        if (interfaces_count>0) {
            interface_name_index.resize(interfaces_count,0);
            for (int i = 0; i < interfaces_count; ++i) {
                interface_name_index[i]=
                    ToLittleEndian_16(*((uint16_t*)(dataPtr+bi)));
                bi+=2;
            }
        }
        *bias=bi;
    }
    ~Interface()=default;
};

/////////////////////////////////////////////////Fields and Methods///////////
/*
*  field_info {
    u2             access_flags;
    u2             name_index;
    u2             descriptor_index;
    u2             attributes_count;
    attribute_info attributes[attributes_count];
 }
 */
struct Field {
    int access_flags=0;//u2
    int name_index=0;//u2
    int descriptor_index=0;//u2
    Attributes* attributes=nullptr;
    Field()=default;
    Field(const char* dataPtr,int* bias) {
        int bi=0,tbi;
        access_flags=ToLittleEndian_16(*((uint16_t*)(dataPtr+bi)));
        bi+=2;
        name_index=ToLittleEndian_16(*((uint16_t*)(dataPtr+bi)));
        bi+=2;
        descriptor_index=ToLittleEndian_16(*((uint16_t*)(dataPtr+bi)));
        bi+=2;
        attributes=new Attributes(dataPtr+bi,&tbi);
        bi+=tbi;
        *bias=bi;
    }
    ~Field() {
        if (attributes){delete attributes;attributes=nullptr;}
    }
};

/*
* method_info {
    u2             access_flags;
    u2             name_index;
    u2             descriptor_index;
    u2             attributes_count;
    attribute_info attributes[attributes_count];
 }
 */
struct Method {
    int access_flags=0;//u2
    int name_index=0;//u2
    int descriptor_index=0;//u2
    Attributes* attributes=nullptr;
    Method()=default;
    Method(const char* dataPtr,int* bias) {
        int bi=0,tbi;
        access_flags=ToLittleEndian_16(*((uint16_t*)(dataPtr+bi)));
        bi+=2;
        name_index=ToLittleEndian_16(*((uint16_t*)(dataPtr+bi)));
        bi+=2;
        descriptor_index=ToLittleEndian_16(*((uint16_t*)(dataPtr+bi)));
        bi+=2;
        attributes=new Attributes(dataPtr+bi,&tbi);
        bi+=tbi;
        *bias=bi;
    }
    ~Method() {
        if (attributes){delete attributes;attributes=nullptr;}
    }
};

struct Fields {
    int fields_count=0;//u2
    std::vector<Field*> fields;
    Fields()=default;
    Fields(const char* dataPtr,int* bias) {
        int bi=0,tbi;
        fields_count=ToLittleEndian_16(*((uint16_t*)(dataPtr+bi)));
        bi+=2;
        if (fields_count>0) {
            fields.resize(fields_count,nullptr);
            for (int i = 0; i < fields_count; ++i) {
                tbi=0;
                fields[i]=new Field(dataPtr+bi,&tbi);
                bi+=tbi;
            }
        }
        *bias=bi;
    }
    ~Fields() {
        for (auto& i:fields) {
            if (i) {delete i;i=nullptr;}
        }
    }
};

struct Methods {
    int methods_count=0;//u2
    std::vector<Method*> methods;
    Methods()=default;
    Methods(const char* dataPtr,int* bias) {
        int bi=0,tbi;
        methods_count=ToLittleEndian_16(*((uint16_t*)(dataPtr+bi)));
        bi+=2;
        if (methods_count>0) {
            methods.resize(methods_count,nullptr);
            for (int i = 0; i < methods_count; ++i) {
                tbi=0;
                methods[i]=new Method(dataPtr+bi,&tbi);
                bi+=tbi;
            }
        }
        *bias=bi;
    }
    ~Methods() {
        for (auto& i:methods) {
            if (i) {delete i;i=nullptr;}
        }
    }
};

struct ClassFile {
    uint32_t magic=0;
    int minor_version=0;
    int major_version=0;//+8
    readfile_ConstantPool* constantPool=nullptr;
    int access_flags=0;
    int this_class=0;
    int super_class=0;
    Interface* interfaces=nullptr;
    Fields* fields=nullptr;
    Methods* methods=nullptr;
    Attributes* classAttributes=nullptr;
    ClassFile()=default;
    ~ClassFile() {
        if (constantPool){delete constantPool;constantPool=nullptr;}
        if (interfaces){delete interfaces;interfaces=nullptr;}
        if (fields){delete fields;fields=nullptr;}
        if (methods){delete methods;methods=nullptr;}
        if (classAttributes){delete classAttributes;classAttributes=nullptr;}
    }
    ClassFile(const char* dataPtr,int* bias);
};

struct ReadBuffer {//读取的缓冲区
    char* ptr=nullptr;
    int size=0;
    ReadBuffer()=default;
    ReadBuffer(const std::string filePath,int *state) {
        std::ifstream file(filePath,std::ios::in|std::ios::binary|std::ios::ate);
        if (file.is_open()) {
            size=file.tellg();
            ptr=(char*)malloc(size);
            file.seekg(0);
            file.read(ptr,size);
            file.close();
            *state=0;//成功
        }else {
            *state=-1;//错误
        }
    }
    ~ReadBuffer() {if (ptr){free(ptr);ptr=nullptr;size=0;}}
};

inline ClassFile* readClassFile(const std::string& filePath) {
    int st;
    auto* rb=new ReadBuffer(filePath,&st);
    ClassFile* re=nullptr;
    if (st==0) {
        int bi=0;
        re=new ClassFile(rb->ptr,&bi);
        if (bi!=rb->size) {
            std::cerr<<"在读取文件"<<filePath<<"时发生了错误，Class文件的格式有问题"<<std::endl;
        }
    }
    // }else {
    //     std::cerr<<"在读取文件"<<filePath<<"时发生了错误，文件无法打开"<<std::endl;
    // }
    delete rb;
    return re;
}

#endif //CLASSFILEFORMAT_H