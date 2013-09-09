/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::universe::physical_item class.
 * \author Julien Jorge
 */
#include "universe/physical_item.hpp"

#include <algorithm>

#include "universe/collision_align_policy.hpp"
#include "universe/collision_info.hpp"
#include "universe/collision_repair.hpp"
#include "universe/item_handle.hpp"
#include "universe/world.hpp"
#include "universe/zone.hpp"
#include "universe/forced_movement/natural_forced_movement.hpp"
#include "universe/link/base_link.hpp"

#include <claw/assert.hpp>
#include <claw/functional.hpp>
#include <claw/logger.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::universe::physical_item::physical_item()
  : m_owner(NULL), m_world_progress_structure(*this), m_age(0)
{

} // physical_item::physical_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 * \param that The item to copy from.
 * \remark Links are not copied.
 */
bear::universe::physical_item::physical_item( const physical_item& that )
  : physical_item_state(that), m_owner(NULL), m_world_progress_structure(*this),
    m_age(0) // new item, new age
{
  set_forced_movement( that.m_forced_movement );
} // physical_item::physical_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::universe::physical_item::~physical_item()
{
  remove_all_links();
  remove_all_handles();
} // physical_item::~physical_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the forced movement applied to this item, if any.
 * \pre this->has_force_movement()
 */
bear::universe::forced_movement&
bear::universe::physical_item::get_forced_movement()
{
  return m_forced_movement;
} // physical_item::get_forced_movement()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply a forced movement to the item.
 * \param m The movement to apply.
 * \remark If this object already have a movement, il will be released.
 */
void
bear::universe::physical_item::set_forced_movement( const forced_movement& m )
{
  clear_forced_movement();

  if ( is_fixed() )
    claw::logger << claw::log_warning
                 << "physical_item::set_forced_movement(): setting a "
                 << "forced movement but the item is fixed." << std::endl;

  set_acceleration( force_type(0, 0) );

  m_forced_movement = m;

  if ( !m_forced_movement.is_null() )
    {
      m_forced_movement.set_item(*this);
      m_forced_movement.init();
    }
} // physical_item::set_forced_movement()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check if the item has a forced movement.
 */
bool bear::universe::physical_item::has_forced_movement() const
{
  return !m_forced_movement.is_null();
} // physical_item::has_forced_movement()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove the forced movement, if any.
 */
void bear::universe::physical_item::clear_forced_movement()
{
  m_forced_movement.clear();
} // physical_item::clear_forced_movement()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the item relative to which this item moves.
 * \param item The item.
 */
void bear::universe::physical_item::set_movement_reference
( const physical_item* item )
{
  m_movement_reference = item;
} // physical_item::set_movement_reference()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the item relative to which this item moves.
 */
const bear::universe::physical_item*
bear::universe::physical_item::get_movement_reference() const
{
  const physical_item* result = NULL;

  if ( !m_forced_movement.is_null() )
    if ( m_forced_movement.has_reference_item() )
      result = &m_forced_movement.get_reference_item();

  if ( result == NULL )
    result = m_movement_reference.get();

  return result;
} // physical_item::get_movement_reference()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if this item is linked to an other item.
 * \param item The other item.
 */
bool bear::universe::physical_item::is_linked_to
( const physical_item& item ) const
{
  link_list_type::const_iterator it;
  bool result(false);

  for (it=m_links.begin(); !result && (it!=m_links.end()); ++it)
    result = ( ( &(*it)->get_first_item() == this )
               && ( &(*it)->get_second_item() == &item ) )
      || ( ( &(*it)->get_first_item() == &item )
           && ( &(*it)->get_second_item() == this ) );

  return result;
} // physical_item::is_linked_to()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if this item is linked to an other item with a given link.
 * \param item The other item.
 * \param id The identifier of the link.
 */
bool bear::universe::physical_item::is_linked_to
( const physical_item& item, std::size_t id ) const
{
  link_list_type::const_iterator it;
  bool result(false);

  for (it=m_links.begin(); !result && (it!=m_links.end()); ++it)
    if ( (*it)->get_id() == id )
      result = ( ( &(*it)->get_first_item() == this )
                 && ( &(*it)->get_second_item() == &item ) )
        || ( ( &(*it)->get_first_item() == &item )
             && ( &(*it)->get_second_item() == this ) );

  return result;
} // physical_item::is_linked_to()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the age of this item.
 */
bear::universe::time_type bear::universe::physical_item::get_age() const
{
  return m_age;
} // physical_item::get_age()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the items concerned by a progress/move of this one.
 * \param d (out) A list to which are added such items.
 */
void bear::universe::physical_item::get_dependent_items( item_list& d ) const
{
  // nothing to do
} // physical_item::get_dependent_items()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do one iteration in the progression of the item.
 * \param elapsed_time Elapsed time since the last call.
 */
void bear::universe::physical_item::time_step( time_type elapsed_time )
{
  m_age += elapsed_time;
} // physical_item::time_step()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the item tat he is entering the active region.
 */
void bear::universe::physical_item::enters_active_region()
{
} // physical_item::enters_active_region()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the item tat he is leaving the active region.
 */
void bear::universe::physical_item::leaves_active_region()
{
  get_world_progress_structure().unselect();
} // physical_item::leaves_active_region()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply the movement of the item.
 * \param elapsed_time Elasped time since the last progress.
 */
void bear::universe::physical_item::move( time_type elapsed_time )
{
  default_move(elapsed_time);
} // physical_item::move()

/*----------------------------------------------------------------------------*/
/**
 * \brief Process a collision.
 * \param info Some informations about the collision.
 */
void bear::universe::physical_item::collision( collision_info& info )
{

} // physical_item::collision()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check if this item is in collision with an other item.
 * \param that The item to check the collision with.
 */
bool
bear::universe::physical_item::collides_with( const physical_item& that ) const
{
  return get_shape().intersects( that.get_shape() );
} // physical_item::collides_with()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell this item which world owns him.
 * \param The world that owns this item.
 */
void bear::universe::physical_item::set_owner(world& owner)
{
  m_owner = &owner;
} // physical_item::set_owner()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove the item from its owner.
 */
void bear::universe::physical_item::quit_owner()
{
  clear_forced_movement();
  remove_all_links();
  remove_all_handles();
  m_owner = NULL;
} // physical_item::quit_owner()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the world's selection structure for this item.
 */
bear::universe::world_progress_structure&
bear::universe::physical_item::get_world_progress_structure()
{
  return m_world_progress_structure;
} // physical_item::get_world_progress_structure()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the world's selection structure for this item.
 */
const bear::universe::world_progress_structure&
bear::universe::physical_item::get_world_progress_structure() const
{
  return m_world_progress_structure;
} // physical_item::get_world_progress_structure()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a link in this item.
 * \param link The link to add.
 * \pre The caller is bear::universe::base_link.
 * \pre The link is not already in the item's list.
 * \remark The item keeps a reference on the link.
 */
void bear::universe::physical_item::add_link( base_link& link )
{
  CLAW_PRECOND
    ( std::find(m_links.begin(), m_links.end(), &link) == m_links.end() );

  m_links.push_front(&link);
} // physical_item::add_link()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove a link from this items.
 * \param link The link to remove.
 * \pre The caller is bear::universe::base_link.
 * \pre The link is in the item's list.
 */
void bear::universe::physical_item::remove_link( base_link& link )
{
  CLAW_PRECOND
    ( std::find(m_links.begin(), m_links.end(), &link) != m_links.end() );

  m_links.erase( std::find(m_links.begin(), m_links.end(), &link) );
} // physical_item::remove_link()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator at the begining of the list of links.
 */
bear::universe::physical_item::const_link_iterator
bear::universe::physical_item::links_begin() const
{
  return m_links.begin();
} // physical_item::links_begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator at the end of the list of links.
 */
bear::universe::physical_item::const_link_iterator
bear::universe::physical_item::links_end() const
{
  return m_links.end();
} // physical_item::links_end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove all the links from this items.
 */
void bear::universe::physical_item::remove_all_links()
{
  // The base_link::unlink() method must call physical_item::remove_link() so
  // we don't have to remove it from here.
  while ( !m_links.empty() )
    delete m_links.front();
} // physical_item::remove_all_links()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add an handle to the item.
 * \pre (h != NULL)
 *      && (std::find(m_handles.begin(), m_handles.end(), h)
 *         == m_handles.end())
 */
void bear::universe::physical_item::add_handle( item_handle* h ) const
{
  CLAW_PRECOND( h != NULL );
  CLAW_PRECOND( std::find(m_handles.begin(), m_handles.end(), h)
                == m_handles.end() );

  m_handles.push_front( h );
} // physical_item::add_handle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a handle to the item.
 * \pre (h != NULL)
 *      && (std::find(m_const_handles.begin(), m_const_handles.end(), h)
 *          == m_const_handles.end())
 */
void bear::universe::physical_item::add_handle( const_item_handle* h ) const
{
  CLAW_PRECOND( h != NULL );
  CLAW_PRECOND( std::find(m_const_handles.begin(), m_const_handles.end(), h)
                == m_const_handles.end() );

  m_const_handles.push_front( h );
} // physical_item::add_handle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove a handle to the item.
 * \pre (h != NULL)
 *      && (std::find(m_handles.begin(), m_handles.end(), h)
 *         != m_handles.end())
 */
void bear::universe::physical_item::remove_handle( item_handle* h ) const
{
  CLAW_PRECOND( h != NULL );
  CLAW_PRECOND( std::find(m_handles.begin(), m_handles.end(), h)
                != m_handles.end() );

  m_handles.erase( std::find( m_handles.begin(), m_handles.end(), h ) );
} // physical_item::remove_handle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove an handle to the item.
 * \pre (h != NULL)
 *      && (std::find(m_const_handles.begin(), m_const_handles.end(), h)
 *         != m_const_handles.end())
 */
void bear::universe::physical_item::remove_handle( const_item_handle* h ) const
{
  CLAW_PRECOND( h != NULL );
  CLAW_PRECOND( std::find(m_const_handles.begin(), m_const_handles.end(), h)
                != m_const_handles.end() );

  m_const_handles.erase
    ( std::find( m_const_handles.begin(), m_const_handles.end(), h ) );
} // physical_item::remove_handle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Calculate the new acceleration of the item.
 */
void bear::universe::physical_item::adjust_cinetic()
{
  speed_type s( get_speed() );

  if ( has_owner() )
    {
      const speed_type eps(get_owner().get_speed_epsilon());

      if ( (s.x < eps.x) && (s.x > -eps.x) )
        s.x = 0;
      if ( (s.y < eps.y) && (s.y > -eps.y) )
        s.y = 0;

      set_speed(s);

      if ( (get_angular_speed() < get_owner().get_angular_speed_epsilon()) &&
           (get_angular_speed() > -get_owner().get_angular_speed_epsilon()) )
        set_angular_speed(0);
    }
} // physical_item::adjust_cinetic()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if this item knows which world owns him.
 */
bool bear::universe::physical_item::has_owner() const
{
  return m_owner != NULL;
} // physical_item::has_owner()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the world that owns this item.
 * \pre has_owner() == true
 */
const bear::universe::world& bear::universe::physical_item::get_owner() const
{
  CLAW_PRECOND( has_owner() );
  return *m_owner;
} // physical_item::get_owner()

/*----------------------------------------------------------------------------*/
/**
 * \brief Align the other item of the collision.
 * \param info Some informations about the collision.
 */
bool bear::universe::physical_item::default_collision
( const collision_info& info )
{
  return default_collision( info, collision_align_policy() );
} // physical_item::default_collision()

/*----------------------------------------------------------------------------*/
/**
 * \brief Align the other item of the collision.
 * \param info Some informations about the collision.
 * \param policy The description of how to align the items.
 */
bool bear::universe::physical_item::default_collision
( const collision_info& info, const collision_align_policy& policy )
{
  bool result(false);
  position_type p( info.get_bottom_left_on_contact() );

  switch( info.get_collision_side() )
    {
    case zone::bottom_zone:
      p.y -= get_align_epsilon();
      result = collision_align_bottom(info, p, policy);
      break;
    case zone::top_zone:
      p.y += get_align_epsilon();
      result = collision_align_top(info, p, policy);
      break;
    case zone::middle_left_zone:
      p.x -= get_align_epsilon();
      result = collision_align_left(info, p, policy);
      break;
    case zone::middle_right_zone:
      p.x += get_align_epsilon();
      result = collision_align_right(info, p, policy);
      break;
    case zone::middle_zone:
      result = collision_middle(info, policy);
      break;
    default: { CLAW_ASSERT( false, "Invalid collision side." ); }
    }

  return result;
} // physical_item::default_collision()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if it is interesting to process a collision against a given
 *        item.
 * \param that The other item.
 */
bool bear::universe::physical_item::interesting_collision
( const physical_item& that ) const
{
  return do_interesting_collision( that );
} // physical_item::interesting_collision()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply the default movement of the item (forced_movement or
 *        acceleration and speed).
 * \param elapsed_time Elasped time since the last progress.
 */
void bear::universe::physical_item::default_move( time_type elapsed_time )
{
  if ( has_forced_movement() )
    m_forced_movement.next_position(elapsed_time);
  else
    {
      natural_forced_movement f;
      f.set_item( *this );
      f.init();
      f.next_position( elapsed_time );
    }
} // physical_item::default_move()

/*----------------------------------------------------------------------------*/
/**
 * \brief Align the other item of the collision on the left of \a this.
 * \param info Some informations about the collision.
 */
bool bear::universe::physical_item::collision_align_left
( const collision_info& info )
{
  return collision_align_left( info, collision_align_policy() );
} // physical_item::collision_align_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Align the other item of the collision on the right of \a this.
 * \param info Some informations about the collision.
 */
bool
bear::universe::physical_item::collision_align_right
( const collision_info& info )
{
  return collision_align_right( info, collision_align_policy() );
} // physical_item::collision_align_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Align the other item of the collision on the top of \a this.
 * \param info Some informations about the collision.
 */
bool bear::universe::physical_item::collision_align_top
( const collision_info& info )
{
  return collision_align_top( info, collision_align_policy() );
} // physical_item::collision_align_top()

/*----------------------------------------------------------------------------*/
/**
 * \brief Align the other item of the collision on the bottom of \a this.
 * \param info Some informations about the collision.
 */
bool
bear::universe::physical_item::collision_align_bottom
( const collision_info& info )
{
  return collision_align_bottom( info, collision_align_policy() );
} // physical_item::collision_align_bottom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the contacts for a collision in the middle of \a this.
 * \param info Some informations about the collision.
 */
bool bear::universe::physical_item::collision_middle
( const collision_info& info )
{
  bool result = false;

  if ( !info.other_item().is_phantom() && can_move_items() )
    {
      info.other_item().set_middle_contact();
      set_middle_contact();
    }

  // We don't align

  return result;
} // physical_item::collision_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Align the other item of the collision on the left of \a this.
 * \param info Some informations about the collision.
 * \param policy The description of how to align the items.
 */
bool bear::universe::physical_item::collision_align_left
( const collision_info& info, const collision_align_policy& policy )
{
  position_type pos;

  if ( info.get_collision_side() == zone::middle_left_zone )
    pos = info.get_bottom_left_on_contact();
  else
    pos.set( get_left() - info.other_item().get_width(),
             info.other_item().get_bottom() );

  pos.x -= get_align_epsilon();

  return collision_align_left( info, pos, policy );
} // physical_item::collision_align_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Align the other item of the collision on the right of \a this.
 * \param info Some informations about the collision.
 * \param policy The description of how to align the items.
 */
bool
bear::universe::physical_item::collision_align_right
( const collision_info& info, const collision_align_policy& policy )
{
  position_type pos;

  if ( info.get_collision_side() == zone::middle_right_zone )
    pos = info.get_bottom_left_on_contact();
  else
    pos.set( get_right(), info.other_item().get_bottom() );

  pos.x += get_align_epsilon();

  return collision_align_right( info, pos, policy );
} // physical_item::collision_align_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Align the other item of the collision on the top of \a this.
 * \param info Some informations about the collision.
 * \param policy The description of how to align the items.
 */
bool bear::universe::physical_item::collision_align_top
( const collision_info& info, const collision_align_policy& policy )
{
  position_type pos;

  if ( info.get_collision_side() == zone::top_zone )
    pos = info.get_bottom_left_on_contact();
  else
    pos.set( info.other_item().get_left(), get_top() );

  pos.y += get_align_epsilon();

  return collision_align_top( info, pos, policy );
} // physical_item::collision_align_top()

/*----------------------------------------------------------------------------*/
/**
 * \brief Align the other item of the collision on the bottom of \a this.
 * \param info Some informations about the collision.
 * \param policy The description of how to align the items.
 */
bool
bear::universe::physical_item::collision_align_bottom
( const collision_info& info, const collision_align_policy& policy )
{
  position_type pos;

  if ( info.get_collision_side() == zone::bottom_zone )
    pos = info.get_bottom_left_on_contact();
  else
    pos.set( info.other_item().get_left(),
             get_bottom() - info.other_item().get_height() );

  pos.y -= get_align_epsilon();

  return collision_align_bottom( info, pos, policy );
} // physical_item::collision_align_bottom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the contacts for a collision in the middle of \a this.
 * \param info Some informations about the collision.
 * \param policy The description of how to align the items.
 */
bool bear::universe::physical_item::collision_middle
( const collision_info& info, const collision_align_policy& policy )
{
  bool result = false;

  physical_item& that( info.other_item() );

  if ( !that.is_phantom() && can_move_items() )
    {
      switch( policy.get_contact_mode() )
        {
        case contact_mode::full_contact:
        case contact_mode::range_contact:
          that.set_middle_contact();
          set_middle_contact();
          break;
        case contact_mode::no_contact:
          // nothing to do
          break;
        }
    }

  // We don't align

  return result;
} // physical_item::collision_middle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Align the other item of the collision on the left of \a this.
 * \param info Some informations about the collision.
 * \param pos The bottom left position to put the other item at.
 */
bool bear::universe::physical_item::collision_align_left
( const collision_info& info, const position_type& pos )
{
  return collision_align_left( info, pos, collision_align_policy() );
} // physical_item::collision_align_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Align the other item of the collision on the right of \a this.
 * \param info Some informations about the collision.
 * \param pos The bottom left position to put the other item at.
 */
bool bear::universe::physical_item::collision_align_right
( const collision_info& info, const position_type& pos )
{
  return collision_align_right( info, pos, collision_align_policy() );
} // physical_item::collision_align_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Align the other item of the collision on the top of \a this.
 * \param info Some informations about the collision.
 * \param pos The bottom left position to put the other item at.
 */
bool bear::universe::physical_item::collision_align_top
( const collision_info& info, const position_type& pos )
{
  return collision_align_top( info, pos, collision_align_policy() );
} // physical_item::collision_align_top()

/*----------------------------------------------------------------------------*/
/**
 * \brief Align the other item of the collision on the bottom of \a this.
 * \param info Some informations about the collision.
 * \param pos The bottom left position to put the other item at.
 */
bool bear::universe::physical_item::collision_align_bottom
( const collision_info& info, const position_type& pos )
{
  return collision_align_bottom( info, pos, collision_align_policy() );
} // physical_item::collision_align_bottom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Align the other item of the collision on the left of \a this.
 * \param info Some informations about the collision.
 * \param pos The bottom left position to put the other item at.
 * \param policy The description of how to align the items.
 */
bool bear::universe::physical_item::collision_align_left
( const collision_info& info, const position_type& pos,
  const collision_align_policy& policy )
{
  bool result(false);

  if ( collision_align_at(info.other_item(), pos) )
    {
      result = true;
    
      physical_item& that = info.other_item();

      switch ( policy.get_contact_mode() )
        {
        case contact_mode::full_contact:
          that.set_right_contact();
          set_left_contact();
          break;
        case contact_mode::range_contact:
          that.set_right_contact( get_bottom(), get_top() );
          set_left_contact( that.get_bottom(), that.get_top() );
          break;
        case contact_mode::no_contact:
          // nothing to do
          break;
        }

      info.get_collision_repair().set_contact_normal
        (info.other_item(), vector_type(-1, 0));
    }

  return result;
} // physical_item::collision_align_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Align the other item of the collision on the right of \a this.
 * \param info Some informations about the collision.
 * \param pos The bottom left position to put the other item at.
 * \param policy The description of how to align the items.
 */
bool bear::universe::physical_item::collision_align_right
( const collision_info& info, const position_type& pos,
  const collision_align_policy& policy )
{
  bool result(false);

  if ( collision_align_at(info.other_item(), pos) )
    {
      result = true;

      physical_item& that = info.other_item();

      switch ( policy.get_contact_mode() )
        {
        case contact_mode::full_contact:
          that.set_left_contact();
          set_right_contact();
          break;
        case contact_mode::range_contact:
          that.set_left_contact( get_bottom(), get_top() );
          set_right_contact( that.get_bottom(), that.get_top() );
          break;
        case contact_mode::no_contact:
          // nothing to do
          break;
        }

      info.get_collision_repair().set_contact_normal
        (info.other_item(), vector_type(1, 0));
    }

  return result;
} // physical_item::collision_align_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Align the other item of the collision on the top of \a this.
 * \param info Some informations about the collision.
 * \param pos The bottom left position to put the other item at.
 * \param policy The description of how to align the items.
 */
bool bear::universe::physical_item::collision_align_top
( const collision_info& info, const position_type& pos,
  const collision_align_policy& policy )
{
  bool result(false);

  if ( collision_align_at(info.other_item(), pos) )
    {
      result = true;

      physical_item& that = info.other_item();

      switch ( policy.get_contact_mode() )
        {
        case contact_mode::full_contact:
          that.set_bottom_contact();
          set_top_contact();
          break;
        case contact_mode::range_contact:
          that.set_bottom_contact( get_left(), get_right() );
          set_top_contact( that.get_left(), that.get_right() );
          break;
        case contact_mode::no_contact:
          // nothing to do
          break;
        }

      info.get_collision_repair().set_contact_normal
        (info.other_item(), vector_type(0, 1));
    }

  return result;
} // physical_item::collision_align_top()

/*----------------------------------------------------------------------------*/
/**
 * \brief Align the other item of the collision on the bottom of \a this.
 * \param info Some informations about the collision.
 * \param pos The bottom left position to put the other item at.
 * \param policy The description of how to align the items.
 */
bool bear::universe::physical_item::collision_align_bottom
( const collision_info& info, const position_type& pos,
  const collision_align_policy& policy )
{
  bool result(false);

  if ( collision_align_at(info.other_item(), pos) )
    {
      result = true;

      physical_item& that = info.other_item();

      switch ( policy.get_contact_mode() )
        {
        case contact_mode::full_contact:
          that.set_top_contact();
          set_bottom_contact();
          break;
        case contact_mode::range_contact:
          that.set_top_contact( get_left(), get_right() );
          set_bottom_contact( that.get_left(), that.get_right() );
          break;
        case contact_mode::no_contact:
          // nothing to do
          break;
        }

      info.get_collision_repair().set_contact_normal
        (info.other_item(), vector_type(0, -1));
    }

  return result;
} // physical_item::collision_align_bottom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Test if the item is only in a given environment.
 * \param e The considered environment.
 */
bool bear::universe::physical_item::is_only_in_environment
(const universe::environment_type e) const
{
  bool result = false;

  if ( has_owner() )
    {
      std::set<universe::environment_type> environments;
      get_owner().get_environments(get_bounding_box(), environments);
      if ( environments.size() == 1 )
        result = ( *(environments.begin()) == e );
    }

  return result;
} // physical_item::is_only_in_environment()

/*----------------------------------------------------------------------------*/
/**
 * \brief Test if the item is in a given environment.
 * \param e The considered environment.
 */
bool bear::universe::physical_item::is_in_environment
(const universe::environment_type e) const
{
  bool result = false;

  if ( has_owner() )
    {
      std::set<universe::environment_type> environments;
      get_owner().get_environments(get_bounding_box(), environments);
      result = ( environments.find(e) != environments.end());
    }

  return result;
} // physical_item::is_in_environment()

/*----------------------------------------------------------------------------*/
/**
 * \brief Align an item at a given position.
 * \param that The other item.
 * \param pos The bottom left position to put the other item at.
 */
bool bear::universe::physical_item::collision_align_at
( physical_item& that, const position_type& pos ) const
{
  bool result(true);

  if ( that.is_phantom() || !can_move_items() )
    result = false;
  else
    that.set_bottom_left( pos );

  return result;
} // physical_item::collision_align_at()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove (invalidates) all the handles on this item.
 */
void bear::universe::physical_item::remove_all_handles() const
{
  // force a call to remove_handle, that will remove the handle from the list
  while ( !m_handles.empty() )
    *m_handles.front() = NULL;

  // force a call to remove_handle, that will remove the handle from the list
  while ( !m_const_handles.empty() )
    *m_const_handles.front() = NULL;

} // physical_item::remove_all_handles()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the epsilon to apply when the item is aligned.
 */
bear::universe::coordinate_type
bear::universe::physical_item::get_align_epsilon() const
{
  if ( has_owner() )
    return get_owner().get_position_epsilon();
  else
    return 0;
} // physical_item::get_align_epsilon()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if it is interesting to process a collision against a given
 *        item.
 * \param that The other item.
 */
bool bear::universe::physical_item::do_interesting_collision
( const physical_item& that ) const
{
  return true;
} // physical_item::do_interesting_collision()
