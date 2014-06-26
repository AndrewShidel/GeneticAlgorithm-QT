var fs = require('fs'),
	files = ["0"],
	express = require('express'),
    app = express();

app.listen(3000);

var alphebet = "0123456789abcdefghijklmnopqrstuvwxyz";
GLOBAL.stack=new Array();
stack.push("0");
var sibCount=0;
var resuming = false;

//These will be the default command line arguments if none are provided.
var tester="./0Tester.js", org="../org/0/0.c", module="", path="";

//If we are not resuming, and there are command line arguments.
if (process.argv.length > 1 && process.argv[2] != "resume"){
	console.log(process.argv[2]);
	if (process.argv.length>=4){
		org=process.argv[2]
	}
	
	if (process.argv.length>=3){
		tester=process.argv[3];
	}
}
path=org.substring(0,org.lastIndexOf("/")+1);

//Import the test module.data.
module = require(tester);

//If server was called with the parameter "resume", then resume from pause.json
if (process.argv.length > 2 && process.argv[2] == "resume"){
	resuming = true;
	var mod = JSON.parse(fs.readFileSync("pause.json"));
	module.data.winners = mod.winners
	module.data.lossers = mod.lossers
	module.data.numWinners = mod.numWinners
	module.data.numLossers = mod.numLossers
	module.data.lossesWon = mod.lossesWon
	module.data.winsLost=mod.winsLost
	module.data.win=mod.win
	module.data.loss=mod.loss
	module.data.difficulty=mod.difficulty
	module.data.massExtinctions = mod.massExtinctions
	path=mod.path;
	org=mod.org;
	stack = mod.stack;
	sibCount = mod.sibCount
}

//Generate some HTML to display statistics.
app.get('/stat/', function(req, res) {
	var html = "<html>"
	html+="<head></head><body><h3>Wins: "+module.data.win+"    Losses: "+module.data.loss+"   Population: "+stack.length+"</h3>";
	html+="<br><br><h3>Mass Extinctions: "+module.data.massExtinctions+"</h3>"
	html+="<h3>Difficulty: "+module.data.difficulty+"</h3>"
	html+="<br><br><h3>Losses Won: "+module.data.lossesWon+"</h3><h3>Wins lost: "+module.data.winsLost+"</h3>"
	html+="</body></html>";
	res.send(html);
});

//Read from standard in for the pause and kill commands.
process.stdin.setEncoding('utf8');
process.stdin.on('readable', function() {
  var chunk = process.stdin.read();
  if (chunk !== null) {
    if (chunk == "pause\n") pause();
    else if (chunk == "kill\n") kill();
    else console.log("Error: "+chunk.substring(0,chunk.length-1)+" is not a valid command.");
  }
});

//Saves the state of execution in "pause.json", and exits the program.
function pause(){
	console.log("pausing...");
	module.data.stack = stack;
	module.data.sibCount = sibCount;
	module.data.path=path;
	module.data.org=org;
	fs.writeFileSync("pause.json", JSON.stringify(module));
	process.exit(0);
}

//Kills the current enviroment, erasing all program data.
function kill(){
	console.log("Killing the program...");
	runCommand("make reset", function(){
		process.exit(0);
	});
}

//Main function where things start.
function main(){
	if (resuming){
		looper();
	}else{
		var command = "gcc -o "+path+"0 "+org+" && g++ -o maker maker.cpp";
		console.log(command);
		runCommand(command,function(){
			looper();
		});
	}
}
main();

//Loops to infinity, here is where the bulk of the work is done.
function looper(){
	if (stack.length == 0){
		setTimeout(looper, 10);
		return;
	}
	var name = stack.slice(-1)[0],
		question = module.getQuestion(),
		newId = nextName(name, false);
	if (newId==null) return fail(name);
	if (newId.length>=250) {reset(); return;}
	ask( name,question, function(res){
		if (res==null || res==undefined || res=="") {fail(); return;};
		res = res.split("|");
		
		if (res[0]==""+module.getAnswer(question)){
			module.data.win++
			if (module.data.lossers.getKeyByValue(res[0]) != null) module.data.lossesWon++;
			//Make a new org, and make it executable.
			command = "./maker "+path+" "+newId+" "+name+" "+res[1]+" "+res[2]
				+" && chmod +x "+path+newId;
			runCommand(command, function(res){
				stack.unshift(newId);
				//setTimeout(function(){
					
				//}, 500);
				//looper();
			});
			looper();
		}else{
			module.data.loss++;
			if (module.data.winners.getKeyByValue(res[0]) != null) module.data.winsLost++;
			fail();
		}
	});
}

//Remove the failed program, and continue with the next item on the stack.
function fail(){
	sibCount=0;
	fs.unlink(path+stack.pop(), function (err) {
		looper();
	});
}

//Get the next filename based on the parent's name
//parent: The and of the parent's executable.
//newGen: True if this program is the first of a new generation.
function nextName(parent, newGen){
	//if (parent=="0")return "0"
	if (sibCount>=alphebet.length-1) return null;
	return parent+alphebet[(++sibCount)];
	if (!newGen) return current.substring(0,current.length-1)+alphebet[(++sibCount)];
	sibCount=0;
	return current+sibCount;
}

//Causes a mass extinction, leaving nly alphebet.length number of files.
function reset(){
	module.data.massExtinctions++;
	console.log("Reseting");
	var i=0;
	var tempStack = stack;
	stack=[];
	resetLoop(tempStack, 0);
	looper();
}

//Called by reset to loop through the stack and delete files.
function resetLoop(tempStack, i){
	if (tempStack.length>0 && i<alphebet.length){
		stack.unshift(alphebet[i]);
		var temp = path+tempStack.pop();
		console.log("ran: "+temp);
		fs.renameSync(temp, path+alphebet[i]);
		resetLoop(tempStack,++i);
	}

	//Delete the remaining files.
	if (tempStack.length>0){
		fs.unlinkSync(path+tempStack.pop());
		resetLoop(tempStack,++i);
	}

}


//Make a new executable with id.
function make(id){
	runCommand()
}

//Ask a org a question.
function ask(askId, question, callback){
	//Ask the question
	var command = path+askId+" "+askId+" "+question;
	runCommand(command,function(res){
		callback(res);
	});
	
}

Object.prototype.getKeyByValue = function( value ) {
    for( var prop in this ) {
        if( this.hasOwnProperty( prop ) ) {
             if( this[ prop ] === value )
                 return prop;
        }
    }
    return null;
}

//Run a bash command.
function runCommand(com,callback){
	var sys = require('sys')
	var exec = require('child_process').exec;
	var child;

	child = exec(com, function (error, stdout, stderr) {
	  	if (error !== null) {
	    	callback(null);
	  	}else{
	  		callback(stdout);
	  	}
	});
}