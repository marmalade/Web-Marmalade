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
 * Generated class for JavaScript bridge module accelerometer
 * Some sections of this module are re-generated when the interface changes the rest is generated once
 */


class CAccelerometerBase : public CIwWMDispatcherModule<CAccelerometerBase>
{
public:
    CAccelerometerBase(CIwWMDispatcherBase* dispatcher);

    class Acceleration
    {
    public:
    public:
        double x;
        double y;
        double z;
        uint64 timestamp;

        Acceleration(double _x,double _y,double _z,uint64 _timestamp) : x(_x), y(_y), z(_z), timestamp(_timestamp) {}

        void To_JSON(Json::Value& value) const;
        static Acceleration From_JSON(const Json::Value& value, CAccelerometerBase* reader);
    };
    class AccelerometerOptions
    {
    public:
    public:
        uint32 frequency;

        AccelerometerOptions(uint32 _frequency) : frequency(_frequency) {}

        void To_JSON(Json::Value& value) const;
        static AccelerometerOptions From_JSON(const Json::Value& value, CAccelerometerBase* reader);
    };

// [[[end]]]

/*
 * Auto Generated section for converting between JavaScript and C++
 */
// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\h.bridge.tmpl") ]]]
private:
    //JS->C++ automated converters
    bool getCurrentAcceleration(int callbackID, const Json::Value& parameters);

    bool watchAcceleration(int callbackID, const Json::Value& parameters);

    bool clearWatch(int callbackID, const Json::Value& parameters);

    //C++->JS automated event setup
public:
    //C++->JS automated callbacks
    bool accelerometerSuccess(int callbackID, const Acceleration& acceleration);

    bool accelerometerError(int callbackID);

    //C++->JS automated events

// [[[end]]]

/*
 * Auto Generated section for defining modules functions
 * The creator of the module needs to implement these functions
 */
// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\h.def.tmpl") ]]]
public:

    //To be implemented in the user's derived class
    virtual void getCurrentAcceleration(int callbackID) = 0;
    virtual void watchAcceleration(int callbackID, const AccelerometerOptions& accelerometerOptions, uint32 watchID) = 0;
    virtual void clearWatch(int callbackID, uint32 watchID) = 0;



// [[[end]]]

// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\h.end.tmpl") ]]]
};
// [[[end]]]
