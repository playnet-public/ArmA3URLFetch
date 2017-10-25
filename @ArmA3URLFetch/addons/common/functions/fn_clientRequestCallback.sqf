
/*
	Author(s):
		Vincent Heins (original)

	Description:
		Same as a3uf_common_fnc_clientRequest, but this function uses a callback for the result.
		More about the usage here: https://github.com/TheMysteriousVincent/ArmA3URLFetch/wiki/Usage
	
	Example(s):
		(Example 1)
		private _res = [{
			systemChat str (_this select 0); //returns value of _res
		}, <int>] call a3uf_common_fnc_clientRequestCallback;
		//_res = nil
*/

params[
	["_code", "", ["", {}]],
	["_cid", 0, [0]],
	["_params", [], [[]]]
];

if (_cid <= 0) exitWith {};

private _res = ([_cid, _params] call a3uf_common_fnc_clientRequest);

if (_code isEqualType "") then
{
	private _code = (compile _code);
};

[_res] call _code;
