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
// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\h.start.tmpl") ]]]

#include "IwDispatcher.h"

/*
 * Generated class for JavaScript bridge module connection
 * Some sections of this module are re-generated when the interface changes the rest is generated once
 */


class CConnectionBase : public CIwWMDispatcherModule<CConnectionBase>
{
public:
    CConnectionBase(CIwWMDispatcherBase* dispatcher);

    class Connection
    {
    public:
        static const char* UNKNOWN;
        static const char* ETHERNET;
        static const char* WIFI;
        static const char* CELL_2G;
        static const char* CELL_3G;
        static const char* CELL_4G;
        static const char* NONE;

    public:
        std::string type;

        Connection(const char* _type) : type(_type) {}

        void To_JSON(Json::Value& value) const;
        static Connection From_JSON(const Json::Value& value, CConnectionBase* reader);
    };

// [[[end]]]

/*
 * Auto Generated section for converting between JavaScript and C++
 */
// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\h.bridge.tmpl") ]]]
private:
    //JS->C++ automated converters
    //C++->JS automated event setup
    bool network(int callbackID, const Json::Value& parameters);

public:
    //C++->JS automated callbacks
    //C++->JS automated events
    bool network(const char* type);


// [[[end]]]

/*
 * Auto Generated section for defining modules functions
 * The creator of the module needs to implement these functions
 */
// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\h.def.tmpl") ]]]
public:

    //To be implemented in the user's derived class


    virtual void networkController(bool isStart) = 0;

// [[[end]]]

// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\h.end.tmpl") ]]]
};
// [[[end]]]
