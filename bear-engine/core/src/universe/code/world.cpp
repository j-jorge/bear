/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::universe::world class.
 * \author Julien Jorge
 */
#include "universe/world.hpp"

#include "universe/collision_info.hpp"
#include "universe/collision_repair.hpp"
#include "universe/density_rectangle.hpp"
#include "universe/environment_rectangle.hpp"
#include "universe/force_rectangle.hpp"
#include "universe/friction_rectangle.hpp"
#include "universe/link/base_link.hpp"
#include "universe/shape/rectangle.hpp"

#include <algorithm>
#include <cassert>
#include <claw/avl.hpp>
#include <claw/graph.hpp>
#include <claw/graph_algorithm.hpp>

/*----------------------------------------------------------------------------*/
const unsigned int bear::universe::world::s_map_compression = 256;

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param size Size of the world.
 */
bear::universe::world::world( const size_box_type& size )
  : m_time(0),
    m_static_surfaces( (unsigned int)size.x + 1, (unsigned int)size.y + 1,
                       s_map_compression ),
    m_size(size), m_unit(50), m_gravity(0, -9.81*m_unit), m_default_friction(1),
    m_default_environment(air_environment), m_default_density(0),
    m_position_epsilon(0.001), m_speed_epsilon(1, 1),
    m_angular_speed_epsilon(0.01), m_acceleration_epsilon(0.01, 0.01)
{

} // world::world()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::universe::world::~world()
{
  unlock();

  for ( ; !m_friction_rectangle.empty(); m_friction_rectangle.pop_front() )
    delete m_friction_rectangle.front();

  for ( ; !m_force_rectangle.empty(); m_force_rectangle.pop_front() )
    delete m_force_rectangle.front();

  for ( ; !m_density_rectangle.empty(); m_density_rectangle.pop_front() )
    delete m_density_rectangle.front();

  for ( ; !m_environment_rectangle.empty();
        m_environment_rectangle.pop_front() )
    delete m_environment_rectangle.front();
} // world::~world()

/*----------------------------------------------------------------------------*/
/**
 * \brief Call the time_step() method on entities in the active region, then
 *        apply physic rules.
 * \param regions The active regions.
 * \param elapsed_time Elapsed time since the last call of this method.
 */
void bear::universe::world::progress_entities
( const region_type& regions, time_type elapsed_time )
{
  item_list items;
  item_list potential_collision;

  lock();

  // search each item in the active zone and global item
  search_interesting_items(regions, items, potential_collision);
  assert
    ( std::set<physical_item*>(items.begin(), items.end()).size()
      == items.size() );

  // call progress for each interesting item
  progress_items(items, elapsed_time);

  // move the item and apply the links
  progress_physic( elapsed_time, items );

  // collision detection
  detect_collision_all( items, potential_collision );

  // inform living_item if they go out the active zone
  active_region_traffic( items );

  while ( !items.empty() )
    unselect_item( items, items.begin() );

  unlock();

  m_time += elapsed_time;
} // world::progress_entities()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a static item in the world.
 * \param who The item to add.
 */
void bear::universe::world::add_static(physical_item* who)
{
  CLAW_PRECOND( who != NULL );
  CLAW_PRECOND( !locked() );

  who->fix();
  who->set_owner(*this);

  if ( who->is_global() )
    m_global_static_items.push_front( who );

  m_static_surfaces.insert( who );
} // world::add_static()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the elapsed time since the creation of the world.
 */
bear::universe::time_type bear::universe::world::get_world_time() const
{
  return m_time;
} // get_world_time()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the size of the world.
 */
const bear::universe::size_box_type& bear::universe::world::get_size() const
{
  return m_size;
} // world::get_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Print some statistics.
 */
void bear::universe::world::print_stats() const
{
  unsigned int min, max;
  double avg;

  m_static_surfaces.cells_load(min, max, avg);

  claw::logger << claw::log_verbose << "World's size is " << m_size.x << ", "
               << m_size.y << '\n'
               << "Cells' size is " << s_map_compression << '\n'
               << "The loading is (min, max, avg) (" << min << '\t' << max
               << '\t' << avg << ")\n"
               << m_static_surfaces.empty_cells() << " cells are empty\n"
               << "There are " << m_entities.size() << " entities."
               << std::endl;
} // world::print_stats()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the gravity applied to the items.
 */
const bear::universe::force_type& bear::universe::world::get_gravity() const
{
  return m_gravity;
} // world::get_gravity()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the gravity applied to the items.
 * \param g The value of the gravity applied to the items.
 */
void bear::universe::world::set_gravity( const force_type& g )
{
  m_gravity = g;
} // world::set_gravity();

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the gravity applied to the items, call to_world_unit() on the
 *        given value.
 * \param g The value of the gravity applied to the items.
 */
void bear::universe::world::set_scaled_gravity( const force_type& g )
{
  m_gravity.set( to_world_unit(g.x), to_world_unit(g.y) );
} // world::set_scaled_gravity();

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the value under which the absolute value of a coordinate is
 *        considered equals to zero.
 */
bear::universe::coordinate_type
bear::universe::world::get_position_epsilon() const
{
  return m_position_epsilon;
} // world::get_position_epsilon()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the value under which the absolute value of a coordinate is
 *        considered equals to zero.
 * \param epsilon The minimum absolute coordinate.
 */
void bear::universe::world::set_position_epsilon( coordinate_type epsilon )
{
  m_position_epsilon = epsilon;
} // world::set_position_epsilon()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the value under which the absolute value of a coordinate is
 *        considered equals to zero. to_world_unit() is called on the given
 *        value.
 * \param epsilon The minimum absolute coordinate.
 */
void
bear::universe::world::set_scaled_position_epsilon( coordinate_type epsilon )
{
  m_position_epsilon = to_world_unit(epsilon);
} // world::set_scaled_position_epsilon()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the value under which the absolute value of the speed is
 *        considered equals to zero.
 */
const bear::universe::speed_type&
bear::universe::world::get_speed_epsilon() const
{
  return m_speed_epsilon;
} // world::get_speed_epsilon()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the value under which the absolute value of the speed is
 *        considered equals to zero.
 * \param speed The minimum absolute speed.
 */
void bear::universe::world::set_speed_epsilon( const speed_type& speed )
{
  m_speed_epsilon = speed;
} // world::set_speed_epsilon()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the value under which the absolute value of the speed is
 *        considered equals to zero. to_world_unit() is called on the given
 *        value.
 * \param speed The minimum absolute speed.
 */
void bear::universe::world::set_scaled_speed_epsilon( const speed_type& speed )
{
  m_speed_epsilon.set( to_world_unit(speed.x), to_world_unit(speed.y) );
} // world::set_scaled_speed_epsilon()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the value under which the absolute value of the angular speed is
 *        considered equals to zero.
 */
double bear::universe::world::get_angular_speed_epsilon() const
{
  return m_angular_speed_epsilon;
} // world::get_angular_speed_epsilon()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the value under which the absolute value of the angular speed is
 *        considered equals to zero.
 * \param angular_speed The minimum absolute angular speed.
 */
void bear::universe::world::set_angular_speed_epsilon( double angular_speed )
{
  m_angular_speed_epsilon = angular_speed;
} // world::set_angular_speed_epsilon()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the unit of the world. \a u units == 1 meter.
 * \param u The new unit.
 */
void bear::universe::world::set_unit( coordinate_type u )
{
  m_unit = u;
} // world::set_unit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert meters in the units of the world.
 * \param m The value to convert.
 */
bear::universe::coordinate_type
bear::universe::world::to_world_unit( coordinate_type m ) const
{
  return m * m_unit;
} // world::to_world_unit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the unit the default friction applied to the items by the
 *        environment.
 * \param f The new friction.
 */
void bear::universe::world::set_default_friction( double f )
{
  m_default_friction = f;
} // world::set_default_friction()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the average friction in an area of the world.
 * \param r The area to search in.
 */
double
bear::universe::world::get_average_friction( const rectangle_type& r ) const
{
  const double r_area( r.area() );
  double result(0);

  if (r_area != 0)
    {
      std::list<friction_rectangle*>::const_iterator it;
      double sum_area(0);

      for ( it=m_friction_rectangle.begin(); it!=m_friction_rectangle.end();
            ++it )
        if ( r.intersects( (*it)->rectangle ) )
          {
            const double area = r.intersection( (*it)->rectangle ).area();
            result += area / r_area * (*it)->friction;
            sum_area += area;
          }

      if ( sum_area < r_area )
        result += (r_area - sum_area) / r_area * m_default_friction;
    }

  return result;
} // world::get_average_friction()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a region with an environment friction different
 *        from m_default_friction.
 * \param r The region of the world where the friction is different.
 * \param f The friction in this region.
 * \return The friction_rectangle stored by the world. You can change its values
 *         as you need.
 */
bear::universe::friction_rectangle*
bear::universe::world::add_friction_rectangle
( const rectangle_type& r, double f )
{
  m_friction_rectangle.push_front( new friction_rectangle(r, f) );
  return m_friction_rectangle.front();
} // world::add_friction_rectangle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the average force in an area of the world.
 * \param r The area to search in.
 */
bear::universe::force_type
bear::universe::world::get_average_force( const rectangle_type& r ) const
{
  const double r_area( r.area() );
  force_type result(0, 0);

  if (r_area != 0)
    {
      std::list<force_rectangle*>::const_iterator it;

      for ( it=m_force_rectangle.begin(); it!=m_force_rectangle.end();
            ++it )
        if ( r.intersects( (*it)->rectangle ) )
          {
            const double area = r.intersection( (*it)->rectangle ).area();
            result += area / r_area * (*it)->force;
          }
    }

  return result;
} // world::get_average_force()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a region with a force is applied.
 * \param r The region of the world where the force is applied.
 * \param f The force in this region.
 * \return The force_rectangle stored by the world. You can change its values
 *         as you need.
 */
bear::universe::force_rectangle*
bear::universe::world::add_force_rectangle
( const rectangle_type& r, universe::force_type f )
{
  m_force_rectangle.push_front( new force_rectangle(r, f) );
  return m_force_rectangle.front();
} // world::add_force_rectangle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the default density applied to the items by the environment.
 * \param f The new density.
 */
void bear::universe::world::set_default_density( double d )
{
  m_default_density = d;
} // world::set_default_density()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the average density in an area of the world.
 * \param r The area to search in.
 */
double
bear::universe::world::get_average_density( const rectangle_type& r ) const
{
  const double r_area( r.area() );
  double result(0);

  if (r_area != 0)
    {
      std::list<density_rectangle*>::const_iterator it;
      double sum_area(0);

      for ( it=m_density_rectangle.begin(); it!=m_density_rectangle.end();
            ++it )
        if ( r.intersects( (*it)->rectangle ) )
          {
            const double area = r.intersection( (*it)->rectangle ).area();
            result += area / r_area * (*it)->density;
            sum_area += area;
          }

      if ( sum_area < r_area )
        result += (r_area - sum_area) / r_area * m_default_density;
    }

  return result;
} // world::get_average_density()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a region with an environment density different
 *        from m_default_density.
 * \param r The region of the world where the density is different.
 * \param f The density in this region.
 * \return The density_rectangle stored by the world. You can change its values
 *         as you need.
 */
bear::universe::density_rectangle*
bear::universe::world::add_density_rectangle
( const rectangle_type& r, double f )
{
  m_density_rectangle.push_front( new density_rectangle(r, f) );
  return m_density_rectangle.front();
} // world::add_density_rectangle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the environments in an area of the world.
 * \param r The area to search in.
 * \param environments The list of environment
 */
void bear::universe::world::get_environments
( const rectangle_type& r,
  std::set<universe::environment_type>& environments) const
{
  const double r_area( r.area() );

  if (r_area != 0)
    {
      std::list<environment_rectangle*>::const_iterator it;
      double sum_area(0);

      for ( it=m_environment_rectangle.begin();
            it!=m_environment_rectangle.end();
            ++it )
        if ( r.intersects( (*it)->rectangle ) )
          {
            const double area = r.intersection( (*it)->rectangle ).area();
            environments.insert((*it)->environment);
            sum_area += area;
          }

      if ( sum_area < r_area )
        environments.insert(m_default_environment);
    }
} // world::get_environments()


/*----------------------------------------------------------------------------*/
/**
 * \brief Test if a position is in a given environment.
 * \param pos The position to consider.
 * \param environment The environment to check.
 */
bool bear::universe::world::is_in_environment
(const position_type& pos, universe::environment_type environment) const
{
  bool result = false;

  std::list<environment_rectangle*>::const_iterator it;

  for ( it=m_environment_rectangle.begin();
        (it!=m_environment_rectangle.end()) && !result;
        ++it )
    if ( ( (*it)->environment== environment ) &&
         (*it)->rectangle.includes( pos ) )
      result = true;

  return result;
} // world::is_in_environment()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the default environment applied to the items by the
 *        environment.
 * \param e The new environment.
 */
void bear::universe::world::set_default_environment
( const universe::environment_type e )
{
  m_default_environment = e;
} // world::set_default_environment()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a region with a environment is applied.
 * \param r The region of the world whith the environment.
 * \param e The environment in this region.
 * \return The environment_rectangle stored by the world.
 * You can change its values as you need.
 */
bear::universe::environment_rectangle*
bear::universe::world::add_environment_rectangle
( const rectangle_type& r, const universe::environment_type e )
{
  m_environment_rectangle.push_front( new environment_rectangle(r, e) );
  return m_environment_rectangle.front();
} // world::add_environment_rectangle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Pick the items overlapping a given coordinate.
 * \param items (out) The interesting items.
 * \param p The position where the items must be.
 * \param filter The conditions the selected items must satisfy.
 */
void bear::universe::world::pick_items_by_position
( item_list& items, const position_type& p,
  const item_picking_filter& filter ) const
{
  region_type region;
  region.push_front( rectangle_type(p.x - 1, p.y - 1, p.x + 1, p.y + 1) );

  item_list candidates;
  item_list::const_iterator it;
  list_active_items(candidates, region, filter);

  for ( it=candidates.begin(); it!=candidates.end(); ++it )
    if ( (*it)->get_bounding_box().includes(p) )
      items.push_back(*it);
} // world::pick_items_by_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Pick the items in a given rectangle.
 * \param items (out) The interesting items.
 * \param r The rectangle where the items must be.
 * \param filter The conditions the selected items must satisfy.
 */
void bear::universe::world::pick_items_in_rectangle
( item_list& items, const rectangle_type& r,
  const item_picking_filter& filter ) const
{
  region_type region;
  region.push_front(r);

  item_list candidates;
  item_list::const_iterator it;
  list_active_items(candidates, region, filter);

  const rectangle s( r );

  for ( it=candidates.begin(); it!=candidates.end(); ++it )
    if ( (*it)->get_shape().intersects( s ) )
      items.push_back(*it);
} // world::pick_items_in_rectangle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Pick the items in a given area according to a given criterion.
 * \param items (out) The interesting items.
 * \param c The center of the circle where the items must be.
 * \param r The radius of the circle where the items must be.
 * \param filter The conditions the selected items must satisfy.
 */
void bear::universe::world::pick_items_in_circle
( item_list& items, const position_type& c, coordinate_type r,
  const item_picking_filter& filter ) const
{
  region_type region;
  region.push_front( rectangle_type(c.x - r, c.y - r, c.x + r, c.y + r) );

  item_list candidates;
  item_list::const_iterator it;
  list_active_items(candidates, region, filter);

  for ( it=candidates.begin(); it!=candidates.end(); ++it )
    if ( (*it)->get_center_of_mass().distance(c) <= r )
      items.push_back(*it);
} // world::pick_items_in_circle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Pick an item in a given direction.
 * \param p The position where the search begins.
 * \param dir The direction in which the items are searched.
 * \param filter The conditions the selected items must satisfy.
 * \return The first item in the direction, or NULL.
 */
bear::universe::physical_item* bear::universe::world::pick_item_in_direction
( const position_type& p, const vector_type& dir,
  const item_picking_filter& filter ) const
{
  region_type region;
  region.push_front( rectangle_type(p, p + dir) );

  item_list candidates;
  item_list::const_iterator it;
  list_active_items(candidates, region, filter);

  typedef claw::math::line_2d<coordinate_type> line_type;

  const line_type line(p, dir);
  coordinate_type dist( std::numeric_limits<coordinate_type>::max() );
  physical_item* result(NULL);
  rectangle_type p_rect(p, p);

  for ( it=candidates.begin(); (dist>0) && (it!=candidates.end()); ++it )
    if ( (*it)->get_bounding_box().includes(p) )
      {
        dist=0;
        result = *it;
      }
    else
      {
        const rectangle_type item_box( (*it)->get_bounding_box() );

        line_type item_line(0, 0, 0, 0);

        // vertical edges
        if ( item_box.left() > p.x )
          item_line = line_type( item_box.bottom_left(), vector_type(0, 1) );
        else if ( item_box.right() < p.x )
          item_line = line_type( item_box.bottom_right(), vector_type(0, 1) );

        if ( !line.parallel(item_line) )
          {
            position_type inter = line.intersection(item_line);

            if ( (inter.y >= item_box.bottom())
                 && (inter.y <= item_box.top()) )
              {
                coordinate_type d( inter.distance(p) );
                if ( d < dist )
                  {
                    dist = d;
                    result = *it;
                  }
              }
          }

        item_line = line_type(0, 0, 0, 0);

        // horizontal edges
        if ( item_box.bottom() > p.y )
          item_line = line_type( item_box.bottom_left(), vector_type(1, 0) );
        else if ( item_box.top() < p.y )
          item_line = line_type( item_box.top_left(), vector_type(1, 0) );

        if ( !line.parallel(item_line) )
          {
            position_type inter = line.intersection(item_line);

            if ( (inter.x >= item_box.left())
                 && (inter.x <= item_box.right()) )
              {
                coordinate_type d( inter.distance(p) );
                if ( d < dist )
                  {
                    dist = d;
                    result = *it;
                  }
              }
          }
      }

  return result;
} // world::pick_item_in_direction()

/*----------------------------------------------------------------------------*/
/**
 * \brief List items and entities which are in the active region.
 * \param items (out) The interesting items.
 * \param regions A list of regions in which we take the items.
 * \param filter The conditions the selected items must satisfy.
 */
void bear::universe::world::list_active_items
( item_list& items, const region_type& regions,
  const item_picking_filter& filter ) const
{
  item_list static_items;

  list_static_items( regions, static_items );
  item_list::const_iterator it;

  for (it=static_items.begin(); it!=static_items.end(); ++it)
    if ( filter.satisfies_condition(**it) )
      items.push_back(*it);

  for ( it=m_entities.begin(); it!=m_entities.end(); ++it )
    if ( item_in_regions(**it, regions) && filter.satisfies_condition(**it) )
      items.push_back( *it );
} // world::list_active_items()

/*----------------------------------------------------------------------------*/
/**
 * \brief Detect and correct the collisions.
 * \param items (in/out) The items on which we detect the collisions.
 * \param potential_collision The items in the world that can be involved in
 *        collisions.
 */
void bear::universe::world::detect_collision_all
( item_list& items, const item_list& potential_collision )
{
  item_list pending;

  for (item_list::iterator it=items.begin(); it!=items.end(); ++it)
    if ( !(*it)->is_fixed() )
      add_to_collision_queue(pending, *it, potential_collision);

  while ( !pending.empty() )
    {
      physical_item* item(pick_next_collision(pending));
      item->get_world_progress_structure().unset_waiting_for_collision();
      detect_collision( item, pending, items, potential_collision );
    }
} // world::detect_collision_all()

/*----------------------------------------------------------------------------*/
/**
 * \brief Find the next item to process in the collision detection.
 * \param pending The list of items waiting for the collision detection.
 * \return The first item with the greatest mass and the largest collision area.
 */
bear::universe::physical_item*
bear::universe::world::pick_next_collision( item_list& pending ) const
{
  CLAW_PRECOND( !pending.empty() );

  item_list::iterator item = pending.begin();
  double m = (*item)->get_world_progress_structure().get_collision_mass();
  double a = (*item)->get_world_progress_structure().get_collision_area();

  for (item_list::iterator it=item; it!=pending.end(); ++it)
    {
      bool update(false);

      if ( (*it)->get_world_progress_structure().get_collision_mass() > m )
        update = true;
      else if
        ( (*it)->get_world_progress_structure().get_collision_mass() == m )
        update =
          (*it)->get_world_progress_structure().get_collision_area() > a;

      if (update)
        {
          item = it;
          m = (*item)->get_world_progress_structure().get_collision_mass();
          a = (*item)->get_world_progress_structure().get_collision_area();
        }
    }

  physical_item* result = *item;
  pending.erase(item);

  return result;
} // world::pick_next_collision()

/*----------------------------------------------------------------------------*/
/**
 * \brief Detect and correct the collisions of an item.
 * \param item The item for which we search the collisions.
 * \param coll_detection (in/out) The instance of collision detection.
 * \param pending (out) A list in which are added the items in collision.
 * \param all_items (out) The set of all items processed in the iteration of the
 *        current world::progress() call.
 * \param potential_collision The items in the world that can be involved in
 *        collisions.
 */
void bear::universe::world::detect_collision
( physical_item* item, item_list& pending, item_list& all_items,
  const item_list& potential_collision ) const
{
  physical_item* it = item->get_world_progress_structure().pick_next_neighbor();

  if ( (it != NULL) && !it->is_artificial() )
    {
      CLAW_ASSERT( it != item, "ref item found in collision" );
      CLAW_ASSERT( !item->get_world_progress_structure().has_met(it),
                   "repeated collision" );

      const rectangle_type item_box( item->get_bounding_box() );
      const rectangle_type it_box( it->get_bounding_box() );

      if ( process_collision(*item, *it) )
        {
          select_item( all_items, it );
          item->get_world_progress_structure().meet(it);

          if ( it->get_bounding_box() != it_box )
            add_to_collision_queue(pending, it, potential_collision);
        }

      if ( item->get_bounding_box() == item_box )
        add_to_collision_queue_no_neighborhood(pending, item);
      else
        add_to_collision_queue(pending, item, potential_collision);
    }
} // world::detect_collision()

/*----------------------------------------------------------------------------*/
/**
 * \brief Process the collision of two items.
 * \param self The first item in the collision.
 * \param that The other item in the collision.
 */
bool bear::universe::world::process_collision
( physical_item& self, physical_item& that ) const
{
  bool result(false);

  if ( self.collides_with(that) )
    {
      result = true;
      collision_repair repair(self, that);

      collision_info info_ab
        ( self.get_world_progress_structure().get_initial_state(),
          that.get_world_progress_structure().get_initial_state(),
          self, that, repair );
      collision_info info_ba
        ( that.get_world_progress_structure().get_initial_state(),
          self.get_world_progress_structure().get_initial_state(),
          that, self, repair );

      self.collision(info_ab);
      that.collision(info_ba);

      repair.apply();

      self.adjust_cinetic();
      that.adjust_cinetic();
    }

  return result;
} // world::process_collision()

/*----------------------------------------------------------------------------*/
/**
 * \brief Search all items interesting for a collision with an other item.
 * \param item The item for which we search the collisions.
 * \param potential_collision The items in the world that can be involved
 *        in collisions.
 * \param colliding (out) The colliding items.
 * \param mass (in/out) The largest mass of the items found in the collision.
 * \param area (in/out) The largest area of the collision with the items of mass
 *        \a mass.
 */
void bear::universe::world::search_items_for_collision
( const physical_item& item, const item_list& potential_collision,
  item_list& colliding, double& mass, double& area ) const
{
  const rectangle_type r(item.get_bounding_box());

  // add static items
  item_list static_items;
  item_list::const_iterator its;
  m_static_surfaces.get_area_unique( r, static_items );

  for( its=static_items.begin(); its!=static_items.end(); ++its)
    if ( !(*its)->is_artificial()
         && !item.get_world_progress_structure().has_met(*its) )
      item_found_in_collision( item, *its, colliding, mass, area );

  item_list::const_iterator it;

  // add living item
  for ( it=potential_collision.begin(); it!=potential_collision.end(); ++it )
    if ( (*it!=&item) && (*it)->get_bounding_box().intersects(r)
         && !item.get_world_progress_structure().has_met(*it)
         && !(*it)->is_artificial() )
      item_found_in_collision( item, *it, colliding, mass, area );
} // world::search_items_for_collision()

/*----------------------------------------------------------------------------*/
/**
 * \brief An item has been found in collision with an other. Update de largest
 *        mass and the largest area, and add the item in the list.
 * \param item The item for which we search the collisions.
 * \param it The item found in collision.
 * \param colliding (out) The list in which we add \a it.
 * \param mass (in/out) The largest mass of the items found in the collision.
 * \param area (in/out) The largest area of the collision with the items of mass
 *        \a mass.
 */
void bear::universe::world::item_found_in_collision
( const physical_item& item, physical_item* it, item_list& colliding,
  double& mass, double& area ) const
{
  const double a =
    it->get_bounding_box().intersection(item.get_bounding_box()).area();

  if ( a != 0 )
    {
      it->get_world_progress_structure().init();
      colliding.push_front(it);

      if ( !(item.is_phantom() || item.is_fixed() ) && it->can_move_items() )
        {
          if ( it->get_mass() > mass )
            {
              mass = it->get_mass();
              area = a;
            }
          else if ( it->get_mass() == mass )
            if ( a > area )
              area = a;
        }
    }
} // world::item_found_in_collision()

/*----------------------------------------------------------------------------*/
/**
 * \brief Search all items in the pending items interesting for a collision
 *        with an other item.
 * \param item The item for which we search the collisions.
 * \param pending The pending items.
 * \param colliding (out) The colliding items.
 */
void bear::universe::world::search_pending_items_for_collision
( const physical_item& item, item_list& pending,
  std::list<item_list::iterator>& colliding ) const
{
  const rectangle_type r(item.get_bounding_box());

  for( item_list::iterator it=pending.begin(); it!=pending.end(); ++it )
    {
      CLAW_ASSERT
        ( !(*it)->is_artificial(), "artificial item in pending items." );

      if ( (*it)->get_bounding_box().intersects(r) )
        colliding.push_front(it);
    }
} // world::search_pending_items_for_collision()

/*----------------------------------------------------------------------------*/
/**
 * \brief Search all interesting items in the active region
 *  and add dependent items.
 * \param regions The active regions.
 * \param items (out) The items in the region.
 * \param potential_collision (out) The items in the world that can be involved
 *        in collisions.
 */
void bear::universe::world::search_interesting_items
( const region_type& regions, item_list& items,
  item_list& potential_collision ) const
{
  item_list::const_iterator it;

  // add static items of the active zone
  item_list static_items;
  m_static_surfaces.get_areas( regions.begin(), regions.end(), static_items );

  for( it=static_items.begin(); it!=static_items.end(); ++it)
    select_item(items, *it);

  // add global static items
  for (it=m_global_static_items.begin(); it!=m_global_static_items.end(); ++it)
    select_item(items, *it);

  // add living item of the active zone and global living item
  for ( it=m_entities.begin(); it!=m_entities.end(); ++it )
    {
      if ( (*it)->is_global() || item_in_regions(**it, regions) )
        select_item(items, *it);

      if ( !(*it)->is_artificial() )
        potential_collision.push_back(*it);
    }

  // add dependent item
  stabilize_dependent_items(items);
} // world::search_interesting_items()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get all items depending of an initial set of items.
 * \param items (in) The items for which we want the dependents, (out) the
 *        initial items plus the dependent ones.
 */
void bear::universe::world::stabilize_dependent_items( item_list& items ) const
{
  typedef claw::graph<physical_item*> graph_type;

  graph_type g;
  item_list pending;
  std::swap(items, pending);

  while ( !pending.empty() )
    {
      physical_item* const src( pending.front() );
      pending.pop_front();
      g.add_vertex(src);

      // get the item relatively to which I move
      physical_item* const ref
        ( const_cast<physical_item*>( src->get_movement_reference() ) );

      if ( ref != NULL )
        {
          select_item( pending, ref );
          g.add_edge(ref, src);
        }

      // get the items depending on me
      item_list dep_items;
      src->get_dependent_items(dep_items);

      // check if there is any new item in dep_items
      for ( ; !dep_items.empty(); dep_items.pop_front() )
        {
          physical_item* dep( dep_items.front() );

          if ( dep == NULL )
            claw::logger << claw::log_warning << "Dependent item is NULL"
                         << std::endl;
          else
            {
              select_item( pending, dep );
              g.add_edge(src, dep);
            }
        }
    }

  claw::topological_sort<graph_type> sort;
  sort(g);
  items = item_list( sort.begin(), sort.end() );
} // world::stabilize_dependent_items()

/*----------------------------------------------------------------------------*/
/**
 * \brief Call the time_step() method on some items.
 * \param items The items on which we call the method.
 * \param elapsed_time Elapsed time since the last call.
 */
void bear::universe::world::progress_items
( const item_list& items, time_type elapsed_time ) const
{
  item_list::const_iterator it;

  for( it=items.begin(); it!=items.end(); ++it )
    (*it)->time_step( elapsed_time );
} // world::progress_items()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update position of some items.
 * \param elapsed_time Elasped time since the last progress.
 * \param items The items to move.
 */
void bear::universe::world::progress_physic
( time_type elapsed_time, const item_list& items ) const
{
  item_list::const_iterator it;

  apply_links(items);

  for(it=items.begin(); it!=items.end(); ++it)
    progress_physic_move_item(elapsed_time, **it);
} // world::progress_physic()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update position of an items.
 * \param elapsed_time Elasped time since the last progress.
 * \param item The item to move.
 */
void bear::universe::world::progress_physic_move_item
( time_type elapsed_time, physical_item& item ) const
{
  if ( item.is_fixed() )
    item.get_world_progress_structure().set_move_done();
  else
    {
      physical_item* ref =
        const_cast<physical_item*>( item.get_movement_reference() );
      bool move_it(true);

      if ( ref != NULL )
        if ( !ref->get_world_progress_structure().move_is_done()
             && (ref != &item) )
          {
            claw::logger << claw::log_warning
                         << "The reference item for the movement has not "
                         << "been moved yet, so the current item will not be "
                         << "moved. The reference item is ";

            if ( !ref->get_world_progress_structure().is_selected() )
              claw::logger << "not ";

            claw::logger << "selected for movement." << std::endl;

            move_it = false;
          }

      if ( move_it )
        {
          item.move( elapsed_time );
          item.get_world_progress_structure().set_move_done();
        }
    }

  item.clear_contacts();
} // world::progress_physic_move_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply the forces of the links between items.
 * \param items The set of items for which we want the links.
 */
void bear::universe::world::apply_links(const item_list& items) const
{
  claw::avl<base_link*> links;
  claw::avl<base_link*>::const_iterator it_link;
  item_list::const_iterator it;

  for (it=items.begin(); it!=items.end(); ++it)
    links.insert( (*it)->links_begin(), (*it)->links_end() );

  for( it_link=links.begin(); it_link!=links.end(); ++it_link )
    (*it_link)->adjust();
} // world::apply_links()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell items if they leave the active region.
 * \param items The items to verify.
 */
void bear::universe::world::active_region_traffic( const item_list& items )
{
  item_list::const_iterator it;

  for( it=m_last_interesting_items.begin(); it!=m_last_interesting_items.end();
       ++it )
    if ( !(*it)->get_world_progress_structure().is_selected() )
      (*it)->leaves_active_region();

  for( it=items.begin(); it!=items.end(); ++it )
    if ( !(*it)->get_world_progress_structure().was_selected() )
      (*it)->enters_active_region();

  m_last_interesting_items = items;
} // world::active_region_traffic()

/*----------------------------------------------------------------------------*/
/**
 * \brief List static items which are in the active region.
 * \param items (out) The interesting items.
 * \param regions A list of regions in which we take the items.
 */
void bear::universe::world::list_static_items
( const region_type& regions, item_list& items ) const
{
  m_static_surfaces.get_areas_unique( regions.begin(), regions.end(), items );
} // world::list_static_items()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if an item is in a region.
 * \param item The item to check.
 * \param regions A list of regions in which search the item.
 */
bool bear::universe::world::item_in_regions
( const physical_item& item, const region_type& regions ) const
{
  return regions.intersects( item.get_bounding_box() );
} // world::item_in_regions()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add an entity in the world.
 * \param who The entity to add.
 */
void bear::universe::world::add(physical_item* const& who)
{
  who->set_owner(*this);
  m_entities.push_front( who );
} // world::add()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove an entity from the world.
 * \param who The entity to remove.
 */
void bear::universe::world::remove(physical_item* const& who)
{
  item_list::iterator eit(m_entities.end());
  item_list::iterator it = std::find( m_entities.begin(), eit, who );

  if ( it != eit )
    {
      m_entities.erase(it);
      who->quit_owner();
    }
  else
    claw::logger << claw::log_warning << "Can't remove unknown item."
                 << std::endl;

  eit = m_last_interesting_items.end();
  it = std::find( m_last_interesting_items.begin(), eit, who );

  if ( it != eit )
    m_last_interesting_items.erase(it);
} // world::remove()

/*----------------------------------------------------------------------------*/
/**
 * \brief Select an item if not already selected.
 * \param items (out) List of items to which is added the item.
 * \param it The item to select.
 * \return true if the item is selected.
 */
bool
bear::universe::world::select_item( item_list& items, physical_item* it ) const
{
  bool result = false;

  if ( !it->get_world_progress_structure().is_selected() )
    {
      items.push_front(it);
      it->get_world_progress_structure().init();
      it->get_world_progress_structure().select();
      result = true;
    }

  return result;
} // world::select_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Unselect an item.
 * \param items (out) List of items to which is removed the item.
 * \param it An iterator on the item to unselect.
 */
void bear::universe::world::unselect_item
( item_list& items, item_list::iterator it ) const
{
  CLAW_PRECOND( (*it)->get_world_progress_structure().is_selected() );

  (*it)->get_world_progress_structure().deinit();
  items.erase(it);
} // world::unselect_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add an item in the queue for collision detection.
 * \param pending (out) List of items to which is added the item.
 * \param item The item to add.
 * \param potential_collision The items in the world that can be involved in
 *        collisions.
 */
void bear::universe::world::add_to_collision_queue
( item_list& pending, physical_item* item,
  const item_list& potential_collision ) const
{
  if ( !item->has_weak_collisions() && !item->is_artificial() )
    if ( create_neighborhood(*item, potential_collision) )
      if ( !item->get_world_progress_structure().is_waiting_for_collision() )
        {
          item->get_world_progress_structure().set_waiting_for_collision();
          pending.push_back(item);
        }
} // world::add_to_collision_queue()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add an item in the queue for collision detection, without computing
 *        the neighborhood.
 * \param pending (out) List of items to which is added the item.
 * \param item The item to add.
 */
void bear::universe::world::add_to_collision_queue_no_neighborhood
( item_list& pending, physical_item* item ) const
{
  if ( !item->has_weak_collisions() && !item->is_artificial() )
    if ( item->get_world_progress_structure().update_collision_penetration() )
      if ( !item->get_world_progress_structure().is_waiting_for_collision() )
        {
          item->get_world_progress_structure().set_waiting_for_collision();
          pending.push_back(item);
        }
} // world::add_to_collision_queue_no_neighborhood()

/*----------------------------------------------------------------------------*/
/**
 * \brief Find the neighborhood of an item.
 * \param item The item for which we want the neighborhood.
 * \param potential_collision The items in the world that can be involved in
 *        collisions.
 */
bool bear::universe::world::create_neighborhood
( physical_item& item, const item_list& potential_collision ) const
{
  item_list n;
  double area(0);
  double mass(0);

  search_items_for_collision( item, potential_collision, n, mass, area );

  bool result(!n.empty());
  item.get_world_progress_structure().set_collision_neighborhood(n, mass, area);

  return result;
} // world::create_neighborhood()
