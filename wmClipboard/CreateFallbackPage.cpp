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

#include "s3eFile.h"
#include <string>

bool CreateFallbackPage(const char* url, const char* content)
{
    if (!url || !content)
        return false;

    s3eFile* file;
    if (!(file = s3eFileOpen(url, "w")))
        return false;

    const uint32 strLen = strlen(content);
    if (s3eFileWrite(content, 1, strLen, file) != strLen)
    {
        s3eFileClose(file);
        s3eFileDelete(url);
        return false;
    }

    s3eFileClose(file);
    return true;
}

void DeleteFallbackPage(const char* url)
{
    s3eFileDelete(url);
    return; 
}
