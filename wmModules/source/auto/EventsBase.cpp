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
#include "EventsBase.h"


CEventsBase::CEventsBase(CIwWMDispatcherBase* dispatcher) :
    CIwWMDispatcherModule<CEventsBase>("events", dispatcher)
{

    RegisterMember("deviceready", &CEventsBase::deviceready);
    RegisterMember("pause", &CEventsBase::pause);
    RegisterMember("resume", &CEventsBase::resume);
    RegisterMember("online", &CEventsBase::online);
    RegisterMember("offline", &CEventsBase::offline);
    RegisterMember("backbutton", &CEventsBase::backbutton);
    RegisterMember("batterycritical", &CEventsBase::batterycritical);
    RegisterMember("batterylow", &CEventsBase::batterylow);
    RegisterMember("batterystatus", &CEventsBase::batterystatus);
    RegisterMember("menubutton", &CEventsBase::menubutton);
    RegisterMember("searchbutton", &CEventsBase::searchbutton);
    RegisterMember("startcallbutton", &CEventsBase::startcallbutton);
    RegisterMember("endcallbutton", &CEventsBase::endcallbutton);
    RegisterMember("volumedownbutton", &CEventsBase::volumedownbutton);
    RegisterMember("volumeupbutton", &CEventsBase::volumeupbutton);
}

// [[[end]]]

/*
 * Auto Generated section for converting between JavaScript and C++
 */
// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\cpp.bridge.tmpl") ]]]

//JS->C++ automated converters
CEventsBase::Device CEventsBase::Device::From_JSON(const Json::Value& value, CEventsBase* reader)
{
    Json::Value name_JSON;
    reader->FetchObject(value, "name", name_JSON);

    std::string name;
    reader->FetchString(name_JSON, name);


    Json::Value phonegap_JSON;
    reader->FetchObject(value, "phonegap", phonegap_JSON);

    std::string phonegap;
    reader->FetchString(phonegap_JSON, phonegap);


    Json::Value platform_JSON;
    reader->FetchObject(value, "platform", platform_JSON);

    std::string platform;
    reader->FetchString(platform_JSON, platform);


    Json::Value uuid_JSON;
    reader->FetchObject(value, "uuid", uuid_JSON);

    std::string uuid;
    reader->FetchString(uuid_JSON, uuid);


    Json::Value version_JSON;
    reader->FetchObject(value, "version", version_JSON);

    std::string version;
    reader->FetchString(version_JSON, version);


    Json::Value marmalade_JSON;
    reader->FetchObject(value, "marmalade", marmalade_JSON);

    std::string marmalade;
    reader->FetchString(marmalade_JSON, marmalade);


    return CEventsBase::Device(name.c_str(), phonegap.c_str(), platform.c_str(), uuid.c_str(), version.c_str(), marmalade.c_str());
}

CEventsBase::BatteryInfo CEventsBase::BatteryInfo::From_JSON(const Json::Value& value, CEventsBase* reader)
{
    Json::Value level_JSON;
    reader->FetchObject(value, "level", level_JSON);

    uint32 level;
    reader->FetchUInt(level_JSON, level);


    Json::Value isPlugged_JSON;
    reader->FetchObject(value, "isPlugged", isPlugged_JSON);

    bool isPlugged;
    reader->FetchBool(isPlugged_JSON, isPlugged);


    return CEventsBase::BatteryInfo(level, isPlugged);
}

//C++->JS automated event setup
bool CEventsBase::deviceready(int callbackID, const Json::Value& parameters)
{
    Json::Value isStart_JSON;
    if (!FetchObject(parameters, "isStart", isStart_JSON))
        return false;

    bool isStart;
    if (!FetchBool(isStart_JSON, isStart))
        return false;

    devicereadyController(isStart);
    return true;
}

bool CEventsBase::pause(int callbackID, const Json::Value& parameters)
{
    Json::Value isStart_JSON;
    if (!FetchObject(parameters, "isStart", isStart_JSON))
        return false;

    bool isStart;
    if (!FetchBool(isStart_JSON, isStart))
        return false;

    pauseController(isStart);
    return true;
}

bool CEventsBase::resume(int callbackID, const Json::Value& parameters)
{
    Json::Value isStart_JSON;
    if (!FetchObject(parameters, "isStart", isStart_JSON))
        return false;

    bool isStart;
    if (!FetchBool(isStart_JSON, isStart))
        return false;

    resumeController(isStart);
    return true;
}

bool CEventsBase::online(int callbackID, const Json::Value& parameters)
{
    Json::Value isStart_JSON;
    if (!FetchObject(parameters, "isStart", isStart_JSON))
        return false;

    bool isStart;
    if (!FetchBool(isStart_JSON, isStart))
        return false;

    onlineController(isStart);
    return true;
}

bool CEventsBase::offline(int callbackID, const Json::Value& parameters)
{
    Json::Value isStart_JSON;
    if (!FetchObject(parameters, "isStart", isStart_JSON))
        return false;

    bool isStart;
    if (!FetchBool(isStart_JSON, isStart))
        return false;

    offlineController(isStart);
    return true;
}

bool CEventsBase::backbutton(int callbackID, const Json::Value& parameters)
{
    Json::Value isStart_JSON;
    if (!FetchObject(parameters, "isStart", isStart_JSON))
        return false;

    bool isStart;
    if (!FetchBool(isStart_JSON, isStart))
        return false;

    backbuttonController(isStart);
    return true;
}

bool CEventsBase::batterycritical(int callbackID, const Json::Value& parameters)
{
    Json::Value isStart_JSON;
    if (!FetchObject(parameters, "isStart", isStart_JSON))
        return false;

    bool isStart;
    if (!FetchBool(isStart_JSON, isStart))
        return false;

    batterycriticalController(isStart);
    return true;
}

bool CEventsBase::batterylow(int callbackID, const Json::Value& parameters)
{
    Json::Value isStart_JSON;
    if (!FetchObject(parameters, "isStart", isStart_JSON))
        return false;

    bool isStart;
    if (!FetchBool(isStart_JSON, isStart))
        return false;

    batterylowController(isStart);
    return true;
}

bool CEventsBase::batterystatus(int callbackID, const Json::Value& parameters)
{
    Json::Value isStart_JSON;
    if (!FetchObject(parameters, "isStart", isStart_JSON))
        return false;

    bool isStart;
    if (!FetchBool(isStart_JSON, isStart))
        return false;

    batterystatusController(isStart);
    return true;
}

bool CEventsBase::menubutton(int callbackID, const Json::Value& parameters)
{
    Json::Value isStart_JSON;
    if (!FetchObject(parameters, "isStart", isStart_JSON))
        return false;

    bool isStart;
    if (!FetchBool(isStart_JSON, isStart))
        return false;

    menubuttonController(isStart);
    return true;
}

bool CEventsBase::searchbutton(int callbackID, const Json::Value& parameters)
{
    Json::Value isStart_JSON;
    if (!FetchObject(parameters, "isStart", isStart_JSON))
        return false;

    bool isStart;
    if (!FetchBool(isStart_JSON, isStart))
        return false;

    searchbuttonController(isStart);
    return true;
}

bool CEventsBase::startcallbutton(int callbackID, const Json::Value& parameters)
{
    Json::Value isStart_JSON;
    if (!FetchObject(parameters, "isStart", isStart_JSON))
        return false;

    bool isStart;
    if (!FetchBool(isStart_JSON, isStart))
        return false;

    startcallbuttonController(isStart);
    return true;
}

bool CEventsBase::endcallbutton(int callbackID, const Json::Value& parameters)
{
    Json::Value isStart_JSON;
    if (!FetchObject(parameters, "isStart", isStart_JSON))
        return false;

    bool isStart;
    if (!FetchBool(isStart_JSON, isStart))
        return false;

    endcallbuttonController(isStart);
    return true;
}

bool CEventsBase::volumedownbutton(int callbackID, const Json::Value& parameters)
{
    Json::Value isStart_JSON;
    if (!FetchObject(parameters, "isStart", isStart_JSON))
        return false;

    bool isStart;
    if (!FetchBool(isStart_JSON, isStart))
        return false;

    volumedownbuttonController(isStart);
    return true;
}

bool CEventsBase::volumeupbutton(int callbackID, const Json::Value& parameters)
{
    Json::Value isStart_JSON;
    if (!FetchObject(parameters, "isStart", isStart_JSON))
        return false;

    bool isStart;
    if (!FetchBool(isStart_JSON, isStart))
        return false;

    volumeupbuttonController(isStart);
    return true;
}


//C++->JS automated callbacks
void CEventsBase::Device::To_JSON(Json::Value& value) const
{
    value["name"]=Json::Value(name);

    value["phonegap"]=Json::Value(phonegap);

    value["platform"]=Json::Value(platform);

    value["uuid"]=Json::Value(uuid);

    value["version"]=Json::Value(version);

    value["marmalade"]=Json::Value(marmalade);

}

void CEventsBase::BatteryInfo::To_JSON(Json::Value& value) const
{
    value["level"]=Json::Value(level);

    value["isPlugged"]=Json::Value(isPlugged);

}

//C++->JS automated events
bool CEventsBase::deviceready(const Device& device)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    Json::Value device_JSON(Json::objectValue);
    device.To_JSON(device_JSON);
    params["device"]=device_JSON;


    SendJavaScript("fireEvent", "deviceready", params);
    return true;
}

bool CEventsBase::pause()
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);

    SendJavaScript("fireEvent", "pause", params);
    return true;
}

bool CEventsBase::resume()
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);

    SendJavaScript("fireEvent", "resume", params);
    return true;
}

bool CEventsBase::online()
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);

    SendJavaScript("fireEvent", "online", params);
    return true;
}

bool CEventsBase::offline()
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);

    SendJavaScript("fireEvent", "offline", params);
    return true;
}

bool CEventsBase::backbutton()
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);

    SendJavaScript("fireEvent", "backbutton", params);
    return true;
}

bool CEventsBase::batterycritical(const BatteryInfo& batteryinfo)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    Json::Value batteryinfo_JSON(Json::objectValue);
    batteryinfo.To_JSON(batteryinfo_JSON);
    params["batteryinfo"]=batteryinfo_JSON;


    SendJavaScript("fireEvent", "batterycritical", params);
    return true;
}

bool CEventsBase::batterylow(const BatteryInfo& batteryinfo)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    Json::Value batteryinfo_JSON(Json::objectValue);
    batteryinfo.To_JSON(batteryinfo_JSON);
    params["batteryinfo"]=batteryinfo_JSON;


    SendJavaScript("fireEvent", "batterylow", params);
    return true;
}

bool CEventsBase::batterystatus(const BatteryInfo& batteryinfo)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    Json::Value batteryinfo_JSON(Json::objectValue);
    batteryinfo.To_JSON(batteryinfo_JSON);
    params["batteryinfo"]=batteryinfo_JSON;


    SendJavaScript("fireEvent", "batterystatus", params);
    return true;
}

bool CEventsBase::menubutton()
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);

    SendJavaScript("fireEvent", "menubutton", params);
    return true;
}

bool CEventsBase::searchbutton()
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);

    SendJavaScript("fireEvent", "searchbutton", params);
    return true;
}

bool CEventsBase::startcallbutton()
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);

    SendJavaScript("fireEvent", "startcallbutton", params);
    return true;
}

bool CEventsBase::endcallbutton()
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);

    SendJavaScript("fireEvent", "endcallbutton", params);
    return true;
}

bool CEventsBase::volumedownbutton()
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);

    SendJavaScript("fireEvent", "volumedownbutton", params);
    return true;
}

bool CEventsBase::volumeupbutton()
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);

    SendJavaScript("fireEvent", "volumeupbutton", params);
    return true;
}


// [[[end]]]
