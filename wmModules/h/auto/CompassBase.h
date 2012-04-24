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
 * Generated class for JavaScript bridge module compass
 * Some sections of this module are re-generated when the interface changes the rest is generated once
 */


class CCompassBase : public CIwWMDispatcherModule<CCompassBase>
{
public:
    CCompassBase(CIwWMDispatcherBase* dispatcher);

    class CompassHeading
    {
    public:
    public:
        double magneticHeading;
        double trueHeading;
        double headingAccuracy;
        uint64 timestamp;

        CompassHeading(double _magneticHeading,double _trueHeading,double _headingAccuracy,uint64 _timestamp) : magneticHeading(_magneticHeading), trueHeading(_trueHeading), headingAccuracy(_headingAccuracy), timestamp(_timestamp) {}

        void To_JSON(Json::Value& value) const;
        static CompassHeading From_JSON(const Json::Value& value, CCompassBase* reader);
    };
    class CompassOptions
    {
    public:
    public:
        uint32 frequency;
        uint32 filter;

        CompassOptions(uint32 _frequency,uint32 _filter) : frequency(_frequency), filter(_filter) {}

        void To_JSON(Json::Value& value) const;
        static CompassOptions From_JSON(const Json::Value& value, CCompassBase* reader);
    };
    class CompassError
    {
    public:
        enum CompassErrorEnum
        {
            COMPASS_INTERNAL_ERR = 0,
            COMPASS_NOT_SUPPORTED = 20,
        };

    public:
        int32 code;

        CompassError(int32 _code) : code(_code) {}

        void To_JSON(Json::Value& value) const;
        static CompassError From_JSON(const Json::Value& value, CCompassBase* reader);
    };

// [[[end]]]

/*
 * Auto Generated section for converting between JavaScript and C++
 */
// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\h.bridge.tmpl") ]]]
private:
    //JS->C++ automated converters
    bool getCurrentHeading(int callbackID, const Json::Value& parameters);

    bool watchHeading(int callbackID, const Json::Value& parameters);

    bool clearWatch(int callbackID, const Json::Value& parameters);

    bool watchHeadingFilter(int callbackID, const Json::Value& parameters);

    bool clearWatchFilter(int callbackID, const Json::Value& parameters);

    //C++->JS automated event setup
public:
    //C++->JS automated callbacks
    bool compassSuccess(int callbackID, const CompassHeading& heading);

    bool compassError(int callbackID, const CompassError& error);

    //C++->JS automated events

// [[[end]]]

/*
 * Auto Generated section for defining modules functions
 * The creator of the module needs to implement these functions
 */
// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\h.def.tmpl") ]]]
public:

    //To be implemented in the user's derived class
    virtual void getCurrentHeading(int callbackID, const CompassOptions& compassOptions) = 0;
    virtual void watchHeading(int callbackID, const CompassOptions& compassOptions, uint32 watchID) = 0;
    virtual void clearWatch(int callbackID, uint32 watchID) = 0;
    virtual void watchHeadingFilter(int callbackID, const CompassOptions& compassOptions, uint32 watchID) = 0;
    virtual void clearWatchFilter(int callbackID, uint32 watchID) = 0;



// [[[end]]]

// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\h.end.tmpl") ]]]
};
// [[[end]]]
