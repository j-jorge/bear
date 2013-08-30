cmake_minimum_required(VERSION 2.6)
project(bear_generic_items)

set( GENERIC_ITEM_TARGET_NAME bear_generic_items )

include_directories(
  ${BEAR_ENGINE_INCLUDE_DIRECTORY}
  )

#-------------------------------------------------------------------------------
set( GENERIC_ITEM_SOURCE_FILES
  code/ambient_sound.cpp
  code/base_ground.cpp
  code/base_train.cpp
  code/block.cpp
  code/bridge.cpp
  code/browser_launcher_toggle.cpp
  code/camera.cpp
  code/camera_on_object.cpp
  code/camera_shaker.cpp
  code/camera_toggle.cpp
  code/change_camera_size.cpp
  code/clone_toggle.cpp
  code/crossfade_sequence.cpp
  code/cursor.cpp
  code/decorative_effect.cpp
  code/decorative_flow.cpp
  code/decorative_item.cpp
  code/decorative_model.cpp
  code/decorative_rectangle.cpp
  code/decorative_toggle.cpp
  code/delayed_kill_item.cpp
  code/delayed_level_loading.cpp
  code/density_rectangle_creator.cpp
  code/descending_ceiling.cpp
  code/environment_rectangle_creator.cpp
  code/explosion_effect_item.cpp
  code/force_rectangle_creator.cpp
  code/friction_rectangle_creator.cpp
  code/game_quit.cpp
  code/hidden_block.cpp
  code/item_creator.cpp
  code/killer.cpp
  code/level_loader_item.cpp
  code/level_loader_progression_item.cpp
  code/level_loader_toggle.cpp
  code/level_popper_toggle.cpp
  code/level_settings.cpp
  code/line.cpp
  code/lines.cpp
  code/linear_movement_sequence_loop.cpp
  code/mouse_detector.cpp
  code/mouse_over_manager.cpp
  code/path_trace.cpp
  code/path_tracer.cpp
  code/reference_item.cpp
  code/reflecting_decoration.cpp
  code/rocket.cpp
  code/rolling_credits.cpp
  code/slope.cpp
  code/spring.cpp
  code/star.cpp
  code/straight_slope.cpp
  code/teleport_item.cpp
  code/time_scale.cpp
  code/time_scale_on_input_toggle.cpp
  code/timer.cpp
  code/toggle.cpp
  code/toggle_group.cpp
  code/train.cpp
  code/trigger.cpp
  code/tweener_item.cpp
  code/unconditional_killer.cpp
  code/world_parameters.cpp

  debug/code/runtime_setting.cpp

  expr/code/any_input_pressed.cpp
  expr/code/applied_expression.cpp
  expr/code/boolean_constant_creator.cpp
  expr/code/check_item_class_creator.cpp
  expr/code/check_item_instance_creator.cpp
  expr/code/expression_creator.cpp
  expr/code/get_toggle_status_creator.cpp
  expr/code/linear_constant_creator.cpp
  expr/code/logical_not_creator.cpp
  expr/code/timer_value.cpp

  forced_movement/code/applied_forced_movement.cpp
  forced_movement/code/forced_aiming_creator.cpp
  forced_movement/code/forced_goto_creator.cpp
  forced_movement/code/forced_join_creator.cpp
  forced_movement/code/forced_movement_toggle.cpp
  forced_movement/code/forced_movement_destructor_toggle.cpp
  forced_movement/code/forced_path_creator.cpp
  forced_movement/code/forced_rotation_creator.cpp
  forced_movement/code/forced_sequence_creator.cpp
  forced_movement/code/forced_stay_around_creator.cpp
  forced_movement/code/forced_tracking_creator.cpp
  forced_movement/code/forced_translation_creator.cpp
  forced_movement/code/natural_forced_movement_creator.cpp
  forced_movement/code/repeated_forced_movement.cpp
  forced_movement/code/toggle_in_forced_movement_creator.cpp

  game_variable/code/bool_game_variable_getter_creator.cpp
  game_variable/code/bool_game_variable_setter.cpp
  game_variable/code/int_game_variable_setter.cpp
  game_variable/code/string_game_variable_setter.cpp
  game_variable/code/real_game_variable_setter.cpp
  game_variable/code/u_int_game_variable_getter_creator.cpp
  game_variable/code/u_int_game_variable_setter.cpp

  layer/code/action_layer.cpp
  layer/code/decoration_layer.cpp
  layer/code/link_layer.cpp
  layer/code/item_information_layer.cpp
  layer/code/pattern_layer.cpp
  layer/code/physics_layer.cpp
  layer/code/recent_path_layer.cpp
  layer/code/runtime_settings_layer.cpp
  layer/code/wireframe_layer.cpp

  level_variable/code/bool_level_variable_getter_creator.cpp
  level_variable/code/u_int_level_variable_getter_creator.cpp

  link/code/base_link_visual.cpp
  link/code/chain_link_visual.cpp
  link/code/continuous_link_visual.cpp
  link/code/link_remover.cpp
  link/code/link_creator.cpp

  script/code/add_script_actor.cpp
  script/code/script_director.cpp

  shader/code/layer_shader.cpp
  shader/code/shader_variable.cpp

  system/code/system_fullscreen_toggle.cpp
  system/code/system_music_toggle.cpp
  system/code/system_sound_toggle.cpp
  )

if( BUILD_PLATFORM STREQUAL "android" )
  set( GENERIC_ITEM_LINK_TYPE STATIC )
else()
  set( GENERIC_ITEM_LINK_TYPE SHARED )
endif()

add_library(
  ${GENERIC_ITEM_TARGET_NAME}
  ${GENERIC_ITEM_LINK_TYPE}
  ${GENERIC_ITEM_SOURCE_FILES}
  )

set_target_properties(
  ${GENERIC_ITEM_TARGET_NAME}
  PROPERTIES
  INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${BEAR_ENGINE_INSTALL_LIBRARY_DIR}"
  )

install(
  TARGETS ${GENERIC_ITEM_TARGET_NAME}
  DESTINATION ${BEAR_ENGINE_INSTALL_LIBRARY_DIR}
  )

if(WIN32)
  target_link_libraries(
    ${GENERIC_ITEM_TARGET_NAME}
    bear_engine
    bear_gui
    intl
    )
else()
  target_link_libraries(
    ${GENERIC_ITEM_TARGET_NAME}
    bear_engine
    bear_gui
    )
endif()
