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

pathLength = 0;

local updateListener = function (delta)
	pathLength = pathLength + delta;
	stereogramA:setPosition(math.sin(pathLength)*2, math.cos(pathLength)*2, 2);
	stereogramB:setPosition(math.sin(pathLength)*2, -2,  math.cos(pathLength)*2);
end;

sap = {
{-2,-2,2},
{-2,-2,1},
{-2,-2,0},
{-2,-2,-1},
{-2,-2,-2},
{-2,-2,-3},
{-2,-2,-4},
{-2,-2,-5},
{-2,-2,-6},
{-2,-2,-7},
{-2,-2,-8},
{-2,-2,-9}
}
texpaths = {
"res/triangle_up.png",
"res/triangle_down.png",
"res/triangle_left.png",
"res/triangle_right.png"
}
local i = 1;
local nextFrame = function (k)
	Scene:log("Input key: "..k);
	stereogramB:setTexture(Texture(2, texpaths[math.random(#texpaths)]));
	local pos = sap[math.random(#sap)];
        stereogramB:setPosition(pos[1], pos[2], pos[3]);
	i = i + 1;
end

--Scene:setEventListener("update", updateListener);
Scene:setEventListener("keyDown", nextFrame);
--Scene:setEventListener("keyUp", function (k) Scene:log("up: "..k); end);

Scene:log("Added stereogram");

--Scene:clearScene();
