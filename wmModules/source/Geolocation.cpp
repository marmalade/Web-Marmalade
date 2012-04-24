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

#include "Geolocation.h"

#include "IwDebug.h"
#include "s3eLocation.h"

//*
#if defined UNITTESTING
#include "mockGeolocation.h"
#define M(fn) fn##Mock
#else
#define M(fn) fn
#endif
/*/
#define M(fn) fn
//*/

CGeolocation::CGeoWatchData::CGeoWatchData(int watchID, int callbackID, unsigned int maxInterval, watchCB callbackFn,  unsigned int timeout)
    : CWMWatchData(watchID, callbackID, maxInterval, callbackFn, timeout)
{
    enableHighAccuracy = false;
    maximumAge = 0;
    timeout = 0;
}
 

CGeolocation::CGeoWatchData::~CGeoWatchData()
{}

void CGeolocation::Init()
{
    IwTrace(WM_MODULE_GEOLOCATION, ("Geolocation: Init."));
    m_Status = s3eLocationAvailable() ? WM_STATUS_GEO_OFF : WM_STATUS_GEO_UNSUPPORTED;
}

void CGeolocation::OnReady()
{
    IwTrace(WM_MODULE_GEOLOCATION, ("Geolocation: OnReady."));    
}

void CGeolocation::Shutdown()
{    
    IwTrace(WM_MODULE_GEOLOCATION, ("Geolocation: Shutdown."));
    if (s3eLocationAvailable())
        (void) s3eLocationStop();
}


bool CGeolocation::CheckLocationStatus()
{
    if (m_Status == WM_STATUS_GEO_UNSUPPORTED)
        return false;

    if (m_Status != WM_STATUS_GEO_ON)
    {
        if (s3eLocationStart() == S3E_RESULT_ERROR)
        {
            m_Status = WM_STATUS_GEO_FAILED;
            return false;
        }
        else
        {
            m_Status = WM_STATUS_GEO_ON;
            return true;
        }
    }
    return true;
}

void CGeolocation::getCurrentPosition(int callbackID, const GeolocationOptions& geolocationOptions)
{
    // geolocationOptions:
    // (frequency) deprecated
    // maximumAge
    // timeout
    // enableHighAccuracy
    
    if (CheckLocationStatus())
    {
        s3eLocation location;
        if (M(s3eLocationGet)(&location) == S3E_RESULT_SUCCESS)
        {
            s3eLocationCourseData course;
            if (M(s3eLocationGetCourse)(&course) != S3E_RESULT_SUCCESS)        
                course.m_Heading = course.m_Speed = 0;

            Coordinates coordinates(location.m_Latitude, location.m_Longitude, location.m_Altitude, location.m_HorizontalAccuracy, location.m_VerticalAccuracy, course.m_Heading, course.m_Speed);
            Position position(coordinates, s3eTimerGetUTC());
            geolocationSuccess(callbackID, position);
            return;
        }
    }
 
    // Error
    PositionError error(PositionError::POSITION_UNAVAILABLE, "Geolocation Error: ");

    if (m_Status == WM_STATUS_GEO_FAILED)
        error.message += "Position Failed To Initialise.";
    else if (m_Status == WM_STATUS_GEO_OFF)
        error.message += "Position is turned off.";
    else if (m_Status == WM_STATUS_GEO_UNSUPPORTED)
        error.message += "Unsupported";
    else if (S3E_LOCATION_ERR_UNAVAIL == s3eLocationGetError())
        error.message += "Position Unavailable";
    else if (S3E_LOCATION_ERR_UNAVAIL != s3eLocationGetError())
        error.message +=  std::string(s3eLocationGetErrorString());                

    geolocationError(callbackID, error);
}

//
// Set up watch 
//
void CGeolocation::watchPosition(int callbackID, const GeolocationOptions& geolocationOptions, uint32 watchID)
{
    if (CheckLocationStatus())
    {
        CGeoWatchData* watchData = new CGeoWatchData(watchID, callbackID, geolocationOptions.frequency, CGeolocation::geolocationCallback, 0);        
        watchData->enableHighAccuracy = geolocationOptions.enableHighAccuracy;
        watchData->timeout = geolocationOptions.timeout;
        watchData->maximumAge = geolocationOptions.maximumAge;

        if (AddWatch(watchData))
        {
            geolocationCallback(watchData);
            return;
        }   
    }
    PositionError error(PositionError::POSITION_UNAVAILABLE, "Geolocation Error: Could not set watch.");
    geolocationError(callbackID, error);
}

void CGeolocation::clearWatch(int callbackID, uint32 watchID)
{
    if (!RemoveWatch(watchID))
    {
        SetError("Geolocation Internal Error: clearWatch failed.");
    }
}

void CGeolocation::geolocationCallback(CWMWatchData* pWatchData)
{
    CGeoWatchData* geoWatchData = (CGeoWatchData*) pWatchData;
    
    GeolocationOptions options(geoWatchData->m_MaxInterval, geoWatchData->enableHighAccuracy, geoWatchData->timeout, geoWatchData->maximumAge);
    ((CGeolocation*) geoWatchData->m_Module)->getCurrentPosition(geoWatchData->m_CallbackID, options);
}
