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
#include "CaptureBase.h"


CCaptureBase::CCaptureBase(CIwWMDispatcherBase* dispatcher) :
    CIwWMDispatcherModule<CCaptureBase>("capture", dispatcher)
{

    RegisterMember("MediaFile_getFormatData", &CCaptureBase::MediaFile_getFormatData);
    RegisterMember("Capture_captureAudio", &CCaptureBase::Capture_captureAudio);
    RegisterMember("Capture_captureImage", &CCaptureBase::Capture_captureImage);
    RegisterMember("Capture_captureVideo", &CCaptureBase::Capture_captureVideo);
}

// [[[end]]]

/*
 * Auto Generated section for converting between JavaScript and C++
 */
// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\cpp.bridge.tmpl") ]]]

//JS->C++ automated converters
bool CCaptureBase::MediaFile_getFormatData(int callbackID, const Json::Value& parameters)
{
    Json::Value self_JSON;
    FetchObject(parameters, "self", self_JSON);

    MediaFile self=MediaFile::From_JSON(self_JSON, this);


    MediaFile_getFormatData(callbackID, self);
    return true;
}

bool CCaptureBase::Capture_captureAudio(int callbackID, const Json::Value& parameters)
{
    Json::Value self_JSON;
    FetchObject(parameters, "self", self_JSON);

    Capture self=Capture::From_JSON(self_JSON, this);


    Json::Value options_JSON;
    FetchObject(parameters, "options", options_JSON);

    CaptureAudioOptions options=CaptureAudioOptions::From_JSON(options_JSON, this);


    Capture_captureAudio(callbackID, self, options);
    return true;
}

bool CCaptureBase::Capture_captureImage(int callbackID, const Json::Value& parameters)
{
    Json::Value self_JSON;
    FetchObject(parameters, "self", self_JSON);

    Capture self=Capture::From_JSON(self_JSON, this);


    Json::Value options_JSON;
    FetchObject(parameters, "options", options_JSON);

    CaptureImageOptions options=CaptureImageOptions::From_JSON(options_JSON, this);


    Capture_captureImage(callbackID, self, options);
    return true;
}

bool CCaptureBase::Capture_captureVideo(int callbackID, const Json::Value& parameters)
{
    Json::Value self_JSON;
    FetchObject(parameters, "self", self_JSON);

    Capture self=Capture::From_JSON(self_JSON, this);


    Json::Value options_JSON;
    FetchObject(parameters, "options", options_JSON);

    CaptureVideoOptions options=CaptureVideoOptions::From_JSON(options_JSON, this);


    Capture_captureVideo(callbackID, self, options);
    return true;
}

CCaptureBase::ConfigurationData CCaptureBase::ConfigurationData::From_JSON(const Json::Value& value, CCaptureBase* reader)
{
    Json::Value type_JSON;
    reader->FetchObject(value, "type", type_JSON);

    std::string type;
    reader->FetchString(type_JSON, type);


    Json::Value height_JSON;
    reader->FetchObject(value, "height", height_JSON);

    uint64 height;
    reader->FetchULong(height_JSON, height);


    Json::Value width_JSON;
    reader->FetchObject(value, "width", width_JSON);

    uint64 width;
    reader->FetchULong(width_JSON, width);


    Json::Value _internal_JSON;
    reader->FetchObject(value, "_internal", _internal_JSON);

    int32 _internal;
    reader->FetchInt(_internal_JSON, _internal);


    return CCaptureBase::ConfigurationData(type.c_str(), height, width, _internal);
}

CCaptureBase::CaptureAudioOptions CCaptureBase::CaptureAudioOptions::From_JSON(const Json::Value& value, CCaptureBase* reader)
{
    Json::Value limit_JSON;
    reader->FetchObject(value, "limit", limit_JSON);

    uint64 limit;
    reader->FetchULong(limit_JSON, limit);


    Json::Value duration_JSON;
    reader->FetchObject(value, "duration", duration_JSON);

    uint64 duration;
    reader->FetchULong(duration_JSON, duration);


    Json::Value mode_JSON;
    reader->FetchObject(value, "mode", mode_JSON);

    ConfigurationData mode=ConfigurationData::From_JSON(mode_JSON, reader);

    return CCaptureBase::CaptureAudioOptions(limit, duration, mode);
}

CCaptureBase::CaptureImageOptions CCaptureBase::CaptureImageOptions::From_JSON(const Json::Value& value, CCaptureBase* reader)
{
    Json::Value limit_JSON;
    reader->FetchObject(value, "limit", limit_JSON);

    uint64 limit;
    reader->FetchULong(limit_JSON, limit);


    Json::Value mode_JSON;
    reader->FetchObject(value, "mode", mode_JSON);

    ConfigurationData mode=ConfigurationData::From_JSON(mode_JSON, reader);

    return CCaptureBase::CaptureImageOptions(limit, mode);
}

CCaptureBase::CaptureVideoOptions CCaptureBase::CaptureVideoOptions::From_JSON(const Json::Value& value, CCaptureBase* reader)
{
    Json::Value limit_JSON;
    reader->FetchObject(value, "limit", limit_JSON);

    uint64 limit;
    reader->FetchULong(limit_JSON, limit);


    Json::Value duration_JSON;
    reader->FetchObject(value, "duration", duration_JSON);

    uint64 duration;
    reader->FetchULong(duration_JSON, duration);


    Json::Value mode_JSON;
    reader->FetchObject(value, "mode", mode_JSON);

    ConfigurationData mode=ConfigurationData::From_JSON(mode_JSON, reader);

    return CCaptureBase::CaptureVideoOptions(limit, duration, mode);
}

CCaptureBase::MediaFileData CCaptureBase::MediaFileData::From_JSON(const Json::Value& value, CCaptureBase* reader)
{
    Json::Value codecs_JSON;
    reader->FetchObject(value, "codecs", codecs_JSON);

    std::string codecs;
    reader->FetchString(codecs_JSON, codecs);


    Json::Value bitrate_JSON;
    reader->FetchObject(value, "bitrate", bitrate_JSON);

    uint64 bitrate;
    reader->FetchULong(bitrate_JSON, bitrate);


    Json::Value height_JSON;
    reader->FetchObject(value, "height", height_JSON);

    uint64 height;
    reader->FetchULong(height_JSON, height);


    Json::Value width_JSON;
    reader->FetchObject(value, "width", width_JSON);

    uint64 width;
    reader->FetchULong(width_JSON, width);


    Json::Value duration_JSON;
    reader->FetchObject(value, "duration", duration_JSON);

    uint64 duration;
    reader->FetchULong(duration_JSON, duration);


    return CCaptureBase::MediaFileData(codecs.c_str(), bitrate, height, width, duration);
}

CCaptureBase::MediaFile CCaptureBase::MediaFile::From_JSON(const Json::Value& value, CCaptureBase* reader)
{
    Json::Value name_JSON;
    reader->FetchObject(value, "name", name_JSON);

    std::string name;
    reader->FetchString(name_JSON, name);


    Json::Value fullPath_JSON;
    reader->FetchObject(value, "fullPath", fullPath_JSON);

    std::string fullPath;
    reader->FetchString(fullPath_JSON, fullPath);


    Json::Value type_JSON;
    reader->FetchObject(value, "type", type_JSON);

    std::string type;
    reader->FetchString(type_JSON, type);


    Json::Value lastModifiedDate_JSON;
    reader->FetchObject(value, "lastModifiedDate", lastModifiedDate_JSON);

    uint64 lastModifiedDate;
    reader->FetchULong(lastModifiedDate_JSON, lastModifiedDate);


    Json::Value size_JSON;
    reader->FetchObject(value, "size", size_JSON);

    uint64 size;
    reader->FetchULong(size_JSON, size);


    return CCaptureBase::MediaFile(name.c_str(), fullPath.c_str(), type.c_str(), lastModifiedDate, size);
}

CCaptureBase::Capture CCaptureBase::Capture::From_JSON(const Json::Value& value, CCaptureBase* reader)
{
    Json::Value supportedAudioModes_JSON;
    reader->FetchObject(value, "supportedAudioModes", supportedAudioModes_JSON);

    std::vector<ConfigurationData > supportedAudioModes;
    for (int supportedAudioMode_i=0; supportedAudioMode_i<(int)supportedAudioModes_JSON.size(); supportedAudioMode_i++)
    {
    Json::Value supportedAudioMode_array;
    reader->FetchArray(supportedAudioModes_JSON, supportedAudioMode_i, supportedAudioMode_array);

    ConfigurationData supportedAudioMode=ConfigurationData::From_JSON(supportedAudioMode_array, reader);

        supportedAudioModes.push_back(supportedAudioMode);
    }


    Json::Value supportedImageModes_JSON;
    reader->FetchObject(value, "supportedImageModes", supportedImageModes_JSON);

    std::vector<ConfigurationData > supportedImageModes;
    for (int supportedImageMode_i=0; supportedImageMode_i<(int)supportedImageModes_JSON.size(); supportedImageMode_i++)
    {
    Json::Value supportedImageMode_array;
    reader->FetchArray(supportedImageModes_JSON, supportedImageMode_i, supportedImageMode_array);

    ConfigurationData supportedImageMode=ConfigurationData::From_JSON(supportedImageMode_array, reader);

        supportedImageModes.push_back(supportedImageMode);
    }


    Json::Value supportedVideoModes_JSON;
    reader->FetchObject(value, "supportedVideoModes", supportedVideoModes_JSON);

    std::vector<ConfigurationData > supportedVideoModes;
    for (int supportedVideoMode_i=0; supportedVideoMode_i<(int)supportedVideoModes_JSON.size(); supportedVideoMode_i++)
    {
    Json::Value supportedVideoMode_array;
    reader->FetchArray(supportedVideoModes_JSON, supportedVideoMode_i, supportedVideoMode_array);

    ConfigurationData supportedVideoMode=ConfigurationData::From_JSON(supportedVideoMode_array, reader);

        supportedVideoModes.push_back(supportedVideoMode);
    }


    return CCaptureBase::Capture(supportedAudioModes, supportedImageModes, supportedVideoModes);
}

CCaptureBase::CaptureError CCaptureBase::CaptureError::From_JSON(const Json::Value& value, CCaptureBase* reader)
{
    Json::Value code_JSON;
    reader->FetchObject(value, "code", code_JSON);

    CaptureError::CaptureErrorEnum code;
    reader->FetchUInt(code_JSON, *((unsigned int*)&code));


    return CCaptureBase::CaptureError(code);
}

//C++->JS automated event setup

//C++->JS automated callbacks
bool CCaptureBase::MediaFileDataSuccessCB(int callbackID, const MediaFileData& mediaFileObject)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    Json::Value mediaFileObject_JSON(Json::objectValue);
    mediaFileObject.To_JSON(mediaFileObject_JSON);
    params["mediaFileObject"]=mediaFileObject_JSON;


    SendJavaScript("MediaFileDataSuccessCB", callbackID, params);
    return true;
}

bool CCaptureBase::MediaFileDataErrorCB(int callbackID)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);

    SendJavaScript("MediaFileDataErrorCB", callbackID, params);
    return true;
}

bool CCaptureBase::CaptureCB(int callbackID, const std::vector<MediaFile > mediaFiles)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    Json::Value mediaFiles_JSON(Json::arrayValue);
    for (int mediaFile_i=0; mediaFile_i<(int)mediaFiles.size(); mediaFile_i++)
    {
    Json::Value mediaFile_JSON(Json::objectValue);
    mediaFiles[mediaFile_i].To_JSON(mediaFile_JSON);
    mediaFiles_JSON[mediaFile_i]=mediaFile_JSON;

    }
    params["mediaFiles"]=mediaFiles_JSON;


    SendJavaScript("CaptureCB", callbackID, params);
    return true;
}

bool CCaptureBase::CaptureErrorCB(int callbackID, const CaptureError& error)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    Json::Value error_JSON(Json::objectValue);
    error.To_JSON(error_JSON);
    params["error"]=error_JSON;


    SendJavaScript("CaptureErrorCB", callbackID, params);
    return true;
}

void CCaptureBase::ConfigurationData::To_JSON(Json::Value& value) const
{
    value["type"]=Json::Value(type);

    value["height"]=Json::Value(height);

    value["width"]=Json::Value(width);

    value["_internal"]=Json::Value(_internal);

}

void CCaptureBase::CaptureAudioOptions::To_JSON(Json::Value& value) const
{
    value["limit"]=Json::Value(limit);

    value["duration"]=Json::Value(duration);

    Json::Value mode_JSON(Json::objectValue);
    mode.To_JSON(mode_JSON);
    value["mode"]=mode_JSON;

}

void CCaptureBase::CaptureImageOptions::To_JSON(Json::Value& value) const
{
    value["limit"]=Json::Value(limit);

    Json::Value mode_JSON(Json::objectValue);
    mode.To_JSON(mode_JSON);
    value["mode"]=mode_JSON;

}

void CCaptureBase::CaptureVideoOptions::To_JSON(Json::Value& value) const
{
    value["limit"]=Json::Value(limit);

    value["duration"]=Json::Value(duration);

    Json::Value mode_JSON(Json::objectValue);
    mode.To_JSON(mode_JSON);
    value["mode"]=mode_JSON;

}

void CCaptureBase::MediaFileData::To_JSON(Json::Value& value) const
{
    value["codecs"]=Json::Value(codecs);

    value["bitrate"]=Json::Value(bitrate);

    value["height"]=Json::Value(height);

    value["width"]=Json::Value(width);

    value["duration"]=Json::Value(duration);

}

void CCaptureBase::MediaFile::To_JSON(Json::Value& value) const
{
    value["name"]=Json::Value(name);

    value["fullPath"]=Json::Value(fullPath);

    value["type"]=Json::Value(type);

    value["lastModifiedDate"]=Json::Value(lastModifiedDate);

    value["size"]=Json::Value(size);

}

void CCaptureBase::Capture::To_JSON(Json::Value& value) const
{
    Json::Value supportedAudioModes_JSON(Json::arrayValue);
    for (int supportedAudioMode_i=0; supportedAudioMode_i<(int)supportedAudioModes.size(); supportedAudioMode_i++)
    {
    Json::Value supportedAudioMode_JSON(Json::objectValue);
    supportedAudioModes[supportedAudioMode_i].To_JSON(supportedAudioMode_JSON);
    supportedAudioModes_JSON[supportedAudioMode_i]=supportedAudioMode_JSON;

    }
    value["supportedAudioModes"]=supportedAudioModes_JSON;

    Json::Value supportedImageModes_JSON(Json::arrayValue);
    for (int supportedImageMode_i=0; supportedImageMode_i<(int)supportedImageModes.size(); supportedImageMode_i++)
    {
    Json::Value supportedImageMode_JSON(Json::objectValue);
    supportedImageModes[supportedImageMode_i].To_JSON(supportedImageMode_JSON);
    supportedImageModes_JSON[supportedImageMode_i]=supportedImageMode_JSON;

    }
    value["supportedImageModes"]=supportedImageModes_JSON;

    Json::Value supportedVideoModes_JSON(Json::arrayValue);
    for (int supportedVideoMode_i=0; supportedVideoMode_i<(int)supportedVideoModes.size(); supportedVideoMode_i++)
    {
    Json::Value supportedVideoMode_JSON(Json::objectValue);
    supportedVideoModes[supportedVideoMode_i].To_JSON(supportedVideoMode_JSON);
    supportedVideoModes_JSON[supportedVideoMode_i]=supportedVideoMode_JSON;

    }
    value["supportedVideoModes"]=supportedVideoModes_JSON;

}

void CCaptureBase::CaptureError::To_JSON(Json::Value& value) const
{
    value["code"]=Json::Value(code);

}

//C++->JS automated events

// [[[end]]]
