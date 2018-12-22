
/*
	Author(s):
		Muzzleflash (original)
		TheMysteriousVincent (modified)
*/

params ["_codePoints", "_fromI", "_toI"];

private _result = [];
_result resize (_toI - _fromI + 1);

for "_i" from _fromI to (_toI) do {
	_result set [_i - _fromI, _codePoints select _i];
};

toString _result;
