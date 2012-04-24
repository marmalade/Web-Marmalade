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

#include "AccelerometerBase.h"

class CAccelerometer : public CAccelerometerBase
{
public:
    CAccelerometer(CIwWMDispatcherBase* dispatcher): CAccelerometerBase(dispatcher){}

    enum statusAccelerometer { WM_STATUS_ACC_OFF, WM_STATUS_ACC_ON, WM_STATUS_ACC_FAILED };    
    statusAccelerometer m_Status;
    
    void Init();
    void OnReady();
    void Shutdown();

    // methods
    virtual void getCurrentAcceleration(int callbackID);
    virtual void watchAcceleration(int callbackID, const AccelerometerOptions& accelerometerOptions, uint32 watchID);
    virtual void clearWatch(int callbackID, uint32 watchID);

    // Helpers
    bool CheckAccelerometerStatus();
    static void accelerometerCallback(CWMWatchData* pWatchData);
    static void getAccelValues(Acceleration& acc);
};
