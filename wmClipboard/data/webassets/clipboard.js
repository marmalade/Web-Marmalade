//
// clipboard API
//

var Clipboard = function() {};
Clipboard.Proxy = function(obj, cbx)
{
    Clipboard.Proxy._super.constructor.apply(this, [obj, cbx]);
};

wm.helper.inherit(Clipboard.Proxy, wm.Bridge.ProxyBase);

if (typeof navigator.clipboard === 'undefined') 
{

navigator.clipboard = new Clipboard();
    
navigator.clipboard.getText = function(clipboardSuccess, clipboardError)
{
    // Check for required parameters
    if (typeof clipboardSuccess === "undefined")
    {
        wm.error("Parameter clipboardSuccess is undefined");
        return false;
    }

    // Check parameter types and default unsupplied ones
    if (clipboardSuccess !== null && typeof clipboardSuccess != 'function')
    {
        wm.error("Parameter clipboardSuccess must be a function or null");
        return false;
    }
    
    if (typeof clipboardError === "undefined")
    {
        clipboardError = null;
    }
    else if (clipboardError !== null && typeof clipboardError != 'function')
    {
        wm.error("Parameter clipboardError must be a function or null");
        return false;
    }

    var proxy = new Clipboard.Proxy(this, {'clipboardSuccess':clipboardSuccess, 'clipboardError':clipboardError});
    proxy.method = 'getText';
    proxy.params = {};

    if (typeof proxy._getText == 'function')
        proxy._getText();

    wm.bridge.exec(proxy, 'clipboard', 'getText', proxy.params);

    return true;
};

navigator.clipboard.setText = function(text, clipboardSuccess, clipboardError)
{
    // Check for required parameters
    if (typeof text === "undefined")
    {
        wm.error("Parameter text is undefined");
        return false;
    }

    // Check parameter types and default unsupplied ones
    if (text !== null && typeof text != 'string')
    {
        wm.error("Parameter text must be a string or null");
        return false;
    }
    
    if (typeof clipboardSuccess === "undefined")
    {
        clipboardSuccess = null;
    }
    else if (clipboardSuccess !== null && typeof clipboardSuccess != 'function')
    {
        wm.error("Parameter clipboardSuccess must be a function or null");
        return false;
    }
       
    if (typeof clipboardError === "undefined")
    {
        clipboardError = null;
    }
    else if (clipboardError !== null && typeof clipboardError != 'function')
    {
        wm.error("Parameter clipboardError must be a function or null");
        return false;
    }

    var proxy = new Clipboard.Proxy(this, {'clipboardSuccess':clipboardSuccess, 'clipboardError':clipboardError});
    proxy.method = 'setText';
    proxy.params = {text:text};

    if (typeof proxy._setText == 'function')
        proxy._setText();
    
    wm.bridge.exec(proxy, 'clipboard', 'setText', proxy.params);
    
    return true;
};

}

// callback proxies
// callback proxies are used to revive the JSON object and pass data through to the user callbacks
// these proxies are also useful for modifying the data before passing it on to the user
// e.g. to convert a date in millseconds to a proper Date object
Clipboard.Proxy.prototype.clipboardSuccess = function(obj)
{    
    var text = obj.text;
    this.userCallback('clipboardSuccess')(text);
};

Clipboard.Proxy.prototype.clipboardError = function(obj)
{
    this.userCallback('clipboardError')();
};
