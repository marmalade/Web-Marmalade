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

#include "ConnectionBase.h"


class CConnection : public CConnectionBase
{

public:
    CConnection(CIwWMDispatcherBase* dispatcher) : CConnectionBase(dispatcher), m_Listening(false) {}
    
    void Init();
    void OnReady();
    void Shutdown();

    virtual void networkController(bool isStart);

protected:
    static int32 NetworkCallback(void *systemData, CConnection *events);
    static int32 NetworkOfflineCallback(void *systemData, CConnection *events);
    void DoNetwork();
    bool m_Listening;
};
