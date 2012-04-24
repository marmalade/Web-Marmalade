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
 * Generated class for JavaScript bridge module notification
 * Some sections of this module are re-generated when the interface changes the rest is generated once
 */


class CNotificationBase : public CIwWMDispatcherModule<CNotificationBase>
{
public:
    CNotificationBase(CIwWMDispatcherBase* dispatcher);


// [[[end]]]

/*
 * Auto Generated section for converting between JavaScript and C++
 */
// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\h.bridge.tmpl") ]]]
private:
    //JS->C++ automated converters
    bool alert(int callbackID, const Json::Value& parameters);

    bool confirm(int callbackID, const Json::Value& parameters);

    bool beep(int callbackID, const Json::Value& parameters);

    bool vibrate(int callbackID, const Json::Value& parameters);

    //C++->JS automated event setup
public:
    //C++->JS automated callbacks
    bool alertSuccess(int callbackID);

    bool confirmSuccess(int callbackID, int32 button);

    //C++->JS automated events

// [[[end]]]

/*
 * Auto Generated section for defining modules functions
 * The creator of the module needs to implement these functions
 */
// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\h.def.tmpl") ]]]
public:

    //To be implemented in the user's derived class
    virtual void alert(int callbackID, const char* message, const char* title, const char* buttonName) = 0;
    virtual void confirm(int callbackID, const char* message, const char* title, const char* buttonLabels) = 0;
    virtual void beep(int callbackID, int32 times) = 0;
    virtual void vibrate(int callbackID, int32 time) = 0;



// [[[end]]]

// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\h.end.tmpl") ]]]
};
// [[[end]]]
