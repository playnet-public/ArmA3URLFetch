
/*
	Author(s):
		Muzzleflash
		TheMysteriousVincent (modified)
*/

private _tokens = _this call a3uf_json_Lex;
//Error?
if (typeName _tokens == typeName "string") exitWith
{
	_tokens
};
private _parseState = [_tokens, 0, ""];
private _result = _parseState call a3uf_json_Parse_Top;

if (_parseState select 2 != "") exitWith
{
	_parseState select 2
};
_result;
