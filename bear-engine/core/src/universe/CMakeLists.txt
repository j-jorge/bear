set( UNIVERSE_TARGET_NAME bear_universe )

#-------------------------------------------------------------------------------
set( UNIVERSE_SOURCE_FILES
  alignment/code/align_bottom.cpp
  alignment/code/align_bottom_left.cpp
  alignment/code/align_bottom_right.cpp
  alignment/code/align_left.cpp
  alignment/code/alignment.cpp
  alignment/code/align_right.cpp
  alignment/code/align_top.cpp
  alignment/code/align_top_left.cpp
  alignment/code/align_top_right.cpp

  code/collision_align_policy.cpp
  code/collision_info.cpp
  code/collision_repair.cpp
  code/const_item_handle.cpp
  code/contact_info.cpp
  code/contact_mode.cpp
  code/contact_range.cpp
  code/density_rectangle.cpp
  code/environment_rectangle.cpp
  code/force_rectangle.cpp
  code/friction_rectangle.cpp
  code/item_handle.cpp
  code/item_picking_filter.cpp
  code/physical_item.cpp
  code/physical_item_attributes.cpp
  code/physical_item_state.cpp
  code/world.cpp
  code/world_progress_structure.cpp
  code/zone.cpp

  forced_movement/code/base_forced_movement.cpp
  forced_movement/code/center_of_mass_reference_point.cpp
  forced_movement/code/forced_aiming.cpp
  forced_movement/code/forced_goto.cpp
  forced_movement/code/forced_join.cpp
  forced_movement/code/forced_movement.cpp
  forced_movement/code/forced_movement_function.cpp
  forced_movement/code/forced_rotation.cpp
  forced_movement/code/forced_sequence.cpp
  forced_movement/code/forced_stay_around.cpp
  forced_movement/code/forced_tracking.cpp
  forced_movement/code/forced_translation.cpp
  forced_movement/code/natural_forced_movement.cpp
  forced_movement/code/ratio_reference_point.cpp
  forced_movement/code/reference_point.cpp
  forced_movement/code/sinus_speed_generator.cpp

  internal/code/item_selection.cpp
  
  link/code/base_link.cpp
  link/code/chain_link.cpp
  link/code/link.cpp

  shape/code/curved_box.cpp
  shape/code/shape_base.cpp
  shape/code/rectangle.cpp
  shape/code/shape.cpp
)

add_library(
  ${UNIVERSE_TARGET_NAME}
  ${BEAR_ENGINE_CORE_LINK_TYPE}
  ${UNIVERSE_SOURCE_FILES}
  )

set_target_properties(
  ${UNIVERSE_TARGET_NAME}
  PROPERTIES
  INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${BEAR_ENGINE_INSTALL_LIBRARY_DIR}"
  )

install(
  TARGETS ${UNIVERSE_TARGET_NAME}
  DESTINATION ${BEAR_ENGINE_INSTALL_LIBRARY_DIR}
  )

target_link_libraries(
  ${UNIVERSE_TARGET_NAME}
  ${CLAW_LOGGER_LIBRARIES}
  )
