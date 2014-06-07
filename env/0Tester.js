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