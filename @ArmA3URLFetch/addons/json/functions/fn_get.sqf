
/*
	Author(s):
		Muzzleflash (original)
		TheMysteriousVincent (modified)
	
	Description:
		This function is used to select a part from a json ArmA 3 array.
	
	Example(s):
		(Example 1)
		private _json = [["object", [["test", 1], ["test2", 2]]]];
		private _res = [_json, "test"] call a3uf_json_fnc_get;
		//_res = 1;

	Notice:
		"Currently there is no support for selecting arrays." -Muzzleflash
*/

params [
	["_node", [], [[]]],
	["_path", "", [""]]
];

private _node = (_this select 0);
private _pathParts = _path call a3uf_json_fnc_pathParts;

//For each part of the path
{
	private _part = _x;

	//Only objects have keys
	if (_node select 0 != "object") exitWith {
		_node = objNull;
	};

	//Find index of key
	private _index = -1;
	{
		if (_x select 0 == _part) exitWith {
			_index = _forEachIndex;
		};
	} forEach (_node select 1);

	if (_index == -1) exitWith {
		_node = objNull;
	};

	//Select child with the key matching the part
	_node = ((_node select 1) select _index) select 1;
} forEach _pathParts;

//Did client mean to extract array?
if (_node isEqualType [] && { _node select 0 == "array" }) then {
	_node = _node select 1;
};
_node;
