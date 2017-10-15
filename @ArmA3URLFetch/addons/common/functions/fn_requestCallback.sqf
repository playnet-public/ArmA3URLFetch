
params[
	["_code", {}, [{}, ""]],
	["_url", "", [""]],
	["_method", "", [""]],
	["_params", [], [[]]],
	["_decodeJson", false, [false]]
];

private _res = ([_url, _method, _params, _decodeJson] call a3uf_fnc_request);

if (_code isEqualType "") then
{
	private _code = (compile _code);
};

[_res, _url, _method] call _code;
