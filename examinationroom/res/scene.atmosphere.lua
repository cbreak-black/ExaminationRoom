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
local cameraDir = {0, -0.1, -1};
local cameraPos = {0, 2.5, cameraDistance};
local cameraFoV = 18;
local cameraSep = 0.15;
cam = Scene:camera();
cam:setPosition(cameraPos);
cam:setDirection(cameraDir);
cam:setFieldOfView(cameraFoV);
cam:setSeparation(cameraSep);
cam:setParalaxPlane(cameraPP);

local sn = Text();
sn:setPosition({0,1,0});
Scene:addObject(sn);

floorPos = {-3, -2.5, -20};
local rectFloor = Rectangle();
rectFloor:setDirA({6,0,0});
rectFloor:setDirB({0,0,26});
rectFloor:setPosition(floorPos);
rectFloor:setTexCoords(0,0, 0,26, 6,0, 6,26);
rectFloor:setTexture(Texture("res/checkerboard.png"));
Scene:addObject(rectFloor);

fogModes = {"Linear", "Exp", "Exp2"};
fogModeIdx = 1;
fogDensity = 0.05;
fogStart = 10;
fogEnd = 30;
local airBox = Atmosphere();
airBox:setMode(fogModes[fogModeIdx]);
airBox:setColor({0,0,0,1});
airBox:setDensity(fogDensity);
airBox:setStart(fogStart);
airBox:setEnd(fogEnd);
Scene:addObject(airBox);
sn:setText("Mode: "..fogModes[fogModeIdx]);

local container1 = AffineTransformation();
container1:setPosition({0, 1, 0});
container1:scale({0.005, 0.005, 0.005});
Scene:addObject(container1);

local container2 = AffineTransformation();
airBox:addObject(container2);

local m1 = Mesh();
m1:loadMesh("res/bunny_243V.obj");
m1:setScaleFactor(12);
m1:setPosition({0, 2, 0});
container2:addObject(m1);
local m2 = Mesh();
-- Dolphin model, with normals
--m2:loadMesh("res/Dolphin/Dolphin.obj");
--m2:setScaleFactor(0.005);
-- Bunny model, with normals
--m2:loadMesh("res/bunny.obj");
--m2:setScaleFactor(4);
-- Elephant model, with normals
m2:loadMesh("res/elephant.obj");
m2:setScaleFactor(0.005);
-- Cow model, without normals
--m2:loadMesh("res/cow.obj");
--m2:setScaleFactor(3);
-- Cube model, without normals
--m2:loadMesh("res/cube.obj");
--m2:setScaleFactor(1);
m2:setPosition({0, -2.5, 0});
container2:addObject(m2);

-- Library
Key = {
[18] = "left";
[19] = "up";
[20] = "right";
[21] = "down";
[22] = "pgUp";
[23] = "pgDown";
[70] = "fogMode";
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
	if fogModeIdx == 1 then
		sn:setText("Mode: "..fogModes[fogModeIdx].." with range "..fogStart..".."..fogEnd);
	else
		sn:setText("Mode: "..fogModes[fogModeIdx].." with density "..fogDensity);
	end
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
		-- factor is sqrt_4(2);
		stepFactor = 1.189207115;
		if d == "up" then
			fogDensity = fogDensity*stepFactor;
			fogEnd = fogEnd + 1;
			airBox:setDensity(fogDensity);
			airBox:setEnd(fogEnd);
		elseif d == "right" then
			cameraAngle = cameraAngle + 0.1;
			cameraDir[1] = math.sin(cameraAngle);
			cameraDir[3] = -math.cos(cameraAngle);
			cameraPos[1] = -math.sin(cameraAngle)*cameraDistance;
			cameraPos[3] = math.cos(cameraAngle)*cameraDistance;
			cam:setDirection(cameraDir);
			cam:setPosition(cameraPos);
		elseif d == "down" then
			fogDensity = fogDensity/stepFactor;
			fogEnd = fogEnd - 1;
			airBox:setDensity(fogDensity);
			airBox:setEnd(fogEnd);
		elseif d == "left" then
			cameraAngle = cameraAngle - 0.1;
			cameraDir[1] = math.sin(cameraAngle);
			cameraDir[3] = -math.cos(cameraAngle);
			cameraPos[1] = -math.sin(cameraAngle)*cameraDistance;
			cameraPos[3] = math.cos(cameraAngle)*cameraDistance;
			cam:setDirection(cameraDir);
			cam:setPosition(cameraPos);
		elseif d == "pgUp" then
			cameraDistance = cameraDistance + 1;
			cameraPos[1] = -math.sin(cameraAngle)*cameraDistance;
			cameraPos[3] = math.cos(cameraAngle)*cameraDistance;
			cam:setPosition(cameraPos);
		elseif d == "pgDown" then
			cameraDistance = cameraDistance - 1;
			cameraPos[1] = -math.sin(cameraAngle)*cameraDistance;
			cameraPos[3] = math.cos(cameraAngle)*cameraDistance;
			cam:setPosition(cameraPos);
		elseif d == "fogMode" then
			fogModeIdx = fogModeIdx%#fogModes + 1
			airBox:setMode(fogModes[fogModeIdx]);
		end;
		nextFrame();
	end
end;

local tPassed = 0;
local updateListener = function (delta)
	container1:rotate({0,1,0}, delta/4);
	container2:rotate({0,1,0}, -delta/4);
end;
updateListener(0);

Scene:setEventListener("update", updateListener);
Scene:setEventListener("keyDown", parseInput);
--Scene:setEventListener("keyUp", function (k) Scene:log("up: "..k); end);
Scene:setEventListener("quit", function (k) Scene:log("Exiting..."); end);
