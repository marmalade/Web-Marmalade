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

#include "s3e.h"
#include "s3eWebView.h"
#include "wmModules.h"
#include <string>
#include "FallbackPage.h"
#include "IwDebug.h"
#include "IwGx.h"
#include "wmClipboard.h"

#define S3E_SURFACE_RESIZE_PENDING (s3eSurfaceCallback)2
#define WEB_DEFAULT_ROOT_DIRECTORY "webassets"
#define WEB_DEFAULT_URL "index.html"

#define ALLOCATION_SIZE_FOR_CONFIG_STRINGS sizeof(char) * S3E_CONFIG_STRING_MAX
#define WEB_DEFAULT_SCHEME LOCAL_ROM_URL
#define URL_HAS_SCHEME(x) strstr(x, "://")

typedef enum CurrentURLType
{
    STARTUP_URL_USER_DEFINED,
    STARTUP_URL_DEFAULT,
    STARTUP_URL_EMBEDDED

} CurrentURLType;

// Tells us which url to try and navigate to (no ownership)
static const char* const * g_WebStartupURL = NULL;
// Scheme to locate local files (might one day not be const)
static const char* const g_LocalFileURLScheme = WEB_DEFAULT_SCHEME;
// Root folder of all web app assets
static char* g_WebAssetsRootDirectory = NULL;
// User defined start URL from icf
static char* g_WebUserDefinedURL = NULL;
// Default start URL if user didn't specify one
static char* g_WebDefaultURL = NULL;
// State representing which stage of fallbacks we're in
static CurrentURLType g_CurrentURLType = STARTUP_URL_USER_DEFINED;

// Name of temp file to create and load as our final fallback url
static const char*  g_WebInternalFallbackURL = LOCAL_RAM_URL "error.html";
// Flag to indicate whether we need to cleanup a tempfile
static bool g_DeleteTempURL = false;

static bool g_UsingWinSim = false;

// The main webview
static s3eWebView* g_WebView = NULL;

// The Javascript Dispatcher
static CIwWMDispatcher* g_Dispatcher = NULL;

// example extra module
static CClipboard* g_Clipboard = NULL;


// Fwd delcarations
bool CreateFallbackPage(const char* url, const char* content);
void DeleteFallbackPage(const char* url);

//Assumes that str is long enough to take the prefix
static void strprepend(const char* prefix, char* str)
{
    const int originalLen = strlen(str);
    const int prefixLen = strlen(prefix);

    //Shift the string along
    memmove(str+prefixLen, str, originalLen);

    //Can use memcpy for the prefix
    memcpy(str, prefix, prefixLen);

    //NULL terminate
    str[originalLen+prefixLen] = '\0';
}

static bool InitGlobals()
{
    if (!g_WebAssetsRootDirectory)
    {
        if (!(g_WebAssetsRootDirectory = (char*)s3eMalloc(ALLOCATION_SIZE_FOR_CONFIG_STRINGS + 1))) // + 1 for possible extra "/"
            return false; //Like it's going to happen at this stage!

        if (s3eConfigGetString("Web", "WebRootDirectory", g_WebAssetsRootDirectory) != S3E_RESULT_SUCCESS)
        {
            strcpy(g_WebAssetsRootDirectory, WEB_DEFAULT_ROOT_DIRECTORY);
        }

        int length = strlen(g_WebAssetsRootDirectory);
        if (g_WebAssetsRootDirectory[length - 1] != '/' || g_WebAssetsRootDirectory[length - 1] != '\\')
        {
            g_WebAssetsRootDirectory[length] = '/';
            g_WebAssetsRootDirectory[length + 1] = 0;
        }

        //Root directory mustn't contain a scheme
        if (URL_HAS_SCHEME(g_WebAssetsRootDirectory))
        {
            IwTrace(WEBMARMALADE, ("WebRootDirectory must not contain a scheme"));
            return false;
        }
    }

    if (!g_WebUserDefinedURL)
    {
        const int totalCapacity = ALLOCATION_SIZE_FOR_CONFIG_STRINGS +
            strlen(g_WebAssetsRootDirectory) + strlen(g_LocalFileURLScheme);

        if (!(g_WebUserDefinedURL = (char*)s3eMalloc(totalCapacity)))
            return false; //Like it's going to happen at this stage!

        if (s3eConfigGetString("Web", "WebStartURL", g_WebUserDefinedURL) != S3E_RESULT_SUCCESS)
        {
            //No user defined URL
            g_CurrentURLType = STARTUP_URL_DEFAULT;
            g_WebUserDefinedURL[0] = 0;
        }
        //This means if the user specifies e.g. www.madewithmarmalade.com then this fails.
        else if (!URL_HAS_SCHEME(g_WebUserDefinedURL))
        {
            //The user defined url is "relative" then add the scheme and the default file location
            strprepend(g_WebAssetsRootDirectory, g_WebUserDefinedURL);
            strprepend(g_LocalFileURLScheme, g_WebUserDefinedURL);
        }
    }

    if (!g_WebDefaultURL)
    {
        const int totalCapacity = strlen(WEB_DEFAULT_URL) +
            strlen(g_WebAssetsRootDirectory) + strlen(g_LocalFileURLScheme);

        if (!(g_WebDefaultURL = (char*)s3eMalloc(totalCapacity)))
            return false; //Like it's going to happen at this stage!

        strcpy(g_WebDefaultURL, WEB_DEFAULT_URL);
        if (!URL_HAS_SCHEME(g_WebDefaultURL))
        {
            //The default url is "relative" then add the scheme and the default file location
            strprepend(g_WebAssetsRootDirectory, g_WebDefaultURL);
            strprepend(g_LocalFileURLScheme, g_WebDefaultURL);
        }
    }

    if (g_CurrentURLType == STARTUP_URL_USER_DEFINED)
        g_WebStartupURL = &g_WebUserDefinedURL;
    else if (g_CurrentURLType == STARTUP_URL_DEFAULT)
        g_WebStartupURL = &g_WebDefaultURL;


    g_UsingWinSim = s3eDeviceGetInt(S3E_DEVICE_OS) == S3E_OS_ID_WINDOWS;

    return true;
}

static bool CheckQuit()
{
    bool rtn = s3eDeviceCheckQuitRequest()
        || (s3eKeyboardGetState(s3eKeyEsc) & S3E_KEY_STATE_PRESSED)
        || (s3eKeyboardGetState(s3eKeyAbsBSK) & S3E_KEY_STATE_PRESSED);
    if (rtn)
        IwTrace(WEBMARMALADE, ("Quitting Web Marmalade App"));
    return rtn;
}

static int32 handleReset(void* systemData, void* userData)
{
    s3eWebViewNavigate(g_WebView, *g_WebStartupURL);

    return 1;
}

static void Terminate()
{
    s3eDeviceUnRegister(S3E_DEVICE_SIMULATOR_RESTART, handleReset);

    if (g_WebView)
    {
        s3eWebViewDestroy(g_WebView);
        g_WebView = NULL;

        //Free the strings
        free(g_WebAssetsRootDirectory);
        free(g_WebDefaultURL);
        free(g_WebUserDefinedURL);

        g_WebAssetsRootDirectory = NULL;
        g_WebDefaultURL = NULL;
        g_WebUserDefinedURL = NULL;

        if (g_DeleteTempURL)
            DeleteFallbackPage(g_WebInternalFallbackURL);
    }
    // example delete module
    if (g_Clipboard)
    {
        g_Clipboard->Shutdown();
        delete g_Clipboard;
        g_Clipboard = NULL;
    }
    if (g_Dispatcher)
    {
        IwWebBridgeShutdown(g_Dispatcher);
        g_Dispatcher = NULL;
    }
}

void DoNavigate()
{
    IwTrace(WEBMARMALADE, ("loading url = %s", *g_WebStartupURL));
    s3eWebViewNavigate(g_WebView, *g_WebStartupURL);
}

static int32 screenResizePending(void* systemData, void* userData)
{
    IwTrace(WEBMARMALADE, ("screenResize Pending, calling s3eSurfaceShow()"));
    // First call to surface show resizes the surface and calls the S3E_SURFACE_SCREENSIZE
    // callback.  No surface is actually displayed
    s3eSurfaceShow();
    // On ios the first call only sets  the state to resize pending. The second call actually performs it
    s3eSurfaceShow();
    return 0;
}

static int32 screenSizeChanged(void* systemData, void* userData)
{
    IwTrace(WEBMARMALADE, ("screenSizeChanged callback fired.  Resizing webview"));
    s3eWebViewResize(g_WebView, 0, 0, s3eSurfaceGetInt(S3E_SURFACE_WIDTH), s3eSurfaceGetInt(S3E_SURFACE_HEIGHT));
    s3eSurfaceShow();
    return 0;
}

static int32 pageNotFound(s3eWebView *instance, void *systemData, void *userData)
{
    switch (g_CurrentURLType)
    {
    case STARTUP_URL_USER_DEFINED:
        g_CurrentURLType = STARTUP_URL_DEFAULT;
        IwTrace(WEBMARMALADE, ("pageNotFound: trying default url = %s", g_WebDefaultURL));
        g_WebStartupURL = &g_WebDefaultURL;
        break;

    case STARTUP_URL_DEFAULT:
        g_CurrentURLType = STARTUP_URL_EMBEDDED;
        IwTrace(WEBMARMALADE, ("pageNotFound: creating and loading fallback url = %s", g_WebInternalFallbackURL));
        if (!(g_DeleteTempURL = CreateFallbackPage(g_WebInternalFallbackURL, g_WebInteralFallbackContent)))
        {
            IwTrace(WEBMARMALADE, ("failed to create fallback url: quitting"));
            s3eDeviceRequestQuit();
            return 0;
        }
        else
        {
            g_WebStartupURL = &g_WebInternalFallbackURL;
        }
        break;

    default:
        IwTrace(WEBMARMALADE, ("pageNotFound: can't find loadable page, quitting"));
        s3eDeviceRequestQuit();
        return 0;
    }

    DoNavigate();
    return 0;
}

bool Init()
{
    // Clear the debug screen from behind the app (you can see it on iOS when you drag
    // the browser).
    s3eSurfaceClear(0, 0, 0);
    s3eSurfaceShow();

    if (!s3eWebViewAvailable())
    {
        IwError(("Webview not available"));
        return false;
    }

    InitGlobals();

    if (!(g_WebView = s3eWebViewCreate()))
    {
        IwTrace(WEBMARMALADE, ("Failed to create webview"));
        return false;
    }

    if (!(g_Dispatcher = IwWebBridgeCreate(g_WebView)))
    {
        IwTrace(WEBMARMALADE, ("Failed to create dispatcher"));
        return false;
    }
    
    // add an example module
    g_Clipboard = new CClipboard(g_Dispatcher);

    g_Dispatcher->SetNativeReady();

    s3eWebViewRegister(S3E_WEBVIEW_FAILED_LOADING, pageNotFound, 0, g_WebView);
    s3eSurfaceRegister(S3E_SURFACE_SCREENSIZE, screenSizeChanged, NULL);
    s3eDeviceRegister(S3E_DEVICE_SIMULATOR_RESTART, handleReset, NULL);
    s3eSurfaceRegister(S3E_SURFACE_RESIZE_PENDING, screenResizePending, NULL);
    s3eWebViewShow(g_WebView, 0, 0, s3eSurfaceGetInt(S3E_SURFACE_WIDTH), s3eSurfaceGetInt(S3E_SURFACE_HEIGHT));

    // For some iOS devices we need to call this on init otherwise the webview is not displayed
    s3eSurfaceShow();

    DoNavigate();

    return true;
}

bool Update()
{
    if (!g_UsingWinSim)
        s3eDeviceYieldUntilEvent();
    else
        s3eDeviceYield(0);
    return true;
}

int main()
{
    IwTrace(WEBMARMALADE, ("Started Web Marmalade App"));

    if (!Init())
        return 0;

    //Main loop
    while (!CheckQuit())
    {
        if (!Update())
            break;
    }

    Terminate();
    return 0;
}
