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
#include "ChildbrowserBase.h"


CChildbrowserBase::CChildbrowserBase(CIwWMDispatcherBase* dispatcher) :
    CIwWMDispatcherModule<CChildbrowserBase>("childBrowser", dispatcher)
{

    RegisterMember("ChildBrowser_showWebPage", &CChildbrowserBase::ChildBrowser_showWebPage);
    RegisterMember("ChildBrowser_close", &CChildbrowserBase::ChildBrowser_close);
    RegisterMember("onLocationChangeEvent", &CChildbrowserBase::onLocationChangeEvent);
    RegisterMember("onFailEvent", &CChildbrowserBase::onFailEvent);
    RegisterMember("onCloseEvent", &CChildbrowserBase::onCloseEvent);
    RegisterMember("onOpenExternalEvent", &CChildbrowserBase::onOpenExternalEvent);
}

// [[[end]]]
/*
 * Auto Generated section for converting between JavaScript and C++
 */
// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\cpp.bridge.tmpl") ]]]

//JS->C++ automated converters
bool CChildbrowserBase::ChildBrowser_showWebPage(int callbackID, const Json::Value& parameters)
{
    Json::Value self_JSON;
    FetchObject(parameters, "self", self_JSON);

    ChildBrowser self=ChildBrowser::From_JSON(self_JSON, this);


    Json::Value url_JSON;
    FetchObject(parameters, "url", url_JSON);

    std::string url;
    FetchString(url_JSON, url);



    ChildBrowser_showWebPage(callbackID, self, url.c_str());
    return true;
}

bool CChildbrowserBase::ChildBrowser_close(int callbackID, const Json::Value& parameters)
{
    Json::Value self_JSON;
    FetchObject(parameters, "self", self_JSON);

    ChildBrowser self=ChildBrowser::From_JSON(self_JSON, this);


    ChildBrowser_close(callbackID, self);
    return true;
}

CChildbrowserBase::ChildBrowser CChildbrowserBase::ChildBrowser::From_JSON(const Json::Value& value, CChildbrowserBase* reader)
{
    return CChildbrowserBase::ChildBrowser();
}

//C++->JS automated event setup
bool CChildbrowserBase::onLocationChangeEvent(int callbackID, const Json::Value& parameters)
{
    Json::Value isStart_JSON;
    if (!FetchObject(parameters, "isStart", isStart_JSON))
        return false;

    bool isStart;
    if (!FetchBool(isStart_JSON, isStart))
        return false;

    onLocationChangeEventController(isStart);
    return true;
}

bool CChildbrowserBase::onFailEvent(int callbackID, const Json::Value& parameters)
{
    Json::Value isStart_JSON;
    if (!FetchObject(parameters, "isStart", isStart_JSON))
        return false;

    bool isStart;
    if (!FetchBool(isStart_JSON, isStart))
        return false;

    onFailEventController(isStart);
    return true;
}

bool CChildbrowserBase::onCloseEvent(int callbackID, const Json::Value& parameters)
{
    Json::Value isStart_JSON;
    if (!FetchObject(parameters, "isStart", isStart_JSON))
        return false;

    bool isStart;
    if (!FetchBool(isStart_JSON, isStart))
        return false;

    onCloseEventController(isStart);
    return true;
}

bool CChildbrowserBase::onOpenExternalEvent(int callbackID, const Json::Value& parameters)
{
    Json::Value isStart_JSON;
    if (!FetchObject(parameters, "isStart", isStart_JSON))
        return false;

    bool isStart;
    if (!FetchBool(isStart_JSON, isStart))
        return false;

    onOpenExternalEventController(isStart);
    return true;
}


//C++->JS automated callbacks
void CChildbrowserBase::ChildBrowser::To_JSON(Json::Value& value) const
{
}

//C++->JS automated events
bool CChildbrowserBase::onLocationChangeEvent(const char* newurl)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    params["newurl"]=Json::Value(newurl);


    SendJavaScript("fireEvent", "onLocationChangeEvent", params);
    return true;
}

bool CChildbrowserBase::onFailEvent(const char* message)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    params["message"]=Json::Value(message);


    SendJavaScript("fireEvent", "onFailEvent", params);
    return true;
}

bool CChildbrowserBase::onCloseEvent()
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);

    SendJavaScript("fireEvent", "onCloseEvent", params);
    return true;
}

bool CChildbrowserBase::onOpenExternalEvent()
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);

    SendJavaScript("fireEvent", "onOpenExternalEvent", params);
    return true;
}


// [[[end]]]
