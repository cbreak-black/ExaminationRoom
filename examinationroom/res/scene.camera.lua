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

local sn = Text();
sn:setText("Watch here");
sn:setPosition({0,0,0});
Scene:addObject(sn);

floorPos = {-3, -2.5, -20};
local rectFloor = Rectangle();
rectFloor:setDirA({6,0,0});
rectFloor:setDirB({0,0,26});
rectFloor:setPosition(floorPos);
rectFloor:setTexCoords(0,0, 0,26, 6,0, 6,26);
rectFloor:setTexture(Texture("res/checkerboard.png"));
Scene:addObject(rectFloor);

local cameraBox = CameraNode();
local c2 = cameraBox:camera();
c2:setPosition(cameraPos);
c2:setDirection(cameraDir);
c2:setFieldOfView(cameraFoV);
c2:setSeparation(cameraSep);
c2:setParalaxPlane(cameraPP);
c2:setType("Parallel");
Scene:addObject(cameraBox);

local pep1 = Parallelepiped();
pep1:setTexture(Texture("res/checkerboard.png"));
local d = 1;
local l = 0.816*d;
local s = 0.578*d;
local s120 = math.sin(math.pi*2/3);
pep1:setDirA({0, s, l});
pep1:setDirB({s120*l, s, -0.5*l});
pep1:setDirC({-s120*l, s, -0.5*l});
pep1:setPosition({-3, -math.sqrt(3*d*d)/2 + 2, 0});
cameraBox:addObject(pep1);

local container = AffineTransformation();
container:setPosition({0, 0, 0});
Scene:addObject(container);

local pep2 = Parallelepiped();
pep2:setTexture(Texture("res/texMarbleGrey.jpg"));
local d = 1;
local l = 0.816*d;
local s = 0.578*d;
local s120 = math.sin(math.pi*2/3);
pep2:setDirA({0, s, l});
pep2:setDirB({s120*l, s, -0.5*l});
pep2:setDirC({-s120*l, s, -0.5*l});
pep2:setPosition({0, -math.sqrt(3*d*d)/2, 0});
container:addObject(pep2);

local camScreen = CameraNode();
camScreen:camera():setType("Screen");
camScreen:setDrawPriority(256);
Scene:addObject(camScreen);

local r1 = Rectangle();
r1:setDirA({512, 0, 0});
r1:setDirB({0, 512, 0});
r1:setPosition({0, -256, 0});
r1:setColor({0, 1, 1, 0.5});
r1:setWireframe(true);
r1:setSubdivision(8);
camScreen:addObject(r1);

local r2 = Rectangle();
r2:setDirA({1024, 0, 0});
r2:setDirB({0, 768, 0});
r2:setPosition({-512, -384, 0});
r2:setColor({1, 1, 0, 0.5});
r2:setWireframe(true);
r2:setSubdivision(4);
camScreen:addObject(r2);

local s1 = Sphere();
s1:setRadius(256);
s1:setPosition({-256, 0, 0});
s1:setColor({1, 1, 1, 0.25});
s1:setWireframe(true);
camScreen:addObject(s1);

-- Library
Key = {
[18] = "left";
[19] = "up";
[20] = "right";
[21] = "down";
[22] = "pgUp";
[23] = "pgDown";
[79] = "oblique"; -- o
[80] = "perspective"; -- p
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
			cam:setSeparation(cameraSep);
		elseif d == "right" then
			cameraAngle = cameraAngle + 0.1;
			cameraDir[1] = math.sin(cameraAngle);
			cameraDir[3] = -math.cos(cameraAngle);
			cameraPos[1] = -math.sin(cameraAngle)*cameraDistance;
			cameraPos[3] = math.cos(cameraAngle)*cameraDistance;
			cam:setDirection(cameraDir);
			cam:setPosition(cameraPos);
		elseif d == "down" then
			cameraSep = cameraSep - 0.01;
			cam:setSeparation(cameraSep);
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
		elseif d == "perspective" then
			cam:setType("Perspective");
		elseif d == "oblique" then
			cam:setType("Parallel");
		end;
		nextFrame();
	end
end;

local tPassed = 0;
local updateListener = function (delta)
	container:rotate({0,1,0}, delta/4);
end;
updateListener(0);

Scene:setEventListener("update", updateListener);
Scene:setEventListener("keyDown", parseInput);
--Scene:setEventListener("keyUp", function (k) Scene:log("up: "..k); end);
Scene:setEventListener("quit", function (k) Scene:log("Exiting..."); end);
