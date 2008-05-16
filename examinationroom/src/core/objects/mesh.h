/*
 *  mesh.h
 *  ExaminationRoom
 *
 *  Created by cbreak on 15.03.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MESH_H
#define MESH_H

#include "object.h"

#include "vec.h"

#include <vector>

#include "parser/obj/obj.hpp"

namespace Examination
{

/**
This class is a helper object. It represents a triangle consisting of three
indexes for vertexes, three indexes for textures, and three indexes for normals.
It is a value type, and can easily be copied.
 \author Gerhard Roethlin
 \ingroup Objects
*/
class Triangle
{
	typedef enum {vertex = 0, texture = 1, normal = 2};
public:
	Triangle(int a, int b, int c);
	void setTexture(int a, int b, int c);
	void setNormal(int a, int b, int c);
public:
	void draw(const std::vector<Tool::Vec3f>& vertexes,
			  const std::vector<Tool::Vec2f>& textures,
			  const std::vector<Tool::Vec3f>& normals) const;
private:
	Tool::Vec3<int> vertexes_;
	Tool::Vec3<int> textures_;
	Tool::Vec3<int> normals_;
	int type_;
};

/**
This class represents a mesh object. It can draw itself in space.
*/
class Mesh : public Object
{
public:
	/**
	Creates a mesh with default parameters.
	The default name is "Mesh", the default scale factor is 1.
	No mesh data is loaded.
	 \warning Don't create instances of this class directly, use Object::Create()
	 \see Object::Create()
	*/
	Mesh();

public:
	/**
	Loads the given mesh file.
	 \param path	A path to an obj. file
	*/
	bool loadMesh(std::string path);

	/**
	Returns the path of the currently loaded mesh, if available.
	 \return The path to the currently loaded mesh, or an empty string
	*/
	std::string getMeshPath() const;

	/**
	Clears the stored mesh.
	*/
	void clearMesh();

protected:
	/**
	Invalidates the display list used for fast drawing, and deletes associated structures.
	*/
	void invalidateCache();

	/**
	Rebuilds the display list  used for fast drawing.
	*/
	void rebuildCache();

public:
	/**
	Draw the mesh by calling the display list after setup.
	It rebuilds the display list if needed. Not implemented.
	 \param dest	Destination widget that contains the current context
	*/
	//virtual void draw(GLWidget * dest);

	/**
	Draw the mesh by calling the display list after setup.
	 \param dest	Destination widget that contains the current context
	*/
	virtual void draw(GLWidget * dest) const;

public:
	/**
	Returns the mesh scale factor. The mesh gets scaled with this factor.
	 \return the scale factor
	*/
	float scaleFactor() const;

	/**
	Sets the mesh scale factor.
	 \param scaleFactor	The new scale factor
	*/
	void setScaleFactor(float scaleFactor);

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

public: // LUA API
	static void registerLuaApi(luabridge::module * m);

protected: // Parameter Dialog
	virtual std::tr1::shared_ptr<ParameterObject> createDialog();

private: // Parser Callbacks
	void info_callback(const std::string& filename, std::size_t line_number, const std::string& message);
	void warning_callback(const std::string& filename, std::size_t line_number, const std::string& message);
	void error_callback(const std::string& filename, std::size_t line_number, const std::string& message);

	// Callbacks for primitives
	void geometric_vertex_callback(obj::float_type x, obj::float_type y, obj::float_type z);
	void texture_vertex_callback(obj::float_type u, obj::float_type v);
	void vertex_normal_callback(obj::float_type x, obj::float_type y, obj::float_type z);

	// Callbacks for faces
	void triangular_face_geometric_vertices_callback(int v1, int v2, int v3);
	void triangular_face_geometric_vertices_texture_vertices_callback(const obj::index_2_tuple_type& v1_vt1,
																	  const obj::index_2_tuple_type& v2_vt2,
																	  const obj::index_2_tuple_type& v3_vt3);
	void triangular_face_geometric_vertices_vertex_normals_callback(const obj::index_2_tuple_type& v1_vn1,
																	const obj::index_2_tuple_type& v2_vn2,
																	const obj::index_2_tuple_type& v3_vn3);
	void triangular_face_geometric_vertices_texture_vertices_vertex_normals_callback(const obj::index_3_tuple_type& v1_vt1_vn1,
																					 const obj::index_3_tuple_type& v2_vt2_vn2,
																					 const obj::index_3_tuple_type& v3_vt3_vn3);

private:
	std::vector<Tool::Point> vertices_;
	std::vector<Tool::Vec3f> normals_;
	std::vector<Tool::Vec2f> textureCoordinates_;
	std::vector<Triangle> triangles_;
	std::string meshPath_;
	float scaleFactor_;
	int displayList_;

public: // Meta
	static const char * className_;
};

}

#endif
