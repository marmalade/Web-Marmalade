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

#include "Media.h"
#include "s3eTimer.h"
#include "IwDebug.h"
#include "IwHTTP.h"
//*
#if defined UNITTESTING
#include "mockMedia.h"
#define M(fn) fn##Mock
#else
#define M(fn) fn
#endif
/*/
#define M(fn) fn
//*/

#include <map>
#include <vector>
using std::map;
using std::vector;


// Remark: This class leaves the audio buffer intact after audio stops playing.
// We clean up on user calls (from js) to release and also if the channel starts playing again.
class CHTTPAudioManager
{
public:

    CHTTPAudioManager();
    ~CHTTPAudioManager();
    void PlayBufferFromHTTP(const char* url, const int channel, const int callbackID);
    
    // Frees the http buffer (if any) and stops any outstanding http request
    void ResetChannel(int channel);

    static void SetMediaPointer(CMediaBase* media) {p_mediaBase = media;}

private:

    CHTTPAudioManager(const CHTTPAudioManager& rhs);

    enum HTTPStatus
    {
        NotCreated,
        Initialized,
        Downloading,
        OK,
        Error,
    };

    struct SHTTPAudio
    {
        SHTTPAudio() : httpObject(NULL){ FreeBuffer(); }
        
        void FreeBuffer()
        {
            // Only do it if size is not 0 
            if (buffer.size() != 0)
            {
                //Save memory
                IwTrace(WEBMARMALADE, ("Freeing http buffer on channel %d", channel));
                vector<char> temp;
                buffer.swap(temp);
            }
        }

        CIwHTTP* httpObject;
        int callbackID;
        int channel;
        //Represents the status of the http object not the audio play
        HTTPStatus status; 
        vector<char> buffer;
        uint32 bufferLength;
        uint32 bytesReadSoFar;
    };
    
    // Callbacks for HTTP and Audio
    static int32 GetDataCallback(void *systemData, void *userData);
    static int32 GetheadersCallback(void *systemData, void *userData);
    
    static const uint32 TIMEOUT;
    map<int, SHTTPAudio> m_HTTPAudioMap;

    static CMediaBase* p_mediaBase;
};

static CHTTPAudioManager g_HTTPAudioManager;

const uint32 CHTTPAudioManager::TIMEOUT = 5000;
CMediaBase* CHTTPAudioManager::p_mediaBase = 0;

CHTTPAudioManager::CHTTPAudioManager()
{
}

CHTTPAudioManager::~CHTTPAudioManager()
{
    map<int, SHTTPAudio>::iterator itHTTPAudioMap = m_HTTPAudioMap.begin();
    map<int, SHTTPAudio>::iterator itHTTPAudioMapE = m_HTTPAudioMap.end();

    for (; itHTTPAudioMap != itHTTPAudioMapE; ++itHTTPAudioMap)
    {
        itHTTPAudioMap->second.httpObject->Cancel();
        delete itHTTPAudioMap->second.httpObject;
    }
}

void CHTTPAudioManager::ResetChannel(int channel)
{
    if (m_HTTPAudioMap.find(channel) != m_HTTPAudioMap.end())
    {
        IwTrace(WEBMARMALADE, ("Calling FreeBuffer (for http) on channel %d", channel));
        m_HTTPAudioMap[channel].httpObject->Cancel();
        m_HTTPAudioMap[channel].FreeBuffer();
    }
}

void CHTTPAudioManager::PlayBufferFromHTTP(const char* url, const int channel, const int callbackID)
{
    IwTrace(WEBMARMALADE, ("Fetching buffer for channel %d from http %s", channel, url));

    SHTTPAudio& object = m_HTTPAudioMap[channel];
    
    // We only create the http object once and never free (until app termination)
    if (object.httpObject == NULL)
        object.httpObject = new CIwHTTP();

    object.FreeBuffer();
    object.callbackID = callbackID;
    object.channel = channel;
    object.status = Initialized;
    object.bufferLength = 0;
    object.bytesReadSoFar = 0;

    if (object.httpObject->Get(url, CHTTPAudioManager::GetheadersCallback, &object) == S3E_RESULT_SUCCESS)
        object.status = Downloading;
}

int32 CHTTPAudioManager::GetheadersCallback(void*, void* userData)
{
    CHTTPAudioManager::SHTTPAudio* object = (CHTTPAudioManager::SHTTPAudio*)userData;
    if (object->httpObject->GetStatus() == S3E_RESULT_ERROR)
    {
        IwTrace(WEBMARMALADE, ("Error getting headers of http"));
        object->status = Error;
        p_mediaBase->mediaErrorCallback(object->callbackID, CMediaBase::MediaError(CMediaBase::MediaError::MEDIA_ERR_ABORTED, M(s3eAudioGetErrorString)()));
        //NB No buffer to free yet
        return 1;
    }
    else
    {
        object->bufferLength = object->httpObject->ContentExpected();
        if (!object->bufferLength)
            object->bufferLength = 1024;

        object->buffer.resize(object->bufferLength);
        (object->buffer)[object->bufferLength] = 0;

        IwTrace(WEBMARMALADE, ("Got http header.  Requesting %d bytes of data asynchronously", object->bufferLength));
        object->httpObject->ReadDataAsync(&(object->buffer[0]), object->bufferLength, TIMEOUT, CHTTPAudioManager::GetDataCallback, object);
    }

    return 0;
}

int32 CHTTPAudioManager::GetDataCallback(void* sysData, void* userData)
{
    CHTTPAudioManager::SHTTPAudio* object = (CHTTPAudioManager::SHTTPAudio*)userData;
    int bytesReadThisTime = (int)sysData; 
    object->bytesReadSoFar += bytesReadThisTime;

    IwTrace(WEBMARMALADE, ("GetDataCallback: Bytes read so far = %d", object->bytesReadSoFar));
    
    if (object->httpObject->GetStatus() == S3E_RESULT_ERROR)
    {
        IwTrace(WEBMARMALADE, ("Error getting data from http"));
        object->status = Error;
        object->FreeBuffer();
        p_mediaBase->mediaErrorCallback(object->callbackID, CMediaBase::MediaError(CMediaBase::MediaError::MEDIA_ERR_ABORTED, M(s3eAudioGetErrorString)()));
        return 1;
    }

    bool timedOut = false;
    if (object->bytesReadSoFar < object->bufferLength)
        timedOut = true;

    if (timedOut && !bytesReadThisTime)
    {
        IwTrace(WEBMARMALADE, ("Http request returned so data since last timeout, cancelling request"));
        object->status = Error;
        object->FreeBuffer();
        p_mediaBase->mediaErrorCallback(object->callbackID, CMediaBase::MediaError(CMediaBase::MediaError::MEDIA_ERR_ABORTED, M(s3eAudioGetErrorString)()));
        return 1;
    }
    
    if (!object->httpObject->ContentFinished())
    {
        const uint32 expectedContent = object->httpObject->ContentExpected();
        if (object->bufferLength < expectedContent)
            object->bufferLength = expectedContent;
        else if (!timedOut)
            object->bufferLength += 1024;

        // Allocate some more space and fetch the data.
        object->buffer.resize(object->bufferLength);
        IwTrace(WEBMARMALADE, ("Requesting %d bytes more of data asynchronously", object->bufferLength - object->bytesReadSoFar));
        object->httpObject->ReadDataAsync(&(object->buffer[object->bytesReadSoFar]), object->bufferLength - object->bytesReadSoFar, TIMEOUT, CHTTPAudioManager::GetDataCallback, object);
    }
    else
    {
        IwTrace(WEBMARMALADE, ("Got all data from http on channel %d, attempting to play", object->channel));
        object->status = OK;

        if (M(s3eAudioPlayFromBuffer)(&(object->buffer[0]), object->bytesReadSoFar, 1) != S3E_RESULT_SUCCESS)
        {
            object->FreeBuffer();
            p_mediaBase->mediaErrorCallback(object->callbackID, CMediaBase::MediaError(CMediaBase::MediaError::MEDIA_ERR_ABORTED, M(s3eAudioGetErrorString)()));
            return 1;
        }
        uint64 duration = M(s3eAudioGetInt)(S3E_AUDIO_DURATION);
        p_mediaBase->_mediaPlaySuccess(object->callbackID, false, duration);
    }
    
    return 0;
}

void PlayBufferFromHTTP(const char* url, const int channel, const int callbackID)
{
    g_HTTPAudioManager.PlayBufferFromHTTP(url, channel, callbackID);
}

bool DeviceIsIOS()
{
    return (s3eDeviceOSID)s3eDeviceGetInt(S3E_DEVICE_OS) == S3E_OS_ID_IPHONE;
}

bool DeviceIsMac()
{
    return (s3eDeviceOSID)s3eDeviceGetInt(S3E_DEVICE_OS) == S3E_OS_ID_OSX;
}

bool URLIsHTTP(const char* url)
{
    return !strncmp(url, "http://", strlen("http://")) || 
           !strncmp(url, "https://", strlen("https://"));
}

int32 CMedia::AudioHasStopped(s3eAudioCallbackData* systemData, CMedia* media)
{
    int channel = 0;
    if (systemData != NULL)
        channel = systemData->m_ChannelID;

    int callbackID = media->GetCallbackID(channel);
    if (callbackID >= 0)
    {
        media->_mediaPlaySuccess(callbackID, true, 0);
    }

    media->m_ChannelToCallbackID[channel] = -1;
    return 0;
}

void CMedia::Init()
{
    m_NumChannels = M(s3eAudioGetInt)(S3E_AUDIO_NUM_CHANNELS);
    m_ChannelToID = new uint64[m_NumChannels];
    m_ChannelToCallbackID = new int[m_NumChannels];

    // Initialize channels
    for (int i=0; i<m_NumChannels; i++)
    {
        m_ChannelToID[i] = -1;
        m_ChannelToCallbackID[i] = -1;
    }

    CHTTPAudioManager::SetMediaPointer(this);
    
    M(s3eAudioRegister)(S3E_AUDIO_STOP, (s3eCallback)AudioHasStopped, this);
}

void CMedia::Shutdown()
{
    IwTrace(WM_MODULE_MEDIA, ("Media: Shutdown."));
    delete[] m_ChannelToID;
    delete[] m_ChannelToCallbackID;

    M(s3eAudioUnRegister)(S3E_AUDIO_STOP, (s3eCallback)AudioHasStopped);
}

//C++ user init code
void CMedia::OnReady()
{
    IwTrace(WM_MODULE_MEDIA, ("Media: OnReady."));

    // clean up channels
    for (int i=0; i<m_NumChannels; i++)
    {
        m_ChannelToID[i] = -1;
        m_ChannelToCallbackID[i] = -1;
    }

}

int CMedia::GetCallbackID(int channel)
{
    if (channel < 0 || channel >= m_NumChannels)
        return -1;

    return m_ChannelToCallbackID[channel];
}

int CMedia::GetChannel(uint64 ID, bool create)
{
    int i;
    for (i=0; i<m_NumChannels; i++)
        if (m_ChannelToID[i] == ID)
            return i;
    
    if (create)
    {
        for (i=0; i<m_NumChannels; i++)
            if (m_ChannelToID[i] == (uint64)-1)
            {
                m_ChannelToID[i] = ID;
                return i;
            }
    }
    return -1;
}

void CMedia::FreeChannel(uint64 ID)
{
    for (int i=0; i<m_NumChannels; i++)
        if (m_ChannelToID[i] == ID)
            m_ChannelToID[i] = -1;
}
    

//C++ user code 
void CMedia::Media_getCurrentPosition(int callbackID, const Media& self)
{
    int channel = GetChannel(self.id, false);
    if (channel == -1)
    {
        mediaErrorCallback(callbackID, MediaError(MediaError::MEDIA_ERR_NONE_ACTIVE, "Channel not in use"));
        return;
    }
    M(s3eAudioSetInt)(S3E_AUDIO_CHANNEL, channel);

    int position = M(s3eAudioGetInt)(S3E_AUDIO_POSITION);

    mediaPosSuccessCallback(callbackID, position);
}
    
bool CMedia::GetFile(const char* input, char* output, int outputSize, bool read)
{
    if (!strncmp(input, "file:///", 8))
    {
        strncpy(output, input+8, outputSize);
        IwTrace(WEBMARMALADE,("GetFile: %s", output));
        return true;
    }
    else if (!strncmp(input, "rom://", 6) || !strncmp(input, "ram://", 6) || !strncmp(input, "raw://", 6))
    {        
        strncpy(output, input, outputSize);
        IwTrace(WEBMARMALADE,("GetFile: %s", output));
        return true;
    }

    if (read && (!strncmp(input, "http://", 7) || !strncmp(input, "https://", 8)))
    {
        strncpy(output, input, outputSize);
        IwTrace(WEBMARMALADE,("GetFile: %s", output));
        return true;
    }

    return false;
}

void CMedia::play(int callbackID, const Media& self, int channel)
{
    
    M(s3eAudioSetInt)(S3E_AUDIO_CHANNEL, channel);

    char buffer[256];

    if (!GetFile(self._src.c_str(), buffer, 256))
    {
        mediaErrorCallback(callbackID, MediaError(MediaError::MEDIA_ERR_ABORTED, "Invalid URL"));
        return;
    }

    m_ChannelToCallbackID[channel] = callbackID;
    g_HTTPAudioManager.ResetChannel(channel);

    if (DeviceIsIOS() && URLIsHTTP(buffer))
    {
        // Need to stop audio first if playing from http as the play call is async
        // and won't be immediate
        M(s3eAudioStop)();

        // This function is async so will call the success/fail callbacks when it's done
        M(PlayBufferFromHTTP)(buffer, channel, callbackID);
        return;
    }

    if (M(s3eAudioPlay)(buffer) != S3E_RESULT_SUCCESS)
    {
        mediaErrorCallback(callbackID, MediaError(MediaError::MEDIA_ERR_ABORTED, M(s3eAudioGetErrorString)()));
        return;
    }    

    uint64 duration = M(s3eAudioGetInt)(S3E_AUDIO_DURATION);

    _mediaPlaySuccess(callbackID, false, duration);

}
void CMedia::Media_play(int callbackID, const Media& self)
{
    if (DeviceIsMac() && URLIsHTTP(self._src.c_str()))
    {
        mediaErrorCallback(callbackID, MediaError(MediaError::MEDIA_ERR_ABORTED, "Playing from HTTP is not supported."));
        return;
    }

    int channel = GetChannel(self.id, false);
    if (channel == -1) 
    {
        // play for first time
        channel = GetChannel(self.id, true);
        if (channel == -1)
        {
            mediaErrorCallback(callbackID, MediaError(MediaError::MEDIA_ERR_ABORTED, "Too many channels in use"));
            return;
        }
        play(callbackID, self, channel);
    }
    else if (self._status == self.MEDIA_PAUSED)
    {
        // resume
        M(s3eAudioSetInt)(S3E_AUDIO_CHANNEL, channel);

        if (M(s3eAudioResume)() != S3E_RESULT_SUCCESS)
        {
            mediaErrorCallback(callbackID, MediaError(MediaError::MEDIA_ERR_ABORTED, M(s3eAudioGetErrorString)()));
            return;
        }

        uint64 duration = M(s3eAudioGetInt)(S3E_AUDIO_DURATION);

        m_ChannelToCallbackID[channel] = callbackID;
        
        _mediaPlaySuccess(callbackID, false, duration);
    }
    else if (self._status == self.MEDIA_STOPPED)
    {
        // restart
        play(callbackID, self, channel);
    }
    else
    {
        mediaErrorCallback(callbackID, MediaError(MediaError::MEDIA_ERR_ABORTED, "Invalid state."));
    }

}

void CMedia::Media_pause(int callbackID, const Media& self)
{
    int channel = GetChannel(self.id, false);
    if (channel == -1)
    {
        mediaErrorCallback(callbackID, MediaError(MediaError::MEDIA_ERR_NONE_ACTIVE, "Channel not in use"));
        return;
    }
    M(s3eAudioSetInt)(S3E_AUDIO_CHANNEL, channel);

    if (!M(s3eAudioIsPlaying)())
    {
        mediaErrorCallback(callbackID, MediaError(MediaError::MEDIA_ERR_NONE_ACTIVE, "Channel is not playing"));
        return;
    }

    if (M(s3eAudioPause)() != S3E_RESULT_SUCCESS)
    {
        mediaErrorCallback(callbackID, MediaError(MediaError::MEDIA_ERR_ABORTED, M(s3eAudioGetErrorString)()));
        return;
    }

    _mediaSuccess(callbackID);
}

void CMedia::Media_stop(int callbackID, const Media& self)
{
    int channel = GetChannel(self.id, false);
    if (channel == -1)
    {
        mediaErrorCallback(callbackID, MediaError(MediaError::MEDIA_ERR_NONE_ACTIVE, "Channel not in use"));
        return;
    }
    M(s3eAudioSetInt)(S3E_AUDIO_CHANNEL, channel);

    int status = M(s3eAudioGetInt)(S3E_AUDIO_STATUS);
    if (status != S3E_AUDIO_PLAYING && status != S3E_AUDIO_PAUSED)
    {
        mediaErrorCallback(callbackID, MediaError(MediaError::MEDIA_ERR_NONE_ACTIVE, "Channel is not playing/paused"));
        return;
    }

    M(s3eAudioStop)();
    
    g_HTTPAudioManager.ResetChannel(channel);

    _mediaSuccess(callbackID);
}

void CMedia::Media_release(int callbackID, const Media& self)
{
    int channel = GetChannel(self.id, false);
    if (channel == -1)
    {
        mediaErrorCallback(callbackID, MediaError(MediaError::MEDIA_ERR_NONE_ACTIVE, "Channel already released"));
        return;
    }
    M(s3eAudioSetInt)(S3E_AUDIO_CHANNEL, channel);

    m_ChannelToCallbackID[channel] = -1;

    int status = M(s3eAudioGetInt)(S3E_AUDIO_STATUS);
    if (status == S3E_AUDIO_PLAYING || status == S3E_AUDIO_PAUSED)
        M(s3eAudioStop)();

    g_HTTPAudioManager.ResetChannel(channel);

    m_ChannelToID[channel] = -1;

    _mediaSuccess(callbackID);
}

void CMedia::Media_seekTo(int callbackID, const Media& self, int64 milliseconds)
{
    if (milliseconds<0)
        milliseconds = 0;
    else if (milliseconds > (int64) self._duration)
        milliseconds = 0;

    int channel = GetChannel(self.id, false);
    if (channel == -1)
    {
        mediaErrorCallback(callbackID, MediaError(MediaError::MEDIA_ERR_NONE_ACTIVE, "Channel not in use"));
        return;
    }
    M(s3eAudioSetInt)(S3E_AUDIO_CHANNEL, channel);
    
    int status = M(s3eAudioGetInt)(S3E_AUDIO_STATUS);
    if (status != S3E_AUDIO_PLAYING && status != S3E_AUDIO_PAUSED)
    {
        mediaErrorCallback(callbackID, MediaError(MediaError::MEDIA_ERR_NONE_ACTIVE, "Channel not playing"));
        return;
    }

    if (M(s3eAudioSetInt)(S3E_AUDIO_POSITION, (int)milliseconds) != S3E_RESULT_SUCCESS)
    {
        mediaErrorCallback(callbackID, MediaError(MediaError::MEDIA_ERR_ABORTED, M(s3eAudioGetErrorString)()));
        return;
    }

    _mediaSuccess(callbackID);
}

void CMedia::Media_startRecord(int callbackID, const Media& self)
{
    int channel = GetChannel(self.id, false);
    if (channel != -1)
    {
        mediaErrorCallback(callbackID, MediaError(MediaError::MEDIA_ERR_NONE_ACTIVE, "Channel used for playing"));
        return;
    }
    if (m_Record.m_ID != (uint64)-1)
    {
        mediaErrorCallback(callbackID, MediaError(MediaError::MEDIA_ERR_ABORTED, "Already recording"));
        return;
    }

    char buffer[256];

    if (!GetFile(self._src.c_str(), buffer, 256, false))
    {
        mediaErrorCallback(callbackID, MediaError(MediaError::MEDIA_ERR_ABORTED, "Invalid URL"));
        return;
    }

    m_Record.m_Audio = IwAudioCaptureToFile(buffer);

    if (m_Record.m_Audio == NULL)
    {
        mediaErrorCallback(callbackID, MediaError(MediaError::MEDIA_ERR_ABORTED, IwAudioCaptureGetErrorString()));
        return;
    }
    m_Record.m_ID = self.id;
    m_Record.m_CallbackID = callbackID;

    _mediaSuccess(callbackID);
}

// purely for tests
void CMedia::DoStopRecord()
{
    m_Record.m_ID = -1;

    IwAudioCaptureStop(m_Record.m_Audio);
}

void CMedia::Media_stopRecord(int callbackID, const Media& self)
{
    int channel = GetChannel(self.id, false);
    if (channel != -1)
    {
        mediaErrorCallback(callbackID, MediaError(MediaError::MEDIA_ERR_NONE_ACTIVE, "Channel used for playing"));
        return;
    }
    if (m_Record.m_ID != self.id)
    {
        mediaErrorCallback(callbackID, MediaError(MediaError::MEDIA_ERR_ABORTED, "Stopping when should not"));
        return;
    }

    m_Record.m_ID = -1;

    if (IwAudioCaptureStop(m_Record.m_Audio) == S3E_RESULT_ERROR)
    {
        mediaErrorCallback(callbackID, MediaError(MediaError::MEDIA_ERR_ABORTED, IwAudioCaptureGetErrorString()));
        return;
    }
    _mediaSuccess(callbackID);
}

void CMedia::Media_setVolume(int callbackID, const Media& self, double volume)
{
    int channel = GetChannel(self.id, false);
    if (channel == -1)
    {
        mediaErrorCallback(callbackID, MediaError(MediaError::MEDIA_ERR_NONE_ACTIVE, "Channel not in use"));
        return;
    }
    M(s3eAudioSetInt)(S3E_AUDIO_CHANNEL, channel);

    if (M(s3eAudioSetInt)(S3E_AUDIO_VOLUME, (int)(volume * 256.0)) != S3E_RESULT_SUCCESS)
    {
        mediaErrorCallback(callbackID, MediaError(MediaError::MEDIA_ERR_ABORTED, M(s3eAudioGetErrorString)()));
        return;
    }

    _mediaSuccess(callbackID);
}
