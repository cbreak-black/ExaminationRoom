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

floorPos = {-3, -2.5, -20};
local rectFloor = Rectangle();
rectFloor:setDirA({6,0,0});
rectFloor:setDirB({0,0,26});
rectFloor:setPosition(floorPos);
rectFloor:setTexCoords(0,0, 0,26, 6,0, 6,26);
rectFloor:setTexture(Texture("res/checkerboard.png"));
Scene:addObject(rectFloor);

local container1 = AffineTransformation();
container1:setPosition({0, 1, 0});
container1:scale({0.005, 0.005, 0.005});
Scene:addObject(container1);

local container = AffineTransformation();
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

local snl = Text();
snl:setText("Label Type Text\nAlways parallel to screen");
snl:setPosition({-3.5,-1,0});
snl:setType("Label");
snl:setColor({0,1,1,1});
container:addObject(snl);

local snp = Text();
snp:setText("Plane Type Text\nAffected by local transformations");
snp:setPosition({1,-1,0});
snp:setType("Plane");
snp:setColor({1,0,1,1});
container:addObject(snp);

local snt = Text();
snt:setText("Time passed since start:");
snt:setPosition({-1,1,0});
snt:setType("Label");
snt:setColor({1,1,0,1});
Scene:addObject(snt);

local snh = Text();
snh:setText("Text objects are created to have a fixed size on screen, independent of their distance to the camera. Some are affected by local transformation, so they can be rotated and scaled anyway.\nTheir Anchor is to the bottom-left.");
snh:setPosition({-2,2,-1});
snh:setDimensions({512,512});
snh:setType("Label");
snh:setColor({0.5,0.5,0.5,1});
Scene:addObject(snh);

-- Library
Key = {
[18] = "left";
[19] = "up";
[20] = "right";
[21] = "down";
[22] = "pgUp";
[23] = "pgDown";
}

local parseInput = function (k)
	local d = Key[string.byte(k)];
	local n = tonumber(k);
	if d then
		Scene:log("input: "..d);
		if d == "up" then
		elseif d == "right" then
			cameraAngle = cameraAngle + 0.1;
			cameraDir[1] = math.sin(cameraAngle);
			cameraDir[3] = -math.cos(cameraAngle);
			cameraPos[1] = -math.sin(cameraAngle)*cameraDistance;
			cameraPos[3] = math.cos(cameraAngle)*cameraDistance;
			cam:setDirection(cameraDir);
			cam:setPosition(cameraPos);
		elseif d == "down" then
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
	container:rotate({0,1,0}, delta/4);
	tPassed = tPassed + delta;
	snt:setText(string.format("Time passed since start: %0.3f",tPassed));
end;
updateListener(0);

Scene:setEventListener("update", updateListener);
Scene:setEventListener("keyDown", parseInput);
