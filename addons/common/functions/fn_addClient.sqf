
/*
	Author(s):
		Vincent Heins (original)

	Description:
		This function is used to add a client to the extension.
		More about the usage here: https://github.com/TheMysteriousVincent/ArmA3URLFetch/wiki/Usage
	
	Example(s):
		(Example 1)
		private _clientID = ["https://httpbin.org/get", "GET", ["Authorization", "<some-token>"], "{"myPost": "data"}", true] call a3uf_common_fnc_addClient;
		//_clientID = <int>
*/

params [
	["_url", "", [""]],
	["_method", "", [""]],
	["_headers", [], [[]]],
	["_postData", "", [[], ""]], //<- can be array of strings or a string
	["_decodeJson", false, [false]]
];

if (_url == "") exitWith { 0; };

private _args = [];
_args append ["#url", _url];

if (_method != "") then {
	_args append ["#method", _method];
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
	_args append _headers;
};

if (_decodeJson) then {
	_args pushBack "#jsonToArray";
};

private _res = [];
_res = ("arma3urlfetch" callExtension ["ADDCLI", _args]);
(parseNumber (_res select 0));
