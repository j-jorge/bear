/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::human_readable class.
 * \author Julien Jorge
 */
#include "bf/human_readable.hpp"

#include <wx/intl.h>

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert a value in a string representation.
 * \param v The value to transform.
 */
wxString bf::human_readable<bf::bool_type>::convert( const value_type& v )
{
  if ( v.get_value() )
    return _("true");
  else
    return _("false");
} // human_readable::convert()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert a value in a string representation.
 * \param v The value to transform.
 */
wxString bf::human_readable<bf::string_type>::convert( const value_type& v )
{
  std::ostringstream oss;

  oss << v;

  return wxT("\"") + std_to_wx_string(oss.str()) + wxT("\"");
} // human_readable::convert()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert a value in a string representation.
 * \param v The value to transform.
 */
wxString bf::human_readable<bf::sprite>::convert( const value_type& v )
{
  std::ostringstream oss;

  oss << "'" << v.get_image_name() << "', x=" << v.get_left()
      << ", y=" << v.get_top() << ", clip_w=" << v.get_clip_width()
      << ", clip_h=" << v.get_clip_height() << ", w=" << v.width()
      << ", h=" << v.height() << ", a=" << v.get_color().get_opacity()
      << ", mirror=" << trinary_logic::to_string(v.get_mirrored_status())
      << ", flip=" << trinary_logic::to_string(v.get_flipped_status());

  return _("sprite:") + std_to_wx_string(oss.str());
} // human_readable::convert()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert a value in a string representation.
 * \param v The value to transform.
 */
wxString bf::human_readable<bf::animation>::convert( const value_type& v )
{
  std::ostringstream oss;

  oss << "a=" << v.get_color().get_opacity() << ", loops=" << v.get_loops()
      << ", mirror=" << trinary_logic::to_string(v.get_mirrored_status())
      << ", flip=" << trinary_logic::to_string(v.get_flipped_status());

  if ( v.get_loop_back() )
    oss << ", loop_back";

  oss << ", first_index=" << v.get_first_index() <<
    ", last_index=" << v.get_last_index();

  return _("animation:") + std_to_wx_string(oss.str());
} // human_readable::convert()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert a value in a string representation.
 * \param v The value to transform.
 */
wxString
bf::human_readable<bf::animation_file_type>::convert( const value_type& v )
{
  return _("animation file:") + std_to_wx_string(v.get_path());
} // human_readable::convert()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert a value in a string representation.
 * \param v The value to transform.
 */
wxString
bf::human_readable<bf::any_animation>::convert( const value_type& v )
{
  if ( v.get_content_type() == any_animation::content_animation )
    return human_readable<animation>::convert( v.get_animation() );
  else
    return human_readable<animation_file_type>::convert
      ( v.get_animation_file() );
} // human_readable::convert()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert a value in a string representation.
 * \param v The value to transform.
 */
wxString bf::human_readable<bf::sample>::convert( const value_type& v )
{
  std::ostringstream oss;

  oss << "'" << v.get_path() << "', loops=" << v.get_loops()
      << ", volume=" << v.get_volume();

  return _("sample:") + std_to_wx_string(oss.str());
} // human_readable::convert()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert a value in a string representation.
 * \param v The value to transform.
 */
wxString bf::human_readable<bf::font>::convert( const value_type& v )
{
  std::ostringstream oss;

  oss << "'" << v.get_font_name() << "', size=" << v.get_size();

  return _("font:") + std_to_wx_string(oss.str());
} // human_readable::convert()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert a value in a string representation.
 * \param v The value to transform.
 */
wxString bf::human_readable<bf::color>::convert( const value_type& v )
{
  std::ostringstream oss;

  oss << " opacity=" << v.get_opacity() 
      << ", red=" << v.get_red_intensity()
      << ", green=" << v.get_green_intensity()
      << ", blue=" << v.get_blue_intensity() ;

  return _("color:") + std_to_wx_string(oss.str());
} // human_readable::convert()
