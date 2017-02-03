#include "visual/detail/get_default_vertex_shader_code.hpp"

const std::string& bear::visual::detail::get_default_vertex_shader_code()
{
  static const std::string result =
    R"(
#version 100

uniform mat4 transform;
attribute vec4 in_position;
attribute vec2 in_texture_coordinates;
attribute vec4 in_color;

varying lowp vec4 g_fragment_color;
varying mediump vec2 g_texture_coordinates;

void main()
{
    gl_Position = transform * in_position;
    g_fragment_color = in_color;
    g_texture_coordinates = in_texture_coordinates;
}
)";

  return result;
}

