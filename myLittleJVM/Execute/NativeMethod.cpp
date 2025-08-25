//
// Created by YZQ on 25-8-7.
//
#include "NativeMethod.h"
#include <codecvt>
#include <locale>
#include <cmath>
#include "../Heap/heap.h"
#include "../MetaSpace/Oop.h"
#include "Instructions.h"
#include "../MetaSpace/ClassLoader.h"

int java_lang_Class_registerNatives_V(Execute *execute,uint64_t* ret) {
    return RUN_CONTINUE;
}

int java_lang_System_registerNatives_V(Execute* execute,uint64_t* ret) {
    return RUN_CONTINUE;
}

int java_lang_Object_registerNatives_V(Execute *execute, uint64_t *ret) {
    return RUN_CONTINUE;
}

int java_lang_Class_desiredAssertionStatus0_LC_V(Execute *execute, uint64_t *ret) {
    *ret=0;
    return RUN_CONTINUE;
}

int java_lang_System_arraycopy_LOILOII_V(Execute *execute, uint64_t *ret) {
    uint32_t srcObj=execute->stack_frame->local_variable_table->getuI32(0);
    int32_t srcPos=execute->stack_frame->local_variable_table->getI32(1);
    uint32_t desObj=execute->stack_frame->local_variable_table->getuI32(2);
    int32_t desPos=execute->stack_frame->local_variable_table->getI32(3);
    int32_t length=execute->stack_frame->local_variable_table->getI32(4);
    if (srcObj==0||desObj==0) {
        std::cerr<<"空指针错误"<<std::endl;
        return RUN_ERROR;
    }
    ArrayOop* srcPtr=(ArrayOop*)heap.getRowPointer(srcObj),
    * desPtr=(ArrayOop*)heap.getRowPointer(desObj);
    if (length+srcPos>srcPtr->eleCount || length+desPos>desPtr->eleCount) {
        std::cerr<<"数组越界"<<std::endl;
        return RUN_ERROR;
    }
    int isSre = isSon_Array(srcPtr->kclass->name,desPtr->kclass->name);
    int isSres=isSon_Array(desPtr->kclass->name,srcPtr->kclass->name);
    if (isSre!=1 && isSres!=1) {
        std::cerr<<"数组类型不匹配"<<std::endl;
        return RUN_ERROR;
    }
    switch (srcPtr->kclass->name[1]) {
        //{'B',1},{'C',2},{'D',8},{'F',4},{'I',4},{'J',8},{'L',4},{'S',2},{'Z',1},{'[',4}
        //B byte    Z bool    J long
        case 'B'://byte
        case 'Z'://bool//1
            memcpy(
                ((char*)(desPtr->elements.ptr))+desPos,
                ((const char*)(srcPtr->elements.ptr))+srcPos,
                length);
            break;
        case 'C':
        case 'S'://2
            memcpy(
                ((char*)(desPtr->elements.ptr))+(desPos<<1),
                ((const char*)(srcPtr->elements.ptr))+(srcPos<<1),
                length<<1);
            break;
        case 'F':
        case 'I':
        case '[':
        case 'L':
            memcpy(
                ((char*)(desPtr->elements.ptr))+(desPos<<2),
                ((const char*)(srcPtr->elements.ptr))+(srcPos<<2),
                length<<2);
            break;
        case 'D':
        case 'J'://long //8
            memcpy(
                ((char*)(desPtr->elements.ptr))+(desPos<<3),
                ((const char*)(srcPtr->elements.ptr))+(srcPos<<3),
                length<<3);
            break;
        default:
            std::cerr<<"类型错误"<<std::endl;
            return RUN_ERROR;
    };
    return RUN_CONTINUE;
}

int java_lang_Class_getPrimitiveClass_LS_LC(Execute *execute, uint64_t *ret) {
    uint32_t SObj=execute->stack_frame->local_variable_table->getuI32(0);
    if (SObj<=0) {
        std::cerr<<"空指针错误"<<std::endl;
        return RUN_ERROR;
    }
    auto* SPtr=(instanceOopDesc*)heap.getRowPointer(SObj);
    auto* SClass=(instanceClass*)(SPtr->kclass);
    auto fii=SClass->instanceSlotMap.find("java/lang/String.value:[C");
    if (fii==SClass->instanceSlotMap.end()) {
        std::cerr<<"在java_lang_Class_getPrimitiveClass_LS_LC中java/lang/String.value:[C不存在"<<std::endl;
        return RUN_ERROR;
    }
    int ci=(*fii).second;
    uint32_t CArrObj=SPtr->instanceSlots->getuI32(ci);
    if (CArrObj<=0) {
        std::cerr<<"String构造错误（NativeMethod）"<<std::endl;
        return RUN_ERROR;
    }
    auto* CArrPtr=(ArrayOop*)heap.getRowPointer(CArrObj);
    if (CArrPtr->kclass->name!="[C") {
        std::cerr<<"String内部char数组错误（NativeMethod）"<<std::endl;
        return RUN_ERROR;
    }
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> u16Tou8;
    std::u16string u16((const char16_t*)(CArrPtr->elements.ptr),CArrPtr->eleCount);
    std::string u8=u16Tou8.to_bytes(u16);
    auto ft=my_class_loader.loadedClass.find(u8);
    if (ft==my_class_loader.loadedClass.end()) {
        std::cerr<<"基本类型"<<u8<<"未加载（NativeMethod）"<<std::endl;
        return RUN_ERROR;
    }
    uint32_t re=(*ft).second->ClassInstancePtr;
    if (re<=0) {
        std::cerr<<"基本类型"<<u8<<"空指针错误（NativeMethod）"<<std::endl;
        return RUN_ERROR;
    }
    auto* rr=(uint32_t*)(ret);
    *rr=re;
    return RUN_CONTINUE;
}

int java_lang_Float_floatToRawIntBits_F_I(Execute *execute, uint64_t *ret) {
    float i=execute->stack_frame->local_variable_table->getF(0);
    int32_t r=*((int32_t*)(&i)),*rrr=(int32_t*)ret;
    *rrr=r;
    return RUN_CONTINUE;
}

int java_lang_Double_doubleToRawLongBits_D_J(Execute *execute, uint64_t *ret) {
    double i=execute->stack_frame->local_variable_table->getD(0);
    int64_t r=*((int64_t*)(&i)),*rrr=(int64_t*)ret;
    *rrr=r;
    return RUN_CONTINUE;
}

int java_lang_Double_longBitsToDouble_J_D(Execute *execute, uint64_t *ret) {
    int64_t i=execute->stack_frame->local_variable_table->getI64(0);
    double r=*((double*)(&i)),*rrr=(double*)ret;
    *rrr=r;
    return RUN_CONTINUE;
}

int sun_misc_VM_initialize_V(Execute *execute, uint64_t *ret) {
    Execute iexecute;
    if (iexecute.initialStack("java/lang/System","initializeSystemClass","()V")==0) {
        iexecute.run();
        if (iexecute.runState!=RUN_FINISH) {
            std::cerr<<"java/lang/System.initializeSystemClass()运行失败（sun_misc_VM_initialize_V）"<<std::endl;
            return RUN_ERROR;
        }
    }else{
        std::cerr<<"找不到java/lang/System.initializeSystemClass（sun_misc_VM_initialize_V）"<<std::endl;
        return RUN_ERROR;
    }
    return RUN_CONTINUE;
}

//java/lang/System.initProperties:(Ljava/util/Properties;)Ljava/util/Properties;
int java_lang_System_initProperties_LP_LP(Execute *execute, uint64_t *ret) {
    //to do
    return RUN_CONTINUE;
}

//java/io/FileInputStream.initIDs:()V
int java_io_FileInputStream_initIDs_V(Execute *execute, uint64_t *ret) {
    return RUN_CONTINUE;
}

//java/io/FileDescriptor.initIDs:()V
int java_io_FileDescriptor_initIDs_V(Execute *execute, uint64_t *ret) {
    return RUN_CONTINUE;
}

//sun/misc/Unsafe.registerNatives:()V
int sun_misc_Unsafe_registerNatives_V(Execute *execute, uint64_t *ret) {
    return RUN_CONTINUE;
}

//java/lang/Object.hashCode:()I
int java_lang_Object_hashCode_I(Execute *execute, uint64_t *ret) {
    uint32_t objRef=execute->stack_frame->local_variable_table->getuI32(0);
    if (objRef<=0) {
        std::cerr<<"java/lang/Object.hashCode:()I空指针错误"<<std::endl;
        return RUN_ERROR;
    }
    OopDesc* objPtr=(OopDesc*)heap.getRowPointer(objRef);
    int32_t re=objPtr->hashCode,*rr=(int32_t*)ret;
    *rr=re;
    return RUN_CONTINUE;
}

//sun/misc/Unsafe.arrayBaseOffset:(Ljava/lang/Class;)I
int sun_misc_Unsafe_arrayBaseOffset_LC_I(Execute *execute, uint64_t *ret) {
    auto* rr=(int32_t*)ret;
    *rr=40;
    return RUN_CONTINUE;
}

//sun/misc/Unsafe.arrayIndexScale:(Ljava/lang/Class;)I
int sun_misc_Unsafe_arrayIndexScale_LC_I(Execute *execute, uint64_t *ret) {
    uint32_t objRef=execute->stack_frame->local_variable_table->getuI32(0);
    if (objRef<=0) {
        std::cerr<<"sun/misc/Unsafe.arrayIndexScale:(Ljava/lang/Class;)I空指针错误"<<std::endl;
        return RUN_ERROR;
    }
    OopDesc* objPtr=(OopDesc*)heap.getRowPointer(objRef);
    if (objPtr->kclass->name[0]!='[') {
        auto* rr=(int32_t*)ret;
        *rr=4;
        return RUN_CONTINUE;
        //std::cerr<<"sun/misc/Unsafe.arrayIndexScale:(Ljava/lang/Class;)I非数组错误"<<std::endl;
        //return RUN_ERROR;
    }
    auto tsf=TypeToBtyeSize.find(objPtr->kclass->name[1]);
    if (tsf==TypeToBtyeSize.end()) {
        std::cerr<<"sun/misc/Unsafe.arrayIndexScale:(Ljava/lang/Class;)I数组类型错误"<<std::endl;
        return RUN_ERROR;
    }
    auto* rr=(int32_t*)ret;
    *rr=(*tsf).second;
    return RUN_CONTINUE;
}

//sun/misc/Unsafe.addressSize:()I
int sun_misc_Unsafe_addressSize_I(Execute *execute, uint64_t *ret) {
    auto* rr=(int32_t*)ret;
    *rr=8;
    return RUN_CONTINUE;
}

//sun/reflect/Reflection.getCallerClass:()Ljava/lang/Class;
int sun_reflect_Reflection_getCallerClass_LC(Execute *execute, uint64_t *ret) {
    StackFrame* top=execute->stack.top();execute->stack.pop();
    StackFrame* reF=nullptr;
    if (!execute->stack.empty()) {
        reF=execute->stack.top();
    }
    execute->stack.push(top);
    auto* rr=(uint32_t*)ret;
    if (reF) {
        *rr=top->Class->ClassInstancePtr;
    }else {
        std::cerr<<"sun/reflect/Reflection.getCallerClass:()Ljava/lang/Class;擅自返回了nullptr"<<std::endl;
        *rr=0;
    }
    return RUN_CONTINUE;
}

//java/io/FileDescriptor.set:(I)J
int java_io_FileDescriptor_set_I_J(Execute *execute, uint64_t *ret) {
    *ret=0;
    return RUN_CONTINUE;
}

//java/io/FileOutputStream.initIDs:()V
int java_io_FileOutputStream_initIDs_V(Execute *execute, uint64_t *ret) {
    return RUN_CONTINUE;
}

//java/security/AccessController.doPrivileged:(Ljava/security/PrivilegedExceptionAction;)Ljava/lang/Object;
int java_security_AccessController_doPrivileged_LP_LO(Execute *execute, uint64_t *ret) {
    uint32_t PERef=execute->stack_frame->local_variable_table->getuI32(0);
    if (PERef<=0) {
        std::cerr<<"java/security/AccessController.doPrivileged:(Ljava/security/PrivilegedExceptionAction;)Ljava/lang/Object;空指针错误"<<std::endl;
        return RUN_ERROR;
    }
    auto temp=(instanceOopDesc*)heap.getRowPointer(PERef);
    if (!temp || !(temp->kclass)) {
        std::cerr<<"java/security/AccessController.doPrivileged:(Ljava/security/PrivilegedExceptionAction;)Ljava/lang/Object;空Klass错误"<<std::endl;
        return RUN_ERROR;
    }
    instanceClass* PEPtr=(instanceClass*)(temp->kclass);
    const MetaMethod* runM=nullptr;
    for (const auto i:PEPtr->methods) {
        if (i->name=="run" && i->descriptor=="()Ljava/lang/Object;") {
            runM=i;break;
        }
    }
    if (runM==nullptr) {
        std::cerr<<"java/security/AccessController.doPrivileged:(Ljava/security/PrivilegedExceptionAction;)Ljava/lang/Object;找不到方法run"<<std::endl;
        return RUN_ERROR;
    }
    Execute exeRun;
    exeRun.stack_frame=new StackFrame(runM->maxLocalVTSize,runM->maxOpStackSize,
        runM->code->code,false,PEPtr,temp,runM);
    exeRun.stack.emplace(exeRun.stack_frame);
    exeRun.code=runM->code->code;
    exeRun.stack_frame->local_variable_table->putuI32(PERef,0);
    exeRun.runState=RUN_ABLE;
    exeRun.run();
    if (exeRun.runState!=RUN_FINISH) {
        std::cerr<<"java/security/AccessController.doPrivileged:(Ljava/security/PrivilegedExceptionAction;)Ljava/lang/Object;运行方法run失败"<<std::endl;
        return RUN_ERROR;
    }
    *ret=exeRun.lastResult;
    return RUN_CONTINUE;
}

//java/lang/Thread.registerNatives:()V
int java_lang_Thread_registerNatives_V(Execute *execute, uint64_t *ret) {
    return RUN_CONTINUE;
}

int64_t getObjectByClassName(const std::string& className) {
    auto* tCla=(instanceClass*)(my_class_loader
        .getInstanceClass(className));
    if (tCla==nullptr) {
        return -1;//找不到类
    }
    uint32_t insRef=heap.NewObj_Instance(sizeof(instanceOopDesc)+
        tCla->instanceSlotSize*4);
    if (insRef<=0) {
        return -2;//无法建立实例
    }
    auto* insPtr=(instanceOopDesc*)heap.getRowPointer(insRef);
    insPtr->instanceSlots=new Slots(tCla->instanceSlotSize,
        (char*)insPtr+sizeof(instanceOopDesc));
    insPtr->kclass=tCla;
    insPtr->runTimeInfo=0;
    return insRef;
}
//java/lang/Thread.currentThread:()Ljava/lang/Thread;
int java_lang_Thread_currentThread_LT(Execute *execute, uint64_t *ret) {
    //to do
    int64_t threadObj=getObjectByClassName("java/lang/Thread");
    if (threadObj==-1) {
        std::cerr<<"java/lang/Thread.currentThread:()Ljava/lang/Thread;找不到类java/lang/Thread"<<std::endl;
        return RUN_ERROR;
    }else if (threadObj==-2) {
        std::cerr<<"java/lang/Thread.currentThread:()Ljava/lang/Thread;无法建立类java/lang/Thread的实例"<<std::endl;
        return RUN_ERROR;
    }
    int64_t threadGroupObj=getObjectByClassName("java/lang/ThreadGroup");
    if (threadGroupObj==-1) {
        std::cerr<<"java/lang/Thread.currentThread:()Ljava/lang/Thread;找不到类java/lang/ThreadGroup"<<std::endl;
        return RUN_ERROR;
    }else if (threadGroupObj==-2) {
        std::cerr<<"java/lang/Thread.currentThread:()Ljava/lang/Thread;无法建立类java/lang/ThreadGroup的实例"<<std::endl;
        return RUN_ERROR;
    }
    auto threadP=(uint32_t)threadObj;
    auto* threadPtr=(instanceOopDesc*)(heap.getRowPointer(threadP));
    int gi=(*(
        ((instanceClass*)(threadPtr->kclass))->instanceSlotMap.find("java/lang/Thread.group:Ljava/lang/ThreadGroup;")
        )).second;
    int ii=(*(
        ((instanceClass*)(threadPtr->kclass))->instanceSlotMap.find("java/lang/Thread.priority:I")
        )).second;
    threadPtr->instanceSlots->putuI32((uint32_t)threadGroupObj,gi);
    threadPtr->instanceSlots->putI32(1,ii);
    *ret=threadObj;
    return RUN_CONTINUE;
}

//java/lang/Class.getName0:()Ljava/lang/String;
int java_lang_Class_getName0_LS(Execute *execute, uint64_t *ret) {
    uint32_t cRef=execute->stack_frame->local_variable_table->getuI32(0);
    if (cRef<=0) {
        std::cerr<<"java/lang/Class.getName0:()Ljava/lang/String;空指针错误"<<std::endl;
        return RUN_ERROR;
    }
    auto cPtr=(classInstanceOopDesc*)heap.getRowPointer(cRef);
    if (cPtr && cPtr->kclassManaged) {
        uint32_t sRef=heap.getStringIstance(cPtr->kclassManaged->name);
        uint32_t* rr=(uint32_t*)ret;
        *rr=sRef;
        return RUN_CONTINUE;
    }else {
        std::cerr<<"java/lang/Class.getName0:()Ljava/lang/String;发生错误"<<std::endl;
        return RUN_ERROR;
    }
}

//java/lang/Class.forName0:(Ljava/lang/String;ZLjava/lang/ClassLoader;Ljava/lang/Class;)Ljava/lang/Class;
int java_lang_Class_forName0_LSZLCLC_LC(Execute *execute, uint64_t *ret) {
    uint32_t sRef=execute->stack_frame->local_variable_table->getuI32(0);
    if (sRef<=0) {
        std::cerr<<"java/lang/Class.forName0:(Ljava/lang/String;ZLjava/lang/ClassLoader;Ljava/lang/Class;)Ljava/lang/Class;空指针错误"<<std::endl;
        return RUN_ERROR;
    }
    auto sPtr=(instanceOopDesc*)heap.getRowPointer(sRef);
    auto sClass=(instanceClass*)(sPtr->kclass);
    int vsi=(*(sClass->instanceSlotMap.find("java/lang/String.value:[C"))).second;
    uint32_t cRef=sPtr->instanceSlots->getuI32(vsi);
    if (cRef<=0) {
        std::cerr<<"java/lang/Class.forName0:(Ljava/lang/String;ZLjava/lang/ClassLoader;Ljava/lang/Class;)Ljava/lang/Class;String的char数组空指针错误"<<std::endl;
        return RUN_ERROR;
    }
    auto cPtr=(ArrayOop*)heap.getRowPointer(cRef);

    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> u16Tou8;
    std::u16string u16((const char16_t*)(cPtr->elements.ptr),cPtr->eleCount);
    std::string u8=u16Tou8.to_bytes(u16);

    int32_t isRunClinit=execute->stack_frame->local_variable_table->getI32(1);
    Kclass* obj=my_class_loader.getInstanceClass(u8,
        isRunClinit?1:0);
    uint32_t* rr=(uint32_t*)ret;
    *rr=obj->ClassInstancePtr;
    if (*ret) {
        return RUN_CONTINUE;
    }else {
        std::cerr<<"java/lang/Class.forName0:(Ljava/lang/String;ZLjava/lang/ClassLoader;Ljava/lang/Class;)Ljava/lang/Class;加载的类有错误"<<std::endl;
        return RUN_ERROR;
    }
}

//java/security/AccessController.getStackAccessControlContext:()Ljava/security/AccessControlContext;
int java_security_AccessController_getStackAccessControlContext_LA(Execute *execute, uint64_t *ret) {
    *ret=0;//返回空指针
    return RUN_CONTINUE;
}

//java/lang/Thread.setPriority0:(I)V
int java_lang_Thread_setPriority0_I_V(Execute *execute, uint64_t *ret) {
    return RUN_CONTINUE;
}

//java/lang/Thread.isAlive:()Z
int java_lang_Thread_isAlive_Z(Execute *execute, uint64_t *ret) {
    *ret=0;
    return RUN_CONTINUE;
}

//java/lang/Thread.start0:()V
int java_lang_Thread_start0_V(Execute *execute, uint64_t *ret) {
    return RUN_CONTINUE;
}

//java/lang/Class.getDeclaredFields0:(Z)[Ljava/lang/reflect/Field;
int java_lang_Class_getDeclaredFields0_Z_ALF(Execute *execute, uint64_t *ret) {
    uint32_t classRef=execute->stack_frame->local_variable_table->getuI32(0);
    int32_t publicOnly=execute->stack_frame->local_variable_table->getI32(1);
    if (classRef<=0) {
        std::cerr<<"java/lang/Class.getDeclaredFields0:(Z)[Ljava/lang/reflect/Field;空指针错误"<<std::endl;
        return RUN_ERROR;
    }
    auto classPtr=(classInstanceOopDesc*)heap.getRowPointer(classRef);
    Kclass* tarPtr=classPtr->kclassManaged;
    std::vector<const MetaField*> tarFields;
    if (tarPtr->name[0]!='[') {
        if (publicOnly) {
            for (const auto i:((instanceClass*)tarPtr)->fields) {
                if (i->accessFlags & 0x1) {
                    tarFields.emplace_back(i);
                }
            }
        }else {
            auto t=((instanceClass*)tarPtr)->fields;
            tarFields.assign(t.begin(), t.end());
        }
    }
    ArrayClass* arrClass=(ArrayClass*)
        my_class_loader.getInstanceClass("[Ljava/lang/reflect/Field;");
    uint32_t arrRef=heap.NewObj_Array(sizeof(ArrayOop)+tarFields.size()*4);
    if (arrRef<=0) {
        std::cerr<<"java/lang/Class.getDeclaredFields0:(Z)[Ljava/lang/reflect/Field;建立数组失败"<<std::endl;
        return RUN_ERROR;
    }
    ArrayOop* arrPtr=(ArrayOop*)heap.getRowPointer(arrRef);
    arrPtr->eleCount=tarFields.size();
    arrPtr->runTimeInfo=0;
    arrPtr->kclass=arrClass;
    arrPtr->elements.ptr=(char*)arrPtr+sizeof(ArrayOop);
    instanceClass* fieldClass=(instanceClass*)
        my_class_loader.getInstanceClass("java/lang/reflect/Field");
    for (int i=0;i<tarFields.size();++i) {
        uint32_t filedRef=heap.NewObj_Instance(sizeof(instanceOopDesc)+
            fieldClass->instanceSlotSize*4);
        if (filedRef<=0) {
            std::cerr<<"java/lang/Class.getDeclaredFields0:(Z)[Ljava/lang/reflect/Field;建立field对象失败"<<std::endl;
            return RUN_ERROR;
        }
        auto fieldPtr=(instanceOopDesc*)heap.getRowPointer(filedRef);
        fieldPtr->kclass=fieldClass;
        fieldPtr->runTimeInfo=0;
        fieldPtr->instanceSlots=new Slots(fieldClass->instanceSlotSize,
            (char*)fieldPtr+sizeof(instanceOopDesc));
        arrPtr->elements.put<uint32_t>(filedRef,i);

        int ci=fieldClass->instanceSlotMap.find
            ("java/lang/reflect/Field.clazz:Ljava/lang/Class;")->second;
        fieldPtr->instanceSlots->putuI32(classRef,ci);

        int ni=fieldClass->instanceSlotMap.find
            ("java/lang/reflect/Field.name:Ljava/lang/String;")->second;
        uint32_t nnn=heap.getStringIstance(tarFields[i]->name);
        if (nnn==0) {
            std::cerr<<"java/lang/Class.getDeclaredFields0:(Z)[Ljava/lang/reflect/Field;建立String对象失败"<<std::endl;
            return RUN_ERROR;
        }
        fieldPtr->instanceSlots->putuI32(nnn,ni);

        int ti=fieldClass->instanceSlotMap.find
            ("java/lang/reflect/Field.type:Ljava/lang/Class;")->second;
        std::string tcc=tarFields[i]->descriptor;
        switch (tcc[0]) {
            //{'B',1},{'C',2},{'D',8},{'F',4},{'I',4},{'J',8},{'L',4},{'S',2},{'Z',1},{'[',4}
            //B byte    Z bool    J long
            //"void","boolean","byte","char","short","int","long","float","double"
            case 'L':
                tcc=tcc.substr(1,tcc.size()-2);
                break;
            case 'B':
               tcc="byte";
                break;
            case 'C':
                tcc="char";
                break;
            case 'D':
                tcc="double";
                break;
            case 'F':
                tcc="float";
                break;
            case 'I':
                tcc="int";
                break;
            case 'J':
                tcc="long";
                break;
            case 'S':
                tcc="short";
                break;
            case 'Z':
                tcc="boolean";
                break;
            case '[':
                break;
            default:
                std::cerr<<"java/lang/Class.getDeclaredFields0:(Z)[Ljava/lang/reflect/Field;找不到"<<tcc<<std::endl;
                return RUN_ERROR;
                break;
        }
        uint32_t tccc=my_class_loader.getInstanceClass(tcc)->ClassInstancePtr;
        if (tccc==0) {
            std::cerr<<"java/lang/Class.getDeclaredFields0:(Z)[Ljava/lang/reflect/Field;获取type的Class对象失败"<<std::endl;
            return RUN_ERROR;
        }
        fieldPtr->instanceSlots->putuI32(tccc,ti);

        int mi=fieldClass->instanceSlotMap.find
            ("java/lang/reflect/Field.modifiers:I")->second;
        fieldPtr->instanceSlots->putI32(tarFields[i]->accessFlags,mi);

        int si=fieldClass->instanceSlotMap.find
            ("java/lang/reflect/Field.slot:I")->second;
        fieldPtr->instanceSlots->putI32(i,si);

        int sgi=fieldClass->instanceSlotMap.find
            ("java/lang/reflect/Field.signature:Ljava/lang/String;")->second;
        fieldPtr->instanceSlots->putuI32(0,sgi);

        int ai=fieldClass->instanceSlotMap.find
            ("java/lang/reflect/Field.annotations:[B")->second;
        fieldPtr->instanceSlots->putuI32(0,ai);
    }
    uint32_t* rrr=(uint32_t*)ret;
    *rrr=arrRef;
    return RUN_CONTINUE;
}

//java/lang/ClassLoader.registerNatives:()V
int java_lang_ClassLoader_registerNatives_V(Execute *execute, uint64_t *ret) {
    return RUN_CONTINUE;
}

//sun/misc/Unsafe.compareAndSwapObject:(Ljava/lang/Object;JLjava/lang/Object;Ljava/lang/Object;)Z
int sun_misc_Unsafe_compareAndSwapObject_LOJLOLO_Z(Execute *execute, uint64_t *ret) {
    uint32_t objRef=execute->stack_frame->local_variable_table->getuI32(1);
    int64_t ofs=execute->stack_frame->local_variable_table->getI64(2);
    uint32_t expect=execute->stack_frame->local_variable_table->getuI32(4);
    uint32_t newVal=execute->stack_frame->local_variable_table->getuI32(5);
    if (objRef==0) {
        std::cerr<<"sun/misc/Unsafe.compareAndSwapObject:(Ljava/lang/Object;JLjava/lang/Object;Ljava/lang/Object;)Z空指针错误"<<std::endl;
        return RUN_ERROR;
    }
    //待续未完
    return RUN_CONTINUE;
}

//java/lang/Object.getClass:()Ljava/lang/Class;
int java_lang_Object_getClass_LC(Execute *execute, uint64_t *ret) {
    uint32_t objRef=execute->stack_frame->local_variable_table->getuI32(0);
    if (objRef==0) {
        std::cerr<<"java/lang/Object.getClass:()Ljava/lang/Class;空指针错误"<<std::endl;
        return RUN_ERROR;
    }
    OopDesc* objPtr=(OopDesc*)heap.getRowPointer(objRef);
    auto P=(uint32_t*)ret;
    *P=objPtr->kclass->ClassInstancePtr;
    return RUN_CONTINUE;
}

//myIO.print:(Ljava/lang/String;)V
int myIO_print_LS_V(Execute *execute, uint64_t *ret) {
    uint32_t objRef=execute->stack_frame->local_variable_table->getuI32(0);
    if (objRef==0) {
        std::cout<<"null"<<std::flush;
        return RUN_CONTINUE;
    }
    OopDesc* objPtr=(OopDesc*)heap.getRowPointer(objRef);
    if (objPtr->kclass->name=="java/lang/String") {
        auto stringPtr=(instanceOopDesc*)objPtr;
        auto stringClass=(instanceClass*)(objPtr->kclass);
        auto cf=stringClass->instanceSlotMap.find("java/lang/String.value:[C");
        if (cf==stringClass->instanceSlotMap.end()) {
            std::cerr<<"myIO.print:(Ljava/lang/String;)V输入的类型不是字符串"<<std::endl;
            return RUN_ERROR;
        }
        uint32_t cArrRef=stringPtr->instanceSlots->getuI32(cf->second);
        if (cArrRef==0) {
            std::cerr<<"myIO.print:(Ljava/lang/String;)V输入的类型不是字符串"<<std::endl;
            return RUN_ERROR;
        }
        ArrayOop* cArrPtr=(ArrayOop*)heap.getRowPointer(cArrRef);
        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> u16Tou8;
        std::u16string u16((const char16_t*)(cArrPtr->elements.ptr),cArrPtr->eleCount);
        std::string u8=u16Tou8.to_bytes(u16);
        std::cout<<u8<<std::flush;
    }else {
        std::cerr<<"myIO.print:(Ljava/lang/String;)V输入的类型不是字符串"<<std::endl;
        return RUN_ERROR;
    }
    return RUN_CONTINUE;
}

//myIO.println:(Ljava/lang/String;)V
int myIO_println_LS_V(Execute *execute, uint64_t *ret) {
    uint32_t objRef=execute->stack_frame->local_variable_table->getuI32(0);
    if (objRef==0) {
        std::cout<<"null"<<std::endl;
        return RUN_CONTINUE;
    }
    OopDesc* objPtr=(OopDesc*)heap.getRowPointer(objRef);
    if (objPtr->kclass->name=="java/lang/String") {
        auto stringPtr=(instanceOopDesc*)objPtr;
        auto stringClass=(instanceClass*)(objPtr->kclass);
        auto cf=stringClass->instanceSlotMap.find("java/lang/String.value:[C");
        if (cf==stringClass->instanceSlotMap.end()) {
            std::cerr<<"myIO.print:(Ljava/lang/String;)V输入的类型不是字符串"<<std::endl;
            return RUN_ERROR;
        }
        uint32_t cArrRef=stringPtr->instanceSlots->getuI32(cf->second);
        if (cArrRef==0) {
            std::cerr<<"myIO.print:(Ljava/lang/String;)V输入的类型不是字符串"<<std::endl;
            return RUN_ERROR;
        }
        ArrayOop* cArrPtr=(ArrayOop*)heap.getRowPointer(cArrRef);
        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> u16Tou8;
        std::u16string u16((const char16_t*)(cArrPtr->elements.ptr),cArrPtr->eleCount);
        std::string u8=u16Tou8.to_bytes(u16);
        std::cout<<u8<<std::endl;
    }else {
        std::cerr<<"myIO.print:(Ljava/lang/String;)V输入的类型不是字符串"<<std::endl;
        return RUN_ERROR;
    }
    return RUN_CONTINUE;
}

//myIO.print:(I)V
int myIO_print_I_V(Execute *execute, uint64_t *ret) {
    int32_t i=execute->stack_frame->local_variable_table->getI32(0);
    std::cout<<i<<std::flush;
    return RUN_CONTINUE;
}

//myIO.println:(I)V
int myIO_println_I_V(Execute *execute, uint64_t *ret) {
    int32_t i=execute->stack_frame->local_variable_table->getI32(0);
    std::cout<<i<<std::endl;
    return RUN_CONTINUE;
}

//myIO.print:(D)V
int myIO_print_D_V(Execute *execute, uint64_t *ret) {
    double i=execute->stack_frame->local_variable_table->getD(0);
    std::cout<<i<<std::flush;
    return RUN_CONTINUE;
}

//myIO.println:(D)V
int myIO_println_D_V(Execute *execute, uint64_t *ret) {
    double i=execute->stack_frame->local_variable_table->getD(0);
    std::cout<<i<<std::endl;
    return RUN_CONTINUE;
}

//myIO.print:(F)V
int myIO_print_F_V(Execute *execute, uint64_t *ret) {
    float i=execute->stack_frame->local_variable_table->getF(0);
    std::cout<<i<<std::flush;
    return RUN_CONTINUE;
}

//myIO.println:(F)V
int myIO_println_F_V(Execute *execute, uint64_t *ret) {
    float i=execute->stack_frame->local_variable_table->getF(0);
    std::cout<<i<<std::endl;
    return RUN_CONTINUE;
}

//myIO.print:(J)V
int myIO_print_J_V(Execute *execute, uint64_t *ret) {
    int64_t i=execute->stack_frame->local_variable_table->getI64(0);
    std::cout<<i<<std::flush;
    return RUN_CONTINUE;
}

//myIO.println:(J)V
int myIO_println_J_V(Execute *execute, uint64_t *ret) {
    int64_t i=execute->stack_frame->local_variable_table->getI64(0);
    std::cout<<i<<std::endl;
    return RUN_CONTINUE;
}

//myIO.print:(C)V
int myIO_print_C_V(Execute *execute, uint64_t *ret) {
    uint16_t i=execute->stack_frame->local_variable_table->getuI32(0);
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> u16Tou8;
    std::u16string u16((const char16_t*)(&i),1);
    std::string u8=u16Tou8.to_bytes(u16);
    std::cout<<u8<<std::flush;
    return RUN_CONTINUE;
}

//myIO.println:(C)V
int myIO_println_C_V(Execute *execute, uint64_t *ret) {
    uint16_t i=execute->stack_frame->local_variable_table->getuI32(0);
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> u16Tou8;
    std::u16string u16((const char16_t*)(&i),1);
    std::string u8=u16Tou8.to_bytes(u16);
    std::cout<<u8<<std::endl;
    return RUN_CONTINUE;
}

//myIO.print:(Z)V
int myIO_print_Z_V(Execute *execute, uint64_t *ret) {
    int32_t i=execute->stack_frame->local_variable_table->getI32(0);
    if (i==0) {
        std::cout<<"false"<<std::flush;
    }else {
        std::cout<<"ture"<<std::flush;
    }
    return RUN_CONTINUE;
}

//myIO.println:(Z)V
int myIO_println_Z_V(Execute *execute, uint64_t *ret) {
    int32_t i=execute->stack_frame->local_variable_table->getI32(0);
    if (i==0) {
        std::cout<<"false"<<std::endl;
    }else {
        std::cout<<"ture"<<std::endl;
    }
    return RUN_CONTINUE;
}

//myIO.readLine:()Ljava/lang/String;
int myIO_readLine_LS(Execute *execute, uint64_t *ret) {
    std::string line;
    std::getline(std::cin,line);
    uint32_t re=heap.getStringIstance(line);
    if (re==0) {
        std::cerr<<"myIO.readLine:()Ljava/lang/String;建立String失败"<<std::endl;
        return RUN_ERROR;
    }
    auto rr=(uint32_t*)ret;
    *rr=re;
    return RUN_CONTINUE;
}

//myIO.readString:()Ljava/lang/String;
int myIO_readString_LS(Execute *execute, uint64_t *ret) {
    std::string s;
    std::cin>>s;
    uint32_t re=heap.getStringIstance(s);
    if (re==0) {
        std::cerr<<"myIO.readString:()Ljava/lang/String;建立String失败"<<std::endl;
        return RUN_ERROR;
    }
    auto rr=(uint32_t*)ret;
    *rr=re;
    return RUN_CONTINUE;
}

//myIO.readInt:()I
int myIO_readInt_I(Execute *execute, uint64_t *ret) {
    int32_t s;
    std::cin>>s;
    auto rr=(int32_t*)ret;
    *rr=s;
    return RUN_CONTINUE;
}

//myIO.readLong:()J
int myIO_readLong_J(Execute *execute, uint64_t *ret) {
    int64_t s;
    std::cin>>s;
    auto rr=(int64_t*)ret;
    *rr=s;
    return RUN_CONTINUE;
}

//myIO.readChar:()C
int myIO_readChar_C(Execute *execute, uint64_t *ret) {
    std::string s;
    std::cin>>s;
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> conv;
    std::u16string u16 = conv.from_bytes(s);
    auto rr=(uint16_t*)ret;
    *rr=u16[0];
    return RUN_CONTINUE;
}

//myIO.readDouble:()D
int myIO_readDouble_D(Execute *execute, uint64_t *ret) {
    double s;
    std::cin>>s;
    auto rr=(double*)ret;
    *rr=s;
    return RUN_CONTINUE;
}

//myIO.readFloat:()F
int myIO_readFloat_F(Execute *execute, uint64_t *ret) {
    float s;
    std::cin>>s;
    auto rr=(float*)ret;
    *rr=s;
    return RUN_CONTINUE;
}

std::map<std::string,int (*)(Execute*,uint64_t*)>nativeMethodNameToFuncPtr={
    {
        "myIO.readFloat:()F",
        myIO_readFloat_F
    },
    {
        "myIO.readChar:()C",
        myIO_readChar_C
    },
    {
        "myIO.print:(Z)V",
        myIO_print_Z_V
    },
    {
        "myIO.println:(Z)V",
        myIO_println_Z_V
    },
    {
        "myIO.readDouble:()D",
        myIO_readDouble_D
    },
    {
        "myIO.readLong:()J",
        myIO_readLong_J
    },
    {
        "myIO.readInt:()I",
        myIO_readInt_I
    },
    {
        "myIO.readString:()Ljava/lang/String;",
        myIO_readString_LS
    },
    {
        "myIO.readLine:()Ljava/lang/String;",
        myIO_readLine_LS
    },
    {
        "myIO.println:(C)V",
        myIO_println_C_V
    },
    {
        "myIO.print:(C)V",
        myIO_print_C_V
    },
    {
        "myIO.println:(J)V",
        myIO_println_J_V
    },
    {
        "myIO.print:(J)V",
        myIO_print_J_V
    },
    {
        "myIO.println:(F)V",
        myIO_println_F_V
    },
    {
        "myIO.print:(F)V",
        myIO_print_F_V
    },
    {
        "myIO.println:(D)V",
        myIO_println_D_V
    },
    {
        "myIO.print:(D)V",
        myIO_print_D_V
    },
    {
        "myIO.println:(I)V",
        myIO_println_I_V
    },
    {
        "myIO.print:(I)V",
        myIO_print_I_V
    },
    {
        "myIO.println:(Ljava/lang/String;)V",
        myIO_println_LS_V
    },
    {
        "myIO.print:(Ljava/lang/String;)V",
        myIO_print_LS_V
    },
    {
        "java/lang/Object.getClass:()Ljava/lang/Class;",
        java_lang_Object_getClass_LC
    },
    {
        "sun/misc/Unsafe.compareAndSwapObject:(Ljava/lang/Object;JLjava/lang/Object;Ljava/lang/Object;)Z",
        sun_misc_Unsafe_compareAndSwapObject_LOJLOLO_Z
    },
    {
        "java/lang/ClassLoader.registerNatives:()V",
        java_lang_ClassLoader_registerNatives_V
    },
    {
        "java/lang/Class.getDeclaredFields0:(Z)[Ljava/lang/reflect/Field;",
        java_lang_Class_getDeclaredFields0_Z_ALF
    },
    {
        "java/lang/Thread.start0:()V",
        java_lang_Thread_start0_V
    },
    {
        "java/lang/Thread.isAlive:()Z",
        java_lang_Thread_isAlive_Z
    },
    {
        "java/lang/Thread.setPriority0:(I)V",
        java_lang_Thread_setPriority0_I_V
    },
    {
        "java/security/AccessController.getStackAccessControlContext:()Ljava/security/AccessControlContext;",
        java_security_AccessController_getStackAccessControlContext_LA
    },
    {
        "java/lang/Class.forName0:(Ljava/lang/String;ZLjava/lang/ClassLoader;Ljava/lang/Class;)Ljava/lang/Class;",
        java_lang_Class_forName0_LSZLCLC_LC
    },
    {
        "java/lang/Class.getName0:()Ljava/lang/String;",
        java_lang_Class_getName0_LS
    },
    {
        "java/lang/Thread.currentThread:()Ljava/lang/Thread;",
        java_lang_Thread_currentThread_LT
    },
    {
        "java/lang/Thread.registerNatives:()V",
        java_lang_Thread_registerNatives_V
    },
    {
        "java/security/AccessController.doPrivileged:(Ljava/security/PrivilegedAction;)Ljava/lang/Object;",
        java_security_AccessController_doPrivileged_LP_LO
    },
    {
        "java/security/AccessController.doPrivileged:(Ljava/security/PrivilegedExceptionAction;)Ljava/lang/Object;",
        java_security_AccessController_doPrivileged_LP_LO
    },
    {
        "java/io/FileOutputStream.initIDs:()V",
        java_io_FileOutputStream_initIDs_V
    },
    {
        "java/io/FileDescriptor.set:(I)J",
        java_io_FileDescriptor_set_I_J
    },
    {
        "sun/reflect/Reflection.getCallerClass:()Ljava/lang/Class;",
        sun_reflect_Reflection_getCallerClass_LC
    },
    {
        "sun/misc/Unsafe.addressSize:()I",
        sun_misc_Unsafe_addressSize_I
    },
    {
        "sun/misc/Unsafe.arrayIndexScale:(Ljava/lang/Class;)I",
        sun_misc_Unsafe_arrayIndexScale_LC_I
    },
    {
        "sun/misc/Unsafe.arrayBaseOffset:(Ljava/lang/Class;)I",
        sun_misc_Unsafe_arrayBaseOffset_LC_I
    },
    {
        "java/lang/Object.hashCode:()I",
        java_lang_Object_hashCode_I
    },
    {
        "sun/misc/Unsafe.registerNatives:()V",
        sun_misc_Unsafe_registerNatives_V
    },
    {
        "java/lang/Class.registerNatives:()V",
        java_lang_Class_registerNatives_V
    },
    {
        "java/lang/System.registerNatives:()V",
        java_lang_System_registerNatives_V
    },
    {
        "java/lang/Object.registerNatives:()V",
        java_lang_Object_registerNatives_V
    },
    {
        "java/lang/Class.desiredAssertionStatus0:(Ljava/lang/Class;)Z",
        java_lang_Class_desiredAssertionStatus0_LC_V
    },
    {
        "java/lang/System.arraycopy:(Ljava/lang/Object;ILjava/lang/Object;II)V",
        java_lang_System_arraycopy_LOILOII_V
    },
    {
        "java/lang/Class.getPrimitiveClass:(Ljava/lang/String;)Ljava/lang/Class;",
        java_lang_Class_getPrimitiveClass_LS_LC
    },
    {
        "java/lang/Float.floatToRawIntBits:(F)I",
        java_lang_Float_floatToRawIntBits_F_I
    },
    {
        "java/lang/Double.doubleToRawLongBits:(D)J",
        java_lang_Double_doubleToRawLongBits_D_J
    },
    {
        "java/lang/Double.longBitsToDouble:(J)D",
        java_lang_Double_longBitsToDouble_J_D
    },
    {
        "sun/misc/VM.initialize:()V",
        sun_misc_VM_initialize_V
    },
    {
        "java/lang/System.initProperties:(Ljava/util/Properties;)Ljava/util/Properties;",
        java_lang_System_initProperties_LP_LP
    },
    {
        "java/io/FileInputStream.initIDs:()V",
        java_io_FileInputStream_initIDs_V
    },
    {
        "java/io/FileDescriptor.initIDs:()V",
        java_io_FileDescriptor_initIDs_V
    }
};