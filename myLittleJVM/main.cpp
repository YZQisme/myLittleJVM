

#include <codecvt>
#include <locale>

#include "Execute/Executor.h"
#include "Heap/heap.h"
#include "MetaSpace/ClassLoader.h"

#include "MetaSpace/Kclass.h"

int main() {
    heap.initial();
    Execute execute;
    my_class_loader.standardLibPath="../../javaClassFile/";
    my_class_loader.standardExtendLibPath="";
    my_class_loader.userLibPath="../../userClassFile/";
    my_class_loader.baseClassLoad();
    std::cout<<"输入主类名字:"<<std::flush;
    std::string mainClassName;
    std::cin>>mainClassName;
    if (!mainClassName.empty()) {
        std::cout<<"-----"<<mainClassName<<".class运行输出-----"<<std::endl;
        execute.initialStack(mainClassName,"main","([Ljava/lang/String;)V");
        execute.run();
        if (execute.runState!=RUN_FINISH){
            std::cerr<<"-----"<<mainClassName<<".class异常退出-----"<<std::endl;
        }else {
            std::cout<<"-----"<<mainClassName<<".class正常退出-----"<<std::endl;
        }
        my_class_loader.unloadAllClass();
    }else {
        std::cerr<<"输入错误"<<std::endl;
    }
    return 0;
}
