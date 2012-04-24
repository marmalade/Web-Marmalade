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

#include "Camera.h"
#include "s3eCameraCapture.h"
#include "s3eImagePicker.h"
#include "IwImage.h"
#include "IwPath.h"
#include "IwDebug.h"
#include "s3eWebView.h"

//*
#if defined UNITTESTING
#include "mockCamera.h"
#define M(fn) fn##Mock
#else
#define M(fn) fn
#endif
/*/
#define M(fn) fn
//*/

#define DEFAULT_JPG_QUALITY 80

static bool DeviceIsWin()
{
    return (s3eDeviceOSID)s3eDeviceGetInt(S3E_DEVICE_OS) == S3E_OS_ID_WINDOWS;
}

void CCamera::Init()
{
    s3eFileList* list = s3eFileListDirectory(NULL);

    char buffer[256];

    while (s3eFileListNext(list, buffer, 256) != S3E_RESULT_ERROR)
    {
        if (!strcmp(buffer,"tmp://"))
            m_TempPath = "tmp://";
    }

    s3eFileListClose(list);

    if (m_TempPath == "") 
    {
        m_TempPath = "ram://tmp/";
        s3eFileMakeDirectory("ram://tmp");
    }
}

void CCamera::Shutdown()
{
    IwTrace(WM_MODULE_CAMERA, ("Camera: Shutdown."));
}

//C++ user init code
void CCamera::OnReady()
{
    IwTrace(WM_MODULE_CAMERA, ("Camera: OnReady."));
}
    
//C++ user code 

void CCamera::getPicture(int callbackID, const CameraOptions& cameraOptions)
{
    if (cameraOptions.destinationType == DATA_URL && cameraOptions.mediaType == VIDEO)
    {
        cameraError(callbackID, "Only pictures can be sent by data URL");
        return;
    }

    if (cameraOptions.sourceType == CAMERA)
        GetFromCamera(callbackID, cameraOptions);
    else
        GetFromLibary(callbackID, cameraOptions);
}

void CCamera::GetFromCamera(int callbackID, const CameraOptions& cameraOptions)
{
    bool iswin = DeviceIsWin();

    char filename[256];
    if (!M(s3eCameraCaptureAvailable)())
    {
        cameraError(callbackID, "Camera not available on this device");
        return;
    }

    s3eCameraCaptureFormat format = S3E_CAMERACAPTURE_FORMAT_JPG;
    if (cameraOptions.mediaType == VIDEO)
        format = S3E_CAMERACAPTURE_FORMAT_VIDEO;
    else if (cameraOptions.encodingType == PNG)
        format = S3E_CAMERACAPTURE_FORMAT_PNG;

    bool checkagain;
    bool forcemodify = false;
    do
    {
        checkagain = false;
        if (!M(s3eCameraCaptureIsFormatSupported)(format))
        {
            // formatted not support, so let's try
            if ((cameraOptions.mediaType == PICTURE) && (format == S3E_CAMERACAPTURE_FORMAT_PNG))
            {
                format = S3E_CAMERACAPTURE_FORMAT_JPG;
                checkagain = true;
                forcemodify = true;
            }
            else
            {
                cameraError(callbackID, "format is not supported");
                return;
            }
        }
    }
    while (checkagain);
    
    if (iswin)
        m_Dispatcher->ShowView(false);

    if (M(s3eCameraCaptureToFile)(filename, 256, format) == S3E_RESULT_ERROR)
    {
        if (iswin)
            m_Dispatcher->ShowView(true);

        cameraError(callbackID, "Failed to get image from camera.");
        return;
    }
    if (iswin)
        m_Dispatcher->ShowView(true);

    const char* error = Modify(cameraOptions, filename, forcemodify);
    if (error !=NULL)
    {
        cameraError(callbackID, error);
        return;
    }

    char buffer[256];
    s3eFileGetFileString(filename,S3E_FILE_REAL_PATH, buffer, 256);    

    cameraSuccess(callbackID, buffer);
}

void CCamera::GetFromLibary(int callbackID, const CameraOptions& cameraOptions)
{
    char filename[256];
    if (!M(s3eImagePickerAvailable)())
    {
        cameraError(callbackID, "image picker not available on this device");
        return;
    }

    s3eImagePickerFormat format = S3E_IMAGEPICKER_FORMAT_ANY;
    if (cameraOptions.mediaType == VIDEO)
        format = S3E_IMAGEPICKER_FORMAT_ANYVIDEO;
    else if (cameraOptions.mediaType == PICTURE)
    {
        if (cameraOptions.encodingType == PNG)
            format = S3E_IMAGEPICKER_FORMAT_PNG;
        else
            format = S3E_IMAGEPICKER_FORMAT_JPG;
    }
    
    int result = M(s3eImagePickerToFilePath)(format, 1);
    if (result <= 0)
    {
        if (result == 0)
        {
            cameraError(callbackID, "user cancelled");
        }
        else
        {
            const char* errorstr = s3eImagePickerGetErrorString();
            if (errorstr == NULL)
                errorstr = "Image Picker cancelled";
            cameraError(callbackID, errorstr);
        }
        return;
    }

    const s3eImagePickerResult* image = M(s3eImagePickerGetIndex)(0);
    strncpy(filename, (const char*)image->m_Buffer, 256);

    const char* error = Modify(cameraOptions, filename);
    if (error !=NULL)
    {
        cameraError(callbackID, error);
        return;
    }

    char buffer[256];
    s3eFileGetFileString(filename,S3E_FILE_REAL_PATH, buffer, 256);

    M(s3eImagePickerReset)();

    cameraSuccess(callbackID, buffer);
}

const char* CCamera::Modify(const CameraOptions& cameraOptions, char* filename, bool forcechange)
{
    // if quality does not match or targetHeight/targetWidth is specified load change and save
    bool change = forcechange;

    if (cameraOptions.encodingType == JPEG && cameraOptions.quality < DEFAULT_JPG_QUALITY)
        change = true;

    if (cameraOptions.targetHeight > 0 || cameraOptions.targetWidth > 0)
        change = true;

    if (!change)
        return NULL;

    CIwImage image;

    image.LoadFromFile(filename);
    
    int origRatio = image.GetWidth() / image.GetHeight();
    int newHeight = (cameraOptions.targetHeight > 0) ? cameraOptions.targetHeight : image.GetHeight();
    int newWidth = (cameraOptions.targetWidth > 0) ? cameraOptions.targetWidth : image.GetWidth();
    int newRatio = newWidth / newHeight;
    
    if (cameraOptions.targetHeight > 0 && cameraOptions.targetWidth > 0)
    {
        if (origRatio > newRatio)
            newHeight = newWidth * image.GetHeight() / image.GetWidth();
        else if (origRatio < newRatio)
            newWidth = newHeight * image.GetWidth() / image.GetHeight();
    }
    else if (cameraOptions.targetHeight > 0)
    {
        newWidth = newHeight * image.GetWidth() / image.GetHeight();
    }
    else if (cameraOptions.targetWidth > 0)
    {
        newHeight = newWidth * image.GetHeight() / image.GetWidth();
    }

    char buffer[256];
    strcpy(buffer, m_TempPath.c_str());
    IwPathGetFilename(filename, buffer + m_TempPath.size(), false);
    if (cameraOptions.encodingType == JPEG)
        strcat(buffer, ".jpg");
    else
        strcat(buffer, ".png");
    strcpy(filename, buffer);
        
    if ((int)image.GetHeight() != newHeight || (int)image.GetWidth() != newWidth)
    {
        CIwImage image2;

        image2.SetFormat(image.GetFormat());
        image2.SetHeight(newHeight);
        image2.SetWidth(newWidth);

        image.ConvertToImage(&image2);
        
        if (cameraOptions.encodingType == JPEG)
            image2.SaveJpg(filename, cameraOptions.quality);
        else
            image2.SavePng(filename);
    }
    else
    {
        if (cameraOptions.encodingType == JPEG)
            image.SaveJpg(filename, cameraOptions.quality);
        else
            image.SavePng(filename);
    }

    return NULL;
}
