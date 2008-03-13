dofile("res/scene.pilotCommon.lua");

Scene:log("Bisect");

-- Permutes a table
local permuteTable = function (t)
	local n = #t;
	-- For every element
	for i = 1, n do
		-- randomly pick an other element to change with
		local j = math.random(i, n);
		t[i], t[j] = t[j], t[i];
	end
end;

-- The near/far limits, initial conditions
farPoint = -16;
farLimit = -90;
nearPoint = 4;
nearLimit = 9;

farCorrect = 0;
farWrong = 0;
nearCorrect = 0;
nearWrong = 0;
farPreviousCorrect = 0;
farPreviousPoint = 0;
nearPreviousCorrect = 0;
nearPreviousPoint = 0;

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
	-- After 5 tests for far
	if farCorrect + farWrong == 5 then
		-- and change the position
		if farCorrect >= 4 then
			if farPreviousPoint == farPoint then
				farPreviousCorrect =  farPreviousCorrect + farCorrect;
			elseif farPreviousPoint > farPoint then
				farPreviousPoint = farPoint;
				farPreviousCorrect = farCorrect;
			end
			farPoint = farPoint -1;
			if farPoint < farLimit then
				farPoint = farLimit;
				Scene:log("Far Limit reached!");
			end
		else
			farPoint = farPoint +1;
		end;
		-- reset
		farCorrect = 0;
		farWrong = 0;
		Scene:log("New far point: "..farPoint);
	end
	-- After 5 tests for near
	if nearCorrect + nearWrong == 5 then
		-- and change the position
		if nearCorrect >= 4 then
			if nearPreviousPoint == nearPoint then
				nearPreviousCorrect =  nearPreviousCorrect + nearCorrect;
			elseif nearPreviousPoint < nearPoint then
				nearPreviousPoint = nearPoint;
				nearPreviousCorrect = nearCorrect;
			end
			nearPoint = nearPoint +0.5;
			if nearPoint > nearLimit then
				nearPoint = nearLimit;
				Scene:log("Near Limit reached!");
			end
		else
			nearPoint = nearPoint -0.5;
		end;
		-- reset
		nearCorrect = 0;
		nearWrong = 0;
		Scene:log("New near point: "..nearPoint);
	end
	Scene:log("Correct Count: Near "..nearPreviousCorrect.." Far "..farPreviousCorrect);
	if nearPreviousCorrect > 12 and farPreviousCorrect > 12 then
		Scene:log("Mission Completed");
		os.exit(0);
	end
end;
Scene:setEventListener("keyDown", parseInput);

-- Return true if this is the trial number of a far target
-- (or false in case of a near target test)
farTrial = function (n)
	return n%2 == 0;
end

testNum = 0;
-- Advance the scene to the next frame
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
	local z;
	if farTrial(testNum) then
		-- Test far
		z = farPoint;
	else
		-- Test near
		z = nearPoint;
	end
	stereogramA:setPosition(pos[1], pos[2], z);
	-- Separation at center
	local sep = statistics:paralaxAtPoint(pos[1]+1, pos[2]+1, z);
	local s = string.format("New Q: %s/%s (%0.2f, %0.2f, %0.2f), s=%0.4f deg",
		replies[testNum],shapes[1], pos[1]+1, pos[2]+1, z, sep);
	Scene:log(s);
end
nextFrame();
