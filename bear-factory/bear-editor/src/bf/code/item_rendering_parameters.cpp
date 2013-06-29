/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::item_rendering_parameters class.
 * \author Julien Jorge
 */
#include "bf/item_rendering_parameters.hpp"

#include "bf/item_class.hpp"
#include "bf/item_instance.hpp"
#include "bf/stream_conv.hpp"

#include <claw/assert.hpp>
#include <sstream>

/*----------------------------------------------------------------------------*/
const std::string bf::item_rendering_parameters::s_field_name_left =
  "base_item.position.left";
const std::string bf::item_rendering_parameters::s_field_name_bottom =
  "base_item.position.bottom";
const std::string bf::item_rendering_parameters::s_field_name_width =
  "base_item.size.width";
const std::string bf::item_rendering_parameters::s_field_name_height =
  "base_item.size.height";
const std::string 
bf::item_rendering_parameters::s_field_name_extend_on_bounding_box =
  "item_with_decoration.extend_on_bounding_box";
const std::string bf::item_rendering_parameters::s_field_name_depth =
  "base_item.position.depth";
const std::string bf::item_rendering_parameters::s_field_name_gap_x =
  "basic_renderable_item.gap.x";
const std::string bf::item_rendering_parameters::s_field_name_gap_y =
  "basic_renderable_item.gap.y";
const std::string bf::item_rendering_parameters::s_field_name_mirror =
  "basic_renderable_item.mirror";
const std::string bf::item_rendering_parameters::s_field_name_flip =
  "basic_renderable_item.flip";
const std::string bf::item_rendering_parameters::s_field_name_color=
  "basic_renderable_item.color";
const std::string bf::item_rendering_parameters::s_field_name_angle =
  "basic_renderable_item.angle";

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param item The item instance that is displayed.
 */
bf::item_rendering_parameters::item_rendering_parameters( item_instance& item )
  : m_item(item), m_left(0), m_bottom(0), m_width(0), m_height(0), m_pos_z(0),
    m_gap_x(0), m_gap_y(0), m_mirror(false), m_flip(false)
{
  m_sprite = new sprite;
} // item_rendering_parameters::item_rendering_parameters()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 * \param that The item to copy from.
 */
bf::item_rendering_parameters::item_rendering_parameters
( const item_rendering_parameters& that )
  : m_item(that.m_item), m_left(that.m_left), m_bottom(that.m_bottom),
    m_width(that.m_width), m_height(that.m_height), 
    m_extend_on_bounding_box(false), m_pos_z(that.m_pos_z),
    m_gap_x(that.m_gap_x), m_gap_y(that.m_gap_x),
    m_mirror(that.m_mirror), m_flip(that.m_flip)
{
  m_sprite = new sprite(*that.m_sprite);
} // item_rendering_parameters::item_rendering_parameters()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bf::item_rendering_parameters::~item_rendering_parameters()
{
  delete m_sprite;
} // item_rendering_parameters::~item_rendering_parameters()

/*----------------------------------------------------------------------------*/
/**
 * \brief Method automatically called when a field or the item has changed.
 * \param field_name The name of the field that changed.
 */
void
bf::item_rendering_parameters::field_changed( const std::string& field_name )
{
  if (field_name == s_field_name_left)
    m_left = get_field_real( field_name, 0 );
  else if (field_name == s_field_name_bottom)
    m_bottom = get_field_real( field_name, 0 );
  else if (field_name == s_field_name_depth)
    m_pos_z = get_field_int( field_name, 0 );
  else if (field_name == s_field_name_gap_x)
    m_gap_x = get_field_int( field_name, 0 );
  else if (field_name == s_field_name_gap_y)
    m_gap_y = get_field_int( field_name, 0 );
  else if (field_name == s_field_name_width)
    m_width = get_field_real( field_name, 0 );
  else if (field_name == s_field_name_height)
    m_height = get_field_real( field_name, 0 );
  else if (field_name == s_field_name_extend_on_bounding_box)
    m_extend_on_bounding_box = get_field_bool( field_name, false );
  else if (field_name == s_field_name_mirror)
    m_mirror = get_field_bool( field_name, false );
  else if (field_name == s_field_name_flip)
    m_flip = get_field_bool( field_name, false );
  else
    {
      const item_class& c(m_item.get_class());

      if ( c.has_field( field_name, type_field::sprite_field_type )
           || c.has_field( field_name, type_field::animation_field_type ) 
           || field_name == s_field_name_color 
           || field_name == s_field_name_angle )
        *m_sprite = get_sprite_from_item();
    }
} // item_rendering_parameters::field_changed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Reset the rendering parameters.
 */
void bf::item_rendering_parameters::reset()
{
  *m_sprite = get_sprite_from_item();
  init();
} // item_rendering_parameters::reset()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read the default values for the position and size.
 */
void bf::item_rendering_parameters::init()
{
  m_left = get_field_real( s_field_name_left, 0 );
  m_bottom = get_field_real( s_field_name_bottom, 0 );
  m_width = get_field_real( s_field_name_width, 0 );
  m_height = get_field_real( s_field_name_height, 0 );
  m_pos_z = get_field_int( s_field_name_depth, 0 );
  m_gap_x = get_field_int( s_field_name_gap_x, 0 );
  m_gap_y = get_field_int( s_field_name_gap_y, 0 );
  m_extend_on_bounding_box = 
    get_field_bool( s_field_name_extend_on_bounding_box, false );
  m_mirror = get_field_bool( s_field_name_mirror, false );
  m_flip = get_field_bool( s_field_name_flip, false );
} // item_rendering_parameters::init()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the x-position of the item, if any.
 */
double bf::item_rendering_parameters::get_left() const
{
  return m_left;
} // item_rendering_parameters::get_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the x-position of the right side of the item, if any.
 */
double bf::item_rendering_parameters::get_right() const
{
  return get_left() + get_width();
} // item_rendering_parameters::get_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the y-position of the item, if any.
 */
double bf::item_rendering_parameters::get_bottom() const
{
  return m_bottom;
} // item_rendering_parameters::get_bottom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the y-position of the top of the item, if any.
 */
double bf::item_rendering_parameters::get_top() const
{
  return get_bottom() + get_height();
} // item_rendering_parameters::get_top()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the x-position of the middle of the item.
 */
double bf::item_rendering_parameters::get_horizontal_middle() const
{
  return get_left() + get_width() / 2;
} // item_rendering_parameters::get_horizontal_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the y-position of the middle of the item.
 */
double bf::item_rendering_parameters::get_vertical_middle() const
{
  return get_bottom() + get_height() / 2;
} // item_rendering_parameters::get_vertical_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the z-position of the item, if any.
 */
int bf::item_rendering_parameters::get_pos_z() const
{
  return m_pos_z;
} // item_rendering_parameters::get_pos_z()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the width of the item, if any.
 */
double bf::item_rendering_parameters::get_width() const
{
  if ( m_width == 0 )
    return m_sprite->width();
  else
    return m_width;
} // item_rendering_parameters::get_width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the height of the item, if any.
 */
double bf::item_rendering_parameters::get_height() const
{
  if ( m_height == 0 )
    return m_sprite->height();
  else
    return m_height;
} // item_rendering_parameters::get_height()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the x-gap of the picture of the item, if any.
 */
int bf::item_rendering_parameters::get_gap_x() const
{
  return m_gap_x;
} // item_rendering_parameters::get_gap_x()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the y-gap of the picture of the item, if any.
 */
int bf::item_rendering_parameters::get_gap_y() const
{
  return m_gap_y;
} // item_rendering_parameters::get_gap_y()

/*----------------------------------------------------------------------------*/
/**
 * \brief Indicates if the item is mirrored.
 */
bool bf::item_rendering_parameters::is_mirrored() const
{
  return m_mirror;
} // item_rendering_parameters::is_mirrored()

/*----------------------------------------------------------------------------*/
/**
 * \brief Indicates if the item is flipped.
 */
bool bf::item_rendering_parameters::is_flipped() const
{
  return m_flip;
} // item_rendering_parameters::is_flipped()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the item has a sprite.
 */
bool bf::item_rendering_parameters::has_sprite() const
{
  if ( m_sprite->get_image_name().empty() )
    *m_sprite = get_sprite_from_item();

  return !m_sprite->get_image_name().empty();
} // item_rendering_parameters::has_sprite()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the default sprite of this item.
 */
const bf::sprite& bf::item_rendering_parameters::get_sprite() const
{
  if ( m_sprite->get_image_name().empty() )
    *m_sprite = get_sprite_from_item();

  if ( ! m_sprite->get_image_name().empty() && m_extend_on_bounding_box )
    m_sprite->set_size(get_width(), get_height());

  return *m_sprite;
} // item_rendering_parameters::get_sprite()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the default sprite of this item.
 */
bf::sprite& bf::item_rendering_parameters::get_sprite()
{
  if ( m_sprite->get_image_name().empty() )
    *m_sprite = get_sprite_from_item();

  if ( ! m_sprite->get_image_name().empty() && m_extend_on_bounding_box )
    m_sprite->set_size(get_width(), get_height());

  return *m_sprite;
} // item_rendering_parameters::get_sprite()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the position of the item, if it has this kind of field.
 * \param x The new X-position.
 * \param y The new Y-position.
 */
void bf::item_rendering_parameters::set_position( double x, double y )
{
  set_left(x);
  set_bottom(y);
} // item_rendering_parameters::set_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the X-position of the item, if it has this kind of field.
 * \param p The new position.
 */
void bf::item_rendering_parameters::set_left( double p )
{
  set_field_real(s_field_name_left, p );
} // item_rendering_parameters::set_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the Y-position of the item, if it has this kind of field.
 * \param p The new position.
 */
void bf::item_rendering_parameters::set_bottom( double p )
{
  set_field_real(s_field_name_bottom, p );
} // item_rendering_parameters::set_bottom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the size of the item, if it has this kind of field.
 */
void bf::item_rendering_parameters::set_size( double x, double y )
{
  set_field_real(s_field_name_width, x );
  set_field_real(s_field_name_height, y );
} // item_rendering_parameters::set_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a sprite of the item.
 */
bf::sprite bf::item_rendering_parameters::get_sprite_from_item() const
{
  sprite result = get_sprite_from_sprites();

  if ( result.get_image_name().empty() )
    result = get_sprite_from_animations();

  result.set_angle
    ( result.get_angle() + get_field_real("basic_renderable_item.angle", 0) );
  result.mirror
    ( trinary_logic::to_bool(result.get_mirrored_status())
      ^ get_field_bool("basic_renderable_item.mirror", false) );
  result.flip
    ( trinary_logic::to_bool(result.get_flipped_status())
      ^ get_field_bool("basic_renderable_item.flip", false) );
  
  color c(get_field_color("basic_renderable_item.color", color()));

  result.get_color().set_opacity
    ( result.get_color().get_opacity() * c.get_opacity() );
  result.get_color().set_intensity
    ( result.get_color().get_red_intensity() * c.get_red_intensity(),
      result.get_color().get_green_intensity() * c.get_green_intensity(),
      result.get_color().get_blue_intensity() * c.get_blue_intensity() );

  return result;
} // item_rendering_parameters::get_sprite_from_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a sprite in the sprites of the item.
 */
bf::sprite bf::item_rendering_parameters::get_sprite_from_sprites() const
{
  const item_class& my_class(m_item.get_class());
  const std::string default_name("item_with_decoration.sprite");
  sprite result;

  if ( my_class.has_field( default_name, type_field::sprite_field_type ) )
    if ( m_item.has_value( my_class.get_field(default_name) ) )
      m_item.get_value( default_name, result );

  if ( result.get_image_name().empty() )
    result = search_sprite_in_class(my_class);

  return result;
} // item_rendering_parameters::get_sprite_from_sprites()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a sprite in the sprites of the item.
 * \param c The class in which we search the sprites.
 */
bf::sprite bf::item_rendering_parameters::search_sprite_in_class
( const item_class& c ) const
{
  item_class::field_iterator it;
  sprite result;

  for (it=c.field_begin();
       result.get_image_name().empty() && (it!=c.field_end()); ++it)
    if ( it->get_field_type() == type_field::sprite_field_type )
      if ( m_item.has_value(*it) )
        {
          if ( it->is_list() )
            {
              std::list<sprite> v;
              m_item.get_value( it->get_name(), v );
              for( ; !v.empty() && result.get_image_name().empty();
                   v.pop_front() )
                result = v.front();
            }
          else
            m_item.get_value( it->get_name(), result );
        }

  if ( result.get_image_name().empty() )
    {
      item_class::const_super_class_iterator itc;

      for ( itc=c.super_class_begin();
            result.get_image_name().empty() && (itc!=c.super_class_end());
            ++itc)
        result = search_sprite_in_class( *itc );
    }

  return result;
} // item_rendering_parameters::search_sprite_in_class()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a sprite in the animations from the item.
 */
bf::sprite bf::item_rendering_parameters::get_sprite_from_animations() const
{
  const item_class& my_class(m_item.get_class());
  const std::string default_name("item_with_decoration.animation");
  animation result;
  any_animation f;

  if ( my_class.has_field( default_name, type_field::animation_field_type ) )
    if ( m_item.has_value( my_class.get_field(default_name) ) )
      {
        m_item.get_value( default_name, f );
        result = f.get_current_animation();
      }

  if ( result.empty() )
    result = search_animation_in_class( my_class );

  if ( result.empty() )
    return sprite();
  else
    return result.get_sprite(0);
} // item_rendering_parameters::get_sprite_from_animations()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a sprite in the animations of the item.
 * \param c The class in which we search the animations.
 */
bf::animation bf::item_rendering_parameters::search_animation_in_class
( const item_class& c ) const
{
  item_class::field_iterator it;
  animation result;

  for (it=c.field_begin(); result.empty() && (it!=c.field_end()); ++it)
    if ( it->get_field_type() == type_field::animation_field_type )
      if ( m_item.has_value(*it) )
        {
          if ( it->is_list() )
            {
              std::list<any_animation> v;
              m_item.get_value( it->get_name(), v );
              for( ; !v.empty() && result.empty(); v.pop_front() )
                result = v.front().get_current_animation();
            }
          else
            {
              any_animation f;
              m_item.get_value( it->get_name(), f );
              result = f.get_current_animation();
            }
        }

  if ( result.empty() )
    {
      item_class::const_super_class_iterator itc;

      for ( itc=c.super_class_begin();
            result.empty() && (itc!=c.super_class_end());
            ++itc )
        result = search_animation_in_class( *itc );
    }

  return result;
} // item_rendering_parameters::search_animation_in_class()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the value of a field of type real.
 * \param field_name The name of the field to get.
 * \param v The default value to return.
 */
double bf::item_rendering_parameters::get_field_real
( const std::string& field_name, double v ) const
{
  real_type result(v);
  const item_class& my_class(m_item.get_class());

  if ( my_class.has_field( field_name, type_field::real_field_type ) )
    {
      if ( m_item.has_value( my_class.get_field(field_name) ) )
        m_item.get_value( field_name, result );
      else
        {
          std::string def = my_class.get_default_value(field_name);

          if ( !def.empty() )
            {
              std::istringstream iss(def);
              stream_conv<real_type>::read( iss, result );
            }
        }
    }

  return result.get_value();
} // item_rendering_parameters::get_field_real()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the value of a field of type color.
 * \param field_name The name of the field to get.
 * \param v The default value to return.
 */
bf::color bf::item_rendering_parameters::get_field_color
( const std::string& field_name, color c ) const
{
  color result(c);
  const item_class& my_class(m_item.get_class());

  if ( my_class.has_field( field_name, type_field::color_field_type ) )
    {
      if ( m_item.has_value( my_class.get_field(field_name) ) )
        m_item.get_value( field_name, result );
      else
        {
          std::string def = my_class.get_default_value(field_name);

          if ( !def.empty() )
            {
              std::istringstream iss(def);
              stream_conv<color>::read( iss, result );
            }
        }
    }

  return result;
} // item_rendering_parameters::get_field_color()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the value of a field of type int.
 * \param field_name The name of the field to get.
 * \param v The default value to return.
 */
int bf::item_rendering_parameters::get_field_int
( const std::string& field_name, int v ) const
{
  integer_type result(v);
  const item_class& my_class(m_item.get_class());

  if ( my_class.has_field( field_name, type_field::integer_field_type ) )
    {
      if ( m_item.has_value( my_class.get_field(field_name) ) )
        m_item.get_value( field_name, result );
      else
        {
          std::string def = my_class.get_default_value(field_name);

          if ( !def.empty() )
            {
              std::istringstream iss(def);
              stream_conv<integer_type>::read( iss, result );
            }
        }
    }

  return result.get_value();
} // item_rendering_parameters::get_field_int()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the value of a field of type int.
 * \param field_name The name of the field to get.
 * \param v The default value to return.
 */
bool bf::item_rendering_parameters::get_field_bool
( const std::string& field_name, bool v ) const
{
  bool_type result(v);
  const item_class& my_class(m_item.get_class());

  if ( my_class.has_field( field_name, type_field::boolean_field_type ) )
    {
      if ( m_item.has_value( my_class.get_field(field_name) ) )
        m_item.get_value( field_name, result );
      else
        {
          std::string def = my_class.get_default_value(field_name);

          if ( !def.empty() )
            {
              std::istringstream iss(def);
              stream_conv<bool_type>::read( iss, result );
            }
        }
    }

  return result.get_value();
} // item_rendering_parameters::get_field_bool()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a fiel of type real.
 * \param field_name The name of the field to set.
 * \param v The value of the field.
 */
void bf::item_rendering_parameters::set_field_real
( const std::string& field_name, double v )
{
  const item_class& my_class(m_item.get_class());

  if ( my_class.has_field( field_name, type_field::real_field_type ) )
    m_item.set_value( field_name, real_type(v) );
} // item_rendering_parameters::set_field_real()
