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
var tester="./0Tester.js", org="../org/0/0.c", module="";

//If we are not resuming, and there are command line arguments.
if (process.argv.length > 1 && process.argv[2] != "resume"){
	console.log(process.argv[2]);
	if (process.argv.length>=4){
		org=process.argv[2]
	}
	var path=org.substring(0,org.lastIndexOf("/")+1);
	if (process.argv.length>=3){
		tester=process.argv[3];
	}
}
//Import the test module.
module = require(tester);

//If server was called with the parameter "resume", then resume from pause.json
if (process.argv.length > 2 && process.argv[2] == "resume"){
	resuming = true;
	var mod = JSON.parse(fs.readFileSync("pause.json"));
	module.winners = mod.winners
	module.lossers = mod.lossers
	module.numWinners = mod.numWinners
	module.numLossers = mod.numLossers
	module.lossesWon = mod.lossesWon
	module.winsLost=mod.winsLost
	module.win=mod.win
	module.loss=mod.loss
	module.difficulty=mod.difficulty
	module.massExtinctions = mod.massExtinctions
	stack = mod.stack;
	sibCount = mod.sibCount
}

//Generate some HTML to display statistics.
app.get('/stat/', function(req, res) {
	var html = "<html>"
	html+="<head></head><body><h3>Wins: "+module.win+"    Losses: "+module.loss+"   Population: "+stack.length+"</h3>";
	html+="<br><br><h3>Mass Extinctions: "+module.massExtinctions+"</h3>"
	html+="<h3>Difficulty: "+module.difficulty+"</h3>"
	html+="<br><br><h3>Losses Won: "+module.lossesWon+"</h3><h3>Wins lost: "+module.winsLost+"</h3>"
	html+="</body></html>";
	res.send(html);
});

//Read from standard in for the pause command.
process.stdin.setEncoding('utf8');
process.stdin.on('readable', function() {
  var chunk = process.stdin.read();
  if (chunk !== null) {
  	console.log("something: "+chunk)
    if (chunk == "pause\n") pause();
  }
});

//Saves the state of execution in "pause.json", and exits the program.
function pause(){
	console.log("pausing...");
	module.stack = stack;
	module.sibCount = sibCount;
	fs.writeFileSync("pause.json", JSON.stringify(module));
	process.exit(1);
}

//Main function where things start.
function main(){
	if (resuming){
		looper();
	}else{
		runCommand("gcc -o ../org/0/0 ../org/0/0.c && g++ -o maker maker.cpp",function(){
			looper();
		});
	}
}
main();

//Loops to infinity, here is where the bulk of the work is done.
function looper(){
	var name = stack.slice(-1)[0],
		question = module.getQuestion(),
		newId = nextName(name, false);
	if (newId==null) return fail(name);
	if (newId.length>=250) {reset(); return;}
	ask( name,question, function(res){
		if (res==null || res==undefined || res=="") {fail(); return;};
		res = res.split("|");
		
		if (res[0]==""+module.getAnswer(question)){
			module.win++
			if (module.lossers.getKeyByValue(res[0]) != null) module.lossesWon++;
			//Make a new org, and make it executable.
			command = "./maker ../org/0/ "+newId+" "+name+" "+res[1]+" "+res[2]
				+" && chmod +x ../org/0/"+newId;
			runCommand(command, function(res){
				stack.unshift(newId);
				//setTimeout(function(){
					looper();
				//}, 500);
				//looper();
			});
		}else{
			module.loss++;
			if (module.winners.getKeyByValue(res[0]) != null) module.winsLost++;
			fail();
			
		}
	});
}

//Remove the failed program, and continue with the next item on the stack.
function fail(){
	sibCount=0;
	fs.unlink("../org/0/"+stack.pop(), function (err) {
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
	module.massExtinctions++;
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
		var temp = "../org/0/"+tempStack.pop();
		console.log("ran: "+temp);
		fs.renameSync(temp, "../org/0/"+alphebet[i]);
		resetLoop(tempStack,++i);
	}

	//Delete the remaining files.
	if (tempStack.length>0){
		fs.unlinkSync("../org/0/"+tempStack.pop());
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
	var command = "../org/0/"+askId+" "+askId+" "+question;
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