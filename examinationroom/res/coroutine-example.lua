-- This is an example scene demonstrating the use of Coroutines
-- to get a continuous thread of execution

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
sphere:setPosition({3, 1, 0});
sphere:setRadius(1);
container:addObject(sphere);

local m1 = Mesh();
m1:loadMesh("res/bunny_243V.obj");
m1:setScaleFactor(12);
m1:setPosition({0, 2, 0});
container:addObject(m1);
local m2 = Mesh();
-- Dolphin model, with normals
m2:loadMesh("res/elephant.obj");
m2:setScaleFactor(0.005);
m2:setPosition({0, -2.5, 0});
container:addObject(m2);

Scene:log("Scene Layout created")

-- This is the main coroutine of execution
-- Do what ever you want here
myProgram = function ()
	while true do
		container:rotate({0,1,0}, -1/4);
		sleep(1)
	end
end

sleep = function (t)
	local d = coroutine.yield()
	while d < t do
		d = d + coroutine.yield()
	end
end

myCoroutine = coroutine.wrap(myProgram)

-- Library
Key = {
[18] = "left";
[19] = "up";
[20] = "right";
[21] = "down";
[22] = "pgUp";
[23] = "pgDown";
}

-- This handles key input, it is effectively
-- in an other thread
local parseInput = function (k)
	local d = Key[string.byte(k)];
	local n = tonumber(k);
	if d then
		Scene:log("input: "..d);
		if d == "up" then
		elseif d == "right" then
		elseif d == "down" then
		elseif d == "left" then
		elseif d == "pgUp" then
		elseif d == "pgDown" then
		end;
	end
end;

Scene:setEventListener("update", myCoroutine);
Scene:setEventListener("keyDown", parseInput);
--Scene:setEventListener("keyUp", function (k) Scene:log("up: "..k); end);
