-- Internal Helpers

--[[ Provides ]]

--[[ StringWriter ]]
-- StringWriter instances are a drop-in replacement for files to write to a string instead a file. They behave like a file created with io.open(<filename>, "w").
-- StringWriter(): Creates and returns a new string writer instance
-- sw:close():	Does nothing
-- sw:flush():	Does nothing
-- sw:lines():	Does nothing
-- sw:read():	Does nothing
-- sw:seek(base, offset):	Seeks as normal files would, final position gets clamped to string length
-- sw:setvbuf():	Does nothing
-- sw:write(...):	Writes the value of it's arguments to a string. Arguments must be strings or numbers. Other values are converted with tostring().
-- sw:get():	Returns the internal string (stored in sw.string_)

--[[ Math Extensions ]]
-- math.clamp(v, min, max): returns the value v clamped to the range min..max

--[[ License: MIT (see bottom) ]]

-- Fairly unique IDentifier (can still be faked though)
local StringWriter_ID = {"StringWriter Metatable Access private"};

-- Does nothing
local voidFunc = function () end;

-- Checks if a given thing is a StringWriter (can be tricked though)
local StringWriter_Check = function (sw)
	return
		getmetatable(sw) == StringWriter_ID and
		type(sw.string_) == "string" and
		type(sw.pos_) == "number"
end

-- Methods for string writers
local StringWriter_Methods = {
	["close"] = voidFunc;
	["flush"] = voidFunc;
	["lines"] = voidFunc;
	["read"] = voidFunc;
	["seek"] = function (this, base, offset)
			-- Only act on StringWriters
			if not StringWriter_Check(this) then
				return nil, "Invalid StringWriter";
			end;
			-- Default offset
			if type(base) == "number" then
				offset = base; -- Not done in file, but reasonable
			else
				offset = offset or 0;
			end
			-- Set position and return it
			if base == "set" then
				this.pos_ = math.clamp(offset,0, #this.string_);
			elseif base == "end" then
				this.pos_ = math.clamp(#this.string_+offset,0, #this.string_);
			else -- "cur"
				this.pos_ = math.clamp(this.pos_+offset,0, #this.string_);
			end
			return this.pos_;
		end;
	["setvbuf"] = voidFunc;
	["write"] = function (this, ...)
			-- Only act on StringWriters
			if not StringWriter_Check(this) then return end;
			-- Concat all arguments (assuming they are valid)
			local s = table.concat({...});
			-- Concat argument string with current string
			if this.pos_ == -1 or this.pos_ == #this.string_ then
				this.string_ = this.string_ .. s;
			else
				this.string_ = string.sub(this.string_, 1, this.pos_)
					.. s .. string.sub(this.string_, this.pos_+1+#s, -1);
			end
			-- Update position
			if this.pos_ >= 0 then
				this.pos_ = this.pos_ + #s;
			end;
		end;
	["get"] = function (this)
			if not StringWriter_Check(this) then
				return nil, "Invalid StringWriter";
			else
				return this.string_;
			end;
		end;
}

-- MetaTable for string writers
local StringWriter_Meta = {
	["__index"] = StringWriter_Methods;
	["__newindex"] = function ()
			-- Don't allow setting values
		end;
	["__metatable"] = StringWriter_ID;
	["__tostring"] = StringWriter_Methods.get;
}

-- StringWriter factory
StringWriter = function ()
	local sw = {
		string_	= "";
		pos_ 	= 0;
	}
	setmetatable(sw, StringWriter_Meta);
	return sw;
end

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

--[[
 Copyright (c) 2010 Gerhard Roethlin

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
]]
