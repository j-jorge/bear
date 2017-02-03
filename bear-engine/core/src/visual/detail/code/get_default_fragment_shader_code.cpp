#include "visual/detail/get_default_fragment_shader_code.hpp"

const std::string& bear::visual::detail::get_default_fragment_shader_code()
{
  static const std::string result =
    R"(
#version 100

precision lowp float;

uniform sampler2D texture;

varying lowp vec4 g_fragment_color;
varying mediump vec2 g_texture_coordinates;

void main()
{
    gl_FragColor =
      g_fragment_color * texture2D( texture, g_texture_coordinates );
}

)";

  return result;
}

