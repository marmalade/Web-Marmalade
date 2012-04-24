#pragma once
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
//-----------------------------------------------------------------------------

#ifndef WM_STANDARD_API_H
#define WM_STANDARD_API_H

#include "IwDispatcher.h"

CIwWMDispatcher* IwWebBridgeCreate(s3eWebView* view);

void IwWebBridgeShutdown(CIwWMDispatcher* dispatcher);

#endif /* !WM_STANDARD_API_H */
