//
// Created by YZQ on 25-8-4.
//

#ifndef MINIVECTOR_H
#define MINIVECTOR_H
struct miniVector {//不拥有对内存空间的free权力
    void* ptr=nullptr;
    miniVector()= default;
    explicit miniVector(void* ptr) {
        this->ptr=ptr;
    }
    ~miniVector(){ptr=nullptr;}

    template<typename T>
    void put(T val,int i) {
        T* p=(T*)ptr;
        *(p+i)=val;
    }

    template<typename T>
    T get(int i) {
        T* p=(T*)ptr;
        return *(p+i);
    }
};

#endif //MINIVECTOR_H

