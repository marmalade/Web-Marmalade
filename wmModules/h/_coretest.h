/*
 * Copyright (C) 2001-2012 Ideaworks3D Ltd.
 * All Rights Reserved.
 *
 * This document is protected by copyright, and contains information
 * proprietary to Ideaworks Labs.
 * This file consists of source code released by Ideaworks Labs under
 * the terms of the accompanying End User License Agreement (EULA).
 * Please do not use this program/source code before you have read the
 * EULA and have agreed to be bound by its terms.
 */

#include "_coretestBase.h"

class C_coretest : public C_coretestBase
{
public:
    C_coretest(CIwWMDispatcherBase* dispatcher) : C_coretestBase(dispatcher) {}

public:
    
    virtual void Init();
    virtual void OnReady();
    virtual void Shutdown();

    virtual void TestInt(int callbackID, int32 testInt);
    virtual void TestUInt(int callbackID, uint32 testUInt);
    virtual void TestLong(int callbackID, int64 testLong);
    virtual void TestULong(int callbackID, uint64 testULong);
    virtual void TestDouble(int callbackID, double testDouble);
    virtual void TestBool(int callbackID, bool testBool);
    virtual void TestString(int callbackID, const char* testString);
    virtual void TestArray(int callbackID, const std::vector<int32 > testArray);
    virtual void TestObject(int callbackID, int32 param);
    virtual void TestPhatObject(int callbackID, const std::vector<int32 > array, int32 param2, const ClassTest& klass, const char* string);
    virtual void TestPhatArray(int callbackID, const std::vector<std::vector<int32 > > testArray);
    virtual void TestPhatArray1(int callbackID, const std::vector<std::string > testArray);
    virtual void TestPhatArray2(int callbackID, const std::vector<ClassTest > testArray);
    virtual void TestClass(int callbackID, const ClassTest& testClass);
    virtual void TestPhatClass(int callbackID, const PhatClassTest& testClass);
    virtual void TestChildClass(int callbackID, const ChildClassTest& testChildClass);
    virtual void TestEnum(int callbackID, EnumTest testEnum);
    virtual void TestDict(int callbackID, const std::map<std::string, std::string>& testDict);
};
