//
// Created by YZQ on 25-7-8.
//
#ifndef METACONSTANTPOOL_H
#define METACONSTANTPOOL_H
#include "../ClassFileRead/ClassFileFormat.h"
#include <string>
#include "../BaseStruct/BaseStruction.h"
#include <vector>
#include "../Heap/heap.h"

struct MetaConstantPoolItem {
    int tag;
    MetaConstantPoolItem():tag(0){}
    virtual ~MetaConstantPoolItem()=default;//让子类的析构函数可以被重写
};

//MCP = MetaConstantPool
struct MCP_Class:MetaConstantPoolItem {
    std::string name;//class name
    MCP_Class()=default;
    MCP_Class(const readfile_constant_item_info& readfile_constant_item,
        const readfile_ConstantPool& conp) {
        tag=readfile_constant_item.tag;
        auto p=(Class_info*)(&readfile_constant_item);
        name=((Utf8_info*)(conp.pool[p->name_index]))->str;
    }
};

struct MCP_Fieldref:MetaConstantPoolItem {
    std::string className;//属于哪一个类或接口
    std::string name;//名字
    std::string type;//类型
    MCP_Fieldref()=default;
    MCP_Fieldref(const readfile_constant_item_info& readfile_constant_item,
        const readfile_ConstantPool& conp) {
        tag=readfile_constant_item.tag;
        auto p=(Fieldref_info*)(&readfile_constant_item);
        auto pp=(Class_info*)(conp.pool[p->class_index]);
        className=((Utf8_info*)(conp.pool[pp->name_index]))->str;
        auto q=(NameAndType_info*)(conp.pool[p->name_and_type_index]);
        name=((Utf8_info*)(conp.pool[q->name_index]))->str;
        type=((Utf8_info*)(conp.pool[q->descriptor_index]))->str;
    }
};

typedef MCP_Fieldref MCP_Methodref,MCP_InterfaceMethodref;

struct MCP_String:MetaConstantPoolItem {
    std::string value;
    uint32_t stringPoolPtr=-1;
    MCP_String()=default;
    MCP_String(const readfile_constant_item_info& readfile_constant_item,
        const readfile_ConstantPool& conp) {
        tag=readfile_constant_item.tag;
        auto p=(String_info*)(&readfile_constant_item);
        value=((Utf8_info*)(conp.pool[p->string_index]))->str;
        if (baseClassLoaded) {
            stringPoolPtr=heap.getStringIstance(value);
        }else {
            stringsBeforeClassLoaded.emplace_back(this);
            stringPoolPtr=0;
        }
    }
};

struct MCP_Integer:MetaConstantPoolItem {
    int value;
    MCP_Integer():value(0){}
    MCP_Integer(const readfile_constant_item_info& readfile_constant_item,
        const readfile_ConstantPool& conp) {
        tag=readfile_constant_item.tag;
        auto p=(Integer_info*)(&readfile_constant_item);
        value=p->value;
    }
};

struct MCP_Float:MetaConstantPoolItem {
    float value;
    MCP_Float():value(0){}
    MCP_Float(const readfile_constant_item_info& readfile_constant_item,
        const readfile_ConstantPool& conp) {
        tag=readfile_constant_item.tag;
        auto p=(Float_info*)(&readfile_constant_item);
        value=p->value;
    }
};

struct MCP_Long:MetaConstantPoolItem {
    int64_t value;
    MCP_Long():value(0){}
    MCP_Long(const readfile_constant_item_info& readfile_constant_item,
        const readfile_ConstantPool& conp) {
        tag=readfile_constant_item.tag;
        auto p=(Long_info*)(&readfile_constant_item);
        value=p->value;
    }
};

struct MCP_Double:MetaConstantPoolItem {
    double value;
    MCP_Double():value(0){}
    MCP_Double(const readfile_constant_item_info& readfile_constant_item,
        const readfile_ConstantPool& conp) {
        tag=readfile_constant_item.tag;
        auto p=(Double_info*)(&readfile_constant_item);
        value=p->value;
    }
};

struct MetaConstantPool {
    int count;
    std::vector<MetaConstantPoolItem*> pool;
    MetaConstantPool():count(0){}
    explicit MetaConstantPool(const ClassFile* class_file);
    ~MetaConstantPool() {
        for (auto& i:pool) {
            if (i){delete i;i=nullptr;}
        }
        count=0;
        pool.resize(0);
    }
};//常量池，存放字面量和符号引用
#endif //METACONSTANTPOOL_H