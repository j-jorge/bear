set( VISUAL_TARGET_NAME bear_visual )

#-------------------------------------------------------------------------------
include(FindFreetype)

#-------------------------------------------------------------------------------
set( VISUAL_SOURCE_FILES
  code/animation.cpp
  code/base_scene_element.cpp
  code/bitmap_rendering_attributes.cpp
  code/bitmap_writing.cpp
  code/color.cpp
  code/gl_error.cpp
  code/gl_image.cpp
  code/gl_screen.cpp
  code/gl_fragment_shader.cpp
  code/gl_shader_program.cpp
  code/image.cpp
  code/image_manager.cpp
  code/placed_sprite.cpp
  code/scene_element.cpp
  code/scene_element_sequence.cpp
  code/scene_sprite.cpp
  code/scene_line.cpp
  code/scene_polygon.cpp
  code/scene_rectangle.cpp
  code/scene_shader_pop.cpp
  code/scene_shader_push.cpp
  code/scene_star.cpp
  code/scene_writing.cpp
  code/screen.cpp
  code/sdl_error.cpp
  code/sequence_effect.cpp
  code/shader_program.cpp
  code/sprite.cpp
  code/sprite_sequence.cpp
  code/star.cpp
  code/text_align.cpp
  code/text_layout.cpp
  code/text_layout_display_size.cpp
  code/text_metric.cpp
  code/writing.cpp

  font/code/base_font.cpp
  font/code/bitmap_charmap.cpp
  font/code/bitmap_font.cpp
  font/code/font.cpp
  font/code/font_manager.cpp
  font/code/freetype_face.cpp
  font/code/glyph_metrics.cpp
  font/code/true_type_font.cpp
  font/code/true_type_memory_file.cpp
)

if( WIN32 )
  set( VISUAL_SOURCE_FILES
    glew/code/glew.cpp
    ${VISUAL_SOURCE_FILES}
    )
endif()

add_library(
  ${VISUAL_TARGET_NAME}
  ${BEAR_ENGINE_CORE_LINK_TYPE}
  ${VISUAL_SOURCE_FILES}
  )

set_target_properties(
  ${VISUAL_TARGET_NAME}
  PROPERTIES
  INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${BEAR_ENGINE_INSTALL_LIBRARY_DIR}"
  )

include_directories( ${FREETYPE_INCLUDE_DIRS} )

install(
  TARGETS ${VISUAL_TARGET_NAME}
  DESTINATION ${BEAR_ENGINE_INSTALL_LIBRARY_DIR}
  )

target_link_libraries(
  ${VISUAL_TARGET_NAME}
  ${SDL_LIBRARY}
  ${OPENGL_LIBRARIES}
  ${CLAW_GRAPHIC_LIBRARIES}
  ${CLAW_LOGGER_LIBRARIES}
  ${FREETYPE_LIBRARIES}
  )
