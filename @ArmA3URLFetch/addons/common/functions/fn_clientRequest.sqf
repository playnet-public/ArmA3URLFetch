
/*
	Author(s):
		Vincent Heins (original)

	Description:
		This function executes a simple client request.
		More about the usage here: https://github.com/TheMysteriousVincent/ArmA3URLFetch/wiki/Usage
	
	Example(s):
		(Example 1)
		private _res = [<int>] call a3uf_common_fnc_clientRequest;
		//_res = <string>
*/

params [
	["_cid", 0, [0]],
	["_params", [], [[]]]
];

if (_cid <= 0) exitWith { ""; };

_params pushBack format["#clientid=%1", _cid];

private _res = [];
_res = ("arma3urlfetch" callExtension ["SENDRQ", _params]);

if ((_res select 1) == 501) exitWith { ""; };

private _rID = (parseNumber (_res select 0));
if (_rID <= 0) exitWith { ""; };

_res = [];
_res = ("arma3urlfetch" callExtension ["GETRQ", [_rID]]);

if ((_res select 1) == 602) then
{
	waitUntil
	{
		uiSleep 0.1;
		_res = ("arma3urlfetch" callExtension ["GETRQ", [_rID]]);
		((_res select 1) != 602);
	};
};

//if ((_res select 1) == 601) then {};

(_res select 0);
