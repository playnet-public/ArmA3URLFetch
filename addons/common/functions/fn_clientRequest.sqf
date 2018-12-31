
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
	["_cid", 0, [0]]
];

if (_cid <= 0) exitWith { ""; };

private _args = [];
_args append ["#clientid", str _cid];

private _res = [];
_res = ("arma3urlfetch" callExtension ["SENDRQ", _args]);

if ((_res select 1) == 501) exitWith { ""; };

private _rID = (parseNumber (_res select 0));
if (_rID <= 0) exitWith { ""; };

_res = [];
_res = ("arma3urlfetch" callExtension ["GETRQ", [_rID]]);

private _text = "";
if ((_res select 1) == 602) then {
	waitUntil {
		uiSleep 0.1;
		_res = ("arma3urlfetch" callExtension ["GETRQ", [_rID]]);
		_text = _text + (_res select 0);
		((_res select 0) == "" && (_res select 1) == 600);
	};
};

_text;
