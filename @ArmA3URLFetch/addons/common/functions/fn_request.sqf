
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

private _reqRes = ("arma3urlfetch" callExtension ["RQST", _params]);
private _id = (parseNumber (_reqRes select 0));

if ((_reqRes select 2) != 0) exitWith { ""; };
if (_id <= 0) exitWith { ""; };

private _status = 0;
waitUntil
{
	private _statRes = ("arma3urlfetch" callExtension ["STAT", [(str _id)]]);
	_status = (parseNumber (_statRes select 0));
	
	if ((_statRes select 2) != 0) then
	{
		_status = 4;
	};

	uiSleep 0.1;
	(_status != 0);
};

if (_status != 1) exitWith { ""; };

_result = "";
if (_status == 1) then
{
	private _recvRes = ("arma3urlfetch" callExtension ["RECV", [(str _id)]]);

	if ((_recvRes select 2) == 0) then
	{
		_result = (_recvRes select 0);
	};
};

_result;
