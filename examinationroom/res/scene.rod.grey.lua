-- Load Libraries
dofile("res/statistics.lua")
-- Distance to screen: 2.5 meter
-- Screen height: 0.79 meter
-- Screen width:  1.31 meter
-- Eye position: screen centered
statistics:setViewingProperties(1.0, 0.79, 0.5);

-- Scene Construction
Scene:setCameraPos(0, 0, 25);
Scene:setCameraDir(0, 0, -1);
Scene:setCameraFoV(18);
Scene:setCameraSep(0.065);
Scene:setCameraParalaxPlane(25);

Scene:setBackgroundColor(127, 127, 127, 255);

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

pep = {};
for i = 1,5 do
	pep[i] = Object("Parallelepiped");
	pep[i]:setPosition(0, -3, -2);
	pep[i]:setTexture(Texture("Simple", "res/checkerboard.png"));
	Scene:addObject(pep[i]);
end
pep.pos = {0,0,0};

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
[80] = "pause"; -- p
[82] = "resume"; -- r
}
Dir = {
["up"] = "concave";
["down"] = "convex";
}

local parseInput = function (k)
	local step = 0.5;
	local d = Key[string.byte(k)];
	if d then
		Scene:log("input: "..d);
		local pos = pep.pos;
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
		elseif d == "pause" then
			Scene:setEventListener("update", nil);
		elseif d == "resume" then
			Scene:setEventListener("update", updateListener);
		end;
		updateListener(0);
	else
		Scene:log("Unknown key "..k.." ("..string.byte(k)..")");
	end
end;

local tPassed = 0;
updateListener = function (delta)
	local fullCircle = math.pi*2;
	tPassed = tPassed + delta;
	if (tPassed > fullCircle)  then
		tPassed = tPassed - fullCircle;
	end;
	local pos = pep.pos;
	local step = math.pi/2
	local x, y;
	local l = 2;
	local r = 0.25;
	for i = 1,4 do
		x = math.sin(tPassed+(i-1)*step);
		y = math.cos(tPassed+(i-1)*step);
		pep[i]:setDirA(x*l, y*l, 0);
		pep[i]:setDirB(y*r, -x*r, 0);
		pep[i]:setDirC(0, 0, r);
		pep[i]:setPosition(pos[1], pos[2], pos[3]-1/2*r);
		pep[i]:setTexCoords(0,0, 0,2/l, 2/r,0, 2/r,2/l);
	end
	pep[5]:setDirA(2*r, 0, 0);
	pep[5]:setDirB(0, 2*r, 0);
	pep[5]:setDirC(0, 0, -4*l);
	pep[5]:setPosition(pos[1]-r, pos[2]-r, pos[3]);
	pep[5]:setTexCoords(0,0, 0,4/l, 2/r,0, 2/r,4/l);
end;
updateListener(0);

Scene:setEventListener("update", updateListener);
Scene:setEventListener("keyDown", parseInput);
--Scene:setEventListener("keyUp", function (k) Scene:log("up: "..k); end);
Scene:setEventListener("quit", function (k) Scene:log("Exiting..."); end);
