-- Load Libraries

-- Distance to screen: 2 meter
-- Screen height: 0.78 meter
-- Screen width:  1.30 meter
-- Eye position: screen centered
local screenHeight = 0.78;
local viewingDistance = 2.0;
local headPosition = 0.5;
-- Camera Position
local camX, camY, camZ = 0, 0, 25;
-- Camera Direction
local viewX, viewY, viewZ = 0, 0, -1;
-- Camera field of View
local fieldOfView = 18;
-- Camera separation
local camSep = 0.15;
-- Camera - ParalaxPlane distance
local camPP = 25;
-- Texture properties
local maxColor = 8;
local exclusiveColor = 1;
-- Test properties
local numTargets = 2;
local targetWidth = 2;
local targetHeight = 2;

-- Appearance (Texture Paths)
shapes = {
	"res/square.png";
	"res/circle.png";
}
patterns = {
	"res/texMarbleBlack.jpg";
	"res/texMarbleGrey.jpg";
	"res/texMarbleBrown.jpg";
}
directions = {
	"convex";
	"concave";
}

-- Input Library
-- Maps chars to symbolic names
Key = {
[18] = "left";
[19] = "up";
[20] = "right";
[21] = "down";
[22] = "pgUp";
[23] = "pgDown";
[32] = "space";
}

Scene:log("Screen Height = "..screenHeight);
Scene:log("Viewing Distance = "..viewingDistance);
Scene:log("Head Position = "..headPosition);

-- Scene Construction
-- Define camera position
Scene:setCameraPos(camX, camY, camZ);
Scene:setCameraDir(viewX, viewY, viewZ);
Scene:setCameraFoV(fieldOfView);
Scene:setCameraSep(camSep);
Scene:setCameraParalaxPlane(camPP);
-- Write everything to the log
Scene:log("Camera Position = ("..camX..","..camY..","..camZ..")");
Scene:log("Camera Direction = ("..viewX..","..viewY..","..viewZ..")");
Scene:log("Camera Field of View = "..fieldOfView);
Scene:log("Camera Separation = "..camSep);
Scene:log("Camera Paralax Plane = "..camPP);
Scene:log("Texture Colors = "..maxColor.."(-"..exclusiveColor..")");
Scene:log("Target Properties = "..numTargets.." @ "..targetWidth.."x"..targetHeight);

-- Create a floor object
-- A Rectangle
rectFloor = Object("Rectangle");
-- Rectangles are defined by two vectors
rectFloor:setDirA(6,0,0);
rectFloor:setDirB(0,0,26);
-- Set the position of the rectangle
rectFloor:setPosition(-3, -2, -18.5);
-- The texture is a checkerboard pattern, and is repeated
-- 26 times in depth, 6 times in width
rectFloor:setTexCoords(0,0, 0,26, 6,0, 6,26);
rectFloor:setTexture(Texture("Simple", "res/checkerboard.png"));
-- Add the floor to the scene
Scene:addObject(rectFloor);

-- Create a table with numTargets targets
targets = {};
for i = 1, numTargets do
	-- Targets are Pixelplanes (textures that get drawn directly on screen
	local t = Object("Pixelplane");
	-- This inserts the object into the list
	table.insert(targets, t);
	-- Set the size of the object. This only matters with auto resize
	t:setSize(targetWidth, targetHeight);
	-- ...which is turned on
	t:setAutoResize(true);
	-- Objects can be treated like tables. However, do NOT overwrite
	-- field 0, or bad things happen.
	-- Here, a new field "pos" is created and filled with a table of coordinates
	t["pos"] = {0,0,0};
	-- Add them to the scene
	Scene:addObject(t);
end

-- This is a helper function. It randomly permutes a table.
-- (Changes the order of the objects)
permuteTable = function (t)
	local n = #t;
	for i = 1, n do
		local j = math.random(i, n);
		t[i], t[j] = t[j], t[i];
	end
end;

-- This method changes the pixel plane texture and updates the position
displayTarget = function (target)
	-- Chose a random shape
	local shape = shapes[math.random(1, #shapes)];
	-- Random direction (in/out)
	local dir = directions[math.random(1, #directions)];
	-- Get the target and it's position
	local pos = target["pos"];

--	// Uncomment the following for a Random Dot version
	local texture = Texture("RandomDot", shape);
	texture:setMaxColor(maxColor);
	texture:setExclusiveColor(exclusiveColor);
--	// Uncomment the following for a Pattern version
--	permuteTable(patterns); -- Pick two random patterns
--	local texture = Texture("Pattern", shape, patterns[1], patterns[2]);
--	// End Comments
	texture:setStyle(dir); -- Here the concave/convex status is set

	-- Set the newly created texture as texture of the target
	target:setTexture(texture);
	-- Move to proper position
	target:setPosition(pos[1], pos[2], pos[3]);
end;

-- This method reacts to user input
parseInput = function (k)
	-- Look up the key in the Key symbol table
	local d = Key[string.byte(k)];
	if d then
		-- The first target is picked and it's position is changed
		local target = targets[1];
		local pos = target["pos"];
		if d == "up" then
			pos[2] = pos[2] + 0.5;
		elseif d == "right" then
			pos[1] = pos[1] + 0.5;
		elseif d == "down" then
			pos[2] = pos[2] - 0.5;
		elseif d == "left" then
			pos[1] = pos[1] - 0.5;
		elseif d == "pgUp" then
			pos[2] = pos[3] - 0.5;
		elseif d == "pgDown" then
			pos[2] = pos[3] + 0.5;
		elseif d == "space" then
		end
		displayTarget(target);
	else
		Scene:log("Input invalid, ignored ("..string.byte(k)..")");
	end
end;

displayTarget(targets[1]);
displayTarget(targets[2]);

-- Sets the function defined above as handler of keyDown events
-- It is executed every time a key is pressed
Scene:setEventListener("keyDown", parseInput);

timePassed = 0;
updateScene = function (t)
	timePassed = timePassed + t;
	-- The second target is picked and it's position is changed
	local target = targets[2];
	local x, y, z;
	x = math.sin(timePassed)*3-1;
	y = math.sin(timePassed/10)*2;
	z = math.cos(timePassed)*2;
	target:setPosition(x, y, z);
end;

-- Sets the function defined above as handler of update events
-- It is executed before every frame redraw
Scene:setEventListener("update", updateScene);
