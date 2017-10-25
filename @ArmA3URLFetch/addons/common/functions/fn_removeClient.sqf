
/*
	Author(s):
		Vincent Heins (original)

	Description:
		Removes a client.
		More about the usage here: https://github.com/TheMysteriousVincent/ArmA3URLFetch/wiki/Usage
	
	Example(s):
		(Example 1)
		private _res = [<int>] call a3uf_common_fnc_removeClient;
		//_res = <int> //same as the input value - if not, error!
*/

params [
	["_cid", 0, [0]]
];

if (_cid <= 0) exitWith { false; };

private _res = [];
_res = ("arma3urlfetch" callExtension ["REMCLI", [_cid]]);

if ((_res select 1) == 202) exitWith { false; };

(parseNumber (_res select 0));
