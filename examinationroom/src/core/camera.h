/*
 *  camera.h
 *  ExaminationRoom
 *
 *  Created by CBreak on 29.01.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef CAMERA_H
#define CAMERA_H

#include "vec.h"
#include "glwidget.h"

namespace Tool
{
	class ScreenProject;
}

namespace Examination
{

/**
Represents a stereoscopic camera pair. It can load itself into the OpenGL
projection matrix stack.
 \author Gerhard Roethlin
*/
class Camera
{
public: // Enums
	/**
	The types of projections that are supported
	*/
	typedef enum
	{
		Perspective,	/**< Perspective projection, parallel cameras */
		Parallel,		/**< Parallel projection, converged cameras */
		Screen			/**< Screen space, separation: camera separation per unit in z, like parallel */
	}	Type;

public:
	/**
	Creates a camera with initial values.
	*/
	Camera();
	
	/**
	Creates a camera at the given position with the given viewing direction.
	 \param pos	The position of the camera in world space.
	 \param dir	The direction of the camera in world space.
	 \param up	The up vector of the camera in world space.
	*/
	Camera(Tool::Point pos, Tool::Vector dir, Tool::Vector up);

	/**
	Creates a camera as clone from the passed camera c.
	 \param c	A camera
	*/
	Camera(const Camera & c);

	/**
	Destroys the camera
	*/
	~Camera();

public:
	/**
	Loads the camera for the side of the view into the OpenGL matrix stack.
	 \param dest	Destination view, used to determine left or right
	*/
	void loadMatrix(GLWidget * dest);

	/**
	Loads the camera with that offset into the OpenGL matrix stack.
	 \param offsetCamera the distance by which the camera is offset
	 */
	void loadMatrix(float offsetCamera);

	/**
	Loads the camera and prepares the screen projection objects for both sides.
	*/
	void preLoadMatrix();

public:
	/**
	Sets the position of the camera.
	 \see position()
	 \param pos	New position vector
	*/
	void setPosition(Tool::Point pos);

	/**
	Sets the direction of the camera.
	 \see direction()
	 \param dir	New direction vector
	*/
	void setDirection(Tool::Vector dir);

	/**
	Sets the up vector of the camera.
	The up vector decides which way of the picture points up.
	 \warning	Make sure that this vector does not lie on the direction vector
	 \see up()
	 \param up	The new up vector
	*/
	void setUp(Tool::Vector up);

	/**
	Sets the separation of the cameras in the stereo set
	 \see separation()
	 \param s	Camera Separation
	*/
	void setSeparation(float s);

	/**
	Sets the field of view of the camera. The default is 50.
	In parallel mode, this is used to set the size of the image.
	 \see fieldOfView()
	 \param fov	Field of view in degrees
	*/
	void setFieldOfView(float fov);

	/**
	Sets the paralax plane distance.
	 \see paralaxPlane()
	 \param dist	Distance of the paralax plane to the camera
	*/
	void setParalaxPlane(float dist);

public:
	/**
	Returns the position of the camera.
	 \see setPosition()
	 \return the position of the camera
	*/
	Tool::Point position() const;

	/**
	Returns the direction of the camera.
	 \see setDirection()
	 \return the direction of the camera
	*/
	Tool::Vector direction() const;

	/**
	Returns the up vector of the camera.
	 \see setUp()
	 \return the up vector of the camera
	*/
	Tool::Vector up() const;

	/**
	Returns the separation of the cameras
	 \see setSeparation()
	 \return the separation of the cameras
	*/
	float separation() const;

	/**
	Returns the field of view of the cameras.
	In parallel mode, this is used to set the size of the image. The size of objects
	at the paralax plane are held constant when switching from parallel to perspective
	or vice versa.
	 \return the field of view of the cameras in degrees
	*/
	float fieldOfView() const;

	/**
	Returns the paralax plane distance.
	 \see setParalaxPlane()
	 \return the paralax plane distance
	*/
	float paralaxPlane() const;

public: // Type
	/**
	Returns the camera type.
	 \see Camera::Type
	 \return the camera type
	*/
	Camera::Type type() const;

	/**
	Sets the camera type.
	 \see Camera::Type
	 \param t	The new camera type
	*/
	void setType(Camera::Type t);

public:
	/**
	Calculates and returns the size on screen in pixel of one unit at a given distance.
	Note that not the distance of the object itself to the camera, but the distance of
	the plane the object lies on matters.
	 \param d	Distance of line to camera (projected on camera viewing direction)
	 \return size of a line of length 1 at distance d on screen in pixel
	*/
	float unitScreenSize(float d) const;

	/**
	Calculates and returns the size on screen in pixel of one unit at a given distance.
	 \param p	Position of the unit measurement
	 \return size of a line of length 1 at position p on screen in pixel
	*/
	float unitScreenSize(Tool::Point p) const;

	/**
	Calculates the seperation of an object on screen.
	 \param p	Position of the separation measurement
	 \return separation on screen of object at position p from camera in pixel.
	*/
	float separationAtPoint(Tool::Point p) const;

public:
	/**
	Returns a pre-loaded screenProject object for the queried side.
	 \return a pre-loaded screenProject object for the queried side.
	*/
	Tool::ScreenProject * screenProject(GLWidget::Side s) const;

public: // Serialisation
	/**
	Writes the LUA commands to set parameters of the camera to the output stream.
	The parameters are set on a camera with the passed name, it is considered to
	be a lua variable containing a camera.
	 \param outStream	A stream that accepts writing
	 \param name		The variable name of the camera that is to be set
	*/
	std::string toLua(std::ostream & outStream, const std::string & name) const;

private:
	Tool::Point		pos_;
	Tool::Vector	dir_;
	Tool::Vector	up_;
	float			sep_;
	float			fov_;
	float			ppd_;
	Tool::ScreenProject *	spL_;
	Tool::ScreenProject *	spR_;
	Camera::Type	type_;
};

typedef std::tr1::shared_ptr<Camera> CameraPtr;

}
#endif
