/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This file contains all mouse button codes values and must not be
 *        included  anywhere but in the input/mouse.hpp file.
 *
 * Values are here only for readability, but are members of the input::mouse
 * class.
 */
static const mouse_code mc_left_button     = 0;
static const mouse_code mc_middle_button   = 1;
static const mouse_code mc_right_button    = 2;
static const mouse_code mc_wheel_up        = 3;
static const mouse_code mc_wheel_down      = 4;

/**
 * \brief Code representing an invalid code.
 * \remark Currently the same as c_mouse_codes_count.
 */
static const mouse_code mc_invalid = 5;

/** \brief Number of valid key codes. */
static const unsigned int c_mouse_codes_count = 5;

private:
static const mouse_code mc_range_min = mc_left_button;
static const mouse_code mc_range_max = mc_wheel_down;
