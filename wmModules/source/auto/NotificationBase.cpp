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
#include "NotificationBase.h"


CNotificationBase::CNotificationBase(CIwWMDispatcherBase* dispatcher) :
    CIwWMDispatcherModule<CNotificationBase>("notification", dispatcher)
{

    RegisterMember("alert", &CNotificationBase::alert);
    RegisterMember("confirm", &CNotificationBase::confirm);
    RegisterMember("beep", &CNotificationBase::beep);
    RegisterMember("vibrate", &CNotificationBase::vibrate);
}

// [[[end]]]

/*
 * Auto Generated section for converting between JavaScript and C++
 */
// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\cpp.bridge.tmpl") ]]]

//JS->C++ automated converters
bool CNotificationBase::alert(int callbackID, const Json::Value& parameters)
{
    Json::Value message_JSON;
    FetchObject(parameters, "message", message_JSON);

    std::string message;
    FetchString(message_JSON, message);



    Json::Value title_JSON;
    FetchObject(parameters, "title", title_JSON);

    std::string title;
    FetchString(title_JSON, title);



    Json::Value buttonName_JSON;
    FetchObject(parameters, "buttonName", buttonName_JSON);

    std::string buttonName;
    FetchString(buttonName_JSON, buttonName);



    alert(callbackID, message.c_str(), title.c_str(), buttonName.c_str());
    return true;
}

bool CNotificationBase::confirm(int callbackID, const Json::Value& parameters)
{
    Json::Value message_JSON;
    FetchObject(parameters, "message", message_JSON);

    std::string message;
    FetchString(message_JSON, message);



    Json::Value title_JSON;
    FetchObject(parameters, "title", title_JSON);

    std::string title;
    FetchString(title_JSON, title);



    Json::Value buttonLabels_JSON;
    FetchObject(parameters, "buttonLabels", buttonLabels_JSON);

    std::string buttonLabels;
    FetchString(buttonLabels_JSON, buttonLabels);



    confirm(callbackID, message.c_str(), title.c_str(), buttonLabels.c_str());
    return true;
}

bool CNotificationBase::beep(int callbackID, const Json::Value& parameters)
{
    Json::Value times_JSON;
    FetchObject(parameters, "times", times_JSON);

    int32 times;
    FetchInt(times_JSON, times);



    beep(callbackID, times);
    return true;
}

bool CNotificationBase::vibrate(int callbackID, const Json::Value& parameters)
{
    Json::Value time_JSON;
    FetchObject(parameters, "time", time_JSON);

    int32 time;
    FetchInt(time_JSON, time);



    vibrate(callbackID, time);
    return true;
}

//C++->JS automated event setup

//C++->JS automated callbacks
bool CNotificationBase::alertSuccess(int callbackID)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);

    SendJavaScript("alertSuccess", callbackID, params);
    return true;
}

bool CNotificationBase::confirmSuccess(int callbackID, int32 button)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    params["button"]=Json::Value(button);


    SendJavaScript("confirmSuccess", callbackID, params);
    return true;
}

//C++->JS automated events

// [[[end]]]
