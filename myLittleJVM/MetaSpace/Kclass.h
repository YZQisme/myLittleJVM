//
// Created by YZQ on 25-7-8.
//

#ifndef KCLASS_H
#define KCLASS_H
#include "MetaConstantPool.h"
#include "../BaseStruct/Slots.h"
#include "../BaseStruct/BaseStruction.h"

struct MetaField {//非static
    int accessFlags=0;
    std::string name;
    std::string descriptor;//数据类型
    int constantValueIndex=-1;
    MetaField()= default;
    explicit MetaField(const Field& field,
        const readfile_ConstantPool& constant_pool);
};

struct MetaMethod {//所有方法
    int accessFlags=0;
    int maxOpStackSize=0;
    int maxLocalVTSize=0;
    Code* code=nullptr;
    ExceptionTable* exceptionTable=nullptr;
    std::string name;
    std::string descriptor;//输入的数据类型和返回值类型
    MetaMethod()=default;
    MetaMethod(const Method& method,
        const readfile_ConstantPool& constant_pool);
    ~MetaMethod() {
        if (code){delete code;code=nullptr;}
        if (exceptionTable){delete exceptionTable;exceptionTable=nullptr;}
    }
};

struct Kclass {//表示一个Java类，接口
    int clinitStatement=0;//0未进行   1正在进行    2已完成    3运行失败
    int accessFlag=0;
    std::string name;
    std::string fatherName;
    uint32_t ClassInstancePtr=0;//类对象，指向堆区的一个空间，该空间不会进行垃圾回收
    Kclass* father=nullptr;//父类
    Kclass()=default;
    virtual ~Kclass() =default;
};

struct instanceClass:Kclass {//普通类
    int metaFieldCount=0;//变量
    std::vector<MetaField*> fields;

    int metaMethodCount=0;//方法
    std::vector<MetaMethod*> methods;

    //常量池
    MetaConstantPool* constantPool=nullptr;

    int interfaceCount=0;//接口
    std::vector<std::string> interfacesName;
    std::vector<instanceClass*> interfaces;

    const myClassLoader* classLoader=nullptr;

    Slots* staticSlots=nullptr;
    //从Field字段的描述映射到slot的位置，< Class.a:I , slot>
    //会收集该类及其所有父类的字段，从父类开始
    //string = name:type
    std::unordered_map<std::string,int> staticSlotMap;
    //从Field字段的描述映射到slot的位置
    //会收集该类及其所有父类的字段，从父类开始
    int instanceSlotSize=0;
    std::unordered_map<std::string,int> instanceSlotMap;
    int buildSlotMap();
    instanceClass()= default;
    explicit instanceClass(const ClassFile& classfile);
    ~instanceClass() override {
        for (auto& i:methods){if (i){delete i;i=nullptr;}}
        for (auto& i:fields){if (i){delete i;i=nullptr;}}
        //for (auto& i:interfaces){if (i){delete i;i=nullptr;}}
        delete constantPool;
        delete staticSlots;
    }
};

struct instanceMirrorClass:instanceClass {//java.lang.Class的类
    instanceMirrorClass()= default;
    explicit instanceMirrorClass(const ClassFile& classfile):instanceClass(classfile) {}
    //to do
};

struct instanceClassLoaderClass:instanceClass {//用于类加载器
    //to do
};

struct instanceRefClass:instanceClass {//java.lang.ref.Reference
    //to do
};


struct ArrayClass:Kclass {//数组类
    int dimension=1;
    std::string elementTypeStr;
    Kclass* elementTypeClassPtr=nullptr;
    ~ArrayClass() override =default;
};

struct TypeArrayClass:ArrayClass {//基本类型元素
};

struct ObjArrayClass:ArrayClass {//引用类型元素
};

#endif //KCLASS_H