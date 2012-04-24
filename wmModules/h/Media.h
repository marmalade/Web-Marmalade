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

#include "MediaBase.h"
#include "s3eSoundRecord.h"
#include "s3eFile.h"
#include "s3eAudio.h"
#include "IwAudioCapture.h"

#define RUL_SIZE 10
#define AUDIO_BUFFER_SIZE S3E_SOUND_RECORD_MAX_SAMPLES * 2

class CMedia : public CMediaBase
{
    struct RecordState {
        uint64 m_ID;
        int m_CallbackID;
        IwAudioCapture* m_Audio;

        RecordState() : m_ID(-1), m_CallbackID(-1), m_Audio(NULL) {}
    };
public:
    CMedia(CIwWMDispatcherBase* dispatcher): CMediaBase(dispatcher){}
    
    void Init();
    void OnReady();
    void Shutdown();

    // methods
    virtual void Media_getCurrentPosition(int callbackID, const Media& self);
    virtual void Media_play(int callbackID, const Media& self);
    virtual void Media_pause(int callbackID, const Media& self);
    virtual void Media_stop(int callbackID, const Media& self);
    virtual void Media_release(int callbackID, const Media& self);
    virtual void Media_seekTo(int callbackID, const Media& self, int64 milliseconds);
    virtual void Media_startRecord(int callbackID, const Media& self);
    virtual void Media_stopRecord(int callbackID, const Media& self);
    virtual void Media_setVolume(int callbackID, const Media& self, double volume);
    virtual void play(int callbackID, const Media& self, int channel);

    // the stop callback should be a static member as it needs access to private members
    // of CMedia
    static int32 AudioHasStopped(s3eAudioCallbackData* systemData, CMedia* media);

public:
    int GetChannel(uint64 ID, bool create);
    void FreeChannel(uint64 ID);
    bool GetFile(const char* input, char* output, int outputSize, bool read = true);
    int GetCallbackID(int channel);
    void DoStopRecord();

private:
    uint64* m_ChannelToID;
    int* m_ChannelToCallbackID;
    int m_NumChannels;
    RecordState m_Record;
};
