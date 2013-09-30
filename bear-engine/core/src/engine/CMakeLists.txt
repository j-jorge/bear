set( ENGINE_TARGET_NAME bear_engine )

#-------------------------------------------------------------------------------
set( ENGINE_SOURCE_FILES
  code/base_item.cpp
  code/bitmap_font_loader.cpp
  code/compiled_file.cpp
  code/game.cpp
  code/game_description.cpp
  code/game_initializer.cpp
  code/game_local_client.cpp
  code/game_network.cpp
  code/game_stats.cpp
  code/item_factory.cpp
  code/item_flag_type.cpp
  code/level.cpp
  code/level_globals.cpp
  code/level_loader.cpp
  code/level_object.cpp
  code/libraries_pool.cpp
  code/model_loader.cpp
  code/population.cpp
  code/resource_pool.cpp
  code/shader_loader.cpp
  code/scene_visual.cpp
  code/sprite_loader.cpp
  code/spritepos.cpp
  code/world.cpp

  comic/code/balloon.cpp
  comic/item/code/speaker_item.cpp
  comic/layer/code/balloon_layer.cpp
  comic/layer/balloon_placement/code/balloon_placement.cpp
  comic/loader/code/speaker_item_loader.cpp
  comic/message/code/balloon_layer_add_message.cpp

  expr/code/check_item_class.cpp
  expr/code/check_item_instance.cpp
  expr/code/collision_in_expression.cpp
  expr/code/count_items_by_class_name.cpp
  expr/code/get_toggle_status.cpp

  forced_movement/code/toggle_in_forced_movement.cpp

  function/code/bool_game_variable_getter.cpp
  function/code/bool_level_variable_getter.cpp

  game_action/code/game_action_load_level.cpp
  game_action/code/game_action_pop_level.cpp
  game_action/code/game_action_push_level.cpp
  game_action/code/game_action_set_current_level.cpp

  i18n/code/base_translator.cpp
  i18n/code/gettext_translator.cpp
  i18n/code/translator.cpp

  item_brick/code/activable_sides.cpp
  item_brick/code/forced_movement_applicator.cpp
  item_brick/code/forced_movement_repeater.cpp
  item_brick/code/with_boolean_expression_assignment.cpp
  item_brick/code/with_boolean_expression_creation.cpp
  item_brick/code/with_linear_expression_assignment.cpp
  item_brick/code/with_linear_expression_creation.cpp
  item_brick/code/with_rendering_attributes.cpp
  item_brick/code/with_text.cpp
  item_brick/code/with_toggle.cpp
  item_brick/code/with_trigger.cpp
  item_brick/code/z_shift.cpp

  item_brick/loader/code/activable_sides_loader.cpp
  item_brick/loader/code/forced_movement_applicator_loader.cpp
  item_brick/loader/code/forced_movement_loader.cpp
  item_brick/loader/code/forced_movement_repeater_loader.cpp
  item_brick/loader/code/single_tweener_loader.cpp
  item_brick/loader/code/z_shift_loader.cpp

  layer/code/base_debugging_layer.cpp
  layer/code/gui_layer.cpp
  layer/code/gui_layer_stack.cpp
  layer/code/layer.cpp
  layer/code/layer_factory.cpp
  layer/code/transition_layer.cpp

  loader/code/base_item_loader.cpp
  loader/code/item_loader.cpp
  loader/code/item_loader_base.cpp
  loader/code/item_loader_fallback.cpp
  loader/code/item_loader_map.cpp

  message/code/transition_effect_erase_message.cpp

  model/code/model_action.cpp
  model/code/model_actor.cpp
  model/code/model_mark.cpp
  model/code/model_mark_item.cpp
  model/code/model_mark_placement.cpp
  model/code/model_snapshot.cpp
  model/code/model_snapshot_tweener.cpp
  
  network/code/client_connection.cpp
  network/code/client_future.cpp
  network/code/client_observer.cpp
  network/code/message_factory.cpp
  network/message/code/sync.cpp

  resource_pool/code/directory_resource_pool.cpp

  script/code/call_sequence.cpp
  script/code/method_call.cpp
  script/code/script_context.cpp
  script/code/script_parser.cpp
  script/code/script_runner.cpp

  script/node_parser/code/node_parser.cpp
  script/node_parser/code/node_parser_argument.cpp
  script/node_parser/code/node_parser_argument_list.cpp
  script/node_parser/code/node_parser_call.cpp
  script/node_parser/code/node_parser_call_entry.cpp
  script/node_parser/code/node_parser_call_group.cpp
  script/node_parser/code/node_parser_file.cpp

  system/code/android_system_event_manager.cpp
  system/code/base_game_filesystem.cpp
  system/code/default_game_filesystem.cpp
  system/code/freedesktop_game_filesystem.cpp
  system/code/game_filesystem.cpp

  transition_effect/code/fade_effect.cpp
  transition_effect/code/strip_effect.cpp
  transition_effect/code/transition_effect.cpp

  variable/code/base_variable.cpp
  variable/code/type_to_string.cpp
  variable/code/var_map.cpp
  variable/code/variable_copy.cpp
  variable/code/variable_list_reader.cpp
  variable/code/variable_eraser.cpp
  variable/code/variable_saver.cpp
  )

add_library(
  ${ENGINE_TARGET_NAME}
  ${BEAR_ENGINE_CORE_LINK_TYPE}
  ${ENGINE_SOURCE_FILES}
  )

set_target_properties(
  ${ENGINE_TARGET_NAME}
  PROPERTIES
  INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${BEAR_ENGINE_INSTALL_LIBRARY_DIR}"
  )

set( BEAR_ENGINE_DEFINITIONS )

if( BEAR_BUILD_TAG )
  set(
    BEAR_ENGINE_DEFINITIONS
    ${BEAR_ENGINE_DEFINITIONS}
    "BEAR_BUILD_TAG=${BEAR_BUILD_TAG}"
    )
endif()

if( BEAR_USES_FREEDESKTOP )
  set(
    BEAR_ENGINE_DEFINITIONS
    ${BEAR_ENGINE_DEFINITIONS}
    "BEAR_USES_FREEDESKTOP"
    )
endif()

if( BEAR_ENGINE_DEFINITIONS )
  set_target_properties(
    ${ENGINE_TARGET_NAME}
    PROPERTIES
    COMPILE_DEFINITIONS ${BEAR_ENGINE_DEFINITIONS}
    )
endif()

install(
  TARGETS ${ENGINE_TARGET_NAME}
  DESTINATION ${BEAR_ENGINE_INSTALL_LIBRARY_DIR} )

set(
  ENGINE_LINK_LIBRARIES
  ${SDLMIXER_LIBRARY}
  ${OPENGL_LIBRARIES}
  bear_audio
  bear_communication
  bear_expr
  bear_gui
  bear_input
  bear_net
  bear_text_interface
  bear_time
  bear_universe
  bear_visual
  bear_debug
  ${CLAW_APPLICATION_LIBRARIES}
  ${CLAW_DYNAMIC_LIBRARY_LIBRARIES}
  ${CLAW_GRAPHIC_LIBRARIES}
  ${CLAW_NET_LIBRARIES}
  ${CLAW_TWEEN_LIBRARIES}
  ${Boost_REGEX_LIBRARY}
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SIGNALS_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  )

if(WIN32 OR APPLE)
  set(
    ENGINE_LINK_LIBRARIES
    ${ENGINE_LINK_LIBRARIES}
    intl
    )
endif(WIN32 OR APPLE)

target_link_libraries(
  ${ENGINE_TARGET_NAME}
  ${ENGINE_LINK_LIBRARIES}
  )
