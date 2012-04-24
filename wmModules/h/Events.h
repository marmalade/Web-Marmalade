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
#include "EventsBase.h"

class CEvents : public CEventsBase
{
public:
    CEvents(CIwWMDispatcherBase* dispatcher): CEventsBase(dispatcher) {}
    
    virtual void OnReady();
    virtual void Init();
    virtual void Shutdown();

    virtual void devicereadyController(bool isStart);
    virtual void pauseController(bool isStart); 
    virtual void resumeController(bool isStart);
    virtual void onlineController(bool isStart);
    virtual void offlineController(bool isStart);
    virtual void backbuttonController(bool isStart);
    virtual void batterycriticalController(bool isStart);
    virtual void batterylowController(bool isStart);
    virtual void batterystatusController(bool isStart);
    virtual void menubuttonController(bool isStart);
    virtual void searchbuttonController(bool isStart);
    virtual void startcallbuttonController(bool isStart);
    virtual void endcallbuttonController(bool isStart);
    virtual void volumedownbuttonController(bool isStart);
    virtual void volumeupbuttonController(bool isStart);


    //callbacks
    static void batteryCheck(CWMWatchData* pWatchData);
    
    static int32 OnlineCallback(void *systemData, CEvents *events);
    static int32 OfflineCallback(void *systemData, CEvents *events);
    static int32 PauseCallback(void* systemData, CEvents* events);
    static int32 ResumeCallback(void* systemData, CEvents* events);
    static int32 ButtonCallback(void* systemData, CEvents* events);

    //helpers
    void startButtonCallback(void);
    void endButtonCallback(void);
    void handleBatteryWatch(bool isStart);

    //guards
    int m_ButtonCount;
    bool m_VolumeButtonUpActive;
    bool m_VolumeButtonDownActive;
    bool m_BackButtonActive;
    bool m_MenuButtonActive;
    bool m_SearchButtonActive;
    bool m_BatteryCtlrStarted[3];
    bool m_HaveWatch; 

    bool m_ReportedCritical;
    bool m_ReportedLow;
    uint32 m_BatteryLevel;
    bool m_IsPlugged;
};
