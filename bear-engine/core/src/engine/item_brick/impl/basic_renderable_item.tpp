/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::basic_renderable_item class.
 * \author Sebastien Angibaud
 */

#include "visual/scene_sprite.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Contructor.
 */
template<class Base>
bear::engine::basic_renderable_item<Base>::basic_renderable_item()
  : m_gap(0, 0), m_system_angle_as_visual(false), m_auto_mirror(false),
    m_auto_flip(false), m_auto_angular_speed_factor(0)
{

} // basic_renderable_item::basic_renderable_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialise the item.
 */
template<class Base>
void bear::engine::basic_renderable_item<Base>::on_enters_layer()
{
  super::on_enters_layer();

  m_last_visual_position = this->get_center_of_mass();
} // basic_renderable_item::on_enters_layer()

/*---------------------------------------------------------------------------*/
/**
 * \brief Do one iteration in the progression of the item.
 * \param elapsed_time Elapsed time since the last call.
 */
template<class Base>
void
bear::engine::basic_renderable_item<Base>::progress
( bear::universe::time_type elapsed_time )
{
  super::progress( elapsed_time );

  if ( m_auto_angular_speed_factor != 0 )
    {
      bear::universe::coordinate_type dist
        ( this->get_center_of_mass().distance(m_last_visual_position) );

      if ( this->get_speed().x < 0 )
        this->add_angular_speed
          ( m_auto_angular_speed_factor * dist * 3.14 /
            ( this->get_height() + this->get_width() ) );
      else
        this->add_angular_speed
          ( - m_auto_angular_speed_factor * dist * 3.14 /
            ( this->get_height() + this->get_width() ) );
    }

  m_last_visual_position = this->get_center_of_mass();
} // basic_renderable_item::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c integer.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
template<class Base>
bool bear::engine::basic_renderable_item<Base>::set_integer_field
( const std::string& name, int value )
{
  bool ok = true;

  if (name == "basic_renderable_item.gap.x")
    m_gap.x = value;
  else if (name == "basic_renderable_item.gap.y")
    m_gap.y = value;
  else
    ok = super::set_integer_field(name, value);

  return ok;
} // basic_renderable_item::set_integer_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c real.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
template<class Base>
bool bear::engine::basic_renderable_item<Base>::set_real_field
( const std::string& name, double value )
{
  bool ok = true;

  if (name == "basic_renderable_item.angle")
    get_rendering_attributes().set_angle(value);
  else if (name == "basic_renderable_item.auto_angular_speed_factor")
    m_auto_angular_speed_factor = value;
  else
    ok = super::set_real_field(name, value);

  return ok;
} // basic_renderable_item::set_real_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c bool.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
template<class Base>
bool bear::engine::basic_renderable_item<Base>::set_bool_field
( const std::string& name, bool value )
{
  bool ok = true;

  if (name == "basic_renderable_item.use_system_angle_as_visual_angle")
    set_system_angle_as_visual_angle( value );
  else if (name == "basic_renderable_item.flip")
    get_rendering_attributes().flip( value );
  else if (name == "basic_renderable_item.mirror")
    get_rendering_attributes().mirror( value );
  else if (name == "basic_renderable_item.auto_flip")
    set_auto_flip( value );
  else if (name == "basic_renderable_item.auto_mirror")
    set_auto_mirror( value );
  else
    ok = super::set_bool_field(name, value);

  return ok;
} // basic_renderable_item::set_bool_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c real.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
template<class Base>
bool bear::engine::basic_renderable_item<Base>::set_color_field
( const std::string& name, visual::color value )
{
  bool ok = true;

  if (name == "basic_renderable_item.color")
    get_rendering_attributes().set_color(value);
  else
    ok = super::set_color_field(name, value);

  return ok;
} // basic_renderable_item::set_clor_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Return the gap between the position of the item and the position of
 *        his sprite.
 */
template<class Base>
const bear::visual::position_type&
bear::engine::basic_renderable_item<Base>::get_gap() const
{
  return m_gap;
} // basic_renderable_item::get_gap()

/*----------------------------------------------------------------------------*/
/**
 * \brief Return the gap between the position of the item and the position of
 *        his sprite, with respect to the orientation of the sprite (flipped,
 *        mirrored).
 * \param w The width to consider for the visual.
 * \param h The height to consider for the visual.
 */
template<class Base>
bear::visual::position_type
bear::engine::basic_renderable_item<Base>::get_oriented_gap
( visual::coordinate_type w, visual::coordinate_type h ) const
{
  visual::position_type result(m_gap);

  if ( get_auto_rendering_attributes().is_mirrored() )
    result.x = this->get_width() - result.x - w;

  if ( get_auto_rendering_attributes().is_flipped() )
    result.y = this->get_height() - result.y - h;

  return result;
} // basic_renderable_item::get_oriented_gap()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the gap between the position of the item and the position of his
 *        sprite.
 * \param gap The new value of the gap.
 */
template<class Base>
void bear::engine::basic_renderable_item<Base>::set_gap
( const visual::position_type& gap )
{
  set_gap( gap.x, gap.y );
} // basic_renderable_item::set_gap()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the gap between the position of the item and the position of his
 *        sprite.
 * \param x The new X value of the gap.
 * \param y The new Y value of the gap.
 */
template<class Base>
void bear::engine::basic_renderable_item<Base>::set_gap
( visual::coordinate_type x, visual::coordinate_type y )
{
  set_gap_x(x);
  set_gap_y(y);
} // basic_renderable_item::set_gap()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the gap between the position of the item and the position of his
 *        sprite, on the X-axis.
 * \param x The new value of the gap.
 */
template<class Base>
void bear::engine::basic_renderable_item<Base>::set_gap_x
( visual::coordinate_type x )
{
  m_gap.x = x;
} // basic_renderable_item::set_gap_x()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the gap between the position of the item and the position of his
 *        sprite, on the Y-axis.
 * \param y The new value of the gap.
 */
template<class Base>
void bear::engine::basic_renderable_item<Base>::set_gap_y
( visual::coordinate_type y )
{
  m_gap.y = y;
} // basic_renderable_item::set_gap_y()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the visuals must be mirrored according to the status of the
 *        item.
 * \param b Mirror it or not.
 */
template<class Base>
void bear::engine::basic_renderable_item<Base>::set_auto_mirror( bool b )
{
  m_auto_mirror = b;
} // basic_renderable_item::set_auto_mirror()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the visuals must be flipped according to the status of the
 *        item.
 * \param b Flip it or not.
 */
template<class Base>
void bear::engine::basic_renderable_item<Base>::set_auto_flip( bool b )
{
  m_auto_flip = b;
} // basic_renderable_item::set_auto_flip()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the angle to use to display this item.
 */
template<class Base>
double bear::engine::basic_renderable_item<Base>::get_visual_angle() const
{
  if (m_system_angle_as_visual)
    return this->get_system_angle();
  else
    return 0;
} // basic_renderable_item::get_system_angle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell to use the system angle as the visual angle.
 * \param b Use it or not.
 */
template<class Base>
void bear::engine::basic_renderable_item<Base>::set_system_angle_as_visual_angle
( bool b )
{
  m_system_angle_as_visual = b;
} // basic_renderable_item::set_system_angle_as_visual_angle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the factor of movement applied at angular speed.
 * \param factor The new factor.
 */
template<class Base>
void bear::engine::basic_renderable_item<Base>::set_auto_angular_speed_factor
( double factor)
{
  m_auto_angular_speed_factor = factor;
} // basic_renderable_item::set_auto_angular_speed_factor()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a visual::scene_element in the rendering pipeline.
 * \param v The element to render.
 * \param visuals (out) The sprites of the item, and their positions.
 */
template<class Base>
void bear::engine::basic_renderable_item<Base>::add_visual
( const visual::scene_element& v, std::list<scene_visual>& visuals ) const
{
  visuals.push_front( get_scene_visual(v) );
} // basic_renderable_item::add_visual()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a sprite in the rendering pipeline.
 * \param spr The sprite to render.
 * \param visuals (out) The sprites of the item, and their positions.
 */
template<class Base>
void bear::engine::basic_renderable_item<Base>::add_visual
( const visual::sprite& spr, std::list<scene_visual>& visuals ) const
{
  if ( spr.is_valid() )
    visuals.push_front( get_scene_visual(spr) );
} // basic_renderable_item::add_visual()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a sprite in the rendering pipeline.
 * \param seq The sequence in which we take the sprite to render.
 * \param visuals (out) The sprites of the item, and their positions.
 */
template<class Base>
void bear::engine::basic_renderable_item<Base>::add_visual
( const visual::sprite_sequence& seq, std::list<scene_visual>& visuals ) const
{
  if ( seq.is_valid() )
    add_visual( seq.get_sprite(), visuals );
} // basic_renderable_item::add_visual()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a scene visual as in the rendering pipeline.
 * \param e The initial element.
 */
template<class Base>
bear::engine::scene_visual
bear::engine::basic_renderable_item<Base>::get_scene_visual
( const visual::scene_element& e ) const
{
  visual::scene_element new_e(e);
  const visual::size_box_type s( e.get_rendering_attributes().get_size() );

  visual::position_type local_position( e.get_position() );
  if ( get_auto_rendering_attributes().is_mirrored() )
    local_position.x = -local_position.x;

  if ( get_auto_rendering_attributes().is_flipped() )
    local_position.y = -local_position.y;
  
  new_e.set_position
    ( this->get_bottom_left()
      + local_position
      + get_oriented_gap
      ( new_e.get_bounding_box().width(), new_e.get_bounding_box().height() ) );

  new_e.set_rendering_attributes(get_auto_rendering_attributes());
  new_e.get_rendering_attributes().set_angle
    ( new_e.get_rendering_attributes().get_angle() + this->get_visual_angle() );
  // do not change the size of the element
  new_e.get_rendering_attributes().set_size(s);

  return scene_visual( new_e );
} // basic_renderable_item::get_scene_visual()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a sprite as in the rendering pipeline.
 * \param spr The initial sprite.
 */
template<class Base>
bear::engine::scene_visual
bear::engine::basic_renderable_item<Base>::get_scene_visual
( const visual::sprite& spr ) const
{
  return get_scene_visual( visual::scene_sprite(0, 0, spr) );
} // basic_renderable_item::get_scene_visual()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a sprite as in the rendering pipeline.
 * \param seq The sequence in which we take the sprite.
 * \param visuals (out) The sprites of the item, and their positions.
 */
template<class Base>
bear::engine::scene_visual
bear::engine::basic_renderable_item<Base>::get_scene_visual
( const visual::sprite_sequence& seq ) const
{
  return get_scene_visual( seq.get_sprite() );
} // basic_renderable_item::get_scene_visual()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the rendering attributes with the values infered from the item.
 */
template<class Base>
bear::visual::bitmap_rendering_attributes
bear::engine::basic_renderable_item<Base>::get_auto_rendering_attributes() const
{
  visual::bitmap_rendering_attributes result(get_rendering_attributes());

  if ( m_auto_mirror )
    {
      if ( this->get_acceleration().x != 0 )
        result.mirror
          ( result.is_mirrored() ^ (this->get_acceleration().x < 0) );
      else
        result.mirror( result.is_mirrored() ^ (this->get_speed().x < 0) );
    }

  if ( m_auto_flip )
    {
      if ( this->get_acceleration().y != 0 )
        result.flip( result.is_flipped() ^ (this->get_acceleration().y > 0) );
      else
        result.flip( result.is_flipped() ^ (this->get_speed().y > 0) );
    }

  return result;
} // basic_renderable_item::get_auto_rendering_attributes()
