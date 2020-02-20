set(target_name bear-editor)

set(module_root "${BEAR_ROOT}/bear-factory/bear-editor")
set(include_root "${module_root}/include")
set(source_root "${module_root}/src")

prepend_string(
  files
  "${source_root}/"
  
  "accelerator_table.cpp"
  "accordion.cpp"
  "animation.cpp"
  "animation_edit.cpp"
  "animation_file_edit.cpp"
  "animation_file_type.cpp"
  "animation_file_xml_reader.cpp"
  "animation_file_xml_writer.cpp"
  "animation_frame.cpp"
  "animation_player.cpp"
  "animation_view_ctrl.cpp"
  "any_animation.cpp"
  "any_animation_edit.cpp"
  "arithmetic_parser.cpp"
  "base_editor_application.cpp"
  "bitmap_rendering_attributes.cpp"
  "bitmap_rendering_attributes_edit.cpp"
  "bool_edit.cpp"
  "check_error.cpp"
  "class_not_found.cpp"
  "class_tree_ctrl.cpp"
  "color.cpp"
  "color_edit.cpp"
  "compilation_context.cpp"
  "compiled_file.cpp"
  "config_frame.cpp"
  "custom_type.cpp"
  "easing_combo.cpp"
  "easing_combo_popup.cpp"
  "easing_edit.cpp"
  "font.cpp"
  "font_edit.cpp"
  "frame_edit.cpp"
  "human_readable.cpp"
  "icons.cpp"
  "image_list_ctrl.cpp"
  "image_pool.cpp"
  "image_selection_dialog.cpp"
  "item_check_result.cpp"
  "item_class.cpp"
  "item_class_pool.cpp"
  "item_class_selection_dialog.cpp"
  "item_class_xml_parser.cpp"
  "item_comparator.cpp"
  "item_event.cpp"
  "item_field_edit.cpp"
  "item_instance.cpp"
  "item_reference_edit.cpp"
  "item_rendering_parameters.cpp"
  "opaque_rectangle/ler_base_problem.cpp"
  "opaque_rectangle/ler_horizontal_problem.cpp"
  "opaque_rectangle/ler_problem.cpp"
  "opaque_rectangle/ler_solver.cpp"
  "opaque_rectangle/test.cpp"
  "path_configuration.cpp"
  "sample.cpp"
  "sample_edit.cpp"
  "slider_ctrl.cpp"
  "slider_with_ticks.cpp"
  "splash_screen.cpp"
  "sprite.cpp"
  "sprite_edit.cpp"
  "sprite_image_cache.cpp"
  "sprite_view.cpp"
  "sprite_view_ctrl.cpp"
  "stream_conv.cpp"
  "tree_builder.cpp"
  "trinary_logic.cpp"
  "type_field.cpp"
  "type_field_interval.cpp"
  "type_field_set.cpp"
  "workspace.cpp"
  "workspace_environment.cpp"
  "wx_type_cast.cpp"
  "xml/exception.cpp"
  "xml/item_class_inherit_node.cpp"
  "xml/item_instance_field_node.cpp"
  "xml/item_instance_fields_node.cpp"
  "xml/item_instance_node.cpp"
  "xml/reader_tool.cpp"
  "xml/util.cpp"
  "xml/value_to_xml.cpp"
  "xml/xml_to_value.cpp"
  )

add_library(${target_name} ${files})

target_include_directories(
  ${target_name} PUBLIC $<BUILD_INTERFACE:${include_root}>
  )

target_link_libraries(
  ${target_name}
  PUBLIC
  bear_engine_common
  wxWidgets::wxWidgets
  Claw::logger
  Claw::configuration_file
  Boost::filesystem
  Boost::thread
  Boost::system
  )

if(NOT WIN32)
  target_compile_definitions(
    ${target_name}
    PRIVATE
    "BEAR_FACTORY_TEXT_DOMAIN_PATH=${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LOCALEDIR}"
    )
endif()

target_compile_definitions(
  ${target_name}
  PRIVATE
  "BF_IMAGES_PATH=${CMAKE_INSTALL_PREFIX}/${BF_IMAGES_PATH}"
  )
  
install(TARGETS ${target_name})
