//
// Created by YZQ on 25-8-5.
//
#include "ClassLoader.h"

#include "Oop.h"
#include "../Execute/Executor.h"
myClassLoader my_class_loader{};

ClassFile* myClassLoader::fileLoad(const std::string &classByteFileName) const {
    std::string t1=standardLibPath+classByteFileName+".class";
    ClassFile* re=nullptr;
    if ((re=readClassFile(t1))==nullptr) {
        t1=standardExtendLibPath+classByteFileName+".class";
        if ((re=readClassFile(t1))==nullptr) {
            t1=userLibPath+classByteFileName+".class";
            if ((re=readClassFile(t1))==nullptr) {
                std::cerr<<"找不到"<<classByteFileName<<std::endl;
                return re;
            }
        }
    }
    return re;
}

Kclass* myClassLoader::buildInstanceClass_ArrayClass(const std::string& classByteFileName) {
    auto sf=my_class_loader.loadedClass.find(classByteFileName);
    if (sf==my_class_loader.loadedClass.end()) {//找不到
        Kclass* re=nullptr;
        if (classByteFileName[0]=='[') {
            Kclass* elemTypeClassPtr=buildInstanceClass_ArrayClass(
                classByteFileName.substr(1));
            if (elemTypeClassPtr==nullptr) {
                re=nullptr;
            }else {
                ArrayClass* arrRe=new ArrayClass();
                auto ff=my_class_loader.loadedClass.find("java/lang/Object");
                if (ff==my_class_loader.loadedClass.end()) {
                    std::cerr<<"buildInstanceClass_ArrayClass不应该找不到java/lang/Object"<<std::endl;
                    return nullptr;
                }
                arrRe->father=ff->second;
                arrRe->fatherName="java/lang/Object";
                arrRe->name=classByteFileName;
                arrRe->elementTypeClassPtr=elemTypeClassPtr;
                arrRe->elementTypeStr=classByteFileName.substr(1);
                if (classByteFileName[1]=='[') {
                    arrRe->dimension=((ArrayClass*)elemTypeClassPtr)->dimension+1;
                }else {
                    arrRe->dimension=1;
                }
                if (baseClassLoaded) {
                    get_java_lang_Class_instance(arrRe);
                }
                re=arrRe;
                loadedClass.emplace(classByteFileName,re);
            }
        }
        else if (classByteFileName[0]=='L' &&
            classByteFileName[classByteFileName.size()-1]==';') {
            re=getInstanceClass(classByteFileName.substr(
                1,classByteFileName.size()-2));
        }
        else {
            std::unordered_map<std::string, Kclass *>::iterator bf;
            switch (classByteFileName[0]) {
                //{'B',1},{'C',2},{'D',8},{'F',4},{'I',4},{'J',8},{'L',4},{'S',2},{'Z',1},{'[',4}
                //B byte    Z bool    J long
                //"void","boolean","byte","char","short","int","long","float","double"
                case 'B':
                   bf=my_class_loader.loadedClass.find("byte");
                    if (bf==my_class_loader.loadedClass.end()) {
                        std::cerr<<"buildInstanceClass_ArrayClass不应该找不到byte"<<std::endl;
                        re=nullptr;
                    }else {
                        re=(*bf).second;
                    }
                    break;
                case 'C':
                    bf=my_class_loader.loadedClass.find("char");
                    if (bf==my_class_loader.loadedClass.end()) {
                        std::cerr<<"buildInstanceClass_ArrayClass不应该找不到char"<<std::endl;
                        re=nullptr;
                    }else {
                        re=(*bf).second;
                    }
                    break;
                case 'D':
                    bf=my_class_loader.loadedClass.find("double");
                    if (bf==my_class_loader.loadedClass.end()) {
                        std::cerr<<"buildInstanceClass_ArrayClass不应该找不到double"<<std::endl;
                        re=nullptr;
                    }else {
                        re=(*bf).second;
                    }
                    break;
                case 'F':
                    bf=my_class_loader.loadedClass.find("float");
                    if (bf==my_class_loader.loadedClass.end()) {
                        std::cerr<<"buildInstanceClass_ArrayClass不应该找不到float"<<std::endl;
                        re=nullptr;
                    }else {
                        re=(*bf).second;
                    }
                    break;
                case 'I':
                    bf=my_class_loader.loadedClass.find("int");
                    if (bf==my_class_loader.loadedClass.end()) {
                        std::cerr<<"buildInstanceClass_ArrayClass不应该找不到int"<<std::endl;
                        re=nullptr;
                    }else {
                        re=(*bf).second;
                    }
                    break;
                case 'J':
                    bf=my_class_loader.loadedClass.find("long");
                    if (bf==my_class_loader.loadedClass.end()) {
                        std::cerr<<"buildInstanceClass_ArrayClass不应该找不到long"<<std::endl;
                        re=nullptr;
                    }else {
                        re=(*bf).second;
                    }
                    break;
                case 'S':
                    bf=my_class_loader.loadedClass.find("short");
                    if (bf==my_class_loader.loadedClass.end()) {
                        std::cerr<<"buildInstanceClass_ArrayClass不应该找不到short"<<std::endl;
                        re=nullptr;
                    }else {
                        re=(*bf).second;
                    }
                    break;
                case 'Z':
                    bf=my_class_loader.loadedClass.find("boolean");
                    if (bf==my_class_loader.loadedClass.end()) {
                        std::cerr<<"buildInstanceClass_ArrayClass不应该找不到boolean"<<std::endl;
                        re=nullptr;
                    }else {
                        re=(*bf).second;
                    }
                    break;
                default:
                    std::cerr<<"buildInstanceClass_ArrayClass找不到"<<classByteFileName<<std::endl;
                    re=nullptr;
                    break;
            }
        }
        if (re==nullptr) {
            std::cerr<<classByteFileName<<"不存在或者其加载时机错误"<<std::endl;
        }
        return re;
    }else {
        return (*sf).second;
    }
}

Kclass *myClassLoader::buildInstanceClass_instanceClass(const std::string &classByteFileName,int runClinit) {
    ClassFile* class_file=fileLoad(classByteFileName);
    if (class_file==nullptr) {
        std::cerr<<classByteFileName<<".class文件加载失败"<<std::endl;
        return nullptr;
    }
    instanceClass* newclass=nullptr;
    if (classByteFileName=="java/lang/Class") {
        newclass=new instanceClass(*class_file);
    }else {
        newclass=new instanceMirrorClass(*class_file);
    }
    newclass->classLoader=this;
    if (newclass->fatherName.empty()) {
        newclass->father=nullptr;
    }else {
        auto lfind=loadedClass.find(newclass->fatherName);
        Kclass* fa=nullptr;
        if (lfind==loadedClass.end()) {//找不到对应的父类，要加载父类
            fa=buildInstanceClass_instanceClass(newclass->fatherName);
            if (fa==nullptr) {
                std::cerr<<classByteFileName<<".class文件加载失败"<<std::endl;
                return nullptr;
            }
        }else {
            fa=(*lfind).second;
        }
        newclass->father=fa;
    }
    newclass->buildSlotMap();
    loadedClass.emplace(classByteFileName,newclass);
    delete class_file;
    //加载该类的类对象，并完成newclass->ClassInstancePtr的赋值
    if (baseClassLoaded) {
        get_java_lang_Class_instance(newclass);
    }
    //加载接口
    for (int i=0;i<newclass->interfaceCount;++i) {
        newclass->interfaces[i]=(instanceClass*)
            getInstanceClass(newclass->interfacesName[i]);
    }
    //执行类的clinit方法
    if (runClinit && newclass->clinitStatement==0) {
        Execute iexecute;
        int initRe=iexecute.initialStack(classByteFileName,"<clinit>","()V");
        newclass->clinitStatement=1;
        if (initRe==0) {
            iexecute.run();
            if (iexecute.runState!=RUN_FINISH) {
                std::cerr<<classByteFileName<<".clinit()V运行失败(buildInstanceClass)"<<std::endl;
                newclass->clinitStatement=3;
                return nullptr;
            }
        }else if (initRe==-2){
            std::cerr<<classByteFileName<<".clinit()V初始化失败(buildInstanceClass)"<<std::endl;
            newclass->clinitStatement=3;
            return nullptr;
        }
        newclass->clinitStatement=2;
    }
    return newclass;
}

int myClassLoader::get_java_lang_Class_instance(Kclass* KclassManagebyClass) {
    instanceMirrorClass* ClassKlass=(instanceMirrorClass*)my_class_loader.getInstanceClass("java/lang/Class");
    uint32_t classRef=heap.NewObj_ClassInstance(
        sizeof(classInstanceOopDesc)+ClassKlass->instanceSlotSize*4);
    if (classRef<=0){return -1;}
    auto classInsPtr=(classInstanceOopDesc*)heap.getRowPointer(classRef);
    classInsPtr->kclass=ClassKlass;
    classInsPtr->runTimeInfo=0;
    classInsPtr->instanceSlots=new Slots(ClassKlass->instanceSlotSize,
        ((char*)classInsPtr)+sizeof(classInstanceOopDesc));
    classInsPtr->kclassManaged=KclassManagebyClass;
    KclassManagebyClass->ClassInstancePtr=classRef;
    return 0;
}

int myClassLoader::loadClass_java_lang_Object() {
    std::string i="java/lang/Object";
    Kclass* j=getInstanceClass(i);
    if (j!=nullptr) {
        loadedClass.emplace(i,j);
        return 0;
    }else {
        std::cerr<<"基本的"<<i<<"加载失败"<<std::endl;
        return -1;
    }
}

int myClassLoader::loadPrimitiveTye(const std::string& PTypeName,instanceMirrorClass* JLClass) {
    instanceClass* Tclass=new instanceClass();
    Tclass->name=PTypeName;
    Tclass->father=getInstanceClass("java/lang/Object");
    Tclass->fatherName="java/lang/Object";
    Tclass->metaFieldCount=Tclass->metaMethodCount=0;
    Tclass->interfaceCount=0;
    Tclass->accessFlag=0;
    Tclass->classLoader=this;
    Tclass->constantPool=nullptr;
    Tclass->buildSlotMap();
    loadedClass.emplace(PTypeName,Tclass);
    uint32_t CIObj=heap.NewObj_ClassInstance(
        sizeof(classInstanceOopDesc)+JLClass->instanceSlotSize*4);
    if (CIObj<=0){return -1;}
    classInstanceOopDesc* CIPtr=(classInstanceOopDesc*)heap.getRowPointer(CIObj);
    CIPtr->kclassManaged=Tclass;
    Tclass->ClassInstancePtr=CIObj;
    CIPtr->runTimeInfo=0;
    CIPtr->kclass=JLClass;
    CIPtr->instanceSlots=new Slots(
        JLClass->instanceSlotSize,((char*)CIPtr)+sizeof(classInstanceOopDesc));
    return 0;
}

int myClassLoader::loadClass_java_lang_Class() {
    std::string i="java/lang/Class";
    Kclass* j=getInstanceClass(i);
    if (j!=nullptr) {
        loadedClass.emplace(i,j);
        std::vector<std::string> primitiveType={
            "void","boolean","byte","char","short","int","long","float","double"
        };
        for (const auto& ii:primitiveType) {
            if (loadPrimitiveTye(ii,(instanceMirrorClass*)j)!=0) {
                return -1;
            }
        }
        return 0;
    }else {
        std::cerr<<"基本的"<<i<<"加载失败"<<std::endl;
        return -1;
    }
}

// int myClassLoader::loadClass_Array() {
//     Kclass* arrClass=new ArrayClass;
//     //[表示数组的名称
//     loadedClass.emplace("[",arrClass);
//     arrClass->father=loadedClass["java/lang/Object"];
//     arrClass->fatherName="java/lang/Object";
//     arrClass->name="[";
//     return 0;
// }

int myClassLoader::loadClass_java_lang_String() {
    std::string i="java/lang/String";
    Kclass* j=getInstanceClass(i);
    if (j!=nullptr) {
        loadedClass.emplace(i,j);
        return 0;
    }else {
        std::cerr<<"基本的"<<i<<"加载失败"<<std::endl;
        return -1;
    }
    //set props //to do
}

int myClassLoader::loadClass_java_lang_System() {
    std::string i="java/lang/System";
    Kclass* j=getInstanceClass(i,1);
    if (j!=nullptr) {
        loadedClass.emplace(i,j);
        return 0;
    }else {
        std::cerr<<"基本的"<<i<<"加载失败"<<std::endl;
        return -1;
    }
}

int myClassLoader::loadClass_sun_misc_VM() {
    std::string i="sun/misc/VM";
    Kclass* j=getInstanceClass(i,1);
    if (j!=nullptr) {
        loadedClass.emplace(i,j);
        return 0;
    }else {
        std::cerr<<"基本的"<<i<<"加载失败"<<std::endl;
        return -1;
    }
}

int myClassLoader::baseClassLoad() {
    if (baseClassLoaded){return 0;}
    loadClass_java_lang_Object();
    loadClass_java_lang_Class();
    loadClass_java_lang_String();
    baseClassLoaded=1;
    for (auto i:stringsBeforeClassLoaded) {
        i->stringPoolPtr=heap.getStringIstance(i->value);
    }
    stringsBeforeClassLoaded.clear();
    for (const auto& i:loadedClass) {
        if (i.second->ClassInstancePtr==0)get_java_lang_Class_instance(i.second);
    }
    //loadClass_java_lang_System();//
    //loadClass_sun_misc_VM();//

    return 0;
}