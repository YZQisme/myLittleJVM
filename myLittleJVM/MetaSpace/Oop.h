//
// Created by YZQ on 25-7-8.
//

#ifndef OOP_H
#define OOP_H
#include "Kclass.h"
#include "../BaseStruct/miniVector.h"
#endif //OOP_H

struct OopDesc {
    virtual ~OopDesc() = default;
    OopDesc() {
        hashCode=my_hash_code.getCode();
    }
    //一个类的堆中实例
    uint64_t runTimeInfo=0;
    Kclass* kclass=nullptr;//指向该类的信息
    int32_t hashCode=0;
};

struct instanceOopDesc:OopDesc {
    instanceOopDesc()=default;
    Slots* instanceSlots=nullptr;
};

struct classInstanceOopDesc:instanceOopDesc {
    classInstanceOopDesc()=default;
    Kclass* kclassManaged=nullptr;
};

struct ArrayOop:OopDesc {
    ArrayOop()=default;
    int eleCount=0;
    miniVector elements;
};

/*
struct TypeArrayOop:ArrayOop {
    int32_t* arr;
    ~TypeArrayOop()override=default;
};

struct ObjArrayOop:ArrayOop {
    uint32_t* arr;
    ~ObjArrayOop()override=default;
};
*/