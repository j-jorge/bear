/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::world class.
 * \author Julien Jorge
 */
#include "engine/world.hpp"

#include "engine/base_item.hpp"
#include "engine/game.hpp"
#include "engine/population.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor
 * \param size Size of the world.
 */
bear::engine::world::world( const universe::size_box_type& size )
  : universe::world(size)
{

} // world::world()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor
 * \param size Size of the world.
 */
bear::engine::world::~world()
{
  for ( ; !m_static_items.empty(); m_static_items.pop_front() )
    delete m_static_items.front();
} // world::~world()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the world.
 */
void bear::engine::world::start()
{
  print_stats();
} // world::start()

/*----------------------------------------------------------------------------*/
/**
 * \brief Call the progress() method on entities in the active region, then
 *        apply physic rules.
 * \param regions The active regions.
 * \param elapsed_time Elapsed time since the last call of this method.
 */
void bear::engine::world::progress_entities
( const region_type& regions, universe::time_type elapsed_time )
{
  universe::world::progress_entities(regions, elapsed_time);
  m_population.remove_dead_items();
  //print_item_stats();
} // world::progress_entities()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the sprites of the visible items in the active region.
 * \param visuals (out) The sprites in the visible area, and their positions.
 * \param camera_box The part of the world visible through the camera.
 */
void bear::engine::world::get_visual
( std::list<scene_visual>& visuals,
  const universe::rectangle_type& camera_box ) const
{
  item_list items;
  item_list::const_iterator it;

  pick_items_in_rectangle(items, camera_box);

  for (it=items.begin(); it!=items.end(); ++it)
    {
      base_item* i( dynamic_cast<base_item*>(*it) );

      if ( i==NULL )
        claw::logger << claw::log_warning
                     << "bear::engine::world::get_visual(): "
                     << "item is not a base_item. Not rendered: "
                     << **it << std::endl;
      else
        visuals.push_back( i->get_visual() );
    }
} // world::get_visual()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the beginning of the living items.
 */
bear::engine::world::const_item_iterator
bear::engine::world::living_items_begin() const
{
  return m_population.begin();
} // world::living_items_begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the end of the living items.
 */
bear::engine::world::const_item_iterator
bear::engine::world::living_items_end() const
{
  return m_population.end();
} // world::living_items_end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a permanent and fixed item in the world.
 * \param who The item to add.
 */
void bear::engine::world::add_static( base_item* const& who )
{
  who->set_world(*this);
  universe::world::add_static( who );
  m_static_items.push_front( who );
} // world::add_static()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add an item in the world.
 * \param who The item to add.
 */
void bear::engine::world::register_item( base_item* const& who )
{
  /* !!Julien. January 2010, 6th.

     The base_item::kill() method may have been called before adding the item
     (in base_item::build(), for example). In such case, the item is not added
     to the world, but we still insert him in the population to kill him
     immediately. */

  m_population.insert( who );

  if ( !who->is_dead() )
    {
      who->set_world(*this);
      universe::world::register_item(who);
    }
  else
    m_population.kill( who );
} // world::register_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Kill and remove an item from the world.
 * \param who The item to kill.
 */
void bear::engine::world::release_item( base_item* const& who )
{
  m_population.kill(who);
  universe::world::release_item(who);
} // world::release_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove an item from the world without killing him.
 * \param who The item to remove.
 */
void bear::engine::world::drop_item( base_item* const& who )
{
  m_population.drop(who);
  universe::world::release_item(who);
} // world::drop_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Print some statistics about the living items in the console.
 */
void bear::engine::world::print_item_stats() const
{
  std::map<std::string, std::size_t> items;
  for ( const_item_iterator it=living_items_begin(); it!=living_items_end();
        ++it )
    {
      const std::string name(it->get_class_name());
      std::map<std::string, std::size_t>::iterator c = items.find(name);
      if ( c != items.end() )
        ++c->second;
      else
        items[name] = 1;
    }

  for ( std::map<std::string, std::size_t>::const_iterator it=items.begin();
        it!=items.end(); ++it )
    std::cout << get_world_time() << '\t' << it->first << '\t' << it->second
              << std::endl;

  std::cout << "----------------------------------------" << std::endl;
} // world::print_item_stats()
