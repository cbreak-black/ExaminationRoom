-- Load Libraries
dofile("res/statistics.lua")
-- Distance to screen: 2.5 meter
-- Screen height: 0.79 meter
-- Screen width:  1.31 meter
-- Eye position: screen centered
statistics:setViewingProperties(1, 0.70, 0.5);

-- Scene Construction
Scene:setCameraPos(0, 0, 25);
Scene:setCameraDir(0, 0, -1);
Scene:setCameraFoV(18);
Scene:setCameraSep(0.15);
Scene:setCameraParalaxPlane(25);

rectFloor = Object("Rectangle");
rectFloor:setDirA(6,0,0);
rectFloor:setDirB(0,0,26);
rectFloor:setPosition(-3, -2.5, -18);
rectFloor:setTexCoords(0,0, 0,26, 6,0, 6,26);
rectFloor:setTexture(Texture("Simple", "res/checkerboard.png"));

rectCeil = Object("Rectangle");
rectCeil:setDirA(6,0,0);
rectCeil:setDirB(0,0,26);
rectCeil:setPosition(-3, 2.5, -18);
rectCeil:setTexCoords(0,0, 0,26, 6,0, 6,26);
rectCeil:setTexture(Texture("Simple", "res/checkerboard.png"));

Scene:log("Added floor and ceil");

stereogramA = Object("Pixelplane");
stereogramA:setSize(2.0,2.0);
stereogramA:setAutoResize(true);
Scene:addObject(stereogramA);

stereogramB = Object("Pixelplane");
stereogramB:setSize(2.0,2.0);
stereogramB:setAutoResize(true);
Scene:addObject(stereogramB);

Scene:log("Added stereogram");

-- Library
Key = {
[18] = "left";
[19] = "up";
[20] = "right";
[21] = "down";
[32] = "space";
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
-- For object
shapes =
{
"res/square.png";
"res/circle.png";
}

-- Test Scene details
depths = {
	-17, -8.5, -3.5, 0, 5, 8
}
xys = {
	{ -3, -2.5 };
	{ 1, -2.5};
	{ -3, 0.5};
	{ 1, 0.5};
}
mountPoints = {}
for j = 1, #depths do
	for i = 1, #xys do
		table.insert(mountPoints, {xys[i][1], xys[i][2], depths[j]});
	end
end

-- Possible replies
replies = {};
for i = 1, #mountPoints/2 do
	table.insert(replies, "convex");
	table.insert(replies, "concave");
end

permuteTable = function (t)
	local n = #t;
	for i = 1, n do
		local j = math.random(i, n);
		t[i], t[j] = t[j], t[i];
	end
end;

checkOverlap = function (t)
	local pold = {0,0,0};
	for i, p in ipairs(t) do
		if p[1] == pold[1] and p[2] == pold[2] then
			return true;
		end;
		pold = p;
	end
	return false;
end

parseInput = function (k)
	local d = Key[string.byte(k)];
	if d then
		if Dir[d] then
			d = Dir[d];
			if d == replies[testNum] then
				Scene:log("Input Correct: "..d);
			else
				Scene:log("Input Incorrect: "..d);
			end;
			nextFrame();
		elseif d == "space" then
			Scene:log("Input Skipped: "..d);
			nextFrame();
		end
	else
		Scene:log("Input invalid, ignored ("..string.byte(k)..")");
	end
end;
