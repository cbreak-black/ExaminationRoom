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

local rectFloor = Object("Rectangle");
rectFloor:setDirA(6,0,0);
rectFloor:setDirB(0,0,12);
rectFloor:setPosition(-3, -3, -9);
rectFloor:setTexCoords(0,0, 0,12, 6,0, 6,12);
rectFloor:setTexture(Texture(1, "res/checkerboard.png"));
Scene:addObject(rectFloor);

local rectCeil = Object("Rectangle");
rectCeil:setDirA(6,0,0);
rectCeil:setDirB(0,0,12);
rectCeil:setPosition(-3, 3, -9);
rectCeil:setTexCoords(0,0, 0,12, 6,0, 6,12);
rectCeil:setTexture(Texture(1, "res/checkerboard.png"));
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
["up"] = "back";
["down"] = "front";
}
texbases = {
"res/marble_%s.%s.png";
"res/rectangles_%s.%s.png";
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

replies = { -- Same size as mountPoints
"back","back","back","back","back","back",
"front","front","front","front","front","front",
}
texIndexes = {
1,1,1,1,1,1,
2,2,2,2,2,2,
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
		permuteTable(texIndexes);
	end
	local texture = Texture(2,
		string.format(texbases[texIndexes[testNum]], replies[testNum], "l"),
		string.format(texbases[texIndexes[testNum]], replies[testNum], "r"));
	local pos = mountPoints[testNum];
	if (testNum % 2 == 0) then
		stereogramB:setTexture(texture);
		stereogramB:setPosition(pos[1], pos[2], pos[3]);
	else
		stereogramA:setTexture(texture);
		stereogramA:setPosition(pos[1], pos[2], pos[3]);
	end;
	local sep = statistics:separationAtPoint(pos[1], pos[2], pos[3]);
	Scene:log("New Q: "..replies[testNum]..
		" @ ("..pos[1]..", "..pos[2]..", "..pos[3].."), s="..sep);
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
