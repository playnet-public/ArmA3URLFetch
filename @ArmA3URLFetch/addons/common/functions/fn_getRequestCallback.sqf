
params[
	["_code", "", [{}, ""]],
	["_mimeType", "", [""]],
	["_url"]
	//... params
];

private _args = [_mimeType, _url];
if ((count _this) > 2) then
{
	{
		_args pushBack ([_x] param [0, "", [""]]);
	} forEach (_this select [2, (count _this)]);
};

private _res = (_args call a3uf_common_fnc_getRequest);

if (_code isEqualType "") then
{
	private _code = (compile _code);
};

[_res] call _code;
