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
	Mesh();

public:
	/**
	Loads the given mesh file.
	 \param path	A path to an obj. file
	*/
	bool loadMesh(std::string path);

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

private: // Parser Callbacks
	void info_callback(const std::string& filename, std::size_t line_number, const std::string& message);
	void warning_callback(const std::string& filename, std::size_t line_number, const std::string& message);
	void error_callback(const std::string& filename, std::size_t line_number, const std::string& message);

	// Callbacks for primitives
	void geometric_vertex_callback(float x, float y, float z);
	void texture_vertex_callback(float u, float v);
	void vertex_normal_callback(float x, float y, float z);

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
	float scaleFactor_;
	int displayList_;
};

}

#endif
