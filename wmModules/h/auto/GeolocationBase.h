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
 * Generated class for JavaScript bridge module geolocation
 * Some sections of this module are re-generated when the interface changes the rest is generated once
 */


class CGeolocationBase : public CIwWMDispatcherModule<CGeolocationBase>
{
public:
    CGeolocationBase(CIwWMDispatcherBase* dispatcher);

    class GeolocationOptions
    {
    public:
    public:
        uint32 frequency;
        bool enableHighAccuracy;
        uint32 timeout;
        uint32 maximumAge;

        GeolocationOptions(uint32 _frequency,bool _enableHighAccuracy,uint32 _timeout,uint32 _maximumAge) : frequency(_frequency), enableHighAccuracy(_enableHighAccuracy), timeout(_timeout), maximumAge(_maximumAge) {}

        void To_JSON(Json::Value& value) const;
        static GeolocationOptions From_JSON(const Json::Value& value, CGeolocationBase* reader);
    };
    class Coordinates
    {
    public:
    public:
        double latitude;
        double longitude;
        double altitude;
        double accuracy;
        double altitudeAccuracy;
        double heading;
        double speed;

        Coordinates(double _latitude,double _longitude,double _altitude,double _accuracy,double _altitudeAccuracy,double _heading,double _speed) : latitude(_latitude), longitude(_longitude), altitude(_altitude), accuracy(_accuracy), altitudeAccuracy(_altitudeAccuracy), heading(_heading), speed(_speed) {}

        void To_JSON(Json::Value& value) const;
        static Coordinates From_JSON(const Json::Value& value, CGeolocationBase* reader);
    };
    class PositionError
    {
    public:
        enum PositionErrorEnum
        {
            UNKNOWN_ERROR = 0,
            PERMISSION_DENIED = 1,
            POSITION_UNAVAILABLE = 2,
            TIMEOUT = 3,
        };

    public:
        PositionError::PositionErrorEnum code;
        std::string message;

        PositionError(PositionError::PositionErrorEnum _code,const char* _message) : code(_code), message(_message) {}

        void To_JSON(Json::Value& value) const;
        static PositionError From_JSON(const Json::Value& value, CGeolocationBase* reader);
    };
    class Position
    {
    public:
    public:
        Coordinates coords;
        uint64 timestamp;

        Position(const Coordinates& _coords,uint64 _timestamp) : coords(_coords), timestamp(_timestamp) {}

        void To_JSON(Json::Value& value) const;
        static Position From_JSON(const Json::Value& value, CGeolocationBase* reader);
    };

// [[[end]]]

/*
 * Auto Generated section for converting between JavaScript and C++
 */
// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\h.bridge.tmpl") ]]]
private:
    //JS->C++ automated converters
    bool getCurrentPosition(int callbackID, const Json::Value& parameters);

    bool watchPosition(int callbackID, const Json::Value& parameters);

    bool clearWatch(int callbackID, const Json::Value& parameters);

    //C++->JS automated event setup
public:
    //C++->JS automated callbacks
    bool geolocationSuccess(int callbackID, const Position& position);

    bool geolocationError(int callbackID, const PositionError& error);

    //C++->JS automated events

// [[[end]]]

/*
 * Auto Generated section for defining modules functions
 * The creator of the module needs to implement these functions
 */
// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\h.def.tmpl") ]]]
public:

    //To be implemented in the user's derived class
    virtual void getCurrentPosition(int callbackID, const GeolocationOptions& geolocationOptions) = 0;
    virtual void watchPosition(int callbackID, const GeolocationOptions& geolocationOptions, uint32 watchID) = 0;
    virtual void clearWatch(int callbackID, uint32 watchID) = 0;



// [[[end]]]

// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\h.end.tmpl") ]]]
};
// [[[end]]]
