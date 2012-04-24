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
/*
 * User #includes go here
 */
// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\cpp.start.tmpl") ]]]
#include "_coretestBase.h"


C_coretestBase::C_coretestBase(CIwWMDispatcherBase* dispatcher) :
    CIwWMDispatcherModule<C_coretestBase>("_coretest", dispatcher)
{

    RegisterMember("TestInt", &C_coretestBase::TestInt);
    RegisterMember("TestUInt", &C_coretestBase::TestUInt);
    RegisterMember("TestLong", &C_coretestBase::TestLong);
    RegisterMember("TestULong", &C_coretestBase::TestULong);
    RegisterMember("TestDouble", &C_coretestBase::TestDouble);
    RegisterMember("TestBool", &C_coretestBase::TestBool);
    RegisterMember("TestString", &C_coretestBase::TestString);
    RegisterMember("TestArray", &C_coretestBase::TestArray);
    RegisterMember("TestObject", &C_coretestBase::TestObject);
    RegisterMember("TestPhatObject", &C_coretestBase::TestPhatObject);
    RegisterMember("TestPhatArray", &C_coretestBase::TestPhatArray);
    RegisterMember("TestPhatArray1", &C_coretestBase::TestPhatArray1);
    RegisterMember("TestPhatArray2", &C_coretestBase::TestPhatArray2);
    RegisterMember("TestClass", &C_coretestBase::TestClass);
    RegisterMember("TestPhatClass", &C_coretestBase::TestPhatClass);
    RegisterMember("TestChildClass", &C_coretestBase::TestChildClass);
    RegisterMember("TestEnum", &C_coretestBase::TestEnum);
    RegisterMember("TestDict", &C_coretestBase::TestDict);
}

// [[[end]]]

/*
 * Auto Generated section for converting between JavaScript and C++
 */
// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\cpp.bridge.tmpl") ]]]

//JS->C++ automated converters
bool C_coretestBase::TestInt(int callbackID, const Json::Value& parameters)
{
    Json::Value testInt_JSON;
    FetchObject(parameters, "testInt", testInt_JSON);

    int32 testInt;
    FetchInt(testInt_JSON, testInt);



    TestInt(callbackID, testInt);
    return true;
}

bool C_coretestBase::TestUInt(int callbackID, const Json::Value& parameters)
{
    Json::Value testUInt_JSON;
    FetchObject(parameters, "testUInt", testUInt_JSON);

    uint32 testUInt;
    FetchUInt(testUInt_JSON, testUInt);



    TestUInt(callbackID, testUInt);
    return true;
}

bool C_coretestBase::TestLong(int callbackID, const Json::Value& parameters)
{
    Json::Value testLong_JSON;
    FetchObject(parameters, "testLong", testLong_JSON);

    int64 testLong;
    FetchLong(testLong_JSON, testLong);



    TestLong(callbackID, testLong);
    return true;
}

bool C_coretestBase::TestULong(int callbackID, const Json::Value& parameters)
{
    Json::Value testULong_JSON;
    FetchObject(parameters, "testULong", testULong_JSON);

    uint64 testULong;
    FetchULong(testULong_JSON, testULong);



    TestULong(callbackID, testULong);
    return true;
}

bool C_coretestBase::TestDouble(int callbackID, const Json::Value& parameters)
{
    Json::Value testDouble_JSON;
    FetchObject(parameters, "testDouble", testDouble_JSON);

    double testDouble;
    FetchDouble(testDouble_JSON, testDouble);



    TestDouble(callbackID, testDouble);
    return true;
}

bool C_coretestBase::TestBool(int callbackID, const Json::Value& parameters)
{
    Json::Value testBool_JSON;
    FetchObject(parameters, "testBool", testBool_JSON);

    bool testBool;
    FetchBool(testBool_JSON, testBool);



    TestBool(callbackID, testBool);
    return true;
}

bool C_coretestBase::TestString(int callbackID, const Json::Value& parameters)
{
    Json::Value testString_JSON;
    FetchObject(parameters, "testString", testString_JSON);

    std::string testString;
    FetchString(testString_JSON, testString);



    TestString(callbackID, testString.c_str());
    return true;
}

bool C_coretestBase::TestArray(int callbackID, const Json::Value& parameters)
{
    Json::Value testArray_JSON;
    FetchObject(parameters, "testArray", testArray_JSON);

    std::vector<int32 > testArray;
    for (int param_i=0; param_i<(int)testArray_JSON.size(); param_i++)
    {
    Json::Value param_array;
    FetchArray(testArray_JSON, param_i, param_array);

    int32 param;
    FetchInt(param_array, param);


        testArray.push_back(param);
    }



    TestArray(callbackID, testArray);
    return true;
}

bool C_coretestBase::TestObject(int callbackID, const Json::Value& parameters)
{
    Json::Value testObject_JSON;
    FetchObject(parameters, "testObject", testObject_JSON);

    Json::Value param_JSON;
    FetchObject(testObject_JSON, "param", param_JSON);

    int32 param;
    FetchInt(param_JSON, param);




    TestObject(callbackID, param);
    return true;
}

bool C_coretestBase::TestPhatObject(int callbackID, const Json::Value& parameters)
{
    Json::Value testObject_JSON;
    FetchObject(parameters, "testObject", testObject_JSON);

    Json::Value array_JSON;
    FetchObject(testObject_JSON, "array", array_JSON);

    std::vector<int32 > array;
    for (int param1_i=0; param1_i<(int)array_JSON.size(); param1_i++)
    {
    Json::Value param1_array;
    FetchArray(array_JSON, param1_i, param1_array);

    int32 param1;
    FetchInt(param1_array, param1);


        array.push_back(param1);
    }


    Json::Value object_JSON;
    FetchObject(testObject_JSON, "object", object_JSON);

    Json::Value param2_JSON;
    FetchObject(object_JSON, "param2", param2_JSON);

    int32 param2;
    FetchInt(param2_JSON, param2);



    Json::Value klass_JSON;
    FetchObject(testObject_JSON, "klass", klass_JSON);

    ClassTest klass=ClassTest::From_JSON(klass_JSON, this);

    Json::Value string_JSON;
    FetchObject(testObject_JSON, "string", string_JSON);

    std::string string;
    FetchString(string_JSON, string);




    TestPhatObject(callbackID, array, param2, klass, string.c_str());
    return true;
}

bool C_coretestBase::TestPhatArray(int callbackID, const Json::Value& parameters)
{
    Json::Value testArray_JSON;
    FetchObject(parameters, "testArray", testArray_JSON);

    std::vector<std::vector<int32 > > testArray;
    for (int array_i=0; array_i<(int)testArray_JSON.size(); array_i++)
    {
    Json::Value array_array;
    FetchArray(testArray_JSON, array_i, array_array);

    std::vector<int32 > array;
    for (int param1_i=0; param1_i<(int)array_array.size(); param1_i++)
    {
    Json::Value param1_array;
    FetchArray(array_array, param1_i, param1_array);

    int32 param1;
    FetchInt(param1_array, param1);


        array.push_back(param1);
    }


        testArray.push_back(array);
    }



    TestPhatArray(callbackID, testArray);
    return true;
}

bool C_coretestBase::TestPhatArray1(int callbackID, const Json::Value& parameters)
{
    Json::Value testArray_JSON;
    FetchObject(parameters, "testArray", testArray_JSON);

    std::vector<std::string > testArray;
    for (int string_i=0; string_i<(int)testArray_JSON.size(); string_i++)
    {
    Json::Value string_array;
    FetchArray(testArray_JSON, string_i, string_array);

    std::string string;
    FetchString(string_array, string);


        testArray.push_back(string);
    }



    TestPhatArray1(callbackID, testArray);
    return true;
}

bool C_coretestBase::TestPhatArray2(int callbackID, const Json::Value& parameters)
{
    Json::Value testArray_JSON;
    FetchObject(parameters, "testArray", testArray_JSON);

    std::vector<ClassTest > testArray;
    for (int klass_i=0; klass_i<(int)testArray_JSON.size(); klass_i++)
    {
    Json::Value klass_array;
    FetchArray(testArray_JSON, klass_i, klass_array);

    ClassTest klass=ClassTest::From_JSON(klass_array, this);

        testArray.push_back(klass);
    }



    TestPhatArray2(callbackID, testArray);
    return true;
}

bool C_coretestBase::TestClass(int callbackID, const Json::Value& parameters)
{
    Json::Value testClass_JSON;
    FetchObject(parameters, "testClass", testClass_JSON);

    ClassTest testClass=ClassTest::From_JSON(testClass_JSON, this);


    TestClass(callbackID, testClass);
    return true;
}

bool C_coretestBase::TestPhatClass(int callbackID, const Json::Value& parameters)
{
    Json::Value testClass_JSON;
    FetchObject(parameters, "testClass", testClass_JSON);

    PhatClassTest testClass=PhatClassTest::From_JSON(testClass_JSON, this);


    TestPhatClass(callbackID, testClass);
    return true;
}

bool C_coretestBase::TestChildClass(int callbackID, const Json::Value& parameters)
{
    Json::Value testChildClass_JSON;
    FetchObject(parameters, "testChildClass", testChildClass_JSON);

    ChildClassTest testChildClass=ChildClassTest::From_JSON(testChildClass_JSON, this);


    TestChildClass(callbackID, testChildClass);
    return true;
}

bool C_coretestBase::TestEnum(int callbackID, const Json::Value& parameters)
{
    Json::Value testEnum_JSON;
    FetchObject(parameters, "testEnum", testEnum_JSON);

    EnumTest testEnum;
    FetchUInt(testEnum_JSON, *((unsigned int*)&testEnum));



    TestEnum(callbackID, testEnum);
    return true;
}

bool C_coretestBase::TestDict(int callbackID, const Json::Value& parameters)
{
    Json::Value testDict_JSON;
    FetchObject(parameters, "testDict", testDict_JSON);

    std::map<std::string, std::string> testDict;
    FetchDictionary(testDict_JSON, testDict);



    TestDict(callbackID, testDict);
    return true;
}

C_coretestBase::ClassTest C_coretestBase::ClassTest::From_JSON(const Json::Value& value, C_coretestBase* reader)
{
    Json::Value field_JSON;
    reader->FetchObject(value, "field", field_JSON);

    int32 field;
    reader->FetchInt(field_JSON, field);


    return C_coretestBase::ClassTest(field);
}

C_coretestBase::PhatClassTest C_coretestBase::PhatClassTest::From_JSON(const Json::Value& value, C_coretestBase* reader)
{
    Json::Value array_JSON;
    reader->FetchObject(value, "array", array_JSON);

    std::vector<int32 > array;
    for (int param1_i=0; param1_i<(int)array_JSON.size(); param1_i++)
    {
    Json::Value param1_array;
    reader->FetchArray(array_JSON, param1_i, param1_array);

    int32 param1;
    reader->FetchInt(param1_array, param1);


        array.push_back(param1);
    }


    Json::Value object_JSON;
    reader->FetchObject(value, "object", object_JSON);

    Json::Value param2_JSON;
    reader->FetchObject(object_JSON, "param2", param2_JSON);

    int32 param2;
    reader->FetchInt(param2_JSON, param2);



    Json::Value klass_JSON;
    reader->FetchObject(value, "klass", klass_JSON);

    ClassTest klass=ClassTest::From_JSON(klass_JSON, reader);

    Json::Value string_JSON;
    reader->FetchObject(value, "string", string_JSON);

    std::string string;
    reader->FetchString(string_JSON, string);


    Json::Value enumy_JSON;
    reader->FetchObject(value, "enumy", enumy_JSON);

    PhatClassTest::EnumChildTest enumy;
    reader->FetchUInt(enumy_JSON, *((unsigned int*)&enumy));


    return C_coretestBase::PhatClassTest(array, param2, klass, string.c_str(), enumy);
}

C_coretestBase::ParentClassTest C_coretestBase::ParentClassTest::From_JSON(const Json::Value& value, C_coretestBase* reader)
{
    Json::Value string_JSON;
    reader->FetchObject(value, "string", string_JSON);

    std::string string;
    reader->FetchString(string_JSON, string);


    return C_coretestBase::ParentClassTest(string.c_str());
}

C_coretestBase::ChildClassTest C_coretestBase::ChildClassTest::From_JSON(const Json::Value& value, C_coretestBase* reader)
{
    Json::Value string_JSON;
    reader->FetchObject(value, "string", string_JSON);

    std::string string;
    reader->FetchString(string_JSON, string);


    Json::Value string2_JSON;
    reader->FetchObject(value, "string2", string2_JSON);

    std::string string2;
    reader->FetchString(string2_JSON, string2);


    return C_coretestBase::ChildClassTest(string.c_str(), string2.c_str());
}

//C++->JS automated event setup

//C++->JS automated callbacks
bool C_coretestBase::TestIntCB(int callbackID, int32 testInt)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    params["testInt"]=Json::Value(testInt);


    SendJavaScript("callbackSuccess", callbackID, params);
    return true;
}

bool C_coretestBase::TestUIntCB(int callbackID, uint32 testUInt)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    params["testUInt"]=Json::Value(testUInt);


    SendJavaScript("callbackSuccess", callbackID, params);
    return true;
}

bool C_coretestBase::TestLongCB(int callbackID, int64 testLong)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    params["testLong"]=Json::Value(testLong);


    SendJavaScript("callbackSuccess", callbackID, params);
    return true;
}

bool C_coretestBase::TestULongCB(int callbackID, uint64 testULong)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    params["testULong"]=Json::Value(testULong);


    SendJavaScript("callbackSuccess", callbackID, params);
    return true;
}

bool C_coretestBase::TestDoubleCB(int callbackID, double testDouble)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    params["testDouble"]=Json::Value(testDouble);


    SendJavaScript("callbackSuccess", callbackID, params);
    return true;
}

bool C_coretestBase::TestBoolCB(int callbackID, bool testBool)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    params["testBool"]=Json::Value(testBool);


    SendJavaScript("callbackSuccess", callbackID, params);
    return true;
}

bool C_coretestBase::TestStringCB(int callbackID, const char* testString)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    params["testString"]=Json::Value(testString);


    SendJavaScript("callbackSuccess", callbackID, params);
    return true;
}

bool C_coretestBase::TestArrayCB(int callbackID, const std::vector<int32 > testArray)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    Json::Value testArray_JSON(Json::arrayValue);
    for (int param_i=0; param_i<(int)testArray.size(); param_i++)
    {
    testArray_JSON[param_i]=Json::Value(testArray[param_i]);

    }
    params["testArray"]=testArray_JSON;


    SendJavaScript("callbackSuccess", callbackID, params);
    return true;
}

bool C_coretestBase::TestObjectCB(int callbackID, int32 param)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    Json::Value testObject_JSON(Json::objectValue);
    testObject_JSON["param"]=Json::Value(param);

    params["testObject"]=testObject_JSON;


    SendJavaScript("callbackSuccess", callbackID, params);
    return true;
}

bool C_coretestBase::TestPhatObjectCB(int callbackID, const std::vector<int32 > array, int32 param2, const ClassTest& klass, const char* string)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    Json::Value testObject_JSON(Json::objectValue);
    Json::Value array_JSON(Json::arrayValue);
    for (int param1_i=0; param1_i<(int)array.size(); param1_i++)
    {
    array_JSON[param1_i]=Json::Value(array[param1_i]);

    }
    testObject_JSON["array"]=array_JSON;

    Json::Value object_JSON(Json::objectValue);
    object_JSON["param2"]=Json::Value(param2);

    testObject_JSON["object"]=object_JSON;

    Json::Value klass_JSON(Json::objectValue);
    klass.To_JSON(klass_JSON);
    testObject_JSON["klass"]=klass_JSON;

    testObject_JSON["string"]=Json::Value(string);

    params["testObject"]=testObject_JSON;


    SendJavaScript("callbackSuccess", callbackID, params);
    return true;
}

bool C_coretestBase::TestPhatArrayCB(int callbackID, const std::vector<std::vector<int32 > > testArray)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    Json::Value testArray_JSON(Json::arrayValue);
    for (int array_i=0; array_i<(int)testArray.size(); array_i++)
    {
    Json::Value array_JSON(Json::arrayValue);
    for (int param1_i=0; param1_i<(int)testArray[array_i].size(); param1_i++)
    {
    array_JSON[param1_i]=Json::Value(testArray[array_i][param1_i]);

    }
    testArray_JSON[array_i]=array_JSON;

    }
    params["testArray"]=testArray_JSON;


    SendJavaScript("callbackSuccess", callbackID, params);
    return true;
}

bool C_coretestBase::TestPhatArray1CB(int callbackID, const std::vector<std::string > testArray)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    Json::Value testArray_JSON(Json::arrayValue);
    for (int string_i=0; string_i<(int)testArray.size(); string_i++)
    {
    testArray_JSON[string_i]=Json::Value(testArray[string_i]);

    }
    params["testArray"]=testArray_JSON;


    SendJavaScript("callbackSuccess", callbackID, params);
    return true;
}

bool C_coretestBase::TestPhatArray2CB(int callbackID, const std::vector<ClassTest > testArray)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    Json::Value testArray_JSON(Json::arrayValue);
    for (int klass_i=0; klass_i<(int)testArray.size(); klass_i++)
    {
    Json::Value klass_JSON(Json::objectValue);
    testArray[klass_i].To_JSON(klass_JSON);
    testArray_JSON[klass_i]=klass_JSON;

    }
    params["testArray"]=testArray_JSON;


    SendJavaScript("callbackSuccess", callbackID, params);
    return true;
}

bool C_coretestBase::TestClassCB(int callbackID, const ClassTest& testClass)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    Json::Value testClass_JSON(Json::objectValue);
    testClass.To_JSON(testClass_JSON);
    params["testClass"]=testClass_JSON;


    SendJavaScript("callbackSuccess", callbackID, params);
    return true;
}

bool C_coretestBase::TestPhatClassCB(int callbackID, const PhatClassTest& testClass)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    Json::Value testClass_JSON(Json::objectValue);
    testClass.To_JSON(testClass_JSON);
    params["testClass"]=testClass_JSON;


    SendJavaScript("callbackSuccess", callbackID, params);
    return true;
}

bool C_coretestBase::TestChildClassCB(int callbackID, const ChildClassTest& testChildClass)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    Json::Value testChildClass_JSON(Json::objectValue);
    testChildClass.To_JSON(testChildClass_JSON);
    params["testChildClass"]=testChildClass_JSON;


    SendJavaScript("callbackSuccess", callbackID, params);
    return true;
}

bool C_coretestBase::TestEnumCB(int callbackID, EnumTest testEnum)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    params["testEnum"]=Json::Value(testEnum);


    SendJavaScript("callbackSuccess", callbackID, params);
    return true;
}

bool C_coretestBase::TestDictCB(int callbackID, const std::map<std::string, std::string>& testDict)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    Json::Value testDict_JSON(Json::objectValue);

    for (std::map<std::string, std::string>::const_iterator it = testDict.begin(); it!=testDict.end(); ++it)
        testDict_JSON[it->first] = it->second;
    params["testDict"]=testDict_JSON;


    SendJavaScript("callbackSuccess", callbackID, params);
    return true;
}

void C_coretestBase::ClassTest::To_JSON(Json::Value& value) const
{
    value["field"]=Json::Value(field);

}

void C_coretestBase::PhatClassTest::To_JSON(Json::Value& value) const
{
    Json::Value array_JSON(Json::arrayValue);
    for (int param1_i=0; param1_i<(int)array.size(); param1_i++)
    {
    array_JSON[param1_i]=Json::Value(array[param1_i]);

    }
    value["array"]=array_JSON;

    Json::Value object_JSON(Json::objectValue);
    object_JSON["param2"]=Json::Value(param2);

    value["object"]=object_JSON;

    Json::Value klass_JSON(Json::objectValue);
    klass.To_JSON(klass_JSON);
    value["klass"]=klass_JSON;

    value["string"]=Json::Value(string);

    value["enumy"]=Json::Value(enumy);

}

void C_coretestBase::ParentClassTest::To_JSON(Json::Value& value) const
{
    value["string"]=Json::Value(string);

}

void C_coretestBase::ChildClassTest::To_JSON(Json::Value& value) const
{
    ParentClassTest::To_JSON(value);
    value["string2"]=Json::Value(string2);

}

//C++->JS automated events

// [[[end]]]
