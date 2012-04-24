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
#include "AccelerometerBase.h"


CAccelerometerBase::CAccelerometerBase(CIwWMDispatcherBase* dispatcher) :
    CIwWMDispatcherModule<CAccelerometerBase>("accelerometer", dispatcher)
{

    RegisterMember("getCurrentAcceleration", &CAccelerometerBase::getCurrentAcceleration);
    RegisterMember("watchAcceleration", &CAccelerometerBase::watchAcceleration);
    RegisterMember("clearWatch", &CAccelerometerBase::clearWatch);
}

// [[[end]]]

/*
 * Auto Generated section for converting between JavaScript and C++
 */
// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\cpp.bridge.tmpl") ]]]

//JS->C++ automated converters
bool CAccelerometerBase::getCurrentAcceleration(int callbackID, const Json::Value& parameters)
{
    getCurrentAcceleration(callbackID);
    return true;
}

bool CAccelerometerBase::watchAcceleration(int callbackID, const Json::Value& parameters)
{
    Json::Value accelerometerOptions_JSON;
    FetchObject(parameters, "accelerometerOptions", accelerometerOptions_JSON);

    AccelerometerOptions accelerometerOptions=AccelerometerOptions::From_JSON(accelerometerOptions_JSON, this);


    Json::Value watchID_JSON;
    FetchObject(parameters, "watchID", watchID_JSON);

    uint32 watchID;
    FetchUInt(watchID_JSON, watchID);



    watchAcceleration(callbackID, accelerometerOptions, watchID);
    return true;
}

bool CAccelerometerBase::clearWatch(int callbackID, const Json::Value& parameters)
{
    Json::Value watchID_JSON;
    FetchObject(parameters, "watchID", watchID_JSON);

    uint32 watchID;
    FetchUInt(watchID_JSON, watchID);



    clearWatch(callbackID, watchID);
    return true;
}

CAccelerometerBase::Acceleration CAccelerometerBase::Acceleration::From_JSON(const Json::Value& value, CAccelerometerBase* reader)
{
    Json::Value x_JSON;
    reader->FetchObject(value, "x", x_JSON);

    double x;
    reader->FetchDouble(x_JSON, x);


    Json::Value y_JSON;
    reader->FetchObject(value, "y", y_JSON);

    double y;
    reader->FetchDouble(y_JSON, y);


    Json::Value z_JSON;
    reader->FetchObject(value, "z", z_JSON);

    double z;
    reader->FetchDouble(z_JSON, z);


    Json::Value timestamp_JSON;
    reader->FetchObject(value, "timestamp", timestamp_JSON);

    uint64 timestamp;
    reader->FetchULong(timestamp_JSON, timestamp);


    return CAccelerometerBase::Acceleration(x, y, z, timestamp);
}

CAccelerometerBase::AccelerometerOptions CAccelerometerBase::AccelerometerOptions::From_JSON(const Json::Value& value, CAccelerometerBase* reader)
{
    Json::Value frequency_JSON;
    reader->FetchObject(value, "frequency", frequency_JSON);

    uint32 frequency;
    reader->FetchUInt(frequency_JSON, frequency);


    return CAccelerometerBase::AccelerometerOptions(frequency);
}

//C++->JS automated event setup

//C++->JS automated callbacks
bool CAccelerometerBase::accelerometerSuccess(int callbackID, const Acceleration& acceleration)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    Json::Value acceleration_JSON(Json::objectValue);
    acceleration.To_JSON(acceleration_JSON);
    params["acceleration"]=acceleration_JSON;


    SendJavaScript("accelerometerSuccess", callbackID, params);
    return true;
}

bool CAccelerometerBase::accelerometerError(int callbackID)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);

    SendJavaScript("accelerometerError", callbackID, params);
    return true;
}

void CAccelerometerBase::Acceleration::To_JSON(Json::Value& value) const
{
    value["x"]=Json::Value(x);

    value["y"]=Json::Value(y);

    value["z"]=Json::Value(z);

    value["timestamp"]=Json::Value(timestamp);

}

void CAccelerometerBase::AccelerometerOptions::To_JSON(Json::Value& value) const
{
    value["frequency"]=Json::Value(frequency);

}

//C++->JS automated events

// [[[end]]]
