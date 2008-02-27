-- Load Libraries
dofile("res/statistics.lua")
-- Distance to screen: 2.5 meter
-- Screen height: 0.79 meter
-- Screen width:  1.31 meter
-- Eye position: screen centered
statistics:setViewingProperties(2.5, 0.79, 0.5);

-- Scene Construction
Scene:setCameraPos(0, 0, 25);
Scene:setCameraDir(0, 0, -1);
Scene:setCameraFoV(18);
Scene:setCameraSep(0.065);
Scene:setCameraParalaxPlane(25);

local rectFloor = Object("Rectangle");
rectFloor:setDirA(6,0,0);
rectFloor:setDirB(0,0,48);
rectFloor:setPosition(-3, -3, -40);
rectFloor:setTexCoords(0,0, 0,48, 6,0, 6,48);
rectFloor:setTexture(Texture("Simple", "res/checkerboard.png"));
Scene:addObject(rectFloor);

local rectCeil = Object("Rectangle");
rectCeil:setDirA(6,0,0);
rectCeil:setDirB(0,0,48);
rectCeil:setPosition(-3, 3, -40);
rectCeil:setTexCoords(0,0, 0,48, 6,0, 6,48);
rectCeil:setTexture(Texture("Simple", "res/checkerboard.png"));
Scene:addObject(rectCeil);

local pep1 = Object("Parallelepiped");
pep1:setDirA(1,0,0);
pep1:setDirB(0,1,0);
pep1:setDirC(0,0,1);
pep1:setTexCoords(0,0, 0,2, 2,0, 2,2);
pep1:setPosition(-3, -3, 0);
pep1:setTexture(Texture("Simple", "res/checkerboard.png"));
Scene:addObject(pep1);
local pep2 = Object("Parallelepiped");
pep2:setPosition(0, -3, -2);
pep2.pos = {0,-3,-2};
pep2:setTexture(Texture("Simple", "res/texMarbleGrey.jpg"));
Scene:addObject(pep2);

Scene:log("Added floor and ceil");

-- Library
Key = {
[18] = "left";
[19] = "up";
[20] = "right";
[21] = "down";
[22] = "pgUp";
[23] = "pgDown";
[72] = "hide"; -- h
[83] = "show"; -- s
}
Dir = {
["up"] = "concave";
["down"] = "convex";
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
	local pos = pep2.pos;
	pep2:setPosition(pos[1], pos[2], pos[3]);
end
nextFrame();

local parseInput = function (k)
	local step = 1;
	local d = Key[string.byte(k)];
	if d then
		Scene:log("input: "..d);
		local pos = pep2.pos;
		if d == "up" then
			pos[2] = pos[2] + step;
		elseif d == "right" then
			pos[1] = pos[1] + step;
		elseif d == "down" then
			pos[2] = pos[2] - step;
		elseif d == "left" then
			pos[1] = pos[1] - step;
		elseif d == "pgUp" then
			pos[3] = pos[3] - step;
		elseif d == "pgDown" then
			pos[3] = pos[3] + step;
		elseif d == "hide" then
			Scene:removeObject(rectFloor);
			Scene:removeObject(rectCeil);
		elseif d == "show" then
			Scene:addObject(rectFloor);
			Scene:addObject(rectCeil);
		end;
		nextFrame();
	else
		Scene:log("Unknown key "..k.." ("..string.byte(k)..")");
	end
end;

local tPassed = 0;
local updateListener = function (delta)
	local fullCircle = math.pi*2;
	tPassed = tPassed + delta;
	if (tPassed > fullCircle)  then
		tPassed = tPassed - fullCircle;
	end;
	local x, y;
	local d = fullCircle/3;
	local l = 0.816*3;
	local s = 0.578*3;
	x = math.sin(tPassed-d);
	y = math.cos(tPassed-d);
	pep2:setDirA(x*l, s, y*l);
	x = math.sin(tPassed);
	y = math.cos(tPassed);
	pep2:setDirB(x*l, s, y*l);
	x = math.sin(tPassed+d);
	y = math.cos(tPassed+d);
	pep2:setDirC(x*l, s, y*l);
end;
updateListener(0);

Scene:setEventListener("update", updateListener);
Scene:setEventListener("keyDown", parseInput);
--Scene:setEventListener("keyUp", function (k) Scene:log("up: "..k); end);
Scene:setEventListener("quit", function (k) Scene:log("Exiting..."); end);

--Scene:clearScene();