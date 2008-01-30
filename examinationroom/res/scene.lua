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
stereogramA:setTexture(Texture(2, "res/triangle.png"));
Scene:addObject(stereogramA);

local stereogramB = Object();
stereogramB:setDirA(1,0,0);
stereogramB:setDirB(0,1,0);
stereogramB:setTexture(Texture(2, "res/triangle.png"));
Scene:addObject(stereogramB);

pathLength = 0;

local updateListener = function (delta)
	pathLength = pathLength + delta;
	stereogramA:setPosition(math.sin(pathLength)*2, math.cos(pathLength)*2, 2);
	stereogramB:setPosition(math.sin(pathLength)*2, -2,  math.cos(pathLength)*2);
end;
updateListener(0);
Scene:setUpdateListener(updateListener);

Scene:log("Added stereogram");

--Scene:clearScene();
