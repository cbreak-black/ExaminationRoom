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

farPointList = {
	0, -1, -2, -3,
	-4, -5, -6,
	-8, -10, -12,
	-15, -18, -21, -24,
	-28, -32, -36, -40,
	-45, -50, -55, -60, -65, -70,
	-80, -90
}
farPoint = 5;
farLimit = #farPointList;
nearPoint = 1;
nearLimit = 9;

farCorrect = 2;
farWrong = 0;
nearCorrect = 0;
nearWrong = 0;
farPreviousCorrect = 0;
farPreviousPoint = 0;
nearPreviousCorrect = 0;
nearPreviousPoint = 0;

Dir.space = "skipped";

voice = "Victoria"; -- Bruce, Vicki, Trindroids, Zarvox

parseInput = function (k)
	local d = Key[string.byte(k)];
	if d then
		if Dir[d] then
			d = Dir[d];
			if d == replies[testNum] then
				--os.execute("say -v Victoria Correct.");
				Scene:log("Input Correct: "..d);
				if farTrial(testNum) then
					farCorrect = farCorrect + 1;
				else
					nearCorrect = nearCorrect + 1;
				end
			elseif d == "skipped" then
				os.execute("say -v "..voice.." Skipped. &");
			else
				os.execute("say -v "..voice.." Wrong. &");
				Scene:log("Input Incorrect: "..d);
				if farTrial(testNum) then
					farWrong = farWrong + 1;
				else
					nearWrong = nearWrong + 1;
				end
			end;
			Scene:log("Correct Count: Near "..nearPreviousCorrect.." Far "..farPreviousCorrect);

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
		if farCorrect >= 5 then
			if farPreviousPoint == farPoint then
				farPreviousCorrect =  farPreviousCorrect + farCorrect;
			elseif farPreviousPoint > farPoint then
				farPreviousPoint = farPoint;
				farPreviousCorrect = farCorrect;
			end
			farPoint = farPoint +1;
			if farPoint > farLimit then
				farPoint = farLimit;
				Scene:log("Far Limit reached!");
			end
			os.execute(string.format("say -v %s Progressing to far point %2.0f &", voice, farPointList[farPoint]));
		else
			farPoint = farPoint -1;
			os.execute(string.format("say -v %s Back to far point %2.0f &", voice, farPointList[farPoint]));
		end;
		-- reset
		farCorrect = 0;
		farWrong = 0;
		Scene:log("New far point: "..farPointList[farPoint]);
	end
	-- After 5 tests for near
	if nearCorrect + nearWrong == 5 then
		-- and change the position
		if nearCorrect >= 5 then
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
			os.execute(string.format("say -v %s Progressing to near point %2.1f &", voice, nearPoint));
		else
			nearPoint = nearPoint -0.5;
			os.execute(string.format("say -v %s Back to near point %2.1f &", voice, nearPoint));
		end;
		-- reset
		nearCorrect = 0;
		nearWrong = 0;
		Scene:log("New near point: "..nearPoint);
	end

	if nearPreviousCorrect > 12 and farPreviousCorrect > 12 then
		Scene:log("Mission Completed");
		os.execute("say -v Zarvox GAME OVER");
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
		z = farPointList[farPoint];
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
