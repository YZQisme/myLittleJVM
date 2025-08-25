//
// Created by YZQ on 25-7-9.
//
#include "Kclass.h"
#include "../Execute/Executor.h"

MetaField::MetaField(const Field &field, const readfile_ConstantPool &constant_pool) {
    accessFlags=field.access_flags;
    name=((Utf8_info*)(constant_pool.pool[field.name_index]))->str;
    descriptor=((Utf8_info*)(constant_pool.pool[field.descriptor_index]))->str;
    for (int i = 0; i < field.attributes->attributes_count; ++i) {
        auto p=constant_pool.pool[field.attributes->attributes[i]->attribute_name_index];
        std::string st=((Utf8_info*)p)->str;
        if (st=="ConstantValue") {
            auto q=(NormalAttribute*)(field.attributes->attributes[i]);
            int ul;
            auto* cp=new ConstantValueAttribute(q->info,&ul);
            constantValueIndex=cp->constantvalue_index;
            delete cp;
            break;
        }
    }
}

MetaMethod::MetaMethod(const Method &method, const readfile_ConstantPool &constant_pool) {
    accessFlags=method.access_flags;
    name=((Utf8_info*)(constant_pool.pool[method.name_index]))->str;
    descriptor=((Utf8_info*)(constant_pool.pool[method.descriptor_index]))->str;
    for (int i = 0; i < method.attributes->attributes_count; ++i) {
        auto p=constant_pool.pool[method.attributes->attributes[i]->attribute_name_index];
        std::string st=((Utf8_info*)p)->str;
        if (st=="Code") {
            auto q=(NormalAttribute*)(method.attributes->attributes[i]);
            int ul;
            auto* cp=new CodeAttribute(q->info,&ul);
            maxOpStackSize=cp->max_stack;
            maxLocalVTSize=cp->max_locals;
            code=cp->codes;
            exceptionTable=cp->exceptionTable;
            cp->exceptionTable=nullptr;
            cp->codes=nullptr;
            delete cp;
            break;
        }
    }
}

instanceClass::instanceClass(const ClassFile &classfile) {
    accessFlag=classfile.access_flags;
    const auto& cp=classfile.constantPool->pool;
    auto cnt=(Class_info*)(cp[classfile.this_class]);
    name=((Utf8_info*)(cp[cnt->name_index]))->str;
    if (classfile.super_class>0) {
        auto snt=(Class_info*)(cp[classfile.super_class]);
        fatherName=((Utf8_info*)(cp[snt->name_index]))->str;
    }else {
        fatherName="";
    }

    constantPool=new MetaConstantPool(&classfile);

    metaFieldCount=classfile.fields->fields_count;
    if (metaFieldCount>0) {
        fields.resize(metaFieldCount,nullptr);
        for (int i = 0; i < metaFieldCount; ++i) {
            fields[i]=new MetaField(*(classfile.fields->fields[i]),
                *(classfile.constantPool));
        }
    }

    metaMethodCount=classfile.methods->methods_count;
    if (metaMethodCount>0) {
        methods.resize(metaMethodCount,nullptr);
        for (int i = 0; i < metaMethodCount; ++i) {
            methods[i]=new MetaMethod(*(classfile.methods->methods[i]),
                *(classfile.constantPool));
        }
    }

    interfaceCount=classfile.interfaces->interfaces_count;
    if (interfaceCount>0) {
        interfacesName.resize(interfaceCount);
        interfaces.resize(interfaceCount,nullptr);
        for (int i = 0; i < interfaceCount; ++i) {
            auto intp=(Class_info*)(cp[classfile.interfaces->interface_name_index[i]]);
            interfacesName[i]=((Utf8_info*)(cp[intp->name_index]))->str;
        }
    }
}

void buildField() {

}
int instanceClass::buildSlotMap() {
    staticSlotMap.clear();
    staticSlots=nullptr;
    instanceSlotSize=0;
    instanceSlotMap.clear();
    int staticSlotInc=0,instanceSlotInc=0,inct,staticSlotCount=0;
    if (father) {
        instanceClass* fa=(instanceClass*)father;
        staticSlotCount=((fa->staticSlots==nullptr)?0:(fa->staticSlots->size));
        staticSlotMap=fa->staticSlotMap;
        instanceSlotSize=fa->instanceSlotSize;
        instanceSlotMap=fa->instanceSlotMap;
    }
    for (int i = 0; i < metaFieldCount; ++i) {
        const auto f=fields[i];
        inct=TypeMapToSlotSize[f->descriptor[0]];
        if (f->accessFlags & 0x8) {//static field
            staticSlotMap.emplace(name+"."+f->name+":"+f->descriptor,
                staticSlotCount+staticSlotInc);
            staticSlotInc+=inct;
        }else {//不是static field
            instanceSlotMap.emplace(name+"."+f->name+":"+f->descriptor,
                instanceSlotSize+instanceSlotInc);
            instanceSlotInc+=inct;
        }
    }
    staticSlotCount+=staticSlotInc;
    instanceSlotSize+=instanceSlotInc;
    //对static slot的一部分进行赋值
    staticSlots=new Slots(staticSlotCount);
    if (father) {
        const auto fa=(instanceClass*)father;
        if (fa->staticSlots && fa->staticSlots->ptr)
            memcpy(staticSlots->ptr,fa->staticSlots->ptr,(fa->staticSlots->size)<<2);
    }

    for (int i = 0; i < metaFieldCount; ++i) {
        const auto f=fields[i];
        //static final的基本数据类型和字符串（static final String s="..."）需要被初始化到slot里
        if ((f->accessFlags & 0x18)==0x18) {//static final
            std::string t=name+"."+f->name+":"+f->descriptor;
            int pos=staticSlotMap[t];
            if (f->constantValueIndex==-1){continue;}
            int tt=constantPool->pool[f->constantValueIndex]->tag;
            if (tt==CONSTANT_String) {
                auto p=(MCP_String*)(constantPool->pool[f->constantValueIndex]);
                staticSlots->putuI32(p->stringPoolPtr,pos);
            }else if (tt==CONSTANT_Integer) {
                auto p=(MCP_Integer*)(constantPool->pool[f->constantValueIndex]);
                staticSlots->putI32(p->value,pos);
            }else if (tt==CONSTANT_Double) {
                auto p=(MCP_Double*)(constantPool->pool[f->constantValueIndex]);
                staticSlots->putD(p->value,pos);
            }else if (tt==CONSTANT_Long) {
                auto p=(MCP_Long*)(constantPool->pool[f->constantValueIndex]);
                staticSlots->putI64(p->value,pos);
            }else if (tt==CONSTANT_Float) {
                auto p=(MCP_Float*)(constantPool->pool[f->constantValueIndex]);
                staticSlots->putF(p->value,pos);
            }else {
                std::cerr<<"static final 初始化错误"<<std::endl;
            }
        }
    }
    return 0;
}