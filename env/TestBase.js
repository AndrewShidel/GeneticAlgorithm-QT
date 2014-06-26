

var fs = require('fs');
var data = {}

//Set all of the statistics variables.
data.winners = JSON.parse(fs.readFileSync("wins.json")),
data.lossers = JSON.parse(fs.readFileSync("losses.json")),
data.numWinners = Object.keys(data.winners).length,
data.numLossers = Object.keys(data.lossers).length,
data.lossesWon = 0, 
dara.winsLost=0,
data.win=0,
data.loss=0,
data.difficulty=0,
data.massExtinctions = 0;
data.treeSize = 1;

exports.data = data;
