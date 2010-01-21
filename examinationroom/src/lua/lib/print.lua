-- Internal print library

-- Provides:
-- format(item): Formats item suitable for logging, returns as string
-- print(...): Formats all passed items and prints them to the log via Scene

--[[ Internal methods ]]
local formaters =
{
	["nil"] = function (item, level)
			return "nil";
		end;
	["number"] = function (item, level)
			return tostring(item);
		end;
	["string"] = function (item, level)
			return string.format("%q", item);
		end;
	["boolean"] = function (item, level)
			if item then
				return "true";
			else
				return "false";
			end
		end;
	["table"] = function (item, level, history)
			history = history or {};
			if history[item] then
				return "-- known "..tostring(item);
			else
				history[item] = true;
				local s = "{ -- "..tostring(item).."\n";
				for k, v in pairs(item) do
					s = s..string.rep("\t",level+1);
					s = s.."[";
					s = s..formatItem(k, level+1, history);
					s = s.."] = ";
					s = s..formatItem(v, level+1, history);
					s = s..";\n";
				end
				s = s..string.rep("\t",level);
				s = s.."}";
				return s;
			end
		end;
	["function"] = function (item, level)
			return tostring(item);
		end;
	["thread"] = function (item, level)
			return tostring(item);
		end;
	["userdata"] = function (item, level)
			return tostring(item);
		end;
}

-- Formats an arbitrary value as string
formatItem = function (item, level, ...)
	level = level or 0;
	return formaters[type(item)](item, level, ...);
end

--[[ Public methods ]]
format = function (item)
	return formatItem(item, 0);
end

print = function (...)
	local narg = select("#", ...);
	local s = "no values";
	if narg > 0 then
		s = format(select(1,...), 0);
		for i = 2, narg do
			s = s..", "..format(select(i,...), 0);
		end
	end
	Scene:log(s);
end
