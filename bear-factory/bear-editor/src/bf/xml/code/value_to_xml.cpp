/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::xml::value_to_xml class.
 * \author Julien Jorge
 */
#include "bf/xml/value_to_xml.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Write the XML attributes of a bitmap_rendering_attributes.
 * \param os The stream in which we save the value.
 * \param att The attributes to write.
 */
void
bf::xml::base_sprite_animation_value_to_xml::bitmap_rendering_attributes_xml
( std::ostream& os, const bitmap_rendering_attributes& att )
{
  os << "auto_size='";

  if ( att.get_auto_size() )
    os << "true";
  else
    os << "false";

  os << "' width='" << att.width() << "' height='" << att.height()
     << "' mirror='" << trinary_logic::to_string(att.get_mirrored_status())
     << "' flip='" << trinary_logic::to_string(att.get_flipped_status());

  os << "' angle='" << att.get_angle();

  os << "' opacity='" << att.get_color().get_opacity() << "' red_intensity='"
     << att.get_color().get_red_intensity() << "' green_intensity='"
     << att.get_color().get_green_intensity() << "' blue_intensity='"
     << att.get_color().get_blue_intensity() << "'";
} // base_sprite_animation_value_to_xml::bitmap_rendering_attributes_xml()

/*----------------------------------------------------------------------------*/
/**
 * \brief Write a XML representation of an animation.
 * \param os The stream in which we write.
 * \param anim The animation to write.
 */
void bf::xml::value_to_xml<bf::animation>::write
( std::ostream& os, const animation& anim )
{
  os << "<animation loops='" << anim.get_loops() << "' first_index='"
     << anim.get_first_index() << "' last_index='" << anim.get_last_index()
     << "' loop_back='";

  if ( anim.get_loop_back() )
    os << "true' ";
  else
    os << "false' ";

  bitmap_rendering_attributes_xml(os, anim);
  os << ">\n";

  animation::const_iterator it;

  for ( it=anim.begin(); it!=anim.end(); ++it )
    {
      os << "  <frame duration='" << it->get_duration() << "'>\n";
      os << "    ";
      value_to_xml<sprite>::write(os, it->get_sprite());
      os << "  </frame>\n";
    }

  os << "</animation>\n";
} // value_to_xml::write() [animation]

/*----------------------------------------------------------------------------*/
/**
 * \brief Write a XML representation of an animation file.
 * \param os The stream in which we write.
 * \param anim The animation file to write.
 */
void bf::xml::value_to_xml<bf::animation_file_type>::write
( std::ostream& os, const animation_file_type& anim )
{
  os << "<animation_file path='"
     << util::replace_special_characters(anim.get_path()) << "' ";

  bitmap_rendering_attributes_xml(os, anim);
  os << "/>\n";
} // value_to_xml::write() [animation_file_type]

/*----------------------------------------------------------------------------*/
/**
 * \brief Write a XML representation of any animation.
 * \param os The stream in which we write.
 * \param anim The animation to write.
 */
void bf::xml::value_to_xml<bf::any_animation>::write
( std::ostream& os, const any_animation& anim )
{
  const any_animation::content_type c = anim.get_content_type();

  if ( c == any_animation::content_file )
    value_to_xml<animation_file_type>::write( os, anim.get_animation_file() );
  else if ( c == any_animation::content_animation )
    value_to_xml<animation>::write( os, anim.get_animation() );
  else
    os << "<!-- Invalid content type. -->";
} // value_to_xml::write() [any_animation]

/*----------------------------------------------------------------------------*/
/**
 * \brief Write a XML representation of a sprite.
 * \param os The stream in which we write.
 * \param spr The sprite to write.
 */
void bf::xml::value_to_xml<bf::sprite>::write
( std::ostream& os, const sprite& spr )
{
  os << "<sprite image='"
     << util::replace_special_characters(spr.get_image_name()) << "' x='"
     << spr.get_left() << "' y='" << spr.get_top() << "' clip_width='"
     << spr.get_clip_width() << "' clip_height='"
     << spr.get_clip_height() << "' spritepos='"
     << util::replace_special_characters(spr.get_spritepos_entry()) << "' ";

  bitmap_rendering_attributes_xml(os, spr);
  os << "/>\n";
} // value_to_xml::write() [sprite]

/*----------------------------------------------------------------------------*/
/**
 * \brief Write a XML representation of a sample.
 * \param os The stream in which we write.
 * \param s The sample to write.
 */
void bf::xml::value_to_xml<bf::sample>::write
( std::ostream& os, const sample& s )
{
  os << "<sample path='"
     << util::replace_special_characters(s.get_path()) << "' loops='"
     << s.get_loops() << "' volume='" << s.get_volume() << "'/>\n";
} // value_to_xml::write() [sample]

/*----------------------------------------------------------------------------*/
/**
 * \brief Write a XML representation of a font.
 * \param os The stream in which we write.
 * \param f The font to write.
 */
void bf::xml::value_to_xml<bf::font>::write
( std::ostream& os, const font& f )
{
  os << "<font path='"
     << util::replace_special_characters(f.get_font_name()) << "' size='"
     << f.get_size() << "'/>\n";
} // value_to_xml::write() [font]

/*----------------------------------------------------------------------------*/
/**
 * \brief Write a XML representation of a color.
 * \param os The stream in which we write.
 * \param f The color to write.
 */
void bf::xml::value_to_xml<bf::color>::write
( std::ostream& os, const color& f )
{
  os << "<color opacity='" << f.get_opacity() 
     << "' red_intensity='" << f.get_red_intensity()
     << "' green_intensity='" << f.get_green_intensity()
     << "' blue_intensity='" << f.get_blue_intensity() 
     << "'/>\n";
} // value_to_xml::write() [color]

/*----------------------------------------------------------------------------*/
/**
 * \brief Write a XML representation of an easing function.
 * \param os The stream in which we write.
 * \param f The value to write.
 */
void bf::xml::value_to_xml<bf::easing_type>::write
( std::ostream& os, const easing_type& f )
{
  os << "<easing function='"
     << bear::easing::function::to_string( f.get_value().get_function() )
     << "' direction='"
     << bear::easing::direction::to_string( f.get_value().get_direction() )
     << "'/>\n";
} // value_to_xml::write() [easing_type]
