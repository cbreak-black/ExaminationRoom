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

local sn = Object("Text");
sn:setText("Watch here");
sn:setPosition(0,0,0);
Scene:addObject(sn);

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

Scene:log("Added floor and ceil");

local stereograms = {};
for i=1,9 do
	stereograms[i] = Object("Pixelplane");
	stereograms[i]:setSize(2.0,2.0);
	stereograms[i]:setAutoResize(true);
	stereograms[i].pos = {0,0,0};
	Scene:addObject(stereograms[i]);
	Scene:log("Added stereogram "..i);
end


-- Library
Key = {
[18] = "left";
[19] = "up";
[20] = "right";
[21] = "down";
[22] = "pgUp";
[23] = "pgDown"
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

-- Possible replies
replies = { -- Same size as mountPoints
"convex","concave",
}

permuteTable = function (t)
	local n = #t;
	for i = 1, n do
		local j = math.random(i, n);
		t[i], t[j] = t[j], t[i];
	end
end;

local cur = 1;

local nextFrame = function ()
	permuteTable(replies);
--	// Uncomment the following for a Random Dot version
	permuteTable(shapes); -- Pick a random shape
	local texture = Texture("RandomDot", shapes[1]);
	texture:setMaxColor(4);
	texture:setExclusiveColor(1);
--	// Uncomment the following for a Pattern version
--	permuteTable(patterns); -- Pick two random patterns
--	permuteTable(shapes); -- Pick a random shape
--	local texture = Texture("Pattern", shapes[1], patterns[1], patterns[2]);
--	// End Comments
	texture:setStyle(replies[1]); -- Here the concave/convex status is set

	local pos = stereograms[cur].pos;
	stereograms[cur]:setTexture(texture);
	stereograms[cur]:setPosition(pos[1], pos[2], pos[3]);
	sn:setPosition(pos[1]+2, pos[2], pos[3]);
	-- Separation at center
	local sep = statistics:separationAtPoint(pos[1]+1, pos[2]+1, pos[3]);
	local s = string.format("%s @ (%0.2f,%0.2f,%0.2f),s=%0.4f",
		replies[1], pos[1]+1, pos[2]+1, pos[3], sep);
	Scene:log(s);
	sn:setText(s);
end
nextFrame();

local parseInput = function (k)
	local step = 1;
	local d = Key[string.byte(k)];
	local n = tonumber(k);
	if d then
		Scene:log("input: "..d);
		local pos = stereograms[cur].pos;
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
		end;
		nextFrame();
	elseif n then
		cur = n;
	end
end;

--Scene:setEventListener("update", updateListener);
Scene:setEventListener("keyDown", parseInput);
--Scene:setEventListener("keyUp", function (k) Scene:log("up: "..k); end);
Scene:setEventListener("quit", function (k) Scene:log("Exiting..."); end);

--Scene:clearScene();
