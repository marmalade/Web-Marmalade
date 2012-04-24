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

// Slightly cleaner way of embedding the fallback content

#define FALLBACK_CONTENT " \
<html> \
<body bgcolor=""#ffffff""> \
</br> \
Error: No startup webpage found</br> \
<a href="""">Exit Application</a> \
</body> \
</html>"

// An embedded html which is our final fallback option if no startup page is found
static const char* g_WebInteralFallbackContent = FALLBACK_CONTENT;
