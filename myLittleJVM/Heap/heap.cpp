//
// Created by YZQ on 25-8-1.
//
#include "heap.h"
#include <codecvt>
#include "../MetaSpace/Oop.h"
#include <cstdlib>
#include <locale>
#include "../MetaSpace/ClassLoader.h"
Heap heap{};
std::unordered_map<std::string,uint32_t> cppStringToStringIstancePtr{};

void Heap::initial() {
    heapPtr=malloc(HEAP_SIZE);
    sPoolPtr=malloc(STRINGPOOL_SIZE);
}

void Heap::destroy() {
    if (heapPtr) {free(heapPtr);heapPtr=nullptr;}
    if (sPoolPtr){free(sPoolPtr);sPoolPtr=nullptr;}
}

void *Heap::getRowPointer(uint32_t objPtr) {
    if (objPtr>=sbias) {
        return ((char*)sPoolPtr+(objPtr-sbias));
    }else {
        return ((char*)heapPtr)+objPtr;
    }
}

uint32_t Heap::NewObj_Instance(uint32_t size) {//size=sizeof(T)+sizeof(Slots)
    if (hp+size>=HEAP_SIZE){return -1;}
    uint32_t rp=hp;
    void* op=(char*)heapPtr+hp;
    hp+=size;
    new(op)instanceOopDesc;
    return rp;
}

uint32_t Heap::NewObj_Array(uint32_t size) {//size=sizeof(T)+sizeof(Slots)
    if (hp+size>=HEAP_SIZE){return -1;}
    uint32_t rp=hp;
    void* op=(char*)heapPtr+hp;
    hp+=size;
    new(op)ArrayOop;
    return rp;
}

uint32_t Heap::NewObj_ArrayInPool(uint32_t size) {//size=sizeof(T)+sizeof(Slots)
    if (sp+size>=STRINGPOOL_SIZE || 0xffff-sp-sbias<=size){return -1;}
    uint32_t rp=sp+sbias;
    void* op=(char*)sPoolPtr+sp;
    sp+=size;
    new(op)ArrayOop;
    return rp;
}

uint32_t Heap::NewString(uint32_t size) {
    if (sp+size>=STRINGPOOL_SIZE || 0xffff-sp-sbias<=size){return -1;}
    uint32_t rp=sp+sbias;
    void* op=(char*)sPoolPtr+sp;
    sp+=size;
    new(op)instanceOopDesc;
    return rp;
}

uint32_t Heap::getStringIstance(const std::string &cppStr) {
    auto sf=cppStringToStringIstancePtr.find(cppStr);
    if (sf==cppStringToStringIstancePtr.end()) {//不存在
        //转化为utf16
        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> utf8Toutf16;
        //std::u16string utf16Str = utf8Toutf16.from_bytes(cppStr);
        std::u16string utf16Str;
        try {
            utf16Str = utf8Toutf16.from_bytes(cppStr);
        } catch (const std::range_error& e) {
            std::cerr << "Heap::getStringIstance发生转码错误" << e.what() << std::endl;
            utf16Str.resize(cppStr.size(),cppStr[0]);
        }
        //建立char[]数组
        uint32_t arrRef=NewObj_ArrayInPool(sizeof(ArrayOop)+utf16Str.size()*2);
        if (arrRef<=0)return -1;
        ArrayOop* arrInstance=(ArrayOop*)getRowPointer(arrRef);
        arrInstance->eleCount=utf16Str.size();
        Kclass* acf=my_class_loader.getInstanceClass("[C");
        if (acf==nullptr) {
            std::cerr<<"Heap::getStringIstance找不到[C"<<std::endl;
            return -1;
        }
        arrInstance->kclass=acf;
        arrInstance->runTimeInfo=0;
        arrInstance->elements.ptr=((char*)arrInstance)+sizeof(ArrayOop);
        memcpy(arrInstance->elements.ptr,utf16Str.c_str(),utf16Str.size()*2);
        //建立String实例
        instanceClass* stringClass=(instanceClass*)my_class_loader.getInstanceClass("java/lang/String");
        uint32_t sRef=NewString(sizeof(instanceOopDesc)+(stringClass->instanceSlotSize)<<2);
        if (sRef<=0)return -1;
        instanceOopDesc* strInstance=(instanceOopDesc*)(getRowPointer(sRef));
        strInstance->runTimeInfo=0;
        strInstance->kclass=stringClass;
        strInstance->instanceSlots=new Slots(stringClass->instanceSlotSize,
            (char*)strInstance+sizeof(instanceOopDesc));
        //手工完成String实例的初始化
        int vsi=(*(stringClass->instanceSlotMap.find("java/lang/String.value:[C"))).second;
        int csi=(*(stringClass->instanceSlotMap.find("java/lang/String.hash:I"))).second;
        strInstance->instanceSlots->putuI32(arrRef,vsi);
        strInstance->instanceSlots->putI32(0,csi);//coder=1表示UTF16，而0表示LATIN1
        cppStringToStringIstancePtr.emplace(cppStr,sRef);//添加
        return sRef;
    }else {
        return (*sf).second;
    }
}

uint32_t Heap::NewObj_ClassInstance(uint32_t size) {
    if (sp+size>=STRINGPOOL_SIZE || 0xffff-sp-sbias<=size){return -1;}
    uint32_t rp=sp+sbias;
    void* op=(char*)sPoolPtr+sp;
    sp+=size;
    new(op)classInstanceOopDesc;
    return rp;
}


void Heap::Delete(uint32_t objPtr) {
    //to do
}

