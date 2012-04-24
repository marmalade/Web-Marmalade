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
 * Generated class for JavaScript bridge module capture
 * Some sections of this module are re-generated when the interface changes the rest is generated once
 */


class CCaptureBase : public CIwWMDispatcherModule<CCaptureBase>
{
public:
    CCaptureBase(CIwWMDispatcherBase* dispatcher);

    class ConfigurationData
    {
    public:
    public:
        std::string type;
        uint64 height;
        uint64 width;
        int32 _internal;

        ConfigurationData(const char* _type,uint64 _height,uint64 _width,int32 __internal) : type(_type), height(_height), width(_width), _internal(__internal) {}

        void To_JSON(Json::Value& value) const;
        static ConfigurationData From_JSON(const Json::Value& value, CCaptureBase* reader);
    };
    class CaptureAudioOptions
    {
    public:
    public:
        uint64 limit;
        uint64 duration;
        ConfigurationData mode;

        CaptureAudioOptions(uint64 _limit,uint64 _duration,const ConfigurationData& _mode) : limit(_limit), duration(_duration), mode(_mode) {}

        void To_JSON(Json::Value& value) const;
        static CaptureAudioOptions From_JSON(const Json::Value& value, CCaptureBase* reader);
    };
    class CaptureImageOptions
    {
    public:
    public:
        uint64 limit;
        ConfigurationData mode;

        CaptureImageOptions(uint64 _limit,const ConfigurationData& _mode) : limit(_limit), mode(_mode) {}

        void To_JSON(Json::Value& value) const;
        static CaptureImageOptions From_JSON(const Json::Value& value, CCaptureBase* reader);
    };
    class CaptureVideoOptions
    {
    public:
    public:
        uint64 limit;
        uint64 duration;
        ConfigurationData mode;

        CaptureVideoOptions(uint64 _limit,uint64 _duration,const ConfigurationData& _mode) : limit(_limit), duration(_duration), mode(_mode) {}

        void To_JSON(Json::Value& value) const;
        static CaptureVideoOptions From_JSON(const Json::Value& value, CCaptureBase* reader);
    };
    class MediaFileData
    {
    public:
    public:
        std::string codecs;
        uint64 bitrate;
        uint64 height;
        uint64 width;
        uint64 duration;

        MediaFileData(const char* _codecs,uint64 _bitrate,uint64 _height,uint64 _width,uint64 _duration) : codecs(_codecs), bitrate(_bitrate), height(_height), width(_width), duration(_duration) {}

        void To_JSON(Json::Value& value) const;
        static MediaFileData From_JSON(const Json::Value& value, CCaptureBase* reader);
    };
    class MediaFile
    {
    public:
    public:
        std::string name;
        std::string fullPath;
        std::string type;
        uint64 lastModifiedDate;
        uint64 size;

        MediaFile(const char* _name,const char* _fullPath,const char* _type,uint64 _lastModifiedDate,uint64 _size) : name(_name), fullPath(_fullPath), type(_type), lastModifiedDate(_lastModifiedDate), size(_size) {}

        void To_JSON(Json::Value& value) const;
        static MediaFile From_JSON(const Json::Value& value, CCaptureBase* reader);
    };
    class Capture
    {
    public:
    public:
        std::vector<ConfigurationData > supportedAudioModes;
        std::vector<ConfigurationData > supportedImageModes;
        std::vector<ConfigurationData > supportedVideoModes;

        Capture(const std::vector<ConfigurationData > _supportedAudioModes,const std::vector<ConfigurationData > _supportedImageModes,const std::vector<ConfigurationData > _supportedVideoModes) : supportedAudioModes(_supportedAudioModes), supportedImageModes(_supportedImageModes), supportedVideoModes(_supportedVideoModes) {}

        void To_JSON(Json::Value& value) const;
        static Capture From_JSON(const Json::Value& value, CCaptureBase* reader);
    };
    class CaptureError
    {
    public:
        enum CaptureErrorEnum
        {
            CAPTURE_INTERNAL_ERR = 0,
            CAPTURE_APPLICATION_BUSY = 1,
            CAPTURE_INVALID_ARGUMENT = 2,
            CAPTURE_NO_MEDIA_FILES = 3,
            CAPTURE_NOT_SUPPORTED = 20,
        };

    public:
        CaptureError::CaptureErrorEnum code;

        CaptureError(CaptureError::CaptureErrorEnum _code) : code(_code) {}

        void To_JSON(Json::Value& value) const;
        static CaptureError From_JSON(const Json::Value& value, CCaptureBase* reader);
    };

// [[[end]]]

/*
 * Auto Generated section for converting between JavaScript and C++
 */
// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\h.bridge.tmpl") ]]]
private:
    //JS->C++ automated converters
    bool MediaFile_getFormatData(int callbackID, const Json::Value& parameters);

    bool Capture_captureAudio(int callbackID, const Json::Value& parameters);

    bool Capture_captureImage(int callbackID, const Json::Value& parameters);

    bool Capture_captureVideo(int callbackID, const Json::Value& parameters);

    //C++->JS automated event setup
public:
    //C++->JS automated callbacks
    bool MediaFileDataSuccessCB(int callbackID, const MediaFileData& mediaFileObject);

    bool MediaFileDataErrorCB(int callbackID);

    bool CaptureCB(int callbackID, const std::vector<MediaFile > mediaFiles);

    bool CaptureErrorCB(int callbackID, const CaptureError& error);

    //C++->JS automated events

// [[[end]]]

/*
 * Auto Generated section for defining modules functions
 * The creator of the module needs to implement these functions
 */
// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\h.def.tmpl") ]]]
public:

    //To be implemented in the user's derived class

    virtual void MediaFile_getFormatData(int callbackID, const MediaFile& self) = 0;
    virtual void Capture_captureAudio(int callbackID, const Capture& self, const CaptureAudioOptions& options) = 0;
    virtual void Capture_captureImage(int callbackID, const Capture& self, const CaptureImageOptions& options) = 0;
    virtual void Capture_captureVideo(int callbackID, const Capture& self, const CaptureVideoOptions& options) = 0;


// [[[end]]]

// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\h.end.tmpl") ]]]
};
// [[[end]]]
