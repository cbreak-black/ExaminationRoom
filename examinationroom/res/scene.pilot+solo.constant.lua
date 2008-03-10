dofile("res/scene.pilotCommon.lua");

Scene:addObject(rectFloor);
Scene:addObject(rectCeil);
stereogramA:setAutoResize(false);
stereogramB:setAutoResize(false);

testNum = 0;
nextFrame = function ()
	testNum = testNum + 1;
	if testNum > #mountPoints then
		return;
	end;
--	// Uncomment the following for a (rerendered) Stereogram version
--	local texture = Texture("Stereogram",
--		string.format(texbases[texIndexes[testNum]], replies[testNum], "l"),
--		string.format(texbases[texIndexes[testNum]], replies[testNum], "r"));
--	// Uncomment the following for a Random Dot version
	local texture = Texture("RandomDot", shapes[math.random(1, #shapes)]);
	local maxColor = 8;
	texture:setMaxColor(maxColor);
	texture:setExclusiveColor(1);

--	// Uncomment the following for a Pattern version
--	permuteTable(patterns); -- Pick two random patterns
--	permuteTable(shapes); -- Pick a random shape
--	local texture = Texture("Pattern", shapes[1], patterns[1], patterns[2]);
--	// End Comments
	texture:setStyle(replies[testNum]); -- Here the concave/convex status is set
	local pos = mountPoints[testNum];
	local zoomX, zoomY = 1,1;
	local offsetX, offsetY = 0,0;
	if pos[1] > 0 then
		zoomX = -1;
		offsetX = 2;
	end;
	if pos[2] > 0 then
		zoomY = -1;
		offsetY = 2;
	end;
	texture:setZoom(zoomX,zoomY);
	if true then
		stereogramB:setPosition(pos[1]+offsetX, pos[2]+offsetY, 0);
		stereogramB:setTexture(texture);
		stereogramB:resizeToCurrent();
		stereogramB:setPosition(pos[1]+offsetX, pos[2]+offsetY, pos[3]);
	else
		stereogramA:setPosition(pos[1]+offsetX, pos[2]+offsetY, 0);
		stereogramA:setTexture(texture);
		stereogramA:resizeToCurrent();
		stereogramA:setPosition(pos[1]+offsetX, pos[2]+offsetY, pos[3]);
	end;
	-- Separation at center
	local sep = statistics:paralaxAtPoint(pos[1]+1, pos[2]+1, pos[3]);
	local s = string.format("New Q: %s/%s (%0.2f, %0.2f, %0.2f), s=%0.4f deg",
		replies[testNum],shapes[1], pos[1]+1, pos[2]+1, pos[3], sep);
	Scene:log(s);
end
nextFrame();
