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

#define WM_MAX_PATHS 3
#define BUFFER_SIZE 4095

#include "tiniconv.h"
#include "FileBase.h"
#include "IwHTTP.h"

class CFile;

class CIwHTTPPost
{
public:
    CIwHTTPPost(CFile* module, int callbackID) : m_Module(module), m_ID(callbackID), m_Response(NULL) {}
    virtual ~CIwHTTPPost()
    {
        if (m_Response != NULL)
            delete[] m_Response;
    }
    
    virtual s3eResult GetStatus() { return m_HTTP.GetStatus(); }
    virtual uint32 ContentSent() { return m_HTTP.ContentSent(); }
    virtual uint32 GetResponseCode() { return m_HTTP.GetResponseCode(); }
    virtual void SetPostChunkedMode(bool isChunked) { m_HTTP.SetPostChunkedMode(isChunked); }
    virtual void SetRequestHeader(const char *pName, const std::string &val) { m_HTTP.SetRequestHeader(pName, val); }
    virtual s3eResult SetFormDataFile(const char *pName, const char* sourceFile, const char* destName, const char* mimeType)
    {
        return m_HTTP.SetFormDataFile(pName, sourceFile, destName, mimeType);
    }
    virtual void SetFormData(const char *pName, const std::string &val) 
    {
        m_HTTP.SetFormData(pName, val); 
    }
    virtual s3eResult Post(const char *URI, const char* Body, int32 BodyLength, s3eCallback callback, void *data)
    {
        return m_HTTP.Post(URI, Body, BodyLength, callback, data);
    }

protected:
    CIwHTTP m_HTTP;
public:
    CFile* m_Module;
    int m_ID;
    char* m_Response;
};

class CFile : public CFileBase
{
public:
    CFile(CIwWMDispatcherBase* dispatcher): CFileBase(dispatcher) {}
    
    FileError::FileErrorEnum CheckEntry(const Entry& entry, int* fileSystem = NULL);
    FileError::FileErrorEnum CheckEntry(const char* file);

    FileError::FileErrorEnum CopyDir(const char* from, const char* to, bool move);
    FileError::FileErrorEnum CopyFile(const char* from, const char* to, bool move);
    void GetFileList(const char* path, std::vector<std::string>& files);
    FileError::FileErrorEnum Create(const char* path, bool dir);
    FileError::FileErrorEnum Delete(const char* path);
    
    virtual void FileReader_readAsDataURL(int callbackID, const FileReader& self, const File& file);
    virtual void FileReader_readAsText(int callbackID, const FileReader& self, const File& file, const char* encoding);
    virtual void FileWriter_write(int callbackID, const FileWriter& self, const char* data);
    virtual void FileWriter_truncate(int callbackID, const FileWriter& self, uint64 size);
    virtual void Entry_getMetadata(int callbackID, const Entry& self);
    virtual void Entry_moveTo(int callbackID, const Entry& self, const DirectoryEntry& parent, const char* newName);
    virtual void Entry_copyTo(int callbackID, const Entry& self, const DirectoryEntry& parent, const char* newName);
    virtual void Entry_remove(int callbackID, const Entry& self);
    virtual void Entry_getParent(int callbackID, const Entry& self);
    virtual void FileEntry_file(int callbackID, const FileEntry& self);
    virtual void DirectoryEntry_getDirectory(int callbackID, const DirectoryEntry& self, const char* path, const Flags& options);
    virtual void DirectoryEntry_getFile(int callbackID, const DirectoryEntry& self, const char* path, const Flags& options);
    virtual void DirectoryEntry_removeRecursively(int callbackID, const DirectoryEntry& self);
    virtual void DirectoryReader_readEntries(int callbackID, const DirectoryReader& self);
    virtual void LocalFileSystem_requestFileSystem(int callbackID, const LocalFileSystem& self, LocalFileSystem::LocalFileSystemEnum type, uint32 size);
    virtual void LocalFileSystem_resolveLocalFileSystemURL(int callbackID, const LocalFileSystem& self, const char* url);
    virtual void FileTransfer_upload(int callbackID, const FileTransfer& self, const char* filePath, const char* server, const FileUploadOptions& options);

    virtual void Init();
    virtual void OnReady();
    virtual void Shutdown();

    virtual CIwHTTPPost* GetPost(int callbackID);
private:
    std::string m_Paths[WM_MAX_PATHS];
    std::string m_FSNames[WM_MAX_PATHS];
};
