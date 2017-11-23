
cd %~dp0
python "compile.py" "-L" "a3uf.tmp.log" "-N" "a3uf_{name}" "-k" "..\keys\arma3urlfetch.1.0.f3c4e31.biprivatekey" "-b" "-T" "..\build\@ArmA3URLFetch\addons" "-F" "..\@ArmA3URLFetch\addons\*" "-I" "*.p3d;*.pac;*.paa;*.rtm;*.sqf;*.sqs;*.bikb;*.fsm;*.wss;*.ogg;*.wav;*.fxy;*.csv;*.html;*.lip;*.txt;*.wrp;*.bisurf;*.xml;*.rvmat;*.hpp;*.jpg;*.h" "-A" %ADDONBUILDERPATH% "-p" "x\a3uf\addons\{folder}"
