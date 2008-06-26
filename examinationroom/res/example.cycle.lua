do -- cam
local cam = Scene:camera();
cam:setType("Perspective");
cam:setPosition({0, 0, 25});
cam:setDirection({0, 0, -1});
cam:setUp({0, 1, 0});
cam:setSeparation(0.15);
cam:setFieldOfView(18);
cam:setParalaxPlane(25);
end --cam
Scene:setBackgroundColor(127, 127, 127, 255);
-- Start sub-objects of Scene

-- Create marker of type Rectangle
marker = Rectangle();
marker:setName("marker");
marker:setPosition({0, 0, 0});
marker:setColor({1, 1, 0, 1});
marker:setWireframe(true);
marker:setShown(false);
marker:setDrawPriority(0);
marker:setSubdivision(0);
marker:setTexCoords(0, 0, 0, 1, 1, 0, 1, 1);
marker:setDirA({2, 0, 0});
marker:setDirB({0, 2, 0});
Scene:addObject(marker);

-- Create floor of type Rectangle
floor = Rectangle();
floor:setName("floor");
floor:setPosition({-3, -2, -18.5});
floor:setColor({1, 1, 1, 1});
floor:setWireframe(false);
floor:setShown(true);
floor:setDrawPriority(0);
do -- texture
local tex = Texture("res/checkerboard.png");
tex:setZoom(1, 1);
floor:setTexture(tex);
end -- texture
floor:setSubdivision(2);
floor:setTexCoords(0, 0, 0, 26, 6, 0, 6, 26);
floor:setDirA({6, 0, 0});
floor:setDirB({0, 0, 26});
Scene:addObject(floor);

-- Create pp1 of type Pixelplane
pp1 = Pixelplane();
pp1:setName("pp1");
pp1:setPosition({0, 0, 0});
pp1:setColor({1, 1, 1, 1});
pp1:setWireframe(false);
pp1:setShown(false);
pp1:setDrawPriority(0);
pp1:setAutoResize(false);
pp1:setSize(2, 2);
pp1:setZoom(1, 1);
Scene:addObject(pp1);

-- Create pp0 of type Pixelplane
pp0 = Pixelplane();
pp0:setName("pp0");
pp0:setPosition({0, 0, 0});
pp0:setColor({1, 1, 1, 1});
pp0:setWireframe(false);
pp0:setShown(false);
pp0:setDrawPriority(0);
pp0:setAutoResize(false);
pp0:setSize(2, 2);
pp0:setZoom(1, 1);
Scene:addObject(pp0);
-- End sub-objects of Scene

-- Loaded Files
Scene:loadLua("res/statistics.lua");
Scene:loadLua("res/questions.lua");
Scene:loadLua("res/example.cycle.code.lua");
