-- Load Libraries
dofile("res/statistics.lua")

-- Distance to screen: 2.5 meter
-- Screen height: 0.79 meter
-- Screen width:  1.31 meter
-- Eye position: screen centered
local screenHeight = 0.79;
local viewingDistance = 1;
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
local numTargets = 2; -- Don't change without changing side dependent code
local targetWidth = 2;
local targetHeight = 2;

-- A perfect cycle visits all three depths/ two positions,
-- and uses every path exactly once
-- Even numbered nodes and odd numbered nodes are grouped
-- Those are label numbers, to find their position, look up their index
-- in the labelToIndex table.
perfectCycles = {
	{4, 3, 2, 1, 2, 5, 2, 3, 4, 1, 6, 1, 4, 5, 6, 3, 6, 5, 4};
	{4, 5, 6, 3, 6, 5, 4, 1, 6, 1, 5, 3, 2, 5, 2, 1, 2, 3, 4}; -- Backwards
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

statistics:setViewingProperties(screenHeight,
	viewingDistance,
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

rectFloor = Object("Rectangle");
rectFloor:setDirA(6,0,0);
rectFloor:setDirB(0,0,26);
rectFloor:setPosition(-3, -2.5, -18.5);
rectFloor:setTexCoords(0,0, 0,26, 6,0, 6,26);
rectFloor:setTexture(Texture("Simple", "res/checkerboard.png"));

targets = {};
for i = 1, numTargets do
	local t = Object("Pixelplane");
	table.insert(targets, t);
	t:setSize(targetWidth, targetHeight);
	t:setAutoResize(true);
end

Scene:log("Added floor");

-- State
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

-- Prepares the state for the next cycle
nextCycle = function ()
	-- Permute the label-to-index table
	for i, t in ipairs(labelToIndex) do
		permuteTable(t);
	end
	permuteTable(replies);
	currentCycle = math.random(1, #perfectCycles);
	Scene:log("New Cycle: "..currentCycle);
end

-- Prepares the state for the next target
nextTarget = function ()
	currentSide = currentSide%2 + 1;
	currentTest = currentTest + 1;
	if currentTest > #perfectCycles[currentCycle] then
		currentTest = 1;
		nextCycle();
	end
	local label = perfectCycles[currentCycle][currentTest];
	local side = label%2; -- 0 for even, 1 for odd
	local index = labelToIndex[side+1][(label+side)/2];
	Scene:log("New Target: Label("..label..") -> Index("..index..")");
end

displayNextTarget = function ()
	nextTarget();
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
	
	local target = targets[currentSide];
	target:setTexture(texture);
	target:setPosition(pos[1], pos[2], pos[3]);
	Scene:addObject(target);

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
			else
				Scene:log("Input Incorrect: "..d);
			end;
			displayNextTarget();
		elseif d == "space" then
			--Scene:log("Input Skipped: "..d);
			--displayNextTarget();
		end
	else
		Scene:log("Input invalid, ignored ("..string.byte(k)..")");
	end
end;

Scene:setEventListener("keyDown", parseInput);
Scene:setEventListener("quit", function (k) Scene:log("Exiting..."); end);

nextCycle();
displayNextTarget();
