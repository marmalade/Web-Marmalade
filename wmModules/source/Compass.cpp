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

#include "Compass.h"

#include "IwDebug.h"
#include "s3eCompass.h"
#include "s3eLocation.h"

//*
#if defined UNITTESTING
#include "mockCompass.h"
#define M(fn) fn##Mock
#else
#define M(fn) fn
#endif
/*/
#define M(fn) fn
//*/

CCompass::CCompassWatchData::CCompassWatchData(int watchID, int callbackID, unsigned int maxInterval, watchCB callbackFn,  unsigned int timeout)
    : CWMWatchData(watchID, callbackID, maxInterval, callbackFn, timeout)
{
    filter = 0;
    lastSavedHeading = false;
}

CCompass::CCompassWatchData::~CCompassWatchData()
{}


void CCompass::Init()
{
    IwTrace(WM_MODULE_COMPASS, ("Compass: Init."));
    m_Status = s3eCompassAvailable() ? WM_STATUS_COMP_OFF : WM_STATUS_COMP_UNSUPPORTED;
}

void CCompass::Shutdown()
{
    IwTrace(WM_MODULE_COMPASS, ("Compass: Shutdown."));

    // stop compass
    if (m_Status == WM_STATUS_COMP_ON)
    {
        M(s3eCompassStop)();
        m_Status = WM_STATUS_COMP_OFF;
    }

    //stop s3eLocation. Needed for true heading
    if (s3eLocationAvailable())
        s3eLocationStop();

    // remove watches from dispatcher belonging to this module
    RemoveWatches();
}

void CCompass::OnReady()
{
    IwTrace(WM_MODULE_COMPASS, ("Compass: OnReady."));

}

bool CCompass::CheckCompassStatus()
{
    if (m_Status == WM_STATUS_COMP_UNSUPPORTED)
        return false;

    if (m_Status != WM_STATUS_COMP_ON)
    {
        if (M(s3eCompassStart)() == S3E_RESULT_ERROR)
        {
            m_Status = WM_STATUS_COMP_FAILED;
            return false;
        }
        else
        {
            //start s3eLocation. Needed for true heading
            if (!s3eLocationAvailable())
                IwTrace(WM_MODULE_COMPASS,("s3eLocation Unavailable"));
            else if (s3eLocationStart() == S3E_RESULT_ERROR)
                IwTrace(WM_MODULE_COMPASS,("s3eLocationStart() failed"));

            m_Status = WM_STATUS_COMP_ON;
            return true;
        }
    }
    return true;
}

bool CCompass::GetCompassValues(CompassHeading& heading)
{
    int magneticHeading = M(s3eCompassGet)();
    if (-1 == magneticHeading)
        return false;

    heading.magneticHeading = (double)magneticHeading;

    s3eLocationCourseData courseData;

    if (s3eLocationGetCourse(&courseData) == S3E_RESULT_SUCCESS)
        heading.trueHeading = courseData.m_Heading;
    else
        heading.trueHeading = heading.magneticHeading;

    //headingAccuracy is the deviation in degrees between the reported heading and the true heading.
    heading.headingAccuracy = heading.magneticHeading - heading.trueHeading;

    heading.timestamp = (int64) s3eTimerGetUTC();

    return true;
}

#define FIRST_USE_WATCHID -200
#define FIRST_USE_WATCH_TIMEOUT 500
#define FIRST_USE_WATCH_INTERVAL 50

void CCompass::getCurrentHeading(int callbackID, const CompassOptions& compassOptions)
{
    IwTrace(WM_MODULE_COMPASS,("getCurrentHeading (callbackID: %d)", callbackID));

    if (CheckCompassStatus())
    {
        CompassHeading heading(0,0,0,0);
        if (GetCompassValues(heading))
        {
            compassSuccess(callbackID, heading);
            return;
        }
        else if (m_Dispatcher->GetWatch(FIRST_USE_WATCHID))
        {
            // Do nothing
            // If there is already a first-use watch in place we don't fire any callbacks here
            return;
        }
        else
        {
            // Compass may take a while to start so need to poll until the values are valid
            CCompassWatchData* watchData = new CCompassWatchData(FIRST_USE_WATCHID, callbackID, FIRST_USE_WATCH_INTERVAL, CompassCallback, FIRST_USE_WATCH_TIMEOUT);
            AddWatch(watchData);
            return;
        }
    }
    compassError(callbackID, CompassError::COMPASS_INTERNAL_ERR);
}

void CCompass::watchHeading(int callbackID, const CompassOptions& compassOptions, uint32 watchID)
{
    if (CheckCompassStatus())
    {
        CCompassWatchData* watchData = new CCompassWatchData(watchID, callbackID, compassOptions.frequency, CompassCallback);
        watchData->filter = compassOptions.filter;
        if (AddWatch(watchData))
            getCurrentHeading(callbackID, compassOptions);
        else
            SetError("watchHeading: Failed to add watch");
    }
    else
    {
        compassError(callbackID, CompassError::COMPASS_INTERNAL_ERR);
    }
}

void CCompass::clearWatch(int callbackID, uint32 watchID)
{
    IwTrace(WM_MODULE_COMPASS,("clearWatch (callbackID: %d, watchID %u)", callbackID, watchID));

    if (!RemoveWatch(watchID))
    {
        IwTrace(WM_MODULE_COMPASS,("clearWatch failed."));
        SetError("Internal Error: clearWatch failed.");
    }
}

void CCompass::watchHeadingFilter(int callbackID, const CompassOptions& compassOptions, uint32 watchID)
{
    watchHeading(callbackID, compassOptions, watchID);
}

void CCompass::clearWatchFilter(int callbackID, uint32 watchID)
{
    clearWatch(callbackID, watchID);
}

void CCompass::CompassCallback(CWMWatchData* pWatchData)
{
    CCompassWatchData* compassWatchData = (CCompassWatchData*) pWatchData;
    CCompass* module = (CCompass*) compassWatchData->m_Module;

    CompassHeading heading(0,0,0,0);
    if (module->CheckCompassStatus() && GetCompassValues(heading))
    {       
        if (!compassWatchData->filter)
        {
            module->compassSuccess(compassWatchData->m_CallbackID, heading);
        }
        else
        {            
            int filter = compassWatchData->filter % 180;
            int lastHeading = compassWatchData->lastSavedHeading;
            int currentHeading = (int) heading.magneticHeading;

            int angleDifference = abs(currentHeading - lastHeading);
            if (angleDifference > 180)
                angleDifference = 360 - angleDifference;

            if (angleDifference >= filter)
            {
                compassWatchData->lastSavedHeading = currentHeading;
                module->compassSuccess(compassWatchData->m_CallbackID, heading);
            }
        }

        if (compassWatchData->m_WatchID == FIRST_USE_WATCHID)
            module->RemoveWatch(FIRST_USE_WATCHID);

        return;
    }

    if (compassWatchData->m_WatchID == FIRST_USE_WATCHID && !compassWatchData->m_Expired)
        return;

    if (module->CheckCompassStatus() && module->m_Dispatcher->GetWatch(FIRST_USE_WATCHID))
    {
        // if FIRST_USE_WATCHID exists surpress errors
        return;
    }

    module->compassError(compassWatchData->m_CallbackID, CompassError::COMPASS_INTERNAL_ERR);
}
