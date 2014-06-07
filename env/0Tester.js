var fs = require('fs');

var winners = JSON.parse(fs.readFileSync("wins.json")),
	lossers = JSON.parse(fs.readFileSync("losses.json")),
	numWinners = Object.keys(winners).length,
	numLossers = Object.keys(lossers).length,
	lossesWon = 0, winsLost=0,
	win=0,
	loss=0,
	difficulty=0,
	massExtinctions = 0;

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

exports.getAnswer = getAnswer;
exports.getQuestion = getQuestion;
exports.winners = winners
exports.lossers = lossers
exports.numWinners = numWinners
exports.numLossers = numLossers
exports.lossesWon = lossesWon
exports.winsLost=winsLost
exports.win=win
exports.loss=loss
exports.difficulty=difficulty
exports.massExtinctions = massExtinctions
