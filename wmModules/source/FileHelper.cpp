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

#include <stl/_algobase.h>
#include "IwDebug.h"
#include "FileHelper.h"

bool FileHelper::IsHTTP(const char* url)
{
     return (!strncmp(url, "http://", 7) || !strncmp(url, "https://", 8));
}

bool FileHelper::FileExtension(string filename, char* out, int size)
{
    size_t pos = filename.find_last_of('.');
    if (pos == string::npos)
        return false;

    string extension = filename.substr(pos + 1);
    strncpy(out, extension.c_str(), size);
    return true;
}

bool FileHelper::WMAbsolutePathToS3ERaw(const char* in, char* out, int len)
{
    if (!in || !out)
        return false;

    // https should be treated the same on all platforms
    if (IsHTTP(in))
    {
        strncpy(out, in, len);
        IwTrace(WEBMARMALADE,("WMAbsolutePathToS3ERaw: Found http %s", out));
        return true;
    }

    uint32 charsCopied = 0;
    uint32 inLength = (int)strlen(in);

    strncpy(out + charsCopied, "raw://", len - charsCopied);
    charsCopied += sizeof("raw://") - 1;
    
    strncpy(out + charsCopied, in, len - charsCopied);
    charsCopied += std::min(len - charsCopied, inLength);

    // Check buffer was big enough
    if (charsCopied < sizeof("raw://") - 1 + inLength)
        return false;

    return true;
}
