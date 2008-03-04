dofile("res/scene.pilotCommon.lua");

Scene:addObject(rectFloor);
Scene:addObject(rectCeil);

testNum = 0;
nextFrame = function ()
	testNum = (testNum % #mountPoints) + 1;
	if testNum == 1 then
		Scene:log("New Test Cycle");
		permuteTable(mountPoints);
		while checkOverlap(mountPoints) do
			permuteTable(mountPoints);
		end
		permuteTable(replies);
	end
--	// Uncomment the following for a (rerendered) Stereogram version
--	local texture = Texture("Stereogram",
--		string.format(texbases[texIndexes[testNum]], replies[testNum], "l"),
--		string.format(texbases[texIndexes[testNum]], replies[testNum], "r"));
--	// Uncomment the following for a Random Dot version
	permuteTable(shapes); -- Pick a random shape
	local texture = Texture("RandomDot", shapes[1]);
	texture:setMaxColor(8);
	texture:setExclusiveColor(1);
--	// Uncomment the following for a Pattern version
--	permuteTable(patterns); -- Pick two random patterns
--	permuteTable(shapes); -- Pick a random shape
--	local texture = Texture("Pattern", shapes[1], patterns[1], patterns[2]);
--	// End Comments
	texture:setStyle(replies[testNum]); -- Here the concave/convex status is set
	local pos = mountPoints[testNum];
	if true then
		stereogramB:setTexture(texture);
		stereogramB:setPosition(pos[1], pos[2], pos[3]);
	else
		stereogramA:setTexture(texture);
		stereogramA:setPosition(pos[1], pos[2], pos[3]);
	end;
	-- Separation at center
	local sep = statistics:paralaxAtPoint(pos[1]+1, pos[2]+1, pos[3]);
	local s = string.format("New Q: %s/%s (%0.2f, %0.2f, %0.2f), s=%0.4f deg",
		replies[testNum],shapes[1], pos[1]+1, pos[2]+1, pos[3], sep);
	Scene:log(s);
end
nextFrame();

Scene:setEventListener("keyDown", parseInput);
Scene:setEventListener("quit", function (k) Scene:log("Exiting..."); end);
