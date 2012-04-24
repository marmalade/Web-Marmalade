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
 * Generated class for JavaScript bridge module events
 * Some sections of this module are re-generated when the interface changes the rest is generated once
 */


class CEventsBase : public CIwWMDispatcherModule<CEventsBase>
{
public:
    CEventsBase(CIwWMDispatcherBase* dispatcher);

    class Device
    {
    public:
    public:
        std::string name;
        std::string phonegap;
        std::string platform;
        std::string uuid;
        std::string version;
        std::string marmalade;

        Device(const char* _name,const char* _phonegap,const char* _platform,const char* _uuid,const char* _version,const char* _marmalade) : name(_name), phonegap(_phonegap), platform(_platform), uuid(_uuid), version(_version), marmalade(_marmalade) {}

        void To_JSON(Json::Value& value) const;
        static Device From_JSON(const Json::Value& value, CEventsBase* reader);
    };
    class BatteryInfo
    {
    public:
    public:
        uint32 level;
        bool isPlugged;

        BatteryInfo(uint32 _level,bool _isPlugged) : level(_level), isPlugged(_isPlugged) {}

        void To_JSON(Json::Value& value) const;
        static BatteryInfo From_JSON(const Json::Value& value, CEventsBase* reader);
    };

// [[[end]]]

/*
 * Auto Generated section for converting between JavaScript and C++
 */
// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\h.bridge.tmpl") ]]]
private:
    //JS->C++ automated converters
    //C++->JS automated event setup
    bool deviceready(int callbackID, const Json::Value& parameters);

    bool pause(int callbackID, const Json::Value& parameters);

    bool resume(int callbackID, const Json::Value& parameters);

    bool online(int callbackID, const Json::Value& parameters);

    bool offline(int callbackID, const Json::Value& parameters);

    bool backbutton(int callbackID, const Json::Value& parameters);

    bool batterycritical(int callbackID, const Json::Value& parameters);

    bool batterylow(int callbackID, const Json::Value& parameters);

    bool batterystatus(int callbackID, const Json::Value& parameters);

    bool menubutton(int callbackID, const Json::Value& parameters);

    bool searchbutton(int callbackID, const Json::Value& parameters);

    bool startcallbutton(int callbackID, const Json::Value& parameters);

    bool endcallbutton(int callbackID, const Json::Value& parameters);

    bool volumedownbutton(int callbackID, const Json::Value& parameters);

    bool volumeupbutton(int callbackID, const Json::Value& parameters);

public:
    //C++->JS automated callbacks
    //C++->JS automated events
    bool deviceready(const Device& device);

    bool pause();

    bool resume();

    bool online();

    bool offline();

    bool backbutton();

    bool batterycritical(const BatteryInfo& batteryinfo);

    bool batterylow(const BatteryInfo& batteryinfo);

    bool batterystatus(const BatteryInfo& batteryinfo);

    bool menubutton();

    bool searchbutton();

    bool startcallbutton();

    bool endcallbutton();

    bool volumedownbutton();

    bool volumeupbutton();


// [[[end]]]

/*
 * Auto Generated section for defining modules functions
 * The creator of the module needs to implement these functions
 */
// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\h.def.tmpl") ]]]
public:

    //To be implemented in the user's derived class


    virtual void devicereadyController(bool isStart) = 0;
    virtual void pauseController(bool isStart) = 0;
    virtual void resumeController(bool isStart) = 0;
    virtual void onlineController(bool isStart) = 0;
    virtual void offlineController(bool isStart) = 0;
    virtual void backbuttonController(bool isStart) = 0;
    virtual void batterycriticalController(bool isStart) = 0;
    virtual void batterylowController(bool isStart) = 0;
    virtual void batterystatusController(bool isStart) = 0;
    virtual void menubuttonController(bool isStart) = 0;
    virtual void searchbuttonController(bool isStart) = 0;
    virtual void startcallbuttonController(bool isStart) = 0;
    virtual void endcallbuttonController(bool isStart) = 0;
    virtual void volumedownbuttonController(bool isStart) = 0;
    virtual void volumeupbuttonController(bool isStart) = 0;

// [[[end]]]

// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\h.end.tmpl") ]]]
};
// [[[end]]]
