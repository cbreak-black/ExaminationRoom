dofile("res/scene.pilotCommon.lua");

Scene:log("Bisect");

local permuteTable = function (t)
	local n = #t;
	for i = 1, n do
		local j = math.random(i, n);
		t[i], t[j] = t[j], t[i];
	end
end;

farPoint = -16;
nearPoint = 4;

farCorrect = 0;
farWrong = 0;
nearCorrect = 0;
nearWrong = 0;

parseInput = function (k)
	local d = Key[string.byte(k)];
	if d then
		if Dir[d] then
			d = Dir[d];
			if d == replies[testNum] then
				Scene:log("Input Correct: "..d);
				if farTrial(testNum) then
					farCorrect = farCorrect + 1;
				else
					nearCorrect = nearCorrect + 1;
				end
			else
				Scene:log("Input Incorrect: "..d);
				if farTrial(testNum) then
					farWrong = farWrong + 1;
				else
					nearWrong = nearWrong + 1;
				end
			end;
			nextFrame();
		elseif d == "space" then
			--Scene:log("Input Skipped: "..d);
			--nextFrame();
		end
	else
		Scene:log("Input invalid, ignored ("..string.byte(k)..")");
	end
	if farCorrect + farWrong == 5 then
		farCorrect = 0;
		farWrong = 0;
		if farCorrect == 5 then
			farPoint = farPoint -1;
		else
			farPoint = farPoint +1;
		end;
	end
	if nearCorrect + nearWrong == 5 then
		nearCorrect = 0;
		nearWrong = 0;
		if nearCorrect == 5 then
			nearPoint = nearPoint +1;
		else
			nearPoint = nearPoint -1;
		end;
	end
end;

farTrial = function (n)
	return n%2 == 0;
end

testNum = 0;
nextFrame = function ()
	testNum = testNum+1;
--	// Uncomment the following for a (rerendered) Stereogram version
--	local texture = Texture("Stereogram",
--		string.format(texbases[texIndexes[testNum]], replies[testNum], "l"),
--		string.format(texbases[texIndexes[testNum]], replies[testNum], "r"));
--	// Uncomment the following for a Random Dot version
	local texture = Texture("RandomDot", shapes[math.random(1, #shapes)]);
	texture:setMaxColor(8);
	texture:setExclusiveColor(1);
--	// Uncomment the following for a Pattern version
--	permuteTable(patterns); -- Pick two random patterns
--	permuteTable(shapes); -- Pick a random shape
--	local texture = Texture("Pattern", shapes[1], patterns[1], patterns[2]);
--	// End Comments
	texture:setStyle(replies[testNum]); -- Here the concave/convex status is set
	local pos = mountPoints[testNum];
	stereogramA:setTexture(texture);
	if farTrial(testNum) then
		-- Test far
		stereogramA:setPosition(pos[1], pos[2], farPoint);
	else
		-- Test near
		stereogramA:setPosition(pos[1], pos[2], nearPoint);
	end
	-- Separation at center
	local sep = statistics:paralaxAtPoint(pos[1]+1, pos[2]+1, pos[3]);
	local s = string.format("New Q: %s/%s (%0.2f, %0.2f, %0.2f), s=%0.4f deg",
		replies[testNum],shapes[1], pos[1]+1, pos[2]+1, pos[3], sep);
	Scene:log(s);
end
nextFrame();
