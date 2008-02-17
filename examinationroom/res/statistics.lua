Scene:log("Statistics Library loaded");

statistics =
{
	viewingDistance = 1;	-- Distance to screen in meters
	screenHeight = 1;		-- Height of screen in meters
	eyePosition = 0.5;		-- Relative height of eyes (1= upper screen border, 0=lower screen border)

	setViewingProperties = function (this, dist, height, eyePos)
		this.viewingDistance = dist;
		this.screenHeight = height;
		if (eyePos == nil) then
			this.eyePosition = 0.5;
		else
			this.eyePosition = eyePos;
		end;
	end;

	separationAtPoint = function (this, x, y, z)
		-- Separation in units
		local sep = Scene:getSeparationAtPoint(x, y, z);
		Scene:log("sep: "..sep);
		-- Field of view the screen takes up
		local fov =
			math.atan2(this.screenHeight*(1-this.eyePosition), this.viewingDistance) +
			math.atan2(this.screenHeight*(this.eyePosition), this.viewingDistance);
		fov = fov / math.pi * 180;
		-- Height of screen in units
		local suh = math.tan(Scene:getCameraFoV()/180*math.pi/2) *
			Scene:getCameraParalaxPlane() * 2;
		-- FoV per Unit
		local fpu = fov / suh;
		-- Asumes object is in camera focus and directly looked at
		return fpu * sep;
	end;

};
