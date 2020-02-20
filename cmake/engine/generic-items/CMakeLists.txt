set(target_name bear_generic_items)
set(target_root ${BEAR_ROOT}/bear-engine/lib)
set(include_root ${target_root}/include)
  
 prepend_string(
  files
  "${target_root}/src/"

  "ambient_sound.cpp"
  "base_ground.cpp"
  "base_train.cpp"
  "block.cpp"
  "bridge.cpp"
  "browser_launcher_toggle.cpp"
  "camera.cpp"
  "camera_on_object.cpp"
  "camera_shaker.cpp"
  "camera_toggle.cpp"
  "change_camera_size.cpp"
  "clone_toggle.cpp"
  "crossfade_sequence.cpp"
  "cursor.cpp"
  "decorative_effect.cpp"
  "decorative_flow.cpp"
  "decorative_item.cpp"
  "decorative_model.cpp"
  "decorative_rectangle.cpp"
  "decorative_toggle.cpp"
  "delayed_kill_item.cpp"
  "delayed_level_loading.cpp"
  "density_rectangle_creator.cpp"
  "descending_ceiling.cpp"
  "environment_rectangle_creator.cpp"
  "explosion_effect_item.cpp"
  "force_rectangle_creator.cpp"
  "friction_rectangle_creator.cpp"
  "game_quit.cpp"
  "hidden_block.cpp"
  "item_creator.cpp"
  "killer.cpp"
  "level_loader_item.cpp"
  "level_loader_progression_item.cpp"
  "level_loader_toggle.cpp"
  "level_popper_toggle.cpp"
  "level_settings.cpp"
  "line.cpp"
  "lines.cpp"
  "linear_movement_sequence_loop.cpp"
  "mouse_detector.cpp"
  "mouse_over_manager.cpp"
  "path_trace.cpp"
  "path_tracer.cpp"
  "reference_item.cpp"
  "reflecting_decoration.cpp"
  "rocket.cpp"
  "rolling_credits.cpp"
  "slope.cpp"
  "spring.cpp"
  "star.cpp"
  "straight_slope.cpp"
  "teleport_item.cpp"
  "time_scale.cpp"
  "time_scale_on_input_toggle.cpp"
  "timer.cpp"
  "toggle.cpp"
  "toggle_group.cpp"
  "train.cpp"
  "trigger.cpp"
  "tweener_item.cpp"
  "unconditional_killer.cpp"
  "world_parameters.cpp"

  "debug/runtime_setting.cpp"

  "expr/any_input_pressed.cpp"
  "expr/applied_expression.cpp"
  "expr/boolean_constant_creator.cpp"
  "expr/check_item_class_creator.cpp"
  "expr/check_item_instance_creator.cpp"
  "expr/check_system_name_creator.cpp"
  "expr/expression_creator.cpp"
  "expr/get_toggle_status_creator.cpp"
  "expr/linear_constant_creator.cpp"
  "expr/logical_not_creator.cpp"
  "expr/timer_value.cpp"

  "forced_movement/applied_forced_movement.cpp"
  "forced_movement/forced_aiming_creator.cpp"
  "forced_movement/forced_goto_creator.cpp"
  "forced_movement/forced_join_creator.cpp"
  "forced_movement/forced_movement_toggle.cpp"
  "forced_movement/forced_movement_destructor_toggle.cpp"
  "forced_movement/forced_path_creator.cpp"
  "forced_movement/forced_rotation_creator.cpp"
  "forced_movement/forced_sequence_creator.cpp"
  "forced_movement/forced_stay_around_creator.cpp"
  "forced_movement/forced_tracking_creator.cpp"
  "forced_movement/forced_translation_creator.cpp"
  "forced_movement/natural_forced_movement_creator.cpp"
  "forced_movement/repeated_forced_movement.cpp"
  "forced_movement/toggle_in_forced_movement_creator.cpp"

  "game_variable/bool_game_variable_getter_creator.cpp"
  "game_variable/bool_game_variable_setter.cpp"
  "game_variable/int_game_variable_setter.cpp"
  "game_variable/string_game_variable_setter.cpp"
  "game_variable/real_game_variable_setter.cpp"
  "game_variable/u_int_game_variable_getter_creator.cpp"
  "game_variable/u_int_game_variable_setter.cpp"

  "layer/action_layer.cpp"
  "layer/decoration_layer.cpp"
  "layer/link_layer.cpp"
  "layer/item_information_layer.cpp"
  "layer/pattern_layer.cpp"
  "layer/physics_layer.cpp"
  "layer/recent_path_layer.cpp"
  "layer/runtime_settings_layer.cpp"
  "layer/wireframe_layer.cpp"

  "level_variable/bool_level_variable_getter_creator.cpp"
  "level_variable/u_int_level_variable_getter_creator.cpp"

  "link/base_link_visual.cpp"
  "link/chain_link_visual.cpp"
  "link/continuous_link_visual.cpp"
  "link/link_remover.cpp"
  "link/link_creator.cpp"

  "script/add_script_actor.cpp"
  "script/script_director.cpp"

  "shader/layer_shader.cpp"
  "shader/shader_variable.cpp"

  "system/system_fullscreen_toggle.cpp"
  "system/system_music_toggle.cpp"
  "system/system_sound_toggle.cpp"
  )

add_library(${target_name} ${files})

target_include_directories(
  ${target_name}
  PUBLIC
  $<BUILD_INTERFACE:${include_root}>
  )

target_link_libraries(${target_name} PUBLIC bear_engine bear_gui)

install(
  DIRECTORY ${include_root}/bear
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )

install(
  TARGETS ${target_name}
  EXPORT ${BEAR_ENGINE_EXPORT}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  )
