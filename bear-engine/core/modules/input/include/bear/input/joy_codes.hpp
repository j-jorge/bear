/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This file contains all joystick button codes values and must not be
 *        included  anywhere but in the input/joystick.hpp file.
 *
 * Values are here only for readability, but are members of the input::joystick
 * class.
 */
static const joy_code jc_axis_up         = 0;
static const joy_code jc_axis_down       = 1;
static const joy_code jc_axis_left       = 2;
static const joy_code jc_axis_right      = 3;
static const joy_code jc_axis_up_left    = 4;
static const joy_code jc_axis_up_right   = 5;
static const joy_code jc_axis_down_left  = 6;
static const joy_code jc_axis_down_right = 7;

static const joy_code jc_button_1  = 8;
static const joy_code jc_button_2  = 9;
static const joy_code jc_button_3  = 10;
static const joy_code jc_button_4  = 11;
static const joy_code jc_button_5  = 12;
static const joy_code jc_button_6  = 13;
static const joy_code jc_button_7  = 14;
static const joy_code jc_button_8  = 15;
static const joy_code jc_button_9  = 16;
static const joy_code jc_button_10 = 17;
static const joy_code jc_button_11 = 18;
static const joy_code jc_button_12 = 19;
static const joy_code jc_button_13 = 20;
static const joy_code jc_button_14 = 21;
static const joy_code jc_button_15 = 22;
static const joy_code jc_button_16 = 23;

/**
 * \brief Code representing an invalid code.
 * \remark Currently the same as c_key_codes_count.
 */
static const joy_code jc_invalid = 24;

/** \brief Number of valid key codes. */
static const unsigned int c_number_of_buttons = 16;

private:
static const joy_code jc_range_min = jc_axis_up;
static const joy_code jc_range_max = jc_button_16;
