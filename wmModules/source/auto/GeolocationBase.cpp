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
#include "GeolocationBase.h"


CGeolocationBase::CGeolocationBase(CIwWMDispatcherBase* dispatcher) :
    CIwWMDispatcherModule<CGeolocationBase>("geolocation", dispatcher)
{

    RegisterMember("getCurrentPosition", &CGeolocationBase::getCurrentPosition);
    RegisterMember("watchPosition", &CGeolocationBase::watchPosition);
    RegisterMember("clearWatch", &CGeolocationBase::clearWatch);
}

// [[[end]]]

/*
 * Auto Generated section for converting between JavaScript and C++
 */
// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\cpp.bridge.tmpl") ]]]

//JS->C++ automated converters
bool CGeolocationBase::getCurrentPosition(int callbackID, const Json::Value& parameters)
{
    Json::Value geolocationOptions_JSON;
    FetchObject(parameters, "geolocationOptions", geolocationOptions_JSON);

    GeolocationOptions geolocationOptions=GeolocationOptions::From_JSON(geolocationOptions_JSON, this);


    getCurrentPosition(callbackID, geolocationOptions);
    return true;
}

bool CGeolocationBase::watchPosition(int callbackID, const Json::Value& parameters)
{
    Json::Value geolocationOptions_JSON;
    FetchObject(parameters, "geolocationOptions", geolocationOptions_JSON);

    GeolocationOptions geolocationOptions=GeolocationOptions::From_JSON(geolocationOptions_JSON, this);


    Json::Value watchID_JSON;
    FetchObject(parameters, "watchID", watchID_JSON);

    uint32 watchID;
    FetchUInt(watchID_JSON, watchID);



    watchPosition(callbackID, geolocationOptions, watchID);
    return true;
}

bool CGeolocationBase::clearWatch(int callbackID, const Json::Value& parameters)
{
    Json::Value watchID_JSON;
    FetchObject(parameters, "watchID", watchID_JSON);

    uint32 watchID;
    FetchUInt(watchID_JSON, watchID);



    clearWatch(callbackID, watchID);
    return true;
}

CGeolocationBase::GeolocationOptions CGeolocationBase::GeolocationOptions::From_JSON(const Json::Value& value, CGeolocationBase* reader)
{
    Json::Value frequency_JSON;
    reader->FetchObject(value, "frequency", frequency_JSON);

    uint32 frequency;
    reader->FetchUInt(frequency_JSON, frequency);


    Json::Value enableHighAccuracy_JSON;
    reader->FetchObject(value, "enableHighAccuracy", enableHighAccuracy_JSON);

    bool enableHighAccuracy;
    reader->FetchBool(enableHighAccuracy_JSON, enableHighAccuracy);


    Json::Value timeout_JSON;
    reader->FetchObject(value, "timeout", timeout_JSON);

    uint32 timeout;
    reader->FetchUInt(timeout_JSON, timeout);


    Json::Value maximumAge_JSON;
    reader->FetchObject(value, "maximumAge", maximumAge_JSON);

    uint32 maximumAge;
    reader->FetchUInt(maximumAge_JSON, maximumAge);


    return CGeolocationBase::GeolocationOptions(frequency, enableHighAccuracy, timeout, maximumAge);
}

CGeolocationBase::Coordinates CGeolocationBase::Coordinates::From_JSON(const Json::Value& value, CGeolocationBase* reader)
{
    Json::Value latitude_JSON;
    reader->FetchObject(value, "latitude", latitude_JSON);

    double latitude;
    reader->FetchDouble(latitude_JSON, latitude);


    Json::Value longitude_JSON;
    reader->FetchObject(value, "longitude", longitude_JSON);

    double longitude;
    reader->FetchDouble(longitude_JSON, longitude);


    Json::Value altitude_JSON;
    reader->FetchObject(value, "altitude", altitude_JSON);

    double altitude;
    reader->FetchDouble(altitude_JSON, altitude);


    Json::Value accuracy_JSON;
    reader->FetchObject(value, "accuracy", accuracy_JSON);

    double accuracy;
    reader->FetchDouble(accuracy_JSON, accuracy);


    Json::Value altitudeAccuracy_JSON;
    reader->FetchObject(value, "altitudeAccuracy", altitudeAccuracy_JSON);

    double altitudeAccuracy;
    reader->FetchDouble(altitudeAccuracy_JSON, altitudeAccuracy);


    Json::Value heading_JSON;
    reader->FetchObject(value, "heading", heading_JSON);

    double heading;
    reader->FetchDouble(heading_JSON, heading);


    Json::Value speed_JSON;
    reader->FetchObject(value, "speed", speed_JSON);

    double speed;
    reader->FetchDouble(speed_JSON, speed);


    return CGeolocationBase::Coordinates(latitude, longitude, altitude, accuracy, altitudeAccuracy, heading, speed);
}

CGeolocationBase::PositionError CGeolocationBase::PositionError::From_JSON(const Json::Value& value, CGeolocationBase* reader)
{
    Json::Value code_JSON;
    reader->FetchObject(value, "code", code_JSON);

    PositionError::PositionErrorEnum code;
    reader->FetchUInt(code_JSON, *((unsigned int*)&code));


    Json::Value message_JSON;
    reader->FetchObject(value, "message", message_JSON);

    std::string message;
    reader->FetchString(message_JSON, message);


    return CGeolocationBase::PositionError(code, message.c_str());
}

CGeolocationBase::Position CGeolocationBase::Position::From_JSON(const Json::Value& value, CGeolocationBase* reader)
{
    Json::Value coords_JSON;
    reader->FetchObject(value, "coords", coords_JSON);

    Coordinates coords=Coordinates::From_JSON(coords_JSON, reader);

    Json::Value timestamp_JSON;
    reader->FetchObject(value, "timestamp", timestamp_JSON);

    uint64 timestamp;
    reader->FetchULong(timestamp_JSON, timestamp);


    return CGeolocationBase::Position(coords, timestamp);
}

//C++->JS automated event setup

//C++->JS automated callbacks
bool CGeolocationBase::geolocationSuccess(int callbackID, const Position& position)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    Json::Value position_JSON(Json::objectValue);
    position.To_JSON(position_JSON);
    params["position"]=position_JSON;


    SendJavaScript("geolocationSuccess", callbackID, params);
    return true;
}

bool CGeolocationBase::geolocationError(int callbackID, const PositionError& error)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    Json::Value error_JSON(Json::objectValue);
    error.To_JSON(error_JSON);
    params["error"]=error_JSON;


    SendJavaScript("geolocationError", callbackID, params);
    return true;
}

void CGeolocationBase::GeolocationOptions::To_JSON(Json::Value& value) const
{
    value["frequency"]=Json::Value(frequency);

    value["enableHighAccuracy"]=Json::Value(enableHighAccuracy);

    value["timeout"]=Json::Value(timeout);

    value["maximumAge"]=Json::Value(maximumAge);

}

void CGeolocationBase::Coordinates::To_JSON(Json::Value& value) const
{
    value["latitude"]=Json::Value(latitude);

    value["longitude"]=Json::Value(longitude);

    value["altitude"]=Json::Value(altitude);

    value["accuracy"]=Json::Value(accuracy);

    value["altitudeAccuracy"]=Json::Value(altitudeAccuracy);

    value["heading"]=Json::Value(heading);

    value["speed"]=Json::Value(speed);

}

void CGeolocationBase::PositionError::To_JSON(Json::Value& value) const
{
    value["code"]=Json::Value(code);

    value["message"]=Json::Value(message);

}

void CGeolocationBase::Position::To_JSON(Json::Value& value) const
{
    Json::Value coords_JSON(Json::objectValue);
    coords.To_JSON(coords_JSON);
    value["coords"]=coords_JSON;

    value["timestamp"]=Json::Value(timestamp);

}

//C++->JS automated events

// [[[end]]]
