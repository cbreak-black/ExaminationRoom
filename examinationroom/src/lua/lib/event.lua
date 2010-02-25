-- Easier wrappers for the basic events

-- Registers a function as update handler that calls a
-- callback after the given number of seconds
-- this requires EXCLUSIVE use of "update" events
setUpdateTimer = function (handleFunction, tLimit)
	if type(handleFunction) ~= "function" then
		error("Invalid argument 1 to setUpdateTimer, function expected", 2);
	end
	if type(tLimit) ~= "number" then
		error("Invalid argument 2 to setUpdateTimer, number expected", 2);
	end
	local tPassed = 0;
	Scene:setEventListener("update", function (tDelta)
		tPassed = tPassed + tDelta
		if tPassed >= tLimit then
			-- We're done, unregister
			Scene:setEventListener("update", nil);
			-- And call the handler (upvalue)
			handleFunction();
		end
	end)
end
