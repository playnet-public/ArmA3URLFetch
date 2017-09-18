
params [
	["_mimeType", "application/json", [""]],
	["_url", "", [""]]
	//params...
];

if (_mimeType == "") exitWith { ""; };
if (_url == "") exitWith { ""; };

private _params = "|";
if ((count _this) > 2) then
{
	_params = "";
	for [{private _i = 2}, {_i < (count _this)}, {_i = _i + 1}] do
	{
		_params = format["|%1%2", _params, (_this param [_i, "", [""]])];
	};
};

private _rKey = ("arma3urlfetch" callExtension format["POST|%1|%2%3", _mimeType, _url, _params]);
if (_rKey == "-1") exitWith { ""; };

private _tKey = "";
waitUntil
{
	_tKey = ("arma3urlfetch" callExtension format["STAT|%1", _rKey]);
	uiSleep 0.1;
	(_tKey == "" ||  _tKey == "1" || _tKey == "2");
};

if (_tKey != "1") exitWith { ""; };
("arma3urlfetch" callExtension format["RECV|%1", _rKey]);
