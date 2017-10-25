
/*
	Author(s):
		Vincent Heins (original)

	Description:
		Same as a3uf_common_fnc_request, but this function uses a callback for the result.
		More about the usage here: https://github.com/TheMysteriousVincent/ArmA3URLFetch/wiki/Usage
	
	Example(s):
		(Example 1)
		private _res = [{
			systemChat str (_this select 0);
		}, "https://httpbin.org/get", "GET", ["test=tester"], false] call a3uf_common_fnc_requestCallback;
		//_res = <string>
*/

params [
	["_code", {}, [{}, ""]],
	["_url", "", [""]],
	["_method", "", [""]],
	["_params", [], [[]]],
	["_decodeJson", false, [false]]
];

private _res = ([_url, _method, _params, _decodeJson] call a3uf_common_fnc_request);

if (_code isEqualType "") then
{
	private _code = (compile _code);
};

[_res] call _code;
