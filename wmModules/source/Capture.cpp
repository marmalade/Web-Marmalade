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

#include "Capture.h"
#include "IwAudioCapture.h"
#include "s3eDevice.h"
#include "s3eFile.h"
#include "IwGx.h"
#include "IwPath.h"
#include "s3eCameraCapture.h"
#include "s3eAudioCapture.h"
#include "s3eAudio.h"
#include "IwNUI.h"

//*
#if defined UNITTESTING
#include "mockCapture.h"
#define M(fn) fn##MockCapture
#else
#define M(fn) fn
#endif
/*/
#define M(fn) fn
//*/

static std::string StripPrefix(std::string inputpath)
{
    char realpath[256];

    s3eFileGetFileString(inputpath.c_str(), S3E_FILE_REAL_PATH, realpath, 256);

    std::string rawpath(realpath);

    int pos = rawpath.find("raw://");
    if (pos < 0)
        pos = rawpath.find("file://");

    if (pos == 0)
        return std::string("") + rawpath.substr(6);

    return rawpath;
}

static bool DeviceIsWin()
{
    return (s3eDeviceOSID)s3eDeviceGetInt(S3E_DEVICE_OS) == S3E_OS_ID_WINDOWS;
}

static bool DeviceIsAndroid()
{
    return (s3eDeviceOSID)s3eDeviceGetInt(S3E_DEVICE_OS) == S3E_OS_ID_ANDROID;
}

void CCapture::Init()
{
    if (s3eAudioCaptureAvailable())
    {
        IwTrace(WM_MODULE_CAPTURE, ("Using s3eAudioCapture."));
        audioFormats.push_back(ConfigurationData("audio/3gpp",0,0,USE_S3E_AUDIO_CAPTURE));
    }
    else
    {
        IwTrace(WM_MODULE_CAPTURE, ("Using IwAudioCapture."));
        audioFormats.push_back(ConfigurationData("audio/wav",0,0,USE_IW_AUDIO_CAPTURE));
    }

    if (!M(s3eCameraCaptureAvailable)())
    {
        IwTrace(WM_MODULE_CAPTURE, ("s3eCameraCapture Unavailable."));
        return;
    }

    if (DeviceIsAndroid())
    {
        if (s3eCameraCaptureIsFormatSupported(S3E_CAMERACAPTURE_FORMAT_VIDEO))
            videoFormats.push_back(ConfigurationData("video/3gpp",0,0,S3E_CAMERACAPTURE_FORMAT_VIDEO));
    }
    else
    {
        if (s3eCameraCaptureIsFormatSupported(S3E_CAMERACAPTURE_FORMAT_VIDEO))
            videoFormats.push_back(ConfigurationData("video/quicktime",0,0,S3E_CAMERACAPTURE_FORMAT_VIDEO));
    }

    if (s3eCameraCaptureIsFormatSupported(S3E_CAMERACAPTURE_FORMAT_JPG))
        imageFormats.push_back(ConfigurationData("image/jpeg",0,0,S3E_CAMERACAPTURE_FORMAT_JPG));
    if (s3eCameraCaptureIsFormatSupported(S3E_CAMERACAPTURE_FORMAT_PNG))
        imageFormats.push_back(ConfigurationData("image/png",0,0,S3E_CAMERACAPTURE_FORMAT_PNG));
    if (s3eCameraCaptureIsFormatSupported(S3E_CAMERACAPTURE_FORMAT_BMP))
        imageFormats.push_back(ConfigurationData("image/bmp",0,0,S3E_CAMERACAPTURE_FORMAT_BMP));
}

void CCapture::Shutdown()
{
}

//C++ user init code
void CCapture::OnReady()
{
    IwTrace(WM_MODULE_NOTIFICATION, ("Capture: OnReady."));
}

//C++ user code
void CCapture::MediaFile_getFormatData(int callbackID, const MediaFile& self)
{
    std::string filename = "raw://" + self.fullPath;

    if (!s3eFileCheckExists(filename.c_str()))
    {
        MediaFileDataErrorCB(callbackID);
        return;
    }

    MediaFileData data = MediaFileData("",0,0,0,0);

    if (self.type.find("audio") != std::string::npos)
    {
        s3eAudioPlay(filename.c_str());
        s3eAudioPause();
        data.duration = s3eAudioGetInt(S3E_AUDIO_DURATION);
        s3eAudioStop();
    }
    else if (self.type.find("video") != std::string::npos)
    {
    }
    else if (self.type.find("image") != std::string::npos)
    {
        CIwImage image;
        image.LoadFromFile(filename.c_str());
        data.width = image.GetWidth();
        data.height = image.GetHeight();
    }

    MediaFileDataSuccessCB(callbackID, data);
}

const char* CCapture::CaptureAudioViaModule(int callbackID, CaptureError::CaptureErrorEnum& error, const CaptureAudioOptions& options)
{
    static char filename[256] = "temp.wav";
    bool iswin = DeviceIsWin();

    if (iswin)
        m_Dispatcher->ShowView(false);

    IwAudioCaptureResult running = M(IwAudioCaptureWithDialog)(filename, (int)options.duration);

    // This surface show will clear the residual image left on the surface from NUI.
    s3eSurfaceClear(0, 0, 0);
    s3eSurfaceShow();

    if (iswin)
        m_Dispatcher->ShowView(true);

    if (running == IW_AUDIO_CAPTURE_RESULT_SUCCESS)
        return filename;

    if (running == IW_AUDIO_CAPTURE_RESULT_CANCELLED)
    {
        error = CaptureError::CAPTURE_NO_MEDIA_FILES;
        return NULL;
    }
    switch (M(IwAudioCaptureGetError)())
    {
        case IW_AUDIO_CAPTURE_ERR_NOT_SUPPORTED:
            error = CaptureError::CAPTURE_NOT_SUPPORTED;
            break;
        case IW_AUDIO_CAPTURE_ERR_ABORTED:
            error = CaptureError::CAPTURE_INTERNAL_ERR;
            break;
        default:
            break;
    }
    return NULL;
}

void CCapture::SendFile(int callbackID, const char* filename, const char* type)
{
    char name[256];

    std::string filepath = StripPrefix(filename);

    IwPathGetFilename(filepath.c_str(), name, true);
    uint64 time = s3eFileGetLastWriteTime(filename);
    uint64 size = s3eFileGetFileInt(filename, S3E_FILE_SIZE);

    MediaFile file(name, filepath.c_str(), type, time, size);

    std::vector<MediaFile> list;
    list.push_back(file);

    CaptureCB(callbackID, list);
}

const char* CCapture::CaptureAudioViaExtension(int callbackID, CaptureError::CaptureErrorEnum& error, const CaptureAudioOptions& options)
{
    static char buffer[256];

    if (M(s3eAudioCaptureToFile)(buffer, 256) != S3E_RESULT_ERROR)
        return buffer;

    switch (M(s3eAudioCaptureGetError)())
    {
        case S3E_AUDIOCAPTURE_ERR_PARAM:
        case S3E_AUDIOCAPTURE_ERR_NOT_FOUND:
        case S3E_AUDIOCAPTURE_ERR_ACCESS:
            error = CaptureError::CAPTURE_INVALID_ARGUMENT;
            break;
        case S3E_AUDIOCAPTURE_ERR_UNAVAIL:
            error = CaptureError::CAPTURE_APPLICATION_BUSY;
            break;
        case S3E_AUDIOCAPTURE_ERR_UNSUPPORTED:
            error = CaptureError::CAPTURE_NOT_SUPPORTED;
            break;
        default:
            error = CaptureError::CAPTURE_INTERNAL_ERR;
            break;
    }
    return NULL;
}

void CCapture::Capture_captureAudio(int callbackID, const Capture& self, const CaptureAudioOptions& options)
{
    CaptureError::CaptureErrorEnum error = CaptureError::CAPTURE_NOT_SUPPORTED;
    const char* filename = NULL;
    const char* type = "";

    for (int i=0; i<(int)audioFormats.size(); i++)
    {
        if (audioFormats[i].type == options.mode.type || (i==0 && options.mode.type == ""))
        {
            if (audioFormats[i]._internal == USE_S3E_AUDIO_CAPTURE)
                filename = CaptureAudioViaExtension(callbackID, error, options);
            else
                filename = CaptureAudioViaModule(callbackID, error, options);
            type = audioFormats[i].type.c_str();
            break;
        }
    }

    if (filename != NULL)
    {
        SendFile(callbackID, filename, type);
    }
    else
    {
        CaptureErrorCB(callbackID, CaptureError(error));
    }
}

void CCapture::Capture_captureImage(int callbackID, const Capture& self, const CaptureImageOptions& options)
{
    bool iswin = DeviceIsWin();
    CaptureError::CaptureErrorEnum error = (CaptureError::CaptureErrorEnum)-1;

    char filename[256];
    if (!M(s3eCameraCaptureAvailable)())
    {
        CaptureErrorCB(callbackID, CaptureError(CaptureError::CAPTURE_NOT_SUPPORTED));
        return;
    }

    for (int i=0; i<(int)imageFormats.size(); i++)
        if (imageFormats[i].type == options.mode.type || (i==0 && options.mode.type == ""))
        {
            if (iswin)
                m_Dispatcher->ShowView(false);

            if (M(s3eCameraCaptureToFile)(filename, 256, (s3eCameraCaptureFormat)imageFormats[i]._internal) == S3E_RESULT_ERROR)
            {
                IwTrace(WM_MODULE_CAPTURE, ("Camera capture type: %d", (int)imageFormats[i]._internal));
                error = CaptureError::CAPTURE_INTERNAL_ERR;
            }

            if (iswin)
                m_Dispatcher->ShowView(true);

            if (error == (CaptureError::CaptureErrorEnum)-1)
                SendFile(callbackID, filename, imageFormats[i].type.c_str());
            else
                CaptureErrorCB(callbackID, CaptureError(error));

            return;
        }

    CaptureErrorCB(callbackID, CaptureError(CaptureError::CAPTURE_NOT_SUPPORTED));
}

void CCapture::Capture_captureVideo(int callbackID, const Capture& self, const CaptureVideoOptions& options)
{
    bool iswin = DeviceIsWin();
    CaptureError::CaptureErrorEnum error = (CaptureError::CaptureErrorEnum)-1;

    char filename[256];
    if (!M(s3eCameraCaptureAvailable)())
    {
        CaptureErrorCB(callbackID, CaptureError(CaptureError::CAPTURE_NOT_SUPPORTED));
        return;
    }

    for (int i=0; i<(int)videoFormats.size(); i++)
        if (videoFormats[i].type == options.mode.type || (i==0 && options.mode.type == ""))
        {
            if (iswin)
                m_Dispatcher->ShowView(false);

            if (M(s3eCameraCaptureToFile)(filename, 256, (s3eCameraCaptureFormat)videoFormats[i]._internal) == S3E_RESULT_ERROR)
                error = CaptureError::CAPTURE_INTERNAL_ERR;

            if (iswin)
                m_Dispatcher->ShowView(true);

            if (error == (CaptureError::CaptureErrorEnum)-1)
                SendFile(callbackID, filename, videoFormats[i].type.c_str());
            else
                CaptureErrorCB(callbackID, CaptureError(error));

            return;
        }

    CaptureErrorCB(callbackID, CaptureError(CaptureError::CAPTURE_NOT_SUPPORTED));
}
