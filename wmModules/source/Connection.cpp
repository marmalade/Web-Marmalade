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

#include "Connection.h"
#include "IwDebug.h"
#include "s3eSocket.h"

//*
#if defined UNITTESTING
#include "mockConnection.h"
#include "mockDevice.h"
#define M(fn) fn##Mock
#else
#define M(fn) fn
#endif
/*/
#define M(fn) fn
//*/


void CConnection::Init()
{
    IwTrace(WM_MODULE_CONNECTION, ("Connection: Init."));
}

void CConnection::Shutdown()
{
    IwTrace(WM_MODULE_CONNECTION, ("Connection: Shutdown."));
    this->networkController(false);
}

void CConnection::OnReady()
{
    IwTrace(WM_MODULE_CONNECTION, ("Connection: OnReady."));
    DoNetwork();
}

int32 CConnection::NetworkOfflineCallback(void *systemData, CConnection *events)
{
    IwTrace(WM_MODULE_CONNECTION, ("Connection: NetworkOfflineCallback."));
    events->network("none");
    return 0;
}

int32 CConnection::NetworkCallback(void *systemData, CConnection *events)
{
    IwTrace(WM_MODULE_CONNECTION, ("Connection: NetworkCallback."));
    events->DoNetwork();
    return 0;
}

void CConnection::DoNetwork()
{
    s3eNetworkType type = (s3eNetworkType)M(s3eSocketGetInt)(S3E_SOCKET_NETWORK_TYPE);
    switch(type)
    {
        case S3E_NETWORK_TYPE_NONE:
            network("none");
            break;
        case S3E_NETWORK_TYPE_LAN:
            network("ethernet");
            break;
        case S3E_NETWORK_TYPE_WLAN:
            network("wifi");
            break;
        case S3E_NETWORK_TYPE_GPRS:
            network("2g");
            break;
        case S3E_NETWORK_TYPE_UMTS:
            network("3g");
            break;
        case S3E_NETWORK_TYPE_EVDO:
            network("3g");
            break;
        case S3E_NETWORK_TYPE_CDMA2000:
            network("3g");
            break;
        case S3E_NETWORK_TYPE_HSDPA:
            network("3g");
            break;
        case S3E_NETWORK_TYPE_WIMAX:
            network("4g");
            break;
        case S3E_NETWORK_TYPE_BLUETOOTH:
            network("wifi");
            break;
        case S3E_NETWORK_TYPE_EDGE:
            network("2g");
            break;
        case S3E_NETWORK_TYPE_CDMA:
            network("3g");
            break;
        case S3E_NETWORK_TYPE_IDEN:
            network("2g");
            break;
        case S3E_NETWORK_TYPE_LTE:
            network("4g");
            break;
        case S3E_NETWORK_TYPE_EHRPD:
            network("3g");
            break;
        case S3E_NETWORK_TYPE_HSPAPLUS:
            network("4g");
            break;
        default:
            network("unknown");
            break;
    }
}

void CConnection::networkController(bool isStart)
{
    IwTrace(WM_MODULE_CONNECTION, ("Connection: networkController: %d", isStart));
    if (isStart && !m_Listening)
    {
        m_Listening = true;
        M(s3eDeviceRegister)(S3E_DEVICE_NETWORK_ONLINE, (s3eCallback)NetworkCallback, this);
        s3eDeviceRegister(S3E_DEVICE_NETWORK_OFFLINE, (s3eCallback)NetworkOfflineCallback, this);
    }
    else if (!isStart && m_Listening)
    {
        m_Listening = false;
        s3eDeviceUnRegister(S3E_DEVICE_NETWORK_ONLINE, (s3eCallback)NetworkCallback);
        s3eDeviceUnRegister(S3E_DEVICE_NETWORK_OFFLINE, (s3eCallback)NetworkOfflineCallback);
    }
}
