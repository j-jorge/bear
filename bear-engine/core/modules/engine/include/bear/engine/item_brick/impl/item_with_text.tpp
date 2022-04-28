/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::item_with_text class.
 * \author Julien Jorge
 */

#include "bear/engine/level_globals.hpp"
#include "bear/visual/scene_writing.hpp"

#include <claw/logger/logger.hpp>
#include <libintl.h>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
template<class Base>
bear::engine::item_with_text<Base>::item_with_text()
  : m_text_inside(false), m_stretched_text(false), m_scale_to_fit(false)
{

} // item_with_text::item_with_text()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the item.
 */
template<class Base>
void bear::engine::item_with_text<Base>::build()
{
  super::build();

  if ( (this->get_size().x == 0) && (this->get_size().y == 0) )
    this->set_size( this->get_writing().get_size() );
} // item_with_text::build()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type string.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
template<class Base>
bool bear::engine::item_with_text<Base>::set_string_field
( const std::string& name, const std::string& value )
{
  bool ok = true;

  if (name == "item_with_text.text")
    this->set_text( gettext(value.c_str()) );
  else if ( name == "item_with_text.horizontal_align" )
    this->set_horizontal_align
      ( visual::text_align::horizontal_align_from_string
        ( value, this->get_horizontal_align() ) );
  else if ( name == "item_with_text.vertical_align" )
    this->set_vertical_align
      ( visual::text_align::vertical_align_from_string
        ( value, this->get_vertical_align() ) );
  else
    ok = super::set_string_field(name, value);

  return ok;
} // item_with_text::set_string_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type bool.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
template<class Base>
bool bear::engine::item_with_text<Base>::set_bool_field
( const std::string& name, bool value )
{
  bool ok = true;

  if (name == "item_with_text.text_inside")
    m_text_inside = value;
  else if (name == "item_with_text.stretched_text")
    m_stretched_text = value;
  else if (name == "item_with_text.scale_to_fit")
    m_scale_to_fit = value;
  else
    ok = super::set_bool_field(name, value);

  return ok;
} // item_with_text::set_bool_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets a field of type font.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
template<class Base>
bool bear::engine::item_with_text<Base>::set_font_field
( const std::string& name, visual::font value )
{
  bool ok = true;

  if ( name == "item_with_text.font" )
    this->set_font( value );
  else
    ok = super::set_font_field(name, value);

  return ok;
} // item_with_text::set_font_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do one iteration in the progression of the item.
 * \param elapsed_time Elapsed time since the last call.
 */
template<class Base>
void bear::engine::item_with_text<Base>::progress
( universe::time_type elapsed_time )
{
  super::progress(elapsed_time);

  if ( m_text_inside && ( this->get_writing().get_size()!=this->get_size() ) )
    this->fit_in_box( this->get_size() );
} // item_with_text::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the sprites representing the item.
 * \param visuals (out) The sprites of the item, and their positions.
 */
template<class Base>
void bear::engine::item_with_text<Base>::get_visual
( std::list<scene_visual>& visuals ) const
{
  super::get_visual(visuals);

  if ( !this->get_text().empty() )
    {
      visual::scene_writing e( 0, 0, this->get_writing() );

      if ( m_stretched_text )
        e.set_scale_factor
          ( this->get_width() / this->get_writing().get_width(),
            this->get_height() / this->get_writing().get_height() );
      else if ( m_scale_to_fit )
        {
          double ratio = 
            std::min
            ( this->get_width() / this->get_writing().get_width(),
              this->get_height() / this->get_writing().get_height() );

          e.set_scale_factor( ratio, ratio );
          bear::visual::position_type gap = get_gap_for_scale_to_fit(e);
          e.set_position(gap);
        }

      this->add_visual( e, visuals );
    }
} // item_with_text::get_visual()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the text is drawn only in the bounds of the item.
 * \param b True if the text must be drawn only in the bounds of the item.
 */
template<class Base>
void bear::engine::item_with_text<Base>::set_text_inside( bool b )
{
  m_text_inside = b;

  if ( m_text_inside )
    this->fit_in_box( this->get_size() );
} // item_with_text::set_text_inside()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the text has to be stretched to fit the bounds of the item.
 * \param b True if the text must be stretched.
 */
template<class Base>
void bear::engine::item_with_text<Base>::set_stretched_text( bool b )
{
  m_stretched_text = b;
} // item_with_text::set_stretched_text()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the text has to be scaled to fit the bounds of the item.
 * \param b True if the text must be scaled.
 */
template<class Base>
void bear::engine::item_with_text<Base>::set_scale_to_fit( bool b )
{
  m_scale_to_fit = b;
} // item_with_text::set_scale_to_fit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell the item to adjust its size to the size of the text.
 */
template<class Base>
void bear::engine::item_with_text<Base>::fit_to_text()
{
  this->refresh_writing();
  this->set_size( this->get_writing().get_size() );
} // item_with_text::fit_to_text()

/*----------------------------------------------------------------------------*/
/**
 * \brief Return the gap of the visual according to alignment.
 * \param e The writing on which we compute the gap.
 */
template<class Base>
bear::visual::position_type 
bear::engine::item_with_text<Base>::get_gap_for_scale_to_fit
(const visual::scene_writing& e) const
{
  bear::visual::position_type gap(0,0);

  visual::text_align::horizontal_align h_align = get_horizontal_align();
  visual::text_align::vertical_align v_align = get_vertical_align();

  if ( h_align == visual::text_align::align_center )
    gap.x = ( this->get_width() - 
              e.get_bounding_box().width() ) / 2;
  else if ( h_align == visual::text_align::align_right )
    gap.x = this->get_width() - e.get_bounding_box().width();
 
  if ( v_align == visual::text_align::align_middle )
    gap.y = ( this->get_height() - 
              e.get_bounding_box().height() ) / 2;
  else if ( v_align == visual::text_align::align_top )
    gap.y = this->get_height() - e.get_bounding_box().height();

  return gap;
} // item_with_text<Base>::get_gap_for_scale_to_fit()
