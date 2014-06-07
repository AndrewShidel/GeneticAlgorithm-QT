//Imports.
var fs = require('fs');
var files = ["0"];

var express = require('express'),
    app = express();
app.listen(3000);

//Setup globals.
var winners = JSON.parse(fs.readFileSync("wins.json")),
	lossers = JSON.parse(fs.readFileSync("losses.json")),
	numWinners = Object.keys(winners).length,
	numLossers = Object.keys(lossers).length,
	lossesWon = 0, winsLost=0,
	win=0,
	loss=0,
	difficulty=0,
	massExtinctions = 0;

var alphebet = "0123456789abcdefghijklmnopqrstuvwxyz";
var stack=new Array();
stack.push("0");
var sibCount=0;

app.get('/stat/', function(req, res) {
	var html = "<html>"
	html+="<head></head><body><h3>Wins: "+win+"    Losses: "+loss+"   Population: "+stack.length+"</h3>";
	html+="<br><br><h3>Mass Extinctions: "+massExtinctions+"</h3>"
	html+="<h3>Difficulty: "+difficulty+"</h3>"
	html+="<br><br><h3>Losses Won: "+lossesWon+"</h3><h3>Wins lost: "+winsLost+"</h3>"
	html+="</body></html>";
	res.send(html);
});

//Main function where things start.
function main(){
	runCommand("gcc -o ../org/0/0 ../org/0/0.c",function(){
		looper();
	});
}
main();

function looper(){
	var name = stack.slice(-1)[0],
		question = getQuestion(),
		newId = nextName(name, false);
	if (newId==null) return fail(name);
	if (newId.length>=250) {reset(); return;}
	ask( name,question, function(res){
		if (res==null || res==undefined || res=="") {fail(); return;};
		res = res.split("|");
		
		if (res[0]==""+getAnswer(question)){
			win++
			if (lossers.getKeyByValue(res[0]) != null) lossesWon++;
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
			loss++;
			if (winners.getKeyByValue(res[0]) != null) winsLost++;
			fail();
			
		}
	});
}

function fail(){
	//stack.pop();
	sibCount=0;
	fs.unlink("../org/0/"+stack.pop(), function (err) {
		looper();
	});
}

function nextName(parent, newGen){
	//if (parent=="0")return "0"
	if (sibCount>=alphebet.length-1) return null;
	return parent+alphebet[(++sibCount)];
	if (!newGen) return current.substring(0,current.length-1)+alphebet[(++sibCount)];
	sibCount=0;
	return current+sibCount;
}

function reset(){
	massExtinctions++;
	console.log("Reseting");
	var i=0;
	var tempStack = stack;
	stack=[];
	resetLoop(tempStack, 0);
	looper();
}

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

//Get the next question.
function getQuestion() {
	var question;
	if (stack.length<=3){
		question=winners[Math.floor(Math.random()*numWinners)];
	}else{
		if (Math.random()<=difficulty){
			question=lossers[Math.floor(Math.random()*numLossers)];
		}else{
			question=winners[Math.floor(Math.random()*numWinners)];
		}
		difficulty=stack.length/100;
	}
	return question;
}

//Get the answer to a question.
function getAnswer(i){
	if (i%7==1) return i*7;
	if (i%6==0) return i*6;
	if (i%5==1) return i*5;
	if (i%4==0) return i*4;
	if (i%3==1) i = i*3;
	if (i%2==0) i = i*2;

	return i;
}

function runFile(){
	//child_process.execFile(file, [args], [options], [callback])
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