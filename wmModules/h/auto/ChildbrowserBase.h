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
 * Generated class for JavaScript bridge module childBrowser
 * Some sections of this module are re-generated when the interface changes the rest is generated once
 */


class CChildbrowserBase : public CIwWMDispatcherModule<CChildbrowserBase>
{
public:
    CChildbrowserBase(CIwWMDispatcherBase* dispatcher);

    class ChildBrowser
    {
    public:
    public:

        ChildBrowser() {}

        void To_JSON(Json::Value& value) const;
        static ChildBrowser From_JSON(const Json::Value& value, CChildbrowserBase* reader);
    };

// [[[end]]]

/*
 * Auto Generated section for converting between JavaScript and C++
 */
// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\h.bridge.tmpl") ]]]
private:
    //JS->C++ automated converters
    bool ChildBrowser_showWebPage(int callbackID, const Json::Value& parameters);

    bool ChildBrowser_close(int callbackID, const Json::Value& parameters);

    //C++->JS automated event setup
    bool onLocationChangeEvent(int callbackID, const Json::Value& parameters);

    bool onFailEvent(int callbackID, const Json::Value& parameters);

    bool onCloseEvent(int callbackID, const Json::Value& parameters);

    bool onOpenExternalEvent(int callbackID, const Json::Value& parameters);

public:
    //C++->JS automated callbacks
    //C++->JS automated events
    bool onLocationChangeEvent(const char* newurl);

    bool onFailEvent(const char* message);

    bool onCloseEvent();

    bool onOpenExternalEvent();


// [[[end]]]

/*
 * Auto Generated section for defining modules functions
 * The creator of the module needs to implement these functions
 */
// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\h.def.tmpl") ]]]
public:

    //To be implemented in the user's derived class

    virtual void ChildBrowser_showWebPage(int callbackID, const ChildBrowser& self, const char* url) = 0;
    virtual void ChildBrowser_close(int callbackID, const ChildBrowser& self) = 0;

    virtual void onLocationChangeEventController(bool isStart) = 0;
    virtual void onFailEventController(bool isStart) = 0;
    virtual void onCloseEventController(bool isStart) = 0;
    virtual void onOpenExternalEventController(bool isStart) = 0;

// [[[end]]]

// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\h.end.tmpl") ]]]
};
// [[[end]]]
