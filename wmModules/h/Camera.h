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

#include "CameraBase.h"

class CCamera : public CCameraBase
{
public:
    CCamera(CIwWMDispatcherBase* dispatcher): CCameraBase(dispatcher){}
    
    void Init();
    void OnReady();
    void Shutdown();

    // methods
    virtual void getPicture(int callbackID, const CameraOptions& cameraOptions);

    void GetFromCamera(int callbackID, const CameraOptions& cameraOptions);
    void GetFromLibary(int callbackID, const CameraOptions& cameraOptions);
    const char* Modify(const CameraOptions& cameraOptions, char* filename, bool forcechange = false);
private:
    std::string m_TempPath;
};
