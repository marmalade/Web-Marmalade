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
 * Generated class for JavaScript bridge module file
 * Some sections of this module are re-generated when the interface changes the rest is generated once
 */


class CFileBase : public CIwWMDispatcherModule<CFileBase>
{
public:
    CFileBase(CIwWMDispatcherBase* dispatcher);

    class File
    {
    public:
    public:
        std::string name;
        std::string fullPath;
        std::string type;
        uint64 lastModifiedDate;
        uint64 size;

        File(const char* _name,const char* _fullPath,const char* _type,uint64 _lastModifiedDate,uint64 _size) : name(_name), fullPath(_fullPath), type(_type), lastModifiedDate(_lastModifiedDate), size(_size) {}

        void To_JSON(Json::Value& value) const;
        static File From_JSON(const Json::Value& value, CFileBase* reader);
    };
    class FileError
    {
    public:
        enum FileErrorEnum
        {
            NOT_FOUND_ERR = 1,
            SECURITY_ERR = 2,
            ABORT_ERR = 3,
            NOT_READABLE_ERR = 4,
            ENCODING_ERR = 5,
            NO_MODIFICATION_ALLOWED_ERR = 6,
            INVALID_STATE_ERR = 7,
            SYNTAX_ERR = 8,
            INVALID_MODIFICATION_ERR = 9,
            QUOTA_EXCEEDED_ERR = 10,
            TYPE_MISMATCH_ERR = 11,
            PATH_EXISTS_ERR = 12,
        };

    public:
        int32 code;

        FileError(int32 _code) : code(_code) {}

        void To_JSON(Json::Value& value) const;
        static FileError From_JSON(const Json::Value& value, CFileBase* reader);
    };
    class FileReader
    {
    public:
        enum FileReaderEnum
        {
            EMPTY = 0,
            LOADING = 1,
            DONE = 2,
        };

    public:
        FileReader::FileReaderEnum readyState;
        std::string result;
        FileError error;

        FileReader(FileReader::FileReaderEnum _readyState,const char* _result,const FileError& _error) : readyState(_readyState), result(_result), error(_error) {}

        void To_JSON(Json::Value& value) const;
        static FileReader From_JSON(const Json::Value& value, CFileBase* reader);
    };
    class FileWriter
    {
    public:
        enum FileWriterEnum
        {
            INIT = 0,
            WRITING = 1,
            DONE = 2,
        };

    public:
        std::string fileName;
        uint64 position;
        uint64 length;
        FileWriter::FileWriterEnum readyState;
        FileError error;

        FileWriter(const char* _fileName,uint64 _position,uint64 _length,FileWriter::FileWriterEnum _readyState,const FileError& _error) : fileName(_fileName), position(_position), length(_length), readyState(_readyState), error(_error) {}

        void To_JSON(Json::Value& value) const;
        static FileWriter From_JSON(const Json::Value& value, CFileBase* reader);
    };
    class Entry
    {
    public:
    public:
        bool isFile;
        bool isDirectory;
        std::string name;
        std::string fullPath;

        Entry(bool _isFile,bool _isDirectory,const char* _name,const char* _fullPath) : isFile(_isFile), isDirectory(_isDirectory), name(_name), fullPath(_fullPath) {}

        void To_JSON(Json::Value& value) const;
        static Entry From_JSON(const Json::Value& value, CFileBase* reader);
    };
    class FileEntry : public Entry
    {
    public:
    public:

        FileEntry(bool _isFile,bool _isDirectory,const char* _name,const char* _fullPath) : Entry(_isFile, _isDirectory, _name, _fullPath) {}

        void To_JSON(Json::Value& value) const;
        static FileEntry From_JSON(const Json::Value& value, CFileBase* reader);
    };
    class DirectoryEntry : public Entry
    {
    public:
    public:

        DirectoryEntry(bool _isFile,bool _isDirectory,const char* _name,const char* _fullPath) : Entry(_isFile, _isDirectory, _name, _fullPath) {}

        void To_JSON(Json::Value& value) const;
        static DirectoryEntry From_JSON(const Json::Value& value, CFileBase* reader);
    };
    class DirectoryReader
    {
    public:
    public:
        std::string fullPath;

        DirectoryReader(const char* _fullPath) : fullPath(_fullPath) {}

        void To_JSON(Json::Value& value) const;
        static DirectoryReader From_JSON(const Json::Value& value, CFileBase* reader);
    };
    class FileSystem
    {
    public:
    public:
        std::string name;
        DirectoryEntry root;

        FileSystem(const char* _name,const DirectoryEntry& _root) : name(_name), root(_root) {}

        void To_JSON(Json::Value& value) const;
        static FileSystem From_JSON(const Json::Value& value, CFileBase* reader);
    };
    class LocalFileSystem
    {
    public:
        enum LocalFileSystemEnum
        {
            TEMPORARY = 0,
            PERSISTENT = 1,
        };

    public:

        LocalFileSystem() {}

        void To_JSON(Json::Value& value) const;
        static LocalFileSystem From_JSON(const Json::Value& value, CFileBase* reader);
    };
    class FileUploadOptions
    {
    public:
    public:
        std::string fileKey;
        std::string fileName;
        std::string mimeType;
        std::map<std::string, std::string> params;
        bool chunkedMode;

        FileUploadOptions(const char* _fileKey,const char* _fileName,const char* _mimeType,const std::map<std::string, std::string>& _params,bool _chunkedMode) : fileKey(_fileKey), fileName(_fileName), mimeType(_mimeType), params(_params), chunkedMode(_chunkedMode) {}

        void To_JSON(Json::Value& value) const;
        static FileUploadOptions From_JSON(const Json::Value& value, CFileBase* reader);
    };
    class FileUploadResult
    {
    public:
    public:
        int32 bytesSent;
        int32 responseCode;
        std::string response;

        FileUploadResult(int32 _bytesSent,int32 _responseCode,const char* _response) : bytesSent(_bytesSent), responseCode(_responseCode), response(_response) {}

        void To_JSON(Json::Value& value) const;
        static FileUploadResult From_JSON(const Json::Value& value, CFileBase* reader);
    };
    class FileTransfer
    {
    public:
    public:

        FileTransfer() {}

        void To_JSON(Json::Value& value) const;
        static FileTransfer From_JSON(const Json::Value& value, CFileBase* reader);
    };
    class Flags
    {
    public:
    public:
        bool create;
        bool exclusive;

        Flags(bool _create,bool _exclusive) : create(_create), exclusive(_exclusive) {}

        void To_JSON(Json::Value& value) const;
        static Flags From_JSON(const Json::Value& value, CFileBase* reader);
    };
    class Metadata
    {
    public:
    public:
        uint64 modificationTime;

        Metadata(uint64 _modificationTime) : modificationTime(_modificationTime) {}

        void To_JSON(Json::Value& value) const;
        static Metadata From_JSON(const Json::Value& value, CFileBase* reader);
    };
    class FileTransferError
    {
    public:
        enum FileTransferErrorEnum
        {
            FILE_NOT_FOUND_ERR = 1,
            INVALID_URL_ERR = 2,
            CONNECTION_ERR = 3,
        };

    public:
        int32 code;

        FileTransferError(int32 _code) : code(_code) {}

        void To_JSON(Json::Value& value) const;
        static FileTransferError From_JSON(const Json::Value& value, CFileBase* reader);
    };

// [[[end]]]

/*
 * Auto Generated section for converting between JavaScript and C++
 */
// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\h.bridge.tmpl") ]]]
private:
    //JS->C++ automated converters
    bool FileReader_readAsDataURL(int callbackID, const Json::Value& parameters);

    bool FileReader_readAsText(int callbackID, const Json::Value& parameters);

    bool FileWriter_write(int callbackID, const Json::Value& parameters);

    bool FileWriter_truncate(int callbackID, const Json::Value& parameters);

    bool Entry_getMetadata(int callbackID, const Json::Value& parameters);

    bool Entry_moveTo(int callbackID, const Json::Value& parameters);

    bool Entry_copyTo(int callbackID, const Json::Value& parameters);

    bool Entry_remove(int callbackID, const Json::Value& parameters);

    bool Entry_getParent(int callbackID, const Json::Value& parameters);

    bool FileEntry_file(int callbackID, const Json::Value& parameters);

    bool DirectoryEntry_getDirectory(int callbackID, const Json::Value& parameters);

    bool DirectoryEntry_getFile(int callbackID, const Json::Value& parameters);

    bool DirectoryEntry_removeRecursively(int callbackID, const Json::Value& parameters);

    bool DirectoryReader_readEntries(int callbackID, const Json::Value& parameters);

    bool LocalFileSystem_requestFileSystem(int callbackID, const Json::Value& parameters);

    bool LocalFileSystem_resolveLocalFileSystemURL(int callbackID, const Json::Value& parameters);

    bool FileTransfer_upload(int callbackID, const Json::Value& parameters);

    //C++->JS automated event setup
public:
    //C++->JS automated callbacks
    bool metadataSuccessCallback(int callbackID, const Metadata& metadata);

    bool fileEntrySuccessCallback(int callbackID, const FileEntry& file);

    bool directoryEntrySuccessCallback(int callbackID, const DirectoryEntry& directory);

    bool fileSuccessCallback(int callbackID, const File& file);

    bool fileSystemSuccessCallback(int callbackID, const FileSystem& filesystem);

    bool readCallback(int callbackID, const char* data, uint64 loaded, uint64 total);

    bool writeCallback(int callbackID, uint64 position, uint64 length);

    bool successCallback(int callbackID);

    bool errorCallback(int callbackID, const FileError& error);

    bool entriesSuccessCallback(int callbackID, const std::vector<Entry > entries);

    bool entrySuccessCallback(int callbackID, const Entry& entry);

    bool uploadSuccessCallback(int callbackID, const FileUploadResult& result);

    bool transferErrorCallback(int callbackID, const FileTransferError& error);

    //C++->JS automated events

// [[[end]]]

/*
 * Auto Generated section for defining modules functions
 * The creator of the module needs to implement these functions
 */
// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\h.def.tmpl") ]]]
public:

    //To be implemented in the user's derived class

    virtual void FileReader_readAsDataURL(int callbackID, const FileReader& self, const File& file) = 0;
    virtual void FileReader_readAsText(int callbackID, const FileReader& self, const File& file, const char* encoding) = 0;
    virtual void FileWriter_write(int callbackID, const FileWriter& self, const char* data) = 0;
    virtual void FileWriter_truncate(int callbackID, const FileWriter& self, uint64 size) = 0;
    virtual void Entry_getMetadata(int callbackID, const Entry& self) = 0;
    virtual void Entry_moveTo(int callbackID, const Entry& self, const DirectoryEntry& parent, const char* newName) = 0;
    virtual void Entry_copyTo(int callbackID, const Entry& self, const DirectoryEntry& parent, const char* newName) = 0;
    virtual void Entry_remove(int callbackID, const Entry& self) = 0;
    virtual void Entry_getParent(int callbackID, const Entry& self) = 0;
    virtual void FileEntry_file(int callbackID, const FileEntry& self) = 0;
    virtual void DirectoryEntry_getDirectory(int callbackID, const DirectoryEntry& self, const char* path, const Flags& options) = 0;
    virtual void DirectoryEntry_getFile(int callbackID, const DirectoryEntry& self, const char* path, const Flags& options) = 0;
    virtual void DirectoryEntry_removeRecursively(int callbackID, const DirectoryEntry& self) = 0;
    virtual void DirectoryReader_readEntries(int callbackID, const DirectoryReader& self) = 0;
    virtual void LocalFileSystem_requestFileSystem(int callbackID, const LocalFileSystem& self, LocalFileSystem::LocalFileSystemEnum type, uint32 size) = 0;
    virtual void LocalFileSystem_resolveLocalFileSystemURL(int callbackID, const LocalFileSystem& self, const char* url) = 0;
    virtual void FileTransfer_upload(int callbackID, const FileTransfer& self, const char* filePath, const char* server, const FileUploadOptions& options) = 0;


// [[[end]]]

// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\h.end.tmpl") ]]]
};
// [[[end]]]
