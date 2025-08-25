//
// Created by YZQ on 25-7-8.
//
#include "MetaConstantPool.h"

MetaConstantPool::MetaConstantPool(const ClassFile *class_file) {
        count=class_file->constantPool->constant_pool_count;
        pool.resize(count,nullptr);
        for (int i = 0; i < count; ++i) {
            if (class_file->constantPool->pool[i]!=nullptr) {
                const auto& pi=*(class_file->constantPool->pool[i]);
                switch (pi.tag) {
                    case CONSTANT_Utf8: {
                        pool[i]=nullptr;
                        break;
                    }
                    case CONSTANT_Integer: {
                        pool[i]=new MCP_Integer(pi,*(class_file->constantPool));
                        break;
                    }
                    case CONSTANT_Float: {
                        pool[i]=new MCP_Float(pi,*(class_file->constantPool));
                        break;
                    }
                    case CONSTANT_Long: {
                        pool[i]=new MCP_Long(pi,*(class_file->constantPool));
                        ++i;
                        break;
                    }
                    case CONSTANT_Double: {
                        pool[i]=new MCP_Double(pi,*(class_file->constantPool));
                        ++i;
                        break;
                    }
                    case CONSTANT_Class: {
                        pool[i]=new MCP_Class(pi,*(class_file->constantPool));
                        break;
                    }
                    case CONSTANT_String: {
                        pool[i]=new MCP_String(pi,*(class_file->constantPool));
                        break;
                    }
                    case CONSTANT_Fieldref:
                    case CONSTANT_Methodref:
                    case CONSTANT_InterfaceMethodref: {
                        pool[i]=new MCP_Fieldref(pi,*(class_file->constantPool));
                        break;
                    }
                    case CONSTANT_NameAndType: {
                        pool[i]=nullptr;
                        break;
                    }
                    case CONSTANT_MethodHandle: {
                        pool[i]=nullptr;
                        break;
                    }
                    case CONSTANT_MethodType: {
                        pool[i]=nullptr;
                        break;
                    }
                    case CONSTANT_InvokeDynamic: {
                        pool[i]=nullptr;
                        break;
                    }
                    default:;
                }
            }
        }
    }