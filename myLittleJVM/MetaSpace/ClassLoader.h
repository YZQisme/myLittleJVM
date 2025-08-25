//
// Created by YZQ on 25-8-5.
//

#ifndef CLASSLOADER_H
#define CLASSLOADER_H
#include "Kclass.h"
#include "MetaConstantPool.h"
struct myClassLoader {//简易的类加载，代替复杂的类加载器
    //加载路径
    std::string standardLibPath="../../javaClassFile/";
    std::string standardExtendLibPath;
    std::string userLibPath="../../userClassFile/";

    //一些基本的类，在启动时加载到MetaSpace
    //加载一些基础的类
    int loadPrimitiveTye(const std::string& PTypeName,instanceMirrorClass* JLClass);
    int get_java_lang_Class_instance(Kclass* KclassManagebyClass);
    int loadClass_java_lang_Object();
    int loadClass_java_lang_Class();
    int loadClass_java_lang_String();
    //int loadClass_Array();
    int loadClass_sun_misc_VM();
    int loadClass_java_lang_System();
    int baseClassLoad();

    //记录哪些类被加载了（全限名）
    std::unordered_map<std::string,Kclass*> loadedClass;

    void unloadAllClass(){//虚拟机关闭时调用
        for (auto& i:loadedClass) {
            delete i.second;
        }
        loadedClass.clear();
    }
    ~myClassLoader(){unloadAllClass();}

    //类加载器如果发现加载不成功（内存不够、文件不存在），报告错误
    ClassFile* fileLoad(const std::string& classByteFileName) const;
    //在MetaSpace里建立普通类的Kclass
    //把一个类字节码的ClassFile形式的数据载入元空间，前提是 class_file的类的所有父类已经被加载完成
    //按照“标准库、标准拓展、用户自定义”的优先级加载
    Kclass* buildInstanceClass_instanceClass(const std::string& classByteFileName,int runClinit=1);

    Kclass* buildInstanceClass_ArrayClass(const std::string& classByteFileName);

    Kclass* getInstanceClass(const std::string& className,int runClinit=1) {
        auto f=loadedClass.find(className);
        if (f==loadedClass.end()) {
            if (className[0]=='[') {
                return buildInstanceClass_ArrayClass(className);
            }else {
                return buildInstanceClass_instanceClass(className,runClinit);
            }
        }else {
            return (*f).second;
        }
    }
};
extern myClassLoader my_class_loader;
#endif //CLASSLOADER_H
