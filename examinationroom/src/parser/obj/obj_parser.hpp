#ifndef OBJ_OBJ_PARSER_HPP_INCLUDED
#define OBJ_OBJ_PARSER_HPP_INCLUDED

#include <fstream>
#include <istream>
#include <string>

#include <functional>

#include <obj/obj.hpp>

namespace obj {

class obj_parser
{
public:
  typedef std::tr1::function<void (std::size_t, const std::string&)> info_callback_type;
  typedef std::tr1::function<void (std::size_t, const std::string&)> warning_callback_type;
  typedef std::tr1::function<void (std::size_t, const std::string&)> error_callback_type;
  typedef std::tr1::function<void (float_type, float_type, float_type)> geometric_vertex_callback_type;
  typedef std::tr1::function<void (float_type, float_type)> texture_vertex_callback_type;
  typedef std::tr1::function<void (float_type, float_type, float_type)> vertex_normal_callback_type;
  typedef std::tr1::function<void (index_type, index_type, index_type)> triangular_face_geometric_vertices_callback_type;
  typedef std::tr1::function<void (const index_2_tuple_type&, const index_2_tuple_type&, const index_2_tuple_type&)> triangular_face_geometric_vertices_texture_vertices_callback_type;
  typedef std::tr1::function<void (const index_2_tuple_type&, const index_2_tuple_type&, const index_2_tuple_type&)> triangular_face_geometric_vertices_vertex_normals_callback_type;
  typedef std::tr1::function<void (const index_3_tuple_type&, const index_3_tuple_type&, const index_3_tuple_type&)> triangular_face_geometric_vertices_texture_vertices_vertex_normals_callback_type;
  typedef std::tr1::function<void (index_type, index_type, index_type, index_type)> quadrilateral_face_geometric_vertices_callback_type;
  typedef std::tr1::function<void (const index_2_tuple_type&, const index_2_tuple_type&, const index_2_tuple_type&, const index_2_tuple_type&)> quadrilateral_face_geometric_vertices_texture_vertices_callback_type;
  typedef std::tr1::function<void (const index_2_tuple_type&, const index_2_tuple_type&, const index_2_tuple_type&, const index_2_tuple_type&)> quadrilateral_face_geometric_vertices_vertex_normals_callback_type;
  typedef std::tr1::function<void (const index_3_tuple_type&, const index_3_tuple_type&, const index_3_tuple_type&, const index_3_tuple_type&)> quadrilateral_face_geometric_vertices_texture_vertices_vertex_normals_callback_type;
  typedef std::tr1::function<void (index_type, index_type, index_type)> polygonal_face_geometric_vertices_begin_callback_type;
  typedef std::tr1::function<void (index_type)> polygonal_face_geometric_vertices_vertex_callback_type;
  typedef std::tr1::function<void ()> polygonal_face_geometric_vertices_end_callback_type;
  typedef std::tr1::function<void (const index_2_tuple_type&, const index_2_tuple_type&, const index_2_tuple_type&)> polygonal_face_geometric_vertices_texture_vertices_begin_callback_type;
  typedef std::tr1::function<void (const index_2_tuple_type&)> polygonal_face_geometric_vertices_texture_vertices_vertex_callback_type;
  typedef std::tr1::function<void ()> polygonal_face_geometric_vertices_texture_vertices_end_callback_type;
  typedef std::tr1::function<void (const index_2_tuple_type&, const index_2_tuple_type&, const index_2_tuple_type&)> polygonal_face_geometric_vertices_vertex_normals_begin_callback_type;
  typedef std::tr1::function<void (const index_2_tuple_type&)> polygonal_face_geometric_vertices_vertex_normals_vertex_callback_type;
  typedef std::tr1::function<void ()> polygonal_face_geometric_vertices_vertex_normals_end_callback_type;
  typedef std::tr1::function<void (const index_3_tuple_type&, const index_3_tuple_type&, const index_3_tuple_type&)> polygonal_face_geometric_vertices_texture_vertices_vertex_normals_begin_callback_type;
  typedef std::tr1::function<void (const index_3_tuple_type&)> polygonal_face_geometric_vertices_texture_vertices_vertex_normals_vertex_callback_type;
  typedef std::tr1::function<void ()> polygonal_face_geometric_vertices_texture_vertices_vertex_normals_end_callback_type;
  typedef std::tr1::function<void (const std::string&)> group_name_callback_type;
  typedef std::tr1::function<void (size_type)> smoothing_group_callback_type;
  typedef std::tr1::function<void (const std::string&)> object_name_callback_type;
  typedef std::tr1::function<void (const std::string&)> material_library_callback_type;
  typedef std::tr1::function<void (const std::string&)> material_name_callback_type;
  typedef std::tr1::function<void (const std::string&)> comment_callback_type;
  typedef int flags_type;
  typedef enum {
    parse_blank_lines_as_comment = 1 << 0,
    triangulate_faces = 1 << 1,
    translate_negative_indices = 1 << 2
  };
  obj_parser(flags_type flags = 0);
  void info_callback(const info_callback_type& info_callback);
  void warning_callback(const warning_callback_type& warning_callback);
  void error_callback(const error_callback_type& error_callback);
  void geometric_vertex_callback(const geometric_vertex_callback_type& geometric_vertex_callback);
  void texture_vertex_callback(const texture_vertex_callback_type& texture_vertex_callback);
  void vertex_normal_callback(const vertex_normal_callback_type& vertex_normal_callback);
  void face_callbacks(const triangular_face_geometric_vertices_callback_type& triangular_face_geometric_vertices_callback, const triangular_face_geometric_vertices_texture_vertices_callback_type& triangular_face_geometric_vertices_texture_vertices_callback, const triangular_face_geometric_vertices_vertex_normals_callback_type& triangular_face_geometric_vertices_vertex_normals_callback, const triangular_face_geometric_vertices_texture_vertices_vertex_normals_callback_type& triangular_face_geometric_vertices_texture_vertices_vertex_normals_callback, const quadrilateral_face_geometric_vertices_callback_type& quadrilateral_face_geometric_vertices_callback, const quadrilateral_face_geometric_vertices_texture_vertices_callback_type& quadrilateral_face_geometric_vertices_texture_vertices_callback, const quadrilateral_face_geometric_vertices_vertex_normals_callback_type& quadrilateral_face_geometric_vertices_vertex_normals_callback, const quadrilateral_face_geometric_vertices_texture_vertices_vertex_normals_callback_type& quadrilateral_face_geometric_vertices_texture_vertices_vertex_normals_callback, const polygonal_face_geometric_vertices_begin_callback_type& polygonal_face_geometric_vertices_begin_callback, const polygonal_face_geometric_vertices_vertex_callback_type& polygonal_face_geometric_vertices_vertex_callback, const polygonal_face_geometric_vertices_end_callback_type& polygonal_face_geometric_vertices_end_callback, const polygonal_face_geometric_vertices_texture_vertices_begin_callback_type& polygonal_face_geometric_vertices_texture_vertices_begin_callback, const polygonal_face_geometric_vertices_texture_vertices_vertex_callback_type& polygonal_face_geometric_vertices_texture_vertices_vertex_callback, const polygonal_face_geometric_vertices_texture_vertices_end_callback_type& polygonal_face_geometric_vertices_texture_vertices_end_callback, const polygonal_face_geometric_vertices_vertex_normals_begin_callback_type& polygonal_face_geometric_vertices_vertex_normals_begin_callback, const polygonal_face_geometric_vertices_vertex_normals_vertex_callback_type& polygonal_face_geometric_vertices_vertex_normals_vertex_callback, const polygonal_face_geometric_vertices_vertex_normals_end_callback_type& polygonal_face_geometric_vertices_vertex_normals_end_callback, const polygonal_face_geometric_vertices_texture_vertices_vertex_normals_begin_callback_type& polygonal_face_geometric_vertices_texture_vertices_vertex_normals_begin_callback, const polygonal_face_geometric_vertices_texture_vertices_vertex_normals_vertex_callback_type& polygonal_face_geometric_vertices_texture_vertices_vertex_normals_vertex_callback, const polygonal_face_geometric_vertices_texture_vertices_vertex_normals_end_callback_type& polygonal_face_geometric_vertices_texture_vertices_vertex_normals_end_callback);
  void group_name_callback(const group_name_callback_type& group_name_callback);
  void smoothing_group_callback(const smoothing_group_callback_type& smoothing_group_callback);
  void object_name_callback(const object_name_callback_type& object_name_callback);
  void material_library_callback(const material_library_callback_type& material_library_callback);
  void material_name_callback(const material_name_callback_type& material_name_callback);
  void comment_callback(const comment_callback_type& comment_callback);
  bool parse(std::istream& istream);
  bool parse(const std::string& filename);
private:
  flags_type flags_;
  info_callback_type info_callback_;
  warning_callback_type warning_callback_;
  error_callback_type error_callback_;
  geometric_vertex_callback_type geometric_vertex_callback_;
  texture_vertex_callback_type texture_vertex_callback_;
  vertex_normal_callback_type vertex_normal_callback_;
  triangular_face_geometric_vertices_callback_type triangular_face_geometric_vertices_callback_;
  triangular_face_geometric_vertices_texture_vertices_callback_type triangular_face_geometric_vertices_texture_vertices_callback_;
  triangular_face_geometric_vertices_vertex_normals_callback_type triangular_face_geometric_vertices_vertex_normals_callback_;
  triangular_face_geometric_vertices_texture_vertices_vertex_normals_callback_type triangular_face_geometric_vertices_texture_vertices_vertex_normals_callback_;
  quadrilateral_face_geometric_vertices_callback_type quadrilateral_face_geometric_vertices_callback_;
  quadrilateral_face_geometric_vertices_texture_vertices_callback_type quadrilateral_face_geometric_vertices_texture_vertices_callback_;
  quadrilateral_face_geometric_vertices_vertex_normals_callback_type quadrilateral_face_geometric_vertices_vertex_normals_callback_;
  quadrilateral_face_geometric_vertices_texture_vertices_vertex_normals_callback_type quadrilateral_face_geometric_vertices_texture_vertices_vertex_normals_callback_;
  polygonal_face_geometric_vertices_begin_callback_type polygonal_face_geometric_vertices_begin_callback_;
  polygonal_face_geometric_vertices_vertex_callback_type polygonal_face_geometric_vertices_vertex_callback_;
  polygonal_face_geometric_vertices_end_callback_type polygonal_face_geometric_vertices_end_callback_;
  polygonal_face_geometric_vertices_texture_vertices_begin_callback_type polygonal_face_geometric_vertices_texture_vertices_begin_callback_;
  polygonal_face_geometric_vertices_texture_vertices_vertex_callback_type polygonal_face_geometric_vertices_texture_vertices_vertex_callback_;
  polygonal_face_geometric_vertices_texture_vertices_end_callback_type polygonal_face_geometric_vertices_texture_vertices_end_callback_;
  polygonal_face_geometric_vertices_vertex_normals_begin_callback_type polygonal_face_geometric_vertices_vertex_normals_begin_callback_;
  polygonal_face_geometric_vertices_vertex_normals_vertex_callback_type polygonal_face_geometric_vertices_vertex_normals_vertex_callback_;
  polygonal_face_geometric_vertices_vertex_normals_end_callback_type polygonal_face_geometric_vertices_vertex_normals_end_callback_;
  polygonal_face_geometric_vertices_texture_vertices_vertex_normals_begin_callback_type polygonal_face_geometric_vertices_texture_vertices_vertex_normals_begin_callback_;
  polygonal_face_geometric_vertices_texture_vertices_vertex_normals_vertex_callback_type polygonal_face_geometric_vertices_texture_vertices_vertex_normals_vertex_callback_;
  polygonal_face_geometric_vertices_texture_vertices_vertex_normals_end_callback_type polygonal_face_geometric_vertices_texture_vertices_vertex_normals_end_callback_;
  group_name_callback_type group_name_callback_;
  smoothing_group_callback_type smoothing_group_callback_;
  object_name_callback_type object_name_callback_;
  material_library_callback_type material_library_callback_;
  material_name_callback_type material_name_callback_;
  comment_callback_type comment_callback_;
};

} // namespace obj

inline obj::obj_parser::obj_parser(flags_type flags)
  : flags_(flags)
{
}

inline bool obj::obj_parser::parse(const std::string& filename)
{
  std::ifstream ifstream(filename.c_str());
  return parse(ifstream);
}

inline void obj::obj_parser::info_callback(const info_callback_type& info_callback)
{
  info_callback_ = info_callback;
}

inline void obj::obj_parser::warning_callback(const warning_callback_type& warning_callback)
{
  warning_callback_ = warning_callback;
}

inline void obj::obj_parser::error_callback(const error_callback_type& error_callback)
{
  error_callback_ = error_callback;
}

inline void obj::obj_parser::geometric_vertex_callback(const geometric_vertex_callback_type& geometric_vertex_callback)
{
  geometric_vertex_callback_ = geometric_vertex_callback;
}

inline void obj::obj_parser::texture_vertex_callback(const texture_vertex_callback_type& texture_vertex_callback)
{
  texture_vertex_callback_ = texture_vertex_callback;
}

inline void obj::obj_parser::vertex_normal_callback(const vertex_normal_callback_type& vertex_normal_callback)
{
  vertex_normal_callback_ = vertex_normal_callback;
}

inline void obj::obj_parser::face_callbacks(const triangular_face_geometric_vertices_callback_type& triangular_face_geometric_vertices_callback, const triangular_face_geometric_vertices_texture_vertices_callback_type& triangular_face_geometric_vertices_texture_vertices_callback, const triangular_face_geometric_vertices_vertex_normals_callback_type& triangular_face_geometric_vertices_vertex_normals_callback, const triangular_face_geometric_vertices_texture_vertices_vertex_normals_callback_type& triangular_face_geometric_vertices_texture_vertices_vertex_normals_callback, const quadrilateral_face_geometric_vertices_callback_type& quadrilateral_face_geometric_vertices_callback, const quadrilateral_face_geometric_vertices_texture_vertices_callback_type& quadrilateral_face_geometric_vertices_texture_vertices_callback, const quadrilateral_face_geometric_vertices_vertex_normals_callback_type& quadrilateral_face_geometric_vertices_vertex_normals_callback, const quadrilateral_face_geometric_vertices_texture_vertices_vertex_normals_callback_type& quadrilateral_face_geometric_vertices_texture_vertices_vertex_normals_callback, const polygonal_face_geometric_vertices_begin_callback_type& polygonal_face_geometric_vertices_begin_callback, const polygonal_face_geometric_vertices_vertex_callback_type& polygonal_face_geometric_vertices_vertex_callback, const polygonal_face_geometric_vertices_end_callback_type& polygonal_face_geometric_vertices_end_callback, const polygonal_face_geometric_vertices_texture_vertices_begin_callback_type& polygonal_face_geometric_vertices_texture_vertices_begin_callback, const polygonal_face_geometric_vertices_texture_vertices_vertex_callback_type& polygonal_face_geometric_vertices_texture_vertices_vertex_callback, const polygonal_face_geometric_vertices_texture_vertices_end_callback_type& polygonal_face_geometric_vertices_texture_vertices_end_callback, const polygonal_face_geometric_vertices_vertex_normals_begin_callback_type& polygonal_face_geometric_vertices_vertex_normals_begin_callback, const polygonal_face_geometric_vertices_vertex_normals_vertex_callback_type& polygonal_face_geometric_vertices_vertex_normals_vertex_callback, const polygonal_face_geometric_vertices_vertex_normals_end_callback_type& polygonal_face_geometric_vertices_vertex_normals_end_callback, const polygonal_face_geometric_vertices_texture_vertices_vertex_normals_begin_callback_type& polygonal_face_geometric_vertices_texture_vertices_vertex_normals_begin_callback, const polygonal_face_geometric_vertices_texture_vertices_vertex_normals_vertex_callback_type& polygonal_face_geometric_vertices_texture_vertices_vertex_normals_vertex_callback, const polygonal_face_geometric_vertices_texture_vertices_vertex_normals_end_callback_type& polygonal_face_geometric_vertices_texture_vertices_vertex_normals_end_callback)
{
  triangular_face_geometric_vertices_callback_ = triangular_face_geometric_vertices_callback;
  triangular_face_geometric_vertices_texture_vertices_callback_ = triangular_face_geometric_vertices_texture_vertices_callback;
  triangular_face_geometric_vertices_vertex_normals_callback_ = triangular_face_geometric_vertices_vertex_normals_callback;
  triangular_face_geometric_vertices_texture_vertices_vertex_normals_callback_ = triangular_face_geometric_vertices_texture_vertices_vertex_normals_callback;
  quadrilateral_face_geometric_vertices_callback_ = quadrilateral_face_geometric_vertices_callback;
  quadrilateral_face_geometric_vertices_texture_vertices_callback_ = quadrilateral_face_geometric_vertices_texture_vertices_callback;
  quadrilateral_face_geometric_vertices_vertex_normals_callback_ = quadrilateral_face_geometric_vertices_vertex_normals_callback;
  quadrilateral_face_geometric_vertices_texture_vertices_vertex_normals_callback_ = quadrilateral_face_geometric_vertices_texture_vertices_vertex_normals_callback;
  polygonal_face_geometric_vertices_begin_callback_ = polygonal_face_geometric_vertices_begin_callback;
  polygonal_face_geometric_vertices_vertex_callback_ = polygonal_face_geometric_vertices_vertex_callback;
  polygonal_face_geometric_vertices_end_callback_ = polygonal_face_geometric_vertices_end_callback;
  polygonal_face_geometric_vertices_texture_vertices_begin_callback_ = polygonal_face_geometric_vertices_texture_vertices_begin_callback;
  polygonal_face_geometric_vertices_texture_vertices_vertex_callback_ = polygonal_face_geometric_vertices_texture_vertices_vertex_callback;
  polygonal_face_geometric_vertices_texture_vertices_end_callback_ = polygonal_face_geometric_vertices_texture_vertices_end_callback;
  polygonal_face_geometric_vertices_vertex_normals_begin_callback_ = polygonal_face_geometric_vertices_vertex_normals_begin_callback;
  polygonal_face_geometric_vertices_vertex_normals_vertex_callback_ = polygonal_face_geometric_vertices_vertex_normals_vertex_callback;
  polygonal_face_geometric_vertices_vertex_normals_end_callback_ = polygonal_face_geometric_vertices_vertex_normals_end_callback;
  polygonal_face_geometric_vertices_texture_vertices_vertex_normals_begin_callback_ = polygonal_face_geometric_vertices_texture_vertices_vertex_normals_begin_callback;
  polygonal_face_geometric_vertices_texture_vertices_vertex_normals_vertex_callback_ = polygonal_face_geometric_vertices_texture_vertices_vertex_normals_vertex_callback;
  polygonal_face_geometric_vertices_texture_vertices_vertex_normals_end_callback_ = polygonal_face_geometric_vertices_texture_vertices_vertex_normals_end_callback;
}

inline void obj::obj_parser::group_name_callback(const group_name_callback_type& group_name_callback)
{
  group_name_callback_ = group_name_callback;
}

inline void obj::obj_parser::smoothing_group_callback(const smoothing_group_callback_type& smoothing_group_callback)
{
  smoothing_group_callback_ = smoothing_group_callback;
}

inline void obj::obj_parser::object_name_callback(const object_name_callback_type& object_name_callback)
{
  object_name_callback_ = object_name_callback;
}

inline void obj::obj_parser::material_library_callback(const material_library_callback_type& material_library_callback)
{
  material_library_callback_ = material_library_callback;
}

inline void obj::obj_parser::material_name_callback(const material_name_callback_type& material_name_callback)
{
  material_name_callback_ = material_name_callback;
}

inline void obj::obj_parser::comment_callback(const comment_callback_type& comment_callback)
{
  comment_callback_ = comment_callback;
}

#endif // OBJ_OBJ_PARSER_HPP_INCLUDED
