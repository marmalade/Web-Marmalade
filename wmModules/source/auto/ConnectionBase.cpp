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
#include "ConnectionBase.h"


CConnectionBase::CConnectionBase(CIwWMDispatcherBase* dispatcher) :
    CIwWMDispatcherModule<CConnectionBase>("connection", dispatcher)
{

    RegisterMember("network", &CConnectionBase::network);
}

// [[[end]]]

/*
 * Auto Generated section for converting between JavaScript and C++
 */
// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\cpp.bridge.tmpl") ]]]

//JS->C++ automated converters
const char* CConnectionBase::Connection::UNKNOWN = "unknown";
const char* CConnectionBase::Connection::ETHERNET = "ethernet";
const char* CConnectionBase::Connection::WIFI = "wifi";
const char* CConnectionBase::Connection::CELL_2G = "2g";
const char* CConnectionBase::Connection::CELL_3G = "3g";
const char* CConnectionBase::Connection::CELL_4G = "4g";
const char* CConnectionBase::Connection::NONE = "none";

CConnectionBase::Connection CConnectionBase::Connection::From_JSON(const Json::Value& value, CConnectionBase* reader)
{
    Json::Value type_JSON;
    reader->FetchObject(value, "type", type_JSON);

    std::string type;
    reader->FetchString(type_JSON, type);


    return CConnectionBase::Connection(type.c_str());
}

//C++->JS automated event setup
bool CConnectionBase::network(int callbackID, const Json::Value& parameters)
{
    Json::Value isStart_JSON;
    if (!FetchObject(parameters, "isStart", isStart_JSON))
        return false;

    bool isStart;
    if (!FetchBool(isStart_JSON, isStart))
        return false;

    networkController(isStart);
    return true;
}


//C++->JS automated callbacks
void CConnectionBase::Connection::To_JSON(Json::Value& value) const
{
    value["type"]=Json::Value(type);

}

//C++->JS automated events
bool CConnectionBase::network(const char* type)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    params["type"]=Json::Value(type);


    SendJavaScript("fireEvent", "network", params);
    return true;
}


// [[[end]]]
