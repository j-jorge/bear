if(WIN32)
  set(platform_files "system/system_api_windows.cpp")
else()
  set(platform_files "system/system_api_default.cpp")
endif()

set(BEAR_BUILD_TAG "" CACHE STRING "The tag to apply to this build.")

option(
  BEAR_USES_FREEDESKTOP
  "Use the FreeDesktop folder specification."
  TRUE)

if(BEAR_BUILD_TAG)
  set(definitions "BEAR_BUILD_TAG=${BEAR_BUILD_TAG}")
endif()

if(BEAR_USES_FREEDESKTOP)
  set(definitions ${definitions} BEAR_USES_FREEDESKTOP)
endif()

add_bear_engine_library(
  NAME
  engine

  FILES
  "base_item.cpp"
  "bitmap_font_loader.cpp"
  "compiled_file.cpp"
  "game.cpp"
  "game_description.cpp"
  "game_initializer.cpp"
  "game_local_client.cpp"
  "game_network.cpp"
  "game_stats.cpp"
  "item_factory.cpp"
  "item_flag_type.cpp"
  "level.cpp"
  "level_globals.cpp"
  "level_loader.cpp"
  "level_object.cpp"
  "libraries_pool.cpp"
  "model_loader.cpp"
  "population.cpp"
  "resource_pool.cpp"
  "scene_visual.cpp"
  "shader_loader.cpp"
  "sprite_loader.cpp"
  "spritepos.cpp"
  "world.cpp"

  "comic/balloon.cpp"
  "comic/item/speaker_item.cpp"
  "comic/layer/balloon_layer.cpp"
  "comic/layer/balloon_placement/balloon_placement.cpp"
  "comic/loader/speaker_item_loader.cpp"
  "comic/message/balloon_layer_add_message.cpp"
  
  "expr/check_item_class.cpp"
  "expr/check_item_instance.cpp"
  "expr/collision_in_expression.cpp"
  "expr/count_items_by_class_name.cpp"
  "expr/get_toggle_status.cpp"
  
  "forced_movement/toggle_in_forced_movement.cpp"
  
  "function/bool_game_variable_getter.cpp"
  "function/bool_level_variable_getter.cpp"
  
  "game_action/game_action_load_level.cpp"
  "game_action/game_action_pop_level.cpp"
  "game_action/game_action_push_level.cpp"
  "game_action/game_action_set_current_level.cpp"
  
  "i18n/android_gettext_translator.cpp"
  "i18n/base_translator.cpp"
  "i18n/gettext_translator.cpp"
  "i18n/translator.cpp"
  
  "item_brick/activable_sides.cpp"
  "item_brick/forced_movement_applicator.cpp"
  "item_brick/forced_movement_repeater.cpp"
  "item_brick/loader/activable_sides_loader.cpp"
  "item_brick/loader/forced_movement_applicator_loader.cpp"
  "item_brick/loader/forced_movement_loader.cpp"
  "item_brick/loader/forced_movement_repeater_loader.cpp"
  "item_brick/loader/single_tweener_loader.cpp"
  "item_brick/loader/z_shift_loader.cpp"
  "item_brick/with_boolean_expression_assignment.cpp"
  "item_brick/with_boolean_expression_creation.cpp"
  "item_brick/with_linear_expression_assignment.cpp"
  "item_brick/with_linear_expression_creation.cpp"
  "item_brick/with_rendering_attributes.cpp"
  "item_brick/with_text.cpp"
  "item_brick/with_toggle.cpp"
  "item_brick/with_trigger.cpp"
  "item_brick/z_shift.cpp"
  
  "layer/base_debugging_layer.cpp"
  "layer/gui_layer.cpp"
  "layer/gui_layer_stack.cpp"
  "layer/layer.cpp"
  "layer/layer_factory.cpp"
  "layer/transition_layer.cpp"
  
  "loader/base_item_loader.cpp"
  "loader/item_loader_base.cpp"
  "loader/item_loader.cpp"
  "loader/item_loader_fallback.cpp"
  "loader/item_loader_map.cpp"

  "message/transition_effect_erase_message.cpp"

  "model/model_action.cpp"
  "model/model_actor.cpp"
  "model/model_mark.cpp"
  "model/model_mark_item.cpp"
  "model/model_mark_placement.cpp"
  "model/model_snapshot.cpp"
  "model/model_snapshot_tweener.cpp"

  "network/client_connection.cpp"
  "network/client_future.cpp"
  "network/client_observer.cpp"
  "network/message_factory.cpp"
  "network/message/sync.cpp"

  "resource_pool/android_resource_pool.cpp"
  "resource_pool/directory_resource_pool.cpp"
  
  "script/call_sequence.cpp"
  "script/method_call.cpp"
  "script/node_parser/node_parser_argument.cpp"
  "script/node_parser/node_parser_argument_list.cpp"
  "script/node_parser/node_parser_call.cpp"
  "script/node_parser/node_parser_call_entry.cpp"
  "script/node_parser/node_parser_call_group.cpp"
  "script/node_parser/node_parser.cpp"
  "script/node_parser/node_parser_file.cpp"
  "script/script_context.cpp"
  "script/script_parser.cpp"
  "script/script_runner.cpp"
  
  "system/android_game_filesystem.cpp"
  "system/android_system_event_manager.cpp"
  "system/base_game_filesystem.cpp"
  "system/default_game_filesystem.cpp"
  "system/default_system_event_manager.cpp"
  "system/freedesktop_game_filesystem.cpp"
  "system/game_filesystem.cpp"
  
  "transition_effect/fade_effect.cpp"
  "transition_effect/strip_effect.cpp"
  "transition_effect/transition_effect.cpp"
  
  "variable/base_variable.cpp"
  "variable/type_to_string.cpp"
  "variable/variable_copy.cpp"
  "variable/variable_eraser.cpp"
  "variable/variable_list_reader.cpp"
  "variable/variable_saver.cpp"
  "variable/var_map.cpp"

  ${platform_files}
  
  LINK
  bear_audio
  bear_communication
  bear_engine_common
  bear_expr
  bear_gui
  bear_input
  bear_net
  bear_text_interface
  bear_time
  bear_universe
  bear_visual
  bear_debug
  Claw::application
  Claw::dynamic_library
  Claw::tween
  Boost::filesystem
  Boost::regex
  Boost::system

  DEFINITIONS
  ${definitions}
  )
