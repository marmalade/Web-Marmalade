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

#if defined UNITTESTING
#include "mockDevice.h"
#define M(fn) fn##Mock
#else
#define M(fn) fn
#endif

#include "MarmaladeVersion.h"
#include "s3eDevice.h"
#include "s3eKeyboard.h"

#include "IwDebug.h"
#include "Events.h"

int32 CEvents::PauseCallback(void* systemData, CEvents* events)
{
    events->pause();
    return 0;
}

int32 CEvents::ResumeCallback(void* systemData, CEvents* events)
{
    events->resume();
    return 0;
}

int32 CEvents::OnlineCallback(void *systemData, CEvents *events)
{
    events->online();
    return 0;
}
int32 CEvents::OfflineCallback(void *systemData, CEvents *events)
{
    events->offline();
    return 0;
}

void CEvents::OnReady()
{
    IwTrace(WM_MODULE_EVENTS,("Events: OnReady."));
    
    // reset counters per deviceready (page load)
    m_ButtonCount = 0;
    m_VolumeButtonUpActive = false;
    m_VolumeButtonDownActive = false;
    m_BackButtonActive = false;
    m_MenuButtonActive = false;
    m_SearchButtonActive = false;
    m_HaveWatch = false;

    // and unregister all existing callbacks
    s3eDeviceUnRegister(S3E_DEVICE_PAUSE, (s3eCallback)PauseCallback);
    s3eDeviceUnRegister(S3E_DEVICE_UNPAUSE, (s3eCallback)ResumeCallback);
    s3eDeviceUnRegister(S3E_DEVICE_NETWORK_ONLINE, (s3eCallback)OnlineCallback);
    s3eKeyboardUnRegister(S3E_KEYBOARD_KEY_EVENT, (s3eCallback)ButtonCallback);
}    

void CEvents::Init()
{
    IwTrace(WM_MODULE_EVENTS,("Events: Init."));
}

void CEvents::Shutdown()
{
    IwTrace(WM_MODULE_EVENTS,("Events: Shutdown."));
}

void CEvents::devicereadyController(bool isStart)
{
    if (!isStart)
        return;

    m_Dispatcher->DoOnReady();
    
    // call deviceready
    // device.name
    std::string device_name = s3eDeviceGetString(S3E_DEVICE_ID);
    
    // device.phonegap TODO version constant
    std::string device_phonegap = "1.2";
    
    // device.platform
    std::string device_platform;
    
    s3eDeviceOSID osid = (s3eDeviceOSID) M(s3eDeviceGetInt)(S3E_DEVICE_OS);
    
    switch (osid)
    {
    case S3E_OS_ID_IPHONE:
        device_platform = "iPhone";
        break;
    case S3E_OS_ID_ANDROID:
        device_platform = "Android";
        break;
    case S3E_OS_ID_WINCE:
        device_platform = "WinCE";
        break;
    case S3E_OS_ID_QNX:
        device_platform = "Blackberry";
        break;
    case S3E_OS_ID_WEBOS:
        device_platform = "webOS";
        break;
    case S3E_OS_ID_WINDOWS:
        device_platform = "Windows";
        break;
    case S3E_OS_ID_OSX:
        device_platform = "OSX";
        break;
    default:
        device_platform = "unknown";
        break;
    }
    
    // device.uuid
    std::string device_uuid = s3eDeviceGetString(S3E_DEVICE_UNIQUE_ID);

    // device.version
    std::string device_version = s3eDeviceGetString(S3E_DEVICE_OS_VERSION);

    // device.marmalade (version)
    std::string device_marmalade = MARMALADE_VERSION_STRING;

    Device dev(device_name.c_str(), device_phonegap.c_str(), device_platform.c_str(), device_uuid.c_str(), device_version.c_str(), device_marmalade.c_str());
    deviceready(dev);
}

void CEvents::pauseController(bool isStart)
{
    if (isStart)
    {
        M(s3eDeviceRegister)(S3E_DEVICE_PAUSE, (s3eCallback)PauseCallback, this);
    }
    else
    {
        s3eDeviceUnRegister(S3E_DEVICE_PAUSE, (s3eCallback)PauseCallback);
    }
}

void CEvents::resumeController(bool isStart)
{
    if (isStart)
    {
        M(s3eDeviceRegister)(S3E_DEVICE_UNPAUSE, (s3eCallback)ResumeCallback, this);
    }
    else
    {
        s3eDeviceUnRegister(S3E_DEVICE_UNPAUSE, (s3eCallback)ResumeCallback);
    }
}

void CEvents::onlineController(bool isStart)
{
    if (isStart)
    {
        M(s3eDeviceRegister)(S3E_DEVICE_NETWORK_ONLINE, (s3eCallback)OnlineCallback, this);
    } else {
        s3eDeviceUnRegister(S3E_DEVICE_NETWORK_ONLINE, (s3eCallback)OnlineCallback);
    }
}

void CEvents::offlineController(bool isStart)
{
    if (isStart)
    {
        M(s3eDeviceRegister)(S3E_DEVICE_NETWORK_OFFLINE, (s3eCallback)OfflineCallback, this);
    } else {
        s3eDeviceUnRegister(S3E_DEVICE_NETWORK_OFFLINE, (s3eCallback)OfflineCallback);
    }
}

int32 CEvents::ButtonCallback(void* systemData, CEvents* events)
{
    s3eKeyboardEvent* keyEvent =  (s3eKeyboardEvent*)systemData;
    if (keyEvent->m_Pressed)
    {
        IwTrace(WM_MODULE_EVENTS, ("ButtonCallback fired with key press = %d", keyEvent->m_Key));
        switch(keyEvent->m_Key)
        {
        case s3eKeyBack:
            if (events->m_BackButtonActive)
            {
                IwTrace(WM_MODULE_EVENTS, ("Firing backbutton event"));
                events->backbutton();
            }
            break;
        case s3eKeyMenu:
            if (events->m_MenuButtonActive)
            {
                IwTrace(WM_MODULE_EVENTS, ("Firing menubutton event"));
                events->menubutton();
            }
            break;
        case s3eKeySearch:
            if (events->m_SearchButtonActive)
            {
                IwTrace(WM_MODULE_EVENTS, ("Firing searchbutton event"));
                events->searchbutton();
            }
            break;
        case s3eKeyVolUp:
            if (events->m_VolumeButtonUpActive)
            {
                IwTrace(WM_MODULE_EVENTS, ("Firing volumeup event"));
                events->volumeupbutton();
            }
            break;
        case s3eKeyVolDown:
            if (events->m_VolumeButtonDownActive)
            {
                IwTrace(WM_MODULE_EVENTS, ("Firing volumedown event"));
                events->volumedownbutton();
            }
            break;
        default:
            break;
        }
    }
    return 0;
}

void CEvents::startButtonCallback()
{
    IwTrace(WM_MODULE_EVENTS,("startButtonCallback, button count: %d", this->m_ButtonCount));
    M(s3eKeyboardRegister)(S3E_KEYBOARD_KEY_EVENT, (s3eCallback)ButtonCallback, this);
}

void CEvents::endButtonCallback()
{
    IwTrace(WM_MODULE_EVENTS,("endButtonCallback"));
    s3eKeyboardUnRegister(S3E_KEYBOARD_KEY_EVENT, (s3eCallback)ButtonCallback);
}

void CEvents::volumedownbuttonController(bool isStart)
{
    if (isStart)
    {
        IwAssertMsg(WM_MODULE_EVENTS, !m_VolumeButtonDownActive, ("Volume down button callback started when already started"));
        m_VolumeButtonDownActive = true;
        if (!m_ButtonCount)
            startButtonCallback();
        ++m_ButtonCount;
    }
    else
    {
        IwAssertMsg(WM_MODULE_EVENTS, m_VolumeButtonDownActive, ("Volume down button callback terminated when not started"));
        m_VolumeButtonDownActive = false;
        --m_ButtonCount;
        if (m_ButtonCount != 1)
            endButtonCallback();
        --m_ButtonCount;
    }
}

void CEvents::volumeupbuttonController(bool isStart)
{
    if (isStart)
    {
        IwAssertMsg(WM_MODULE_EVENTS, !m_VolumeButtonUpActive, ("Volume up button callback started when already started"));
        m_VolumeButtonUpActive = true;
        if (!m_ButtonCount)
            startButtonCallback();
        ++m_ButtonCount;
    }
    else
    {
        IwAssertMsg(WM_MODULE_EVENTS, m_VolumeButtonUpActive, ("Volume up button callback terminated when not started"));
        m_VolumeButtonUpActive = false;
        --m_ButtonCount;
        if (m_ButtonCount != 1)
            endButtonCallback();
        --m_ButtonCount;
    }
}

void CEvents::backbuttonController(bool isStart)
{
    if (isStart)
    {
        IwAssertMsg(WM_MODULE_EVENTS, !m_BackButtonActive, ("Back button callback started when already started"));
        m_BackButtonActive = true;
        if (!m_ButtonCount)
            startButtonCallback();
        ++m_ButtonCount;
    }
    else
    {
        IwAssertMsg(WM_MODULE_EVENTS, m_BackButtonActive, ("Back button callback terminated when not started"));
        m_BackButtonActive = false;
        --m_ButtonCount;
        if (m_ButtonCount != 1)
            endButtonCallback();
        --m_ButtonCount;
    }
}

void CEvents::menubuttonController(bool isStart)
{
    if (isStart)
    {
        IwAssertMsg(WM_MODULE_EVENTS, !m_MenuButtonActive, ("Menu button callback started when already started"));
        m_MenuButtonActive = true;
        if (!m_ButtonCount)
            startButtonCallback();
        ++m_ButtonCount;
    }
    else
    {
        IwAssertMsg(WM_MODULE_EVENTS, m_MenuButtonActive, ("Menu button callback terminated when not started"));
        m_MenuButtonActive = false;
        --m_ButtonCount;
        if (m_ButtonCount != 1)
            endButtonCallback();
        --m_ButtonCount;
    }
}

void CEvents::searchbuttonController(bool isStart)
{
    if (isStart)
    {
        IwAssertMsg(WM_MODULE_EVENTS, !m_SearchButtonActive, ("Search button callback started when already started"));
        m_SearchButtonActive = true;
        if (!m_ButtonCount)
            startButtonCallback();
        ++m_ButtonCount;
    }
    else
    {
        IwAssertMsg(WM_MODULE_EVENTS, m_SearchButtonActive, ("Search button callback terminated when not started"));
        m_SearchButtonActive = false;
        --m_ButtonCount;
        if (m_ButtonCount != 1)
            endButtonCallback();
        --m_ButtonCount;
    }
}

void CEvents::startcallbuttonController(bool isStart)
{
}

void CEvents::endcallbuttonController(bool isStart)
{
}


//-- Battery Events

#define BATTERY_CRITICAL_EVENT  0
#define BATTERY_LOW_EVENT       1
#define BATTERY_STATUS_EVENT    2


void CEvents::batteryCheck(CWMWatchData* pWatchData)
{
    CEvents* eventsModule = (CEvents*) pWatchData->m_Module;

    IwAssertMsg(WM_MODULE_EVENTS, eventsModule, ("batteryCheck: watch data is NULL"));

    BatteryInfo info( M(s3eDeviceGetInt)(S3E_DEVICE_BATTERY_LEVEL) , 
                                       M(s3eDeviceGetInt)(S3E_DEVICE_MAINS_POWER)!=0);

    if (eventsModule->m_BatteryCtlrStarted[BATTERY_CRITICAL_EVENT] && !eventsModule->m_ReportedCritical && info.level < 10)
    {
        eventsModule->batterycritical(info);
        eventsModule->m_ReportedCritical = true;
    }
    else if (eventsModule->m_BatteryCtlrStarted[BATTERY_LOW_EVENT] && !eventsModule->m_ReportedLow && info.level < 30)
    {
        eventsModule->batterylow(info);
        eventsModule->m_ReportedLow = true;
    }

    if (eventsModule->m_BatteryCtlrStarted[BATTERY_STATUS_EVENT] && 
        (eventsModule->m_BatteryLevel != info.level || eventsModule->m_IsPlugged != info.isPlugged) )
    {
        eventsModule->m_BatteryLevel = info.level;
        eventsModule->m_IsPlugged = info.isPlugged;
        eventsModule->batterystatus(info);
    }
}    

#define BATTERY_WATCH_ID 1000000

#define BATTERY_WATCH_INTERVAL 500

void CEvents::handleBatteryWatch(bool isStart)
{
    //static bool haveWatch = false;    
    if (isStart && !m_HaveWatch)
    {
        CWMWatchData* watchData = new CWMWatchData(BATTERY_WATCH_ID, 0, BATTERY_WATCH_INTERVAL, batteryCheck);
        AddWatch(watchData);
        m_HaveWatch = true;
        m_ReportedCritical = false;
        m_ReportedLow = false;
        m_BatteryLevel = 100;
        m_IsPlugged = false;
    }
    else if (!isStart && !m_BatteryCtlrStarted[BATTERY_CRITICAL_EVENT] 
            && !m_BatteryCtlrStarted[BATTERY_LOW_EVENT] && !m_BatteryCtlrStarted[BATTERY_STATUS_EVENT])
    {
        m_Dispatcher->RemoveWatch(BATTERY_WATCH_ID);
        m_HaveWatch = false;
    }
}

void CEvents::batterycriticalController(bool isStart)
{
    m_BatteryCtlrStarted[BATTERY_CRITICAL_EVENT] = isStart;
    handleBatteryWatch(isStart);
}

void CEvents::batterylowController(bool isStart)
{
    m_BatteryCtlrStarted[BATTERY_LOW_EVENT] = isStart;
    handleBatteryWatch(isStart);
}

void CEvents::batterystatusController(bool isStart)
{
    m_BatteryCtlrStarted[BATTERY_STATUS_EVENT] = isStart;
    handleBatteryWatch(isStart);
}
