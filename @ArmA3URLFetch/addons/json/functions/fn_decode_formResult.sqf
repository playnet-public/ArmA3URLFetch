
params [
	["_from", -1, [0]],
	["_scope", -1, [0]],
	["_array", [], [[]]],
	["_first", false, [false]]
];

private _result = ([[], nil] call CBA_fnc_hashCreate);

if (_first) then
{
	private _index = -1;
	{
		switch (count _x) do {
			case 2:
			{
				if ((_x select 0) == 1) then
				{
					[_result, (_x select 1), ([_forEachIndex, 1, _resultArray] call a3uf_json_fnc_decode_formResult)] call CBA_fnc_hashSet;
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
	} forEach _resultArray;
}
else
{
	if (_scope <= -1) exitWith { _result = nil; };
	if ((count _array) <= 0) exitWith { _result = nil; };

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
							private _var = ([_forEachIndex, _scope + 1, _array] call a3uf_json_fnc_decode_formResult);
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
};

_result;
