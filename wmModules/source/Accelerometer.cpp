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
#include "mockAccelerometer.h"
#define M(fn) fn##Mock
#else
#define M(fn) fn
#endif

#include "s3eAccelerometer.h"

#include "s3eTimer.h"
#include "IwDebug.h"
#include "Accelerometer.h"

void CAccelerometer::Init()
{
    IwTrace(WM_MODULE_ACCEL,("Accelerometer: Init"));
    m_Status = WM_STATUS_ACC_OFF;
}

void CAccelerometer::Shutdown()
{
    IwTrace(WM_MODULE_ACCEL, ("Accelerometer: Shutdown."));

        M(s3eAccelerometerStop)();

    // remove watches from dispatcher belonging to this module
    RemoveWatches();
}

//C++ user init code
void CAccelerometer::OnReady()
{
    IwTrace(WM_MODULE_ACCEL, ("Accelerometer: OnReady."));
}

bool CAccelerometer::CheckAccelerometerStatus()
{
    if (!s3eAccelerometerGetInt(S3E_ACCELEROMETER_AVAILABLE))
    {
        m_Status = WM_STATUS_ACC_OFF;
        return false;
    }

    if (m_Status != WM_STATUS_ACC_ON)
    {
        if (M(s3eAccelerometerStart)() == S3E_RESULT_ERROR)
        {
            m_Status = WM_STATUS_ACC_FAILED;
            return false;
        }
        else
        {
            m_Status = WM_STATUS_ACC_ON;
            return true;
        }
    }
    return true;
}


void CAccelerometer::getAccelValues(Acceleration& acc)
{
    int64 timestamp = s3eTimerGetUTC();
    // Cap and scale to [-1,1]
    double x = ((double) M(s3eAccelerometerGetX)()) / 1000.0f;
    double y = ((double) M(s3eAccelerometerGetY)()) / 1000.0f;
    double z = ((double) M(s3eAccelerometerGetZ)()) / 1000.0f;
    x = x <- 1 ? -1 : x;  x = x > 1 ? 1 : x;
    y = y <- 1 ? -1 : y;  y = y > 1 ? 1 : y;
    z = z <- 1 ? -1 : z;  z = z > 1 ? 1 : z;
    acc = Acceleration(x, y, z, timestamp);
}

void CAccelerometer::getCurrentAcceleration(int callbackID)
{
    IwTrace(WM_MODULE_ACCEL_VERBOSE,("getCurrentAcceleration (callbackID: %d)", callbackID));
    if (CheckAccelerometerStatus())
    {
        Acceleration acc(0.0f, 0.0f, 0.0f, 0);
        getAccelValues(acc);
        accelerometerSuccess(callbackID, acc);
    }
    else
    {
        accelerometerError(callbackID);
    }
}

void CAccelerometer::watchAcceleration(int callbackID, const AccelerometerOptions& accelerometerOptions, uint32 watchID)
{
    IwTrace(WM_MODULE_ACCEL,("watchAcceleration (callbackID: %d, watchID %u)", callbackID, watchID));

    if (CheckAccelerometerStatus())
    {
        Acceleration acc(0.0f, 0.0f, 0.0f, 0);
        getAccelValues(acc);
        CWMWatchData* wd = new CWMWatchData(watchID, callbackID, accelerometerOptions.frequency, accelerometerCallback);
        AddWatch(wd);
        accelerometerSuccess(callbackID, acc);
    }
    else
        accelerometerError(callbackID);
}


void CAccelerometer::clearWatch(int callbackID, uint32 watchID)
{
    IwTrace(WM_MODULE_ACCEL,("clearWatch (callbackID: %d, watchID %u)", callbackID, watchID));

    if (!RemoveWatch(watchID))
    {
        SetError("Internal Error: clearWatch failed.");
    }
}

void CAccelerometer::accelerometerCallback(CWMWatchData* pWatchData)
{
    ((CAccelerometer*) pWatchData->m_Module)->getCurrentAcceleration(pWatchData->m_CallbackID);
}
