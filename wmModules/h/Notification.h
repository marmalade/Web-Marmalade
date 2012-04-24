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

#include "NotificationBase.h"

class CNotification : public CNotificationBase
{
public:
    CNotification(CIwWMDispatcherBase* dispatcher): CNotificationBase(dispatcher){}
    
    void Init();
    void OnReady();
    void Shutdown();

    // methods
    virtual void alert(int callbackID, const char* message, const char* title, const char* buttonName);
    virtual void confirm(int callbackID, const char* message, const char* title, const char* buttonLabels);
    virtual void beep(int callbackID, int32 times);
    virtual void vibrate(int callbackID, int32 time);
};
