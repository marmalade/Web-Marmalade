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

#include "Notification.h"
#include "s3eVibra.h"
#include "s3eSound.h"
#include "s3eDialog.h"
#include "s3eDevice.h"
#include "IwDebug.h"

//*
#if defined UNITTESTING
#include "mockNotification.h"
#define M(fn) fn##Mock
#else
#define M(fn) fn
#endif
/*/
#define M(fn) fn
//*/

#include "ding.h"

struct DialogData 
{
    bool m_Alert;
    int m_CallbackID;
    CNotification* m_Parent;

    DialogData() : m_Alert(false), m_CallbackID(-1), m_Parent(NULL){}
    DialogData(CNotification* parent, bool alert, int callbackID) :
        m_Parent(parent), m_Alert(alert), m_CallbackID(callbackID) {}
};

// Id and map for keeping track of user data associated to each dialog
int g_NextFreeId = 0;
std::map<int, DialogData> g_DialogMap;

int32 DialogCallback(s3eDialogCallbackData* data, void* userData)
{
    if (data == NULL)
        return 0;

    const DialogData& dialogData = g_DialogMap[data->m_Id];

    if (dialogData.m_Alert)
        dialogData.m_Parent->alertSuccess(dialogData.m_CallbackID);
    else
        dialogData.m_Parent->confirmSuccess(dialogData.m_CallbackID, data->m_ButtonPressed + 1);

    // remove the data from the map
    g_DialogMap.erase(data->m_Id);
    return 0;
}

void CNotification::Init()
{
    IwTrace(WM_MODULE_NOTIFICATION, ("Notification: Init."));
    M(s3eDialogRegister)(S3E_DIALOG_FINISHED, (s3eCallback)DialogCallback, NULL);
}

void CNotification::Shutdown()
{
    IwTrace(WM_MODULE_NOTIFICATION, ("Notification: Shutdown."));
    M(s3eDialogUnRegister)(S3E_DIALOG_FINISHED, (s3eCallback)DialogCallback);
}

//C++ user init code
void CNotification::OnReady()
{
    IwTrace(WM_MODULE_NOTIFICATION, ("Notification: OnReady."));
}
    

//C++ user code 

void CNotification::alert(int callbackID, const char* message, const char* title, const char* buttonName)
{
    s3eDialogAlertInfo info;
    info.m_Message = message;
    info.m_Title = title;
    info.m_Button[0] = buttonName;
    info.m_Id = g_NextFreeId;
    
    g_DialogMap[g_NextFreeId++] = DialogData(this, true, callbackID);
    M(s3eDialogAlertBox)(&info);
}

void CNotification::confirm(int callbackID, const char* message, const char* title, const char* buttonLabels)
{
    std::string buttonNames[3];
    int button = 0;
    for (int i=0; i<(int)strlen(buttonLabels); i++)
    {
        if (buttonLabels[i] == ',')
        {
            button++;
            if (button >= 3)
                break;
        }
        else
            buttonNames[button] += buttonLabels[i];
    }
    
    s3eDialogAlertInfo info;
    info.m_Message = message;
    info.m_Title = title;
    info.m_Button[0] = buttonNames[0].c_str();
    if (button >= 1)
        info.m_Button[1] = buttonNames[1].c_str();
    if (button >= 2)
        info.m_Button[2] = buttonNames[2].c_str();
    info.m_Id = g_NextFreeId;

    g_DialogMap[g_NextFreeId++] = DialogData(this, false, callbackID);

    M(s3eDialogAlertBox)(&info);
}

void CNotification::beep(int callbackID, int32 times)
{
    int channel = s3eSoundGetFreeChannel();
    M(s3eSoundSetInt)(S3E_SOUND_DEFAULT_FREQ, 8000);

    M(s3eSoundChannelPlay)(channel, (int16*)s_DingData, DING_LENGTH/2, times, 0);
}

void CNotification::vibrate(int callbackID, int32 time)
{
    if (M(s3eVibraGetInt)(S3E_VIBRA_AVAILABLE) == 0)
        return;

    M(s3eVibraSetInt)(S3E_VIBRA_ENABLED, 1);
    M(s3eVibraVibrate)(200, time);
}
