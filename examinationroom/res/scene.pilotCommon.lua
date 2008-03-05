-- Load Libraries
dofile("res/statistics.lua")
dofile("res/persistence.lua")
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

Scene:setEventListener("keyDown", parseInput);
Scene:setEventListener("quit", function (k) Scene:log("Exiting..."); end);

mountPoints,replies = persistence.load("res/scene.pilot.in.lua");

-- If no mount points exist, recalculate
if mountPoints == nil or replies == nil then
	Scene:log("Recalculating mount points");
	dofile("res/scene.generator.lua");
	mountPoints,replies = persistence.load("res/scene.pilot.in.lua");
end
