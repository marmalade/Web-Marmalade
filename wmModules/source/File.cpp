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
#include "IwPath.h"
#include "wmModuleEncoding.h"
#include "File.h"
#include <algorithm>

static bool DeviceIsWin()
{
    return (s3eDeviceOSID)s3eDeviceGetInt(S3E_DEVICE_OS) == S3E_OS_ID_WINDOWS;
}

void CFile::Init()
{
    IwTrace(WM_MODULE_FILE,("File: Init."));
    s3eFileList* list = s3eFileListDirectory(NULL);

    char buffer[256];

    while (s3eFileListNext(list, buffer, 256) != S3E_RESULT_ERROR)
    {
        if (!strcmp(buffer,"tmp://"))
        {
            m_Paths[0] = "tmp://wm";
            s3eFileMakeDirectory(m_Paths[0].c_str());
        }
    }

    s3eFileListClose(list);

    if (m_Paths[0] == "") 
    {
        m_Paths[0] = "ram://tmp/wm";
        s3eFileMakeDirectory("ram://tmp");
        s3eFileMakeDirectory(m_Paths[0].c_str());
    }

    m_Paths[1] = "wm";
    s3eFileMakeDirectory(m_Paths[1].c_str());

    m_Paths[2] = "raw://";

    m_FSNames[0] = "tmp";
    m_FSNames[1] = "wm";
    m_FSNames[2] = "raw";

    tiniconv_init_charsets();
}

void CFile::OnReady()
{
    IwTrace(WM_MODULE_FILE, ("File: OnReady."));
}

void CFile::Shutdown()
{
    IwTrace(WM_MODULE_FILE, ("File: Shutdown."));
}

CFile::FileError::FileErrorEnum CFile::CheckEntry(const CFile::Entry& entry, int* fileSystem)
{
    for (int i=0; i<WM_MAX_PATHS; i++)
    {
        if (!strnicmp(entry.fullPath.c_str(), m_Paths[i].c_str(), m_Paths[i].size()))
        {
            if (fileSystem != NULL)
            {
                (*fileSystem) = i;
            }
            if (entry.isDirectory)
            {
                if (entry.fullPath != std::string("raw://") && s3eFileGetFileInt(entry.fullPath.c_str(), S3E_FILE_ISDIR) == 0)
                    return FileError::NOT_FOUND_ERR;
            }

            if (entry.isFile && s3eFileGetFileInt(entry.fullPath.c_str(), S3E_FILE_ISFILE) == 0)
                return FileError::NOT_FOUND_ERR;

            return (FileError::FileErrorEnum)0;
        }
    }
    return FileError::SECURITY_ERR;
}

CFile::FileError::FileErrorEnum CFile::CheckEntry(const char* file)
{
    for (int i=0; i<WM_MAX_PATHS; i++)
    {
        if (!strnicmp(file, m_Paths[i].c_str(), m_Paths[i].size()))
        {
            if (s3eFileGetFileInt(file, S3E_FILE_ISFILE) == 0)
                return FileError::NOT_FOUND_ERR;

            return (FileError::FileErrorEnum)0;
        }
    }
    return FileError::SECURITY_ERR;
}

//C++ user code
void CFile::Entry_getMetadata(int callbackID, const Entry& self)
{
    FileError::FileErrorEnum err = CheckEntry(self);
    if (err != (FileError::FileErrorEnum)0)
    {
        errorCallback(callbackID,FileError(err));
        return;
    }

    uint64 time = s3eFileGetFileInt(self.fullPath.c_str(), S3E_FILE_MODIFIED_DATE);
    if (time == (uint64)-1)
        errorCallback(callbackID,FileError(FileError::NOT_FOUND_ERR));
    else
        metadataSuccessCallback(callbackID,Metadata(time));
}

void CFile::GetFileList(const char* path, std::vector<std::string>& files)
{
    char File[256];
    s3eFileList* list = s3eFileListDirectory(path);

    while (s3eFileListNext(list, File, 256) != S3E_RESULT_ERROR)
    {
        files.push_back(File);
    }
    s3eFileListClose(list);
}

CFile::FileError::FileErrorEnum CFile::CopyDir(const char* from, const char* to, bool move)
{
    char Buffer[256];
    s3eFileList* list;

    if (s3eFileGetFileInt(to, S3E_FILE_ISFILE))
    {
        return FileError::INVALID_MODIFICATION_ERR;
    }

    if (!strnicmp(from, to, strlen(from)) && (to[strlen(from)] == 0 || to[strlen(from)] == '/'))
    {
        return FileError::INVALID_MODIFICATION_ERR;
    }

    if (s3eFileGetFileInt(to, S3E_FILE_ISDIR))
    {
        if (move)
        {
            list = s3eFileListDirectory(to);

            if (s3eFileListNext(list, Buffer, 256) != S3E_RESULT_ERROR)
            {
                s3eFileListClose(list);
                return FileError::INVALID_MODIFICATION_ERR;
            }
            s3eFileListClose(list);
        }
    }
    else
    if (s3eFileMakeDirectory(to) != S3E_RESULT_SUCCESS)
    {
        return FileError::NO_MODIFICATION_ALLOWED_ERR;
    }
    
    std::vector<std::string> files;
    GetFileList(from, files);

    for (int i=0; i<(int)files.size(); i++)
    {
        char FromPath[256];
        strcpy(FromPath, from);
        IwPathJoin(FromPath, files[i].c_str(), 256);

        char ToPath[256];
        strcpy(ToPath, to);
        IwPathJoin(ToPath, files[i].c_str(), 256);

        if (s3eFileGetFileInt(FromPath, S3E_FILE_ISFILE))
        {
            FileError::FileErrorEnum err = CopyFile(FromPath, ToPath, move);
            if (err != (FileError::FileErrorEnum)0)
                return err;
        }
        else
        {
            FileError::FileErrorEnum err = CopyDir(FromPath, ToPath, move);
            if (err != (FileError::FileErrorEnum)0)
                return err;

            if (move)
                s3eFileDeleteDirectory(FromPath);
        }
    }
    if (move)
        s3eFileDeleteDirectory(from);

    return (FileError::FileErrorEnum)0;
}

CFile::FileError::FileErrorEnum CFile::CopyFile(const char* from, const char* to, bool move)
{
    if (move)
    {
        if (s3eFileRename(from, to) == S3E_RESULT_SUCCESS)
            return (FileError::FileErrorEnum)0;

        s3eFileError fErr = s3eFileGetError();
        switch (fErr)
        {
            case S3E_FILE_ERR_NOT_FOUND:
            case S3E_FILE_ERR_ACCESS:
                return FileError::INVALID_MODIFICATION_ERR;
            case S3E_FILE_ERR_EXISTS:
                return FileError::INVALID_MODIFICATION_ERR;
            default:
                return FileError::NOT_FOUND_ERR;
        }
    }
    else
    {
        s3eFile* fromFP = s3eFileOpen(from, "rb");
        if (fromFP == NULL)
            return FileError::NOT_READABLE_ERR;

        s3eFile* toFP = s3eFileOpen(to, "wb");
        if (toFP == NULL)
        {
            s3eFileClose(fromFP);
            return FileError::NOT_READABLE_ERR;
        }

        char Buffer[4096];

        int size = s3eFileRead(Buffer, 1, 4096, fromFP);
        while (size > 0)
        {
            s3eFileWrite(Buffer, 1, size, toFP);
            size = s3eFileRead(Buffer, 1, 4096, fromFP);
        }

        
        s3eFileClose(toFP);
        s3eFileClose(fromFP);
    }
    return (FileError::FileErrorEnum)0;
}

void CFile::Entry_moveTo(int callbackID, const Entry& self, const DirectoryEntry& parent, const char* newName)
{
    FileError::FileErrorEnum err = CheckEntry(self);
    if (err != (FileError::FileErrorEnum)0)
    {
        errorCallback(callbackID,FileError(err));
        return;
    }

    FileError::FileErrorEnum errParent = CheckEntry(parent);
    if (errParent != (FileError::FileErrorEnum)0)
    {
        errorCallback(callbackID,FileError(errParent));
        return;
    }

    Entry newFile(self.isFile, self.isDirectory, "", parent.fullPath.c_str());
    if (newName[0] != 0)
        newFile.name = newName;
    else
        newFile.name = self.name;
    newFile.fullPath += "/" + newFile.name;

    if (self.fullPath == newFile.fullPath)
    {
        errorCallback(callbackID, FileError(FileError::INVALID_MODIFICATION_ERR));
        return;
    }

    if (newFile.isDirectory)
    {
        err = CopyDir(self.fullPath.c_str(), newFile.fullPath.c_str(), true);
        if (err != (FileError::FileErrorEnum)0)
        {
            errorCallback(callbackID,FileError(err));
        }
        else
        {
            entrySuccessCallback(callbackID,newFile);
        }
    }
    else
    {
        if (s3eFileCheckExists(newFile.fullPath.c_str()))
        {
            if (s3eFileDelete(newFile.fullPath.c_str()) != S3E_RESULT_SUCCESS)
            {
                errorCallback(callbackID,FileError(FileError::SECURITY_ERR));
                return;
            }
        }
        err = CopyFile(self.fullPath.c_str(), newFile.fullPath.c_str(), true);
        if (err != (FileError::FileErrorEnum)0)
            errorCallback(callbackID,FileError(err));
        else
            entrySuccessCallback(callbackID,newFile);
    }
}

void CFile::Entry_copyTo(int callbackID, const Entry& self, const DirectoryEntry& parent, const char* newName)
{
    FileError::FileErrorEnum err = CheckEntry(self);
    if (err != (FileError::FileErrorEnum)0)
    {
        errorCallback(callbackID,FileError(err));
        return;
    }

    FileError::FileErrorEnum errParent = CheckEntry(parent);
    if (errParent != (FileError::FileErrorEnum)0)
    {
        errorCallback(callbackID,FileError(errParent));
        return;
    }

    Entry newFile(self.isFile, self.isDirectory, "", parent.fullPath.c_str());
    if (newName[0] != 0)
        newFile.name = newName;
    else
        newFile.name = self.name;
    newFile.fullPath += "/" + newFile.name;

    if (self.fullPath == newFile.fullPath)
    {
        errorCallback(callbackID, FileError(FileError::INVALID_MODIFICATION_ERR));
        return;
    }

    if (newFile.isDirectory)
    {
        err = CopyDir(self.fullPath.c_str(), newFile.fullPath.c_str(), false);
        if (err != (FileError::FileErrorEnum)0)
        {
            errorCallback(callbackID,FileError(err));
        }
        else
        {
            entrySuccessCallback(callbackID,newFile);
        }
    }
    else
    {
        if (s3eFileCheckExists(newFile.fullPath.c_str()))
        {
            if (s3eFileDelete(newFile.fullPath.c_str()) != S3E_RESULT_SUCCESS)
            {
                errorCallback(callbackID,FileError(FileError::SECURITY_ERR));
                return;
            }
        }
        err = CopyFile(self.fullPath.c_str(), newFile.fullPath.c_str(), false);
        if (err != (FileError::FileErrorEnum)0)
            errorCallback(callbackID,FileError(err));
        else
            entrySuccessCallback(callbackID,newFile);
    }
}

void CFile::Entry_remove(int callbackID, const Entry& self)
{
    int fileSystem;
    FileError::FileErrorEnum err = CheckEntry(self, &fileSystem);
    if (err != (FileError::FileErrorEnum)0)
    {
        errorCallback(callbackID,FileError(err));
        return;
    }
    
    if (self.isDirectory)
    {
        if (m_Paths[fileSystem] == self.fullPath)
        {
            errorCallback(callbackID,FileError(FileError::NO_MODIFICATION_ALLOWED_ERR));
        }
        else
        if (s3eFileDeleteDirectory(self.fullPath.c_str()) != S3E_RESULT_SUCCESS)
        {
            s3eFileError fErr = s3eFileGetError();
            FileError::FileErrorEnum err;
            switch (fErr)
            {
                case S3E_FILE_ERR_NOT_FOUND:
                case S3E_FILE_ERR_ACCESS:
                    err = FileError::INVALID_MODIFICATION_ERR;
                    break;
                case S3E_FILE_ERR_EXISTS:
                    err = FileError::NO_MODIFICATION_ALLOWED_ERR;
                    break;
                case S3E_FILE_ERR_NOT_EMPTY:
                    err = FileError::INVALID_MODIFICATION_ERR;
                    break;
                default:
                    err = FileError::NOT_FOUND_ERR;
            }
            errorCallback(callbackID,FileError(err));
        }
        else
            successCallback(callbackID);
    }
    else
    {
        if (s3eFileDelete(self.fullPath.c_str()) != S3E_RESULT_SUCCESS)
        {
            s3eFileError fErr = s3eFileGetError();
            FileError::FileErrorEnum err;
            switch (fErr)
            {
                case S3E_FILE_ERR_NOT_FOUND:
                case S3E_FILE_ERR_ACCESS:
                    err = FileError::INVALID_MODIFICATION_ERR;
                    break;
                case S3E_FILE_ERR_EXISTS:
                    err = FileError::NO_MODIFICATION_ALLOWED_ERR;
                    break;
                default:
                    err = FileError::NOT_FOUND_ERR;
            }
            errorCallback(callbackID,FileError(err));
        }
        else
            successCallback(callbackID);
    }
}

void CFile::Entry_getParent(int callbackID, const Entry& self)
{
    int fileSystem;
    FileError::FileErrorEnum err = CheckEntry(self, &fileSystem);
    if (err != (FileError::FileErrorEnum)0)
    {
        errorCallback(callbackID,FileError(err));
        return;
    }

    DirectoryEntry dir(false, true, self.name.c_str(), self.fullPath.c_str());

    if (m_Paths[fileSystem] == self.fullPath)
    {
        directoryEntrySuccessCallback(callbackID, dir);
    }
    else
    {
        dir.fullPath.resize(dir.fullPath.size() - dir.name.size() - 1);
        
        if (m_Paths[fileSystem] == dir.fullPath)
        {
            dir.name = "";
        }
        else
        {
            char* ptr = strrchr(dir.fullPath.c_str(), '/');
            dir.name = ptr + 1;
        }
        directoryEntrySuccessCallback(callbackID, dir);
    }
}

void CFile::FileEntry_file(int callbackID, const FileEntry& self)
{
    FileError::FileErrorEnum err = CheckEntry(self);
    if (err != (FileError::FileErrorEnum)0)
    {
        errorCallback(callbackID,FileError(err));
        return;
    }

    uint64 date = s3eFileGetFileInt(self.fullPath.c_str(), S3E_FILE_MODIFIED_DATE);
    uint64 size = s3eFileGetFileInt(self.fullPath.c_str(), S3E_FILE_SIZE);

    File file(self.name.c_str(), self.fullPath.c_str(), "", date, size);

    fileSuccessCallback(callbackID, file);
}

CFile::FileError::FileErrorEnum CFile::Create(const char* path, bool dir)
{
    if (dir && s3eFileGetFileInt(path, S3E_FILE_ISFILE) != 0)
        return FileError::TYPE_MISMATCH_ERR;
    if (!dir && s3eFileGetFileInt(path, S3E_FILE_ISFILE) != 0)
        return (FileError::FileErrorEnum)0;

    if (!dir && s3eFileGetFileInt(path, S3E_FILE_ISDIR) != 0)
        return FileError::TYPE_MISMATCH_ERR;
    if (dir && s3eFileGetFileInt(path, S3E_FILE_ISDIR) != 0)
        return (FileError::FileErrorEnum)0;

    char Buffer[1024];
    strcpy(Buffer, path);
    char* ptr = strrchr(Buffer, '/');
    ptr[0] = 0;

    if (s3eFileGetFileInt(Buffer, S3E_FILE_ISDIR) == 0)
        return dir ? FileError::PATH_EXISTS_ERR : FileError::INVALID_MODIFICATION_ERR;

    if (dir)
        s3eFileMakeDirectory(path);
    else
    {
        s3eFile* fp = s3eFileOpen(path,"w");
        s3eFileClose(fp);
    }
    return (FileError::FileErrorEnum)0;
}

void CFile::DirectoryEntry_getDirectory(int callbackID, const DirectoryEntry& self, const char* path, const Flags& options)
{
    int fileSystem;
    FileError::FileErrorEnum err = CheckEntry(self, &fileSystem);
    if (err != (FileError::FileErrorEnum)0)
    {
        errorCallback(callbackID,FileError(err));
        return;
    }

    DirectoryEntry dir(false, true, "", "");

    if (path[0] == '/')     //absolute path
    {
        if (s3eFileGetFileInt(path + 1, S3E_FILE_ISDIR) != 0)
            dir.fullPath = path + 1;
        else
            dir.fullPath = m_Paths[fileSystem] + path;
    }
    else    //relative path
    {
        dir.fullPath = self.fullPath + "/" + path;
    }

    char Buffer[1024];
    
    strcpy(Buffer, dir.fullPath.c_str());
    IwPathNorm(Buffer, true);
    dir.fullPath = Buffer;

    if (dir.fullPath.size() < m_Paths[fileSystem].size())
    {
        errorCallback(callbackID,FileError(FileError::ENCODING_ERR));
        return;
    }

    char* ptr = strrchr(Buffer, '/');
    if (dir.fullPath == m_Paths[fileSystem])
        dir.name = "";
    else
        dir.name = ptr + 1;

    err = CheckEntry(dir);
    if (err == FileError::SECURITY_ERR)
    {
        errorCallback(callbackID,FileError(err));
        return;
    }

    if (err == FileError::NOT_FOUND_ERR)
    {
        if (s3eFileGetFileInt(dir.fullPath.c_str(), S3E_FILE_ISFILE) != 0)
        {
            errorCallback(callbackID,FileError(FileError::TYPE_MISMATCH_ERR));
            return;
        }

        if (!options.create)
        {
            errorCallback(callbackID,FileError(FileError::NOT_FOUND_ERR));
            return;
        }
        else
        {
            err = Create(dir.fullPath.c_str(), true);
            if (err != (FileError::FileErrorEnum)0)
            {
                errorCallback(callbackID, FileError(err));
                return;
            }
        }
    }
    else if (options.create && options.exclusive)
    {
        errorCallback(callbackID,FileError(FileError::PATH_EXISTS_ERR));
        return;
    }

    directoryEntrySuccessCallback(callbackID, dir);
}

void CFile::DirectoryEntry_getFile(int callbackID, const DirectoryEntry& self, const char* path, const Flags& options)
{
    int fileSystem;

    FileError::FileErrorEnum err = CheckEntry(self, &fileSystem);
    if (err != (FileError::FileErrorEnum)0)
    {
        errorCallback(callbackID,FileError(err));
        return;
    }

    FileEntry file(true, false, "", "");

    if (path[0] == '/')     //absolute path
    {
        if (s3eFileGetFileInt(path + 1, S3E_FILE_ISFILE) != 0) 
            file.fullPath = path + 1;
        else 
            file.fullPath = m_Paths[fileSystem] + path;            
    }
    else    //relative path
    {
        file.fullPath = self.fullPath + "/" + path;
    }
    char Buffer[1024];

    strcpy(Buffer, file.fullPath.c_str());
    IwPathNorm(Buffer, true); //raw:///path/  raw://path.
    
    file.fullPath = Buffer;
    int pos = file.fullPath.find("raw://");
    if (pos>=0)
    {
        if (!DeviceIsWin())
        {
            file.fullPath.insert(pos+4, std::string("/"));
        }
    }

    if (file.fullPath.size() < m_Paths[fileSystem].size())
    {
        errorCallback(callbackID,FileError(FileError::ENCODING_ERR));
        return;
    }

    char* ptr = strrchr(Buffer, '/');
    if (file.fullPath == m_Paths[fileSystem])
        file.name = "";
    else
        file.name = ptr + 1;

    err = CheckEntry(file);
    if (err == FileError::SECURITY_ERR)
    {
        errorCallback(callbackID,FileError(err));
        return;
    }

    if (err == FileError::NOT_FOUND_ERR)
    {
        if (s3eFileGetFileInt(file.fullPath.c_str(), S3E_FILE_ISDIR) != 0)
        {
            errorCallback(callbackID,FileError(FileError::TYPE_MISMATCH_ERR));
            return;
        }
        if (!options.create)
        {
            errorCallback(callbackID,FileError(FileError::NOT_FOUND_ERR));
            return;
        }
        else
        {
            err = Create(file.fullPath.c_str(), false);
            if (err != (FileError::FileErrorEnum)0)
            {
                errorCallback(callbackID, FileError(err));
                return;
            }
        }
    }
    else if (options.create && options.exclusive)
    {
        errorCallback(callbackID,FileError(FileError::PATH_EXISTS_ERR));
        return;
    }
    fileEntrySuccessCallback(callbackID, file);
}

CFile::FileError::FileErrorEnum CFile::Delete(const char* path)
{
    std::vector<std::string> files;
    GetFileList(path, files);

    for (int i=0; i<(int)files.size(); i++)
    {
        char NextPath[256];
        strcpy(NextPath, path);
        IwPathJoin(NextPath, files[i].c_str(), 256);

        if (s3eFileGetFileInt(NextPath, S3E_FILE_ISFILE))
        {
            if (s3eFileDelete(NextPath) != S3E_RESULT_SUCCESS)
            {
                return FileError::NO_MODIFICATION_ALLOWED_ERR;
            }
        }
        else
        {
            FileError::FileErrorEnum err = Delete(NextPath);
            if (err != (FileError::FileErrorEnum)0)
            {
                return err;
            }
        }
    }
    if (s3eFileDeleteDirectory(path) != S3E_RESULT_SUCCESS)
    {
        return FileError::NO_MODIFICATION_ALLOWED_ERR;
    }

    return (FileError::FileErrorEnum)0;
}

void CFile::DirectoryEntry_removeRecursively(int callbackID, const DirectoryEntry& self)
{
    int fileSystem;
    FileError::FileErrorEnum err = CheckEntry(self, &fileSystem);
    if (err != (FileError::FileErrorEnum)0)
    {
        errorCallback(callbackID,FileError(err));
        return;
    }
    
    if (m_Paths[fileSystem] == self.fullPath)
    {
        errorCallback(callbackID,FileError(FileError::NO_MODIFICATION_ALLOWED_ERR));
    }
    else
    {
        err = Delete(self.fullPath.c_str());
        if (err != (FileError::FileErrorEnum)0)
        {
            errorCallback(callbackID, FileError(err));
            return;
        }
        successCallback(callbackID);
    }
}

void CFile::DirectoryReader_readEntries(int callbackID, const DirectoryReader& self)
{
    DirectoryEntry dir(false, true, "", self.fullPath.c_str());

    FileError::FileErrorEnum err = CheckEntry(dir);
    if (err != (FileError::FileErrorEnum)0)
    {
        errorCallback(callbackID,FileError(err));
        return;
    }
    
    std::vector<std::string> files;
    GetFileList(self.fullPath.c_str(), files);

    std::vector<Entry> list;

    for (int i=0; i<(int)files.size(); i++)
    {
        char Path[256];
        strcpy(Path, self.fullPath.c_str());
        IwPathJoin(Path, files[i].c_str(), 256);

        Entry entry(false, false, "", Path);

        entry.isFile = s3eFileGetFileInt(Path, S3E_FILE_ISFILE)?true:false;
        entry.isDirectory = s3eFileGetFileInt(Path, S3E_FILE_ISDIR)?true:false;
        
        char Buffer[256];
        IwPathGetFilename(entry.fullPath.c_str(), Buffer, true);
        entry.name = Buffer;

        list.push_back(entry);
    }

    entriesSuccessCallback(callbackID, list);
}

void CFile::LocalFileSystem_requestFileSystem(int callbackID, const LocalFileSystem& self, LocalFileSystem::LocalFileSystemEnum type, unsigned int size)
{
    if ((int)type >= WM_MAX_PATHS)
    {
        errorCallback(callbackID, FileError(FileError::NOT_FOUND_ERR));
        return;
    }

    DirectoryEntry dir(false, true, "", m_Paths[type].c_str());
    FileSystem fs(m_Paths[type].c_str(), dir);

    fileSystemSuccessCallback(callbackID, fs);
}

void CFile::LocalFileSystem_resolveLocalFileSystemURL(int callbackID, const LocalFileSystem& self, const char* url)
{
    //expecting file:///[wm|tmp]/path/to/file.ext
    if (strncmp(url,"file:///",8))
    {
        errorCallback(callbackID, FileError(FileError::ENCODING_ERR));
        return;
    }

    int i;
    Entry entry(false, false, "", url + 8);

    for (i=0; i<(int)entry.fullPath.size(); i++)
        if (entry.fullPath[i] == '\\')
            entry.fullPath[i] = '/';

    if (entry.fullPath.size()>0 && entry.fullPath[entry.fullPath.size()-1]=='/')
        entry.fullPath.resize(entry.fullPath.size()-1);

    for (i=0; i<WM_MAX_PATHS; i++)
    {
        if (!strnicmp(entry.fullPath.c_str(), m_FSNames[i].c_str(), m_FSNames[i].size()))
        {
            entry.fullPath = m_Paths[i] + entry.fullPath.substr(m_FSNames[i].size());
            break;
        }
        if (!strnicmp(entry.fullPath.c_str(), m_Paths[i].c_str(), m_Paths[i].size()))
        {
            entry.fullPath = m_Paths[i] + entry.fullPath.substr(m_FSNames[i].size());
            break;
        }
    }

    entry.isFile = (s3eFileGetFileInt(entry.fullPath.c_str(), S3E_FILE_ISFILE) == 1);
    entry.isDirectory = (s3eFileGetFileInt(entry.fullPath.c_str(), S3E_FILE_ISDIR) == 1);

    char Buffer[256];
    IwPathGetFilename(entry.fullPath.c_str(), Buffer, true);
    entry.name = Buffer;
        
    int fileSystem = 0;
    FileError::FileErrorEnum err = CheckEntry(entry, &fileSystem);
    if (err != (FileError::FileErrorEnum)0)
    {
        errorCallback(callbackID, FileError(err));
        return;
    }
    if (m_Paths[fileSystem] == entry.fullPath)
    {
        entry.name = "";
    }

    if (!entry.isFile && !entry.isDirectory)
    {
        errorCallback(callbackID, FileError(FileError::NOT_FOUND_ERR));
        return;
    }

    entrySuccessCallback(callbackID, entry);
}

static const unsigned char base64_table[65] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

size_t base64_size(const char *src, size_t len)
{
    size_t olen;

    olen = len * 4 / 3 + 4; /* 3-byte blocks to 4-byte */
    olen += olen / 72; /* line feeds */
    olen++; /* nul termination */

    return olen;
}

char* base64_encode(const char *src, size_t len, char *out)
{
        char *pos;
        const char *end, *in;
        if (out == NULL)
                return NULL;

        end = src + len;
        in = src;
        pos = out;

        while (end - in >= 3) {
                *pos++ = base64_table[in[0] >> 2];
                *pos++ = base64_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
                *pos++ = base64_table[((in[1] & 0x0f) << 2) | (in[2] >> 6)];
                *pos++ = base64_table[in[2] & 0x3f];
                in += 3;
        }

        if (end - in) {
                *pos++ = base64_table[in[0] >> 2];
                if (end - in == 1) {
                        *pos++ = base64_table[(in[0] & 0x03) << 4];
                        *pos++ = '=';
                } else {
                        *pos++ = base64_table[((in[0] & 0x03) << 4) |
                                              (in[1] >> 4)];
                        *pos++ = base64_table[(in[1] & 0x0f) << 2];
                }
                *pos++ = '=';
        }

        *pos = '\0';
        return out;
}

#define DATA_URL_HDR_SIZE 13

void CFile::FileReader_readAsDataURL(int callbackID, const FileReader& self, const File& file)
{
    if (s3eFileGetFileInt(file.fullPath.c_str(), S3E_FILE_ISFILE) == 0)
    {
        errorCallback(callbackID,FileError(FileError::NOT_FOUND_ERR));
        return;
    }

    s3eFile* fp = s3eFileOpen(file.fullPath.c_str() ,"rb");
    if (fp == NULL)
    {
        errorCallback(callbackID, FileError(FileError::NOT_READABLE_ERR));
        return;
    }

    const char* mimeType = "";
    char ext[32];

    IwPathGetExtension(file.fullPath.c_str(), ext);

    for (int i=0; g_MimeTypeTable[i].m_Extension!=NULL; i++)
        if (!stricmp(g_MimeTypeTable[i].m_Extension, ext))
        {
            mimeType = g_MimeTypeTable[i].m_MimeType;
            break;
        }

    char buffer[BUFFER_SIZE];
    int size = s3eFileGetSize(fp);
    int pos = 0;

    while (pos < size)
    {
        int offset = s3eFileRead(buffer, 1, BUFFER_SIZE, fp);
        
        size_t out_size = base64_size(buffer, offset);
        if (pos == 0)
            out_size += DATA_URL_HDR_SIZE + strlen(mimeType);
        char* out = new char[out_size];
        
        if (pos == 0)
        {
            sprintf(out, "data:%s;base64,", mimeType);
            base64_encode(buffer, offset, out + DATA_URL_HDR_SIZE + strlen(mimeType));
        }
        else
            base64_encode(buffer, offset, out);
        
        pos = s3eFileTell(fp);

        readCallback(callbackID, out, pos, size);

        delete out;
    }
    s3eFileClose(fp);
}

void CFile::FileReader_readAsText(int callbackID, const FileReader& self, const File& file, const char* encoding)
{
    if (s3eFileGetFileInt(file.fullPath.c_str(), S3E_FILE_ISFILE) == 0)
    {
        errorCallback(callbackID,FileError(FileError::NOT_FOUND_ERR));
        return;
    }

    s3eFile* fp = s3eFileOpen(file.fullPath.c_str() ,"rb");
    if (fp == NULL)
    {
        errorCallback(callbackID, FileError(FileError::NOT_READABLE_ERR));
        return;
    }

    char buffer[BUFFER_SIZE+1];
    char buffer2[BUFFER_SIZE+1];
    int size = s3eFileGetSize(fp);
    int pos = 0;

    int encodingType = TINICONV_CHARSET_UFT_8;

    if (encoding[0] != 0)
    {
        for (int i=0; g_EncodingTable[i].m_Name!=NULL; i++)
            if (!stricmp(g_EncodingTable[i].m_Name, encoding))
            {
                encodingType = g_EncodingTable[i].m_Type;
                break;
            }
    }
    else
    {
        s3eFileRead(buffer, 1, 3, fp);
        s3eFileSeek(fp, 0, S3E_FILESEEK_SET);

        if (buffer[0] == 0xFE && buffer[1] == 0xFF)
            encodingType = TINICONV_CHARSET_UCS_2;
        else if (buffer[0] == 0xFF && buffer[1] == 0xFE)
            encodingType = TINICONV_CHARSET_UCS_2;
        else if (buffer[0] == 0xEF && buffer[1] == 0xBB && buffer[2] == 0xBF)
            encodingType = TINICONV_CHARSET_UFT_8;
    }

    struct tiniconv_ctx_s conv;
    tiniconv_init(encodingType, TINICONV_CHARSET_UFT_8, 0, &conv);

    int real_size = 0;
    while (pos < size)
    {
        int offset = s3eFileRead(buffer, 1, BUFFER_SIZE, fp);
        pos = s3eFileTell(fp);
        buffer[offset] = 0;

        char* ptr = buffer;

        if (encodingType != TINICONV_CHARSET_UFT_8)
        {
            int out_size = 0;
            int result = tiniconv_convert(&conv, (unsigned char*)buffer, offset, NULL, (unsigned char*)buffer2, BUFFER_SIZE, &out_size);
            if (result != 0)
            {
                errorCallback(callbackID, FileError(FileError::ENCODING_ERR));
                return;
            }
            buffer2[out_size] = 0;
            ptr = buffer2;
            real_size += out_size;
        }
        else
            real_size += offset;
        if (pos == size)
            pos = size = real_size;

        readCallback(callbackID, ptr, pos, size);
    }
    s3eFileClose(fp);
}


void CFile::FileWriter_write(int callbackID, const FileWriter& self, const char* data)
{

    FileError::FileErrorEnum err = CheckEntry(self.fileName.c_str());
    if (err == FileError::SECURITY_ERR)
    {
        errorCallback(callbackID,FileError(err));
        return;
    }

    // following the phonegap code not the spec
    const char* fpFlag = "w";

    if (self.position > 0)
    {
        if (err == FileError::NOT_FOUND_ERR)
        {
            errorCallback(callbackID, FileError(FileError::NOT_FOUND_ERR));
            return;
        }
        fpFlag = "a";
        s3eFileTruncate(self.fileName.c_str(), (int)self.position);
    }

    s3eFile* fp = s3eFileOpen(self.fileName.c_str() ,fpFlag);
    if (fp == NULL)
    {
        s3eFileError fErr = s3eFileGetError();
        switch (fErr)
        {
            case S3E_FILE_ERR_NOT_FOUND:
                err = FileError::NOT_FOUND_ERR;
                break;
            default:
                err = FileError::NO_MODIFICATION_ALLOWED_ERR;
        }
        errorCallback(callbackID,FileError(err));
        return;
    }

    s3eFileSeek(fp, (int)self.position, S3E_FILESEEK_SET);
    s3eFileWrite(data, 1, strlen(data), fp);

    int position = s3eFileTell(fp);
    int length = s3eFileGetSize(fp);

    s3eFileClose(fp);

    writeCallback(callbackID, position, length);
}

void CFile::FileWriter_truncate(int callbackID, const FileWriter& self, uint64 length)
{
    FileError::FileErrorEnum err = CheckEntry(self.fileName.c_str());
    if (err != (FileError::FileErrorEnum)0)
    {
        errorCallback(callbackID,FileError(err));
        return;
    }

    if (s3eFileTruncate(self.fileName.c_str(), (int)length) != S3E_RESULT_SUCCESS)
    {
        s3eFileError fErr = s3eFileGetError();
        switch (fErr)
        {
            case S3E_FILE_ERR_NOT_FOUND:
                err = FileError::NOT_FOUND_ERR;
                break;
            default:
                err = FileError::NO_MODIFICATION_ALLOWED_ERR;
        }
        errorCallback(callbackID,FileError(err));
        return;
    }

    uint64 position =  std::min(self.position, length);

    writeCallback(callbackID, position, length);
}

static int32 GotData(int num, CIwHTTPPost* post)
{
    if (post->GetStatus() == S3E_RESULT_ERROR)
    {
        post->m_Module->transferErrorCallback(post->m_ID, CFile::FileTransferError(CFile::FileTransferError::CONNECTION_ERR));
    }
    else
    {
        post->m_Response[num]=0;
        post->m_Module->uploadSuccessCallback(post->m_ID,
            CFile::FileUploadResult(post->ContentSent(), post->GetResponseCode(), post->m_Response));
        delete post;
    }
    return 0;
}

static int32 GotHeaders(CIwHTTP* http, CIwHTTPPost* post)
{
    if (post->GetStatus() == S3E_RESULT_ERROR)
    {
        post->m_Module->transferErrorCallback(post->m_ID, CFile::FileTransferError(CFile::FileTransferError::CONNECTION_ERR));
    }
    else if (http->ContentExpected())
    {
        post->m_Response = new char[http->ContentExpected()+1];
        http->ReadDataAsync(post->m_Response, http->ContentExpected(), 1000, (s3eCallback)GotData, post);
        return 0;
    }
    
    delete post;
    return 0;
}

CIwHTTPPost* CFile::GetPost(int callbackID)
{
    return new CIwHTTPPost(this, callbackID);
}

void CFile::FileTransfer_upload(int callbackID, const CFile::FileTransfer& self, const char* filePath,
    const char* server, const CFile::FileUploadOptions& options)
{
    CIwHTTPPost* post = GetPost(callbackID);
    post->SetPostChunkedMode(options.chunkedMode);

    std::map<std::string, std::string>::const_iterator it = options.params.begin();
    for (;it != options.params.end(); ++it)
    {
        // NB We had to change this from SetRequestHeader to SetFormData.  The FB example highlighted
        // that PG pass the params through as body data.  HOWEVER (! TODO !) they allow an undocced
        // JSON sub object of options, namely headers : {[list]}, and each of these headers will be
        // set as headers rather than form data.
        post->SetFormData(it->first.c_str(), it->second.c_str());
    }
    if (!strncmp("file:///",filePath, 8))
        filePath = filePath + 8;

    if (post->SetFormDataFile(options.fileKey.c_str(), filePath,
        options.fileName.c_str(), options.mimeType.c_str()) != S3E_RESULT_SUCCESS)
    {
        delete post;

        transferErrorCallback(callbackID, FileTransferError(FileTransferError::FILE_NOT_FOUND_ERR));
        return;
    }
    
    if (post->Post(server, "", 0, (s3eCallback)GotHeaders, post) != S3E_RESULT_SUCCESS)
    {
        delete post;

        transferErrorCallback(callbackID, FileTransferError(FileTransferError::INVALID_URL_ERR));
        return;
    }
}
