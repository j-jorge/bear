cmake_minimum_required(VERSION 2.6)
project(bf-level-editor)

set( BF_LE_TARGET_NAME bf-level-editor )

include_directories( .. ${BEAR_FACTORY_EDITOR_INCLUDE_DIRECTORY})

#-------------------------------------------------------------------------------
set( BF_LE_SOURCE_FILES
  code/about_dialog.cpp
  code/arrange.cpp
  code/clone_selection_dialog.cpp
  code/configuration.cpp
  code/drag_info.cpp
  code/edit_mode.cpp
  code/error_check_level_dialog.cpp
  code/grid.cpp
  code/grid_properties_frame.cpp
  code/gui_level.cpp
  code/ingame_view.cpp
  code/ingame_view_frame.cpp
  code/item_choice_frame.cpp
  code/item_selection.cpp
  code/layer.cpp
  code/layer_check_result.cpp
  code/layer_list_frame.cpp
  code/layer_properties_frame.cpp
  code/level.cpp
  code/level_check_result.cpp
  code/level_clipboard.cpp
  code/level_editor.cpp
  code/level_file_xml_reader.cpp
  code/level_file_xml_writer.cpp
  code/level_overview_frame.cpp
  code/level_properties_frame.cpp
  code/level_renderer.cpp
  code/level_runner.cpp
  code/level_thumbnail_ctrl.cpp
  code/main.cpp
  code/main_frame.cpp
  code/offset_selection_dialog.cpp
  code/properties_frame.cpp
  code/run_configuration.cpp
  code/run_configuration_frame.cpp
  code/selection_manager.cpp
  code/slope.cpp
  code/windows_layout.cpp

  history/code/action_add_item.cpp
  history/code/action_add_layer.cpp
  history/code/action_align_selection_bottom.cpp
  history/code/action_align_selection_center_x.cpp
  history/code/action_align_selection_center_y.cpp
  history/code/action_align_selection_left.cpp
  history/code/action_align_selection_right.cpp
  history/code/action_align_selection_top.cpp
  history/code/action_arrange_selection_horizontally.cpp
  history/code/action_arrange_selection_vertically.cpp
  history/code/action_clone_selection.cpp
  history/code/action_copy_selection.cpp
  history/code/action_delete_selection.cpp
  history/code/action_deprioritize.cpp
  history/code/action_group.cpp
  history/code/action_move_in_other_layer.cpp
  history/code/action_move_layer_backward.cpp
  history/code/action_move_layer_forward.cpp
  history/code/action_move_selection.cpp
  history/code/action_paste_items.cpp
  history/code/action_prioritize.cpp
  history/code/action_remove_item.cpp
  history/code/action_remove_layer.cpp
  history/code/action_resize_layer.cpp
  history/code/action_resize_level.cpp
  history/code/action_rotate_selection.cpp
  history/code/action_set_item_bottom.cpp
  history/code/action_set_item_class.cpp
  history/code/action_set_item_fixed_attribute.cpp
  history/code/action_set_item_id.cpp
  history/code/action_set_item_left.cpp
  history/code/action_set_item_position.cpp
  history/code/action_set_item_position_and_size.cpp
  history/code/action_set_item_size.cpp
  history/code/action_set_priority.cpp
  history/code/action_set_slope_curve.cpp
  history/code/level_history.cpp

  item_filter/code/item_filter.cpp
  item_filter/code/item_filter_by_class_name.cpp
  item_filter/code/item_filter_by_class_name_glob.cpp

  xml/code/run_argument_node.cpp
  xml/code/run_arguments_node.cpp
  xml/code/run_configuration_node.cpp
  xml/code/run_configuration_file.cpp
  xml/code/run_items_node.cpp
)

include(${wxWidgets_USE_FILE})

add_executable( ${BF_LE_TARGET_NAME} ${BF_LE_SOURCE_FILES} )

set_target_properties(
  ${BF_LE_TARGET_NAME}
  PROPERTIES
  INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${BEAR_FACTORY_INSTALL_LIBRARY_DIR}"
  )

target_link_libraries(
  ${BF_LE_TARGET_NAME}
  bear-editor
  ${wxWidgets_LIBRARIES}
  ${CLAW_CONFIGURATION_FILE_LIBRARIES}
  ${CLAW_LOGGER_LIBRARIES}
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  )

install(
  TARGETS ${BF_LE_TARGET_NAME}
  DESTINATION "${BEAR_FACTORY_INSTALL_EXECUTABLE_DIR}"
  )
