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

#include "CompassBase.h"

class CCompass : public CCompassBase
{
    class CCompassWatchData : public CWMWatchData
    {
    public:
        int filter;
        int lastSavedHeading;

        CCompassWatchData(int watchID, int callbackID, unsigned int maxInterval, watchCB callbackFn,  unsigned int timeout = 0);
        virtual ~CCompassWatchData();
    };

public:
    CCompass(CIwWMDispatcherBase* dispatcher): CCompassBase(dispatcher){}

    enum statusCompassE { WM_STATUS_COMP_OFF, WM_STATUS_COMP_ON, WM_STATUS_COMP_FAILED, WM_STATUS_COMP_UNSUPPORTED  };
    statusCompassE m_Status;

    void Init();
    void OnReady();
    void Shutdown();

    // Methods
    void getCurrentHeading(int callbackID, const CompassOptions& compassOptions);
    void watchHeading(int callbackID, const CompassOptions& compassOptions, uint32 watchID);
    void clearWatch(int callbackID, uint32 watchID);
    void watchHeadingFilter(int callbackID, const CompassOptions& compassOptions, uint32 watchID);
    void clearWatchFilter(int callbackID, uint32 watchID);

private:
    // Helpers
    bool CheckCompassStatus();
    static bool GetCompassValues(CompassHeading& heading);

    static void CompassCallback(CWMWatchData* pWatchData);
};
