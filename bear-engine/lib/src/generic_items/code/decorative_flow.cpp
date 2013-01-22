/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::decorative_flow class.
 * \author Sebastie Angibaud
 */
#include "generic_items/decorative_flow.hpp"

#include "engine/item_brick/loader/activable_sides_loader.hpp"

BASE_ITEM_EXPORT( decorative_flow, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::decorative_flow::decorative_flow()
  : m_min_speed_flow(0, 0), m_max_speed_flow(1, 1), m_item_per_second(1),
    m_decoration_size(0, 0)
{
  set_phantom(true);
  set_can_move_items(false);
  set_artificial(true);
} // decorative_flow::decorative_flow()

/*---------------------------------------------------------------------------*/
/**
 * \brief Do one iteration in the progression of the item.
 */
void bear::decorative_flow::progress( universe::time_type elapsed_time )
{
  super::progress(elapsed_time);

  double a = (double)( (1.0 + m_item_per_second * elapsed_time) * rand() /
                       RAND_MAX);
  for ( unsigned int i=1; i <= (unsigned int)a; ++i )
    create_decoration();

  update_decorations(elapsed_time);
} // decorative_flow::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the item.
 */
void bear::decorative_flow::build()
{
  super::build();

  if ( get_animation().is_valid() )
    {
      m_decoration_size = get_animation().get_max_size();
      initiate_decoration();
    }
} // decorative_flow::build()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c real.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool
bear::decorative_flow::set_real_field( const std::string& name, double value )
{
  bool ok = true;

  if (name == "decorative_flow.min_speed.x")
    m_min_speed_flow.x = value;
  else if (name == "decorative_flow.min_speed.y")
    m_min_speed_flow.y = value;
  else if (name == "decorative_flow.max_speed.x")
    m_max_speed_flow.x = value;
  else if (name == "decorative_flow.max_speed.y")
    m_max_speed_flow.y = value;
  else if (name == "decorative_flow.items_per_second")
    m_item_per_second = value;
  else
    ok = super::set_real_field(name, value);

  return ok;
} // decorative_flow::set_real_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the sprites representing the item.
 * \param visuals (out) The sprites of the item, and their positions.
 */
void bear::decorative_flow::get_visual
( std::list<engine::scene_visual>& visuals ) const
{
  visual::sprite spr( get_sprite() );

  if ( spr.is_valid() )
    {
      std::list<universe::position_type>::const_iterator it;

      for ( it=m_decorations.begin(); it!=m_decorations.end(); ++it)
        {
          universe::position_type pos( *it + get_bottom_left() + get_gap() );
          pos.y += m_decoration_size.y;

          visuals.push_front
            ( engine::scene_visual( pos, spr, get_z_position() ) );
        }
    }
} // decorative_flow::get_visual()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add the loaders of this item class into a given loader map.
 * \param m The map in which the loaders are inserted.
 */
void bear::decorative_flow::populate_loader_map( engine::item_loader_map& m )
{
  super::populate_loader_map(m);

  m.insert( engine::activable_sides_loader(*this) );
} // decorative_flow::populate_loader_map()

/*---------------------------------------------------------------------------*/
/**
 * \brief Update positions of decorations.
 */
void bear::decorative_flow::update_decorations
( universe::time_type elapsed_time )
{
  std::list<universe::position_type> new_list;
  std::list<universe::position_type>::const_iterator it;

  for ( it = m_decorations.begin(); it != m_decorations.end(); ++it)
    {
      universe::position_type pos(*it);
      double rand_x = 1.0 * rand() / RAND_MAX;
      double rand_y = 1.0 * rand() / RAND_MAX;

      pos.x += elapsed_time * ( m_min_speed_flow.x +
          ( m_max_speed_flow.x - m_min_speed_flow.x )*rand_x );
      pos.y += elapsed_time * ( m_min_speed_flow.y +
          ( m_max_speed_flow.y - m_min_speed_flow.y )*rand_y );

      if ( (pos.x + m_decoration_size.x <= get_width() ) &&
           (pos.x >= 0 ) &&
           (pos.y >= 0 ) &&
           (pos.y + m_decoration_size.y <= get_height() ) )
        new_list.push_back(pos);
    }

  m_decorations = new_list;
} // decorative_flow::update_positions()

/*---------------------------------------------------------------------------*/
/**
 * \brief Update positions of decorations.
 */
void bear::decorative_flow::create_decoration()
{
  universe::position_type pos(universe::position_type(0, 0));
  double rand_val;

  if ( left_side_is_active() )
    {
      rand_val = get_height() * rand() / RAND_MAX;
      pos.y += rand_val;
      m_decorations.push_back(pos);
    }

  if ( right_side_is_active() )
    {
      pos.x -= m_decoration_size.x;
      rand_val = get_height() * rand() / RAND_MAX;
      pos.y += rand_val;
      m_decorations.push_back(pos);
    }

   if ( bottom_side_is_active() )
    {
      rand_val = get_width() * rand() / RAND_MAX;
      pos.x += rand_val;
      m_decorations.push_back(pos);
    }

   if ( top_side_is_active() )
    {
      pos.y -= m_decoration_size.y;
      rand_val = get_width() * rand() / RAND_MAX;
      pos.x += rand_val;
      m_decorations.push_back(pos);
    }
} // decorative_flow::create_decoration()

/*---------------------------------------------------------------------------*/
/**
 * \brief Initiate decorations.
 */
void bear::decorative_flow::initiate_decoration()
{
  unsigned int max = (unsigned int)(get_bounding_box().area() / 2000);

  for ( unsigned int i = 0; i <= max; ++i)
    {
      double rand_val1(get_width() * rand() / RAND_MAX);
      double rand_val2(get_height() * rand() / RAND_MAX);

      m_decorations.push_back( universe::position_type(rand_val1, rand_val2) );
    }
} // decorative_flow::initiate_decoration()
