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
#include "CompassBase.h"


CCompassBase::CCompassBase(CIwWMDispatcherBase* dispatcher) :
    CIwWMDispatcherModule<CCompassBase>("compass", dispatcher)
{

    RegisterMember("getCurrentHeading", &CCompassBase::getCurrentHeading);
    RegisterMember("watchHeading", &CCompassBase::watchHeading);
    RegisterMember("clearWatch", &CCompassBase::clearWatch);
    RegisterMember("watchHeadingFilter", &CCompassBase::watchHeadingFilter);
    RegisterMember("clearWatchFilter", &CCompassBase::clearWatchFilter);
}

// [[[end]]]

/*
 * Auto Generated section for converting between JavaScript and C++
 */
// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\cpp.bridge.tmpl") ]]]

//JS->C++ automated converters
bool CCompassBase::getCurrentHeading(int callbackID, const Json::Value& parameters)
{
    Json::Value compassOptions_JSON;
    FetchObject(parameters, "compassOptions", compassOptions_JSON);

    CompassOptions compassOptions=CompassOptions::From_JSON(compassOptions_JSON, this);


    getCurrentHeading(callbackID, compassOptions);
    return true;
}

bool CCompassBase::watchHeading(int callbackID, const Json::Value& parameters)
{
    Json::Value compassOptions_JSON;
    FetchObject(parameters, "compassOptions", compassOptions_JSON);

    CompassOptions compassOptions=CompassOptions::From_JSON(compassOptions_JSON, this);


    Json::Value watchID_JSON;
    FetchObject(parameters, "watchID", watchID_JSON);

    uint32 watchID;
    FetchUInt(watchID_JSON, watchID);



    watchHeading(callbackID, compassOptions, watchID);
    return true;
}

bool CCompassBase::clearWatch(int callbackID, const Json::Value& parameters)
{
    Json::Value watchID_JSON;
    FetchObject(parameters, "watchID", watchID_JSON);

    uint32 watchID;
    FetchUInt(watchID_JSON, watchID);



    clearWatch(callbackID, watchID);
    return true;
}

bool CCompassBase::watchHeadingFilter(int callbackID, const Json::Value& parameters)
{
    Json::Value compassOptions_JSON;
    FetchObject(parameters, "compassOptions", compassOptions_JSON);

    CompassOptions compassOptions=CompassOptions::From_JSON(compassOptions_JSON, this);


    Json::Value watchID_JSON;
    FetchObject(parameters, "watchID", watchID_JSON);

    uint32 watchID;
    FetchUInt(watchID_JSON, watchID);



    watchHeadingFilter(callbackID, compassOptions, watchID);
    return true;
}

bool CCompassBase::clearWatchFilter(int callbackID, const Json::Value& parameters)
{
    Json::Value watchID_JSON;
    FetchObject(parameters, "watchID", watchID_JSON);

    uint32 watchID;
    FetchUInt(watchID_JSON, watchID);



    clearWatchFilter(callbackID, watchID);
    return true;
}

CCompassBase::CompassHeading CCompassBase::CompassHeading::From_JSON(const Json::Value& value, CCompassBase* reader)
{
    Json::Value magneticHeading_JSON;
    reader->FetchObject(value, "magneticHeading", magneticHeading_JSON);

    double magneticHeading;
    reader->FetchDouble(magneticHeading_JSON, magneticHeading);


    Json::Value trueHeading_JSON;
    reader->FetchObject(value, "trueHeading", trueHeading_JSON);

    double trueHeading;
    reader->FetchDouble(trueHeading_JSON, trueHeading);


    Json::Value headingAccuracy_JSON;
    reader->FetchObject(value, "headingAccuracy", headingAccuracy_JSON);

    double headingAccuracy;
    reader->FetchDouble(headingAccuracy_JSON, headingAccuracy);


    Json::Value timestamp_JSON;
    reader->FetchObject(value, "timestamp", timestamp_JSON);

    uint64 timestamp;
    reader->FetchULong(timestamp_JSON, timestamp);


    return CCompassBase::CompassHeading(magneticHeading, trueHeading, headingAccuracy, timestamp);
}

CCompassBase::CompassOptions CCompassBase::CompassOptions::From_JSON(const Json::Value& value, CCompassBase* reader)
{
    Json::Value frequency_JSON;
    reader->FetchObject(value, "frequency", frequency_JSON);

    uint32 frequency;
    reader->FetchUInt(frequency_JSON, frequency);


    Json::Value filter_JSON;
    reader->FetchObject(value, "filter", filter_JSON);

    uint32 filter;
    reader->FetchUInt(filter_JSON, filter);


    return CCompassBase::CompassOptions(frequency, filter);
}

CCompassBase::CompassError CCompassBase::CompassError::From_JSON(const Json::Value& value, CCompassBase* reader)
{
    Json::Value code_JSON;
    reader->FetchObject(value, "code", code_JSON);

    int32 code;
    reader->FetchInt(code_JSON, code);


    return CCompassBase::CompassError(code);
}

//C++->JS automated event setup

//C++->JS automated callbacks
bool CCompassBase::compassSuccess(int callbackID, const CompassHeading& heading)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    Json::Value heading_JSON(Json::objectValue);
    heading.To_JSON(heading_JSON);
    params["heading"]=heading_JSON;


    SendJavaScript("compassSuccess", callbackID, params);
    return true;
}

bool CCompassBase::compassError(int callbackID, const CompassError& error)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    Json::Value error_JSON(Json::objectValue);
    error.To_JSON(error_JSON);
    params["error"]=error_JSON;


    SendJavaScript("compassError", callbackID, params);
    return true;
}

void CCompassBase::CompassHeading::To_JSON(Json::Value& value) const
{
    value["magneticHeading"]=Json::Value(magneticHeading);

    value["trueHeading"]=Json::Value(trueHeading);

    value["headingAccuracy"]=Json::Value(headingAccuracy);

    value["timestamp"]=Json::Value(timestamp);

}

void CCompassBase::CompassOptions::To_JSON(Json::Value& value) const
{
    value["frequency"]=Json::Value(frequency);

    value["filter"]=Json::Value(filter);

}

void CCompassBase::CompassError::To_JSON(Json::Value& value) const
{
    value["code"]=Json::Value(code);

}

//C++->JS automated events

// [[[end]]]
