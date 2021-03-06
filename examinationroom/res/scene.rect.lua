-- Scene Construction
Scene:setCameraPos(0, 0, 10);
Scene:setCameraDir(0, 0, -1);
Scene:setCameraFoV(50);
Scene:setCameraSep(0.2);
Scene:setCameraParalaxPlane(10);

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

local stereogramA = Object("Rectangle");
stereogramA:setTexture(Texture("Simple", "res/e_right.png"));
stereogramA:setDirA(2,0,0);
stereogramA:setDirB(0,2,0);
stereogramA:setPosition(1, 1, 2);
Scene:addObject(stereogramA);

local stereogramC = Object("Rectangle");
stereogramC:setTexture(Texture("Simple", "res/e_up.png"));
stereogramC:setDirA(2,0,0);
stereogramC:setDirB(0,2,0);
stereogramC:setPosition(1, -3, 2);
Scene:addObject(stereogramC);

local stereogramB = Object("Rectangle");
stereogramB:setDirA(2,0,0);
stereogramB:setDirB(0,2,0);
--stereogramB:setAutoResize(true);
Scene:addObject(stereogramB);

Scene:log("Added stereogram");

-- Library
Key = {
[18] = "left";
[19] = "up";
[20] = "right";
[21] = "down";
}
texpaths = {
"res/triangle_left.png",
"res/triangle_up.png",
"res/triangle_right.png",
"res/triangle_down.png",
}
texbase = "res/triangle_%s.png"

-- Test Scene details
mountPoints = {
{-3,-3,2},
{-3,-3,1},
{-3,-3,0},
{-3,-3,-1},
{-3,-3,-2},
{-3,-3,-3},
{-3,-3,-4},
{-3,-3,-5},
}

arrowDirs = { -- Same size as mountPoints
"left", "up", "right", "down",
"left", "up", "right", "down",
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
		permuteTable(mountPoints);
		permuteTable(arrowDirs);
	end
	stereogramB:setTexture(Texture("RandomDot", string.format(texbase, arrowDirs[testNum])));
	local pos = mountPoints[testNum];
	stereogramB:setPosition(pos[1], pos[2], pos[3]);
	Scene:log("New Q: "..arrowDirs[testNum]..
		" @ ("..pos[1]..", "..pos[2]..", "..pos[3]..")");
end
nextFrame();

local parseInput = function (k)
	local d = Key[string.byte(k)];
	if d then
		if d == arrowDirs[testNum] then
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

--Scene:clearScene();
