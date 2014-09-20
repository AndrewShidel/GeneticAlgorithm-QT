var fs = require('fs');
var base = require('./TestBase.js').data;

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
		question=base.winners[Math.floor(Math.random()*base.numWinners)];
	}else{
		if (Math.random()<=base.difficulty){
			question=base.lossers[Math.floor(Math.random()*base.numLossers)];
		}else{
			question=base.winners[Math.floor(Math.random()*base.numWinners)];
		}
		base.difficulty=stack.length/100;
	}
	return question;
}

exports.getAnswer = getAnswer;
exports.getQuestion = getQuestion;
exports.data = base;
