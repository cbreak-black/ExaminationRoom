-- Scene Construction
Scene:setCameraPos(0, 0, -10);
Scene:setCameraDir(0, 0, -1);
Scene:setCameraFoV(50);
Scene:setCameraSep(0.2);

local rectFloor = Object();
rectFloor:setDirA(3,0,0);
rectFloor:setDirB(0,0,6);
rectFloor:setPosition(0, -3, -3);
rectFloor:setTexCoords(0,0, 0,6, 3,0, 3,6);
rectFloor:setTexture(Texture(1, "res/checkerboard.png"));
Scene:addObject(rectFloor);

local rectCeil = Object();
rectCeil:setDirA(3,0,0);
rectCeil:setDirB(0,0,6);
rectCeil:setPosition(0, 3, -3);
rectCeil:setTexCoords(0,0, 0,6, 3,0, 3,6);
rectCeil:setTexture(Texture(1, "res/checkerboard.png"));
Scene:addObject(rectCeil);

Scene:log("Added floor and ceil");

local stereogramA = Object();
stereogramA:setDirA(1,0,0);
stereogramA:setDirB(0,1,0);
stereogramA:setTexture(Texture(2, "res/triangle_up.png"));
stereogramA:setPosition(2, 2, 2);
stereogramA:setAutoResize(true);
Scene:addObject(stereogramA);

local stereogramB = Object();
stereogramB:setDirA(1,0,0);
stereogramB:setDirB(0,1,0);
stereogramB:setTexture(Texture(2, "res/triangle_down.png"));
stereogramB:setPosition(-2, -2, -2);
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
texpaths = {
"res/triangle_left.png",
"res/triangle_up.png",
"res/triangle_right.png",
"res/triangle_down.png",
}
texbase = "res/triangle_%s.png"

-- Test Scene details
mountPoints = {
{-2,-2,2},
{-2,-2,1},
{-2,-2,0},
{-2,-2,-1},
{-2,-2,-2},
{-2,-2,-3},
{-2,-2,-4},
{-2,-2,-5},
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
	stereogramB:setTexture(Texture(2, string.format(texbase, arrowDirs[testNum])));
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
