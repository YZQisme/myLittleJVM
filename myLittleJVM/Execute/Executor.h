//
// Created by YZQ on 25-7-9.
//

#ifndef EXECUTOR_H
#define EXECUTOR_H
#define RUN_CONTINUE 0
#define RUN_ERROR (-1)
#define RUN_NATIVE 2
#define RUN_UNABLE (-2)
#define RUN_ABLE 1
#define RUN_INSTRUCTION_UNDO (-123456);
#define RUN_FINISH 3
#include "../Stack/Stack.h"
struct Execute {
    // Kclass* Class;
    // OopDesc* instance;
    // MetaMethod* method;
    StackFrame* stack_frame=nullptr;
    const char* code=nullptr;
    Stack stack;
    int runState=RUN_UNABLE;
    uint64_t lastResult=0;//栈最后一个方法运行完成后，返回值会被return指令放在这里
    int initialStack(const std::string &className, const std::string &MethodName,const std::string& describe);
    int NativeStackPop(uint64_t* ret);
    void run();
};

#endif //EXECUTOR_H