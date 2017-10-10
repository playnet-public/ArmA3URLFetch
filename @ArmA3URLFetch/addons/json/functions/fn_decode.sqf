
params [
	["_jsonString", "", [""]]
];

private _latestSigns = [];
private _currentScope = 0;
private _lCI = -1; //lCI = last comma index
private _resultArray = [];
private _lIChar = ""; //lIChar = last index char
private _sLIChar = ""; //sLIChar = second last index char
private _lSign = ""; //lSign = last sign
private _lSignI = -1; //lSignI = last sign index
private _sLSign = ""; //sLSign = second last sign
private _sLSignI = -1; //sLSignI = second last sign index
private _endingBracket = false;
private _lText = "";
private _sLText = "";

_stringC = (count _jsonString);

for [{_i = 0}, {_i < _stringC}, {_i = _i + 1}] do
{
	private _char = (_jsonString select [_i, 1]);
	_lIChar = "";
	_sLIChar = "";
	_lSign = "";
	_lSignI = -1;
	_sLSign = "";
	_sLSignI = -1;
	
	_lIChar = (_jsonString select [(_i - 1), 1]);
	
	if (_i > 1) then
	{
		_sLIChar = (_jsonString select [(_i - 2), 1]);
	};
	
	if ((count _latestSigns) > 0) then
	{
		_lSignI = (_latestSigns select ((count _latestSigns) - 1));
		_lSign = (_jsonString select [_lSignI, 1]);
	};
	
	if ((count _latestSigns) > 1) then
	{
		_sLSignI = (_latestSigns select ((count _latestSigns) - 2));
		_sLSign = (_jsonString select [_sLSignI, 1]);
	};
	
	switch (_char) do
	{
		case "{";
		case "[":
		{
			if (_lSign != '"') then
			{
				_currentScope = _currentScope + 1;
				
				if ((count _latestSigns) > 0) then
				{
					if (_lSign == "[") then
					{
						_resultArray pushBack [_currentScope, nil];
					}
					else
					{
						_resultArray pushBack [_currentScope, _lText];
					};
				};
				
				_lCI = (_i + 1);
				_latestSigns pushBack _i;
			};
		};
		
		case "}";
		case "]":
		{
			private _isText = false;
			if (_endingBracket) then
			{
				_endingBracket = false;
				_latestSigns deleteAt ((count _latestSigns) - 1);
				_currentScope = _currentScope - 1;
			};
			
			if (_lSign == '"' && _sLSign != ":") then
			{
				_latestSigns deleteRange [((count _latestSigns) - 1), 1];
				_lSignI = (_latestSigns select ((count _latestSigns) - 1));
				_lSign = (_jsonString select [_lSignI, 1]);
				_isText = true;
			};
			
			if (_lSign == ":") then
			{
				_resultArray pushBack [_currentScope, _lText, (_jsonString select [(_lSignI + 1), (_i - _lSignI) - 1])];
				_latestSigns deleteRange [((count _latestSigns) - 2), 2];
				
				_lSignI = (_latestSigns select ((count _latestSigns) - 1));
				_lSign = (_jsonString select [_lSignI, 1]);
			};
			
			if (_lSign in ["[", "{"]) then
			{
				_endingBracket = true;
				
				if (_sLSign == ":") then
				{
					if (_lSign == "[") then
					{
						_resultArray pushBack [_currentScope, nil, (_jsonString select [_lCI, (_i - _lCI)])];
					};
					_latestSigns deleteRange [((count _latestSigns) - 2), 2];
					_lSignI = (_latestSigns select ((count _latestSigns) - 1));
					_lSign = (_jsonString select [_lSignI, 1]);
				}
				else
				{
					if (_lSign != "{" && _isText) then
					{}
					else
					{
						if (_isText) then
						{
							_resultArray pushBack [_currentScope, _sLText, _lText];
						};
					}
				};
				
				if (_lSign == ":") then
				{
					_latestSigns deleteRange [((count _latestSigns) - 2), 2];
				};
			};
		};
		
		case ",":
		{
			switch (_lSign) do
			{
				case '"':
				{
					if (_sLSign != ":") then
					{
						_latestSigns deleteAt ((count _latestSigns) - 1);
						
						switch (_sLSign) do
						{
							case "{":
							{
								if !(_endingBracket) then
								{
									_resultArray pushBack [_currentScope, _sLText, _lText];
								};
							};
							
							case "[":
							{
								if !(_endingBracket) then
								{
									_resultArray pushBack [_currentScope, nil, _lText];
								};
							};
						};
					};
				};
			
				case "{";
				case "[":
				{
					if (_endingBracket) then
					{
						_endingBracket = false;
						_currentScope = _currentScope - 1;
						_latestSigns deleteAt ((count _latestSigns) - 1);
						_lCI = (_i + 1);
					}
					else
					{
						_resultArray pushBack [_currentScope, nil, (_jsonString select [_lCI, (_i - _lCI)])];
						_lCI = (_i + 1);
					};
				};
				
				case ":":
				{
					if !(_endingBracket) then
					{
						_resultArray pushBack [_currentScope, _lText, (_jsonString select [(_lSignI + 1), (_i - _lSignI) - 1])];
						_latestSigns deleteRange [((count _latestSigns) - 2), 2];
					}
					else
					{
						_endingBracket = false;
						_currentScope = _currentScope - 1;
						_resultArray pushBack [_currentScope, _lText, (_jsonString select [(_lSignI + 1), (_i - _lSignI) - 1])];
						_latestSigns deleteAt ((count _latestSigns) - 1);
						_latestSigns deleteAt ((count _latestSigns) - 1);
					};
				};
			};
		};
		
		case '"':
		{
			if (_lSign == '"') then
			{
				if (_lIChar != '\') then
				{
					if (_sLSign == "[") then
					{
						_latestSigns deleteAt ((count _latestSigns) - 1);
					};
					
					if (_sLSign == ":") then
					{
						_latestSigns deleteRange [((count _latestSigns) - 2), 2];
					};
					_sLText = _lText;
					_lText = (_jsonString select [(_lSignI), (_i - _lSignI) + 1]);
				};
			}
			else
			{
				_latestSigns pushBack _i;
			};
		};
		
		case ":":
		{
			if (_lSign == '"' && _sLSign != ":") then
			{
				_latestSigns pushBack _i;
				if (_endingBracket) then
				{
					_endingBracket = false;
						_currentScope = _currentScope - 1;
				};
			};
		};
	};
};

{
	_x set [1, (call compile (_x select 1))];
	if ((count _x) == 3) then
	{
		private _text = toArray (_x select 2);
		private _b1 = (_text find 34);
		reverse _text;
		private _b2 = (_text find 34);
		reverse _text;
		
		if (_b1 == -1 && _b2 == -1) then
		{
			if ((toString _text find "null") != -1) then
			{
				_x set [2, nil];
			}
			else
			{
				_x set [2, (call compile (_x select 2))];
			};
		}
		else
		{
			if (_b1 != -1 && _b2 != -1 && _b1 == _b2) then {
				_x set [2, (_x select 2) select [1, (count _text) - 2]];
			}
			else
			{
				_x = nil;
			};
		};
	}
	else
	{
		if ((count _x) == 2) then {};
	};
} forEach _resultArray;

_resultArray = _resultArray select { (!isNil { _x; }) };

_resultArray = _resultArray apply {
	(if ((count _x) == 3) then
	{
		[((_x select 0) - 1), (_x select 1), (_x select 2)];
	}
	else
	{
		[((_x select 0) - 1), (_x select 1)];
	});
};

private _result = [];
_result = ([0, 0, _resultArray, true] call a3uf_json_fnc_decode_formResult);

/*getNestedStmt = {
	params [
		["_from", -1, [0]],
		["_scope", -1, [0]],
		["_array", [], [[]]]
	];
	private _result = ([[], nil] call CBA_fnc_hashCreate);

	if (_scope <= -1) exitWith { nil; };
	if ((count _array) <= 0) exitWith { nil; };

	private _index = -1;
	{
		private _fin = false;
		if (_from <= _forEachIndex) then
		{
			if (_from != _forEachIndex && ((count _x) == 2) && (_x select 0) <= _scope) then
			{
				_fin = true;
			}
			else
			{
				switch (count _x) do {
					case 2:
					{
						if ((_x select 0) == (_scope + 1)) then
						{
							private _var = ([_forEachIndex, _scope + 1, _array] call getNestedStmt);
							[_result, (_x select 1), _var] call CBA_fnc_hashSet;
						};
					};
					case 3:
					{
						if (isNil { (_x select 1); }) then
						{
							if (_index == -1) then
							{
								_index = 0;
							}
							else
							{
								_index = _index + 1;
							};
							
							[_result, _index, (_x select 2)] call CBA_fnc_hashSet;
						}
						else
						{
							[_result, (_x select 1), (_x select 2)] call CBA_fnc_hashSet;
						};
					};
				};
			};
		};

		if (_fin) exitWith {};
	} forEach _resultArray;

	_result;
};

private _index = -1;
{
	switch (count _x) do {
		case 2:
		{
			if ((_x select 0) == 1) then
			{
				[_result, (_x select 1), ([_forEachIndex, 1, _resultArray] call getNestedStmt)] call CBA_fnc_hashSet;
			};
			_index = -1;
		};
		case 3:
		{
			if ((_x select 0) == 0) then
			{
				if (isNil { (_x select 1); }) then
				{
					if (_index == -1) then
					{
						_index = 0;
					}
					else
					{
						_index = _index + 1;
					};
					
					[_result, _index, (_x select 2)] call CBA_fnc_hashSet;
				}
				else
				{
					[_result, (_x select 1), (_x select 2)] call CBA_fnc_hashSet;
				};
			};
		};
	}
} forEach _resultArray;*/

_result;
