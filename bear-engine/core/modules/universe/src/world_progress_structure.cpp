/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::universe::selection_structure class.
 * \author Julien Jorge
 */
#include "bear/universe/world_progress_structure.hpp"

#include "bear/universe/physical_item.hpp"

#include <algorithm>

namespace bear
{
  namespace universe
  {
    namespace detail
    {
      static constexpr std::uint32_t initialized( 1 << 0 );
      static constexpr std::uint32_t is_selected( 1 << 1 );
      static constexpr std::uint32_t was_selected( 1 << 2 );
      static constexpr std::uint32_t move_is_done( 1 << 3 );
      static constexpr std::uint32_t is_waiting_for_collision( 1 << 4 );
    }
  }
}

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param item The item with which we compute the intersection.
 */
bear::universe::world_progress_structure::lt_collision::lt_collision
( const physical_item& item )
  : m_item(item)
{

} // world_progress_structure::lt_collision::lt_collision()

/*----------------------------------------------------------------------------*/
/**
 * \brief Comparator.
 * \param a The first item to compare.
 * \param b The second item to compare.
 */
bool bear::universe::world_progress_structure::lt_collision::operator()
  ( const physical_item* a, const physical_item* b ) const
{
  bool result;

  if ( a->get_mass() == b->get_mass() )
    {
      double area_a(0);
      double area_b(0);

      const rectangle_type& r( m_item.get_bounding_box() );
      const rectangle_type& a_box( a->get_bounding_box() );

      if ( a_box.intersects(r) )
        area_a = a_box.intersection(r).area();

      const rectangle_type& b_box( b->get_bounding_box() );
      
      if ( b_box.intersects(r) )
        area_b = b_box.intersection(r).area();

      result = area_a < area_b;
    }
  else
    result = a->get_mass() < b->get_mass();

  return result;
} // world_progress_structure::lt_collision::operator()()




/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param item The item that can be selected.
 */
bear::universe::world_progress_structure::world_progress_structure
( physical_item& item )
  : m_item(item), m_collision_mass(0), m_collision_area(0), m_flags( 0 )
{

} // world_progress_structure::world_progress_structure()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the structure, if not already initialized.
 */
void bear::universe::world_progress_structure::init()
{
  if ( m_flags & detail::initialized )
    return;
  
  m_initial_state = m_item;
  m_flags |= detail::initialized;

  m_collision_neighborhood.clear();
  m_collision_mass = 0;
  m_collision_area = 0;

  m_already_met.clear();
} // world_progress_structure::init()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove the selection.
 */
void bear::universe::world_progress_structure::deinit()
{
  CLAW_PRECOND( m_flags & detail::initialized );

  if ( is_selected() )
    m_flags = detail::was_selected;
  else
    m_flags = 0;

  m_collision_neighborhood.clear();
  m_collision_mass = 0;
  m_collision_area = 0;

  m_already_met.clear();
} // world_progress_structure::deinit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Mark the item as selected.
 */
void bear::universe::world_progress_structure::select()
{
  CLAW_PRECOND( m_flags & detail::initialized );
  m_flags |= detail::is_selected;
} // world_progress_structure::select()

/*----------------------------------------------------------------------------*/
/**
 * \brief Mark the item as unselected.
 */
void bear::universe::world_progress_structure::unselect()
{
  m_flags &= ~( detail::is_selected | detail::was_selected );
} // world_progress_structure::select()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the item is selected.
 */
bool bear::universe::world_progress_structure::is_selected() const
{
  CLAW_PRECOND( !( m_flags & detail::is_selected )
                || ( m_flags & detail::initialized ) );
  return m_flags & detail::is_selected;
} // world_progress_structure::is_selected()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the item was previously selected.
 */
bool bear::universe::world_progress_structure::was_selected() const
{
  return m_flags & detail::was_selected;
} // world_progress_structure::was_selected()

/*----------------------------------------------------------------------------*/
/**
 * \brief Mark the item as waiting for collision detection.
 */
void bear::universe::world_progress_structure::set_waiting_for_collision()
{
  m_flags |= detail::is_waiting_for_collision;
} // world_progress_structure::set_waiting_for_collision()

/*----------------------------------------------------------------------------*/
/**
 * \brief Mark the item as not waiting for collision detection.
 */
void bear::universe::world_progress_structure::unset_waiting_for_collision()
{
  m_flags &= ~detail::is_waiting_for_collision;
} // world_progress_structure::unset_waiting_for_collision()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the item is waiting for collision detection.
 */
bool bear::universe::world_progress_structure::is_waiting_for_collision() const
{
  return m_flags & detail::is_waiting_for_collision;
} // world_progress_structure::is_waiting_for_collision()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set that the move of the item has been done.
 */
void bear::universe::world_progress_structure::set_move_done()
{
  m_flags |= detail::move_is_done;
} // world_progress_structure::set_move_done()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the move of the item has been done.
 */
bool bear::universe::world_progress_structure::move_is_done() const
{
  return m_flags & detail::move_is_done;
} // world_progress_structure::move_is_done()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the initial state.
 */
const bear::universe::physical_item_state&
bear::universe::world_progress_structure::get_initial_state() const
{
  CLAW_PRECOND( m_flags & detail::initialized );

  return m_initial_state;
} // world_progress_structure::get_initial_state()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the neighborhood in collisions with the item.
 * \param n The items in collision.
 * \param mass The largest mass of the items in \a n.
 * \param area The largest area of the items in \a n with the same mass
 *        \a mass.
 * \post n.empty() == true.
 */
void bear::universe::world_progress_structure::set_collision_neighborhood
( item_list& n, double mass, double area )
{
  m_collision_neighborhood.clear();
  std::swap( m_collision_neighborhood, n );
  m_collision_mass = mass;
  m_collision_area = area;
} // world_progress_structure::set_collision_neighborhood()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the collision neighborhood of this item.
 * \param n (out) The neighborhood.
 * \post m_collision_neighborhood.empty() == true.
 */
void bear::universe::world_progress_structure::swap_collision_neighborhood
( item_list& n )
{
  std::swap( n, m_collision_neighborhood );
  m_collision_neighborhood.clear();
} // world_progress_structure::swap_collision_neighborhood()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the largest mass of the items in collision with this item.
 */
double bear::universe::world_progress_structure::get_collision_mass() const
{
  return m_collision_mass;
} // world_progress_structure::get_collision_mass()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the largest area of the items in collision with this item, having
 *        the same mass (equal to get_collision_mass()).
 */
double bear::universe::world_progress_structure::get_collision_area() const
{
  return m_collision_area;
} // world_progress_structure::get_collision_area()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell that this item meets an other item.
 * \param item The other item.
 */
void bear::universe::world_progress_structure::meet( physical_item* item )
{
  if ( &m_item < item )
    m_already_met.push_back(item);
  else
    item->get_world_progress_structure().meet(&m_item);
} // world_progress_structure::meet()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if this item meets an other item.
 * \param item The other item.
 */
bool bear::universe::world_progress_structure::has_met
( const physical_item* item ) const
{
  if ( &m_item < item )
    return std::find(m_already_met.begin(), m_already_met.end(), item)
      != m_already_met.end();
  else
    return item->get_world_progress_structure().has_met(&m_item);
} // world_progress_structure::has_met()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the next neighbor to process.
 * \param item The other item.
 */
bear::universe::physical_item*
bear::universe::world_progress_structure::pick_next_neighbor()
{
  physical_item* result(NULL);

  while ( (result==NULL) && !m_collision_neighborhood.empty() )
    {
      const item_list::iterator eit( m_collision_neighborhood.end() );

      item_list::iterator it = std::max_element
        ( m_collision_neighborhood.begin(), eit, lt_collision(m_item) );

      if ( it != eit )
        {
          if ( !has_met(*it) )
            result = *it;

          m_collision_neighborhood.erase(it);
        }
    }

  return result;
} // world_progress_structure::pick_next_neighbor()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update the largest mass and area of the collisions with this item.
 */
bool bear::universe::world_progress_structure::update_collision_penetration()
{
  m_collision_mass = 0;
  m_collision_area = 0;

  item_list::iterator it = m_collision_neighborhood.begin();
  const rectangle_type& item_box( m_item.get_bounding_box() );
  
  while ( it != m_collision_neighborhood.end() )
    {
      bool collision(false);
      const rectangle_type& box( ( *it )->get_bounding_box() );
      
      if ( item_box.intersects( box ) )
        {
          const double a( item_box.intersection( box ).area() );

          if ( a != 0 )
            {
              collision = true;

              if ( (*it)->get_mass() > m_collision_mass )
                {
                  m_collision_mass = (*it)->get_mass();
                  m_collision_area = a;
                }
              else if ( (*it)->get_mass() == m_collision_mass )
                {
                  m_collision_mass = (*it)->get_mass();

                  if ( a > m_collision_area )
                    m_collision_area = a;
                }
            }
        }

      if ( collision )
        ++it;
      else
        it = m_collision_neighborhood.erase( it );
    }

  return !m_collision_neighborhood.empty();
} // world_progress_structure::update_collision_penetration()
