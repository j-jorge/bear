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
#include "universe/world_progress_structure.hpp"

#include "universe/physical_item.hpp"

#include <algorithm>

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

      const rectangle_type r(m_item.get_bounding_box());

      if ( a->get_bounding_box().intersects(r) )
        area_a = a->get_bounding_box().intersection(r).area();

      if ( b->get_bounding_box().intersects(r) )
        area_b = b->get_bounding_box().intersection(r).area();

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
  : m_item(item), m_initial_state(NULL), m_is_selected(false),
    m_was_selected(false), m_move_is_done(false),
    m_is_waiting_for_collision(false), m_collision_mass(0), m_collision_area(0)
{

} // world_progress_structure::world_progress_structure()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the structure, if not already initialized.
 */
void bear::universe::world_progress_structure::init()
{
  if ( m_initial_state == NULL )
    {
      m_initial_state = new physical_item_state(m_item);
      m_is_selected = false;
      m_move_is_done = false;
      m_is_waiting_for_collision = false;

      m_collision_neighborhood.clear();
      m_collision_mass = 0;
      m_collision_area = 0;

      m_already_met.clear();
    }
} // world_progress_structure::init()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove the selection.
 */
void bear::universe::world_progress_structure::deinit()
{
  CLAW_PRECOND( m_initial_state != NULL );

  delete m_initial_state;
  m_initial_state = NULL;

  m_was_selected = m_is_selected;
  m_is_selected = false;
  m_move_is_done = false;
  m_is_waiting_for_collision = false;

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
  CLAW_PRECOND( m_initial_state != NULL );
  m_is_selected = true;
} // world_progress_structure::select()

/*----------------------------------------------------------------------------*/
/**
 * \brief Mark the item as unselected.
 */
void bear::universe::world_progress_structure::unselect()
{
  m_is_selected = false;
  m_was_selected = false;
} // world_progress_structure::select()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the item is selected.
 */
bool bear::universe::world_progress_structure::is_selected() const
{
  CLAW_PRECOND( !m_is_selected || (m_initial_state != NULL) );
  return m_is_selected;
} // world_progress_structure::is_selected()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the item was previously selected.
 */
bool bear::universe::world_progress_structure::was_selected() const
{
  return m_was_selected;
} // world_progress_structure::was_selected()

/*----------------------------------------------------------------------------*/
/**
 * \brief Mark the item as waiting for collision detection.
 */
void bear::universe::world_progress_structure::set_waiting_for_collision()
{
  m_is_waiting_for_collision = true;
} // world_progress_structure::set_waiting_for_collision()

/*----------------------------------------------------------------------------*/
/**
 * \brief Mark the item as not waiting for collision detection.
 */
void bear::universe::world_progress_structure::unset_waiting_for_collision()
{
  m_is_waiting_for_collision = false;
} // world_progress_structure::unset_waiting_for_collision()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the item is waiting for collision detection.
 */
bool bear::universe::world_progress_structure::is_waiting_for_collision() const
{
  return m_is_waiting_for_collision;
} // world_progress_structure::is_waiting_for_collision()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set that the move of the item has been done.
 */
void bear::universe::world_progress_structure::set_move_done()
{
  m_move_is_done = true;
} // world_progress_structure::set_move_done()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the move of the item has been done.
 */
bool bear::universe::world_progress_structure::move_is_done() const
{
  return m_move_is_done;
} // world_progress_structure::move_is_done()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the initial state.
 */
const bear::universe::physical_item_state&
bear::universe::world_progress_structure::get_initial_state() const
{
  CLAW_PRECOND( m_initial_state != NULL );

  return *m_initial_state;
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
    m_already_met.push_front(item);
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

  const item_list::iterator eit( m_collision_neighborhood.end() );

  while ( (result==NULL) && !m_collision_neighborhood.empty() )
    {
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
  const item_list::iterator eit( m_collision_neighborhood.end() );
  item_list::iterator it = m_collision_neighborhood.begin();

  m_collision_mass = 0;
  m_collision_area = 0;

  while ( it != eit )
    {
      bool collision(false);

      if ( m_item.get_bounding_box().intersects((*it)->get_bounding_box()) )
        {
          const double a =
            m_item.get_bounding_box().intersection
            ( (*it)->get_bounding_box() ).area();

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
        {
          item_list::iterator rit(it);
          ++it;
          m_collision_neighborhood.erase(rit);
        }
    }

  return !m_collision_neighborhood.empty();
} // world_progress_structure::update_collision_penetration()
