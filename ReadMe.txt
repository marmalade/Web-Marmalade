This is a package of the source of web marmalade and provides an example of extending it to add a new module.
Currently adding new modules is a mechanism to create a bridge between your Web Marmalade code and Marmalade C++ code.

To get started:
Copy the contents into your Marmalade/6.0/web directory, a couple of files will be overwritten.

These are the folders in the source tree
	
wmApplication - This adds some source files needed to compile the application
wmModules - This is the source for the "Web Marmalade" modules
wmClipboard - This is a copy of wmApplication with an added extra module (it adds the simple s3eClipboard API)

In order to add new native calls the wmApplication module is modified and re-built. The wmClipboard is a modified 
version of wmApplication with the additional module for calling the clipboard API.

To compile and run, double click wmClipboard/wmClipboard.mkb to open the project in your IDE and build in the same 
way as a standard Marmalade project. See our documentation if you need more information about building Marmalade C++ 
projects in Visual studio or X-Code.

The key files for the implementation of the clipboard API are within the wmClipboard folder:
wmClipboard.cpp and wmClipboard.h - this is the clipboard module itself
wmApplication.cpp - this is the main application code, modified to add in the clipboard module


To add a new module:
1) create a new class derived from CIwWMDispatcherModule<class_name> (see the wmClipboard.h file for an example)

2) Add in functions to the header and source files:
	 void Init();		called when module is registered
	 void OnReady();	called when a page is about to be ready
	 void Shutdown();	called when module is shut down

3) Add the functions that will be called from your Javascript code, the declaration of these functions will be:
	bool <function name>(int callbackID, const Json::Value& parameters); 
callbackID is passed on to any callbacks that need to be called 
parameters is a JSON object with each parameter passed as a member

See wmClipboard.cpp for an example of how to handle these parameters

4) Add your callback functions:
	these need to be passed the callbackID and any other parameters to be called
	the function needs to call SendJavascript with the name of the callback, the callback id and
	a JSON object with any parameters defined on it. See the clipboardSuccess and clipboardError methods
	in the wmClipboard.cpp file

5) In your initialiser you need to set the name of the module in the parent constructor
	and register any functions that will be called from Javascript with RegisterMember. See the 
	initiliser method for the CClipboard class in the wmClipboard.cpp file

6) Create a Javascript file which creates an object that is the Javascript interface to the module,
	this has a proxy object derived from wm.Bridge.ProxyBase that handles the callbacks. You can
	look at the clipboard.js file in the wmClipboard example under data/webassets

7) Create the functions to be called on this object, these do error checking and call wm.bridge.exec
	to call through to the C++. See the clipboard.js file in the wsClipboard project under data/webassets
	for examples

8) On the proxy add the callback proxies to extract the parameters and call the users callback with
	these parameters
	for more information, take a look at the source in the wmClipboard example clipboard.js in data/webassets,
	the proxies are at the bottom of this file

9) In wmApplication.cpp, in Init after the dispatcher has been created, but before SetNativeReady
	has been called, create your module and pass the dispatcher to it. See the example of this in the source file

10) In the Terminate() method in wmApplication.cpp, call the Shutdown() method on the new module then delete it

11) See the index.html file in data/webassets from the wmClipboard example showing how to call the methods from html

Final note
It is possible to remove the iwaudiocapture and iwui_style assets from wmApplication to reduce the overall size of the
binaries. You can simply comment or remove these from the assets section of the mkb project file. Note that this means your application cannot use the Capture.captureAudio call.