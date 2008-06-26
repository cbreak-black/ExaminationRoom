-- Insert LUA code here

-- Distance to screen: 2 meter
-- Screen height: 0.78 meter
-- Screen width:  1.30 meter
-- Eye position: screen centered
local screenHeight = 0.78;
local viewingDistance = 2.0;
local headPosition = 0.5;
-- Texture properties
local maxColor = 8;
local exclusiveColor = 1;
-- Target properties
local numTargets = 2;
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
	{4, 5, 6, 3, 6, 5, 4, 1, 6, 1, 4, 3, 2, 5, 2, 1, 2, 3, 4}; -- Backwards
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
--"res/square.png";
"res/circle.png";
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

-- Set parameters for statistics library
statistics:setViewingProperties(
	viewingDistance,
	screenHeight,
	headPosition);
Scene:log("Screen Height = "..screenHeight);
Scene:log("Viewing Distance = "..viewingDistance);
Scene:log("Head Position = "..headPosition);

-- Test Parameters (logging for statistics)
Scene:log("Texture Colors = "..maxColor.."(-"..exclusiveColor..")");
Scene:log("Target Properties = "..numTargets.." @ "..targetWidth.."x"..targetHeight);
Scene:log("Scene Properties = "..blocksPerScene.." blocks with "..cyclesPerBlock.." cycles per Block");

-- Make targets easier accessible
targets = {
	pp0,
	pp1
};

-- State for progression function
cycleNum = 0;
blockNum = 0;
currentBlockType = 1; -- 1 = continuous depth, 2 = no continuous depth
currentCycle = math.random(1, #perfectCycles);
currentSide = math.random(1, 2); -- 1 = left, 2 = right, as defined in targetX
currentTest = 0;
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

	local texture = RandomDot(shape);
	texture:setMaxColor(maxColor);
	texture:setExclusiveColor(exclusiveColor);
	texture:setStyle(replies[currentTest]); -- Here the concave/convex status is set

	local target;
	if numTargets == 2 then
		target = targets[currentSide];
	else
		target = targets[1];
	end
	target:setTexture(texture);
	-- Resize pixelplane to paralax plane size
	target:setShown(true);
	target:setPosition({0, 0, 0});
	target:resizeToCurrent();
	-- Move to proper position
	target:setPosition(pos);
	-- Resize marker to paralax plane size (HACK!)
	local pps = Scene:getUnitScreenSize({0,0,0});
	local dps = Scene:getUnitScreenSize(pos);
	marker:setDirA({targetWidth*pps/dps, 0, 0});
	marker:setDirB({0, targetHeight*pps/dps, 0});
	-- Move to proper position
	marker:setPosition({pos[1], pos[2]+0.001, pos[3]+0.001});
	marker:setShown(true);

	-- Floor
	if currentBlockType == 2 then
		floor:setShown(true);
	else
		floor:setShown(false);
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
					Scene:beep();
				end
				if (voiceCorrect) then
					os.execute("say -v "..voice.." correct &");
				end
			else
				Scene:log("Input Incorrect: "..d);
				if (audioIncorrect) then
					Scene:beep();
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
	os.exit();
end;

-- Questions
qs = {};
if lang == "de" then
	table.insert(qs, questions:createQuestion("Wie stark ist die Ermuedung ihrer Augen im Moment?",
		"gar nicht", "sehr stark", 6));
	table.insert(qs, questions:createQuestion("Jetzt fuehle ich mich",
		"angespannt", "gelassen", 6));
	table.insert(qs, questions:createQuestion("Jetzt fuehle ich mich",
		"geloest", "beklommen", 6));
	table.insert(qs, questions:createQuestion("Jetzt fuehle ich mich",
		"besorgt", "unbekuemmert", 6));
	table.insert(qs, questions:createQuestion("Jetzt fuehle ich mich",
		"entspannt", "unruhig", 6));
	table.insert(qs, questions:createQuestion("Jetzt fuehle ich mich",
		"skeptisch", "vertrauensvoll", 6));
	table.insert(qs, questions:createQuestion("Jetzt fuehle ich mich",
		"behaglich", "unwohl", 6));
	table.insert(qs, questions:createQuestion("Allgemeines Unwohlsein",
		{"nicht vorhanden", "etwas", "deutlich", "sehr stark"}));
	table.insert(qs, questions:createQuestion("Muedigkeit",
		{"nicht vorhanden", "etwas", "deutlich", "sehr stark"}));
	table.insert(qs, questions:createQuestion("Kopfschmerzen",
		{"nicht vorhanden", "etwas", "deutlich", "sehr stark"}));
	table.insert(qs, questions:createQuestion("Ueberanstrengte Augen",
		{"nicht vorhanden", "etwas", "deutlich", "sehr stark"}));
	table.insert(qs, questions:createQuestion("Schweirigkeiten beim Scharfsehen",
		{"nicht vorhanden", "etwas", "deutlich", "sehr stark"}));
	table.insert(qs, questions:createQuestion("Erhoehter Speichelfluss",
		{"nicht vorhanden", "etwas", "deutlich", "sehr stark"}));
	table.insert(qs, questions:createQuestion("Schwitzen",
		{"nicht vorhanden", "etwas", "deutlich", "sehr stark"}));
	table.insert(qs, questions:createQuestion("Uebelkeit",
		{"nicht vorhanden", "etwas", "deutlich", "sehr stark"}));
	table.insert(qs, questions:createQuestion("Konzentrationsschwierigkeiten",
		{"nicht vorhanden", "etwas", "deutlich", "sehr stark"}));
	table.insert(qs, questions:createQuestion("Druckgefuehl im Kopf",
		{"nicht vorhanden", "etwas", "deutlich", "sehr stark"}));
	table.insert(qs, questions:createQuestion("Verschwommenes Sehen",
		{"nicht vorhanden", "etwas", "deutlich", "sehr stark"}));
	table.insert(qs, questions:createQuestion("Schwindel bei geoeffneten Augen",
		{"nicht vorhanden", "etwas", "deutlich", "sehr stark"}));
	table.insert(qs, questions:createQuestion("Schwindel bei geschlossenen Augen",
		{"nicht vorhanden", "etwas", "deutlich", "sehr stark"}));
	table.insert(qs, questions:createQuestion("Drehschwindel",
		{"nicht vorhanden", "etwas", "deutlich", "sehr stark"}));
	table.insert(qs, questions:createQuestion("Missempfinden im Magenbereich",
		{"nicht vorhanden", "etwas", "deutlich", "sehr stark"}));
	table.insert(qs, questions:createQuestion("Aufstossen",
		{"nicht vorhanden", "etwas", "deutlich", "sehr stark"}));
elseif lang == "en" then
	table.insert(qs, questions:createQuestion("How strong is the fatigue of your eyes at the moment?",
		"not at all", "very strong", 6));
	table.insert(qs, questions:createQuestion("At the moment I feel",
		"tense", "relaxed", 6));
	table.insert(qs, questions:createQuestion("At the moment I feel",
		"relaxed", "queasy", 6));
	table.insert(qs, questions:createQuestion("At the moment I feel",
		"worried", "untroubled", 6));
	table.insert(qs, questions:createQuestion("At the moment I feel",
		"calm", "nervous", 6));
	table.insert(qs, questions:createQuestion("At the moment I feel",
		"skeptical", "trustful", 6));
	table.insert(qs, questions:createQuestion("At the moment I feel",
		"comfortable", "miserable", 6));
	table.insert(qs, questions:createQuestion("General discomfort",
		{"non", "slight", "moderate", "severe"}));
	table.insert(qs, questions:createQuestion("Fatigue",
		{"non", "slight", "moderate", "severe"}));
	table.insert(qs, questions:createQuestion("Headache",
		{"non", "slight", "moderate", "severe"}));
	table.insert(qs, questions:createQuestion("Eyestrain",
		{"non", "slight", "moderate", "severe"}));
	table.insert(qs, questions:createQuestion("Difficulty focusing",
		{"non", "slight", "moderate", "severe"}));
	table.insert(qs, questions:createQuestion("Increased salivation",
		{"non", "slight", "moderate", "severe"}));
	table.insert(qs, questions:createQuestion("Sweating",
		{"non", "slight", "moderate", "severe"}));
	table.insert(qs, questions:createQuestion("Nausea",
		{"non", "slight", "moderate", "severe"}));
	table.insert(qs, questions:createQuestion("Difficulty concentrating",
		{"non", "slight", "moderate", "severe"}));
	table.insert(qs, questions:createQuestion("Fullness of head",
		{"non", "slight", "moderate", "severe"}));
	table.insert(qs, questions:createQuestion("Blurred vision",
		{"non", "slight", "moderate", "severe"}));
	table.insert(qs, questions:createQuestion("Dizzy (eyes open)",
		{"non", "slight", "moderate", "severe"}));
	table.insert(qs, questions:createQuestion("Dizzy (eyes closed)",
		{"non", "slight", "moderate", "severe"}));
	table.insert(qs, questions:createQuestion("Vertigo",
		{"non", "slight", "moderate", "severe"}));
	table.insert(qs, questions:createQuestion("Stomach awareness",
		{"non", "slight", "moderate", "severe"}));
	table.insert(qs, questions:createQuestion("Burping",
		{"non", "slight", "moderate", "severe"}));
else
	Scene:log("Invalid language for questions: "..lang);
end;

-- Start the questioning
startQuestions = function (callback)
	testing = false;
	-- Hide the targets
	targets[1]:setShown(false);
	targets[2]:setShown(false);
	marker:setShown(false);
	-- For high constrast during questions
	Scene:setBackgroundColor(0,0,0,255);
	questions:startQuestioning(qs, callback);
end;

startQuestions(startTest);
