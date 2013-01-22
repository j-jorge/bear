cmake_minimum_required(VERSION 2.6)
project(bear-editor)

set( BF_E_TARGET_NAME bear-editor )

include_directories( .. )

#-------------------------------------------------------------------------------
set( BF_E_SOURCE_FILES
  code/accelerator_table.cpp
  code/accordion.cpp
  code/animation.cpp
  code/animation_edit.cpp
  code/animation_file_edit.cpp
  code/animation_file_type.cpp
  code/animation_file_xml_reader.cpp
  code/animation_file_xml_writer.cpp
  code/animation_frame.cpp
  code/animation_player.cpp
  code/animation_view_ctrl.cpp
  code/any_animation.cpp
  code/any_animation_edit.cpp
  code/base_editor_application.cpp
  code/bitmap_rendering_attributes.cpp
  code/bitmap_rendering_attributes_edit.cpp
  code/bool_edit.cpp
  code/check_error.cpp
  code/class_not_found.cpp
  code/class_tree_ctrl.cpp
  code/color.cpp
  code/color_edit.cpp
  code/compilation_context.cpp
  code/compiled_file.cpp
  code/config_frame.cpp
  code/custom_type.cpp
  code/font.cpp
  code/font_edit.cpp
  code/frame_edit.cpp
  code/human_readable.cpp
  code/image_list_ctrl.cpp
  code/image_pool.cpp
  code/image_selection_dialog.cpp
  code/item_check_result.cpp
  code/item_class.cpp
  code/item_class_pool.cpp
  code/item_class_selection_dialog.cpp
  code/item_class_xml_parser.cpp
  code/item_comparator.cpp
  code/item_event.cpp
  code/item_field_edit.cpp
  code/item_instance.cpp
  code/item_reference_edit.cpp
  code/item_rendering_parameters.cpp
  code/path_configuration.cpp
  code/sample.cpp
  code/sample_edit.cpp
  code/slider_ctrl.cpp
  code/slider_with_ticks.cpp
  code/splash_screen.cpp
  code/sprite.cpp
  code/sprite_edit.cpp
  code/sprite_image_cache.cpp
  code/sprite_view.cpp
  code/sprite_view_ctrl.cpp
  code/stream_conv.cpp
  code/tree_builder.cpp
  code/trinary_logic.cpp
  code/type_field.cpp
  code/type_field_interval.cpp
  code/type_field_set.cpp

  opaque_rectangle/code/ler_base_problem.cpp
  opaque_rectangle/code/ler_horizontal_problem.cpp
  opaque_rectangle/code/ler_problem.cpp
  opaque_rectangle/code/ler_solver.cpp
  opaque_rectangle/code/test.cpp

  xml/code/exception.cpp
  xml/code/item_class_inherit_node.cpp
  xml/code/item_instance_node.cpp
  xml/code/item_instance_field_node.cpp
  xml/code/item_instance_fields_node.cpp
  xml/code/reader_tool.cpp
  xml/code/util.cpp
  xml/code/value_to_xml.cpp
  xml/code/xml_to_value.cpp
)

if( WIN32 )
  set( wxWidgets_LIBRARIES
    ${wxWidgets_LIBRARIES}
    gdi32
    comdlg32
    ole32
    oleaut32
    uuid
    )
endif( WIN32 )

include(${wxWidgets_USE_FILE})

if( WIN32 )
  add_library( ${BF_E_TARGET_NAME} STATIC ${BF_E_SOURCE_FILES} )
  add_definitions( "-DBEAR_EDITOR_EXPORT" )
else( WIN32 )
  add_library( ${BF_E_TARGET_NAME} SHARED ${BF_E_SOURCE_FILES} )
endif( WIN32 )

set_target_properties(
  ${BF_E_TARGET_NAME}
  PROPERTIES
  INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${BEAR_FACTORY_INSTALL_LIBRARY_DIR}"
  )

if( NOT WIN32 )
  set_target_properties(
    ${BF_E_TARGET_NAME}
    PROPERTIES
    COMPILE_DEFINITIONS "BEAR_FACTORY_TEXT_DOMAIN_PATH=${CMAKE_INSTALL_PREFIX}/${TRANSLATIONS_INSTALL_DIR}"
    )
endif( NOT WIN32 )

set(
  BF_E_LINK_LIBRARIES
  ${wxWidgets_LIBRARIES}
  ${CLAW_LOGGER_LIBRARIES}
  ${CLAW_CONFIGURATION_FILE_LIBRARIES}
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_THREAD_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  )

if(APPLE)
  set(
    BF_E_LINK_LIBRARIES
    ${BF_E_LINK_LIBRARIES}
    intl )
endif(APPLE)

target_link_libraries(
  ${BF_E_TARGET_NAME}
  ${BF_E_LINK_LIBRARIES}
  )

install(
  TARGETS ${BF_E_TARGET_NAME}
  DESTINATION "${BEAR_FACTORY_INSTALL_LIBRARY_DIR}"
  )
