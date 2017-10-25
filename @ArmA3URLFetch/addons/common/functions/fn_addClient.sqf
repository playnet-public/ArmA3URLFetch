
/*
	Author(s):
		Vincent Heins (original)

	Description:
		This function is used to add a client to the extension.
		More about the usage here: https://github.com/TheMysteriousVincent/ArmA3URLFetch/wiki/Usage
	
	Example(s):
		(Example 1)
		private _clientID = ["https://httpbin.org/get", "GET", ["hey=there"], true] call a3uf_common_fnc_addClient;
		//_clientID = <int>
*/

params [
	["_url", "", [""]],
	["_method", "", [""]],
	["_params", [], [[]]],
	["_decodeJson", false, [false]]
];

if (_url == "") exitWith { 0; };
if (_method == "") exitWith { 0; };

_params pushBack format["#url=%1", _url];
_params pushBack format["#method=%1", _method];

if (_decodeJson) then
{
	_params pushBack "#jsonToArray=true";
};

private _res = [];
_res = ("arma3urlfetch" callExtension ["ADDCLI", _params]);
(parseNumber (_res select 0));
