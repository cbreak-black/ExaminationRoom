/*
 *  scene.h
 *  ExaminationRoom
 *
 *  Created by CBreak on 18.01.08.
 *  Copyright 2008 Gerhard Roethlin. All rights reserved.
 *
 */

#ifndef SCENE_H
#define SCENE_H

#include "objects/container.h"

#include "vec.h"

namespace Examination
{
	class Object;
	class GLWidget;
	class Camera;
	class Program;

/**
This class represents the scene. It contains references to objects.
While it is itself an object, it should NOT be used as such.

\author Gerhard Roethlin
*/
class Scene : public Container
{

public:
	/**
	Creator of scenes.
	*/
    Scene();

	/**
	Creates a clone of scene s.
	 \param s	The scene that is to be cloned
	*/
	Scene(const Scene & s);

	/**
	Destructor of scenes. Also deletes all contained objects.
	*/
	~Scene();

public: // Cloning
	/**
	Returns a newly allocated clone of this object.
	 \return a newly created copy of this object
	*/
	virtual ObjectPtr clone() const;

public: // Camera
	void setCamera(std::tr1::shared_ptr<Camera> camera);
	std::tr1::shared_ptr<Camera> camera() const;

public: // BG Color
	/**
	Sets the background color of this scene. This is a wrapper for setBackgroundColor(Tool::Vec4f c)
	 \param r	Red component
	 \param g	Green component
	 \param b	Blue component
	 \param a	Alpha component
	*/
	void setBackgroundColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

	/**
	Sets the background color of this scene.
	 \param c	A color vector. Each component should be in the range of [0.0f, 1.0f]
	*/
	void setBackgroundColor(Tool::Color4 c);

	/**
	Returns the current background color of this scene.
	 \return The background color of this scene as vector. Each component should be in the range of [0.0f, 1.0f]
	*/
	Tool::Color4 backgroundColor() const;

public: // Serialisation
	/**
	Returns the name of the class of this object. This can be used in LUA
	object creation.
	 \return The name of this object's class as c++ string
	*/
	virtual std::string className() const;

	/**
	Writes the LUA commands to set parameters of this object to the output stream.
	 \param outStream	A stream that accepts writing
	*/
	virtual std::string toLua(std::ostream & outStream) const;

	/**
	The scene object is not constructed in LUA, so this method does nothing.
	 \param outStream	A stream that accepts writing
	*/
	virtual std::string toLuaCreate(std::ostream & outStream) const;

protected: // Parameter Dialog
	virtual std::tr1::shared_ptr<ParameterObject> createDialog();

public: // Signals: Called by sub objects
	/**
	Called by objects that are about to change. Changes include attributes and data.
	 \param obj	The object that is about to change
	*/
	void objectWillChange(const Object * obj) const;

	/**
	Called by objects that have changed. Changes include attributes and data.
	 \param obj	The object that has changed
	*/
	void objectDidChange(const Object * obj) const;

	/**
	Called by containers that are about to change. Changes include the adding and
	removing of objects, the reordering.
	 \param cont	The container that is about to change
	*/
	void layoutWillChange(const Container * cont) const;

	/**
	Called by containers that are have changed. Changes include the adding and
	removing of objects, the reordering.
	 \param cont	The container that has changed
	*/
	void layoutDidChange(const Container * cont) const;

public: // Signals: Observed by clients
	/**
	Adds a callback to be informed when an object did change.
	 \param cb	A callback functional/functor thing
	*/
	void addCallbackObjectWillChange(const SignalCallbackType & cb);
	/**
	Adds a callback to be informed when an object did change.
	 \param cb	A callback functional/functor thing
	*/
	void addCallbackObjectDidChange(const SignalCallbackType & cb);
	/**
		Adds a callback to be informed when the scene layout will change.
	 \param cb	A callback functional/functor thing
	*/
	void addCallbackLayoutWillChange(const SignalCallbackType & cb);
	/**
	Adds a callback to be informed when the scene layout did change.
	 \param cb	A callback functional/functor thing
	*/
	void addCallbackLayoutDidChange(const SignalCallbackType & cb);
	/**
	Removes a callback that was added to be informed when an object will change.
	 \warning	Not tested
	 \param target	A reference_wrapper to a target
	*/
	template <typename T>
	void removeCallbackObjectWillChange(const T * target)
	{
		removeCallbacks(objWillChangeCallbacks_, target);
	}
	/**
	Removes a callback that was added to be informed when an object did change.
	 \warning	Not tested
	 \param target	A reference_wrapper to a target
	 */
	template <typename T>
	void removeCallbackObjectDidChange(const T * target)
	{
		removeCallbacks(objDidChangeCallbacks_, target);
	}
	/**
	Removes a callback that was added to be informed when the scene layout will change.
	 \warning	Not tested
	 \param target	A reference_wrapper to a target
	*/
	template <typename T>
	void removeCallbackLayoutWillChange(const T * target)
	{
		removeCallbacks(layoutWillChangeCallbacks_, target);
	}
	/**
	Removes a callback that was added to be informed when the scene layout did change.
	 \warning	Not tested
	 \param target	A reference_wrapper to a target
	*/
	template <typename T>
	void removeCallbackLayoutDidChange(const T * target)
	{
		removeCallbacks(layoutDidChangeCallbacks_, target);
	}

private: // Signals: Internal data and methods
	// Lists with callbacks
	std::list<SignalCallbackType> objWillChangeCallbacks_;
	std::list<SignalCallbackType> objDidChangeCallbacks_;
	std::list<SignalCallbackType> layoutWillChangeCallbacks_;
	std::list<SignalCallbackType> layoutDidChangeCallbacks_;

protected:
	virtual std::tr1::shared_ptr<Container> getParent();
	virtual std::tr1::shared_ptr<Scene> getScene();

private:
	std::tr1::shared_ptr<Camera> camera_;

public: // Linking
	/**
	Returns the associated program if it exists, an invalid poionter otherwise.
	 \return the associated program object
	*/
	std::tr1::shared_ptr<Program> program() const;

	/**
	Sets the associated program. It is internaly stored as weak_ptr, so the
	no posession rights are asumed.
	 \param program the shared_ptr to the program that contains this scene
	*/
	void setProgram(std::tr1::shared_ptr<Program> program);

	/**
	Returns a pointer to the scene this object is which is itself. It is equivalent
	to the sharedPtr() method.
	 \return a pointer to the scene of this object
	*/
	virtual std::tr1::shared_ptr<Scene> scene();
	virtual std::tr1::shared_ptr<Scene const> scene() const;

	/**
	Sets the scene pointer of this object. Since this method is pointless for
	the scene itself, it does nothing.
	 \param s	Scene pointer
	*/
	virtual void setScene(std::tr1::shared_ptr<Scene> s);

private: // Linking
	std::tr1::weak_ptr<Program> program_;
};

}

#endif
