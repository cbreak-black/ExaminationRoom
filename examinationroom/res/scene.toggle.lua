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

sceneBackups = {};
sceneCurrent = 0;

local lightPos = {-10, 0, 0};

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

local containerLight = AffineTransformation();
lightBox:addObject(containerLight);

containerLight:addObject(rectFloor);

local sphere = Sphere();
--sphere:setTexture(Texture("res/checkerboard.png"));
sphere:setPosition({3, 1, 0});
sphere:setRadius(1);
containerLight:addObject(sphere);

local ee = Rectangle();
ee:setTexture(Texture("res/e_right.png"));
ee:setDirA({1,0,0});
ee:setDirB({0,1,0});
ee:setPosition({-0.5, 1, 0});
containerLight:addObject(ee);

local r1 = Parallelepiped();
local d = 1;
local l = 0.816*d;
local s = 0.578*d;
local s120 = math.sin(math.pi*2/3);
r1:setDirA({0, s, l});
r1:setDirB({s120*l, s, -0.5*l});
r1:setDirC({-s120*l, s, -0.5*l});
r1:setPosition({-3, -math.sqrt(3*d*d)/2, 0})
containerLight:addObject(r1);

local m1 = Mesh();
m1:loadMesh("res/bunny_243V.obj");
m1:setScaleFactor(12);
m1:setPosition({0, 2, 0});
containerLight:addObject(m1);
local m2 = Mesh();
m2:loadMesh("res/elephant.obj");
m2:setScaleFactor(0.005);
m2:setPosition({0, -2.5, 0});
containerLight:addObject(m2);

sceneBackups[1] = Scene:split();

local rectFloor = Rectangle();
rectFloor:setDirA({6,0,0});
rectFloor:setDirB({0,0,26});
rectFloor:setPosition(floorPos);
rectFloor:setTexCoords(0,0, 0,26, 6,0, 6,26);
rectFloor:setTexture(Texture("res/checkerboard.png"));
Scene:addObject(rectFloor);

local lightBox = LightNode();
lightBox:setPosition({-4, 2, 2});
lightBox:setColor({1.0, 1.0, 1.0, 1.0});
lightBox:setAmbient({0.1, 0.1, 0.1, 1.0});
lightBox:setDrawPriority(64); -- Draw after background
Scene:addObject(lightBox);

local containerDepth = AffineTransformation();
lightBox:addObject(containerDepth);

local depthBox = DepthBuffer();
containerDepth:addObject(depthBox);

local sphere = Sphere();
--sphere:setTexture(Texture("res/checkerboard.png"));
sphere:setPosition({3, 1, 0});
sphere:setRadius(1);
sphere:setColor({0,1,0,1});
sphere:setDrawPriority(-8); -- Draw second
containerDepth:addObject(sphere);

local r1 = Parallelepiped();
local d = 1;
local l = 0.816*d;
local s = 0.578*d;
local s120 = math.sin(math.pi*2/3);
r1:setDirA({0, s, l});
r1:setDirB({s120*l, s, -0.5*l});
r1:setDirC({-s120*l, s, -0.5*l});
r1:setPosition({-3, -math.sqrt(3*d*d)/2, 0});
r1:setColor({1,0,0,1});
depthBox:addObject(r1);

local m1 = Mesh();
m1:loadMesh("res/bunny_243V.obj");
m1:setScaleFactor(12);
m1:setPosition({0, 1, 0});
m1:setColor({0,0,1,1});
m1:setDrawPriority(-16); -- Draw first
depthBox:addObject(m1);
local m2 = Mesh();
m2:loadMesh("res/elephant.obj");
m2:setScaleFactor(0.005);
m2:setPosition({0, -2.5, 0});
m2:setColor({1,0,1,1});
depthBox:addObject(m2);

sceneBackups[2] = Scene:split();

-- Stores the current scene in a backup slot with Scene:split()
-- And loads the backup id with Scene:merge()
loadScene = function (id)
	if sceneBackups[id] then
		sceneBackups[sceneCurrent] = Scene:split();
		Scene:merge(sceneBackups[id])
		sceneBackups[id] = nil;
		sceneCurrent = id;
	end
end;

loadScene(1);

-- Library
Key = {
[18] = "left";
[19] = "up";
[20] = "right";
[21] = "down";
[22] = "pgUp";
[23] = "pgDown";
[76] = "load";
[83] = "store";
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
		elseif d == "right" then
		elseif d == "down" then
		elseif d == "left" then
		elseif d == "pgUp" then
		elseif d == "pgDown" then
		elseif d == "load" then
			if storedScene then
				sceneBackups[sceneCurrent] = Scene:split();
				Scene:merge(storedScene)
				storedScene = nil;
				sceneCurrent = -1; -- Out of range
			end
		elseif d == "store" then
			storedScene = Scene:clone();
		end;
		nextFrame();
	end
	if n then
		loadScene(n);
	end
end;

local tPassed = 0;
local updateListener = function (delta)
	containerLight:rotate({0,1,0}, -delta/4);
	containerDepth:rotate({0,1,0}, -delta/4);
end;
updateListener(0);

Scene:setEventListener("update", updateListener);
Scene:setEventListener("keyDown", parseInput);
--Scene:setEventListener("keyUp", function (k) Scene:log("up: "..k); end);
Scene:setEventListener("quit", function (k) Scene:log("Exiting..."); end);
