
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
	["_decodeJson", false, [false]]
];

_method = (toUpper _method);

if (_url == "") exitWith { ""; };
if (_method == "" || !(_method in ["GET", "PUT", "POST", "PATCH", "DELETE", "TRACE"])) exitWith { ""; };

_params pushBack format["#url=%1", _url];
_params pushBack format["#method=%1", _method];

if (_decodeJson) then
{
	_params pushBack "#jsonToArray=true";
};

private _res = [];
_res = ("arma3urlfetch" callExtension ["SENDRQ", _params]);

if ((_res select 1) == 501) exitWith { ""; };

private _rID = (parseNumber (_res select 0));
if (_rID <= 0) exitWith { ""; };

_res = [];
_res = ("arma3urlfetch" callExtension ["GETRQ", [_rID]]);

private _text = (_res select 0);
if ((_res select 1) == 602) then
{
	waitUntil
	{
		uiSleep 0.1;
		_res = ("arma3urlfetch" callExtension ["GETRQ", [_rID]]);
		_text = _text + (_res select 0);
		((_res select 0) == "" && (_res select 1) == 600);
	};
};

_text;
