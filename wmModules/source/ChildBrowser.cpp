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
#include "mockWebView.h"
#define M(fn) fn##Mock
#else
#define M(fn) fn
#endif

#include "s3e.h"
#include "s3eWebView.h"

#include "IwDebug.h"
#include "ChildBrowser.h"

#include "IwDebug.h"


static bool DeviceIsWin()
{
    return (s3eDeviceOSID)s3eDeviceGetInt(S3E_DEVICE_OS) == S3E_OS_ID_WINDOWS;
}

static s3eWebView* g_WebViewChild = NULL;
CChildbrowser* CChildbrowser::browser = 0;

void CChildbrowser::Init()
{
    if (!s3eOverlayAvailable())
    {
        IwTrace(WM_MODULE_CHILDBROWSER,("Child Browser is not available for current platform"));
        return;
    }

    IwTrace(WM_MODULE_CHILDBROWSER,("ChildBrowser: Init"));
    s3eOverlayRegister(S3E_OVERLAY_BUTTON_CLICKED, CloseButtonClicked, NULL);
}

void CChildbrowser::Shutdown()
{
    if (!s3eOverlayAvailable())
    {
        IwTrace(WM_MODULE_CHILDBROWSER,("Child Browser is not available for current platform"));
        return;
    }

    IwTrace(WM_MODULE_CHILDBROWSER, ("ChildBrowser: Shutdown."));
    s3eOverlayUnRegister(S3E_OVERLAY_BUTTON_CLICKED, CloseButtonClicked);
}

//C++ user init code
void CChildbrowser::OnReady()
{
    if (!s3eOverlayAvailable())
    {
        IwTrace(WM_MODULE_CHILDBROWSER,("Child Browser is not available for current platform"));
        return;
    }

    IwTrace(WM_MODULE_CHILDBROWSER, ("ChildBrowser: OnReady."));
    browser = this;
}

// Private Callback Function(s)
int32 CChildbrowser::webPageFailedToLoad(void* instance, void* systemData, void* userData)
{
    const char* message = (const char*) systemData;
    browser->CloseChildBrowser();
    browser->onFailEvent(message);
    return 1;
}

int32 CChildbrowser::webPageFinishedLoad(void* instance, void* systemData, void* userData)
{
    const char* url = (const char*) systemData;
    browser->onLocationChangeEvent(url);
    return 1;
}

int32 CChildbrowser::webViewClosed(void* instance, void* systemData, void* userData)
{
    browser->CloseChildBrowser();
    browser->onCloseEvent();
    return 1;
}

int32 CChildbrowser::screenSizeChanged(void* systemData, void* userData)
{
    s3eWebViewResize(g_WebViewChild, 0, 0, s3eSurfaceGetInt(S3E_SURFACE_WIDTH), (s3eSurfaceGetInt(S3E_SURFACE_HEIGHT)/8) *7);

    // The Android surface rotates so we need to recalculate the Overlay item.
    // This is not currently necessary on iOS as the actual surface does not auto-rotate
    if (s3eDeviceGetInt(S3E_DEVICE_OS) == S3E_OS_ID_ANDROID)
    {
        s3eOverlaySetPosition(browser->m_Overlay, 0, (s3eSurfaceGetInt(S3E_SURFACE_HEIGHT)/8) *7);
        s3eOverlayResize(browser->m_Overlay, s3eSurfaceGetInt(S3E_SURFACE_WIDTH), s3eSurfaceGetInt(S3E_SURFACE_HEIGHT)/8);
    }
    return 1;
}

int32 CChildbrowser::CloseButtonClicked(void* systemData, void* userData)
{
    browser->CloseChildBrowser();
    browser->onCloseEvent();
    return 1;
}

//C++ user code
// Private Helper Function(s)
void CChildbrowser::CloseChildBrowser()
{
    if (g_WebViewChild)
    {
        s3eSurfaceUnRegister(S3E_SURFACE_SCREENSIZE, screenSizeChanged);
        M(s3eWebViewUnRegister)(S3E_WEBVIEW_CLOSED, (s3eWebViewCallbackFn) webViewClosed, g_WebViewChild);
        M(s3eWebViewUnRegister)(S3E_WEBVIEW_FINISHED_LOADING, (s3eWebViewCallbackFn) webPageFinishedLoad, g_WebViewChild);
        M(s3eWebViewUnRegister)(S3E_WEBVIEW_FAILED_LOADING, (s3eWebViewCallbackFn) webPageFailedToLoad, g_WebViewChild);

        M(s3eWebViewDestroy)(g_WebViewChild);

        g_WebViewChild = 0;
    }

    if (m_Overlay != NULL)
        s3eOverlayDestroy(m_Overlay);

#if defined UNITTESTING
// ONLY USED FOR TESTING. OTHERWISE CODE IS OMITTED
    s3eWebViewHideMock(g_WebViewChild);
    s3eWebViewDestroyMock(g_WebViewChild);
#endif
}

//C++ user code
// Base Function(s)
void CChildbrowser::ChildBrowser_showWebPage(int callbackID, const ChildBrowser& self, const char* url)
{
    if (!s3eOverlayAvailable())
    {
        IwTrace(WM_MODULE_CHILDBROWSER,("Child Browser is not available for current platform"));
        return;
    }

    IwTrace(WM_MODULE_CHILDBROWSER,("ChildBrowser_showWebPage (callbackID: %d)", callbackID));

    if (!g_WebViewChild) //Check g_WebViewChild is not already loaded
    {
        g_WebViewChild = M(s3eWebViewCreate)(S3E_FALSE);
        if (g_WebViewChild)
        {
            M(s3eWebViewRegister)(S3E_WEBVIEW_FAILED_LOADING, (s3eWebViewCallbackFn) webPageFailedToLoad, this, g_WebViewChild);
            M(s3eWebViewRegister)(S3E_WEBVIEW_FINISHED_LOADING, (s3eWebViewCallbackFn) webPageFinishedLoad, this, g_WebViewChild);
            M(s3eWebViewRegister)(S3E_WEBVIEW_CLOSED, (s3eWebViewCallbackFn) webViewClosed, this, g_WebViewChild);
            s3eSurfaceRegister(S3E_SURFACE_SCREENSIZE, screenSizeChanged, NULL);
        }
        else
            return;
    }
    else
        return;

    browser->onOpenExternalEvent();

    if (DeviceIsWin())
    {
        M(s3eWebViewNavigate)(g_WebViewChild, url);
        M(s3eWebViewShow)(g_WebViewChild, 0, 0, s3eSurfaceGetInt(S3E_SURFACE_WIDTH), (s3eSurfaceGetInt(S3E_SURFACE_HEIGHT)/8) *7, S3E_WEBVIEW_USE_SEPARATE_WINDOW);

        m_Overlay = NULL;
    }
    else
    {

        // Multiple show calls are used to force this webview to appear in front
        M(s3eWebViewShow)(g_WebViewChild, 0, 0, s3eSurfaceGetInt(S3E_SURFACE_WIDTH), (s3eSurfaceGetInt(S3E_SURFACE_HEIGHT)/8) *7);
        M(s3eWebViewNavigate)(g_WebViewChild, url);
        M(s3eWebViewShow)(g_WebViewChild, 0, 0, s3eSurfaceGetInt(S3E_SURFACE_WIDTH), (s3eSurfaceGetInt(S3E_SURFACE_HEIGHT)/8) *7);

        //Create s3eOverlay for button
        m_Overlay = s3eOverlayCreate(0, (s3eSurfaceGetInt(S3E_SURFACE_HEIGHT)/8) *7, s3eSurfaceGetInt(S3E_SURFACE_WIDTH), s3eSurfaceGetInt(S3E_SURFACE_HEIGHT)/8);
        s3eOverlaySetBackgroundColour(m_Overlay, S3E_OVERLAY_COLOUR_BLACK);
        s3eOverlayAddItem(m_Overlay, S3E_OVERLAY_ITEM_BUTTON, 30, 5, 40, 90);
    }
}

void CChildbrowser::ChildBrowser_close(int callbackID, const ChildBrowser& self)
{
    if (!s3eOverlayAvailable())
    {
        IwTrace(WM_MODULE_CHILDBROWSER,("Child Browser is not available for current platform"));
        return;
    }

    CloseChildBrowser();
    onCloseEvent();
}


void CChildbrowser::onFailEventController(bool isStart)
{
}

void CChildbrowser::onLocationChangeEventController(bool isStart)
{
}

void CChildbrowser::onCloseEventController(bool isStart)
{
}

void CChildbrowser::onOpenExternalEventController(bool isStart)
{
}
