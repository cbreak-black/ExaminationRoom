-- Load Libraries
dofile("res/statistics.lua")
-- Distance to screen: 1.0 meter
-- Screen height: 0.79 meter
-- Screen width:  1.31 meter
-- Eye position: screen centered
statistics:setViewingProperties(1.0, 0.79, 0.5);

-- Scene Construction
local cameraAngle = 0;
local cameraDistance = 25;
local cameraPP = 25;
local cameraDir = {0, 0, -1};
local cameraPos = {0, 0, cameraDistance};
local cameraFoV = 18;
local cameraSep = 0.15;
Scene:setCameraPos(cameraPos[1], cameraPos[2], cameraPos[3]);
Scene:setCameraDir(cameraDir[1], cameraDir[2], cameraDir[3]);
Scene:setCameraFoV(cameraFoV);
Scene:setCameraSep(cameraSep);
Scene:setCameraParalaxPlane(cameraPP);

local sn = Object("Text");
sn:setText("Watch here");
sn:setPosition(0,0,0);
Scene:addObject(sn);

floorPos = {-3, -2.5, -20};
local rectFloor = Object("Rectangle");
rectFloor:setDirA(6,0,0);
rectFloor:setDirB(0,0,26);
rectFloor:setPosition(-3, -2.5, -18);
rectFloor:setTexCoords(0,0, 0,26, 6,0, 6,26);
rectFloor:setTexture(Texture("Simple", "res/checkerboard.png"));
Scene:addObject(rectFloor);

local rectCeil = Object("Rectangle");
rectCeil:setDirA(6,0,0);
rectCeil:setDirB(0,0,26);
rectCeil:setPosition(-3, 2.5, -18);
rectCeil:setTexCoords(0,0, 0,26, 6,0, 6,26);
rectCeil:setTexture(Texture("Simple", "res/checkerboard.png"));
Scene:addObject(rectCeil);

Scene:log("Added floor and ceil");

local container = Object("AffineTransformation");
container:setPosition(0, 0, 0);
Scene:addObject(container);

local pep2 = Object("Parallelepiped");
pep2:setTexture(Texture("Simple", "res/texMarbleGrey.jpg"));
local d = 1;
local l = 0.816*d;
local s = 0.578*d;
local s120 = math.sin(math.pi*2/3);
pep2:setDirA(0, s, l);
pep2:setDirB(s120*l, s, -0.5*l);
pep2:setDirC(-s120*l, s, -0.5*l);
pep2:setPosition(0, -math.sqrt(3*d*d)/2, 0)
container:addObject(pep2);

-- Library
Key = {
[18] = "left";
[19] = "up";
[20] = "right";
[21] = "down";
[22] = "pgUp";
[23] = "pgDown";
}
-- For FG/BG
patterns = {
"res/texMarbleBlack.jpg";
"res/texMarbleGrey.jpg";
"res/texMarbleBrown.jpg";
}

permuteTable = function (t)
	local n = #t;
	for i = 1, n do
		local j = math.random(i, n);
		t[i], t[j] = t[j], t[i];
	end
end;

local nextFrame = function ()
	local s = string.format("D: %0.1f, V: (%0.2f,%0.2f,%0.2f), F: %0.1f, S: %0.2f",
		cameraDistance,
		cameraDir[1], cameraDir[2], cameraDir[3],
		cameraFoV,
		cameraSep
	);
	sn:setText(s);
end
nextFrame();

local parseInput = function (k)
	local d = Key[string.byte(k)];
	local n = tonumber(k);
	if d then
		Scene:log("input: "..d);
		-- Unused
--		Scene:setCameraFoV(cameraFoV);
--		Scene:setCameraParalaxPlane(cameraDistance);
		if d == "up" then
			cameraSep = cameraSep + 0.01;
			Scene:setCameraSep(cameraSep);
		elseif d == "right" then
			cameraAngle = cameraAngle + 0.1;
			cameraDir[1] = math.sin(cameraAngle);
			cameraDir[3] = math.cos(cameraAngle);
			cameraPos[1] = -math.sin(cameraAngle)*cameraDistance;
			cameraPos[3] = -math.cos(cameraAngle)*cameraDistance;
			Scene:setCameraDir(cameraDir[1], cameraDir[2], cameraDir[3]);
			Scene:setCameraPos(cameraPos[1], cameraPos[2], cameraPos[3]);
		elseif d == "down" then
			cameraSep = cameraSep - 0.01;
			Scene:setCameraSep(cameraSep);
		elseif d == "left" then
			cameraAngle = cameraAngle - 0.1;
			cameraDir[1] = math.sin(cameraAngle);
			cameraDir[3] = math.cos(cameraAngle);
			cameraPos[1] = -math.sin(cameraAngle)*cameraDistance;
			cameraPos[3] = -math.cos(cameraAngle)*cameraDistance;
			Scene:setCameraDir(cameraDir[1], cameraDir[2], cameraDir[3]);
			Scene:setCameraPos(cameraPos[1], cameraPos[2], cameraPos[3]);
		elseif d == "pgUp" then
			cameraDistance = cameraDistance + 1;
			cameraPos[1] = -math.sin(cameraAngle)*cameraDistance;
			cameraPos[3] = -math.cos(cameraAngle)*cameraDistance;
			Scene:setCameraPos(cameraPos[1], cameraPos[2], cameraPos[3]);
		elseif d == "pgDown" then
			cameraDistance = cameraDistance - 1;
			cameraPos[1] = -math.sin(cameraAngle)*cameraDistance;
			cameraPos[3] = -math.cos(cameraAngle)*cameraDistance;
			Scene:setCameraPos(cameraPos[1], cameraPos[2], cameraPos[3]);
		end;
		nextFrame();
	end
end;

local tPassed = 0;
local updateListener = function (delta)
	container:rotate(0,1,0, delta/4);
end;
updateListener(0);

Scene:setEventListener("update", updateListener);
Scene:setEventListener("keyDown", parseInput);
--Scene:setEventListener("keyUp", function (k) Scene:log("up: "..k); end);
Scene:setEventListener("quit", function (k) Scene:log("Exiting..."); end);