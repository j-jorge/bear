/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::layer class.
 * \author Julien Jorge
 */
#include "engine/layer/layer.hpp"

#include <claw/logger.hpp>
#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param size The size of the layer.
 */
bear::engine::layer::layer( const universe::size_box_type& size )
  : m_size( size ), m_tag("common")
{
  CLAW_PRECOND( size.x != 0 );
  CLAW_PRECOND( size.y != 0 );
} // engine::layer::layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::engine::layer::~layer()
{
  // nothing to do
} // layer::~layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the content of the layer.
 */
void bear::engine::layer::start()
{
  // nothing to do
} // layer::start()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the size of the layer.
 */
const bear::universe::size_box_type& bear::engine::layer::get_size() const
{
  return m_size;
} // layer::get_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the tag of the layer.
 * \param tag The new tag of the layer.
 */
void bear::engine::layer::set_tag(const std::string& tag)
{
  m_tag = tag;
} // layer::set_tag()

/*----------------------------------------------------------------------------*/
/**
 * \brief Return the tag of the layer.
 */
std::string bear::engine::layer::get_tag() const
{
  return m_tag;
} // layer::get_tag()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the sprites of the items in the visible area.
 * \param visuals (out) The sprites in the visible area, and their positions.
 * \param visible_area The visible part of the layer.
 */
void bear::engine::layer::get_visual
( std::list<scene_visual>& visuals,
  const universe::rectangle_type& visible_area ) const
{
  std::set<base_item*>::const_iterator it;

  for ( it=m_always_displayed.begin(); it!=m_always_displayed.end(); ++it )
    if ( !visible_area.intersects( (*it)->get_bounding_box() ) )
      (*it)->insert_visual(visuals);

  do_get_visual(visuals, visible_area);
} // layer::get_visual()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add an item in the layer and call item.build_item().
 * \param item The item to add.
 * \pre item.is_valid() && !item.is_fixed()
 */
void bear::engine::layer::add_item( base_item& item )
{
  CLAW_PRECOND( item.is_valid() );
  CLAW_PRECOND( !item.is_fixed() );

  claw::logger << claw::log_verbose << "Adding item #" << item.get_id()
               << " '" << item.get_class_name() << "' in layer." << std::endl;

  item.set_environment(*this);
  item.pre_cache();
  item.build_item();
  item.enters_layer();

  if ( item.get_insert_as_static() )
    {
      item.fix();
      item.set_phantom(true);
    }

  do_add_item(item);
} // layer::add_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove an item from the layer and delete it.
 * \param item The item to remove.
 */
void bear::engine::layer::remove_item( base_item& item )
{
  m_always_displayed.erase(&item);

  do_remove_item(item);
} // layer::remove_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove an item from the layer without deleting it.
 * \param item The item to remove.
 */
void bear::engine::layer::drop_item( base_item& item )
{
  m_always_displayed.erase(&item);

  do_drop_item(item);
  item.clear_environment();
} // layer::drop_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell that an item is always displayed.
 * \param item The item to display.
 */
void bear::engine::layer::set_always_displayed( base_item& item )
{
  m_always_displayed.insert(&item);
} // layer::set_always_displayed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell that an item is not always displayed.
 * \param item The item not to display.
 */
void bear::engine::layer::unset_always_displayed( base_item& item )
{
  m_always_displayed.erase(&item);
} // layer::unset_always_displayed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the layer has a world.
 */
bool bear::engine::layer::has_world() const
{
  return do_get_world() != NULL;
} // layer::has_world()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the world.
 */
bear::engine::world& bear::engine::layer::get_world()
{
  CLAW_PRECOND( has_world() );
  return *do_get_world();
} // layer::get_world()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the world.
 */
const bear::engine::world& bear::engine::layer::get_world() const
{
  CLAW_PRECOND( has_world() );
  return *do_get_world();
} // layer::get_world()

/*----------------------------------------------------------------------------*/
/**
 * \brief Effectively add an item in the layer.
 * \param item The item to add.
 */
void bear::engine::layer::do_add_item( base_item& item )
{
  // nothing to do
} // layer::do_add_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Effectively remove an item from the layer.
 * \param item The item to remove.
 */
void bear::engine::layer::do_remove_item( base_item& item )
{
  // nothing to do
} // layer::do_remove_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Effectively drop an item from the layer.
 * \param item The item to remove.
 */
void bear::engine::layer::do_drop_item( base_item& item )
{
  // nothing to do
} // layer::do_drop_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the world.
 */
bear::engine::world* bear::engine::layer::do_get_world()
{
  return NULL;
} // layer::do_get_world()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the world.
 */
const bear::engine::world* bear::engine::layer::do_get_world() const
{
  return NULL;
} // layer::do_get_world()
