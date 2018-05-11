
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
	["_params", [], [[]]],
	["_headers", [], [[]]],
	["_decodeJson", false, [false]]
];

if (_url == "") exitWith { ""; };
if (_method == "" || !(_method in ["GET", "PUT", "POST", "PATCH", "DELETE", "TRACE"])) exitWith { ""; };

private _args = [];

_args append ["#url", _url];

if (_method != "") then {
	_args append ["#method", _method];
};

if (_decodeJson) then {
	_args pushBack "#jsonToArray";
};

if ((count _params) > 0) then {
	_args pushBack "#parameters";
	_args append _params;
};

if ((count _headers) > 0) then {
	_args pushBack "#headers";
	_args append _headers;
};

private _res = [];
_res = ("arma3urlfetch" callExtension ["SENDRQ", _args]);

if ((_res select 1) == 501) exitWith { ""; };

private _rID = (parseNumber (_res select 0));
if (_rID <= 0) exitWith { ""; };

_res = [];
_res = ("arma3urlfetch" callExtension ["GETRQ", [_rID]]);

private _text = (_res select 0);
if ((_res select 1) == 602) then
{
	waitUntil { uiSleep 0.1; (("arma3urlfetch" callExtension ["GETST", [_rID]]) select 1) != 700; };

	while {
		_res = "arma3urlfetch" callExtension ["GETRQ", [_rID]];
		(_res select 0) != "";
	} do {
		_text = _text + (_res select 0);
	};
};

_text;
