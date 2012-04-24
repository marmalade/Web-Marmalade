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
#include "CameraBase.h"


CCameraBase::CCameraBase(CIwWMDispatcherBase* dispatcher) :
    CIwWMDispatcherModule<CCameraBase>("camera", dispatcher)
{

    RegisterMember("getPicture", &CCameraBase::getPicture);
}

// [[[end]]]

/*
 * Auto Generated section for converting between JavaScript and C++
 */
// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\cpp.bridge.tmpl") ]]]

//JS->C++ automated converters
bool CCameraBase::getPicture(int callbackID, const Json::Value& parameters)
{
    Json::Value cameraOptions_JSON;
    FetchObject(parameters, "cameraOptions", cameraOptions_JSON);

    CameraOptions cameraOptions=CameraOptions::From_JSON(cameraOptions_JSON, this);


    getPicture(callbackID, cameraOptions);
    return true;
}

CCameraBase::CameraOptions CCameraBase::CameraOptions::From_JSON(const Json::Value& value, CCameraBase* reader)
{
    Json::Value quality_JSON;
    reader->FetchObject(value, "quality", quality_JSON);

    int32 quality;
    reader->FetchInt(quality_JSON, quality);


    Json::Value destinationType_JSON;
    reader->FetchObject(value, "destinationType", destinationType_JSON);

    DestinationType destinationType;
    reader->FetchUInt(destinationType_JSON, *((unsigned int*)&destinationType));


    Json::Value sourceType_JSON;
    reader->FetchObject(value, "sourceType", sourceType_JSON);

    PictureSourceType sourceType;
    reader->FetchUInt(sourceType_JSON, *((unsigned int*)&sourceType));


    Json::Value allowEdit_JSON;
    reader->FetchObject(value, "allowEdit", allowEdit_JSON);

    bool allowEdit;
    reader->FetchBool(allowEdit_JSON, allowEdit);


    Json::Value encodingType_JSON;
    reader->FetchObject(value, "encodingType", encodingType_JSON);

    EncodingType encodingType;
    reader->FetchUInt(encodingType_JSON, *((unsigned int*)&encodingType));


    Json::Value targetWidth_JSON;
    reader->FetchObject(value, "targetWidth", targetWidth_JSON);

    int32 targetWidth;
    reader->FetchInt(targetWidth_JSON, targetWidth);


    Json::Value targetHeight_JSON;
    reader->FetchObject(value, "targetHeight", targetHeight_JSON);

    int32 targetHeight;
    reader->FetchInt(targetHeight_JSON, targetHeight);


    Json::Value mediaType_JSON;
    reader->FetchObject(value, "mediaType", mediaType_JSON);

    MediaType mediaType;
    reader->FetchUInt(mediaType_JSON, *((unsigned int*)&mediaType));


    return CCameraBase::CameraOptions(quality, destinationType, sourceType, allowEdit, encodingType, targetWidth, targetHeight, mediaType);
}

//C++->JS automated event setup

//C++->JS automated callbacks
bool CCameraBase::cameraSuccess(int callbackID, const char* imageData)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    params["imageData"]=Json::Value(imageData);


    SendJavaScript("cameraSuccess", callbackID, params);
    return true;
}

bool CCameraBase::cameraError(int callbackID, const char* message)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    params["message"]=Json::Value(message);


    SendJavaScript("cameraError", callbackID, params);
    return true;
}

void CCameraBase::CameraOptions::To_JSON(Json::Value& value) const
{
    value["quality"]=Json::Value(quality);

    value["destinationType"]=Json::Value(destinationType);

    value["sourceType"]=Json::Value(sourceType);

    value["allowEdit"]=Json::Value(allowEdit);

    value["encodingType"]=Json::Value(encodingType);

    value["targetWidth"]=Json::Value(targetWidth);

    value["targetHeight"]=Json::Value(targetHeight);

    value["mediaType"]=Json::Value(mediaType);

}

//C++->JS automated events

// [[[end]]]
