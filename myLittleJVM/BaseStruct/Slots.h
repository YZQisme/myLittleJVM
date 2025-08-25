//
// Created by YZQ on 25-8-1.
//

#ifndef SLOTS_H
#define SLOTS_H
#include <cstdint>
#include <cstdlib>

struct Slots {
    int size=0;//slot
    uint32_t* ptr=nullptr;
    int canDel=1;
    void reset0() {
        for (int i = 0; i < size; ++i) {
            *(ptr+i)=0;
        }
    }
    Slots()=default;
    explicit Slots(int size) {
        if (size>0) {
            this->size=size;
            ptr=(uint32_t*) std::malloc(size<<2);
            reset0();
        }
    }
    Slots(int size,void* place) {
        canDel=0;
        if (size>0) {
            this->size=size;
            ptr=(uint32_t*) place;
            reset0();
        }
    }
    ~Slots() {
        if (ptr && canDel){free(ptr);ptr=nullptr;}
        size=0;
    }

    void putuI32(uint32_t val,int index) const {
        ptr[index]=val;
    }
    uint32_t getuI32(int index) const {
        return ptr[index];
    }

    void putI32(int32_t val,int index) const {
        ptr[index]=*((uint32_t*)(&val));
    }
    int32_t getI32(int index) const {
        auto p=(int32_t*)(ptr+index);
        return *p;
    }

    void putF(float val,int index) const {
        ptr[index]=*((uint32_t*)(&val));
    }
    float getF(int index) const {
        auto p=(float*)(ptr+index);
        return *p;
    }

    void putI64(int64_t val,int index) const {
        auto p=(int64_t*)(ptr+index);
        *p=val;
    }
    int64_t getI64(int index) const {
        auto p=(int64_t*)(ptr+index);
        return *p;
    }

    void putD(double val,int index) const {
        auto p=(double*)(ptr+index);
        *p=val;
    }
    double getD(int index) const {
        auto p=(double*)(ptr+index);
        return *p;
    }

    void putuI64(uint64_t val,int index) const {
        auto p=(uint64_t*)(ptr+index);
        *p=val;
    }
    uint64_t getuI64(int index) const {
        auto p=(uint64_t*)(ptr+index);
        return *p;
    }

    void putuI32n(const uint32_t* pptr,int psize,int index) {
        void* d=ptr+index;
        memcpy(d,pptr,psize<<2);
    }
    void getuI32n(uint32_t* pptr,int psize,int index) {
        const void* s=ptr+index;
        memcpy(pptr,s,psize<<2);
    }
};
#endif //SLOTS_H
