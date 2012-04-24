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
 * Generated class for JavaScript bridge module media
 * Some sections of this module are re-generated when the interface changes the rest is generated once
 */


class CMediaBase : public CIwWMDispatcherModule<CMediaBase>
{
public:
    CMediaBase(CIwWMDispatcherBase* dispatcher);

    class MediaError
    {
    public:
        enum MediaErrorEnum
        {
            MEDIA_ERR_NONE_ACTIVE = 0,
            MEDIA_ERR_ABORTED = 1,
            MEDIA_ERR_NETWORK = 2,
            MEDIA_ERR_DECODE = 3,
            MEDIA_ERR_NONE_SUPPORTED = 4,
        };

    public:
        int32 code;
        std::string message;

        MediaError(int32 _code,const char* _message) : code(_code), message(_message) {}

        void To_JSON(Json::Value& value) const;
        static MediaError From_JSON(const Json::Value& value, CMediaBase* reader);
    };
    class Media
    {
    public:
        enum MediaEnum
        {
            MEDIA_NONE = 0,
            MEDIA_STARTING = 1,
            MEDIA_RUNNING = 2,
            MEDIA_PAUSED = 3,
            MEDIA_STOPPED = 4,
            MEDIA_STATE = 1,
            MEDIA_DURATION = 2,
            MEDIA_POSITION = 3,
            MEDIA_ERROR = 9,
        };

    public:
        std::string src;
        std::string _src;
        uint64 id;
        uint64 _duration;
        uint64 _position;
        uint64 _status;

        Media(const char* _src,const char* __src,uint64 _id,uint64 __duration,uint64 __position,uint64 __status) : src(_src), _src(__src), id(_id), _duration(__duration), _position(__position), _status(__status) {}

        void To_JSON(Json::Value& value) const;
        static Media From_JSON(const Json::Value& value, CMediaBase* reader);
    };

// [[[end]]]

/*
 * Auto Generated section for converting between JavaScript and C++
 */
// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\h.bridge.tmpl") ]]]
private:
    //JS->C++ automated converters
    bool Media_getCurrentPosition(int callbackID, const Json::Value& parameters);

    bool Media_play(int callbackID, const Json::Value& parameters);

    bool Media_pause(int callbackID, const Json::Value& parameters);

    bool Media_stop(int callbackID, const Json::Value& parameters);

    bool Media_release(int callbackID, const Json::Value& parameters);

    bool Media_seekTo(int callbackID, const Json::Value& parameters);

    bool Media_startRecord(int callbackID, const Json::Value& parameters);

    bool Media_stopRecord(int callbackID, const Json::Value& parameters);

    bool Media_setVolume(int callbackID, const Json::Value& parameters);

    //C++->JS automated event setup
public:
    //C++->JS automated callbacks
    bool mediaPosSuccessCallback(int callbackID, uint64 position);

    bool mediaErrorCallback(int callbackID, const MediaError& error);

    bool _mediaSuccess(int callbackID);

    bool _mediaPlaySuccess(int callbackID, bool isDone, uint64 duration);

    //C++->JS automated events

// [[[end]]]

/*
 * Auto Generated section for defining modules functions
 * The creator of the module needs to implement these functions
 */
// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\h.def.tmpl") ]]]
public:

    //To be implemented in the user's derived class

    virtual void Media_getCurrentPosition(int callbackID, const Media& self) = 0;
    virtual void Media_play(int callbackID, const Media& self) = 0;
    virtual void Media_pause(int callbackID, const Media& self) = 0;
    virtual void Media_stop(int callbackID, const Media& self) = 0;
    virtual void Media_release(int callbackID, const Media& self) = 0;
    virtual void Media_seekTo(int callbackID, const Media& self, int64 milliseconds) = 0;
    virtual void Media_startRecord(int callbackID, const Media& self) = 0;
    virtual void Media_stopRecord(int callbackID, const Media& self) = 0;
    virtual void Media_setVolume(int callbackID, const Media& self, double volume) = 0;


// [[[end]]]

// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\h.end.tmpl") ]]]
};
// [[[end]]]
