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
/*
 * User #includes go here
 */
// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\cpp.start.tmpl") ]]]
#include "FileBase.h"


CFileBase::CFileBase(CIwWMDispatcherBase* dispatcher) :
    CIwWMDispatcherModule<CFileBase>("file", dispatcher)
{

    RegisterMember("FileReader_readAsDataURL", &CFileBase::FileReader_readAsDataURL);
    RegisterMember("FileReader_readAsText", &CFileBase::FileReader_readAsText);
    RegisterMember("FileWriter_write", &CFileBase::FileWriter_write);
    RegisterMember("FileWriter_truncate", &CFileBase::FileWriter_truncate);
    RegisterMember("Entry_getMetadata", &CFileBase::Entry_getMetadata);
    RegisterMember("Entry_moveTo", &CFileBase::Entry_moveTo);
    RegisterMember("Entry_copyTo", &CFileBase::Entry_copyTo);
    RegisterMember("Entry_remove", &CFileBase::Entry_remove);
    RegisterMember("Entry_getParent", &CFileBase::Entry_getParent);
    RegisterMember("FileEntry_file", &CFileBase::FileEntry_file);
    RegisterMember("DirectoryEntry_getDirectory", &CFileBase::DirectoryEntry_getDirectory);
    RegisterMember("DirectoryEntry_getFile", &CFileBase::DirectoryEntry_getFile);
    RegisterMember("DirectoryEntry_removeRecursively", &CFileBase::DirectoryEntry_removeRecursively);
    RegisterMember("DirectoryReader_readEntries", &CFileBase::DirectoryReader_readEntries);
    RegisterMember("LocalFileSystem_requestFileSystem", &CFileBase::LocalFileSystem_requestFileSystem);
    RegisterMember("LocalFileSystem_resolveLocalFileSystemURL", &CFileBase::LocalFileSystem_resolveLocalFileSystemURL);
    RegisterMember("FileTransfer_upload", &CFileBase::FileTransfer_upload);
    RegisterMember("FileEntry_getMetadata", &CFileBase::Entry_getMetadata);
    RegisterMember("FileEntry_moveTo", &CFileBase::Entry_moveTo);
    RegisterMember("FileEntry_copyTo", &CFileBase::Entry_copyTo);
    RegisterMember("FileEntry_remove", &CFileBase::Entry_remove);
    RegisterMember("FileEntry_getParent", &CFileBase::Entry_getParent);
    RegisterMember("DirectoryEntry_getMetadata", &CFileBase::Entry_getMetadata);
    RegisterMember("DirectoryEntry_moveTo", &CFileBase::Entry_moveTo);
    RegisterMember("DirectoryEntry_copyTo", &CFileBase::Entry_copyTo);
    RegisterMember("DirectoryEntry_remove", &CFileBase::Entry_remove);
    RegisterMember("DirectoryEntry_getParent", &CFileBase::Entry_getParent);
}

// [[[end]]]

/*
 * Auto Generated section for converting between JavaScript and C++
 */
// [[[cog import wmGenerate; wmGenerate.GenerateCog("templates\cpp.bridge.tmpl") ]]]

//JS->C++ automated converters
bool CFileBase::FileReader_readAsDataURL(int callbackID, const Json::Value& parameters)
{
    Json::Value self_JSON;
    FetchObject(parameters, "self", self_JSON);

    FileReader self=FileReader::From_JSON(self_JSON, this);


    Json::Value file_JSON;
    FetchObject(parameters, "file", file_JSON);

    File file=File::From_JSON(file_JSON, this);


    FileReader_readAsDataURL(callbackID, self, file);
    return true;
}

bool CFileBase::FileReader_readAsText(int callbackID, const Json::Value& parameters)
{
    Json::Value self_JSON;
    FetchObject(parameters, "self", self_JSON);

    FileReader self=FileReader::From_JSON(self_JSON, this);


    Json::Value file_JSON;
    FetchObject(parameters, "file", file_JSON);

    File file=File::From_JSON(file_JSON, this);


    Json::Value encoding_JSON;
    FetchObject(parameters, "encoding", encoding_JSON);

    std::string encoding;
    FetchString(encoding_JSON, encoding);



    FileReader_readAsText(callbackID, self, file, encoding.c_str());
    return true;
}

bool CFileBase::FileWriter_write(int callbackID, const Json::Value& parameters)
{
    Json::Value self_JSON;
    FetchObject(parameters, "self", self_JSON);

    FileWriter self=FileWriter::From_JSON(self_JSON, this);


    Json::Value data_JSON;
    FetchObject(parameters, "data", data_JSON);

    std::string data;
    FetchString(data_JSON, data);



    FileWriter_write(callbackID, self, data.c_str());
    return true;
}

bool CFileBase::FileWriter_truncate(int callbackID, const Json::Value& parameters)
{
    Json::Value self_JSON;
    FetchObject(parameters, "self", self_JSON);

    FileWriter self=FileWriter::From_JSON(self_JSON, this);


    Json::Value size_JSON;
    FetchObject(parameters, "size", size_JSON);

    uint64 size;
    FetchULong(size_JSON, size);



    FileWriter_truncate(callbackID, self, size);
    return true;
}

bool CFileBase::Entry_getMetadata(int callbackID, const Json::Value& parameters)
{
    Json::Value self_JSON;
    FetchObject(parameters, "self", self_JSON);

    Entry self=Entry::From_JSON(self_JSON, this);


    Entry_getMetadata(callbackID, self);
    return true;
}

bool CFileBase::Entry_moveTo(int callbackID, const Json::Value& parameters)
{
    Json::Value self_JSON;
    FetchObject(parameters, "self", self_JSON);

    Entry self=Entry::From_JSON(self_JSON, this);


    Json::Value parent_JSON;
    FetchObject(parameters, "parent", parent_JSON);

    DirectoryEntry parent=DirectoryEntry::From_JSON(parent_JSON, this);


    Json::Value newName_JSON;
    FetchObject(parameters, "newName", newName_JSON);

    std::string newName;
    FetchString(newName_JSON, newName);



    Entry_moveTo(callbackID, self, parent, newName.c_str());
    return true;
}

bool CFileBase::Entry_copyTo(int callbackID, const Json::Value& parameters)
{
    Json::Value self_JSON;
    FetchObject(parameters, "self", self_JSON);

    Entry self=Entry::From_JSON(self_JSON, this);


    Json::Value parent_JSON;
    FetchObject(parameters, "parent", parent_JSON);

    DirectoryEntry parent=DirectoryEntry::From_JSON(parent_JSON, this);


    Json::Value newName_JSON;
    FetchObject(parameters, "newName", newName_JSON);

    std::string newName;
    FetchString(newName_JSON, newName);



    Entry_copyTo(callbackID, self, parent, newName.c_str());
    return true;
}

bool CFileBase::Entry_remove(int callbackID, const Json::Value& parameters)
{
    Json::Value self_JSON;
    FetchObject(parameters, "self", self_JSON);

    Entry self=Entry::From_JSON(self_JSON, this);


    Entry_remove(callbackID, self);
    return true;
}

bool CFileBase::Entry_getParent(int callbackID, const Json::Value& parameters)
{
    Json::Value self_JSON;
    FetchObject(parameters, "self", self_JSON);

    Entry self=Entry::From_JSON(self_JSON, this);


    Entry_getParent(callbackID, self);
    return true;
}

bool CFileBase::FileEntry_file(int callbackID, const Json::Value& parameters)
{
    Json::Value self_JSON;
    FetchObject(parameters, "self", self_JSON);

    FileEntry self=FileEntry::From_JSON(self_JSON, this);


    FileEntry_file(callbackID, self);
    return true;
}

bool CFileBase::DirectoryEntry_getDirectory(int callbackID, const Json::Value& parameters)
{
    Json::Value self_JSON;
    FetchObject(parameters, "self", self_JSON);

    DirectoryEntry self=DirectoryEntry::From_JSON(self_JSON, this);


    Json::Value path_JSON;
    FetchObject(parameters, "path", path_JSON);

    std::string path;
    FetchString(path_JSON, path);



    Json::Value options_JSON;
    FetchObject(parameters, "options", options_JSON);

    Flags options=Flags::From_JSON(options_JSON, this);


    DirectoryEntry_getDirectory(callbackID, self, path.c_str(), options);
    return true;
}

bool CFileBase::DirectoryEntry_getFile(int callbackID, const Json::Value& parameters)
{
    Json::Value self_JSON;
    FetchObject(parameters, "self", self_JSON);

    DirectoryEntry self=DirectoryEntry::From_JSON(self_JSON, this);


    Json::Value path_JSON;
    FetchObject(parameters, "path", path_JSON);

    std::string path;
    FetchString(path_JSON, path);



    Json::Value options_JSON;
    FetchObject(parameters, "options", options_JSON);

    Flags options=Flags::From_JSON(options_JSON, this);


    DirectoryEntry_getFile(callbackID, self, path.c_str(), options);
    return true;
}

bool CFileBase::DirectoryEntry_removeRecursively(int callbackID, const Json::Value& parameters)
{
    Json::Value self_JSON;
    FetchObject(parameters, "self", self_JSON);

    DirectoryEntry self=DirectoryEntry::From_JSON(self_JSON, this);


    DirectoryEntry_removeRecursively(callbackID, self);
    return true;
}

bool CFileBase::DirectoryReader_readEntries(int callbackID, const Json::Value& parameters)
{
    Json::Value self_JSON;
    FetchObject(parameters, "self", self_JSON);

    DirectoryReader self=DirectoryReader::From_JSON(self_JSON, this);


    DirectoryReader_readEntries(callbackID, self);
    return true;
}

bool CFileBase::LocalFileSystem_requestFileSystem(int callbackID, const Json::Value& parameters)
{
    Json::Value self_JSON;
    FetchObject(parameters, "self", self_JSON);

    LocalFileSystem self=LocalFileSystem::From_JSON(self_JSON, this);


    Json::Value type_JSON;
    FetchObject(parameters, "type", type_JSON);

    LocalFileSystem::LocalFileSystemEnum type;
    FetchUInt(type_JSON, *((unsigned int*)&type));



    Json::Value size_JSON;
    FetchObject(parameters, "size", size_JSON);

    uint32 size;
    FetchUInt(size_JSON, size);



    LocalFileSystem_requestFileSystem(callbackID, self, type, size);
    return true;
}

bool CFileBase::LocalFileSystem_resolveLocalFileSystemURL(int callbackID, const Json::Value& parameters)
{
    Json::Value self_JSON;
    FetchObject(parameters, "self", self_JSON);

    LocalFileSystem self=LocalFileSystem::From_JSON(self_JSON, this);


    Json::Value url_JSON;
    FetchObject(parameters, "url", url_JSON);

    std::string url;
    FetchString(url_JSON, url);



    LocalFileSystem_resolveLocalFileSystemURL(callbackID, self, url.c_str());
    return true;
}

bool CFileBase::FileTransfer_upload(int callbackID, const Json::Value& parameters)
{
    Json::Value self_JSON;
    FetchObject(parameters, "self", self_JSON);

    FileTransfer self=FileTransfer::From_JSON(self_JSON, this);


    Json::Value filePath_JSON;
    FetchObject(parameters, "filePath", filePath_JSON);

    std::string filePath;
    FetchString(filePath_JSON, filePath);



    Json::Value server_JSON;
    FetchObject(parameters, "server", server_JSON);

    std::string server;
    FetchString(server_JSON, server);



    Json::Value options_JSON;
    FetchObject(parameters, "options", options_JSON);

    FileUploadOptions options=FileUploadOptions::From_JSON(options_JSON, this);


    FileTransfer_upload(callbackID, self, filePath.c_str(), server.c_str(), options);
    return true;
}

CFileBase::File CFileBase::File::From_JSON(const Json::Value& value, CFileBase* reader)
{
    Json::Value name_JSON;
    reader->FetchObject(value, "name", name_JSON);

    std::string name;
    reader->FetchString(name_JSON, name);


    Json::Value fullPath_JSON;
    reader->FetchObject(value, "fullPath", fullPath_JSON);

    std::string fullPath;
    reader->FetchString(fullPath_JSON, fullPath);


    Json::Value type_JSON;
    reader->FetchObject(value, "type", type_JSON);

    std::string type;
    reader->FetchString(type_JSON, type);


    Json::Value lastModifiedDate_JSON;
    reader->FetchObject(value, "lastModifiedDate", lastModifiedDate_JSON);

    uint64 lastModifiedDate;
    reader->FetchULong(lastModifiedDate_JSON, lastModifiedDate);


    Json::Value size_JSON;
    reader->FetchObject(value, "size", size_JSON);

    uint64 size;
    reader->FetchULong(size_JSON, size);


    return CFileBase::File(name.c_str(), fullPath.c_str(), type.c_str(), lastModifiedDate, size);
}

CFileBase::FileError CFileBase::FileError::From_JSON(const Json::Value& value, CFileBase* reader)
{
    Json::Value code_JSON;
    reader->FetchObject(value, "code", code_JSON);

    int32 code;
    reader->FetchInt(code_JSON, code);


    return CFileBase::FileError(code);
}

CFileBase::FileReader CFileBase::FileReader::From_JSON(const Json::Value& value, CFileBase* reader)
{
    Json::Value readyState_JSON;
    reader->FetchObject(value, "readyState", readyState_JSON);

    FileReader::FileReaderEnum readyState;
    reader->FetchUInt(readyState_JSON, *((unsigned int*)&readyState));


    Json::Value result_JSON;
    reader->FetchObject(value, "result", result_JSON);

    std::string result;
    reader->FetchString(result_JSON, result);


    Json::Value error_JSON;
    reader->FetchObject(value, "error", error_JSON);

    FileError error=FileError::From_JSON(error_JSON, reader);

    return CFileBase::FileReader(readyState, result.c_str(), error);
}

CFileBase::FileWriter CFileBase::FileWriter::From_JSON(const Json::Value& value, CFileBase* reader)
{
    Json::Value fileName_JSON;
    reader->FetchObject(value, "fileName", fileName_JSON);

    std::string fileName;
    reader->FetchString(fileName_JSON, fileName);


    Json::Value position_JSON;
    reader->FetchObject(value, "position", position_JSON);

    uint64 position;
    reader->FetchULong(position_JSON, position);


    Json::Value length_JSON;
    reader->FetchObject(value, "length", length_JSON);

    uint64 length;
    reader->FetchULong(length_JSON, length);


    Json::Value readyState_JSON;
    reader->FetchObject(value, "readyState", readyState_JSON);

    FileWriter::FileWriterEnum readyState;
    reader->FetchUInt(readyState_JSON, *((unsigned int*)&readyState));


    Json::Value error_JSON;
    reader->FetchObject(value, "error", error_JSON);

    FileError error=FileError::From_JSON(error_JSON, reader);

    return CFileBase::FileWriter(fileName.c_str(), position, length, readyState, error);
}

CFileBase::Entry CFileBase::Entry::From_JSON(const Json::Value& value, CFileBase* reader)
{
    Json::Value isFile_JSON;
    reader->FetchObject(value, "isFile", isFile_JSON);

    bool isFile;
    reader->FetchBool(isFile_JSON, isFile);


    Json::Value isDirectory_JSON;
    reader->FetchObject(value, "isDirectory", isDirectory_JSON);

    bool isDirectory;
    reader->FetchBool(isDirectory_JSON, isDirectory);


    Json::Value name_JSON;
    reader->FetchObject(value, "name", name_JSON);

    std::string name;
    reader->FetchString(name_JSON, name);


    Json::Value fullPath_JSON;
    reader->FetchObject(value, "fullPath", fullPath_JSON);

    std::string fullPath;
    reader->FetchString(fullPath_JSON, fullPath);


    return CFileBase::Entry(isFile, isDirectory, name.c_str(), fullPath.c_str());
}

CFileBase::FileEntry CFileBase::FileEntry::From_JSON(const Json::Value& value, CFileBase* reader)
{
    Json::Value isFile_JSON;
    reader->FetchObject(value, "isFile", isFile_JSON);

    bool isFile;
    reader->FetchBool(isFile_JSON, isFile);


    Json::Value isDirectory_JSON;
    reader->FetchObject(value, "isDirectory", isDirectory_JSON);

    bool isDirectory;
    reader->FetchBool(isDirectory_JSON, isDirectory);


    Json::Value name_JSON;
    reader->FetchObject(value, "name", name_JSON);

    std::string name;
    reader->FetchString(name_JSON, name);


    Json::Value fullPath_JSON;
    reader->FetchObject(value, "fullPath", fullPath_JSON);

    std::string fullPath;
    reader->FetchString(fullPath_JSON, fullPath);


    return CFileBase::FileEntry(isFile, isDirectory, name.c_str(), fullPath.c_str());
}

CFileBase::DirectoryEntry CFileBase::DirectoryEntry::From_JSON(const Json::Value& value, CFileBase* reader)
{
    Json::Value isFile_JSON;
    reader->FetchObject(value, "isFile", isFile_JSON);

    bool isFile;
    reader->FetchBool(isFile_JSON, isFile);


    Json::Value isDirectory_JSON;
    reader->FetchObject(value, "isDirectory", isDirectory_JSON);

    bool isDirectory;
    reader->FetchBool(isDirectory_JSON, isDirectory);


    Json::Value name_JSON;
    reader->FetchObject(value, "name", name_JSON);

    std::string name;
    reader->FetchString(name_JSON, name);


    Json::Value fullPath_JSON;
    reader->FetchObject(value, "fullPath", fullPath_JSON);

    std::string fullPath;
    reader->FetchString(fullPath_JSON, fullPath);


    return CFileBase::DirectoryEntry(isFile, isDirectory, name.c_str(), fullPath.c_str());
}

CFileBase::DirectoryReader CFileBase::DirectoryReader::From_JSON(const Json::Value& value, CFileBase* reader)
{
    Json::Value fullPath_JSON;
    reader->FetchObject(value, "fullPath", fullPath_JSON);

    std::string fullPath;
    reader->FetchString(fullPath_JSON, fullPath);


    return CFileBase::DirectoryReader(fullPath.c_str());
}

CFileBase::FileSystem CFileBase::FileSystem::From_JSON(const Json::Value& value, CFileBase* reader)
{
    Json::Value name_JSON;
    reader->FetchObject(value, "name", name_JSON);

    std::string name;
    reader->FetchString(name_JSON, name);


    Json::Value root_JSON;
    reader->FetchObject(value, "root", root_JSON);

    DirectoryEntry root=DirectoryEntry::From_JSON(root_JSON, reader);

    return CFileBase::FileSystem(name.c_str(), root);
}

CFileBase::LocalFileSystem CFileBase::LocalFileSystem::From_JSON(const Json::Value& value, CFileBase* reader)
{
    return CFileBase::LocalFileSystem();
}

CFileBase::FileUploadOptions CFileBase::FileUploadOptions::From_JSON(const Json::Value& value, CFileBase* reader)
{
    Json::Value fileKey_JSON;
    reader->FetchObject(value, "fileKey", fileKey_JSON);

    std::string fileKey;
    reader->FetchString(fileKey_JSON, fileKey);


    Json::Value fileName_JSON;
    reader->FetchObject(value, "fileName", fileName_JSON);

    std::string fileName;
    reader->FetchString(fileName_JSON, fileName);


    Json::Value mimeType_JSON;
    reader->FetchObject(value, "mimeType", mimeType_JSON);

    std::string mimeType;
    reader->FetchString(mimeType_JSON, mimeType);


    Json::Value params_JSON;
    reader->FetchObject(value, "params", params_JSON);

    std::map<std::string, std::string> params;
    reader->FetchDictionary(params_JSON, params);


    Json::Value chunkedMode_JSON;
    reader->FetchObject(value, "chunkedMode", chunkedMode_JSON);

    bool chunkedMode;
    reader->FetchBool(chunkedMode_JSON, chunkedMode);


    return CFileBase::FileUploadOptions(fileKey.c_str(), fileName.c_str(), mimeType.c_str(), params, chunkedMode);
}

CFileBase::FileUploadResult CFileBase::FileUploadResult::From_JSON(const Json::Value& value, CFileBase* reader)
{
    Json::Value bytesSent_JSON;
    reader->FetchObject(value, "bytesSent", bytesSent_JSON);

    int32 bytesSent;
    reader->FetchInt(bytesSent_JSON, bytesSent);


    Json::Value responseCode_JSON;
    reader->FetchObject(value, "responseCode", responseCode_JSON);

    int32 responseCode;
    reader->FetchInt(responseCode_JSON, responseCode);


    Json::Value response_JSON;
    reader->FetchObject(value, "response", response_JSON);

    std::string response;
    reader->FetchString(response_JSON, response);


    return CFileBase::FileUploadResult(bytesSent, responseCode, response.c_str());
}

CFileBase::FileTransfer CFileBase::FileTransfer::From_JSON(const Json::Value& value, CFileBase* reader)
{
    return CFileBase::FileTransfer();
}

CFileBase::Flags CFileBase::Flags::From_JSON(const Json::Value& value, CFileBase* reader)
{
    Json::Value create_JSON;
    reader->FetchObject(value, "create", create_JSON);

    bool create;
    reader->FetchBool(create_JSON, create);


    Json::Value exclusive_JSON;
    reader->FetchObject(value, "exclusive", exclusive_JSON);

    bool exclusive;
    reader->FetchBool(exclusive_JSON, exclusive);


    return CFileBase::Flags(create, exclusive);
}

CFileBase::Metadata CFileBase::Metadata::From_JSON(const Json::Value& value, CFileBase* reader)
{
    Json::Value modificationTime_JSON;
    reader->FetchObject(value, "modificationTime", modificationTime_JSON);

    uint64 modificationTime;
    reader->FetchULong(modificationTime_JSON, modificationTime);


    return CFileBase::Metadata(modificationTime);
}

CFileBase::FileTransferError CFileBase::FileTransferError::From_JSON(const Json::Value& value, CFileBase* reader)
{
    Json::Value code_JSON;
    reader->FetchObject(value, "code", code_JSON);

    int32 code;
    reader->FetchInt(code_JSON, code);


    return CFileBase::FileTransferError(code);
}

//C++->JS automated event setup

//C++->JS automated callbacks
bool CFileBase::metadataSuccessCallback(int callbackID, const Metadata& metadata)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    Json::Value metadata_JSON(Json::objectValue);
    metadata.To_JSON(metadata_JSON);
    params["metadata"]=metadata_JSON;


    SendJavaScript("metadataSuccessCallback", callbackID, params);
    return true;
}

bool CFileBase::fileEntrySuccessCallback(int callbackID, const FileEntry& file)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    Json::Value file_JSON(Json::objectValue);
    file.To_JSON(file_JSON);
    params["file"]=file_JSON;


    SendJavaScript("fileEntrySuccessCallback", callbackID, params);
    return true;
}

bool CFileBase::directoryEntrySuccessCallback(int callbackID, const DirectoryEntry& directory)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    Json::Value directory_JSON(Json::objectValue);
    directory.To_JSON(directory_JSON);
    params["directory"]=directory_JSON;


    SendJavaScript("directoryEntrySuccessCallback", callbackID, params);
    return true;
}

bool CFileBase::fileSuccessCallback(int callbackID, const File& file)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    Json::Value file_JSON(Json::objectValue);
    file.To_JSON(file_JSON);
    params["file"]=file_JSON;


    SendJavaScript("fileSuccessCallback", callbackID, params);
    return true;
}

bool CFileBase::fileSystemSuccessCallback(int callbackID, const FileSystem& filesystem)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    Json::Value filesystem_JSON(Json::objectValue);
    filesystem.To_JSON(filesystem_JSON);
    params["filesystem"]=filesystem_JSON;


    SendJavaScript("fileSystemSuccessCallback", callbackID, params);
    return true;
}

bool CFileBase::readCallback(int callbackID, const char* data, uint64 loaded, uint64 total)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    params["data"]=Json::Value(data);

    params["loaded"]=Json::Value(loaded);

    params["total"]=Json::Value(total);


    SendJavaScript("readCallback", callbackID, params);
    return true;
}

bool CFileBase::writeCallback(int callbackID, uint64 position, uint64 length)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    params["position"]=Json::Value(position);

    params["length"]=Json::Value(length);


    SendJavaScript("writeCallback", callbackID, params);
    return true;
}

bool CFileBase::successCallback(int callbackID)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);

    SendJavaScript("successCallback", callbackID, params);
    return true;
}

bool CFileBase::errorCallback(int callbackID, const FileError& error)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    Json::Value error_JSON(Json::objectValue);
    error.To_JSON(error_JSON);
    params["error"]=error_JSON;


    SendJavaScript("errorCallback", callbackID, params);
    return true;
}

bool CFileBase::entriesSuccessCallback(int callbackID, const std::vector<Entry > entries)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    Json::Value entries_JSON(Json::arrayValue);
    for (int entry_i=0; entry_i<(int)entries.size(); entry_i++)
    {
    Json::Value entry_JSON(Json::objectValue);
    entries[entry_i].To_JSON(entry_JSON);
    entries_JSON[entry_i]=entry_JSON;

    }
    params["entries"]=entries_JSON;


    SendJavaScript("entriesSuccessCallback", callbackID, params);
    return true;
}

bool CFileBase::entrySuccessCallback(int callbackID, const Entry& entry)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    Json::Value entry_JSON(Json::objectValue);
    entry.To_JSON(entry_JSON);
    params["entry"]=entry_JSON;


    SendJavaScript("entrySuccessCallback", callbackID, params);
    return true;
}

bool CFileBase::uploadSuccessCallback(int callbackID, const FileUploadResult& result)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    Json::Value result_JSON(Json::objectValue);
    result.To_JSON(result_JSON);
    params["result"]=result_JSON;


    SendJavaScript("uploadSuccessCallback", callbackID, params);
    return true;
}

bool CFileBase::transferErrorCallback(int callbackID, const FileTransferError& error)
{
    Json::FastWriter writer;

    Json::Value params(Json::objectValue);
    Json::Value error_JSON(Json::objectValue);
    error.To_JSON(error_JSON);
    params["error"]=error_JSON;


    SendJavaScript("transferErrorCallback", callbackID, params);
    return true;
}

void CFileBase::File::To_JSON(Json::Value& value) const
{
    value["name"]=Json::Value(name);

    value["fullPath"]=Json::Value(fullPath);

    value["type"]=Json::Value(type);

    value["lastModifiedDate"]=Json::Value(lastModifiedDate);

    value["size"]=Json::Value(size);

}

void CFileBase::FileError::To_JSON(Json::Value& value) const
{
    value["code"]=Json::Value(code);

}

void CFileBase::FileReader::To_JSON(Json::Value& value) const
{
    value["readyState"]=Json::Value(readyState);

    value["result"]=Json::Value(result);

    Json::Value error_JSON(Json::objectValue);
    error.To_JSON(error_JSON);
    value["error"]=error_JSON;

}

void CFileBase::FileWriter::To_JSON(Json::Value& value) const
{
    value["fileName"]=Json::Value(fileName);

    value["position"]=Json::Value(position);

    value["length"]=Json::Value(length);

    value["readyState"]=Json::Value(readyState);

    Json::Value error_JSON(Json::objectValue);
    error.To_JSON(error_JSON);
    value["error"]=error_JSON;

}

void CFileBase::Entry::To_JSON(Json::Value& value) const
{
    value["isFile"]=Json::Value(isFile);

    value["isDirectory"]=Json::Value(isDirectory);

    value["name"]=Json::Value(name);

    value["fullPath"]=Json::Value(fullPath);

}

void CFileBase::FileEntry::To_JSON(Json::Value& value) const
{
    Entry::To_JSON(value);
}

void CFileBase::DirectoryEntry::To_JSON(Json::Value& value) const
{
    Entry::To_JSON(value);
}

void CFileBase::DirectoryReader::To_JSON(Json::Value& value) const
{
    value["fullPath"]=Json::Value(fullPath);

}

void CFileBase::FileSystem::To_JSON(Json::Value& value) const
{
    value["name"]=Json::Value(name);

    Json::Value root_JSON(Json::objectValue);
    root.To_JSON(root_JSON);
    value["root"]=root_JSON;

}

void CFileBase::LocalFileSystem::To_JSON(Json::Value& value) const
{
}

void CFileBase::FileUploadOptions::To_JSON(Json::Value& value) const
{
    value["fileKey"]=Json::Value(fileKey);

    value["fileName"]=Json::Value(fileName);

    value["mimeType"]=Json::Value(mimeType);

    Json::Value params_JSON(Json::objectValue);

    for (std::map<std::string, std::string>::const_iterator it = params.begin(); it!=params.end(); ++it)
        params_JSON[it->first] = it->second;
    value["params"]=params_JSON;

    value["chunkedMode"]=Json::Value(chunkedMode);

}

void CFileBase::FileUploadResult::To_JSON(Json::Value& value) const
{
    value["bytesSent"]=Json::Value(bytesSent);

    value["responseCode"]=Json::Value(responseCode);

    value["response"]=Json::Value(response);

}

void CFileBase::FileTransfer::To_JSON(Json::Value& value) const
{
}

void CFileBase::Flags::To_JSON(Json::Value& value) const
{
    value["create"]=Json::Value(create);

    value["exclusive"]=Json::Value(exclusive);

}

void CFileBase::Metadata::To_JSON(Json::Value& value) const
{
    value["modificationTime"]=Json::Value(modificationTime);

}

void CFileBase::FileTransferError::To_JSON(Json::Value& value) const
{
    value["code"]=Json::Value(code);

}

//C++->JS automated events

// [[[end]]]
