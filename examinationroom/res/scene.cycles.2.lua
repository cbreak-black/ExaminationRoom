-- Load Libraries
dofile("res/statistics.lua")
dofile("res/questions.lua")

-- Distance to screen: 1 meter
-- Screen height: 0.70 meter
-- Screen width:  1.17 meter
-- Eye position: screen centered
local screenHeight = 0.70;
local viewingDistance = 1.0;
local headPosition = 0.5;
-- Camera Position
local camX, camY, camZ = 0, 0, 25;
-- Camera Direction
local viewX, viewY, viewZ = 0, 0, -1;
-- Camera field of View
local fieldOfView = 18;
-- Camera separation
local camSep = 0.15;
-- Camera - ParalaxPlane distance
local camPP = 25;
-- Texture properties
local maxColor = 8;
local exclusiveColor = 1;
-- Target properties
local numTargets = 2; -- Should only be 1 or 2
local targetWidth = 2;
local targetHeight = 2;
-- Scene Properties
local cyclesPerBlock = 10;
local blocksPerScene = 12;
-- Warnings (Warning: Platform dependent)
local audioCorrect = false;
local voiceCorrect = false;
local audioIncorrect = true;
local voiceIncorrect = false;
local voice = "Vicki"
-- Questionns
local cyclesPerQuestionnaire = 60;
local lang = "de";

-- A perfect cycle visits all three depths/ two positions,
-- and uses every path exactly once
-- Even numbered nodes and odd numbered nodes are grouped
-- Those are label numbers, to find their position, look up their index
-- in the labelToIndex table.
perfectCycles = {
	{4, 3, 2, 1, 2, 5, 2, 3, 4, 1, 6, 1, 4, 5, 6, 3, 6, 5, 4};
	{4, 5, 6, 3, 6, 5, 4, 1, 6, 1, 5, 3, 2, 5, 2, 1, 2, 3, 4}; -- Backwards
	{5, 4, 1, 2, 3, 6, 5, 2, 1, 6, 3, 4, 3, 2, 5, 6, 1, 4, 5};
	{5, 4, 1, 6, 5, 2, 3, 4, 3, 6, 1, 2, 5, 6, 3, 2, 1, 4, 5}; -- Backwards
	{1, 4, 3, 2, 5, 6, 1, 2, 3, 4, 5, 2, 1, 6, 3, 6, 5, 4, 1};
	{1, 4, 5, 6, 3, 6, 1, 2, 5, 4, 3, 2, 1, 6, 5, 2, 3, 4, 1}; -- Backwards
};

-- Position Definition for target
targetX = {-3, 1}; -- Even/Odd axis
targetY = -2.0;
targetZ = { 7.5, 0, -18 };

-- Appearance (Texture Paths)
shapes =
{
"res/square.png";
"res/circle.png";
}
patterns = {
"res/texMarbleBlack.jpg";
"res/texMarbleGrey.jpg";
"res/texMarbleBrown.jpg";
}

-- Input Library
Key = {
[18] = "left";
[19] = "up";
[20] = "right";
[21] = "down";
[32] = "space";
}
Dir = {
["up"] = "concave";
["down"] = "convex";
}

statistics:setViewingProperties(
	viewingDistance,
	screenHeight,
	headPosition);
Scene:log("Screen Height = "..screenHeight);
Scene:log("Viewing Distance = "..viewingDistance);
Scene:log("Head Position = "..headPosition);

-- Scene Construction
Scene:setCameraPos(camX, camY, camZ);
Scene:setCameraDir(viewX, viewY, viewZ);
Scene:setCameraFoV(fieldOfView);
Scene:setCameraSep(camSep);
Scene:setCameraParalaxPlane(camPP);
Scene:log("Camera Position = ("..camX..","..camY..","..camZ..")");
Scene:log("Camera Direction = ("..viewX..","..viewY..","..viewZ..")");
Scene:log("Camera Field of View = "..fieldOfView);
Scene:log("Camera Separation = "..camSep);
Scene:log("Camera Paralax Plane = "..camPP);
Scene:log("Texture Colors = "..maxColor.."(-"..exclusiveColor..")");
Scene:log("Target Properties = "..numTargets.." @ "..targetWidth.."x"..targetHeight);
Scene:log("Scene Properties = "..blocksPerScene.." blocks with "..cyclesPerBlock.." cycles per Block");

rectFloor = Object("Rectangle");
rectFloor:setDirA(6,0,0);
rectFloor:setDirB(0,0,26);
rectFloor:setPosition(-3, -2, -18.5);
rectFloor:setTexCoords(0,0, 0,26, 6,0, 6,26);
rectFloor:setTexture(Texture("Simple", "res/checkerboard.png"));

targets = {};
for i = 1, numTargets do
	local t = Object("Pixelplane");
	table.insert(targets, t);
	t:setSize(targetWidth, targetHeight);
	t:setAutoResize(true);
end

marker = Object("Rectangle");
marker:setWireframe(true);
marker:setColor(1, 1, 0, 1);
marker:setDirA(targetWidth, 0, 0);
marker:setDirB(0, targetHeight, 0);

-- State
cycleNum = 0;
blockNum = 0;
currentBlockType = 1; -- 1 = continuous depth, 2 = no continuous depth
currentCycle = math.random(1, #perfectCycles);
currentSide = math.random(1,  2); -- 1 = left, 2 = right, as defined in targetX
currentTest = 1;
labelToIndex = {
	{ 1, 2, 3}; -- first side (even)
	{ 1, 2, 3}; -- second side (odd)
}
replies = {};
possibleReplies = { "convex", "concave" };
-- Fill the replies so the list is bigger or equal to a cycle
while #replies < #perfectCycles[1] do
	for i, r in ipairs(possibleReplies) do
		table.insert(replies, r);
	end;
end;

positionForTest = function (test)
	return positionForLabel(perfectCycles[currentCycle][test]);
end

positionForLabel = function (label)
	local x, y, z;
	-- The X coordinate only depends on the side the target is on
	x = targetX[currentSide];
	-- The Y coordinate is the same for all targets
	y = targetY;
	-- The Z coordinate depends on the cycle, and the labeling of the nodes
	-- in the cycle
	local side = label%2; -- 0 for even, 1 for odd
	-- Look up the correct table in labelToIndex, with the correct label
	z = targetZ[labelToIndex[side+1][(label+side)/2]];
	return {x, y, z};
end

permuteTable = function (t)
	local n = #t;
	for i = 1, n do
		local j = math.random(i, n);
		t[i], t[j] = t[j], t[i];
	end
end;

-- Prepares the state for the next block
nextBlock = function ()
	currentBlockType = currentBlockType % 2 + 1;
	if currentBlockType == 2 then
		Scene:log("New Block: "..blockNum.." (Continuous depth)");
	else
		Scene:log("New Block: "..blockNum.." (No Continuous depth)");
	end
	blockNum = blockNum + 1;
end

-- Prepares the state for the next cycle
nextCycle = function ()
	-- Permute the label-to-index table
	for i, t in ipairs(labelToIndex) do
		permuteTable(t);
	end
	-- Randomly chose the order of replies
	permuteTable(replies);
	-- Randomly chose which cycle to test
	currentCycle = math.random(1, #perfectCycles);
	Scene:log("New Cycle: "..cycleNum);
	cycleNum = cycleNum + 1;
end

-- Prepares the state for the next target
nextTarget = function ()
	currentSide = currentSide%2 + 1;
	currentTest = currentTest + 1;
	-- Cycle has ended
	if currentTest > #perfectCycles[currentCycle] then
		-- Start a new block if the current one is completed
		-- (and the cycle has ended)
		if cycleNum%cyclesPerBlock == 0 then
			if blockNum == blocksPerScene then
				startQuestions(endTest);
				return;
			end;
			nextBlock();
		end;
		-- Start questioning if the required number of cycles have completed
		if cycleNum%cyclesPerQuestionnaire == 0 then
			startQuestions(continueTest);
		end;
		-- Start a new cycle since the old one is over
		currentTest = 1;
		nextCycle();
	end
	local label = perfectCycles[currentCycle][currentTest];
	local side = label%2; -- 0 for even, 1 for odd
	local index = labelToIndex[side+1][(label+side)/2];
	Scene:log("New Target: Label("..label..") -> Index("..index..")");
	displayTarget();
end

-- Only draw if set to true
testing = false;

displayTarget = function ()
	if not testing then
		return;
	end;
	local pos = positionForTest(currentTest);
	local sep = statistics:paralaxAtPoint(pos[1]+targetWidth/2, pos[2]+targetHeight/2, pos[3]);
	local shape = shapes[math.random(1, #shapes)];

	--	// Uncomment the following for a (rerendered) Stereogram version
--	local texture = Texture("Stereogram",
--		string.format(texbases[texIndexes[testNum]], replies[testNum], "l"),
--		string.format(texbases[texIndexes[testNum]], replies[testNum], "r"));
--	// Uncomment the following for a Random Dot version
	local texture = Texture("RandomDot", shape);
	texture:setMaxColor(maxColor);
	texture:setExclusiveColor(exclusiveColor);
--	// Uncomment the following for a Pattern version
--	permuteTable(patterns); -- Pick two random patterns
--	local texture = Texture("Pattern", shape, patterns[1], patterns[2]);
--	// End Comments
	texture:setStyle(replies[currentTest]); -- Here the concave/convex status is set

	local target;
	if numTargets == 2 then
		target = targets[currentSide];
	else
		target = targets[1];
	end
	target:setTexture(texture);
	target:setPosition(pos[1], pos[2], pos[3]);
	marker:setPosition(pos[1], pos[2]+0.001, pos[3]+0.001);
	Scene:addObject(target);
	Scene:addObject(marker);

	-- Floor
	if currentBlockType == 2 then
		Scene:addObject(rectFloor);
	else
		Scene:removeObject(rectFloor);
	end

	-- Extensive logging of object properties
	local s = string.format("Target Properties: %s/%s (%0.2f, %0.2f, %0.2f), s=%0.4f deg",
		replies[currentTest],shape,
		pos[1]+targetWidth/2, pos[2]+targetHeight/2, pos[3],
		sep);
	Scene:log(s);
end;

parseInput = function (k)
	local d = Key[string.byte(k)];
	if d then
		if Dir[d] then
			d = Dir[d];
			if d == replies[currentTest] then
				Scene:log("Input Correct: "..d);
				if (audioCorrect) then
					os.execute("osascript -e \'beep 1\' &");
				end
				if (voiceCorrect) then
					os.execute("say -v "..voice.." correct &");
				end
			else
				Scene:log("Input Incorrect: "..d);
				if (audioIncorrect) then
					os.execute("osascript -e \'beep 1\' &");
				end
				if (voiceIncorrect) then
					os.execute("say -v "..voice.." incorrect &");
				end
			end;
			nextTarget();
		elseif d == "space" then
			--Scene:log("Input Skipped: "..d);
			--displayNextTarget();
		end
	else
		Scene:log("Input invalid, ignored ("..string.byte(k)..")");
	end
end;

startTest = function ()
	testing = true;
	-- For constant luminance during testing
	Scene:setBackgroundColor(127,127,127,255);
	Scene:setEventListener("keyDown", parseInput);
	Scene:setEventListener("quit", function (k) Scene:log("Exiting..."); end);
	nextBlock();
	nextCycle();
	nextTarget();
end;

continueTest = function ()
	testing = true;
	-- For constant luminance during testing
	Scene:setBackgroundColor(127,127,127,255);
	Scene:setEventListener("keyDown", parseInput);
	-- Restore Targets
	displayTarget();
end;

endTest = function ()
	Scene:log("Scene Completed");
	os.exit(0);
end;

-- Questions
qs = {};
if lang == "de" then
	table.insert(qs, questions:createQuestion("Wie stark ist die Ermuedung ihrer Augen im Moment?",
		"gar nicht", "sehr stark", 6));
	table.insert(qs, questions:createQuestion("Jetzt fuehle ich mich (1)",
		"angespannt", "gelassen", 6));
	table.insert(qs, questions:createQuestion("Jetzt fuehle ich mich (2)",
		"geloest", "beklommen", 6));
	table.insert(qs, questions:createQuestion("Jetzt fuehle ich mich (3)",
		"besorgt", "unbekuemmert", 6));
	table.insert(qs, questions:createQuestion("Jetzt fuehle ich mich (4)",
		"entspannt", "unruhig", 6));
	table.insert(qs, questions:createQuestion("Jetzt fuehle ich mich (5)",
		"skeptisch", "vertrauensvoll", 6));
	table.insert(qs, questions:createQuestion("Jetzt fuehle ich mich (6)",
		"behaglich", "unwohl", 6));
elseif lang == "en" then
	table.insert(qs, questions:createQuestion("How strong is the fatigue of your eyes at the moment?",
		"not at all", "very strong", 6));
	table.insert(qs, questions:createQuestion("At the moment I feel (1)",
		"tense", "relaxed", 6));
	table.insert(qs, questions:createQuestion("At the moment I feel (2)",
		"relaxed", "queasy", 6));
	table.insert(qs, questions:createQuestion("At the moment I feel (3)",
		"worried", "untroubled", 6));
	table.insert(qs, questions:createQuestion("At the moment I feel (4)",
		"calm", "nervous", 6));
	table.insert(qs, questions:createQuestion("At the moment I feel (5)",
		"skeptically", "trustful", 6));
	table.insert(qs, questions:createQuestion("At the moment I feel (6)",
		"comfortable", "miserable", 6));
else
	Scene:log("Invalid language for questions: "..lang);
end;

-- Start the questioning
startQuestions = function (callback)
	testing = false;
	-- Hide Targets
	for i, o in ipairs(targets) do
		Scene:removeObject(o);
	end;
	Scene:removeObject(marker);
	-- Hide Floor
	Scene:removeObject(rectFloor);
	-- For high constrast during questions
	Scene:setBackgroundColor(0,0,0,255);
	questions:startQuestioning(qs, callback);
end;

startQuestions(startTest);