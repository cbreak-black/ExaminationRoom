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

	paralaxAtPoint = function (this, x, y, z)
		-- Separation in pixel
		local sep = Scene:getSeparationAtPoint(x, y, z);
		-- Field of view the screen takes up
		local fov =
			math.atan2(this.screenHeight*(1-this.eyePosition), this.viewingDistance) +
			math.atan2(this.screenHeight*(this.eyePosition), this.viewingDistance);
		fov = fov / math.pi * 180;
		-- FoV per Pixel (approximation)
		local vx, vy, vw, vh = Scene:getViewport();
		local fpp = fov / vh;
		-- Distance of a pixel
		local dpp = this.screenHeight / vh;
		if (dpp * sep > 0.065) then
			Scene:log(string.format("Warning: Separation %0.4f > 0.065m", dpp*sep));
		end;
		-- Asumes object is in camera focus and directly looked at
		return fpp * sep;
	end;

};
