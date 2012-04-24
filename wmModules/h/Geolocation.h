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

#include "GeolocationBase.h"

class CGeolocation : public CGeolocationBase
{
    enum statusGeolocation { WM_STATUS_GEO_OFF, WM_STATUS_GEO_ON, WM_STATUS_GEO_FAILED, WM_STATUS_GEO_UNSUPPORTED };
    statusGeolocation m_Status;   

    class CGeoWatchData : public CWMWatchData
    {
    public:
        bool enableHighAccuracy; // not supported in simulator
        uint32 timeout;    // not suported: not the same as our watch timeout
        uint32 maximumAge; // not suported

        CGeoWatchData(int watchID, int callbackID, unsigned int maxInterval, watchCB callbackFn,  unsigned int timeout = 0);
        virtual ~CGeoWatchData();
    };    

public:

    CGeolocation(CIwWMDispatcherBase* dispatcher) : CGeolocationBase(dispatcher), m_Status(WM_STATUS_GEO_OFF) {}

    virtual void Init();
    virtual void OnReady();
    virtual void Shutdown();

    virtual void getCurrentPosition(int callbackID, const GeolocationOptions& geolocationOptions);
    virtual void watchPosition(int callbackID, const GeolocationOptions& geolocationOptions, uint32 watchID);
    virtual void clearWatch(int callbackID, uint32 watchID);

    static void geolocationCallback(CWMWatchData* pWatchData);

    // Helpers
    bool CheckLocationStatus();
};
