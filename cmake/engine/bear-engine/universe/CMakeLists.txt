add_bear_engine_library(
  NAME
  universe

  FILES
  "collision_align_policy.cpp"
  "collision_info.cpp"
  "collision_repair.cpp"
  "const_item_handle.cpp"
  "contact_info.cpp"
  "contact_mode.cpp"
  "contact_range.cpp"
  "density_rectangle.cpp"
  "environment_rectangle.cpp"
  "force_rectangle.cpp"
  "friction_rectangle.cpp"
  "item_handle.cpp"
  "item_picking_filter.cpp"
  "physical_item_attributes.cpp"
  "physical_item.cpp"
  "physical_item_state.cpp"
  "world.cpp"
  "world_progress_structure.cpp"
  "zone.cpp"

  "alignment/align_bottom.cpp"
  "alignment/align_bottom_left.cpp"
  "alignment/align_bottom_right.cpp"
  "alignment/align_left.cpp"
  "alignment/alignment.cpp"
  "alignment/align_right.cpp"
  "alignment/align_top.cpp"
  "alignment/align_top_left.cpp"
  "alignment/align_top_right.cpp"

  "collision_test/bottom_contact_is_lower.cpp"

  "forced_movement/base_forced_movement.cpp"
  "forced_movement/center_of_mass_reference_point.cpp"
  "forced_movement/forced_aiming.cpp"
  "forced_movement/forced_goto.cpp"
  "forced_movement/forced_join.cpp"
  "forced_movement/forced_movement.cpp"
  "forced_movement/forced_movement_function.cpp"
  "forced_movement/forced_rotation.cpp"
  "forced_movement/forced_sequence.cpp"
  "forced_movement/forced_stay_around.cpp"
  "forced_movement/forced_tracking.cpp"
  "forced_movement/forced_translation.cpp"
  "forced_movement/natural_forced_movement.cpp"
  "forced_movement/ratio_reference_point.cpp"
  "forced_movement/reference_point.cpp"
  "forced_movement/sinus_speed_generator.cpp"

  "internal/item_selection.cpp"

  "link/base_link.cpp"
  "link/chain_link.cpp"
  "link/link.cpp"

  "shape/curved_box.cpp"
  "shape/rectangle.cpp"
  "shape/shape_base.cpp"
  "shape/shape.cpp"
  
  LINK
  bear_concept
  Claw::logger
  )
