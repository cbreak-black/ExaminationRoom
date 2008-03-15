/*
 *  mesh.cpp
 *  ExaminationRoom
 *
 *  Created by cbreak on 15.03.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "mesh.h"

#include "parser/obj.hpp"
#include <string>
#include <iostream>

#include "glwidget.h"

namespace Examination
{
	using namespace Tool;

// Triangle
Triangle::Triangle(int a, int b, int c)
{
	vertexes_ = Vec3<int>(a, b, c);
	type_ = vertex;
}

void Triangle::setTexture(int a, int b, int c)
{
	textures_ = Vec3<int>(a, b, c);
	type_ |= texture;
}

void Triangle::setNormal(int a, int b, int c)
{
	normals_ = Vec3<int>(a, b, c);
	type_ |= normal;
}

void Triangle::draw(const std::vector<Tool::Vec3f>& vertexes,
					const std::vector<Tool::Vec2f>& textures,
					const std::vector<Tool::Vec3f>& normals) const
{
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 3; i++)
	{
		if (type_ & normal == normal) // normals are set
		{
			// Get the normal index for vertex i
			// look it up in the normal vectors vector
			// pass it to OpenGL
			glNormal3fv(normals[normals_[i]].vec);
		}
		if (type_ & texture == texture) // texture coordinates are set
		{
			// Get the texture index for vertex i
			// look it up in the texture coordinate vector
			// pass it to OpenGL
			glTexCoord2fv(textures[textures_[i]].vec);
		}
		// Get the vertex index for vertex i
		// look it up in the vertex position vector
		// pass it to OpenGL
		glVertex3fv(vertexes[vertexes_[i]].vec);
	}
	glEnd();
}

// Mesh
Mesh::Mesh()
{
}

void voidFunc()
{
}

void Mesh::loadMesh(std::string path)
{
	using namespace std::tr1::placeholders;

	obj::obj_parser::flags_type obj_parser_flags = 0;
	obj_parser_flags |= obj::obj_parser::triangulate_faces;
	obj_parser_flags |= obj::obj_parser::translate_negative_indices;

	obj::obj_parser obj_parser(obj_parser_flags);
	obj_parser.info_callback(std::tr1::bind(&Mesh::info_callback, this, path, _1, _2));
	obj_parser.warning_callback(std::tr1::bind(&Mesh::warning_callback, this, path, _1, _2));
	obj_parser.error_callback(std::tr1::bind(&Mesh::error_callback, this, path, _1, _2));

	obj_parser.geometric_vertex_callback(std::tr1::bind(&Mesh::geometric_vertex_callback, this, _1, _2, _3));
	obj_parser.texture_vertex_callback(std::tr1::bind(&Mesh::texture_vertex_callback, this, _1, _2));
	obj_parser.vertex_normal_callback(std::tr1::bind(&Mesh::vertex_normal_callback, this, _1, _2, _3));
	
	obj_parser.face_callbacks(
		std::tr1::bind(&Mesh::triangular_face_geometric_vertices_callback, this, _1, _2, _3),
		std::tr1::bind(&Mesh::triangular_face_geometric_vertices_texture_vertices_callback, this, _1, _2, _3),
		std::tr1::bind(&Mesh::triangular_face_geometric_vertices_vertex_normals_callback, this, _1, _2, _3),
		std::tr1::bind(&Mesh::triangular_face_geometric_vertices_texture_vertices_vertex_normals_callback, this, _1, _2, _3),
		// Unused
		obj::obj_parser::quadrilateral_face_geometric_vertices_callback_type(),
		obj::obj_parser::quadrilateral_face_geometric_vertices_texture_vertices_callback_type(),
		obj::obj_parser::quadrilateral_face_geometric_vertices_vertex_normals_callback_type(),
		obj::obj_parser::quadrilateral_face_geometric_vertices_texture_vertices_vertex_normals_callback_type(),
		obj::obj_parser::polygonal_face_geometric_vertices_begin_callback_type(),
		obj::obj_parser::polygonal_face_geometric_vertices_vertex_callback_type(),
		obj::obj_parser::polygonal_face_geometric_vertices_end_callback_type(),
		obj::obj_parser::polygonal_face_geometric_vertices_texture_vertices_begin_callback_type(),
		obj::obj_parser::polygonal_face_geometric_vertices_texture_vertices_vertex_callback_type(),
		obj::obj_parser::polygonal_face_geometric_vertices_texture_vertices_end_callback_type(),
		obj::obj_parser::polygonal_face_geometric_vertices_vertex_normals_begin_callback_type(),
		obj::obj_parser::polygonal_face_geometric_vertices_vertex_normals_vertex_callback_type(),
		obj::obj_parser::polygonal_face_geometric_vertices_vertex_normals_end_callback_type(),
		obj::obj_parser::polygonal_face_geometric_vertices_texture_vertices_vertex_normals_begin_callback_type(),
		obj::obj_parser::polygonal_face_geometric_vertices_texture_vertices_vertex_normals_vertex_callback_type(),
		obj::obj_parser::polygonal_face_geometric_vertices_texture_vertices_vertex_normals_end_callback_type()
	);
	clearMesh();
	obj_parser.parse(path);
}

void Mesh::clearMesh()
{
	vertices_.clear();
	normals_.clear();
	textureCoordinates_.clear();
	triangles_.clear();
}

void Mesh::draw(GLWidget * dest) const
{
	for (std::vector<Triangle>::const_iterator it = triangles_.begin();
		 it != triangles_.end();
		 it++)
	{
		it->draw(vertices_, textureCoordinates_, normals_);
	}
}

// Parser Callbacks
void Mesh::info_callback(const std::string& filename, std::size_t line_number, const std::string& message)
{
	std::cout << "Parser Info: " << filename << ":" << line_number << "\t" << message;
}

void Mesh::warning_callback(const std::string& filename, std::size_t line_number, const std::string& message)
{
	std::cerr << "Parser Warning: " << filename << ":" << line_number << "\t" << message;
}

void Mesh::error_callback(const std::string& filename, std::size_t line_number, const std::string& message)
{
	std::cerr << "Parser Error: " << filename << ":" << line_number << "\t" << message;
}

void Mesh::geometric_vertex_callback(float x, float y, float z)
{
	vertices_.push_back(Tool::Point(x, y, z));
}

void Mesh::texture_vertex_callback(float u, float v)
{
	textureCoordinates_.push_back(Tool::Vec2f(u, v));
}

void Mesh::vertex_normal_callback(float x, float y, float z)
{
	normals_.push_back(Tool::Vector(x, y, z));
}

void Mesh::triangular_face_geometric_vertices_callback(int v1, int v2, int v3)
{
	Triangle triangle(v1-1, v2-1, v3-1);
	triangles_.push_back(triangle);
}

void Mesh::triangular_face_geometric_vertices_texture_vertices_callback(const obj::index_2_tuple_type& v1_vt1, const obj::index_2_tuple_type& v2_vt2, const obj::index_2_tuple_type& v3_vt3)
{
	Triangle triangle(std::tr1::get<0>(v1_vt1)-1, std::tr1::get<0>(v2_vt2)-1, std::tr1::get<0>(v3_vt3)-1);
	triangle.setTexture(std::tr1::get<1>(v1_vt1)-1, std::tr1::get<1>(v2_vt2)-1, std::tr1::get<1>(v3_vt3)-1);
	triangles_.push_back(triangle);
}

void Mesh::triangular_face_geometric_vertices_vertex_normals_callback(const obj::index_2_tuple_type& v1_vn1, const obj::index_2_tuple_type& v2_vn2, const obj::index_2_tuple_type& v3_vn3)
{
	Triangle triangle(std::tr1::get<0>(v1_vn1)-1, std::tr1::get<0>(v2_vn2)-1, std::tr1::get<0>(v3_vn3)-1);
	triangle.setNormal(std::tr1::get<1>(v1_vn1)-1, std::tr1::get<1>(v2_vn2)-1, std::tr1::get<1>(v3_vn3)-1);
	triangles_.push_back(triangle);
}

void Mesh::triangular_face_geometric_vertices_texture_vertices_vertex_normals_callback(const obj::index_3_tuple_type& v1_vt1_vn1, const obj::index_3_tuple_type& v2_vt2_vn2, const obj::index_3_tuple_type& v3_vt3_vn3)
{
	Triangle triangle(std::tr1::get<0>(v1_vt1_vn1)-1, std::tr1::get<0>(v2_vt2_vn2)-1, std::tr1::get<0>(v3_vt3_vn3)-1);
	triangle.setTexture(std::tr1::get<1>(v1_vt1_vn1)-1, std::tr1::get<1>(v2_vt2_vn2)-1, std::tr1::get<1>(v3_vt3_vn3)-1);
	triangle.setNormal(std::tr1::get<2>(v1_vt1_vn1)-1, std::tr1::get<2>(v2_vt2_vn2)-1, std::tr1::get<2>(v3_vt3_vn3)-1);
	triangles_.push_back(triangle);
}

}
