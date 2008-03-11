-- Execute the common parts of all scenes
dofile("res/scene.pilotCommon.lua");

Scene:log("Intro scene loaded");

-- Globals
maxColor = 8; -- Number of gray scales in rds
excColor = 1; -- Number of gray scales exclusive to fg/bg
convex = "convex";
concave = "concave";

-- Create two new Text Objects...
textObject1 = Object("Text");
textObject2 = Object("Text");

-- Scene list tokens
textToken = "text";		-- Changes text of an object
rdsToken = "rds";		-- Changes RDS properties
posToken = "position";	-- Changes the position
showToken = "show";		-- Shows (adds to the scene)
hideToken = "hide";		-- Hides (removes from the scene)
waitKeyToken = "waitKey";	-- Waits for a key
waitTimeToken = "waitTime";	-- Waits a number of seconds

-- Token evaluators
-- 1. Take a token
-- 2. do something
-- 3. return if execution is continued
tokenEval = {
-- { textToken, <object>, <string> }
[textToken] = function (token)
		token[2]:setText(token[3]);
		local s = string.format("New Text: %s", token[3]);
		Scene:log(s);
		return true;
	end;
-- { rdsToken, <object>, <pathtodepthmap>, <convex|concave> }
[rdsToken] = function (token)
		local texture = Texture("RandomDot", token[3]);
		texture:setMaxColor(maxColor);
		texture:setExclusiveColor(excColor);
		texture:setStyle(token[4]); -- Here the concave/convex status is set
		token[2]:setTexture(texture);
		local s = string.format("New RDS: %s | %s", token[3], token[4]);
		Scene:log(s);
		return true;
	end;
-- { posToken, <object>, {<x>, <y>, <z>} }
[posToken] = function (token)
		local pos = token[3];
		token[2]:setPosition(pos[1], pos[2], pos[3]);
		local sep = statistics:paralaxAtPoint(pos[1], pos[2], pos[3]);
		local s = string.format("New Pos: (%0.2f, %0.2f, %0.2f), s=%0.4f deg",
			pos[1], pos[2], pos[3], sep);
		Scene:log(s);
		return true;
	end;
-- { showToken, <object>}
[showToken] = function (token)
		Scene:addObject(token[2]);
		return true;
	end;
-- { hideToken, <object>}
[hideToken] = function (token)
		Scene:removeObject(token[2]);
		return true;
	end;
-- { waitKeyToken, <keylist> }
-- { waitKeyToken, <keylist>, <tokenlist correct> }
-- { waitKeyToken, <keylist>, <tokenlist correct>, <tokenlist incorrect> }
[waitKeyToken] = function (token)
		waitInput(token[2], token[3], token[4]);
		return false;
	end;
-- { waitTimeToken, <wait-seconds>, <tokenlist> }
[waitTimeToken] = function (token)

		return false;
	end;
}

waitingDataSuccess = nil;	-- The Data that is waiting
waitingDataFailure = nil;	-- The Data that is waiting

-- Wait for the given input key list
waitInput = function (keyList, sequenceSuccess, sequenceFailure)
	waitingDataSuccess = sequenceSuccess;
	waitingDataFailure = sequenceFailure;
	inputKeys = keyList;
	Scene:setEventListener("keyDown", parseInput);
end;

waitCallback = function (success)
	if success then
		Scene:setEventListener("keyDown", nil);
		Scene:setEventListener("update", nil);
		if waitingDataSuccess then
			executeTokenList(waitingDataSuccess);
		else
			executeTokenList();
		end;
	elseif waitingDataFailure then
		Scene:setEventListener("keyDown", nil);
		Scene:setEventListener("update", nil);
		executeTokenList(waitingDataFailure);
	end
end;

-- Execute a token list from it's current position
tokenStack = {};
executeTokenList = function (tokenList)
	local currentListObject;
	if not tokenList then
		-- Continue last "function"
		if #tokenStack > 0 then
			-- Get function
			currentListObject = tokenStack[#tokenStack];
		else
			-- No last function stored
			return;
		end;
	else
		-- Start a new "function"
		currentListObject = {
			["TokenList"] = tokenList;
			["TokenPos"] = 1;
			};
		table.insert(tokenStack, currentListObject);
	end;
	-- Get current token
	local token = currentListObject.TokenList[currentListObject.TokenPos];
	local continue = true;
	-- Execute tokens until wait point
	while continue do
		if token then
			-- Evaluate token
			continue = tokenEval[token[1]](token);
			-- Get next token
			currentListObject.TokenPos = currentListObject.TokenPos +1;
			token = currentListObject.TokenList[currentListObject.TokenPos];
		else
			-- TokenList end reached
			-- Remove current func
			table.remove(tokenStack);
			-- Continue with last func
			executeTokenList();
			return;
		end;
	end;
end

inputKeys = {};
parseInput = function (k)
	local id = string.byte(k)
	local name = Key[id];
	-- Search for the name in the input list
	if name then
		Scene:log("Input: "..name.." ("..id..")");
		for i, c in ipairs(inputKeys) do
			if c == name then
				-- Match
				Scene:log("Input Match");
				waitCallback(true);
				return;
			end;
		end
	else
		Scene:log("Input: - ("..id..")");
	end
	-- Search for the id in inputKeys
	for i, c in ipairs(inputKeys) do
		if c == id then
			-- Match
			Scene:log("Input Match");
			waitCallback(true);
			return;
		end;
	end
	-- If the list is empty, always succeed
	if #inputKeys == 0 then
		waitCallback(true);
	else
		waitCallback(false);
	end;
end;

Scene:setEventListener("quit", function (k) Scene:log("Exiting..."); end);

-- The scene list
scenes = {
	{textToken, textObject1, "Welcome!"};
	{textToken, textObject2, "Press <the any key> to continue"};
	{posToken, textObject1, {-2,1,0}};
	{posToken, textObject2, {-2,0.5,0}};
	{showToken, rectFloor};
	{showToken, rectCeil};
	{showToken, textObject1};
	{showToken, textObject2};
	{waitKeyToken, {}};

	{textToken, textObject1, "Focus on the random pattern"};
	{textToken, textObject2, "Press <the any key> to continue"};
	{showToken, stereogramA};
	{rdsToken, stereogramA, shapes[1], convex};
	{posToken, stereogramA, {-3, -2.5, 0}};
	{waitKeyToken, {}};

	{textToken, textObject1, "This target comes out of the screen"};
	{textToken, textObject2, "Press <down> for such targets"};
	{waitKeyToken, {"down"}};

	{rdsToken, stereogramA, shapes[2], concave};
	{posToken, stereogramA, {1, -2.5, 0}};
	{textToken, textObject1, "This target goes into the screen"};
	{textToken, textObject2, "Press <up> for such targets"};
	{waitKeyToken, {"up"}};

	{rdsToken, stereogramA, shapes[1], concave};
	{posToken, stereogramA, {-3, -2.5, 2}};
	{textToken, textObject1, "Here's some practice"};
	{textToken, textObject2, ""};
	{waitKeyToken, {"up"},
		{{textToken, textObject2, "Correct :)"};},
		{{textToken, textObject2, "Wrong :("};}
	};

	{rdsToken, stereogramA, shapes[1], convex};
	{posToken, stereogramA, {1, 0.5, -2}};
	{textToken, textObject1, "Here's more practice"};
	{waitKeyToken, {"down"},
		{{textToken, textObject2, "Correct :)"};},
		{{textToken, textObject2, "Wrong :("};}
	};

	{rdsToken, stereogramA, shapes[2], concave};
	{posToken, stereogramA, {1, 0.5, 2}};
	{textToken, textObject1, "Here's an other practice"};
	{waitKeyToken, {"up"},
		{{textToken, textObject2, "Correct :)"};},
		{{textToken, textObject2, "Wrong :("};}
	};

	{rdsToken, stereogramA, shapes[2], concave};
	{posToken, stereogramA, {1, 0.5, -9}};
	{textToken, textObject1, "Here's a hard one"};
	{waitKeyToken, {"up"},
		{{textToken, textObject2, "Correct :)"};},
		{{textToken, textObject2, "Wrong :("};}
	};

	{rdsToken, stereogramA, shapes[2], concave};
	{posToken, stereogramA, {1, 0.5, -18}};
	{textToken, textObject1, "Here's a harder one"};
	{waitKeyToken, {"up"},
		{{textToken, textObject2, "Correct :)"};},
		{{textToken, textObject2, "Wrong :("};}
	};

	{hideToken, rectFloor};
	{hideToken, rectCeil};
	{rdsToken, stereogramA, shapes[2], convex};
	{posToken, stereogramA, {1, -2.5, 0}};
	{textToken, textObject1, "And no floor"};
	{waitKeyToken, {"down"},
		{{textToken, textObject2, "Correct :)"};},
		{{textToken, textObject2, "Wrong :("};}
	};

	{rdsToken, stereogramA, shapes[2], convex};
	{posToken, stereogramA, {-3, -2.5, 7.5}};
	{textToken, textObject1, "Very close"};
	{waitKeyToken, {"down"},
		{{textToken, textObject2, "Correct :)"};},
		{{textToken, textObject2, "Wrong :("};}
	};

	{rdsToken, stereogramA, shapes[1], convex};
	{posToken, stereogramA, {1, -2.5, -18}};
	{textToken, textObject1, "Very far"};
	{waitKeyToken, {"down"},
		{{textToken, textObject2, "Correct :)"};},
		{{textToken, textObject2, "Wrong :("};}
	};

	{hideToken, stereogramA};
	{textToken, textObject1, "That's it"};
};

-- Start the Scene
executeTokenList(scenes);
