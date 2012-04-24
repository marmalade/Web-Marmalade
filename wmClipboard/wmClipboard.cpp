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

#include "s3eClipboard.h"

#include "wmClipboard.h"

CClipboard::CClipboard(CIwWMDispatcherBase* dispatcher) :
    CIwWMDispatcherModule<CClipboard>("clipboard", dispatcher)
{
    // Register the functions to be exposed to javascript
    RegisterMember("getText", &CClipboard::getText);
    RegisterMember("setText", &CClipboard::setText);
}

void CClipboard::Init()
{
}

void CClipboard::Shutdown()
{
}

void CClipboard::OnReady()
{
}

bool CClipboard::getText(int callbackID, const Json::Value& parameters)
{
    // call s3e code
    if (!s3eClipboardAvailable())
    {
        // call fail callback
        clipboardError(callbackID);
        return true;
    }

    char text[256];
    if (s3eClipboardGetText(text, 256) < 1)
    {
        // call fail callback
        clipboardError(callbackID);
        return true;
    }

    // call success callback with text
    clipboardSuccess(callbackID, text);
    return true;
}

bool CClipboard::setText(int callbackID, const Json::Value& parameters)
{
    // fetch member 'text' of parameters
    Json::Value text_JSON;
    FetchObject(parameters, "text", text_JSON);

    // fetch string
    std::string text;
    FetchString(text_JSON, text);

    // call s3e code
    if (!s3eClipboardAvailable())
    {
        // call fail callback
        clipboardError(callbackID);
        return true;
    }

    if (s3eClipboardSetText(text.c_str()) == S3E_RESULT_ERROR)
    {
        // call fail callback
        clipboardError(callbackID);
        return true;
    }

    // call success callback with text
    clipboardSuccess(callbackID, text.c_str());
    return true;
}

bool CClipboard::clipboardSuccess(int callbackID, const char* text)
{
    Json::Value params(Json::objectValue);
    params["text"]=Json::Value(text);

    SendJavaScript("clipboardSuccess", callbackID, params);
    return true;
}

bool CClipboard::clipboardError(int callbackID)
{
    Json::Value params(Json::objectValue);

    SendJavaScript("clipboardError", callbackID, params);
    return true;
}