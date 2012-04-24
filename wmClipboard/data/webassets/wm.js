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
 
// main webmarmalade global object
var wm = {};

// wm version must match native marmalade version
wm.version = "6.0.0";

// Needed before device onReady is excuted
wm.platform = "";
if (typeof navigator.userAgent !== "undefined" && navigator.userAgent !== null)
{
    if (navigator.userAgent.indexOf("OS X") !== -1)
    {
        // iPhone etc also contain "OS X" in their userAgent, need to rule them out
        if (!navigator.userAgent.match(/(iPad)|(iPhone)|(iPod)/i))
        {
            wm.platform = "OSX";
        }
    }
    else if (navigator.userAgent.indexOf("Windows") !== -1)
    {
        wm.platform = "Windows";
    }
}

wm.SINGLECALL = 0;
wm.MULTICALL = 1;

// wm helper functions
wm.helper = {};
wm.helper.validateParam = function (obj, param, type, jsDefault)
{
    if (param in obj  && typeof obj[param] != type)
    {
        wm.error("Parameter " + param + " must be a " + type);
        return false;
    }
    
    if (!(param in obj))
    {
        obj[param] = jsDefault;
    }
        
    return true;
};

wm.helper.inherit = function (C, P)
{   
    var F = function() {};
    F.prototype = P.prototype;
    C.prototype = new F();
    C._super = P.prototype;
};

//Logging and error functions that can be directed as desired
wm.log = function(out)
{
    //Do nothing by default
};

wm.error = function(out)
{
    if (typeof console != "undefined" && typeof console.debug == "function")
        console.debug(out);
};

wm.throwError = function(err)
{
    throw(err);
};

wm.deviceready = function (event)
{
    // todo: ondomcontentload
    // Signal to the bridge that we're ready
    wm.bridge.exec(null, "events", "deviceready", {"isStart": true});
};

wm.addRemoveEventListener = function (add, evt, handler, capture)
{
    //TODO Should we consider passing on these events to the default functions?
    if (typeof evt == "string" && typeof wm.bridge.m_validEvents[evt] == 'string')
    {
        // This is one of our events
        if (add ? wm.bridge.addEvent(evt, handler) : wm.bridge.removeEvent(evt, handler))
        {
            // We should never call into the bridge for deviceready
            if (evt !== "deviceready")
            {
                // We only call through to exec if the first event of this type has been added
                // or the last one removed, i.e. only if listeners need switching on/off
                wm.bridge.exec(null, wm.bridge.m_validEvents[evt], evt, {isStart : add});
            }
        }
        return;
    }

    // Not one of our events so pass it on
    if (add)
        wm.realDocumentAddEventListener.call(document, evt, handler, capture);
    else
        wm.realDocumentRemoveEventListener.call(document, evt, handler, capture);

    return;
};

wm.Bridge = function()
{    
    this.m_errorLog = [];
    this.m_callbackID = 0;
    this.m_watchID = 0;
    this.m_callbackTable = [];
    this.m_eventTable = {};
    this.m_validEvents = {};
    
    // Register this one first as it needs handling by the default function
    window.addEventListener("load", wm.deviceready);

    // Save the real event listener to the document (needs to be the document
    // so it'll be in scope when we call the override).
    //document.wm = {};
    wm.realDocumentAddEventListener = document.addEventListener;
    wm.realDocumentRemoveEventListener = document.removeEventListener;

    // Override the default in order to intercept
    document.addEventListener = function(evt, handler, capture)
    {
        wm.addRemoveEventListener(true, evt, handler, capture);
    };

    // Override the default in order to intercept
    document.removeEventListener = function(evt, handler, capture)
    {
        wm.addRemoveEventListener(false, evt, handler, capture);
    };
};

wm.Bridge.ProxyBase = function(obj, usercb)
{
    this.obj = obj;
    this.callbacks = usercb;
    this.params = {};
    this.callbackid = null;
    this.callbacktype = wm.SINGLECALL; 
    this.method = "";
};

wm.nop = function()
{
};

wm.Bridge.ProxyBase.prototype.userCallback = function(cbstr)
{
    if (typeof this.callbacks[cbstr] == "function")
        return this.callbacks[cbstr];
    return wm.nop;
};

wm.Bridge.ProxyBase.prototype.userClassCallback = function(cbstr)
{
    if (typeof this.obj[cbstr] == "function")
        return this.obj[cbstr];    
    return wm.nop;
};

wm.Bridge.prototype.exec = function(cbproxy, module, name, params)
{
    if (typeof module != "string" || typeof name != "string")
        return false;

    var callbackID = this.addCallback(cbproxy);
    if (callbackID === null)
    {
        this.internalError("wm.bridge.exec: callbackID could not be generated.");
        return false;
    }
    
    var jsonString = JSON.stringify( { name: name, module: module, callbackID: callbackID, parameters: params } );
    //var jsonString = JSON.stringify(new function()
        //{this.name = name; this.module = module; this.callbackID = callbackID; this.parameters = params;});

    this.execPlatform(jsonString);
    return true;    
};

wm.Bridge.prototype.execPlatform = function(string)
{
    if (typeof s3e != "undefined" && typeof s3e.exec == "function")
    {
        s3e.exec(string);
    }
    else
    {
        this.internalError("Web Bridge not initialised.");
    }
};

wm.Bridge.prototype.addCallback = function(cbproxy)
{
    // check if cbproxy is defined
    if (typeof cbproxy === "undefined" || (cbproxy !== null && typeof cbproxy !== "object"))
    {        
        this.internalError("wm.bridge.addCallback: callback proxy is undefined.");
        return null;
    }
    
    // generate new callbackid
    var retVal = this.m_callbackID;
    this.m_callbackID++;
    
    // store callbackid so we can delete from table later on
    if (cbproxy !== null)
        cbproxy.callbackid = retVal;//this.m_callbackID;
    
    // store callback proxy in table
    this.m_callbackTable[retVal] = cbproxy;
        
    return retVal;
};

wm.Bridge.prototype.deleteCallback = function(id)
{
    // check if callbackid within range and callback exists
    if (id >= this.m_callbackTable.length || this.m_callbackTable[id] === null)
    {
        this.internalError("wm.bridge.deleteCallback: callback does not exist.");
        return false;
    }
    
    // delete callback proxy
    delete this.m_callbackTable[id];
    this.m_callbackTable[id] = null;
    return true;
};

wm.Bridge.prototype.callCallback = function(id, cbstr, data)
{
    if (id < 0 || id >= this.m_callbackTable.length)
    {
        this.internalError("wm.bridge.callCallback: invalid callback id.");
        return false;
    }

    var cbproxy = this.m_callbackTable[id];

    if (cbproxy !== null && typeof cbproxy[cbstr] == 'function')
    {
        // call proxy callback
        cbproxy[cbstr](data);
        
        // delete callback if we are one-shot
        if (cbproxy.callbacktype == wm.SINGLECALL)
        {
            this.deleteCallback(id);
        }        
    }
    else
    {
        this.internalError("wm.bridge.callCallback: callback does not exist.");
        return false;
    }

    return true;
};

// main entry point from native to call callback proxy and then user callback
wm.Bridge.prototype.callback = function(obj)
{
    if (typeof obj.callbackID === "undefined")
    {
        this.internalError("wm.bridge.callback: undefined callbackID received.");
        return;
    }

    if (typeof obj.parameters === "undefined")
    {
        this.internalError("wm.bridge.callback: undefined parameters received.");
        return;
    }

    if (typeof obj.callbackName === "undefined")
    {
        this.internalError("wm.bridge.callback: undefined callbackName received.");
        return;
    }    
    
    this.callCallback(obj.callbackID, obj.callbackName, obj.parameters);
};

wm.Bridge.prototype.registerEvent = function(eventName, moduleName)
{
    this.m_validEvents[eventName] = moduleName;
};

// Returns true if this handler is the first one added to this event
wm.Bridge.prototype.addEvent = function(eventName, handler)
{
    if (handler === null || typeof handler !== "function")
    {
        wm.error("Invalid event handler");
        return false;
    }

    if (!(eventName in this.m_eventTable))
    {
        this.m_eventTable[eventName] = new Array();
        this.m_eventTable[eventName].push(handler);
        return true;
    }

    if (this.m_eventTable[eventName].indexOf(handler) != -1)
        return false; //handler already registered (not an error)

    // Add the handler
    this.m_eventTable[eventName].push(handler);

    // Return true if the table was previously empty
    return (this.m_eventTable[eventName].length == 1);
};

// Returns true if, after removing, this event has no more handlers
wm.Bridge.prototype.removeEvent = function(eventName, handler)
{
    if (handler === null || typeof handler !== "function")
    {
        wm.error("Invalid event handler");
        return false;
    }

    if (!(eventName in this.m_eventTable))
        return false; //handler already registered (not an error)

    var index = this.m_eventTable[eventName].indexOf(handler);

    if (index == -1)
        return false; //handler already registered (not an error)

    this.m_eventTable[eventName].splice(index, 1);
    return (this.m_eventTable[eventName].length === 0);
};

wm.Bridge.prototype.fireEvent = function(obj)
{
    // A special case for device ready.  We need to populate our globals first
    if (obj.eventName === "deviceready")
    {
        // TODO This is a handwritten reviver - needs auto-genning
        // Copy the globals and fire the deviceReady event
        var d = obj.parameters.device;
        var dev = new Device(d.name, d.phonegap, d.platform, d.uuid, d.version, d.marmalade);
        dev.addTo(window);        
        
        // TODO Temporary wm check
        if (window.device.marmalade != wm.version)
        {
            this.internalError("Marmalade native/js version mismatch");
        }

    }
    if (!(obj.eventName in this.m_eventTable))
    {
        return; //no events (not an error)
    }

    // Wanted to use Object.keys() to check the sub-objects of obj.parameters,
    // e.g. Object.keys(obj.parameters), but it doesn't work on Android 2.1

    var numProperties = 0;
    var firstProperty = null;
    for(var prop in obj.parameters)
    {
        if (numProperties === 0)
            firstProperty = prop;
        numProperties = numProperties + 1;
    }

    if (numProperties > 1)
    {
        this.internalError(obj.eventName + " event has bad data");
        return;
    }

    var eventData = null;
    if (numProperties == 1)
        eventData = obj.parameters[firstProperty];

    for (var iHandler = 0; iHandler < this.m_eventTable[obj.eventName].length; ++iHandler)
    {
        this.m_eventTable[obj.eventName][iHandler](eventData);
    }

    return;
};

wm.Bridge.prototype.createID = function()
{
    return this.m_watchID++;
};

wm.Bridge.prototype.internalError = function(errstr)
{
    this.m_errorLog.push(errstr);
    wm.error("Internal error: " + errstr);
};

wm.bridge = new wm.Bridge();


/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Platform specific code
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 * This javascript code must be included into any webapp that wants to
 * make callbacks to native (C/C++) code on iOS.
 *
 * Ideally this would be inject automatically by the iOS extension but
 * I wasn't able to find a javascript injection hook that would allow
 * me to run this code on a new page before the onload handlers.  So we
 * have to ask the application to load this bit of javascript.
 */
function _iOSSetup()
{
    var frame = document.createElement('iframe');
    frame.setAttribute('style', 'display:none;');
    frame.setAttribute('height','0px');
    frame.setAttribute('width','0px');
    frame.setAttribute('frameborder','0');

    document.documentElement.appendChild(frame);

    s3e = {};
    s3e.queue = null;
    s3e.frame = frame;

    s3e.exec = function(jsonString)
    {
        if (s3e.queue === null)
        {
            s3e.queue = '!' + jsonString.length + '!' + jsonString;
            s3e.frame.src = 's3ebridge://queued';
        }
        else
        {
            s3e.queue += '!' + jsonString.length + '!' + jsonString;
        }
    };

    s3e.fetchqueued = function fetchqueued()
    {
        var queue = s3e.queue;
        s3e.queue = null;
        return queue;
    };
}

if (navigator.userAgent.match(/(iPad)|(iPhone)|(iPod)/i))
{
    _iOSSetup();
}
//
// events API
//
wm.bridge.registerEvent('deviceready','events');
wm.bridge.registerEvent('pause','events');
wm.bridge.registerEvent('resume','events');
wm.bridge.registerEvent('online','events');
wm.bridge.registerEvent('offline','events');
wm.bridge.registerEvent('backbutton','events');
wm.bridge.registerEvent('batterycritical','events');
wm.bridge.registerEvent('batterylow','events');
wm.bridge.registerEvent('batterystatus','events');
wm.bridge.registerEvent('menubutton','events');
wm.bridge.registerEvent('searchbutton','events');
wm.bridge.registerEvent('startcallbutton','events');
wm.bridge.registerEvent('endcallbutton','events');
wm.bridge.registerEvent('volumedownbutton','events');
wm.bridge.registerEvent('volumeupbutton','events');

var Events = function() {};
Events.Proxy = function(obj, cbx)
{
    Events.Proxy._super.constructor.apply(this, [obj, cbx]);
};

wm.helper.inherit(Events.Proxy, wm.Bridge.ProxyBase);


var Device = function(name, phonegap, platform, uuid, version, marmalade)
{
    this.name = (typeof(name) == "undefined") ? "" : name;
    this.phonegap = (typeof(phonegap) == "undefined") ? "" : phonegap;
    this.platform = (typeof(platform) == "undefined") ? "" : platform;
    this.uuid = (typeof(uuid) == "undefined") ? "" : uuid;
    this.version = (typeof(version) == "undefined") ? "" : version;
    this.marmalade = (typeof(marmalade) == "undefined") ? "" : marmalade;

};

Device.validateParams = function(obj)
{
    // Check that the values in obj are valid parameters for this class.
    // Keys which do not match class parameters are ignored
    // Missing keys are added with default values for the type
    if (!wm.helper.validateParam(obj, 'name', 'string', ""))
        return false;

    if (!wm.helper.validateParam(obj, 'phonegap', 'string', ""))
        return false;

    if (!wm.helper.validateParam(obj, 'platform', 'string', ""))
        return false;

    if (!wm.helper.validateParam(obj, 'uuid', 'string', ""))
        return false;

    if (!wm.helper.validateParam(obj, 'version', 'string', ""))
        return false;

    if (!wm.helper.validateParam(obj, 'marmalade', 'string', ""))
        return false;

    return true;
};



var BatteryInfo = function()
{
    this.level = 0;
    this.isPlugged = false;

};

BatteryInfo.validateParams = function(obj)
{
    // Check that the values in obj are valid parameters for this class.
    // Keys which do not match class parameters are ignored
    // Missing keys are added with default values for the type
    if (!wm.helper.validateParam(obj, 'level', 'number', 0))
        return false;

    if (!wm.helper.validateParam(obj, 'isPlugged', 'boolean', false))
        return false;

    return true;
};







Device.prototype.addTo = function(obj)
{
    if (typeof obj.device == 'undefined')
        obj.device = this;
    else 
    {
        var tmp = obj.device;
        obj.device = this;
        
        for (item in tmp)
        {
            // Only add back stuff that hasn't been redefined.
            if (!obj.device.hasOwnProperty(item))
                obj.device[item] = tmp[item];
        }
    }
};

if (typeof window.device == 'undefined')
	window.device = {};


window.device = new Device();        
//
// accelerometer API
//

var Accelerometer = function() {};
Accelerometer.Proxy = function(obj, cbx)
{
    Accelerometer.Proxy._super.constructor.apply(this, [obj, cbx]);
};

wm.helper.inherit(Accelerometer.Proxy, wm.Bridge.ProxyBase);


var Acceleration = function(x, y, z)
{
    this.x = (typeof(x) == "undefined") ? 0 : x;
    this.y = (typeof(y) == "undefined") ? 0 : y;
    this.z = (typeof(z) == "undefined") ? 0 : z;
    this.timestamp = new Date().getTime();

};

Acceleration.validateParams = function(obj)
{
    // Check that the values in obj are valid parameters for this class.
    // Keys which do not match class parameters are ignored
    // Missing keys are added with default values for the type
    if (!wm.helper.validateParam(obj, 'x', 'number', 0))
        return false;

    if (!wm.helper.validateParam(obj, 'y', 'number', 0))
        return false;

    if (!wm.helper.validateParam(obj, 'z', 'number', 0))
        return false;

    if (!wm.helper.validateParam(obj, 'timestamp', 'object', null))
        return false;

    return true;
};



var AccelerometerOptions = function()
{
    this.frequency = 10000;

};

AccelerometerOptions.validateParams = function(obj)
{
    // Check that the values in obj are valid parameters for this class.
    // Keys which do not match class parameters are ignored
    // Missing keys are added with default values for the type
    if (!wm.helper.validateParam(obj, 'frequency', 'number', 0))
        return false;

    return true;
};





if (typeof navigator.accelerometer === 'undefined') {
    navigator.accelerometer = new Accelerometer();
navigator.accelerometer.getCurrentAcceleration = function(accelerometerSuccess, accelerometerError)
{
    // Check for required parameters
    if (typeof accelerometerSuccess === "undefined")
    {
        wm.error("Parameter accelerometerSuccess is undefined");
        return false;
    }

    // Check parameter types and default unsupplied ones
    if (accelerometerSuccess !== null && typeof accelerometerSuccess != 'function')
    {
        wm.error("Parameter accelerometerSuccess must be a function or null");
        return false;
    }
    
    
    if (typeof accelerometerError === "undefined")
    {
        accelerometerError = null;
    }
    else if (accelerometerError !== null && typeof accelerometerError != 'function')
    {
        wm.error("Parameter accelerometerError must be a function or null");
        return false;
    }
    
    

    var proxy = new Accelerometer.Proxy(this, {'accelerometerSuccess':accelerometerSuccess, 'accelerometerError':accelerometerError});
    proxy.method = 'getCurrentAcceleration';
    proxy.params = {};

    if (typeof proxy._getCurrentAcceleration == 'function')
        proxy._getCurrentAcceleration();
    wm.bridge.exec(proxy, 'accelerometer', 'getCurrentAcceleration', proxy.params);

    return true;
};

navigator.accelerometer.watchAcceleration = function(accelerometerSuccess, accelerometerError, accelerometerOptions)
{
    // Check for required parameters
    if (typeof accelerometerSuccess === "undefined")
    {
        wm.error("Parameter accelerometerSuccess is undefined");
        return false;
    }

    // Check parameter types and default unsupplied ones
    if (accelerometerSuccess !== null && typeof accelerometerSuccess != 'function')
    {
        wm.error("Parameter accelerometerSuccess must be a function or null");
        return false;
    }
    
    
    if (typeof accelerometerError === "undefined")
    {
        accelerometerError = null;
    }
    else if (accelerometerError !== null && typeof accelerometerError != 'function')
    {
        wm.error("Parameter accelerometerError must be a function or null");
        return false;
    }
    
    
    if (typeof accelerometerOptions === "undefined")
    {
        accelerometerOptions = new AccelerometerOptions();
    }
    else if (accelerometerOptions !== null && !(accelerometerOptions instanceof AccelerometerOptions))
    {
        // Check that the object contains valid parameters for the class
        if (!AccelerometerOptions.validateParams(accelerometerOptions))
        {
            wm.error("Parameter accelerometerOptions must be an instance of AccelerometerOptions or an Object containinig valid parameters or be null");
            return false;
        }
    }
    
    

    var watchID;

    var proxy = new Accelerometer.Proxy(this, {'accelerometerSuccess':accelerometerSuccess, 'accelerometerError':accelerometerError});
    proxy.method = 'watchAcceleration';
    proxy.params = {watchID:watchID, accelerometerOptions:accelerometerOptions};

    if (typeof proxy._watchAcceleration == 'function')
        watchID = proxy._watchAcceleration();
    proxy.params['watchID'] = watchID;
    wm.bridge.exec(proxy, 'accelerometer', 'watchAcceleration', proxy.params);

    return watchID;
};

navigator.accelerometer.clearWatch = function(watchID)
{
    // Check for required parameters
    if (typeof watchID === "undefined")
    {
        wm.error("Parameter watchID is undefined");
        return false;
    }

    // Check parameter types and default unsupplied ones
    if (watchID !== null && typeof watchID != 'number')
    {
        wm.error("Parameter watchID must be a number or null");
        return false;
    }
    
    // check unsigned
    if (watchID < 0)
    {
        wm.error("Parameter watchID cannot be negative.");
        return false;
    }


    var proxy = new Accelerometer.Proxy(this, {});
    proxy.method = 'clearWatch';
    proxy.params = {watchID:watchID};

    if (typeof proxy._clearWatch == 'function')
        proxy._clearWatch();
    wm.bridge.exec(null, 'accelerometer', 'clearWatch', proxy.params);

    return true;
};

}



Accelerometer.Proxy.prototype.accelerometerSuccess = function(obj)
{
    var acc = new Acceleration(obj.acceleration.x, obj.acceleration.y, obj.acceleration.z);
    acc.timestamp = new Date(obj.acceleration.timestamp);
    
    this.userCallback('accelerometerSuccess')(acc);
};

Accelerometer.Proxy.prototype.accelerometerError = function(obj)
{
    this.userCallback('accelerometerError')();
};


Accelerometer.Proxy.prototype._getCurrentAcceleration = function()
{
    this.callbacktype = wm.SINGLECALL;
};

Accelerometer.Proxy.prototype._watchAcceleration = function()
{
    this.callbacktype = wm.MULTICALL;
    return wm.bridge.createID();
};

Accelerometer.Proxy.prototype._clearWatch = function()
{
};
//
// camera API
//

var Camera = function() {};
Camera.Proxy = function(obj, cbx)
{
    Camera.Proxy._super.constructor.apply(this, [obj, cbx]);
};

wm.helper.inherit(Camera.Proxy, wm.Bridge.ProxyBase);


var CameraOptions = function()
{
    this.quality = 80;
    this.destinationType = 0;
    this.sourceType = 0;
    this.allowEdit = false;
    this.encodingType = 0;
    this.targetWidth = 0;
    this.targetHeight = 0;
    this.mediaType = 0;

};

CameraOptions.validateParams = function(obj)
{
    // Check that the values in obj are valid parameters for this class.
    // Keys which do not match class parameters are ignored
    // Missing keys are added with default values for the type
    if (!wm.helper.validateParam(obj, 'quality', 'number', 0))
        return false;

    if (!wm.helper.validateParam(obj, 'destinationType', 'number', 0))
        return false;

    if (!wm.helper.validateParam(obj, 'sourceType', 'number', 0))
        return false;

    if (!wm.helper.validateParam(obj, 'allowEdit', 'boolean', false))
        return false;

    if (!wm.helper.validateParam(obj, 'encodingType', 'number', 0))
        return false;

    if (!wm.helper.validateParam(obj, 'targetWidth', 'number', 0))
        return false;

    if (!wm.helper.validateParam(obj, 'targetHeight', 'number', 0))
        return false;

    if (!wm.helper.validateParam(obj, 'mediaType', 'number', 0))
        return false;

    return true;
};




Camera.DestinationType = {};
Camera.DestinationType.DATA_URL = 0;
Camera.DestinationType.FILE_URI = 1;
Camera.prototype.DestinationType = Camera.DestinationType;

Camera.PictureSourceType = {};
Camera.PictureSourceType.PHOTOLIBRARY = 0;
Camera.PictureSourceType.CAMERA = 1;
Camera.PictureSourceType.SAVEDPHOTOALBUM = 2;
Camera.prototype.PictureSourceType = Camera.PictureSourceType;

Camera.EncodingType = {};
Camera.EncodingType.JPEG = 0;
Camera.EncodingType.PNG = 1;
Camera.prototype.EncodingType = Camera.EncodingType;

Camera.MediaType = {};
Camera.MediaType.PICTURE = 0;
Camera.MediaType.VIDEO = 1;
Camera.MediaType.ALLMEDIA = 2;
Camera.prototype.MediaType = Camera.MediaType;


if (typeof navigator.camera === 'undefined') {
    navigator.camera = new Camera();
navigator.camera.getPicture = function(cameraSuccess, cameraError, cameraOptions)
{
    // Check for required parameters
    if (typeof cameraSuccess === "undefined")
    {
        wm.error("Parameter cameraSuccess is undefined");
        return false;
    }
    if (typeof cameraError === "undefined")
    {
        wm.error("Parameter cameraError is undefined");
        return false;
    }

    // Check parameter types and default unsupplied ones
    if (cameraSuccess !== null && typeof cameraSuccess != 'function')
    {
        wm.error("Parameter cameraSuccess must be a function or null");
        return false;
    }
    
    
    if (cameraError !== null && typeof cameraError != 'function')
    {
        wm.error("Parameter cameraError must be a function or null");
        return false;
    }
    
    
    if (typeof cameraOptions === "undefined")
    {
        cameraOptions = new CameraOptions();
    }
    else if (cameraOptions !== null && !(cameraOptions instanceof CameraOptions))
    {
        // Check that the object contains valid parameters for the class
        if (!CameraOptions.validateParams(cameraOptions))
        {
            wm.error("Parameter cameraOptions must be an instance of CameraOptions or an Object containinig valid parameters or be null");
            return false;
        }
    }
    
    

    var proxy = new Camera.Proxy(this, {'cameraSuccess':cameraSuccess, 'cameraError':cameraError});
    proxy.method = 'getPicture';
    proxy.params = {cameraOptions:cameraOptions};

    if (typeof proxy._getPicture == 'function')
        proxy._getPicture();
    wm.bridge.exec(proxy, 'camera', 'getPicture', proxy.params);

    return true;
};

}



Camera.Proxy.prototype.cameraSuccess = function(obj)
{
    var imgURI = obj.imageData;
    var cameraSuccess = this.userCallback('cameraSuccess');
    var cameraError = this.userCallback('cameraError');
    var cameraOptions = this.params['cameraOptions'];
    
    if (cameraOptions.destinationType == Camera.DestinationType.FILE_URI) // user requested uri
    {
        if (imgURI.indexOf("raw://")>=0) {
            imgURI = imgURI.substr("raw://".length, imgURI.length-"raw://".length);
            imgURI = "file://" + imgURI;
        }        
        cameraSuccess(imgURI);        
    }
    else if (cameraOptions.destinationType == Camera.DestinationType.DATA_URL) // user requested base64
    {            
        window.requestFileSystem(LocalFileSystem.RAW, 0, 
            function(fileSystem) { // got filesystem
                if (imgURI.indexOf("raw://")>=0) {
                    imgURI = imgURI.substr("raw://".length, imgURI.length-"raw://".length);
                }
                fileSystem.root.getFile(imgURI, {create:false},
                    function(file) {
                        // create new FileReader
                        var reader = new FileReader();
                        reader.onloadend = function(evt) {
                            cameraSuccess(evt.target.result);
                        };
                        reader.onerror = function(evt) {
                            // todo: should we append camera specific error msg to this?
                            cameraError("Image file read error. FileError code: " + evt.target.error.code);
                        };
                        reader.readAsDataURL(file);
                    }, 
                    function(err) {
                        cameraError("Failed to retrieve image. FileError code: " + err.code);
                    });
            }, 
            function(err) { // failed to get filesystem
                // todo:should we append camera specific error msg to this?
                cameraError("Failed to access filesystem. FileError code: " + err.code);
            });
    }
    else
    {
        cameraError("Invalid DestinationType.");
    }    
};

Camera.Proxy.prototype.cameraError = function(obj)
{
    this.userCallback('cameraError')(obj.message);
};

Camera.Proxy.prototype._getPicture = function()
{

};
//
// capture API
//

var ConfigurationData = function()
{
    this.type = "";
    this.height = 0;
    this.width = 0;
    this._internal = 0;

};

ConfigurationData.validateParams = function(obj)
{
    // Check that the values in obj are valid parameters for this class.
    // Keys which do not match class parameters are ignored
    // Missing keys are added with default values for the type
    if (!wm.helper.validateParam(obj, 'type', 'string', ""))
        return false;

    if (!wm.helper.validateParam(obj, 'height', 'number', 0))
        return false;

    if (!wm.helper.validateParam(obj, 'width', 'number', 0))
        return false;

    if (!wm.helper.validateParam(obj, '_internal', 'number', 0))
        return false;

    return true;
};



var CaptureAudioOptions = function()
{
    this.limit = 1;
    this.duration = 0;
    this.mode = null;

};

CaptureAudioOptions.validateParams = function(obj)
{
    // Check that the values in obj are valid parameters for this class.
    // Keys which do not match class parameters are ignored
    // Missing keys are added with default values for the type
    if (!wm.helper.validateParam(obj, 'limit', 'number', 0))
        return false;

    if (!wm.helper.validateParam(obj, 'duration', 'number', 0))
        return false;

    if (!wm.helper.validateParam(obj, 'mode', 'object', null))
        return false;

    return true;
};



var CaptureImageOptions = function()
{
    this.limit = 1;
    this.mode = null;

};

CaptureImageOptions.validateParams = function(obj)
{
    // Check that the values in obj are valid parameters for this class.
    // Keys which do not match class parameters are ignored
    // Missing keys are added with default values for the type
    if (!wm.helper.validateParam(obj, 'limit', 'number', 0))
        return false;

    if (!wm.helper.validateParam(obj, 'mode', 'object', null))
        return false;

    return true;
};



var CaptureVideoOptions = function()
{
    this.limit = 1;
    this.duration = 0;
    this.mode = null;

};

CaptureVideoOptions.validateParams = function(obj)
{
    // Check that the values in obj are valid parameters for this class.
    // Keys which do not match class parameters are ignored
    // Missing keys are added with default values for the type
    if (!wm.helper.validateParam(obj, 'limit', 'number', 0))
        return false;

    if (!wm.helper.validateParam(obj, 'duration', 'number', 0))
        return false;

    if (!wm.helper.validateParam(obj, 'mode', 'object', null))
        return false;

    return true;
};



var MediaFileData = function(codecs, bitrate, height, width, duration)
{
    this.codecs = (typeof(codecs) == "undefined") ? "" : codecs;
    this.bitrate = (typeof(bitrate) == "undefined") ? 0 : bitrate;
    this.height = (typeof(height) == "undefined") ? 0 : height;
    this.width = (typeof(width) == "undefined") ? 0 : width;
    this.duration = (typeof(duration) == "undefined") ? 0 : duration;

};

MediaFileData.validateParams = function(obj)
{
    // Check that the values in obj are valid parameters for this class.
    // Keys which do not match class parameters are ignored
    // Missing keys are added with default values for the type
    if (!wm.helper.validateParam(obj, 'codecs', 'string', ""))
        return false;

    if (!wm.helper.validateParam(obj, 'bitrate', 'number', 0))
        return false;

    if (!wm.helper.validateParam(obj, 'height', 'number', 0))
        return false;

    if (!wm.helper.validateParam(obj, 'width', 'number', 0))
        return false;

    if (!wm.helper.validateParam(obj, 'duration', 'number', 0))
        return false;

    return true;
};



var MediaFile = function(name, fullPath, type, lastModifiedDate, size)
{
    this.name = (typeof(name) == "undefined") ? "" : name;
    this.fullPath = (typeof(fullPath) == "undefined") ? "" : fullPath;
    this.type = (typeof(type) == "undefined") ? "" : type;
    this.lastModifiedDate = (typeof(lastModifiedDate) == "undefined") ? new Date() : lastModifiedDate;
    this.size = (typeof(size) == "undefined") ? 0 : size;

};

MediaFile.Proxy = function(obj, cbx)
{
    MediaFile.Proxy._super.constructor.apply(this, [obj, cbx]);
};

wm.helper.inherit(MediaFile.Proxy, wm.Bridge.ProxyBase);



MediaFile.validateParams = function(obj)
{
    // Check that the values in obj are valid parameters for this class.
    // Keys which do not match class parameters are ignored
    // Missing keys are added with default values for the type
    if (!wm.helper.validateParam(obj, 'name', 'string', ""))
        return false;

    if (!wm.helper.validateParam(obj, 'fullPath', 'string', ""))
        return false;

    if (!wm.helper.validateParam(obj, 'type', 'string', ""))
        return false;

    if (!wm.helper.validateParam(obj, 'lastModifiedDate', 'object', null))
        return false;

    if (!wm.helper.validateParam(obj, 'size', 'number', 0))
        return false;

    return true;
};

MediaFile.prototype.getFormatData = function(successCallback, errorCallback)
{
    // Check for required parameters
    if (typeof successCallback === "undefined")
    {
        wm.error("Parameter successCallback is undefined");
        return false;
    }

    // Check parameter types and default unsupplied ones
    if (successCallback !== null && typeof successCallback != 'function')
    {
        wm.error("Parameter successCallback must be a function or null");
        return false;
    }
    
    
    if (typeof errorCallback === "undefined")
    {
        errorCallback = null;
    }
    else if (errorCallback !== null && typeof errorCallback != 'function')
    {
        wm.error("Parameter errorCallback must be a function or null");
        return false;
    }
    
    

    var proxy = new MediaFile.Proxy(this, {'successCallback':successCallback, 'errorCallback':errorCallback});
    proxy.method = 'getFormatData';
    proxy.params = {self:this};

    if (typeof proxy._getFormatData == 'function')
        proxy._getFormatData();
    wm.bridge.exec(proxy, 'capture', 'MediaFile_getFormatData', proxy.params);

    return true;
};



var Capture = function()
{
    this.supportedAudioModes = [];
    this.supportedImageModes = [];
    this.supportedVideoModes = [];

};

Capture.Proxy = function(obj, cbx)
{
    Capture.Proxy._super.constructor.apply(this, [obj, cbx]);
};

wm.helper.inherit(Capture.Proxy, wm.Bridge.ProxyBase);



Capture.validateParams = function(obj)
{
    // Check that the values in obj are valid parameters for this class.
    // Keys which do not match class parameters are ignored
    // Missing keys are added with default values for the type
    if (!wm.helper.validateParam(obj, 'supportedAudioModes', 'object', []))
        return false;

    if (!wm.helper.validateParam(obj, 'supportedImageModes', 'object', []))
        return false;

    if (!wm.helper.validateParam(obj, 'supportedVideoModes', 'object', []))
        return false;

    return true;
};

Capture.prototype.captureAudio = function(captureSuccess, captureError, options)
{
    // Check for required parameters
    if (typeof captureSuccess === "undefined")
    {
        wm.error("Parameter captureSuccess is undefined");
        return false;
    }
    if (typeof captureError === "undefined")
    {
        wm.error("Parameter captureError is undefined");
        return false;
    }

    // Check parameter types and default unsupplied ones
    if (captureSuccess !== null && typeof captureSuccess != 'function')
    {
        wm.error("Parameter captureSuccess must be a function or null");
        return false;
    }
    
    
    if (captureError !== null && typeof captureError != 'function')
    {
        wm.error("Parameter captureError must be a function or null");
        return false;
    }
    
    
    if (typeof options === "undefined")
    {
        options = new CaptureAudioOptions();
    }
    else if (options !== null && !(options instanceof CaptureAudioOptions))
    {
        // Check that the object contains valid parameters for the class
        if (!CaptureAudioOptions.validateParams(options))
        {
            wm.error("Parameter options must be an instance of CaptureAudioOptions or an Object containinig valid parameters or be null");
            return false;
        }
    }
    
    

    var proxy = new Capture.Proxy(this, {'captureSuccess':captureSuccess, 'captureError':captureError});
    proxy.method = 'captureAudio';
    proxy.params = {self:this, options:options};

    if (typeof proxy._captureAudio == 'function')
        proxy._captureAudio();
    wm.bridge.exec(proxy, 'capture', 'Capture_captureAudio', proxy.params);

    return true;
};

Capture.prototype.captureImage = function(captureSuccess, captureError, options)
{
    // Check for required parameters
    if (typeof captureSuccess === "undefined")
    {
        wm.error("Parameter captureSuccess is undefined");
        return false;
    }
    if (typeof captureError === "undefined")
    {
        wm.error("Parameter captureError is undefined");
        return false;
    }

    // Check parameter types and default unsupplied ones
    if (captureSuccess !== null && typeof captureSuccess != 'function')
    {
        wm.error("Parameter captureSuccess must be a function or null");
        return false;
    }
    
    
    if (captureError !== null && typeof captureError != 'function')
    {
        wm.error("Parameter captureError must be a function or null");
        return false;
    }
    
    
    if (typeof options === "undefined")
    {
        options = new CaptureImageOptions();
    }
    else if (options !== null && !(options instanceof CaptureImageOptions))
    {
        // Check that the object contains valid parameters for the class
        if (!CaptureImageOptions.validateParams(options))
        {
            wm.error("Parameter options must be an instance of CaptureImageOptions or an Object containinig valid parameters or be null");
            return false;
        }
    }
    
    

    var proxy = new Capture.Proxy(this, {'captureSuccess':captureSuccess, 'captureError':captureError});
    proxy.method = 'captureImage';
    proxy.params = {self:this, options:options};

    if (typeof proxy._captureImage == 'function')
        proxy._captureImage();
    wm.bridge.exec(proxy, 'capture', 'Capture_captureImage', proxy.params);

    return true;
};

Capture.prototype.captureVideo = function(captureSuccess, captureError, options)
{
    // Check for required parameters
    if (typeof captureSuccess === "undefined")
    {
        wm.error("Parameter captureSuccess is undefined");
        return false;
    }
    if (typeof captureError === "undefined")
    {
        wm.error("Parameter captureError is undefined");
        return false;
    }

    // Check parameter types and default unsupplied ones
    if (captureSuccess !== null && typeof captureSuccess != 'function')
    {
        wm.error("Parameter captureSuccess must be a function or null");
        return false;
    }
    
    
    if (captureError !== null && typeof captureError != 'function')
    {
        wm.error("Parameter captureError must be a function or null");
        return false;
    }
    
    
    if (typeof options === "undefined")
    {
        options = new CaptureVideoOptions();
    }
    else if (options !== null && !(options instanceof CaptureVideoOptions))
    {
        // Check that the object contains valid parameters for the class
        if (!CaptureVideoOptions.validateParams(options))
        {
            wm.error("Parameter options must be an instance of CaptureVideoOptions or an Object containinig valid parameters or be null");
            return false;
        }
    }
    
    

    var proxy = new Capture.Proxy(this, {'captureSuccess':captureSuccess, 'captureError':captureError});
    proxy.method = 'captureVideo';
    proxy.params = {self:this, options:options};

    if (typeof proxy._captureVideo == 'function')
        proxy._captureVideo();
    wm.bridge.exec(proxy, 'capture', 'Capture_captureVideo', proxy.params);

    return true;
};



var CaptureError = function()
{
    this.code = 0;

};

CaptureError.validateParams = function(obj)
{
    // Check that the values in obj are valid parameters for this class.
    // Keys which do not match class parameters are ignored
    // Missing keys are added with default values for the type
    if (!wm.helper.validateParam(obj, 'code', 'number', 0))
        return false;

    return true;
};



CaptureError.CAPTURE_INTERNAL_ERR = 0;
CaptureError.CAPTURE_APPLICATION_BUSY = 1;
CaptureError.CAPTURE_INVALID_ARGUMENT = 2;
CaptureError.CAPTURE_NO_MEDIA_FILES = 3;
CaptureError.CAPTURE_NOT_SUPPORTED = 20;






Capture.Proxy.prototype._captureAudio = function()
{
};

Capture.Proxy.prototype._captureVideo = function()
{
};

Capture.Proxy.prototype._captureImage = function()
{
};

Capture.Proxy.prototype.CaptureCB = function(obj)
{
    var mediaFiles = [];
    for (var i=0; i<obj.mediaFiles.length; i++)
    {
        mediaFiles[i] = new MediaFile(
            obj.mediaFiles[i].name,
            obj.mediaFiles[i].fullPath,
            obj.mediaFiles[i].type,
            new Date(obj.mediaFiles[i].lastModifiedDate),
            obj.mediaFiles[i].size);
    }    
    
    this.userCallback('captureSuccess')(mediaFiles);    
};

Capture.Proxy.prototype.CaptureErrorCB = function(obj)
{
    var caperror = new CaptureError();
    caperror.code = obj.error.code;

    this.userCallback('captureError')(caperror);    
};

MediaFile.Proxy.prototype.MediaFileDataSuccessCB = function(obj)
{
    var mediadata = new MediaFileData(
        obj.mediaFileObject.codecs,
        obj.mediaFileObject.bitrate,
        obj.mediaFileObject.height,
        obj.mediaFileObject.width,
        obj.mediaFileObject.duration);    
    
    this.userCallback('successCallback')(mediadata);
};

MediaFile.Proxy.prototype.MediaFileDataErrorCB = function(obj)
{
    this.userCallback('errorCallback')();
};

if (typeof navigator.device == 'undefined')
	navigator.device = {};
if (typeof navigator.device.capture == 'undefined')
	navigator.device.capture = {};

if (typeof window.device == 'undefined')
	window.device = {};
if (typeof window.device.capture == 'undefined')
	window.device.capture = {};


navigator.device.capture = window.device.capture = new Capture();        
//
// childBrowser API
//
wm.bridge.registerEvent('onLocationChangeEvent','childBrowser');
wm.bridge.registerEvent('onFailEvent','childBrowser');
wm.bridge.registerEvent('onCloseEvent','childBrowser');
wm.bridge.registerEvent('onOpenExternalEvent','childBrowser');

var Childbrowser = function() {};
Childbrowser.Proxy = function(obj, cbx)
{
    Childbrowser.Proxy._super.constructor.apply(this, [obj, cbx]);
};

wm.helper.inherit(Childbrowser.Proxy, wm.Bridge.ProxyBase);


var ChildBrowser = function()
{

};

ChildBrowser.Proxy = function(obj, cbx)
{
    ChildBrowser.Proxy._super.constructor.apply(this, [obj, cbx]);
};

wm.helper.inherit(ChildBrowser.Proxy, wm.Bridge.ProxyBase);



ChildBrowser.validateParams = function(obj)
{
    // Check that the values in obj are valid parameters for this class.
    // Keys which do not match class parameters are ignored
    // Missing keys are added with default values for the type
    return true;
};

ChildBrowser.prototype.showWebPage = function(url)
{
    // Check for required parameters
    if (typeof url === "undefined")
    {
        wm.error("Parameter url is undefined");
        return false;
    }

    // Check parameter types and default unsupplied ones
    if (url !== null && typeof url != 'string')
    {
        wm.error("Parameter url must be a string or null");
        return false;
    }
    
    

    var proxy = new ChildBrowser.Proxy(this, {});
    proxy.method = 'showWebPage';
    proxy.params = {self:this, url:url};

    if (typeof proxy._showWebPage == 'function')
        proxy._showWebPage();
    wm.bridge.exec(proxy, 'childBrowser', 'ChildBrowser_showWebPage', proxy.params);

    return true;
};

ChildBrowser.prototype.close = function()
{
    // Check for required parameters

    // Check parameter types and default unsupplied ones

    var proxy = new ChildBrowser.Proxy(this, {});
    proxy.method = 'close';
    proxy.params = {self:this};

    if (typeof proxy._close == 'function')
        proxy._close();
    wm.bridge.exec(proxy, 'childBrowser', 'ChildBrowser_close', proxy.params);

    return true;
};







// No handwritten code for this module

ChildBrowser.Proxy.prototype._showWebPage = function(url)
{
    document.addEventListener("onFailEvent", this.userClassCallback("onFail"), false);
    document.addEventListener("onLocationChangeEvent", this.userClassCallback("onLocationChange"), false);
    document.addEventListener("onCloseEvent", this.userClassCallback("onClose"), false);
    document.addEventListener("onOpenExternalEvent", this.userClassCallback("onOpenExternal"), false);
};

ChildBrowser.Proxy.prototype._close = function()
{
    document.removeEventListener("onFailEvent", this.userClassCallback("onFail"), false);
    document.removeEventListener("onLocationChangeEvent", this.userClassCallback("onLocationChange"), false);
    document.removeEventListener("onCloseEvent", this.userClassCallback("onClose"), false);
    document.removeEventListener("onOpenExternalEvent", this.userClassCallback("onOpenExternal"), false);
};
if (typeof window.plugins == 'undefined')
	window.plugins = {};
if (typeof window.plugins.childBrowser == 'undefined')
	window.plugins.childBrowser = {};
window.plugins.childBrowser = new ChildBrowser();
//
// compass API
//

var Compass = function() {};
Compass.Proxy = function(obj, cbx)
{
    Compass.Proxy._super.constructor.apply(this, [obj, cbx]);
};

wm.helper.inherit(Compass.Proxy, wm.Bridge.ProxyBase);


var CompassHeading = function()
{
    this.magneticHeading = 0;
    this.trueHeading = 0;
    this.headingAccuracy = 0;
    this.timestamp = new Date().getTime();

};

CompassHeading.validateParams = function(obj)
{
    // Check that the values in obj are valid parameters for this class.
    // Keys which do not match class parameters are ignored
    // Missing keys are added with default values for the type
    if (!wm.helper.validateParam(obj, 'magneticHeading', 'number', 0))
        return false;

    if (!wm.helper.validateParam(obj, 'trueHeading', 'number', 0))
        return false;

    if (!wm.helper.validateParam(obj, 'headingAccuracy', 'number', 0))
        return false;

    if (!wm.helper.validateParam(obj, 'timestamp', 'object', null))
        return false;

    return true;
};



var CompassOptions = function()
{
    this.frequency = 100;
    this.filter = 0;

};

CompassOptions.validateParams = function(obj)
{
    // Check that the values in obj are valid parameters for this class.
    // Keys which do not match class parameters are ignored
    // Missing keys are added with default values for the type
    if (!wm.helper.validateParam(obj, 'frequency', 'number', 0))
        return false;

    if (!wm.helper.validateParam(obj, 'filter', 'number', 0))
        return false;

    return true;
};



var CompassError = function()
{
    this.code = 0;

};

CompassError.validateParams = function(obj)
{
    // Check that the values in obj are valid parameters for this class.
    // Keys which do not match class parameters are ignored
    // Missing keys are added with default values for the type
    if (!wm.helper.validateParam(obj, 'code', 'number', 0))
        return false;

    return true;
};



CompassError.COMPASS_INTERNAL_ERR = 0;
CompassError.COMPASS_NOT_SUPPORTED = 20;



if (typeof navigator.compass === 'undefined') {
    navigator.compass = new Compass();
navigator.compass.getCurrentHeading = function(compassSuccess, compassError, compassOptions)
{
    // Check for required parameters
    if (typeof compassSuccess === "undefined")
    {
        wm.error("Parameter compassSuccess is undefined");
        return false;
    }
    if (typeof compassError === "undefined")
    {
        wm.error("Parameter compassError is undefined");
        return false;
    }

    // Check parameter types and default unsupplied ones
    if (compassSuccess !== null && typeof compassSuccess != 'function')
    {
        wm.error("Parameter compassSuccess must be a function or null");
        return false;
    }
    
    
    if (compassError !== null && typeof compassError != 'function')
    {
        wm.error("Parameter compassError must be a function or null");
        return false;
    }
    
    
    if (typeof compassOptions === "undefined")
    {
        compassOptions = new CompassOptions();
    }
    else if (compassOptions !== null && !(compassOptions instanceof CompassOptions))
    {
        // Check that the object contains valid parameters for the class
        if (!CompassOptions.validateParams(compassOptions))
        {
            wm.error("Parameter compassOptions must be an instance of CompassOptions or an Object containinig valid parameters or be null");
            return false;
        }
    }
    
    

    var proxy = new Compass.Proxy(this, {'compassSuccess':compassSuccess, 'compassError':compassError});
    proxy.method = 'getCurrentHeading';
    proxy.params = {compassOptions:compassOptions};

    if (typeof proxy._getCurrentHeading == 'function')
        proxy._getCurrentHeading();
    wm.bridge.exec(proxy, 'compass', 'getCurrentHeading', proxy.params);

    return true;
};

navigator.compass.watchHeading = function(compassSuccess, compassError, compassOptions)
{
    // Check for required parameters
    if (typeof compassSuccess === "undefined")
    {
        wm.error("Parameter compassSuccess is undefined");
        return false;
    }
    if (typeof compassError === "undefined")
    {
        wm.error("Parameter compassError is undefined");
        return false;
    }

    // Check parameter types and default unsupplied ones
    if (compassSuccess !== null && typeof compassSuccess != 'function')
    {
        wm.error("Parameter compassSuccess must be a function or null");
        return false;
    }
    
    
    if (compassError !== null && typeof compassError != 'function')
    {
        wm.error("Parameter compassError must be a function or null");
        return false;
    }
    
    
    if (typeof compassOptions === "undefined")
    {
        compassOptions = new CompassOptions();
    }
    else if (compassOptions !== null && !(compassOptions instanceof CompassOptions))
    {
        // Check that the object contains valid parameters for the class
        if (!CompassOptions.validateParams(compassOptions))
        {
            wm.error("Parameter compassOptions must be an instance of CompassOptions or an Object containinig valid parameters or be null");
            return false;
        }
    }
    
    

    var watchID;

    var proxy = new Compass.Proxy(this, {'compassSuccess':compassSuccess, 'compassError':compassError});
    proxy.method = 'watchHeading';
    proxy.params = {watchID:watchID, compassOptions:compassOptions};

    if (typeof proxy._watchHeading == 'function')
        watchID = proxy._watchHeading();
    proxy.params['watchID'] = watchID;
    wm.bridge.exec(proxy, 'compass', 'watchHeading', proxy.params);

    return watchID;
};

navigator.compass.clearWatch = function(watchID)
{
    // Check for required parameters
    if (typeof watchID === "undefined")
    {
        wm.error("Parameter watchID is undefined");
        return false;
    }

    // Check parameter types and default unsupplied ones
    if (watchID !== null && typeof watchID != 'number')
    {
        wm.error("Parameter watchID must be a number or null");
        return false;
    }
    
    // check unsigned
    if (watchID < 0)
    {
        wm.error("Parameter watchID cannot be negative.");
        return false;
    }


    var proxy = new Compass.Proxy(this, {});
    proxy.method = 'clearWatch';
    proxy.params = {watchID:watchID};

    if (typeof proxy._clearWatch == 'function')
        proxy._clearWatch();
    wm.bridge.exec(null, 'compass', 'clearWatch', proxy.params);

    return true;
};

navigator.compass.watchHeadingFilter = function(compassSuccess, compassError, compassOptions)
{
    // Check for required parameters
    if (typeof compassSuccess === "undefined")
    {
        wm.error("Parameter compassSuccess is undefined");
        return false;
    }
    if (typeof compassError === "undefined")
    {
        wm.error("Parameter compassError is undefined");
        return false;
    }
    if (typeof compassOptions === "undefined")
    {
        wm.error("Parameter compassOptions is undefined");
        return false;
    }

    // Check parameter types and default unsupplied ones
    if (compassSuccess !== null && typeof compassSuccess != 'function')
    {
        wm.error("Parameter compassSuccess must be a function or null");
        return false;
    }
    
    
    if (compassError !== null && typeof compassError != 'function')
    {
        wm.error("Parameter compassError must be a function or null");
        return false;
    }
    
    
    if (compassOptions !== null && !(compassOptions instanceof CompassOptions))
    {
        // Check that the object contains valid parameters for the class
        if (!CompassOptions.validateParams(compassOptions))
        {
            wm.error("Parameter compassOptions must be an instance of CompassOptions or an Object containinig valid parameters or be null");
            return false;
        }
    }
    
    

    var watchID;

    var proxy = new Compass.Proxy(this, {'compassSuccess':compassSuccess, 'compassError':compassError});
    proxy.method = 'watchHeadingFilter';
    proxy.params = {watchID:watchID, compassOptions:compassOptions};

    if (typeof proxy._watchHeadingFilter == 'function')
        watchID = proxy._watchHeadingFilter();
    proxy.params['watchID'] = watchID;
    wm.bridge.exec(proxy, 'compass', 'watchHeadingFilter', proxy.params);

    return watchID;
};

navigator.compass.clearWatchFilter = function(watchID)
{
    // Check for required parameters
    if (typeof watchID === "undefined")
    {
        wm.error("Parameter watchID is undefined");
        return false;
    }

    // Check parameter types and default unsupplied ones
    if (watchID !== null && typeof watchID != 'number')
    {
        wm.error("Parameter watchID must be a number or null");
        return false;
    }
    
    // check unsigned
    if (watchID < 0)
    {
        wm.error("Parameter watchID cannot be negative.");
        return false;
    }


    var proxy = new Compass.Proxy(this, {});
    proxy.method = 'clearWatchFilter';
    proxy.params = {watchID:watchID};

    if (typeof proxy._clearWatchFilter == 'function')
        proxy._clearWatchFilter();
    wm.bridge.exec(null, 'compass', 'clearWatchFilter', proxy.params);

    return true;
};

}


Compass.Proxy.prototype.compassSuccess = function(obj)
{
    var compass = new CompassHeading();    
    compass.magneticHeading = obj.heading.magneticHeading;
    compass.trueHeading = obj.heading.trueHeading;
    compass.headingAccuracy = obj.heading.headingAccuracy;
    compass.timestamp = new Date(obj.heading.timestamp);
    
    this.userCallback('compassSuccess')(compass);
};

Compass.Proxy.prototype.compassError = function(obj)
{
    var error = new CompassError();
    error.code = obj.error.code;
    
    this.userCallback('compassError')(error);
};

Compass.Proxy.prototype._watchHeading = function()
{
    this.callbacktype = wm.MULTICALL;
    return wm.bridge.createID();
};

Compass.Proxy.prototype._watchHeadingFilter = function()
{
    this.callbacktype = wm.MULTICALL;
    return wm.bridge.createID();
};

Compass.Proxy.prototype._getCurrentHeading = function()
{
};

Compass.Proxy.prototype._clearWatch = function()
{
};
//
// connection API
//
wm.bridge.registerEvent('network','connection');

var Connection = function()
{
    this.type = "none";

    Connection._construct(this);
};

Connection.validateParams = function(obj)
{
    // Check that the values in obj are valid parameters for this class.
    // Keys which do not match class parameters are ignored
    // Missing keys are added with default values for the type
    if (!wm.helper.validateParam(obj, 'type', 'string', ""))
        return false;

    return true;
};



Connection.UNKNOWN = "unknown";
Connection.ETHERNET = "ethernet";
Connection.WIFI = "wifi";
Connection.CELL_2G = "2g";
Connection.CELL_3G = "3g";
Connection.CELL_4G = "4g";
Connection.NONE = "none";





Connection._construct = function(obj)
{
    // register callback for network event
    var me = obj;
    
    this.networkchange = function(type)
    {
        me.type = type;    
    };
    
    document.addEventListener("network", this.networkchange, false);
};


if (typeof navigator.network == 'undefined')
	navigator.network = {};
if (typeof navigator.network.connection == 'undefined')
	navigator.network.connection = {};


navigator.network.connection = new Connection();        
//
// file API
//

var File = function(name, fullPath, type, lastModifiedDate, size)
{
    this.name = (typeof(name) == "undefined") ? "" : name;
    this.fullPath = (typeof(fullPath) == "undefined") ? "" : fullPath;
    this.type = (typeof(type) == "undefined") ? "" : type;
    this.lastModifiedDate = (typeof(lastModifiedDate) == "undefined") ? new Date() : lastModifiedDate;
    this.size = (typeof(size) == "undefined") ? 0 : size;

};

File.validateParams = function(obj)
{
    // Check that the values in obj are valid parameters for this class.
    // Keys which do not match class parameters are ignored
    // Missing keys are added with default values for the type
    if (!wm.helper.validateParam(obj, 'name', 'string', ""))
        return false;

    if (!wm.helper.validateParam(obj, 'fullPath', 'string', ""))
        return false;

    if (!wm.helper.validateParam(obj, 'type', 'string', ""))
        return false;

    if (!wm.helper.validateParam(obj, 'lastModifiedDate', 'object', null))
        return false;

    if (!wm.helper.validateParam(obj, 'size', 'number', 0))
        return false;

    return true;
};



var FileError = function(code)
{
    this.code = (typeof(code) == "undefined") ? 0 : code;

};

FileError.validateParams = function(obj)
{
    // Check that the values in obj are valid parameters for this class.
    // Keys which do not match class parameters are ignored
    // Missing keys are added with default values for the type
    if (!wm.helper.validateParam(obj, 'code', 'number', 0))
        return false;

    return true;
};



FileError.NOT_FOUND_ERR = 1;
FileError.SECURITY_ERR = 2;
FileError.ABORT_ERR = 3;
FileError.NOT_READABLE_ERR = 4;
FileError.ENCODING_ERR = 5;
FileError.NO_MODIFICATION_ALLOWED_ERR = 6;
FileError.INVALID_STATE_ERR = 7;
FileError.SYNTAX_ERR = 8;
FileError.INVALID_MODIFICATION_ERR = 9;
FileError.QUOTA_EXCEEDED_ERR = 10;
FileError.TYPE_MISMATCH_ERR = 11;
FileError.PATH_EXISTS_ERR = 12;

var FileReader = function()
{
    this.readyState = 0;
    this.result = "";
    this.error = null;
    this.onloadstart = null;
    this.onprogress = null;
    this.onload = null;
    this.onabort = null;
    this.onerror = null;
    this.onloadend = null;

};

FileReader.Proxy = function(obj, cbx)
{
    FileReader.Proxy._super.constructor.apply(this, [obj, cbx]);
};

wm.helper.inherit(FileReader.Proxy, wm.Bridge.ProxyBase);



FileReader.validateParams = function(obj)
{
    // Check that the values in obj are valid parameters for this class.
    // Keys which do not match class parameters are ignored
    // Missing keys are added with default values for the type
    if (!wm.helper.validateParam(obj, 'readyState', 'number', 0))
        return false;

    if (!wm.helper.validateParam(obj, 'result', 'string', ""))
        return false;

    if (!wm.helper.validateParam(obj, 'error', 'object', null))
        return false;

    if (!wm.helper.validateParam(obj, 'onloadstart', 'function', null))
        return false;

    if (!wm.helper.validateParam(obj, 'onprogress', 'function', null))
        return false;

    if (!wm.helper.validateParam(obj, 'onload', 'function', null))
        return false;

    if (!wm.helper.validateParam(obj, 'onabort', 'function', null))
        return false;

    if (!wm.helper.validateParam(obj, 'onerror', 'function', null))
        return false;

    if (!wm.helper.validateParam(obj, 'onloadend', 'function', null))
        return false;

    return true;
};

FileReader.prototype.readAsDataURL = function(file)
{
    // Check for required parameters
    if (typeof file === "undefined")
    {
        wm.error("Parameter file is undefined");
        return false;
    }

    // Check parameter types and default unsupplied ones
    if (file !== null && !(file instanceof File))
    {
        // Check that the object contains valid parameters for the class
        if (!File.validateParams(file))
        {
            wm.error("Parameter file must be an instance of File or an Object containinig valid parameters or be null");
            return false;
        }
    }
    
    

    var proxy = new FileReader.Proxy(this, {});
    proxy.method = 'readAsDataURL';
    proxy.params = {self:this, file:file};

    if (typeof proxy._readAsDataURL == 'function')
        proxy._readAsDataURL();
    wm.bridge.exec(proxy, 'file', 'FileReader_readAsDataURL', proxy.params);

    return true;
};

FileReader.prototype.readAsText = function(file, encoding)
{
    // Check for required parameters
    if (typeof file === "undefined")
    {
        wm.error("Parameter file is undefined");
        return false;
    }

    // Check parameter types and default unsupplied ones
    if (file !== null && !(file instanceof File))
    {
        // Check that the object contains valid parameters for the class
        if (!File.validateParams(file))
        {
            wm.error("Parameter file must be an instance of File or an Object containinig valid parameters or be null");
            return false;
        }
    }
    
    
    if (typeof encoding === "undefined")
    {
        encoding = "";
    }
    else if (encoding !== null && typeof encoding != 'string')
    {
        wm.error("Parameter encoding must be a string or null");
        return false;
    }
    
    

    var proxy = new FileReader.Proxy(this, {});
    proxy.method = 'readAsText';
    proxy.params = {self:this, file:file, encoding:encoding};

    if (typeof proxy._readAsText == 'function')
        proxy._readAsText();
    wm.bridge.exec(proxy, 'file', 'FileReader_readAsText', proxy.params);

    return true;
};

FileReader.prototype.abort = function()
{
    // Check for required parameters

    // Check parameter types and default unsupplied ones

    var proxy = new FileReader.Proxy(this, {});
    proxy.method = 'abort';
    proxy.params = {self:this};

    if (typeof proxy._abort == 'function')
        proxy._abort();
    
    return true;
};



FileReader.EMPTY = 0;
FileReader.LOADING = 1;
FileReader.DONE = 2;

var FileWriter = function(file)
{
    this.fileName = "";
    this.position = 0;
    this.length = 0;
    this.readyState = 0;
    this.error = null;
    this.onwritestart = null;
    this.onprogress = null;
    this.onwrite = null;
    this.onabort = null;
    this.onerror = null;
    this.onwriteend = null;

    
    if (file) {
        this.fileName = file.fullPath || "";
        this.length = file.size || 0;
    }                
            
};

FileWriter.Proxy = function(obj, cbx)
{
    FileWriter.Proxy._super.constructor.apply(this, [obj, cbx]);
};

wm.helper.inherit(FileWriter.Proxy, wm.Bridge.ProxyBase);



FileWriter.validateParams = function(obj)
{
    // Check that the values in obj are valid parameters for this class.
    // Keys which do not match class parameters are ignored
    // Missing keys are added with default values for the type
    if (!wm.helper.validateParam(obj, 'fileName', 'string', ""))
        return false;

    if (!wm.helper.validateParam(obj, 'position', 'number', 0))
        return false;

    if (!wm.helper.validateParam(obj, 'length', 'number', 0))
        return false;

    if (!wm.helper.validateParam(obj, 'readyState', 'number', 0))
        return false;

    if (!wm.helper.validateParam(obj, 'error', 'object', null))
        return false;

    if (!wm.helper.validateParam(obj, 'onwritestart', 'function', null))
        return false;

    if (!wm.helper.validateParam(obj, 'onprogress', 'function', null))
        return false;

    if (!wm.helper.validateParam(obj, 'onwrite', 'function', null))
        return false;

    if (!wm.helper.validateParam(obj, 'onabort', 'function', null))
        return false;

    if (!wm.helper.validateParam(obj, 'onerror', 'function', null))
        return false;

    if (!wm.helper.validateParam(obj, 'onwriteend', 'function', null))
        return false;

    return true;
};

FileWriter.prototype.write = function(data)
{
    // Check for required parameters
    if (typeof data === "undefined")
    {
        wm.error("Parameter data is undefined");
        return false;
    }

    // Check parameter types and default unsupplied ones
    if (data !== null && typeof data != 'string')
    {
        wm.error("Parameter data must be a string or null");
        return false;
    }
    
    

    var proxy = new FileWriter.Proxy(this, {});
    proxy.method = 'write';
    proxy.params = {self:this, data:data};

    if (typeof proxy._write == 'function')
        proxy._write();
    wm.bridge.exec(proxy, 'file', 'FileWriter_write', proxy.params);

    return true;
};

FileWriter.prototype.seek = function(position)
{
    // Check for required parameters
    if (typeof position === "undefined")
    {
        wm.error("Parameter position is undefined");
        return false;
    }

    // Check parameter types and default unsupplied ones
    if (position !== null && typeof position != 'number')
    {
        wm.error("Parameter position must be a number or null");
        return false;
    }
    
    

    var proxy = new FileWriter.Proxy(this, {});
    proxy.method = 'seek';
    proxy.params = {self:this, position:position};

    if (typeof proxy._seek == 'function')
        proxy._seek();
    
    return true;
};

FileWriter.prototype.truncate = function(size)
{
    // Check for required parameters
    if (typeof size === "undefined")
    {
        wm.error("Parameter size is undefined");
        return false;
    }

    // Check parameter types and default unsupplied ones
    if (size !== null && typeof size != 'number')
    {
        wm.error("Parameter size must be a number or null");
        return false;
    }
    
    // check unsigned
    if (size < 0)
    {
        wm.error("Parameter size cannot be negative.");
        return false;
    }


    var proxy = new FileWriter.Proxy(this, {});
    proxy.method = 'truncate';
    proxy.params = {self:this, size:size};

    if (typeof proxy._truncate == 'function')
        proxy._truncate();
    wm.bridge.exec(proxy, 'file', 'FileWriter_truncate', proxy.params);

    return true;
};

FileWriter.prototype.abort = function()
{
    // Check for required parameters

    // Check parameter types and default unsupplied ones

    var proxy = new FileWriter.Proxy(this, {});
    proxy.method = 'abort';
    proxy.params = {self:this};

    if (typeof proxy._abort == 'function')
        proxy._abort();
    
    return true;
};



FileWriter.INIT = 0;
FileWriter.WRITING = 1;
FileWriter.DONE = 2;

var FileEntry = function()
{
    this.isFile = false;
    this.isDirectory = false;
    this.name = "";
    this.fullPath = "";

    
	this.isFile = true;
	this.isDirectory = false;
        
};

FileEntry.Proxy = function(obj, cbx)
{
    FileEntry.Proxy._super.constructor.apply(this, [obj, cbx]);
};

wm.helper.inherit(FileEntry.Proxy, wm.Bridge.ProxyBase);



FileEntry.validateParams = function(obj)
{
    // Check that the values in obj are valid parameters for this class.
    // Keys which do not match class parameters are ignored
    // Missing keys are added with default values for the type
    if (!wm.helper.validateParam(obj, 'isFile', 'boolean', false))
        return false;

    if (!wm.helper.validateParam(obj, 'isDirectory', 'boolean', false))
        return false;

    if (!wm.helper.validateParam(obj, 'name', 'string', ""))
        return false;

    if (!wm.helper.validateParam(obj, 'fullPath', 'string', ""))
        return false;

    return true;
};

FileEntry.prototype.getMetadata = function(metadataSuccessCallback, errorCallback)
{
    // Check for required parameters
    if (typeof metadataSuccessCallback === "undefined")
    {
        wm.error("Parameter metadataSuccessCallback is undefined");
        return false;
    }
    if (typeof errorCallback === "undefined")
    {
        wm.error("Parameter errorCallback is undefined");
        return false;
    }

    // Check parameter types and default unsupplied ones
    if (metadataSuccessCallback !== null && typeof metadataSuccessCallback != 'function')
    {
        wm.error("Parameter metadataSuccessCallback must be a function or null");
        return false;
    }
    
    
    if (errorCallback !== null && typeof errorCallback != 'function')
    {
        wm.error("Parameter errorCallback must be a function or null");
        return false;
    }
    
    

    var proxy = new FileEntry.Proxy(this, {'metadataSuccessCallback':metadataSuccessCallback, 'errorCallback':errorCallback});
    proxy.method = 'getMetadata';
    proxy.params = {self:this};

    if (typeof proxy._getMetadata == 'function')
        proxy._getMetadata();
    wm.bridge.exec(proxy, 'file', 'FileEntry_getMetadata', proxy.params);

    return true;
};

FileEntry.prototype.moveTo = function(parent, newName, entrySuccessCallback, errorCallback)
{
    // Check for required parameters
    if (typeof parent === "undefined")
    {
        wm.error("Parameter parent is undefined");
        return false;
    }
    if (typeof newName === "undefined")
    {
        wm.error("Parameter newName is undefined");
        return false;
    }
    if (typeof entrySuccessCallback === "undefined")
    {
        wm.error("Parameter entrySuccessCallback is undefined");
        return false;
    }
    if (typeof errorCallback === "undefined")
    {
        wm.error("Parameter errorCallback is undefined");
        return false;
    }

    // Check parameter types and default unsupplied ones
    if (parent !== null && !(parent instanceof DirectoryEntry))
    {
        // Check that the object contains valid parameters for the class
        if (!DirectoryEntry.validateParams(parent))
        {
            wm.error("Parameter parent must be an instance of DirectoryEntry or an Object containinig valid parameters or be null");
            return false;
        }
    }
    
    
    if (newName !== null && typeof newName != 'string')
    {
        wm.error("Parameter newName must be a string or null");
        return false;
    }
    
    
    if (entrySuccessCallback !== null && typeof entrySuccessCallback != 'function')
    {
        wm.error("Parameter entrySuccessCallback must be a function or null");
        return false;
    }
    
    
    if (errorCallback !== null && typeof errorCallback != 'function')
    {
        wm.error("Parameter errorCallback must be a function or null");
        return false;
    }
    
    

    var proxy = new FileEntry.Proxy(this, {'entrySuccessCallback':entrySuccessCallback, 'errorCallback':errorCallback});
    proxy.method = 'moveTo';
    proxy.params = {self:this, parent:parent, newName:newName};

    if (typeof proxy._moveTo == 'function')
        proxy._moveTo();
    wm.bridge.exec(proxy, 'file', 'FileEntry_moveTo', proxy.params);

    return true;
};

FileEntry.prototype.copyTo = function(parent, newName, entrySuccessCallback, errorCallback)
{
    // Check for required parameters
    if (typeof parent === "undefined")
    {
        wm.error("Parameter parent is undefined");
        return false;
    }
    if (typeof newName === "undefined")
    {
        wm.error("Parameter newName is undefined");
        return false;
    }
    if (typeof entrySuccessCallback === "undefined")
    {
        wm.error("Parameter entrySuccessCallback is undefined");
        return false;
    }
    if (typeof errorCallback === "undefined")
    {
        wm.error("Parameter errorCallback is undefined");
        return false;
    }

    // Check parameter types and default unsupplied ones
    if (parent !== null && !(parent instanceof DirectoryEntry))
    {
        // Check that the object contains valid parameters for the class
        if (!DirectoryEntry.validateParams(parent))
        {
            wm.error("Parameter parent must be an instance of DirectoryEntry or an Object containinig valid parameters or be null");
            return false;
        }
    }
    
    
    if (newName !== null && typeof newName != 'string')
    {
        wm.error("Parameter newName must be a string or null");
        return false;
    }
    
    
    if (entrySuccessCallback !== null && typeof entrySuccessCallback != 'function')
    {
        wm.error("Parameter entrySuccessCallback must be a function or null");
        return false;
    }
    
    
    if (errorCallback !== null && typeof errorCallback != 'function')
    {
        wm.error("Parameter errorCallback must be a function or null");
        return false;
    }
    
    

    var proxy = new FileEntry.Proxy(this, {'entrySuccessCallback':entrySuccessCallback, 'errorCallback':errorCallback});
    proxy.method = 'copyTo';
    proxy.params = {self:this, parent:parent, newName:newName};

    if (typeof proxy._copyTo == 'function')
        proxy._copyTo();
    wm.bridge.exec(proxy, 'file', 'FileEntry_copyTo', proxy.params);

    return true;
};

FileEntry.prototype.remove = function(successCallback, errorCallback)
{
    // Check for required parameters
    if (typeof successCallback === "undefined")
    {
        wm.error("Parameter successCallback is undefined");
        return false;
    }
    if (typeof errorCallback === "undefined")
    {
        wm.error("Parameter errorCallback is undefined");
        return false;
    }

    // Check parameter types and default unsupplied ones
    if (successCallback !== null && typeof successCallback != 'function')
    {
        wm.error("Parameter successCallback must be a function or null");
        return false;
    }
    
    
    if (errorCallback !== null && typeof errorCallback != 'function')
    {
        wm.error("Parameter errorCallback must be a function or null");
        return false;
    }
    
    

    var proxy = new FileEntry.Proxy(this, {'successCallback':successCallback, 'errorCallback':errorCallback});
    proxy.method = 'remove';
    proxy.params = {self:this};

    if (typeof proxy._remove == 'function')
        proxy._remove();
    wm.bridge.exec(proxy, 'file', 'FileEntry_remove', proxy.params);

    return true;
};

FileEntry.prototype.getParent = function(directoryEntrySuccessCallback, errorCallback)
{
    // Check for required parameters
    if (typeof directoryEntrySuccessCallback === "undefined")
    {
        wm.error("Parameter directoryEntrySuccessCallback is undefined");
        return false;
    }
    if (typeof errorCallback === "undefined")
    {
        wm.error("Parameter errorCallback is undefined");
        return false;
    }

    // Check parameter types and default unsupplied ones
    if (directoryEntrySuccessCallback !== null && typeof directoryEntrySuccessCallback != 'function')
    {
        wm.error("Parameter directoryEntrySuccessCallback must be a function or null");
        return false;
    }
    
    
    if (errorCallback !== null && typeof errorCallback != 'function')
    {
        wm.error("Parameter errorCallback must be a function or null");
        return false;
    }
    
    

    var proxy = new FileEntry.Proxy(this, {'directoryEntrySuccessCallback':directoryEntrySuccessCallback, 'errorCallback':errorCallback});
    proxy.method = 'getParent';
    proxy.params = {self:this};

    if (typeof proxy._getParent == 'function')
        proxy._getParent();
    wm.bridge.exec(proxy, 'file', 'FileEntry_getParent', proxy.params);

    return true;
};

FileEntry.prototype.toURI = function()
{
    // Check for required parameters

    // Check parameter types and default unsupplied ones

    var uri;

    var proxy = new FileEntry.Proxy(this, {});
    proxy.method = 'toURI';
    proxy.params = {uri:uri, self:this};

    if (typeof proxy._toURI == 'function')
        uri = proxy._toURI();
    proxy.params['uri'] = uri;
    
    return "file:///" + this.fullPath;
};

FileEntry.prototype.createWriter = function(fileWriterSuccessCallback, errorCallback)
{
    // Check for required parameters
    if (typeof fileWriterSuccessCallback === "undefined")
    {
        wm.error("Parameter fileWriterSuccessCallback is undefined");
        return false;
    }
    if (typeof errorCallback === "undefined")
    {
        wm.error("Parameter errorCallback is undefined");
        return false;
    }

    // Check parameter types and default unsupplied ones
    if (fileWriterSuccessCallback !== null && typeof fileWriterSuccessCallback != 'function')
    {
        wm.error("Parameter fileWriterSuccessCallback must be a function or null");
        return false;
    }
    
    
    if (errorCallback !== null && typeof errorCallback != 'function')
    {
        wm.error("Parameter errorCallback must be a function or null");
        return false;
    }
    
    

    var proxy = new FileEntry.Proxy(this, {'fileWriterSuccessCallback':fileWriterSuccessCallback, 'errorCallback':errorCallback});
    proxy.method = 'createWriter';
    proxy.params = {self:this};

    if (typeof proxy._createWriter == 'function')
        proxy._createWriter();
    
    return true;
};

FileEntry.prototype.file = function(fileSuccessCallback, errorCallback)
{
    // Check for required parameters
    if (typeof fileSuccessCallback === "undefined")
    {
        wm.error("Parameter fileSuccessCallback is undefined");
        return false;
    }
    if (typeof errorCallback === "undefined")
    {
        wm.error("Parameter errorCallback is undefined");
        return false;
    }

    // Check parameter types and default unsupplied ones
    if (fileSuccessCallback !== null && typeof fileSuccessCallback != 'function')
    {
        wm.error("Parameter fileSuccessCallback must be a function or null");
        return false;
    }
    
    
    if (errorCallback !== null && typeof errorCallback != 'function')
    {
        wm.error("Parameter errorCallback must be a function or null");
        return false;
    }
    
    

    var proxy = new FileEntry.Proxy(this, {'fileSuccessCallback':fileSuccessCallback, 'errorCallback':errorCallback});
    proxy.method = 'file';
    proxy.params = {self:this};

    if (typeof proxy._file == 'function')
        proxy._file();
    wm.bridge.exec(proxy, 'file', 'FileEntry_file', proxy.params);

    return true;
};



var DirectoryEntry = function()
{
    this.isFile = false;
    this.isDirectory = false;
    this.name = "";
    this.fullPath = "";

    
	this.isFile = false;
	this.isDirectory = true;
	
};

DirectoryEntry.Proxy = function(obj, cbx)
{
    DirectoryEntry.Proxy._super.constructor.apply(this, [obj, cbx]);
};

wm.helper.inherit(DirectoryEntry.Proxy, wm.Bridge.ProxyBase);



DirectoryEntry.validateParams = function(obj)
{
    // Check that the values in obj are valid parameters for this class.
    // Keys which do not match class parameters are ignored
    // Missing keys are added with default values for the type
    if (!wm.helper.validateParam(obj, 'isFile', 'boolean', false))
        return false;

    if (!wm.helper.validateParam(obj, 'isDirectory', 'boolean', false))
        return false;

    if (!wm.helper.validateParam(obj, 'name', 'string', ""))
        return false;

    if (!wm.helper.validateParam(obj, 'fullPath', 'string', ""))
        return false;

    return true;
};

DirectoryEntry.prototype.getMetadata = function(metadataSuccessCallback, errorCallback)
{
    // Check for required parameters
    if (typeof metadataSuccessCallback === "undefined")
    {
        wm.error("Parameter metadataSuccessCallback is undefined");
        return false;
    }
    if (typeof errorCallback === "undefined")
    {
        wm.error("Parameter errorCallback is undefined");
        return false;
    }

    // Check parameter types and default unsupplied ones
    if (metadataSuccessCallback !== null && typeof metadataSuccessCallback != 'function')
    {
        wm.error("Parameter metadataSuccessCallback must be a function or null");
        return false;
    }
    
    
    if (errorCallback !== null && typeof errorCallback != 'function')
    {
        wm.error("Parameter errorCallback must be a function or null");
        return false;
    }
    
    

    var proxy = new DirectoryEntry.Proxy(this, {'metadataSuccessCallback':metadataSuccessCallback, 'errorCallback':errorCallback});
    proxy.method = 'getMetadata';
    proxy.params = {self:this};

    if (typeof proxy._getMetadata == 'function')
        proxy._getMetadata();
    wm.bridge.exec(proxy, 'file', 'DirectoryEntry_getMetadata', proxy.params);

    return true;
};

DirectoryEntry.prototype.moveTo = function(parent, newName, entrySuccessCallback, errorCallback)
{
    // Check for required parameters
    if (typeof parent === "undefined")
    {
        wm.error("Parameter parent is undefined");
        return false;
    }
    if (typeof newName === "undefined")
    {
        wm.error("Parameter newName is undefined");
        return false;
    }
    if (typeof entrySuccessCallback === "undefined")
    {
        wm.error("Parameter entrySuccessCallback is undefined");
        return false;
    }
    if (typeof errorCallback === "undefined")
    {
        wm.error("Parameter errorCallback is undefined");
        return false;
    }

    // Check parameter types and default unsupplied ones
    if (parent !== null && !(parent instanceof DirectoryEntry))
    {
        // Check that the object contains valid parameters for the class
        if (!DirectoryEntry.validateParams(parent))
        {
            wm.error("Parameter parent must be an instance of DirectoryEntry or an Object containinig valid parameters or be null");
            return false;
        }
    }
    
    
    if (newName !== null && typeof newName != 'string')
    {
        wm.error("Parameter newName must be a string or null");
        return false;
    }
    
    
    if (entrySuccessCallback !== null && typeof entrySuccessCallback != 'function')
    {
        wm.error("Parameter entrySuccessCallback must be a function or null");
        return false;
    }
    
    
    if (errorCallback !== null && typeof errorCallback != 'function')
    {
        wm.error("Parameter errorCallback must be a function or null");
        return false;
    }
    
    

    var proxy = new DirectoryEntry.Proxy(this, {'entrySuccessCallback':entrySuccessCallback, 'errorCallback':errorCallback});
    proxy.method = 'moveTo';
    proxy.params = {self:this, parent:parent, newName:newName};

    if (typeof proxy._moveTo == 'function')
        proxy._moveTo();
    wm.bridge.exec(proxy, 'file', 'DirectoryEntry_moveTo', proxy.params);

    return true;
};

DirectoryEntry.prototype.copyTo = function(parent, newName, entrySuccessCallback, errorCallback)
{
    // Check for required parameters
    if (typeof parent === "undefined")
    {
        wm.error("Parameter parent is undefined");
        return false;
    }
    if (typeof newName === "undefined")
    {
        wm.error("Parameter newName is undefined");
        return false;
    }
    if (typeof entrySuccessCallback === "undefined")
    {
        wm.error("Parameter entrySuccessCallback is undefined");
        return false;
    }
    if (typeof errorCallback === "undefined")
    {
        wm.error("Parameter errorCallback is undefined");
        return false;
    }

    // Check parameter types and default unsupplied ones
    if (parent !== null && !(parent instanceof DirectoryEntry))
    {
        // Check that the object contains valid parameters for the class
        if (!DirectoryEntry.validateParams(parent))
        {
            wm.error("Parameter parent must be an instance of DirectoryEntry or an Object containinig valid parameters or be null");
            return false;
        }
    }
    
    
    if (newName !== null && typeof newName != 'string')
    {
        wm.error("Parameter newName must be a string or null");
        return false;
    }
    
    
    if (entrySuccessCallback !== null && typeof entrySuccessCallback != 'function')
    {
        wm.error("Parameter entrySuccessCallback must be a function or null");
        return false;
    }
    
    
    if (errorCallback !== null && typeof errorCallback != 'function')
    {
        wm.error("Parameter errorCallback must be a function or null");
        return false;
    }
    
    

    var proxy = new DirectoryEntry.Proxy(this, {'entrySuccessCallback':entrySuccessCallback, 'errorCallback':errorCallback});
    proxy.method = 'copyTo';
    proxy.params = {self:this, parent:parent, newName:newName};

    if (typeof proxy._copyTo == 'function')
        proxy._copyTo();
    wm.bridge.exec(proxy, 'file', 'DirectoryEntry_copyTo', proxy.params);

    return true;
};

DirectoryEntry.prototype.remove = function(successCallback, errorCallback)
{
    // Check for required parameters
    if (typeof successCallback === "undefined")
    {
        wm.error("Parameter successCallback is undefined");
        return false;
    }
    if (typeof errorCallback === "undefined")
    {
        wm.error("Parameter errorCallback is undefined");
        return false;
    }

    // Check parameter types and default unsupplied ones
    if (successCallback !== null && typeof successCallback != 'function')
    {
        wm.error("Parameter successCallback must be a function or null");
        return false;
    }
    
    
    if (errorCallback !== null && typeof errorCallback != 'function')
    {
        wm.error("Parameter errorCallback must be a function or null");
        return false;
    }
    
    

    var proxy = new DirectoryEntry.Proxy(this, {'successCallback':successCallback, 'errorCallback':errorCallback});
    proxy.method = 'remove';
    proxy.params = {self:this};

    if (typeof proxy._remove == 'function')
        proxy._remove();
    wm.bridge.exec(proxy, 'file', 'DirectoryEntry_remove', proxy.params);

    return true;
};

DirectoryEntry.prototype.getParent = function(directoryEntrySuccessCallback, errorCallback)
{
    // Check for required parameters
    if (typeof directoryEntrySuccessCallback === "undefined")
    {
        wm.error("Parameter directoryEntrySuccessCallback is undefined");
        return false;
    }
    if (typeof errorCallback === "undefined")
    {
        wm.error("Parameter errorCallback is undefined");
        return false;
    }

    // Check parameter types and default unsupplied ones
    if (directoryEntrySuccessCallback !== null && typeof directoryEntrySuccessCallback != 'function')
    {
        wm.error("Parameter directoryEntrySuccessCallback must be a function or null");
        return false;
    }
    
    
    if (errorCallback !== null && typeof errorCallback != 'function')
    {
        wm.error("Parameter errorCallback must be a function or null");
        return false;
    }
    
    

    var proxy = new DirectoryEntry.Proxy(this, {'directoryEntrySuccessCallback':directoryEntrySuccessCallback, 'errorCallback':errorCallback});
    proxy.method = 'getParent';
    proxy.params = {self:this};

    if (typeof proxy._getParent == 'function')
        proxy._getParent();
    wm.bridge.exec(proxy, 'file', 'DirectoryEntry_getParent', proxy.params);

    return true;
};

DirectoryEntry.prototype.toURI = function()
{
    // Check for required parameters

    // Check parameter types and default unsupplied ones

    var uri;

    var proxy = new DirectoryEntry.Proxy(this, {});
    proxy.method = 'toURI';
    proxy.params = {uri:uri, self:this};

    if (typeof proxy._toURI == 'function')
        uri = proxy._toURI();
    proxy.params['uri'] = uri;
    
    return "file:///" + this.fullPath;
};

DirectoryEntry.prototype.createReader = function()
{
    // Check for required parameters

    // Check parameter types and default unsupplied ones

    var reader;

    var proxy = new DirectoryEntry.Proxy(this, {});
    proxy.method = 'createReader';
    proxy.params = {reader:reader, self:this};

    if (typeof proxy._createReader == 'function')
        reader = proxy._createReader();
    proxy.params['reader'] = reader;
    
    return new DirectoryReader(this.fullPath);
};

DirectoryEntry.prototype.getDirectory = function(path, options, directoryEntrySuccessCallback, errorCallback)
{
    // Check for required parameters
    if (typeof path === "undefined")
    {
        wm.error("Parameter path is undefined");
        return false;
    }
    if (typeof options === "undefined")
    {
        wm.error("Parameter options is undefined");
        return false;
    }
    if (typeof directoryEntrySuccessCallback === "undefined")
    {
        wm.error("Parameter directoryEntrySuccessCallback is undefined");
        return false;
    }
    if (typeof errorCallback === "undefined")
    {
        wm.error("Parameter errorCallback is undefined");
        return false;
    }

    // Check parameter types and default unsupplied ones
    if (path !== null && typeof path != 'string')
    {
        wm.error("Parameter path must be a string or null");
        return false;
    }
    
    
    if (options !== null && !(options instanceof Flags))
    {
        // Check that the object contains valid parameters for the class
        if (!Flags.validateParams(options))
        {
            wm.error("Parameter options must be an instance of Flags or an Object containinig valid parameters or be null");
            return false;
        }
    }
    
    
    if (directoryEntrySuccessCallback !== null && typeof directoryEntrySuccessCallback != 'function')
    {
        wm.error("Parameter directoryEntrySuccessCallback must be a function or null");
        return false;
    }
    
    
    if (errorCallback !== null && typeof errorCallback != 'function')
    {
        wm.error("Parameter errorCallback must be a function or null");
        return false;
    }
    
    

    var proxy = new DirectoryEntry.Proxy(this, {'directoryEntrySuccessCallback':directoryEntrySuccessCallback, 'errorCallback':errorCallback});
    proxy.method = 'getDirectory';
    proxy.params = {self:this, path:path, options:options};

    if (typeof proxy._getDirectory == 'function')
        proxy._getDirectory();
    wm.bridge.exec(proxy, 'file', 'DirectoryEntry_getDirectory', proxy.params);

    return true;
};

DirectoryEntry.prototype.getFile = function(path, options, fileEntrySuccessCallback, errorCallback)
{
    // Check for required parameters
    if (typeof path === "undefined")
    {
        wm.error("Parameter path is undefined");
        return false;
    }
    if (typeof options === "undefined")
    {
        wm.error("Parameter options is undefined");
        return false;
    }
    if (typeof fileEntrySuccessCallback === "undefined")
    {
        wm.error("Parameter fileEntrySuccessCallback is undefined");
        return false;
    }
    if (typeof errorCallback === "undefined")
    {
        wm.error("Parameter errorCallback is undefined");
        return false;
    }

    // Check parameter types and default unsupplied ones
    if (path !== null && typeof path != 'string')
    {
        wm.error("Parameter path must be a string or null");
        return false;
    }
    
    
    if (options !== null && !(options instanceof Flags))
    {
        // Check that the object contains valid parameters for the class
        if (!Flags.validateParams(options))
        {
            wm.error("Parameter options must be an instance of Flags or an Object containinig valid parameters or be null");
            return false;
        }
    }
    
    
    if (fileEntrySuccessCallback !== null && typeof fileEntrySuccessCallback != 'function')
    {
        wm.error("Parameter fileEntrySuccessCallback must be a function or null");
        return false;
    }
    
    
    if (errorCallback !== null && typeof errorCallback != 'function')
    {
        wm.error("Parameter errorCallback must be a function or null");
        return false;
    }
    
    

    var proxy = new DirectoryEntry.Proxy(this, {'fileEntrySuccessCallback':fileEntrySuccessCallback, 'errorCallback':errorCallback});
    proxy.method = 'getFile';
    proxy.params = {self:this, path:path, options:options};

    if (typeof proxy._getFile == 'function')
        proxy._getFile();
    wm.bridge.exec(proxy, 'file', 'DirectoryEntry_getFile', proxy.params);

    return true;
};

DirectoryEntry.prototype.removeRecursively = function(successCallback, errorCallback)
{
    // Check for required parameters
    if (typeof successCallback === "undefined")
    {
        wm.error("Parameter successCallback is undefined");
        return false;
    }
    if (typeof errorCallback === "undefined")
    {
        wm.error("Parameter errorCallback is undefined");
        return false;
    }

    // Check parameter types and default unsupplied ones
    if (successCallback !== null && typeof successCallback != 'function')
    {
        wm.error("Parameter successCallback must be a function or null");
        return false;
    }
    
    
    if (errorCallback !== null && typeof errorCallback != 'function')
    {
        wm.error("Parameter errorCallback must be a function or null");
        return false;
    }
    
    

    var proxy = new DirectoryEntry.Proxy(this, {'successCallback':successCallback, 'errorCallback':errorCallback});
    proxy.method = 'removeRecursively';
    proxy.params = {self:this};

    if (typeof proxy._removeRecursively == 'function')
        proxy._removeRecursively();
    wm.bridge.exec(proxy, 'file', 'DirectoryEntry_removeRecursively', proxy.params);

    return true;
};



var DirectoryReader = function(fullPath)
{
    this.fullPath = (typeof(fullPath) == "undefined") ? "" : fullPath;

};

DirectoryReader.Proxy = function(obj, cbx)
{
    DirectoryReader.Proxy._super.constructor.apply(this, [obj, cbx]);
};

wm.helper.inherit(DirectoryReader.Proxy, wm.Bridge.ProxyBase);



DirectoryReader.validateParams = function(obj)
{
    // Check that the values in obj are valid parameters for this class.
    // Keys which do not match class parameters are ignored
    // Missing keys are added with default values for the type
    if (!wm.helper.validateParam(obj, 'fullPath', 'string', ""))
        return false;

    return true;
};

DirectoryReader.prototype.readEntries = function(entriesSuccessCallback, errorCallback)
{
    // Check for required parameters
    if (typeof entriesSuccessCallback === "undefined")
    {
        wm.error("Parameter entriesSuccessCallback is undefined");
        return false;
    }
    if (typeof errorCallback === "undefined")
    {
        wm.error("Parameter errorCallback is undefined");
        return false;
    }

    // Check parameter types and default unsupplied ones
    if (entriesSuccessCallback !== null && typeof entriesSuccessCallback != 'function')
    {
        wm.error("Parameter entriesSuccessCallback must be a function or null");
        return false;
    }
    
    
    if (errorCallback !== null && typeof errorCallback != 'function')
    {
        wm.error("Parameter errorCallback must be a function or null");
        return false;
    }
    
    

    var proxy = new DirectoryReader.Proxy(this, {'entriesSuccessCallback':entriesSuccessCallback, 'errorCallback':errorCallback});
    proxy.method = 'readEntries';
    proxy.params = {self:this};

    if (typeof proxy._readEntries == 'function')
        proxy._readEntries();
    wm.bridge.exec(proxy, 'file', 'DirectoryReader_readEntries', proxy.params);

    return true;
};



var FileSystem = function(name, root)
{
    this.name = (typeof(name) == "undefined") ? "" : name;
    this.root = (typeof(root) == "undefined") ? null : root;

};

FileSystem.validateParams = function(obj)
{
    // Check that the values in obj are valid parameters for this class.
    // Keys which do not match class parameters are ignored
    // Missing keys are added with default values for the type
    if (!wm.helper.validateParam(obj, 'name', 'string', ""))
        return false;

    if (!wm.helper.validateParam(obj, 'root', 'object', null))
        return false;

    return true;
};



var LocalFileSystem = function()
{

};

LocalFileSystem.Proxy = function(obj, cbx)
{
    LocalFileSystem.Proxy._super.constructor.apply(this, [obj, cbx]);
};

wm.helper.inherit(LocalFileSystem.Proxy, wm.Bridge.ProxyBase);



LocalFileSystem.validateParams = function(obj)
{
    // Check that the values in obj are valid parameters for this class.
    // Keys which do not match class parameters are ignored
    // Missing keys are added with default values for the type
    return true;
};

LocalFileSystem.prototype.requestFileSystem = function(type, size, fileSystemSuccessCallback, errorCallback)
{
    // Check for required parameters
    if (typeof type === "undefined")
    {
        wm.error("Parameter type is undefined");
        return false;
    }
    if (typeof size === "undefined")
    {
        wm.error("Parameter size is undefined");
        return false;
    }
    if (typeof fileSystemSuccessCallback === "undefined")
    {
        wm.error("Parameter fileSystemSuccessCallback is undefined");
        return false;
    }
    if (typeof errorCallback === "undefined")
    {
        wm.error("Parameter errorCallback is undefined");
        return false;
    }

    // Check parameter types and default unsupplied ones
    if (type !== null && typeof type != 'number')
    {
        wm.error("Parameter type must be a number or null");
        return false;
    }
    
    
    if (size !== null && typeof size != 'number')
    {
        wm.error("Parameter size must be a number or null");
        return false;
    }
    
    // check unsigned
    if (size < 0)
    {
        wm.error("Parameter size cannot be negative.");
        return false;
    }

    if (fileSystemSuccessCallback !== null && typeof fileSystemSuccessCallback != 'function')
    {
        wm.error("Parameter fileSystemSuccessCallback must be a function or null");
        return false;
    }
    
    
    if (errorCallback !== null && typeof errorCallback != 'function')
    {
        wm.error("Parameter errorCallback must be a function or null");
        return false;
    }
    
    

    var proxy = new LocalFileSystem.Proxy(this, {'fileSystemSuccessCallback':fileSystemSuccessCallback, 'errorCallback':errorCallback});
    proxy.method = 'requestFileSystem';
    proxy.params = {self:this, type:type, size:size};

    if (typeof proxy._requestFileSystem == 'function')
        proxy._requestFileSystem();
    wm.bridge.exec(proxy, 'file', 'LocalFileSystem_requestFileSystem', proxy.params);

    return true;
};

LocalFileSystem.prototype.resolveLocalFileSystemURL = function(url, entrySuccessCallback, errorCallback)
{
    // Check for required parameters
    if (typeof url === "undefined")
    {
        wm.error("Parameter url is undefined");
        return false;
    }
    if (typeof entrySuccessCallback === "undefined")
    {
        wm.error("Parameter entrySuccessCallback is undefined");
        return false;
    }
    if (typeof errorCallback === "undefined")
    {
        wm.error("Parameter errorCallback is undefined");
        return false;
    }

    // Check parameter types and default unsupplied ones
    if (url !== null && typeof url != 'string')
    {
        wm.error("Parameter url must be a string or null");
        return false;
    }
    
    
    if (entrySuccessCallback !== null && typeof entrySuccessCallback != 'function')
    {
        wm.error("Parameter entrySuccessCallback must be a function or null");
        return false;
    }
    
    
    if (errorCallback !== null && typeof errorCallback != 'function')
    {
        wm.error("Parameter errorCallback must be a function or null");
        return false;
    }
    
    

    var proxy = new LocalFileSystem.Proxy(this, {'entrySuccessCallback':entrySuccessCallback, 'errorCallback':errorCallback});
    proxy.method = 'resolveLocalFileSystemURL';
    proxy.params = {self:this, url:url};

    if (typeof proxy._resolveLocalFileSystemURL == 'function')
        proxy._resolveLocalFileSystemURL();
    wm.bridge.exec(proxy, 'file', 'LocalFileSystem_resolveLocalFileSystemURL', proxy.params);

    return true;
};

// add member methods to window or navigator
var _wm_LocalFileSystem = function() {
    var _tmp_LocalFileSystem = new LocalFileSystem(); 
        if(typeof window.requestFileSystem == "undefined")
            window.requestFileSystem = _tmp_LocalFileSystem.requestFileSystem;
        if(typeof window.resolveLocalFileSystemURL == "undefined")
            window.resolveLocalFileSystemURL = _tmp_LocalFileSystem.resolveLocalFileSystemURL;
};
_wm_LocalFileSystem();


LocalFileSystem.TEMPORARY = 0;
LocalFileSystem.PERSISTENT = 1;

var FileUploadOptions = function()
{
    this.fileKey = 'file';
    this.fileName = 'image.jpg';
    this.mimeType = 'image/jpeg';
    this.params = {};
    this.chunkedMode = true;

};

FileUploadOptions.validateParams = function(obj)
{
    // Check that the values in obj are valid parameters for this class.
    // Keys which do not match class parameters are ignored
    // Missing keys are added with default values for the type
    if (!wm.helper.validateParam(obj, 'fileKey', 'string', ""))
        return false;

    if (!wm.helper.validateParam(obj, 'fileName', 'string', ""))
        return false;

    if (!wm.helper.validateParam(obj, 'mimeType', 'string', ""))
        return false;

    if (!wm.helper.validateParam(obj, 'params', 'object', {}))
        return false;

    if (!wm.helper.validateParam(obj, 'chunkedMode', 'boolean', false))
        return false;

    return true;
};



var FileUploadResult = function()
{
    this.bytesSent = 0;
    this.responseCode = 0;
    this.response = "";

};

FileUploadResult.validateParams = function(obj)
{
    // Check that the values in obj are valid parameters for this class.
    // Keys which do not match class parameters are ignored
    // Missing keys are added with default values for the type
    if (!wm.helper.validateParam(obj, 'bytesSent', 'number', 0))
        return false;

    if (!wm.helper.validateParam(obj, 'responseCode', 'number', 0))
        return false;

    if (!wm.helper.validateParam(obj, 'response', 'string', ""))
        return false;

    return true;
};



var FileTransfer = function()
{

};

FileTransfer.Proxy = function(obj, cbx)
{
    FileTransfer.Proxy._super.constructor.apply(this, [obj, cbx]);
};

wm.helper.inherit(FileTransfer.Proxy, wm.Bridge.ProxyBase);



FileTransfer.validateParams = function(obj)
{
    // Check that the values in obj are valid parameters for this class.
    // Keys which do not match class parameters are ignored
    // Missing keys are added with default values for the type
    return true;
};

FileTransfer.prototype.upload = function(filePath, server, successCallback, errorCallback, options)
{
    // Check for required parameters
    if (typeof filePath === "undefined")
    {
        wm.error("Parameter filePath is undefined");
        return false;
    }
    if (typeof server === "undefined")
    {
        wm.error("Parameter server is undefined");
        return false;
    }
    if (typeof successCallback === "undefined")
    {
        wm.error("Parameter successCallback is undefined");
        return false;
    }
    if (typeof errorCallback === "undefined")
    {
        wm.error("Parameter errorCallback is undefined");
        return false;
    }
    if (typeof options === "undefined")
    {
        wm.error("Parameter options is undefined");
        return false;
    }

    // Check parameter types and default unsupplied ones
    if (filePath !== null && typeof filePath != 'string')
    {
        wm.error("Parameter filePath must be a string or null");
        return false;
    }
    
    
    if (server !== null && typeof server != 'string')
    {
        wm.error("Parameter server must be a string or null");
        return false;
    }
    
    
    if (successCallback !== null && typeof successCallback != 'function')
    {
        wm.error("Parameter successCallback must be a function or null");
        return false;
    }
    
    
    if (errorCallback !== null && typeof errorCallback != 'function')
    {
        wm.error("Parameter errorCallback must be a function or null");
        return false;
    }
    
    
    if (options !== null && !(options instanceof FileUploadOptions))
    {
        // Check that the object contains valid parameters for the class
        if (!FileUploadOptions.validateParams(options))
        {
            wm.error("Parameter options must be an instance of FileUploadOptions or an Object containinig valid parameters or be null");
            return false;
        }
    }
    
    

    var proxy = new FileTransfer.Proxy(this, {'successCallback':successCallback, 'errorCallback':errorCallback});
    proxy.method = 'upload';
    proxy.params = {self:this, filePath:filePath, server:server, options:options};

    if (typeof proxy._upload == 'function')
        proxy._upload();
    wm.bridge.exec(proxy, 'file', 'FileTransfer_upload', proxy.params);

    return true;
};



var Flags = function(create, exclusive)
{
    this.create = (typeof(create) == "undefined") ? false : create;
    this.exclusive = (typeof(exclusive) == "undefined") ? false : exclusive;

};

Flags.validateParams = function(obj)
{
    // Check that the values in obj are valid parameters for this class.
    // Keys which do not match class parameters are ignored
    // Missing keys are added with default values for the type
    if (!wm.helper.validateParam(obj, 'create', 'boolean', false))
        return false;

    if (!wm.helper.validateParam(obj, 'exclusive', 'boolean', false))
        return false;

    return true;
};



var Metadata = function()
{
    this.modificationTime = new Date();

};

Metadata.validateParams = function(obj)
{
    // Check that the values in obj are valid parameters for this class.
    // Keys which do not match class parameters are ignored
    // Missing keys are added with default values for the type
    if (!wm.helper.validateParam(obj, 'modificationTime', 'object', null))
        return false;

    return true;
};



var FileTransferError = function()
{
    this.code = 0;

};

FileTransferError.validateParams = function(obj)
{
    // Check that the values in obj are valid parameters for this class.
    // Keys which do not match class parameters are ignored
    // Missing keys are added with default values for the type
    if (!wm.helper.validateParam(obj, 'code', 'number', 0))
        return false;

    return true;
};



FileTransferError.FILE_NOT_FOUND_ERR = 1;
FileTransferError.INVALID_URL_ERR = 2;
FileTransferError.CONNECTION_ERR = 3;

var FileEvent = function(type, target)
{
    this.type = (typeof(type) == "undefined") ? "" : type;
    this.target = (typeof(target) == "undefined") ? null : target;

};

FileEvent.validateParams = function(obj)
{
    // Check that the values in obj are valid parameters for this class.
    // Keys which do not match class parameters are ignored
    // Missing keys are added with default values for the type
    if (!wm.helper.validateParam(obj, 'type', 'string', ""))
        return false;

    if (!wm.helper.validateParam(obj, 'target', 'object', null))
        return false;

    return true;
};



var ProgressEvent = function(type, target, loaded, total)
{
    this.type = (typeof(type) == "undefined") ? "" : type;
    this.target = (typeof(target) == "undefined") ? null : target;
    this.loaded = (typeof(loaded) == "undefined") ? 0 : loaded;
    this.total = (typeof(total) == "undefined") ? 0 : total;

};

ProgressEvent.validateParams = function(obj)
{
    // Check that the values in obj are valid parameters for this class.
    // Keys which do not match class parameters are ignored
    // Missing keys are added with default values for the type
    if (!wm.helper.validateParam(obj, 'type', 'string', ""))
        return false;

    if (!wm.helper.validateParam(obj, 'target', 'object', null))
        return false;

    if (!wm.helper.validateParam(obj, 'loaded', 'number', 0))
        return false;

    if (!wm.helper.validateParam(obj, 'total', 'number', 0))
        return false;

    return true;
};







LocalFileSystem.RAW = 2;

File.EntryReviver = function(obj)
{
    var entry;
    if (obj.entry.isFile)
        entry = new FileEntry();
    else
        entry = new DirectoryEntry();
    
    entry.isFile = obj.entry.isFile;
    entry.isDirectory = obj.entry.isDirectory;
    entry.name = obj.entry.name;
    entry.fullPath = obj.entry.fullPath;
        
    return entry;
};

//
// LocalFileSystem
LocalFileSystem.Proxy.prototype._requestFileSystem = function()
{
    // avoid json circular reference
    this.params.self = null;
};

LocalFileSystem.Proxy.prototype._resolveLocalFileSystemURL = function()
{
    // avoid json circular reference
    this.params.self = null;
};


LocalFileSystem.Proxy.prototype.fileSystemSuccessCallback = function(obj)
{
    var filesystem = new FileSystem();
    filesystem.name = obj.filesystem.name;
    filesystem.root = new DirectoryEntry();
    filesystem.root.isFile = obj.filesystem.root.isFile;
    filesystem.root.isDirectory = obj.filesystem.root.isDirectory;
    filesystem.root.name = obj.filesystem.root.name;
    filesystem.root.fullPath = obj.filesystem.root.fullPath;
    
    this.userCallback('fileSystemSuccessCallback')(filesystem);
};

LocalFileSystem.Proxy.prototype.entrySuccessCallback = function(obj)
{
    var entry = File.EntryReviver(obj);
    
    this.userCallback('entrySuccessCallback')(entry);
};

LocalFileSystem.Proxy.prototype.errorCallback = function(obj)
{
    var error = new FileError();
    error.code = obj.error.code;
    
    this.userCallback('errorCallback')(error);
};

//
// FileTransfer
FileTransfer.Proxy.prototype._upload = function()
{
    var filePath = this.params.filePath;
    if (filePath.indexOf("://") >= 0)
        return;
        
    if (filePath.indexOf('/android_asset/')===0)
    {
        filePath = "rom://" + filePath.substr('/android_asset/'.length);
    }
    else
    {   
        if (filePath[0] == '/')
            filePath = "raw://" + filePath;
        else 
        {
            if ((filePath.length > 1) && (filePath[1] == ':')) 
                filePath = "raw://" + filePath;
            else
                filePath = "ram://" + filePath;
        }
    }
    
    this.params.filePath = filePath;    
};

FileTransfer.Proxy.prototype.uploadSuccessCallback = function(obj)
{
    var result = new FileUploadResult();
    result.bytesSent = obj.result.bytesSent;
    result.responseCode = obj.result.responseCode;
    result.response = obj.result.response;

    this.userCallback('successCallback')(result);
};

FileTransfer.Proxy.prototype.transferErrorCallback = function(obj)
{
    var error = new FileTransferError();
    error.code = obj.error.code;
    
    this.userCallback('errorCallback')(error);
};

//
// FileEntry
FileEntry.Proxy.prototype.metadataSuccessCallback = function(obj)
{
    var metadata = new Metadata();
    metadata.modificationTime = new Date(obj.metadata.modificationTime);    
    this.userCallback('metadataSuccessCallback')(metadata);
};

FileEntry.Proxy.prototype.entrySuccessCallback = function(obj)
{    
    // moveTo/copyTo    
    var entry = File.EntryReviver(obj);    
    this.userCallback('entrySuccessCallback')(entry);
};

FileEntry.Proxy.prototype.successCallback = function(obj)
{
    this.userCallback('successCallback')();
};

FileEntry.Proxy.prototype.directoryEntrySuccessCallback = function(obj)
{
    var directory = File.EntryReviver({entry: obj.directory});
    this.userCallback('directoryEntrySuccessCallback')(directory);
};

FileEntry.Proxy.prototype.fileSuccessCallback = function(obj)
{
    var file = new File(obj.file.name, obj.file.fullPath, obj.file.type, new Date(obj.file.lastModifiedDate), obj.file.size);    
    this.userCallback('fileSuccessCallback')(file);
};

FileEntry.Proxy.prototype.errorCallback = function(obj)
{
    var error = new FileError(obj.error.code);
    this.userCallback('errorCallback')(error);
};

FileEntry.Proxy.prototype._createWriter = function()
{    
    var me = this.obj;
    var successCallback = this.userCallback('fileWriterSuccessCallback');
    var errorCallback = this.userCallback('errorCallback');
    
    me.file(
        function(filePointer) 
        {
            var writer = new FileWriter(filePointer);
    
            if (writer.fileName === null || writer.fileName === "") {
                errorCallback(new FileError(FileError.INVALID_STATE_ERR));
            }
                
            successCallback(writer);            
        }, 
        errorCallback);
};

//
// DirectoryEntry
DirectoryEntry.Proxy.prototype.metadataSuccessCallback = function(obj)
{
    var metadata = new Metadata();
    metadata.modificationTime = new Date(obj.metadata.modificationTime);
    
    this.userCallback('metadataSuccessCallback')(metadata);
};

DirectoryEntry.Proxy.prototype.entrySuccessCallback = function(obj)
{
    // moveTo/copyTo    
    var entry = File.EntryReviver(obj);
    this.userCallback('entrySuccessCallback')(entry);
};

DirectoryEntry.Proxy.prototype.successCallback = function(obj)
{
    // removeRecursively/remove
    this.userCallback('successCallback')();
};

DirectoryEntry.Proxy.prototype.directoryEntrySuccessCallback = function(obj)
{
    // getDirectory/getParent
    var directory = File.EntryReviver({entry: obj.directory});
    this.userCallback('directoryEntrySuccessCallback')(directory);    
};

DirectoryEntry.Proxy.prototype.fileEntrySuccessCallback = function(obj)
{
    var file = File.EntryReviver({entry: obj.file});
    this.userCallback('fileEntrySuccessCallback')(file);    
};

DirectoryEntry.Proxy.prototype.errorCallback = function(obj)
{
    var error = new FileError(obj.error.code);
    this.userCallback('errorCallback')(error);
};

//
// FileReader
FileReader.Proxy.prototype._abort = function()
{
    var me = this.obj;
   
    var evt;   
    me.readyState = FileReader.DONE;
    me.result = null;

    // set error
    me.error = new FileError(FileError.ABORT_ERR);
    
    this.userClassCallback('onerror')(new FileEvent("error", me));
    this.userClassCallback('onabort')(new FileEvent("abort", me));
    this.userClassCallback('onloadend')(new FileEvent("loadend", me));    
};
   
FileReader.Proxy.prototype._readAsDataURL = function()
{
    var me = this.obj;
    this.loaddone = false;
    this.callbacktype = wm.MULTICALL;

    var evt;
    
    if (me.readyState == FileReader.LOADING)
    {
        wm.throwError(new FileError(FileError.INVALID_STATE_ERR));
        return;
    }
    
    // 1. Set LOADING state
    me.readyState = FileReader.LOADING;
    me.result = "";
    
    // 2. Call loadstart
    this.userClassCallback('onloadstart')(new FileEvent("loadstart", me));
};

FileReader.Proxy.prototype.readAsDataURL_readCallback = function(obj)
{
    var me = this.obj;
    
    var data = obj.data;
    var loaded = obj.loaded;
    var total = obj.total;    
    var evt;
    
    if (me.readyState === FileReader.DONE)
        return;
  
    // Add on next part of blob
    if (!this.loaddone)
        me.result = data;   
    else
        me.result += data;          
               
    if (typeof me.onprogress === "function") 
    {        
        evt = new ProgressEvent();
        evt.loaded = loaded;
        evt.total = total;
        evt.target = me;                
        evt.type = "progress";
        me.onprogress(evt);
    }
    
    // Call onload 
    if (!this.loaddone)
    {               
        this.loaddone = true;
        this.userClassCallback('onload')(new FileEvent("load", me));
    }

    // Call onloadend callback
    if (loaded == total)
    {
        me.readyState = FileReader.DONE;
        this.userClassCallback('onloadend')(new FileEvent("loadend", me));
        
        // this.obj = null; // GC
        wm.bridge.deleteCallback(this.callbackid);
    }
};

FileReader.Proxy.prototype.readAsDataURL_errorCallback = function(obj)
{
    var me = this.obj;    
    var evt;
    
    if (me.readyState === FileReader.DONE) 
        return;

    // Store error
    me.error = new FileError(obj.error.code);

    this.userClassCallback('onerror')(new FileEvent("error", me));

    // DONE state
    me.readyState = FileReader.DONE;        
    
    this.userClassCallback('onloadend')(new FileEvent("loadend", me));

    // this.obj = null; // GC
    wm.bridge.deleteCallback(this.callbackid);
};

FileReader.Proxy.prototype._readAsText = function()
{   
    var me = this.obj;
    
    this.callbacktype = wm.MULTICALL; 
    this.loaddone = false; 
    
    var evt;
    
    // todo: var enc = encoding ? encoding : "UTF-8";
    if (me.readyState == FileReader.LOADING)
    {
        wm.throwError(new FileError(FileError.INVALID_STATE_ERR));
        return;
    }
    
    me.readyState = FileReader.LOADING;
    me.result = "";
    
    this.userClassCallback('onloadstart')(new FileEvent("loadstart", me));
};

FileReader.Proxy.prototype.readAsText_readCallback = function(obj)
{
    var me = this.obj;
    
    var data = obj.data;
    var loaded = obj.loaded;
    var total = obj.total;
    var evt;
        
    if (me.readyState === FileReader.DONE) 
        return;
    
    // Add on next part of blob
    if (!this.loaddone)
        me.result = data;   
    else
        me.result += data;   

    // progress event
    this.userClassCallback('onprogress')(new ProgressEvent("progress", me, loaded, total));
    
    // Call onload 
    if (!this.loaddone)
    {               
        this.loaddone = true;
        this.userClassCallback('onload')(new FileEvent("load", me));
    }

    // Call onloadend callback
    if (loaded == total)
    {
        me.readyState = FileReader.DONE;
        
        this.userClassCallback('onloadend')(new FileEvent("loadend", me));
        
        // this.obj = null; // GC
        wm.bridge.deleteCallback(this.callbackid);
    }    
};

FileReader.Proxy.prototype.readAsText_errorCallback = function(obj)
{
    var me = this.obj;
    var evt;
    
    if (me.readyState === FileReader.DONE) 
        return;

    // Store error
    me.error = new FileError(obj.error.code);

    this.userClassCallback('onerror')(new FileEvent("error", me));

    me.readyState = FileReader.DONE;
    
    this.userClassCallback('onloadend')(new FileEvent("loadend", me));

    // this.obj = null; // GC
    wm.bridge.deleteCallback(this.callbackid);
};

FileReader.Proxy.prototype.readCallback = function(obj)
{
    // readAsDataURL/readAsText
    if (this.method == 'readAsDataURL')
        this.readAsDataURL_readCallback(obj);
    else if (this.method == 'readAsText')
        this.readAsText_readCallback(obj);
    else
        wm.bridge.internalError("FileReader.Proxy.readCallback: unknown method - " + this.method);        
};

FileReader.Proxy.prototype.errorCallback = function(obj)
{
    // readAsDataURL/readAsText
    if (this.method == 'readAsDataURL')
        this.readAsDataURL_readCallback(obj);
    else if (this.method == 'readAsText')
        this.readAsText_readCallback(obj);
    else
        wm.bridge.internalError("FileReader.Proxy.errorCallback: unknown method - " + this.method);
};

//
// FileWriter
FileWriter.Proxy.prototype._write = function()
{    
    // this.callbacktype = wm.MULTICALL;    
    var me = this.obj;
    var evt;
    
    // Are we already writing?
    if (me.readyState === FileWriter.WRITING) 
    {    
        wm.throwError(new FileError(FileError.INVALID_STATE_ERR));
        return;
    }
    
    me.readyState = FileWriter.WRITING;
    
    this.userClassCallback('onwritestart')(new FileEvent("writestart", me));
};


FileWriter.Proxy.prototype.write_writeCallback = function(obj)
{
    var me = this.obj;
    var evt;
    
    if (me.readyState === FileWriter.DONE)
        return;

    me.position = obj.position;
    me.length = obj.length;

    this.userClassCallback('onprogress')(new ProgressEvent("progress", me, me.position, me.length));
    
    // DONE state
    me.readyState = FileWriter.DONE;

    this.userClassCallback('onwrite')(new FileEvent("write", me));
    this.userClassCallback('onwriteend')(new FileEvent("writeend", me));
        
    // this.obj = null; // GC
};

FileWriter.Proxy.prototype.write_errorCallback = function(obj)
{
    var me = this.obj;
    var evt;

    if (me.readyState === FileWriter.DONE) 
        return;

    // Store error
    me.error = new FileError(obj.error.code);

    this.userClassCallback('onerror')(new FileEvent("error", me));

    // DONE state
    me.readyState = FileWriter.DONE;

    this.userClassCallback('onwriteend')(new FileEvent("writeend", me));
    
    // this.obj = null; // GC
};

FileWriter.Proxy.prototype._seek = function()
{
    var position = this.params.position;
    var me = this.obj;
    
    // Are we already writing?
    if (me.readyState === FileWriter.WRITING) 
    {    
        wm.throwError(new FileError(FileError.INVALID_STATE_ERR));
        return;
    }

    if (typeof(position) != "number") 
    {
        wm.error("Error: seek position should be a number");
        return;
    }
    
    if (position < 0) 
    {
        me.position = Math.max(position + me.length, 0);
    }   
    else if (position > me.length)  // position > file size:
    {
        me.position = me.length;
    } 
    else  // set position if within bounds
    {
        me.position = position;
    }
};

FileWriter.Proxy.prototype._truncate = function()
{
    var size = this.params.size;
    var me = this.obj;
    
    var evt;
    
    // Are we already writing?
    if (me.readyState === FileWriter.WRITING) 
    {    
        wm.throwError(new FileError(FileError.INVALID_STATE_ERR));
        return;
    }

    // Set WRITING state
    me.readyState = FileWriter.WRITING;
    
    this.userClassCallback('onwritestart')(new FileEvent("writestart", me));
};


FileWriter.Proxy.prototype.truncate_writeCallback = function(obj)
{
    var me = this.obj;

    var position = obj.position;
    var length = obj.length;    
    var evt;
    
    if (me.readyState === FileWriter.DONE)
        return;

    // Update the length and position
    me.length = length;
    me.position = position;

    // Set DONE state (w3c sets this first before write + writeend)
    me.readyState = FileWriter.DONE;
    
    this.userClassCallback('onwrite')(new FileEvent("write", me));
    this.userClassCallback('onwriteend')(new FileEvent("writeend", me));

    // this.obj = null; // GC
};

FileWriter.Proxy.prototype.truncate_errorCallback = function(obj)
{
    var me = this.obj;
    var evt;
    
    if (me.readyState === FileWriter.DONE) 
        return;

    // Save error
    me.error = new FileError(obj.error.code);

    this.userClassCallback('onerror')(new FileEvent("error", me));

    // DONE state
    me.readyState = FileWriter.DONE;

    this.userClassCallback('onwriteend')(new FileEvent("writeend", me));
        
    // this.obj = null; // GC
};

FileWriter.Proxy.prototype.writeCallback = function(obj)
{
    // write/truncate
    if (this.method == 'write')
        this.write_writeCallback(obj);
    else if (this.method == 'truncate')
        this.truncate_writeCallback(obj);
    else
        wm.bridge.internalError("FileWriter.Proxy.writeCallback: unknown method - " + this.method);        
};

FileWriter.Proxy.prototype.errorCallback = function(obj)
{
    // write/truncate
    if (this.method == 'write')
        this.write_errorCallback(obj);
    else if (this.method == 'truncate')
        this.truncate_errorCallback(obj);
    else
        wm.bridge.internalError("FileWriter.Proxy.errorCallback: unknown method - " + this.method);        
};

FileWriter.Proxy.prototype._abort = function()
{
    var evt;
    var me = this.obj;
    
    if (me.readyState === FileWriter.DONE || me.readyState === FileWriter.INIT) 
    {
        wm.throwError(new FileError(FileError.INVALID_STATE_ERR));
        return;
    }
    
    me.error = new FileError(FileError.ABORT_ERR);    

    this.userClassCallback('onerror')(new FileEvent("error", me));
    this.userClassCallback('onabort')(new FileEvent("abort", me));
    
    me.readyState = FileWriter.DONE;

    this.userClassCallback('onwriteend')(new FileEvent("writeend", me));
};

//
// DirectoryReader
DirectoryReader.Proxy.prototype.entriesSuccessCallback = function(obj)
{
    var entries = [];
    var entry;
    for (var i=0; i<obj.entries.length; i++)
    {
        entry = File.EntryReviver({ entry: obj.entries[i] });
        entries[i] = entry;        
    }
    
    this.userCallback('entriesSuccessCallback')(entries); 
};

DirectoryReader.Proxy.prototype.errorCallback = function(obj)
{
    var error = new FileError(obj.error.code);
    this.userCallback('errorCallback')(error);
};

window.resolveLocalFileSystemURI = window.resolveLocalFileSystemURL;
FileEntry.prototype.toURL = FileEntry.prototype.toURI;
DirectoryEntry.prototype.toURL = DirectoryEntry.prototype.toURI;
//
// geolocation API
//

var Geolocation = function() {};
Geolocation.Proxy = function(obj, cbx)
{
    Geolocation.Proxy._super.constructor.apply(this, [obj, cbx]);
};

wm.helper.inherit(Geolocation.Proxy, wm.Bridge.ProxyBase);


var GeolocationOptions = function()
{
    this.frequency = 10000;
    this.enableHighAccuracy = false;
    this.timeout = false;
    this.maximumAge = 0;

};

GeolocationOptions.validateParams = function(obj)
{
    // Check that the values in obj are valid parameters for this class.
    // Keys which do not match class parameters are ignored
    // Missing keys are added with default values for the type
    if (!wm.helper.validateParam(obj, 'frequency', 'number', 0))
        return false;

    if (!wm.helper.validateParam(obj, 'enableHighAccuracy', 'boolean', false))
        return false;

    if (!wm.helper.validateParam(obj, 'timeout', 'number', 0))
        return false;

    if (!wm.helper.validateParam(obj, 'maximumAge', 'number', 0))
        return false;

    return true;
};



var Coordinates = function(latitude, longitude, altitude, accuracy, altitudeAccuracy, heading, speed)
{
    this.latitude = (typeof(latitude) == "undefined") ? 0 : latitude;
    this.longitude = (typeof(longitude) == "undefined") ? 0 : longitude;
    this.altitude = (typeof(altitude) == "undefined") ? 0 : altitude;
    this.accuracy = (typeof(accuracy) == "undefined") ? 0 : accuracy;
    this.altitudeAccuracy = (typeof(altitudeAccuracy) == "undefined") ? 0 : altitudeAccuracy;
    this.heading = (typeof(heading) == "undefined") ? 0 : heading;
    this.speed = (typeof(speed) == "undefined") ? 0 : speed;

};

Coordinates.validateParams = function(obj)
{
    // Check that the values in obj are valid parameters for this class.
    // Keys which do not match class parameters are ignored
    // Missing keys are added with default values for the type
    if (!wm.helper.validateParam(obj, 'latitude', 'number', 0))
        return false;

    if (!wm.helper.validateParam(obj, 'longitude', 'number', 0))
        return false;

    if (!wm.helper.validateParam(obj, 'altitude', 'number', 0))
        return false;

    if (!wm.helper.validateParam(obj, 'accuracy', 'number', 0))
        return false;

    if (!wm.helper.validateParam(obj, 'altitudeAccuracy', 'number', 0))
        return false;

    if (!wm.helper.validateParam(obj, 'heading', 'number', 0))
        return false;

    if (!wm.helper.validateParam(obj, 'speed', 'number', 0))
        return false;

    return true;
};



var PositionError = function()
{
    this.code = 0;
    this.message = "";

};

PositionError.validateParams = function(obj)
{
    // Check that the values in obj are valid parameters for this class.
    // Keys which do not match class parameters are ignored
    // Missing keys are added with default values for the type
    if (!wm.helper.validateParam(obj, 'code', 'number', 0))
        return false;

    if (!wm.helper.validateParam(obj, 'message', 'string', ""))
        return false;

    return true;
};



PositionError.UNKNOWN_ERROR = 0;
PositionError.PERMISSION_DENIED = 1;
PositionError.POSITION_UNAVAILABLE = 2;
PositionError.TIMEOUT = 3;

var Position = function()
{
    this.coords = null;
    this.timestamp = new Date();

};

Position.validateParams = function(obj)
{
    // Check that the values in obj are valid parameters for this class.
    // Keys which do not match class parameters are ignored
    // Missing keys are added with default values for the type
    if (!wm.helper.validateParam(obj, 'coords', 'object', null))
        return false;

    if (!wm.helper.validateParam(obj, 'timestamp', 'object', null))
        return false;

    return true;
};





if (typeof navigator.geolocation === 'undefined' || (wm.platform.length !== 0 && "OSX".indexOf(wm.platform) !== -1)) {
    navigator.geolocation = new Geolocation();
navigator.geolocation.getCurrentPosition = function(geolocationSuccess, geolocationError, geolocationOptions)
{
    // Check for required parameters
    if (typeof geolocationSuccess === "undefined")
    {
        wm.error("Parameter geolocationSuccess is undefined");
        return false;
    }

    // Check parameter types and default unsupplied ones
    if (geolocationSuccess !== null && typeof geolocationSuccess != 'function')
    {
        wm.error("Parameter geolocationSuccess must be a function or null");
        return false;
    }
    
    
    if (typeof geolocationError === "undefined")
    {
        geolocationError = null;
    }
    else if (geolocationError !== null && typeof geolocationError != 'function')
    {
        wm.error("Parameter geolocationError must be a function or null");
        return false;
    }
    
    
    if (typeof geolocationOptions === "undefined")
    {
        geolocationOptions = new GeolocationOptions();
    }
    else if (geolocationOptions !== null && !(geolocationOptions instanceof GeolocationOptions))
    {
        // Check that the object contains valid parameters for the class
        if (!GeolocationOptions.validateParams(geolocationOptions))
        {
            wm.error("Parameter geolocationOptions must be an instance of GeolocationOptions or an Object containinig valid parameters or be null");
            return false;
        }
    }
    
    

    var proxy = new Geolocation.Proxy(this, {'geolocationSuccess':geolocationSuccess, 'geolocationError':geolocationError});
    proxy.method = 'getCurrentPosition';
    proxy.params = {geolocationOptions:geolocationOptions};

    if (typeof proxy._getCurrentPosition == 'function')
        proxy._getCurrentPosition();
    wm.bridge.exec(proxy, 'geolocation', 'getCurrentPosition', proxy.params);

    return true;
};

navigator.geolocation.watchPosition = function(geolocationSuccess, geolocationError, geolocationOptions)
{
    // Check for required parameters
    if (typeof geolocationSuccess === "undefined")
    {
        wm.error("Parameter geolocationSuccess is undefined");
        return false;
    }

    // Check parameter types and default unsupplied ones
    if (geolocationSuccess !== null && typeof geolocationSuccess != 'function')
    {
        wm.error("Parameter geolocationSuccess must be a function or null");
        return false;
    }
    
    
    if (typeof geolocationError === "undefined")
    {
        geolocationError = null;
    }
    else if (geolocationError !== null && typeof geolocationError != 'function')
    {
        wm.error("Parameter geolocationError must be a function or null");
        return false;
    }
    
    
    if (typeof geolocationOptions === "undefined")
    {
        geolocationOptions = new GeolocationOptions();
    }
    else if (geolocationOptions !== null && !(geolocationOptions instanceof GeolocationOptions))
    {
        // Check that the object contains valid parameters for the class
        if (!GeolocationOptions.validateParams(geolocationOptions))
        {
            wm.error("Parameter geolocationOptions must be an instance of GeolocationOptions or an Object containinig valid parameters or be null");
            return false;
        }
    }
    
    

    var watchID;

    var proxy = new Geolocation.Proxy(this, {'geolocationSuccess':geolocationSuccess, 'geolocationError':geolocationError});
    proxy.method = 'watchPosition';
    proxy.params = {watchID:watchID, geolocationOptions:geolocationOptions};

    if (typeof proxy._watchPosition == 'function')
        watchID = proxy._watchPosition();
    proxy.params['watchID'] = watchID;
    wm.bridge.exec(proxy, 'geolocation', 'watchPosition', proxy.params);

    return watchID;
};

navigator.geolocation.clearWatch = function(watchID)
{
    // Check for required parameters
    if (typeof watchID === "undefined")
    {
        wm.error("Parameter watchID is undefined");
        return false;
    }

    // Check parameter types and default unsupplied ones
    if (watchID !== null && typeof watchID != 'number')
    {
        wm.error("Parameter watchID must be a number or null");
        return false;
    }
    
    // check unsigned
    if (watchID < 0)
    {
        wm.error("Parameter watchID cannot be negative.");
        return false;
    }


    var proxy = new Geolocation.Proxy(this, {});
    proxy.method = 'clearWatch';
    proxy.params = {watchID:watchID};

    if (typeof proxy._clearWatch == 'function')
        proxy._clearWatch();
    wm.bridge.exec(null, 'geolocation', 'clearWatch', proxy.params);

    return true;
};

}


       
Geolocation.Proxy.prototype.geolocationSuccess = function(obj)
{
    var pos = new Position();
    pos.coords = new Coordinates(obj.position.coords.latitude, 
        obj.position.coords.longitude,
        obj.position.coords.altitude,
        obj.position.coords.accuracy,
        obj.position.coords.altitudeAccuracy,
        obj.position.coords.heading,
        obj.position.coords.speed);    
    pos.timestamp = new Date(obj.position.timestamp);
    
    this.userCallback('geolocationSuccess')(pos);
};

Geolocation.Proxy.prototype.geolocationError = function(obj)
{
    var err = new PositionError();
    err.code = obj.error.code;
    err.message = obj.error.message;
    
    this.userCallback('geolocationError')(err);
};

Geolocation.Proxy.prototype._getCurrentPosition = function()
{    
};

Geolocation.Proxy.prototype._watchPosition = function()
{
    this.callbacktype = wm.MULTICALL;
    return wm.bridge.createID();
};

Geolocation.Proxy.prototype._clearWatch = function()
{
};
//
// media API
//

var MediaError = function()
{
    this.code = 0;
    this.message = "";

};

MediaError.validateParams = function(obj)
{
    // Check that the values in obj are valid parameters for this class.
    // Keys which do not match class parameters are ignored
    // Missing keys are added with default values for the type
    if (!wm.helper.validateParam(obj, 'code', 'number', 0))
        return false;

    if (!wm.helper.validateParam(obj, 'message', 'string', ""))
        return false;

    return true;
};



MediaError.MEDIA_ERR_NONE_ACTIVE = 0;
MediaError.MEDIA_ERR_ABORTED = 1;
MediaError.MEDIA_ERR_NETWORK = 2;
MediaError.MEDIA_ERR_DECODE = 3;
MediaError.MEDIA_ERR_NONE_SUPPORTED = 4;

var Media = function(src, successCallback, errorCallback, statusCallback)
{
    this.src = (typeof(src) == "undefined") ? "" : src;
    this._src = "";
    this.successCallback = (typeof(successCallback) == "undefined") ? null : successCallback;
    this.errorCallback = (typeof(errorCallback) == "undefined") ? null : errorCallback;
    this.statusCallback = (typeof(statusCallback) == "undefined") ? null : statusCallback;
    this.id = 0;
    this._duration = 0;
    this._position = 0;
    this._status = 0;

    Media._construct(this);
};

Media.Proxy = function(obj, cbx)
{
    Media.Proxy._super.constructor.apply(this, [obj, cbx]);
};

wm.helper.inherit(Media.Proxy, wm.Bridge.ProxyBase);



Media.validateParams = function(obj)
{
    // Check that the values in obj are valid parameters for this class.
    // Keys which do not match class parameters are ignored
    // Missing keys are added with default values for the type
    if (!wm.helper.validateParam(obj, 'src', 'string', ""))
        return false;

    if (!wm.helper.validateParam(obj, '_src', 'string', ""))
        return false;

    if (!wm.helper.validateParam(obj, 'successCallback', 'function', null))
        return false;

    if (!wm.helper.validateParam(obj, 'errorCallback', 'function', null))
        return false;

    if (!wm.helper.validateParam(obj, 'statusCallback', 'function', null))
        return false;

    if (!wm.helper.validateParam(obj, 'id', 'number', 0))
        return false;

    if (!wm.helper.validateParam(obj, '_duration', 'number', 0))
        return false;

    if (!wm.helper.validateParam(obj, '_position', 'number', 0))
        return false;

    if (!wm.helper.validateParam(obj, '_status', 'number', 0))
        return false;

    return true;
};

Media.prototype.getCurrentPosition = function(mediaSuccess, mediaError)
{
    // Check for required parameters
    if (typeof mediaSuccess === "undefined")
    {
        wm.error("Parameter mediaSuccess is undefined");
        return false;
    }

    // Check parameter types and default unsupplied ones
    if (mediaSuccess !== null && typeof mediaSuccess != 'function')
    {
        wm.error("Parameter mediaSuccess must be a function or null");
        return false;
    }
    
    
    if (typeof mediaError === "undefined")
    {
        mediaError = null;
    }
    else if (mediaError !== null && typeof mediaError != 'function')
    {
        wm.error("Parameter mediaError must be a function or null");
        return false;
    }
    
    

    var proxy = new Media.Proxy(this, {'mediaSuccess':mediaSuccess, 'mediaError':mediaError});
    proxy.method = 'getCurrentPosition';
    proxy.params = {self:this};

    if (typeof proxy._getCurrentPosition == 'function')
        proxy._getCurrentPosition();
    wm.bridge.exec(proxy, 'media', 'Media_getCurrentPosition', proxy.params);

    return true;
};

Media.prototype.getDuration = function()
{
    // Check for required parameters

    // Check parameter types and default unsupplied ones

    var duration;

    var proxy = new Media.Proxy(this, {});
    proxy.method = 'getDuration';
    proxy.params = {self:this};

    if (typeof proxy._getDuration == 'function')
        duration = proxy._getDuration();
    proxy.params['duration'] = duration;
    
    return duration;
};

Media.prototype.play = function()
{
    // Check for required parameters

    // Check parameter types and default unsupplied ones

    var proxy = new Media.Proxy(this, {});
    proxy.method = 'play';
    proxy.params = {self:this};

    if (typeof proxy._play == 'function')
        proxy._play();
    wm.bridge.exec(proxy, 'media', 'Media_play', proxy.params);

    return true;
};

Media.prototype.pause = function()
{
    // Check for required parameters

    // Check parameter types and default unsupplied ones

    var proxy = new Media.Proxy(this, {});
    proxy.method = 'pause';
    proxy.params = {self:this};

    if (typeof proxy._pause == 'function')
        proxy._pause();
    wm.bridge.exec(proxy, 'media', 'Media_pause', proxy.params);

    return true;
};

Media.prototype.stop = function()
{
    // Check for required parameters

    // Check parameter types and default unsupplied ones

    var proxy = new Media.Proxy(this, {});
    proxy.method = 'stop';
    proxy.params = {self:this};

    if (typeof proxy._stop == 'function')
        proxy._stop();
    wm.bridge.exec(proxy, 'media', 'Media_stop', proxy.params);

    return true;
};

Media.prototype.release = function()
{
    // Check for required parameters

    // Check parameter types and default unsupplied ones

    var proxy = new Media.Proxy(this, {});
    proxy.method = 'release';
    proxy.params = {self:this};

    if (typeof proxy._release == 'function')
        proxy._release();
    wm.bridge.exec(proxy, 'media', 'Media_release', proxy.params);

    return true;
};

Media.prototype.seekTo = function(milliseconds)
{
    // Check for required parameters
    if (typeof milliseconds === "undefined")
    {
        wm.error("Parameter milliseconds is undefined");
        return false;
    }

    // Check parameter types and default unsupplied ones
    if (milliseconds !== null && typeof milliseconds != 'number')
    {
        wm.error("Parameter milliseconds must be a number or null");
        return false;
    }
    
    

    var proxy = new Media.Proxy(this, {});
    proxy.method = 'seekTo';
    proxy.params = {self:this, milliseconds:milliseconds};

    if (typeof proxy._seekTo == 'function')
        proxy._seekTo();
    wm.bridge.exec(proxy, 'media', 'Media_seekTo', proxy.params);

    return true;
};

Media.prototype.startRecord = function()
{
    // Check for required parameters

    // Check parameter types and default unsupplied ones

    var proxy = new Media.Proxy(this, {});
    proxy.method = 'startRecord';
    proxy.params = {self:this};

    if (typeof proxy._startRecord == 'function')
        proxy._startRecord();
    wm.bridge.exec(proxy, 'media', 'Media_startRecord', proxy.params);

    return true;
};

Media.prototype.stopRecord = function()
{
    // Check for required parameters

    // Check parameter types and default unsupplied ones

    var proxy = new Media.Proxy(this, {});
    proxy.method = 'stopRecord';
    proxy.params = {self:this};

    if (typeof proxy._stopRecord == 'function')
        proxy._stopRecord();
    wm.bridge.exec(proxy, 'media', 'Media_stopRecord', proxy.params);

    return true;
};

Media.prototype.setVolume = function(volume)
{
    // Check for required parameters
    if (typeof volume === "undefined")
    {
        wm.error("Parameter volume is undefined");
        return false;
    }

    // Check parameter types and default unsupplied ones
    if (volume !== null && typeof volume != 'number')
    {
        wm.error("Parameter volume must be a number or null");
        return false;
    }
    
    

    var proxy = new Media.Proxy(this, {});
    proxy.method = 'setVolume';
    proxy.params = {self:this, volume:volume};

    if (typeof proxy._setVolume == 'function')
        proxy._setVolume();
    wm.bridge.exec(proxy, 'media', 'Media_setVolume', proxy.params);

    return true;
};



Media.MEDIA_NONE = 0;
Media.MEDIA_STARTING = 1;
Media.MEDIA_RUNNING = 2;
Media.MEDIA_PAUSED = 3;
Media.MEDIA_STOPPED = 4;
Media.MEDIA_STATE = 1;
Media.MEDIA_DURATION = 2;
Media.MEDIA_POSITION = 3;
Media.MEDIA_ERROR = 9;





Media._id = 1234;

Media.MEDIA_MSG = ['None', 'Starting', 'Running', 'Paused', 'Stopped'];

Media._createID = function()
{
    return Media._id++;
};

Media._construct = function(self)
{
    self.id = Media._createID();
    self._duration = -1;
    self._position = -1;
    self._status = Media.MEDIA_NONE;
};

Media.prototype._setStatus = function(status)
{
    this._status = status;
    if (typeof this.statusCallback == "function")
        this.statusCallback(status);    
};

Media.prototype._mapPath = function()
{
    // map from src to _src
    if (this.src !== null)
    {
        if (this.src.indexOf('/android_asset/')===0)
        {
            this._src = "rom://" + this.src.substr('/android_asset/'.length);
        }
        else if (this.src.indexOf('http://')===0)
        {
            this._src = this.src;
        }
        else
        {   
            if (this.src[0] == '/')
                this._src = "raw://" + this.src;
            else 
            {
                if ((this.src.length > 1) && (this.src[1] == ':')) 
                    this._src = "raw://" + this.src;
                else
                    this._src = "ram://" + this.src;
                
            }
        }        
    }
    
};

// Media.Proxy preprocess
Media.Proxy.prototype._getCurrentPosition = function()
{
    this.obj._mapPath();
};

Media.Proxy.prototype._getDuration = function()
{
    return this.obj._duration;
};

Media.Proxy.prototype._play = function()
{
    this.callbacktype = wm.MULTICALL;
    this.obj._mapPath();
    // todo check: should we send this here?
    if (this.obj._status != Media.MEDIA_PAUSED && this.obj._status != Media.MEDIA_STOPPED)
    {
        this.obj._setStatus(Media.MEDIA_STARTING);
    }
};

Media.Proxy.prototype._pause = function()
{    
    this.obj._mapPath();
};

Media.Proxy.prototype._seekTo = function()
{   
    // milliseconds
    this.obj._mapPath();
};

Media.Proxy.prototype._startRecord = function()
{
    this.obj._mapPath();
};

// Media.Proxy callbacks
Media.Proxy.prototype._mediaPlaySuccess = function(obj)
{    
    var media = this.obj;
    var isDone = obj.isDone;
    var duration = obj.duration;
    
    if (isDone)
    {
        this.userClassCallback('successCallback')();
        media._setStatus(Media.MEDIA_STOPPED);
        media._position = -1;
        
        wm.bridge.deleteCallback(this.callbackid);
    }
    else
    {
        media._duration = duration;
        media._setStatus(Media.MEDIA_RUNNING);
    }
};

Media.Proxy.prototype.mediaPosSuccessCallback = function(obj)
{
    var media = this.obj;
    var position = obj.position;
    media._position = position;
    this.userCallback('mediaSuccess')(position);
};

Media.Proxy.prototype._mediaSuccess = function(obj)
{
    var media = this.obj;
    
    switch (this.method)
    {
    case '':
    case 'stop':
    case 'setVolume':
        break;
    case 'pause':
        media._setStatus(Media.MEDIA_PAUSED);
        break;
    case 'seekTo':
        if (this.params['milliseconds'] < 0 || this.params['milliseconds'] > media._duration)
            media._position = 0;
        else
            media._position = this.params['milliseconds'];
        break;
    case 'startRecord':
        media._setStatus(Media.MEDIA_RUNNING);
        break;
    case 'stopRecord':
        this.userClassCallback('successCallback')();
        media._setStatus(Media.MEDIA_STOPPED);
        media._position = -1;
        break;
    case 'release':
        media._position = media._duration = -1;
        break;
    }    
};

Media.Proxy.prototype.mediaErrorCallback = function(obj)
{    
    var error = new MediaError();
    error.code = obj.error.code;
    error.message = obj.error.message;
 
    switch (this.method)
    {
    case 'release':
    case 'setVolume':
    case '':
        break;
    case 'getCurrentPosition':
        this.userCallback('mediaError')(error);
        break;
    default: // play/stop/pause/seekTo/startRecord/stopRecord        
        this.userClassCallback('errorCallback')(error);
        break;
    }    
};
//
// notification API
//

var Notification = function() {};
Notification.Proxy = function(obj, cbx)
{
    Notification.Proxy._super.constructor.apply(this, [obj, cbx]);
};

wm.helper.inherit(Notification.Proxy, wm.Bridge.ProxyBase);




if (typeof navigator.notification === 'undefined') {
    navigator.notification = new Notification();
navigator.notification.alert = function(message, alertSuccess, title, buttonName)
{
    // Check for required parameters
    if (typeof message === "undefined")
    {
        wm.error("Parameter message is undefined");
        return false;
    }
    if (typeof title === "undefined")
    {
        title = 'Alert';
    }
    if (typeof buttonName === "undefined")
    {
        buttonName = 'OK';
    }

    // Check parameter types and default unsupplied ones
    if (message !== null && typeof message != 'string')
    {
        wm.error("Parameter message must be a string or null");
        return false;
    }
    
    
    if (typeof alertSuccess === "undefined")
    {
        alertSuccess = null;
    }
    else if (alertSuccess !== null && typeof alertSuccess != 'function')
    {
        wm.error("Parameter alertSuccess must be a function or null");
        return false;
    }
    
    
    if (typeof title === "undefined")
    {
        title = 'Alert';
    }
    else if (title !== null && typeof title != 'string')
    {
        wm.error("Parameter title must be a string or null");
        return false;
    }
    
    
    if (typeof buttonName === "undefined")
    {
        buttonName = 'OK';
    }
    else if (buttonName !== null && typeof buttonName != 'string')
    {
        wm.error("Parameter buttonName must be a string or null");
        return false;
    }
    
    

    var proxy = new Notification.Proxy(this, {'alertSuccess':alertSuccess});
    proxy.method = 'alert';
    proxy.params = {message:message, title:title, buttonName:buttonName};

    if (typeof proxy._alert == 'function')
        proxy._alert();
    wm.bridge.exec(proxy, 'notification', 'alert', proxy.params);

    return true;
};

navigator.notification.confirm = function(message, confirmSuccess, title, buttonLabels)
{
    // Check for required parameters
    if (typeof message === "undefined")
    {
        wm.error("Parameter message is undefined");
        return false;
    }
    if (typeof confirmSuccess === "undefined")
    {
        wm.error("Parameter confirmSuccess is undefined");
        return false;
    }
    if (typeof title === "undefined")
    {
        title = 'Confirm';
    }
    if (typeof buttonLabels === "undefined")
    {
        buttonLabels = 'OK,Cancel';
    }

    // Check parameter types and default unsupplied ones
    if (message !== null && typeof message != 'string')
    {
        wm.error("Parameter message must be a string or null");
        return false;
    }
    
    
    if (confirmSuccess !== null && typeof confirmSuccess != 'function')
    {
        wm.error("Parameter confirmSuccess must be a function or null");
        return false;
    }
    
    
    if (typeof title === "undefined")
    {
        title = 'Confirm';
    }
    else if (title !== null && typeof title != 'string')
    {
        wm.error("Parameter title must be a string or null");
        return false;
    }
    
    
    if (typeof buttonLabels === "undefined")
    {
        buttonLabels = 'OK,Cancel';
    }
    else if (buttonLabels !== null && typeof buttonLabels != 'string')
    {
        wm.error("Parameter buttonLabels must be a string or null");
        return false;
    }
    
    

    var proxy = new Notification.Proxy(this, {'confirmSuccess':confirmSuccess});
    proxy.method = 'confirm';
    proxy.params = {message:message, title:title, buttonLabels:buttonLabels};

    if (typeof proxy._confirm == 'function')
        proxy._confirm();
    wm.bridge.exec(proxy, 'notification', 'confirm', proxy.params);

    return true;
};

navigator.notification.beep = function(times)
{
    // Check for required parameters
    if (typeof times === "undefined")
    {
        wm.error("Parameter times is undefined");
        return false;
    }

    // Check parameter types and default unsupplied ones
    if (times !== null && typeof times != 'number')
    {
        wm.error("Parameter times must be a number or null");
        return false;
    }
    
    

    var proxy = new Notification.Proxy(this, {});
    proxy.method = 'beep';
    proxy.params = {times:times};

    if (typeof proxy._beep == 'function')
        proxy._beep();
    wm.bridge.exec(null, 'notification', 'beep', proxy.params);

    return true;
};

navigator.notification.vibrate = function(time)
{
    // Check for required parameters
    if (typeof time === "undefined")
    {
        wm.error("Parameter time is undefined");
        return false;
    }

    // Check parameter types and default unsupplied ones
    if (time !== null && typeof time != 'number')
    {
        wm.error("Parameter time must be a number or null");
        return false;
    }
    
    

    var proxy = new Notification.Proxy(this, {});
    proxy.method = 'vibrate';
    proxy.params = {time:time};

    if (typeof proxy._vibrate == 'function')
        proxy._vibrate();
    wm.bridge.exec(null, 'notification', 'vibrate', proxy.params);

    return true;
};

}



// Notification callback proxy
Notification.Proxy.prototype.alertSuccess = function(obj)
{
    this.userCallback('alertSuccess')();
};

Notification.Proxy.prototype.confirmSuccess = function(obj)
{
    this.userCallback('confirmSuccess')(obj.button);    
};
