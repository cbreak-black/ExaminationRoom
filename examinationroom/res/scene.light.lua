-- Load Libraries
Scene:loadLua("res/statistics.lua")
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

local lightPos = {-10, 0, 0};

local sn = Text();
sn:setText("Watch here");
sn:setPosition({0,0,0});
--Scene:addObject(sn);

floorPos = {-3, -2.5, -20};
local rectFloor = Rectangle();
rectFloor:setDirA({6,0,0});
rectFloor:setDirB({0,0,-26});
rectFloor:setPosition({-3, -2.5, 8});
rectFloor:setTexCoords(0,0, 0,26, 6,0, 6,26);
rectFloor:setSubdivision(16);
rectFloor:setTexture(Texture("res/checkerboard.png"));

local lightBox2 = LightNode();
lightBox2:setPosition({-lightPos[1], -lightPos[2], -lightPos[3]});
lightBox2:setColor({1.0, 0.0, 0.0, 1.0});
lightBox2:setAmbient({0.0, 0.0, 0.0, 1.0});
Scene:addObject(lightBox2);

local lightBox = LightNode();
lightBox:setPosition({lightPos[1], lightPos[2], lightPos[3]});
lightBox:setColor({0.0, 1.0, 1.0, 1.0});
lightBox:setAmbient({0.0, 0.0, 0.0, 1.0});
lightBox2:addObject(lightBox);

local container = AffineTransformation();
lightBox:addObject(container);

container:addObject(rectFloor);

local sphere = Sphere();
--sphere:setTexture(Texture("res/checkerboard.png"));
sphere:setPosition({3, 1, 0});
sphere:setRadius(1);
container:addObject(sphere);

local ee = Rectangle();
ee:setTexture(Texture("res/e_right.png"));
ee:setDirA({1,0,0});
ee:setDirB({0,1,0});
ee:setPosition({-0.5, 1, 0});
container:addObject(ee);

local r1 = Parallelepiped();
local d = 1;
local l = 0.816*d;
local s = 0.578*d;
local s120 = math.sin(math.pi*2/3);
r1:setDirA({0, s, l});
r1:setDirB({s120*l, s, -0.5*l});
r1:setDirC({-s120*l, s, -0.5*l});
r1:setPosition({-3, -math.sqrt(3*d*d)/2, 0})
container:addObject(r1);

local m1 = Mesh();
m1:loadMesh("res/bunny_243V.obj");
m1:setScaleFactor(12);
m1:setPosition({0, 2, 0});
container:addObject(m1);
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
container:addObject(m2);

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
			lightPos[2] = lightPos[2] + 0.5;
			lightBox:setPosition(lightPos);
			lightBox2:setPosition({-lightPos[1], lightPos[2], -lightPos[3]});
		elseif d == "right" then
			cameraAngle = cameraAngle + 0.1;
			cameraDir[1] = math.sin(cameraAngle);
			cameraDir[3] = -math.cos(cameraAngle);
			cameraPos[1] = -math.sin(cameraAngle)*cameraDistance;
			cameraPos[3] = math.cos(cameraAngle)*cameraDistance;
			cam:setDirection(cameraDir);
			cam:setPosition(cameraPos);
		elseif d == "down" then
			lightPos[2] = lightPos[2] - 0.5;
			lightBox:setPosition(lightPos);
			lightBox2:setPosition({-lightPos[1], lightPos[2], -lightPos[3]});
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
		end;
		nextFrame();
	end
end;

local tPassed = 0;
local updateListener = function (delta)
	container:rotate({0,1,0}, -delta/4);
end;
updateListener(0);

Scene:setEventListener("update", updateListener);
Scene:setEventListener("keyDown", parseInput);
--Scene:setEventListener("keyUp", function (k) Scene:log("up: "..k); end);
Scene:setEventListener("quit", function (k) Scene:log("Exiting..."); end);
