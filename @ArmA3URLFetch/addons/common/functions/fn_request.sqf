
/*
	Author(s):
		Vincent Heins (original)

	Description:
		The default request function for usage without clients.
		More about the usage here: https://github.com/TheMysteriousVincent/ArmA3URLFetch/wiki/Usage
	
	Example(s):
		(Example 1)
		private _res = ["https://httpbin.org/get", "GET", ["test=tester"], false] call a3uf_common_fnc_request;
		//_res = <string>
*/

params [
	["_url", "", [""]],
	["_method", "", [""]],
	["_headers", [], [[]]],
	["_postData", "", ["", []]],
	["_decodeJson", false, [false]]
];

if (_url == "") exitWith { []; };

private _args = [];

_args append ["#url", _url];

if (_method != "") then {
	_args append ["#method", _method];
};

if (_decodeJson) then {
	_args pushBack "#jsonToArray";
};

if (_postData isEqualType "" && { _postData != "" }) then {
	_args append ["#postData", _postData];
} else {
	if (_postData isEqualType [] && { (count _postData) > 0 }) then {
		_args pushBack "#postData";
		_args append _postData;
	};
};

if ((count _headers) > 0) then {
	_args pushBack "#headers";
	_args append _headers;
};

private _res = [];
_res = "arma3urlfetch" callExtension ["SENDRQ", _args];
if ((_res select 1) == 2) exitWith { []; };

private _rID = (parseNumber (_res select 0));
if (_rID <= 0) exitWith { []; };

private _response = "";
private _httpCode = 0;

waitUntil {
	uiSleep 0.05;
	("arma3urlfetch" callExtension ["GETST", [_rID]]) select 1 == 0;
};

while {
	_res = "arma3urlfetch" callExtension ["GETRQ", [_rID]];
	(_res select 1) == 0;
} do {
	_response = _response + (_res select 0);
};

_response = _response + (_res select 0);
_httpCode = _res select 1;

[_response, _httpCode];
