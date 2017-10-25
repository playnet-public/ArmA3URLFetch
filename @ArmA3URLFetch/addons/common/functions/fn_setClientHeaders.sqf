
/*
	Author(s):
		Vincent Heins (original)

	Description:
		(Re)sets or modifies the headers of a client.
		More about the usage here: https://github.com/TheMysteriousVincent/ArmA3URLFetch/wiki/Usage
	
	Example(s):
		(Example 1)
		private _bool = [
			<int>,
			[
				"Authorization: 123SafeAuthKey"
			]
		] call a3uf_common_fnc_setClientHeaders;
		//_bool = <bool>
*/

params [
	["_cid", 0, [0]],
	["_params", [], [[]]]
];

if (_cid <= 0) exitWith { false; };
if ((count _params) <= 0) exitWith { false; };

_params pushBack format["#clientid=%1", _cid];
private _res = ("arma3urlfetch" callExtension [
	"SETCLIH",
	_params
]);

if ((_res select 1) == 402) exitWith { false; };
true;
