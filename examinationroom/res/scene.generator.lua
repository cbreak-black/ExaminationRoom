-- Create a list of positions and stores them persistently
dofile("res/persistence.lua");

-- 	-18, -6.5, 0, 4.25, 7.5
local depths = {
	-18, 0, 7.5 -- +- 3 degrees of parallax at 1m
--	-4.25, 0, 3.25 -- +-1 degree of parallax at 1m
--	-11.25, 0, 7 -- +- 2 degrees parallax at 1m
}
local xys = {
	{ -3, -2.5 };
	{ 1, -2.5};
	{ -3, 0.5};
	{ 1, 0.5};
}
local mountPoints = {}
for j = 1, #depths do
	for i = 1, #xys do
		table.insert(mountPoints, {xys[i][1], xys[i][2], depths[j]});
	end
end

-- Possible replies
local replies = {};
for i = 1, #mountPoints/2 do
	table.insert(replies, "convex");
	table.insert(replies, "concave");
end

local permuteTable = function (t)
	local n = #t;
	for i = 1, n do
		local j = math.random(i, n);
		t[i], t[j] = t[j], t[i];
	end
end;

local checkOverlap = function (t, pold)
	for i, p in ipairs(t) do
		if p[1] == pold[1] and p[2] == pold[2] then
			return true;
		end;
		pold = p;
	end
	return false;
end

local testPositions = {};
local testReplies = {};

local lastPoint = {0,0,0};
for i = 1, 100 do -- 100 cycles
	permuteTable(mountPoints);
	while checkOverlap(mountPoints, lastPoint) do
		permuteTable(mountPoints);
	end
	permuteTable(replies);
	for j = 1, #mountPoints do
		table.insert(testPositions, mountPoints[j]);
		table.insert(testReplies, replies[j]);
	end
	lastPoint = mountPoints[#mountPoints];
end;

persistence.store("res/scene.pilot.in.lua", testPositions, testReplies);
