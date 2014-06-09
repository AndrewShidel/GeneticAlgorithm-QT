reset: 
	 find ../org/0/ -perm +100 -type f -delete && if [[ -f pause.json ]];then rm pause.json; fi;
start: reset
	node server.js;
resume: pause.json
	node server.js resume;
