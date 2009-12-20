dofile("res/scene.pilotCommon.lua");

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
	local texture = RandomDot(shapes[math.random(1, #shapes)]);
	local maxColor = 8;
	texture:setMaxColor(maxColor);
	texture:setExclusiveColor(1);
	for i=0, (maxColor-1) do
		texture:setColor(i, 0, 0, 255/(maxColor-1)*i);
	end

--	// Uncomment the following for a Pattern version
--	permuteTable(patterns); -- Pick two random patterns
--	permuteTable(shapes); -- Pick a random shape
--	local texture = Texture("Pattern", shapes[1], patterns[1], patterns[2]);
--	// End Comments
	texture:setStyle(replies[testNum]); -- Here the concave/convex status is set
	local pos = mountPoints[testNum];
	if true then
		stereogramB:setTexture(texture);
		stereogramB:setPosition(pos);
	else
		stereogramA:setTexture(texture);
		stereogramA:setPosition(pos);
	end;
	-- Separation at center
	local sep = statistics:paralaxAtPoint(pos[1]+1, pos[2]+1, pos[3]);
	local s = string.format("New Q: %s/%s (%0.2f, %0.2f, %0.2f), s=%0.4f deg",
		replies[testNum],shapes[1], pos[1]+1, pos[2]+1, pos[3], sep);
	Scene:log(s);
end
nextFrame();
