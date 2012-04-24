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
#include "MediaBase.h"


CMediaBase::CMediaBase(CIwWMDispatcherBase* dispatcher) :
    CIwWMDispatcherModule<CMediaBase>("media", dispatcher)
{

    RegisterMember("Media_getCurrentPosition", &CMediaBase::Media_getCurrentPosition);
    RegisterMember("Media_play", &CMediaBase::Media_play);
    RegisterMember("Media_pause", &CMediaBase::Media_pause);
    RegisterMember("Media_stop", &CMediaBase::Media_stop);
    RegisterMember("Media_release", &CMediaBase::Media_release);
    RegisterMember("Media_seekTo", &CMediaBase::Media_seekTo);
    RegisterMember("Media_startRecord", &CMediaBase::Media_startRecord);
    RegisterMember("Media_stopRecord", &CMediaBase::Media_stopRecord);
    RegisterMember("Media_setVolume", &CMediaBase::Media_setVolume);
}

// [[[end]]]
/*
 * Auto Generated section for converting between JavaScript and C++
 */
// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\cpp.bridge.tmpl") ]]]

//JS->C++ automated converters
bool CMediaBase::Media_getCurrentPosition(int callbackID, const Json::Value& parameters)
{
    Json::Value self_JSON;
    FetchObject(parameters, "self", self_JSON);

    Media self=Media::From_JSON(self_JSON, this);


    Media_getCurrentPosition(callbackID, self);
    return true;
}

bool CMediaBase::Media_play(int callbackID, const Json::Value& parameters)
{
    Json::Value self_JSON;
    FetchObject(parameters, "self", self_JSON);

    Media self=Media::From_JSON(self_JSON, this);


    Media_play(callbackID, self);
    return true;
}

bool CMediaBase::Media_pause(int callbackID, const Json::Value& parameters)
{
    Json::Value self_JSON;
    FetchObject(parameters, "self", self_JSON);

    Media self=Media::From_JSON(self_JSON, this);


    Media_pause(callbackID, self);
    return true;
}

bool CMediaBase::Media_stop(int callbackID, const Json::Value& parameters)
{
    Json::Value self_JSON;
    FetchObject(parameters, "self", self_JSON);

    Media self=Media::From_JSON(self_JSON, this);


    Media_stop(callbackID, self);
    return true;
}

bool CMediaBase::Media_release(int callbackID, const Json::Value& parameters)
{
    Json::Value self_JSON;
    FetchObject(parameters, "self", self_JSON);

    Media self=Media::From_JSON(self_JSON, this);


    Media_release(callbackID, self);
    return true;
}

bool CMediaBase::Media_seekTo(int callbackID, const Json::Value& parameters)
{
    Json::Value self_JSON;
    FetchObject(parameters, "self", self_JSON);

    Media self=Media::From_JSON(self_JSON, this);


    Json::Value milliseconds_JSON;
    FetchObject(parameters, "milliseconds", milliseconds_JSON);

    int64 milliseconds;
    FetchLong(milliseconds_JSON, milliseconds);



    Media_seekTo(callbackID, self, milliseconds);
    return true;
}

bool CMediaBase::Media_startRecord(int callbackID, const Json::Value& parameters)
{
    Json::Value self_JSON;
    FetchObject(parameters, "self", self_JSON);

    Media self=Media::From_JSON(self_JSON, this);


    Media_startRecord(callbackID, self);
    return true;
}

bool CMediaBase::Media_stopRecord(int callbackID, const Json::Value& parameters)
{
    Json::Value self_JSON;
    FetchObject(parameters, "self", self_JSON);

    Media self=Media::From_JSON(self_JSON, this);


    Media_stopRecord(callbackID, self);
    return true;
}

bool CMediaBase::Media_setVolume(int callbackID, const Json::Value& parameters)
{
    Json::Value self_JSON;
    FetchObject(parameters, "self", self_JSON);

    Media self=Media::From_JSON(self_JSON, this);


    Json::Value volume_JSON;
    FetchObject(parameters, "volume", volume_JSON);

    double volume;
    FetchDouble(volume_JSON, volume);



    Media_setVolume(callbackID, self, volume);
    return true;
}

CMediaBase::MediaError CMediaBase::MediaError::From_JSON(const Json::Value& value, CMediaBase* reader)
{
    Json::Value code_JSON;
    reader->FetchObject(value, "code", code_JSON);

    int32 code;
    reader->FetchInt(code_JSON, code);


    Json::Value message_JSON;
    reader->FetchObject(value, "message", message_JSON);

    std::string message;
    reader->FetchString(message_JSON, message);


    return CMediaBase::MediaError(code, message.c_str());
}

CMediaBase::Media CMediaBase::Media::From_JSON(const Json::Value& value, CMediaBase* reader)
{
    Json::Value src_JSON;
    reader->FetchObject(value, "src", src_JSON);

    std::string src;
    reader->FetchString(src_JSON, src);


    Json::Value _src_JSON;
    reader->FetchObject(value, "_src", _src_JSON);

    std::string _src;
    reader->FetchString(_src_JSON, _src);


    Json::Value id_JSON;
    reader->FetchObject(value, "id", id_JSON);

    uint64 id;
    reader->FetchULong(id_JSON, id);


    Json::Value _duration_JSON;
    reader->FetchObject(value, "_duration", _duration_JSON);

    uint64 _duration;
    reader->FetchULong(_duration_JSON, _duration);


    Json::Value _position_JSON;
    reader->FetchObject(value, "_position", _position_JSON);

    uint64 _position;
    reader->FetchULong(_position_JSON, _position);


    Json::Value _status_JSON;
    reader->FetchObject(value, "_status", _status_JSON);

    uint64 _status;
    reader->FetchULong(_status_JSON, _status);


    return CMediaBase::Media(src.c_str(), _src.c_str(), id, _duration, _position, _status);
}

//C++->JS automated event setup

//C++->JS automated callbacks
bool CMediaBase::mediaPosSuccessCallback(int callbackID, uint64 position)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    params["position"]=Json::Value(position);


    SendJavaScript("mediaPosSuccessCallback", callbackID, params);
    return true;
}

bool CMediaBase::mediaErrorCallback(int callbackID, const MediaError& error)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    Json::Value error_JSON(Json::objectValue);
    error.To_JSON(error_JSON);
    params["error"]=error_JSON;


    SendJavaScript("mediaErrorCallback", callbackID, params);
    return true;
}

bool CMediaBase::_mediaSuccess(int callbackID)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);

    SendJavaScript("_mediaSuccess", callbackID, params);
    return true;
}

bool CMediaBase::_mediaPlaySuccess(int callbackID, bool isDone, uint64 duration)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    params["isDone"]=Json::Value(isDone);

    params["duration"]=Json::Value(duration);


    SendJavaScript("_mediaPlaySuccess", callbackID, params);
    return true;
}

void CMediaBase::MediaError::To_JSON(Json::Value& value) const
{
    value["code"]=Json::Value(code);

    value["message"]=Json::Value(message);

}

void CMediaBase::Media::To_JSON(Json::Value& value) const
{
    value["src"]=Json::Value(src);

    value["_src"]=Json::Value(_src);

    value["id"]=Json::Value(id);

    value["_duration"]=Json::Value(_duration);

    value["_position"]=Json::Value(_position);

    value["_status"]=Json::Value(_status);

}

//C++->JS automated events

// [[[end]]]
