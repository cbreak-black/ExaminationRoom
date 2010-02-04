--[[ Math Extensions ]]
-- math.clamp(v, min, max): returns the value v clamped to the range min..max

--[[ License: MIT, Copyright (c) 2010 Gerhard Roethlin ]]

-- math.clamp
math.clamp = function (v, min, max)
	if v > max then
		return max;
	elseif v < min then
		return min;
	else
		return v;
	end;
end;
