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

#include "ChildbrowserBase.h"
#include "s3eOverlay.h"

class CChildbrowser : public CChildbrowserBase
{
private:
    static CChildbrowser* browser;
    s3eOverlay* m_Overlay;

    // Callback Function(s)
    static int32 webPageFailedToLoad(void* instance, void* systemData, void* userData);
    static int32 webPageFinishedLoad(void* instance, void* systemData, void* userData);
    static int32 webViewClosed(void* instance, void* systemData, void* userData);
    static int32 screenSizeChanged(void* systemData, void* userData);
    static int32 CloseButtonClicked(void* systemData, void* userData);

    // Helper Function(s)
    void CloseChildBrowser();

public:
    CChildbrowser(CIwWMDispatcherBase* dispatcher): CChildbrowserBase(dispatcher), m_Overlay(NULL) {}

    void Init();
    void OnReady();
    void Shutdown();

    // Base Function(s)
    void ChildBrowser_showWebPage(int callbackID, const ChildBrowser& self, const char* url);
    void ChildBrowser_close(int callbackID, const ChildBrowser& self);

    void onFailEventController(bool isStart);
    void onLocationChangeEventController(bool isStart);
    void onCloseEventController(bool isStart);
    void onOpenExternalEventController(bool isStart);
};
