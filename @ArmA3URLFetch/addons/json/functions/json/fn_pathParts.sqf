
/*
	Author(s):
		Muzzleflash (original)
		TheMysteriousVincent (modified)
*/

private _path = toArray _this;
private _parts = [];
private _dot = toArray "." select 0;
private _start = 0;

for "_i" from 0 to (count _path - 1) do
{
	if (_path select _i == _dot) then {
		_parts set [count _parts, [_path, _start, _i - 1] call a3uf_json_fnc_lexExtract];
		_start = _i + 1;
	};
};

//Extract last part
_parts set [count _parts, [_path, _start, count _path - 1] call a3uf_json_fnc_lexExtract];
_parts;
