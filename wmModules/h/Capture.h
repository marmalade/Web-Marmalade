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

#include "CaptureBase.h"

class CCapture : public CCaptureBase
{
protected:
    enum {
        USE_S3E_AUDIO_CAPTURE = 0,
        USE_IW_AUDIO_CAPTURE = 1,
    };
public:
    CCapture(CIwWMDispatcherBase* dispatcher): CCaptureBase(dispatcher){}
    
    void Init();
    void OnReady();
    void Shutdown();

    // methods
    virtual void MediaFile_getFormatData(int callbackID, const MediaFile& self);
    virtual void Capture_captureAudio(int callbackID, const Capture& self, const CaptureAudioOptions& options);
    virtual void Capture_captureImage(int callbackID, const Capture& self, const CaptureImageOptions& options);
    virtual void Capture_captureVideo(int callbackID, const Capture& self, const CaptureVideoOptions& options);

protected:
    const char* CaptureAudioViaModule(int callbackID, CaptureError::CaptureErrorEnum& error, const CaptureAudioOptions& options);
    const char* CaptureAudioViaExtension(int callbackID, CaptureError::CaptureErrorEnum& error, const CaptureAudioOptions& options);
    void SendFile(int callbackID, const char* filename, const char* type);

protected:
    std::vector<ConfigurationData> audioFormats;
    std::vector<ConfigurationData> videoFormats;
    std::vector<ConfigurationData> imageFormats;
};
