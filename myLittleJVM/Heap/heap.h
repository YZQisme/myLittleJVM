//
// Created by YZQ on 25-8-1.
//

#ifndef HEAP_H
#define HEAP_H

#include <cstdint>
#include <string>
#include <unordered_map>
#define HEAP_SIZE (64*1024*1024)
//16MB
#define STRINGPOOL_SIZE (8*1024*1024)
//逻辑上，字符串常量池在堆的里面，即：Heap和StringPool共享同一个逻辑寻址空间
//Heap使用低地址空间，String使用高地址空间

struct Heap {
    void* heapPtr=nullptr;
    void* sPoolPtr=nullptr;
    uint32_t hp=1;//heap
    uint32_t sp=0,sbias=0xefffffff;//string
    void initial();
    void destroy();
    void* getRowPointer(uint32_t objPtr);
    uint32_t NewObj_Instance(uint32_t size);
    uint32_t NewObj_ClassInstance(uint32_t size);
    uint32_t NewObj_Array(uint32_t size);
    uint32_t NewString(uint32_t size);
    uint32_t NewObj_ArrayInPool(uint32_t size);
    uint32_t getStringIstance(const std::string &cppStr);
    void Delete(uint32_t objPtr);
};
extern Heap heap;
extern std::unordered_map<std::string,uint32_t> cppStringToStringIstancePtr;
//一个实例对象在堆的存放方式：[Oop][Slots]

#endif //HEAP_H
