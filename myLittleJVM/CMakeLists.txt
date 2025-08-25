cmake_minimum_required(VERSION 3.31)
project(myLittleJVM)

set(CMAKE_CXX_STANDARD 11)

add_executable(myLittleJVM
        main.cpp
        BaseStruct/miniVector.h
        BaseStruct/Slots.h
        ClassFileRead/ClassFileFormat.h
        ClassFileRead/ClassFileFormat.cpp
        Stack/Stack.h
        Stack/Stack.cpp
        MetaSpace/Kclass.h
        MetaSpace/Kclass.cpp
        MetaSpace/MetaConstantPool.h
        MetaSpace/MetaConstantPool.cpp
        MetaSpace/Oop.h
        Heap/heap.h
        Heap/heap.cpp
        Execute/Instructions.h
        Execute/Instructions.cpp
        Execute/Executor.h
        Execute/Executor.cpp
        Execute/NativeMethod.h
        MetaSpace/ClassLoader.h
        MetaSpace/ClassLoader.cpp
        BaseStruct/BaseStruction.h
        BaseStruct/BaseStruction.cpp
        Execute/NativeMethod.cpp

)
