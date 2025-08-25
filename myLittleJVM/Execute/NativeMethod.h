//
// Created by YZQ on 25-8-5.
//

#ifndef NATIVEMETHOD_H
#define NATIVEMETHOD_H
#include <map>

#include "Executor.h"

extern std::map<std::string,int (*)(Execute*,uint64_t*)>nativeMethodNameToFuncPtr;

int java_lang_Class_registerNatives_V(Execute* execute,uint64_t* ret);

int java_lang_System_registerNatives_V(Execute* execute,uint64_t* ret);

int java_lang_Object_registerNatives_V(Execute* execute,uint64_t* ret);

//java/lang/Class.desiredAssertionStatus0:(Ljava/lang/Class;)Z
int java_lang_Class_desiredAssertionStatus0_LC_V(Execute* execute,uint64_t* ret);

//java/lang/System.arraycopy:(Ljava/lang/Object;ILjava/lang/Object;II)V
int java_lang_System_arraycopy_LOILOII_V(Execute* execute,uint64_t* ret);

//java/lang/Class.getPrimitiveClass:(Ljava/lang/String;)Ljava/lang/Class;
int java_lang_Class_getPrimitiveClass_LS_LC(Execute* execute,uint64_t* ret);

//java/lang/Float.floatToRawIntBits:(F)I
int java_lang_Float_floatToRawIntBits_F_I(Execute* execute,uint64_t* ret);

//java/lang/Double.doubleToRawLongBits:(D)J
int java_lang_Double_doubleToRawLongBits_D_J(Execute* execute,uint64_t* ret);

//java/lang/Double.longBitsToDouble:(J)D
int java_lang_Double_longBitsToDouble_J_D(Execute* execute,uint64_t* ret);

//sun/misc/VM.initialize:()V
int sun_misc_VM_initialize_V(Execute* execute,uint64_t* ret);

//java/lang/System.initProperties:(Ljava/util/Properties;)Ljava/util/Properties;
int java_lang_System_initProperties_LP_LP(Execute *execute, uint64_t *ret);

//java/io/FileInputStream.initIDs:()V
int java_io_FileInputStream_initIDs_V(Execute *execute, uint64_t *ret);

//java/io/FileDescriptor.initIDs:()V
int java_io_FileDescriptor_initIDs_V(Execute *execute, uint64_t *ret);

//sun/misc/Unsafe.registerNatives:()V
int sun_misc_Unsafe_registerNatives_V(Execute *execute, uint64_t *ret);

//java/lang/Object.hashCode:()I
int java_lang_Object_hashCode_I(Execute *execute, uint64_t *ret);

//sun/misc/Unsafe.arrayBaseOffset:(Ljava/lang/Class;)I
int sun_misc_Unsafe_arrayBaseOffset_LC_I(Execute *execute, uint64_t *ret);

//sun/misc/Unsafe.arrayIndexScale:(Ljava/lang/Class;)I
int sun_misc_Unsafe_arrayIndexScale_LC_I(Execute *execute, uint64_t *ret);

//sun/misc/Unsafe.addressSize:()I
int sun_misc_Unsafe_addressSize_I(Execute *execute, uint64_t *ret);

//sun/reflect/Reflection.getCallerClass:()Ljava/lang/Class
int sun_reflect_Reflection_getCallerClass_LC(Execute *execute, uint64_t *ret);

//java/io/FileDescriptor.set:(I)J
int java_io_FileDescriptor_set_I_J(Execute *execute, uint64_t *ret);

//java/io/FileOutputStream.initIDs:()V
int java_io_FileOutputStream_initIDs_V(Execute *execute, uint64_t *ret);

//java/security/AccessController.doPrivileged:(Ljava/security/PrivilegedExceptionAction;)Ljava/lang/Object;
int java_security_AccessController_doPrivileged_LP_LO(Execute *execute, uint64_t *ret);
//java/security/AccessController.doPrivileged:(Ljava/security/PrivilegedAction;)Ljava/lang/Object;

//java/lang/Thread.registerNatives:()V
int java_lang_Thread_registerNatives_V(Execute *execute, uint64_t *ret);

//java/lang/Thread.currentThread:()Ljava/lang/Thread;
int java_lang_Thread_currentThread_LT(Execute *execute, uint64_t *ret);

//java/lang/Class.getName0:()Ljava/lang/String;
int java_lang_Class_getName0_LS(Execute *execute, uint64_t *ret);

//java/lang/Class.forName0:(Ljava/lang/String;ZLjava/lang/ClassLoader;Ljava/lang/Class;)Ljava/lang/Class;
int java_lang_Class_forName0_LSZLCLC_LC(Execute *execute, uint64_t *ret);

//java/security/AccessController.getStackAccessControlContext:()Ljava/security/AccessControlContext;
int java_security_AccessController_getStackAccessControlContext_LA(Execute *execute, uint64_t *ret);

//java/lang/Thread.setPriority0:(I)V
int java_lang_Thread_setPriority0_I_V(Execute *execute, uint64_t *ret);

//java/lang/Thread.isAlive:()Z
int java_lang_Thread_isAlive_Z(Execute *execute, uint64_t *ret);

//java/lang/Thread.start0:()V
int java_lang_Thread_start0_V(Execute *execute, uint64_t *ret);

//java/lang/Class.getDeclaredFields0:(Z)[Ljava/lang/reflect/Field;
int java_lang_Class_getDeclaredFields0_Z_ALF(Execute *execute, uint64_t *ret);

//java/lang/ClassLoader.registerNatives:()V
int java_lang_ClassLoader_registerNatives_V(Execute *execute, uint64_t *ret);

//sun/misc/Unsafe.compareAndSwapObject:(Ljava/lang/Object;JLjava/lang/Object;Ljava/lang/Object;)Z
int sun_misc_Unsafe_compareAndSwapObject_LOJLOLO_Z(Execute *execute, uint64_t *ret);

//java/lang/Object.getClass:()Ljava/lang/Class;
int java_lang_Object_getClass_LC(Execute *execute, uint64_t *ret);

//myIO.print:(Ljava/lang/String;)V
int myIO_print_LS_V(Execute *execute, uint64_t *ret);

//myIO.println:(Ljava/lang/String;)V
int myIO_println_LS_V(Execute *execute, uint64_t *ret);

//myIO.print:(I)V
int myIO_print_I_V(Execute *execute, uint64_t *ret);

//myIO.println:(I)V
int myIO_println_I_V(Execute *execute, uint64_t *ret);

//myIO.print:(D)V
int myIO_print_D_V(Execute *execute, uint64_t *ret);

//myIO.println:(D)V
int myIO_println_D_V(Execute *execute, uint64_t *ret);

//myIO.print:(F)V
int myIO_print_F_V(Execute *execute, uint64_t *ret);

//myIO.println:(F)V
int myIO_println_F_V(Execute *execute, uint64_t *ret);

//myIO.print:(J)V
int myIO_print_J_V(Execute *execute, uint64_t *ret);

//myIO.println:(J)V
int myIO_println_J_V(Execute *execute, uint64_t *ret);

//myIO.print:(C)V
int myIO_print_C_V(Execute *execute, uint64_t *ret);

//myIO.println:(C)V
int myIO_println_C_V(Execute *execute, uint64_t *ret);

//myIO.print:(Z)V
int myIO_print_Z_V(Execute *execute, uint64_t *ret);

//myIO.println:(Z)V
int myIO_println_Z_V(Execute *execute, uint64_t *ret);

//myIO.readLine:()Ljava/lang/String;
int myIO_readLine_LS(Execute *execute, uint64_t *ret);

//myIO.readString:()Ljava/lang/String;
int myIO_readString_LS(Execute *execute, uint64_t *ret);

//myIO.readInt:()I
int myIO_readInt_I(Execute *execute, uint64_t *ret);

//myIO.readLong:()J
int myIO_readLong_J(Execute *execute, uint64_t *ret);

//myIO.readChar:()C
int myIO_readChar_C(Execute *execute, uint64_t *ret);

//myIO.readDouble:()D
int myIO_readDouble_D(Execute *execute, uint64_t *ret);

//myIO.readFloat:()F
int myIO_readFloat_F(Execute *execute, uint64_t *ret);

#endif //NATIVEMETHOD_H
