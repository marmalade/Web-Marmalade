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
 * Generated class for JavaScript bridge module camera
 * Some sections of this module are re-generated when the interface changes the rest is generated once
 */


class CCameraBase : public CIwWMDispatcherModule<CCameraBase>
{
public:
    CCameraBase(CIwWMDispatcherBase* dispatcher);

    enum DestinationType
    {
        DATA_URL = 0,
        FILE_URI = 1,
    };

    enum PictureSourceType
    {
        PHOTOLIBRARY = 0,
        CAMERA = 1,
        SAVEDPHOTOALBUM = 2,
    };

    enum EncodingType
    {
        JPEG = 0,
        PNG = 1,
    };

    enum MediaType
    {
        PICTURE = 0,
        VIDEO = 1,
        ALLMEDIA = 2,
    };

    class CameraOptions
    {
    public:
    public:
        int32 quality;
        DestinationType destinationType;
        PictureSourceType sourceType;
        bool allowEdit;
        EncodingType encodingType;
        int32 targetWidth;
        int32 targetHeight;
        MediaType mediaType;

        CameraOptions(int32 _quality,DestinationType _destinationType,PictureSourceType _sourceType,bool _allowEdit,EncodingType _encodingType,int32 _targetWidth,int32 _targetHeight,MediaType _mediaType) : quality(_quality), destinationType(_destinationType), sourceType(_sourceType), allowEdit(_allowEdit), encodingType(_encodingType), targetWidth(_targetWidth), targetHeight(_targetHeight), mediaType(_mediaType) {}

        void To_JSON(Json::Value& value) const;
        static CameraOptions From_JSON(const Json::Value& value, CCameraBase* reader);
    };

// [[[end]]]

/*
 * Auto Generated section for converting between JavaScript and C++
 */
// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\h.bridge.tmpl") ]]]
private:
    //JS->C++ automated converters
    bool getPicture(int callbackID, const Json::Value& parameters);

    //C++->JS automated event setup
public:
    //C++->JS automated callbacks
    bool cameraSuccess(int callbackID, const char* imageData);

    bool cameraError(int callbackID, const char* message);

    //C++->JS automated events

// [[[end]]]

/*
 * Auto Generated section for defining modules functions
 * The creator of the module needs to implement these functions
 */
// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\h.def.tmpl") ]]]
public:

    //To be implemented in the user's derived class
    virtual void getPicture(int callbackID, const CameraOptions& cameraOptions) = 0;



// [[[end]]]

// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\h.end.tmpl") ]]]
};
// [[[end]]]
