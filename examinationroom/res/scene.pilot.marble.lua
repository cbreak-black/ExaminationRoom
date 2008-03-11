dofile("res/scene.pilotCommon.lua");

rectFloor:setTexture(Texture("Simple", patterns[2]));
rectCeil:setTexture(Texture("Simple", patterns[2]));
rectFloor:setTexCoords(0,0, 0,13, 3,0, 3,13);
rectCeil:setTexCoords(0,0, 0,13, 3,0, 3,13);
Scene:addObject(rectFloor);
Scene:addObject(rectCeil);

local permuteTable = function (t)
	local n = #t;
	for i = 1, n do
		local j = math.random(i, n);
		t[i], t[j] = t[j], t[i];
	end
end;

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
--	local texture = Texture("RandomDot", shapes[math.random(1, #shapes)]);
--	texture:setMaxColor(8);
--	texture:setExclusiveColor(1);
--	// Uncomment the following for a Pattern version
	permuteTable(patterns); -- Pick two random patterns
	permuteTable(shapes); -- Pick a random shape
	local texture = Texture("Pattern", shapes[1], patterns[1], patterns[2]);
--	// End Comments
	texture:setStyle(replies[testNum]); -- Here the concave/convex status is set
	local pos = mountPoints[testNum];
	if (testNum % 2 == 0) then
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
