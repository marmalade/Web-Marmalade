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
#include "wmModules.h"
#include "IwDebug.h"

#include "Accelerometer.h"
#include "Camera.h"
#include "ChildBrowser.h"
#include "Compass.h"
#include "Connection.h"
#include "Events.h"
#include "File.h"
#include "Geolocation.h"
#include "Media.h"
#include "Notification.h"
#include "Capture.h"

class CIwWMDispatcherWithCommon : public CIwWMDispatcher
{
public:
    CIwWMDispatcherWithCommon(s3eWebView* view) : CIwWMDispatcher(view) {}
    virtual ~CIwWMDispatcherWithCommon()
    {
        for (int i=0; i<(int)m_CommonModules.size(); i++)
        {
            m_CommonModules[i]->Shutdown();
            delete m_CommonModules[i];
        }
    }

public:
    void AddModule(CIwWMDispatcherModuleBase* module)
    {
        m_CommonModules.push_back(module);
        module->Init();
    }

private:
    std::vector<CIwWMDispatcherModuleBase*> m_CommonModules;
};

CIwWMDispatcher* IwWebBridgeCreate(s3eWebView* view)
{
    CIwWMDispatcherWithCommon* dispatcher = new CIwWMDispatcherWithCommon(view);

    dispatcher->AddModule((CIwWMDispatcherModuleBase*) new CAccelerometer(dispatcher));
    dispatcher->AddModule((CIwWMDispatcherModuleBase*) new CCamera(dispatcher));
    dispatcher->AddModule((CIwWMDispatcherModuleBase*) new CCapture(dispatcher));
    dispatcher->AddModule((CIwWMDispatcherModuleBase*) new CChildbrowser(dispatcher));
    dispatcher->AddModule((CIwWMDispatcherModuleBase*) new CCompass(dispatcher));
    dispatcher->AddModule((CIwWMDispatcherModuleBase*) new CConnection(dispatcher));
    dispatcher->AddModule((CIwWMDispatcherModuleBase*) new CEvents(dispatcher));
    dispatcher->AddModule((CIwWMDispatcherModuleBase*) new CFile(dispatcher));
    dispatcher->AddModule((CIwWMDispatcherModuleBase*) new CGeolocation(dispatcher));
    dispatcher->AddModule((CIwWMDispatcherModuleBase*) new CMedia(dispatcher));
    dispatcher->AddModule((CIwWMDispatcherModuleBase*) new CNotification(dispatcher));
    return dispatcher;
}

void IwWebBridgeShutdown(CIwWMDispatcher* dispatcher)
{
    delete dispatcher;
}
