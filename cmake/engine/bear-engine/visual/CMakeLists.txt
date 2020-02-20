if(WIN32)
  set(platform_files "glew/glew.cpp")
endif()

add_bear_engine_library(
  NAME
  visual

  FILES
  "animation.cpp"
  "base_scene_element.cpp"
  "bitmap_rendering_attributes.cpp"
  "bitmap_writing.cpp"
  "capture.cpp"
  "color.cpp"

  "gl_capture.cpp"
  "gl_capture_queue.cpp"
  "gl_draw.cpp"
  "gl_error.cpp"
  "gl_fragment_shader.cpp"
  "gl_image.cpp"
  "gl_renderer.cpp"
  "gl_screen.cpp"
  "gl_shader_program.cpp"
  "gl_state.cpp"
  "gl_vertex_shader.cpp"
  "image.cpp"
  "image_manager.cpp"
  "placed_sprite.cpp"
  "scene_element.cpp"
  "scene_element_sequence.cpp"
  "scene_line.cpp"
  "scene_polygon.cpp"
  "scene_rectangle.cpp"
  "scene_shader_pop.cpp"
  "scene_shader_push.cpp"
  "scene_sprite.cpp"
  "scene_star.cpp"
  "scene_writing.cpp"
  "screen.cpp"
  "sdl_error.cpp"
  "sequence_effect.cpp"
  "shader_program.cpp"
  "sprite.cpp"
  "sprite_sequence.cpp"
  "star.cpp"
  "text_align.cpp"
  "text_layout.cpp"
  "text_layout_display_size.cpp"
  "text_metric.cpp"
  "writing.cpp"

  "detail/apply_shader.cpp"
  "detail/get_default_fragment_shader_code.cpp"
  "detail/get_default_vertex_shader_code.cpp"

  "font/base_font.cpp"
  "font/bitmap_charmap.cpp"
  "font/bitmap_font.cpp"
  "font/font.cpp"
  "font/font_manager.cpp"
  "font/freetype_face.cpp"
  "font/glyph_metrics.cpp"
  "font/true_type_font.cpp"
  "font/true_type_memory_file.cpp"

  ${platform_files}
  
  LINK
  bear_charset
  bear_time
  Claw::graphic
  Claw::logger
  Boost::thread
  Boost::system
  Freetype::Freetype
  OpenGL::GL
  SDL2
  )
