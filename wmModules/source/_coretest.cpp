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

#include "IwDebug.h"
#include "_coretest.h"

void C_coretest::Init()
{
}

void C_coretest::OnReady()
{
}

void C_coretest::Shutdown()
{
}

void C_coretest::TestInt(int callbackID, int32 testInt)
{
    TestIntCB(callbackID, testInt);
}

void C_coretest::TestUInt(int callbackID, uint32 testUInt)
{
    TestUIntCB(callbackID, testUInt);
}

void C_coretest::TestLong(int callbackID, int64 testLong)
{
    TestLongCB(callbackID, testLong);
}

void C_coretest::TestULong(int callbackID, uint64 testULong)
{
    TestULongCB(callbackID, testULong);
}

void C_coretest::TestDouble(int callbackID, double testDouble)
{
    TestDoubleCB(callbackID, testDouble);
}

void C_coretest::TestBool(int callbackID, bool testBool)
{
    TestBoolCB(callbackID, testBool);
}

void C_coretest::TestString(int callbackID, const char* testString)
{
    TestStringCB(callbackID, testString);
}

void C_coretest::TestArray(int callbackID, const std::vector<int32 > testArray)
{
    TestArrayCB(callbackID, testArray);
}

void C_coretest::TestObject(int callbackID, int32 param)
{
    TestObjectCB(callbackID, param);
}

void C_coretest::TestPhatObject(int callbackID, const std::vector<int32 > array, int32 param2, const ClassTest& klass, const char* string)
{
    TestPhatObjectCB(callbackID, array, param2, klass, string);
}

void C_coretest::TestPhatArray(int callbackID, const std::vector<std::vector<int32 > > testArray)
{
    TestPhatArrayCB(callbackID, testArray);
}

void C_coretest::TestPhatArray1(int callbackID, const std::vector<std::string > testArray)
{
    TestPhatArray1CB(callbackID, testArray);
}

void C_coretest::TestPhatArray2(int callbackID, const std::vector<ClassTest > testArray)
{
    TestPhatArray2CB(callbackID, testArray);
}

void C_coretest::TestClass(int callbackID, const ClassTest& testClass)
{
    TestClassCB(callbackID, testClass);
}

void C_coretest::TestPhatClass(int callbackID, const PhatClassTest& testClass)
{
    TestPhatClassCB(callbackID, testClass);
}

void C_coretest::TestEnum(int callbackID, EnumTest testEnum)
{
    TestEnumCB(callbackID, testEnum);
}

void C_coretest::TestChildClass(int callbackID, const ChildClassTest& testChildClass)
{
    TestChildClassCB(callbackID, testChildClass);
}

void C_coretest::TestDict(int callbackID, const std::map<std::string, std::string>& testDict)
{
    TestDictCB(callbackID, testDict);
}
