cmake_minimum_required(VERSION 2.6)
project(bf-model-editor)

set( BF_ME_TARGET_NAME bf-model-editor )

include_directories( .. ${BEAR_FACTORY_EDITOR_INCLUDE_DIRECTORY})

#-------------------------------------------------------------------------------
set( BF_ME_SOURCE_FILES
  code/about_dialog.cpp
  code/action.cpp
  code/action_list_frame.cpp
  code/action_player.cpp
  code/action_properties_frame.cpp
  code/configuration.cpp
  code/error_check_model_dialog.cpp
  code/gui_model.cpp
  code/model_editor.cpp
  code/model_file_compiler.cpp
  code/model_properties_ctrl.cpp
  code/main.cpp
  code/main_frame.cpp
  code/mark.cpp
  code/mark_list_frame.cpp
  code/mark_properties_frame.cpp
  code/mark_placement.cpp
  code/model.cpp
  code/model_frame.cpp
  code/model_selection.cpp
  code/model_snapshot_tweener.cpp
  code/model_view.cpp
  code/model_view_ctrl.cpp
  code/snapshot.cpp
  code/snapshot_date_frame.cpp
  code/snapshot_frame.cpp
  code/sound_description.cpp
  code/sound_frame.cpp
  code/windows_layout.cpp

  history/code/action_add_action.cpp
  history/code/action_add_mark.cpp
  history/code/action_add_snapshot.cpp
  history/code/action_group.cpp
  history/code/action_modify_placement.cpp
  history/code/action_modify_snapshot_alignment.cpp  
  history/code/action_modify_snapshot_size.cpp
  history/code/action_modify_snapshot_sound.cpp
  history/code/action_move_snapshot.cpp
  history/code/action_remove_action.cpp
  history/code/action_remove_mark.cpp
  history/code/action_remove_snapshot.cpp
  history/code/action_modify_action.cpp
  history/code/action_modify_function.cpp
  history/code/action_modify_mark.cpp
  history/code/action_set_angle_easing.cpp
  history/code/action_set_height_easing.cpp
  history/code/action_set_placement_function.cpp
  history/code/action_set_width_easing.cpp
  history/code/action_set_x_easing.cpp
  history/code/action_set_y_easing.cpp
  history/code/model_history.cpp

  xml/code/model_action_node.cpp
  xml/code/model_file.cpp
  xml/code/model_mark_node.cpp
  xml/code/model_mark_placement_node.cpp
  xml/code/model_mark_placements_node.cpp
  xml/code/model_marks_node.cpp
  xml/code/model_snapshot_node.cpp
  xml/code/model_snapshots_node.cpp
  xml/code/model_sound_node.cpp
)

include(${wxWidgets_USE_FILE})

add_executable( ${BF_ME_TARGET_NAME} ${BF_ME_SOURCE_FILES} )

set_target_properties(
  ${BF_ME_TARGET_NAME}
  PROPERTIES
  INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${BEAR_FACTORY_INSTALL_LIBRARY_DIR}"
  )

target_link_libraries(
  ${BF_ME_TARGET_NAME}
  bear-editor
  ${wxWidgets_LIBRARIES}
  ${CLAW_CONFIGURATION_FILE_LIBRARIES}
  ${CLAW_LOGGER_LIBRARIES}
  ${CLAW_TWEEN_LIBRARIES}
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  )

install(
  TARGETS ${BF_ME_TARGET_NAME}
  DESTINATION "${BEAR_FACTORY_INSTALL_EXECUTABLE_DIR}"
  )
