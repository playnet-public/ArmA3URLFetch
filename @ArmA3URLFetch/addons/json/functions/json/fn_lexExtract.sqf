
/*
	Author(s):
		Muzzleflash (original)
		TheMysteriousVincent (modified)
*/

private _codePoints = _this select 0;
private _fromI = _this select 1;
private _toI = _this select 2;

private _result = [];
_result resize (_toI - _fromI + 1);

for "_i" from _fromI to (_toI) do
{
	_result set [_i - _fromI, _codePoints select _i];
};

toString _result;
