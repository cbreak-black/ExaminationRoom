-- Load Libraries
dofile("res/statistics.lua")
-- Distance to screen: 0.5 meter
-- Screen height: 0.5 meter
-- Eye position: screen centered
statistics:setViewingProperties(2, 0.75, 0.5);

-- Scene Construction
Scene:setCameraPos(0, 0, 10);
Scene:setCameraDir(0, 0, -1);
Scene:setCameraFoV(50);
Scene:setCameraSep(0.15);
Scene:setCameraParalaxPlane(10);

local sn = Object("Text");
sn:setText("Watch here");
sn:setPosition(0,0,0);
Scene:addObject(sn);

local rectFloor = Object("Rectangle");
rectFloor:setDirA(6,0,0);
rectFloor:setDirB(0,0,12);
rectFloor:setPosition(-3, -3, -9);
rectFloor:setTexCoords(0,0, 0,12, 6,0, 6,12);
rectFloor:setTexture(Texture("Simple", "res/checkerboard.png"));
Scene:addObject(rectFloor);

local rectCeil = Object("Rectangle");
rectCeil:setDirA(6,0,0);
rectCeil:setDirB(0,0,12);
rectCeil:setPosition(-3, 3, -9);
rectCeil:setTexCoords(0,0, 0,12, 6,0, 6,12);
rectCeil:setTexture(Texture("Simple", "res/checkerboard.png"));
Scene:addObject(rectCeil);

Scene:log("Added floor and ceil");

local stereogramA = Object("Pixelplane");
stereogramA:setSize(2.0,2.0);
stereogramA:setAutoResize(true);
Scene:addObject(stereogramA);

local stereogramB = Object("Pixelplane");
stereogramB:setSize(2.0,2.0);
stereogramB:setAutoResize(true);
Scene:addObject(stereogramB);

Scene:log("Added stereogram");

-- Library
Key = {
[18] = "left";
[19] = "up";
[20] = "right";
[21] = "down";
}
Dir = {
["up"] = "concave";
["down"] = "convex";
}
-- For FG/BG
patterns = {
"res/texMarbleBlack.jpg";
"res/texMarbleGrey.jpg";
"res/texMarbleBrown.jpg";
}
-- For object
shapes =
{
"res/square.png";
"res/circle.png";
}

-- Test Scene details
mountPoints = { -- Pre Permuted, no oclusion
{-3,1,-5},
{-3,-3,0},
{1,1,-5},
{-3,1,2},
{1,1,0},
{1,-3,-5},
{-3,-3,-5},
{1,-3,0},
{1,1,2},
{-3,1,0},
{-3,-3,2},
{1,-3,2},
}

-- Possible replies
replies = { -- Same size as mountPoints
"convex","convex","convex","convex","convex","convex",
"concave","concave","concave","concave","concave","concave",
}

permuteTable = function (t)
	local n = #t;
	for i = 1, n do
		local j = math.random(i, n);
		t[i], t[j] = t[j], t[i];
	end
end;

local testNum = 0;
local nextFrame = function ()
	testNum = (testNum % #mountPoints) + 1;
	if testNum == 1 then
		Scene:log("New Test Cycle");
--		permuteTable(mountPoints);
		permuteTable(replies);
	end
--	// Uncomment the following for a (rerendered) Stereogram version
--	local texture = Texture("Stereogram",
--		string.format(texbases[texIndexes[testNum]], replies[testNum], "l"),
--		string.format(texbases[texIndexes[testNum]], replies[testNum], "r"));
--	// Uncomment the following for a Random Dot version
--	permuteTable(shapes); -- Pick a random shape
--	local texture = Texture("RandomDot", shapes[1]);
--	texture:setMaxColor(4);
--	texture:setExclusiveColor(1);
--	// Uncomment the following for a Pattern version
	permuteTable(patterns); -- Pick two random patterns
	permuteTable(shapes); -- Pick a random shape
	local texture = Texture("Pattern", shapes[1], patterns[1], patterns[2]);
--	// End Comments
	texture:setStyle(replies[testNum]); -- Here the concave/convex status is set
	local pos = mountPoints[testNum];
	if (testNum % 2 == 0) then
		stereogramB:setTexture(texture);
		stereogramB:setPosition(pos[1], pos[2], pos[3]);
	else
		stereogramA:setTexture(texture);
		stereogramA:setPosition(pos[1], pos[2], pos[3]);
	end;
	sn:setPosition(pos[1]+2, pos[2], pos[3]);
	-- Separation at center
	local sep = statistics:separationAtPoint(pos[1]+1, pos[2]+1, pos[3]);
	local s = string.format("New Q: %s @ (%0.2f, %0.2f, %0.2f), s=%0.4f deg",
		replies[testNum], pos[1]+1, pos[2]+1, pos[3], sep);
	Scene:log(s);
end
nextFrame();

local parseInput = function (k)
	local d = Key[string.byte(k)];
	if d then d = Dir[d] end;
	if d then
		if d == replies[testNum] then
			Scene:log("Input Correct: "..d);
		else
			Scene:log("Input Incorrect: "..d);
		end;
		nextFrame();
	else
		Scene:log("Input invalid, ignored ("..string.byte(k)..")");
	end
end;

pathLength = 0;
local updateListener = function (delta)
	pathLength = pathLength + delta;
	stereogramA:setPosition(math.sin(pathLength)*2, math.cos(pathLength)*2, 2);
	stereogramB:setPosition(math.sin(pathLength)*2, -2,  math.cos(pathLength)*2);
end;


--Scene:setEventListener("update", updateListener);
Scene:setEventListener("keyDown", parseInput);
--Scene:setEventListener("keyUp", function (k) Scene:log("up: "..k); end);
Scene:setEventListener("quit", function (k) Scene:log("Exiting..."); end);

--Scene:clearScene();
