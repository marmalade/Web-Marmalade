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

#include "IwDispatcher.h"

class CClipboard : public CIwWMDispatcherModule<CClipboard>
{
public:
    // Initialisation
    CClipboard(CIwWMDispatcherBase* dispatcher);
    
    // Handle system events
    void Init();
    void OnReady();
    void Shutdown();

    // From Javascript
    bool getText(int callbackID, const Json::Value& parameters);
    bool setText(int callbackID, const Json::Value& parameters);

    // To Javascript
    bool clipboardSuccess(int callbackID, const char* text);
    bool clipboardError(int callbackID);
};
