/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::continuous_link_visual class.
 * \author Julien Jorge
 */
#include "generic_items/link/continuous_link_visual.hpp"

#include "visual/scene_line.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param item The instance loaded by this loader.
 */
bear::continuous_link_visual::loader::loader( continuous_link_visual& item )
  : super("continuous_link_visual"), m_item(item)
{

} // continuous_link_visual::loader::loader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Creates a copy of this instance.
 */
bear::continuous_link_visual::loader*
bear::continuous_link_visual::loader::clone() const
{
  return new loader( *this );
} // continuous_link_visual::loader::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets a field of type \c real.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::continuous_link_visual::loader::set_field
( const std::string& name, double value )
{
  bool result = true;

  if ( name == "line_width" )
    m_item.set_line_width( value );
  else
    result = super::set_field(name, value);

  return result;
} // continuous_link_visual::loader::set_field()





BASE_ITEM_EXPORT( continuous_link_visual, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::continuous_link_visual::continuous_link_visual()
: m_line_width(1)
{

} // continuous_link_visual::continuous_link_visual()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the visuals of this item.
 * \param visuals (out) The visuals.
 */
void bear::continuous_link_visual::get_visual
( std::list<engine::scene_visual>& visuals ) const
{
  visual::sprite s(get_sprite());

  if ( s.is_valid() )
    visuals.push_front( build_sprite_visual( s ) );
  else
    visuals.push_front( build_line_visual() );
} // continuous_link_visual::get_visual()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the width of the line displayed when no sprite has been assigned
 *        to the item.
 * \param w The width of the line.
 */
void bear::continuous_link_visual::set_line_width( double w )
{
  m_line_width = std::max( 0.0, w );
} // continuous_link_visual::set_line_width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adds the loaders of this item class into a given loader map.
 * \param m The map in which the loaders are inserted.
 */
void bear::continuous_link_visual::populate_loader_map
( engine::item_loader_map& m )
{
  super::populate_loader_map(m);

  m.insert( loader( *this ) );
} // continuous_link_visual::populate_loader_map()

/*----------------------------------------------------------------------------*/
/**
 * \brief Builds the visual of the link with a given sprite.
 * \param s The sprite from which we create the visual.
 */
bear::engine::scene_visual
bear::continuous_link_visual::build_sprite_visual( visual::sprite s ) const
{
  const universe::coordinate_type length =
    get_end_position().distance(get_start_position());

  s.set_width(length);

  universe::position_type pos(get_center_of_mass());

  pos.x -= length / 2;
  pos.y += s.height() / 2;

  s.set_angle( std::atan2( get_end_position().y - get_start_position().y,
                           get_end_position().x - get_start_position().x ) );

  return engine::scene_visual( pos, s, get_z_position() );
} // continuous_link_visual::build_sprite_visual()

/*----------------------------------------------------------------------------*/
/**
 * \brief Builds the visual of the link with a given sprite.
 * \param s The sprite from which we create the visual.
 */
bear::engine::scene_visual
bear::continuous_link_visual::build_line_visual() const
{
  std::vector<visual::position_type> p(2);
  p[0] = get_start_position();
  p[1] = get_end_position();

  const visual::color_type color( get_rendering_attributes().get_color() );

  visual::scene_line line( 0, 0, color, p, m_line_width );

  return engine::scene_visual( line, get_z_position() );
} // continuous_link_visual::build_line_visual()
