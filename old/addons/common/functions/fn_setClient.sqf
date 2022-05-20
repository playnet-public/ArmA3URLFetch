
/*
	Author(s):
		Vincent Heins (original)

	Description:
		(Re)sets or modifies the headers of a client.
		More about the usage here: https://github.com/TheMysteriousVincent/ArmA3URLFetch/wiki/Usage
	
	Example(s):
		(Example 1)
		private _bool = [
			<int>,
			[
				"Authorization: 123SafeAuthKey"
			]
		] call a3uf_common_fnc_setClientHeaders;
		//_bool = <bool>
*/

params [
	["_cid", 0, [0]],
	["_url", "", [""]],
	["_method", "", [""]],
	["_headers", [], [[]]],
	["_postData", [], ["", []]],
	["_decodeJson", false, [false]]
];

if (_cid <= 0) exitWith {};
if (_url == "") exitWith {};

private _args = [];

_args append ["#clientid", _cid];
_args append ["#url", _url];

if (_method != "") then {
	_args append ["#method", _method];
};

if (_decodeJson) then {
	_args pushBack "#jsonToArray";
};

if (_postData isEqualType "" && { _postData != "" }) then {
	_args append ["#postData", _postData];
} else {
	if (_postData isEqualType [] && { (count _postData) > 0 }) then {
		_args pushBack "#postData";
		_args append _postData;
	};
};

if ((count _headers) > 0) then {
	_args pushBack "#headers";
	_args append _headers;
};

private _res = ("arma3urlfetch" callExtension [
	"SETCLI",
	_args
]);

if ((_res select 1) == 402) exitWith { false; };
true;
