-- Load Libraries
dofile("res/statistics.lua")
-- Distance to screen: 2.5 meter
-- Screen height: 0.79 meter
-- Screen width:  1.31 meter
-- Eye position: screen centered
statistics:setViewingProperties(2.5, 0.79, 0.5);

-- Scene Construction
Scene:setCameraPos(0, 0, 25);
Scene:setCameraDir(0, 0, -1);
Scene:setCameraFoV(18);
Scene:setCameraSep(0.2);
Scene:setCameraParalaxPlane(25);

local rectFloor = Object("Rectangle");
rectFloor:setDirA(6,0,0);
rectFloor:setDirB(0,0,48);
rectFloor:setPosition(-3, -2.5, -40);
rectFloor:setTexCoords(0,0, 0,48, 6,0, 6,48);
rectFloor:setTexture(Texture("Simple", "res/checkerboard.png"));
--Scene:addObject(rectFloor);

local rectCeil = Object("Rectangle");
rectCeil:setDirA(6,0,0);
rectCeil:setDirB(0,0,48);
rectCeil:setPosition(-3, 2.5, -40);
rectCeil:setTexCoords(0,0, 0,48, 6,0, 6,48);
rectCeil:setTexture(Texture("Simple", "res/checkerboard.png"));
--Scene:addObject(rectCeil);

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
depths = {
	-40, -20, -7.5, 0, 4.5, 8
}
xys = {
	{ -3, -2.5 };
	{ 1, -2.5};
	{ -3, 0.5};
	{ 1, 0.5};
}
mountPoints = {}
for j = 1, #depths do
	for i = 1, #xys do
		table.insert(mountPoints, {xys[i][1], xys[i][2], depths[j]});
	end
end

-- Possible replies
replies = {};
for i = 1, #mountPoints/2 do
	table.insert(replies, "convex");
	table.insert(replies, "concave");
end

permuteTable = function (t)
	local n = #t;
	for i = 1, n do
		local j = math.random(i, n);
		t[i], t[j] = t[j], t[i];
	end
end;

checkOverlap = function (t)
	local pold = {0,0,0};
	for i, p in ipairs(t) do
		if p[1] == pold[1] and p[2] == pold[2] then
			return true;
		end;
		pold = p;
	end
	return false;
end

local testNum = 0;
local nextFrame = function ()
	testNum = (testNum % #mountPoints) + 1;
	if testNum == 1 then
		Scene:log("New Test Cycle");
		permuteTable(mountPoints);
		while checkOverlap(mountPoints) do
			permuteTable(mountPoints);
		end
		permuteTable(replies);
	end
--	// Uncomment the following for a (rerendered) Stereogram version
--	local texture = Texture("Stereogram",
--		string.format(texbases[texIndexes[testNum]], replies[testNum], "l"),
--		string.format(texbases[texIndexes[testNum]], replies[testNum], "r"));
--	// Uncomment the following for a Random Dot version
	permuteTable(shapes); -- Pick a random shape
	local texture = Texture("RandomDot", shapes[1]);
	texture:setMaxColor(8);
	texture:setExclusiveColor(1);
--	// Uncomment the following for a Pattern version
--	permuteTable(patterns); -- Pick two random patterns
--	permuteTable(shapes); -- Pick a random shape
--	local texture = Texture("Pattern", shapes[1], patterns[1], patterns[2]);
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
	-- Separation at center
	local sep = statistics:paralaxAtPoint(pos[1]+1, pos[2]+1, pos[3]);
	local s = string.format("New Q: %s/%s (%0.2f, %0.2f, %0.2f), s=%0.4f deg",
		replies[testNum],shapes[1], pos[1]+1, pos[2]+1, pos[3], sep);
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
