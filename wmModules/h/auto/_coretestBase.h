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
// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\h.start.tmpl") ]]]

#include "IwDispatcher.h"

/*
 * Generated class for JavaScript bridge module _coretest
 * Some sections of this module are re-generated when the interface changes the rest is generated once
 */


class C_coretestBase : public CIwWMDispatcherModule<C_coretestBase>
{
public:
    C_coretestBase(CIwWMDispatcherBase* dispatcher);

    enum EnumTest
    {
        Zero = 0,
        One = 1,
        Two = 2,
        Three = 3,
    };

    class ClassTest
    {
    public:
    public:
        int32 field;

        ClassTest(int32 _field) : field(_field) {}

        void To_JSON(Json::Value& value) const;
        static ClassTest From_JSON(const Json::Value& value, C_coretestBase* reader);
    };
    class PhatClassTest
    {
    public:
        enum EnumChildTest
        {
            Zero = 0,
            One = 1,
            Two = 2,
            Three = 3,
        };

    public:
        std::vector<int32 > array;
        int32 param2;
        ClassTest klass;
        std::string string;
        PhatClassTest::EnumChildTest enumy;

        PhatClassTest(const std::vector<int32 > _array,int32 _param2,const ClassTest& _klass,const char* _string,PhatClassTest::EnumChildTest _enumy) : array(_array), param2(_param2), klass(_klass), string(_string), enumy(_enumy) {}

        void To_JSON(Json::Value& value) const;
        static PhatClassTest From_JSON(const Json::Value& value, C_coretestBase* reader);
    };
    class ParentClassTest
    {
    public:
    public:
        std::string string;

        ParentClassTest(const char* _string) : string(_string) {}

        void To_JSON(Json::Value& value) const;
        static ParentClassTest From_JSON(const Json::Value& value, C_coretestBase* reader);
    };
    class ChildClassTest : public ParentClassTest
    {
    public:
    public:
        std::string string2;

        ChildClassTest(const char* _string,const char* _string2) : ParentClassTest(_string), string2(_string2) {}

        void To_JSON(Json::Value& value) const;
        static ChildClassTest From_JSON(const Json::Value& value, C_coretestBase* reader);
    };

// [[[end]]]

/*
 * Auto Generated section for converting between JavaScript and C++
 */
// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\h.bridge.tmpl") ]]]
private:
    //JS->C++ automated converters
    bool TestInt(int callbackID, const Json::Value& parameters);

    bool TestUInt(int callbackID, const Json::Value& parameters);

    bool TestLong(int callbackID, const Json::Value& parameters);

    bool TestULong(int callbackID, const Json::Value& parameters);

    bool TestDouble(int callbackID, const Json::Value& parameters);

    bool TestBool(int callbackID, const Json::Value& parameters);

    bool TestString(int callbackID, const Json::Value& parameters);

    bool TestArray(int callbackID, const Json::Value& parameters);

    bool TestObject(int callbackID, const Json::Value& parameters);

    bool TestPhatObject(int callbackID, const Json::Value& parameters);

    bool TestPhatArray(int callbackID, const Json::Value& parameters);

    bool TestPhatArray1(int callbackID, const Json::Value& parameters);

    bool TestPhatArray2(int callbackID, const Json::Value& parameters);

    bool TestClass(int callbackID, const Json::Value& parameters);

    bool TestPhatClass(int callbackID, const Json::Value& parameters);

    bool TestChildClass(int callbackID, const Json::Value& parameters);

    bool TestEnum(int callbackID, const Json::Value& parameters);

    bool TestDict(int callbackID, const Json::Value& parameters);

    //C++->JS automated event setup
public:
    //C++->JS automated callbacks
    bool TestIntCB(int callbackID, int32 testInt);

    bool TestUIntCB(int callbackID, uint32 testUInt);

    bool TestLongCB(int callbackID, int64 testLong);

    bool TestULongCB(int callbackID, uint64 testULong);

    bool TestDoubleCB(int callbackID, double testDouble);

    bool TestBoolCB(int callbackID, bool testBool);

    bool TestStringCB(int callbackID, const char* testString);

    bool TestArrayCB(int callbackID, const std::vector<int32 > testArray);

    bool TestObjectCB(int callbackID, int32 param);

    bool TestPhatObjectCB(int callbackID, const std::vector<int32 > array, int32 param2, const ClassTest& klass, const char* string);

    bool TestPhatArrayCB(int callbackID, const std::vector<std::vector<int32 > > testArray);

    bool TestPhatArray1CB(int callbackID, const std::vector<std::string > testArray);

    bool TestPhatArray2CB(int callbackID, const std::vector<ClassTest > testArray);

    bool TestClassCB(int callbackID, const ClassTest& testClass);

    bool TestPhatClassCB(int callbackID, const PhatClassTest& testClass);

    bool TestChildClassCB(int callbackID, const ChildClassTest& testChildClass);

    bool TestEnumCB(int callbackID, EnumTest testEnum);

    bool TestDictCB(int callbackID, const std::map<std::string, std::string>& testDict);

    //C++->JS automated events

// [[[end]]]

/*
 * Auto Generated section for defining modules functions
 * The creator of the module needs to implement these functions
 */
// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\h.def.tmpl") ]]]
public:

    //To be implemented in the user's derived class
    virtual void TestInt(int callbackID, int32 testInt) = 0;
    virtual void TestUInt(int callbackID, uint32 testUInt) = 0;
    virtual void TestLong(int callbackID, int64 testLong) = 0;
    virtual void TestULong(int callbackID, uint64 testULong) = 0;
    virtual void TestDouble(int callbackID, double testDouble) = 0;
    virtual void TestBool(int callbackID, bool testBool) = 0;
    virtual void TestString(int callbackID, const char* testString) = 0;
    virtual void TestArray(int callbackID, const std::vector<int32 > testArray) = 0;
    virtual void TestObject(int callbackID, int32 param) = 0;
    virtual void TestPhatObject(int callbackID, const std::vector<int32 > array, int32 param2, const ClassTest& klass, const char* string) = 0;
    virtual void TestPhatArray(int callbackID, const std::vector<std::vector<int32 > > testArray) = 0;
    virtual void TestPhatArray1(int callbackID, const std::vector<std::string > testArray) = 0;
    virtual void TestPhatArray2(int callbackID, const std::vector<ClassTest > testArray) = 0;
    virtual void TestClass(int callbackID, const ClassTest& testClass) = 0;
    virtual void TestPhatClass(int callbackID, const PhatClassTest& testClass) = 0;
    virtual void TestChildClass(int callbackID, const ChildClassTest& testChildClass) = 0;
    virtual void TestEnum(int callbackID, EnumTest testEnum) = 0;
    virtual void TestDict(int callbackID, const std::map<std::string, std::string>& testDict) = 0;



// [[[end]]]

// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\h.end.tmpl") ]]]
};
// [[[end]]]
