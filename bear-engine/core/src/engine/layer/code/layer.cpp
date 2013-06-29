/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::layer class.
 * \author Julien Jorge
 */
#include "engine/layer/layer.hpp"

#include <claw/logger.hpp>
#include <claw/assert.hpp>

#include "visual/scene_shader_pop.hpp"
#include "visual/scene_shader_push.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param size The size of the layer.
 */
bear::engine::layer::layer( const universe::size_box_type& size )
  : m_size( size ), m_visible( true ), m_active( true ),
    m_currently_updating( false )
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
 * \brief Updates a region in the layer.
 * \param active_area The region to update.
 * \param elapsed_time The elapsed time since the last call.
 */
void bear::engine::layer::update
( const region_type& active_area, universe::time_type elapsed_time  )
{
  if ( !is_active() )
    return;

  m_currently_updating = true;

  progress( active_area, elapsed_time );

  m_currently_updating = false;
  apply_post_update_changes();
} // layer::update()

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
  if ( !is_visible() )
    return;

  std::set<base_item*>::const_iterator it;

  for ( it=m_always_displayed.begin(); it!=m_always_displayed.end(); ++it )
    if ( !visible_area.intersects( (*it)->get_bounding_box() ) )
      visuals.push_back( (*it)->get_visual() );

  do_get_visual(visuals, visible_area);
  visuals.sort( scene_visual::z_position_compare() );

  if ( m_shader.is_valid() )
    {
      visuals.push_front( visual::scene_shader_push( m_shader ) );
      visuals.push_back( visual::scene_shader_pop() );
    }
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
  CLAW_PRECOND( !item.is_in_layer() );

  claw::logger << claw::log_verbose << "Adding item #" << item.get_id()
               << " '" << item.get_class_name() << "' in layer." << std::endl;

  m_post_creation_action[ &item ] = add;

  item.set_environment(*this);

  if ( !item.is_built() )
    {
      item.pre_cache();
      item.build_item();
    }

  item.enters_layer();

  if ( item.get_insert_as_static() )
    {
      item.fix();
      item.set_phantom(true);
    }

  switch ( mark_as_built( item ) )
    {
    case add:
      do_add_item( item );
      break;
    case remove:
      remove_item( item );
      break;
    case drop:
      drop_item( item );
      break;
    }
} // layer::add_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove an item from the layer and delete it.
 * \param item The item to remove.
 */
void bear::engine::layer::remove_item( base_item& item )
{
  if ( m_currently_updating )
    m_post_update_removal.push_back( &item );
  else if ( is_currently_building( item ) )
    m_post_creation_action[ &item ] = remove;
  else
    {
      m_always_displayed.erase(&item);

      do_remove_item(item);

      item.clear_environment();
      item.leaves_layer();
    }
} // layer::remove_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove an item from the layer without deleting it.
 * \param item The item to remove.
 */
void bear::engine::layer::drop_item( base_item& item )
{
  CLAW_PRECOND( item.is_in_layer( *this ) );
  CLAW_PRECOND( std::find
                ( m_post_update_removal.begin(), m_post_update_removal.end(),
                  &item )
                == m_post_update_removal.end() );

  if ( is_currently_building( item ) )
    m_post_creation_action[ &item ] = remove;
  else
    {
      m_always_displayed.erase(&item);

      do_drop_item(item);

      item.clear_environment();
      item.leaves_layer();
    }
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
 * \brief Changes the visibility of the layer.
 * \param v The new visibility.
 */
void bear::engine::layer::set_visible( bool v )
{
  m_visible = v;
} // layer::set_visible()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if the layer is visible.
 */
bool bear::engine::layer::is_visible() const
{
  return m_visible;
} // layer::is_visible()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets if the layer is active, i.e. the update function has an effect.
 * \param v The new value of the active flag.
 */
void bear::engine::layer::set_active( bool v )
{
  m_active = v;
} // layer::set_active()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if the layer is active, i.e. the update function has an effect.
 */
bool bear::engine::layer::is_active() const
{
  return m_active;
} // layer::is_active()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the shader to apply to the items in this layer.
 * \param s The shader.
 */
void bear::engine::layer::set_shader( visual::shader_program s )
{
  m_shader = s;
} // layer::set_shader()

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

/*----------------------------------------------------------------------------*/
/**
 * \brief Marks an item as built and returns the action that must be done after
 *        the creation.
 * \param item The item.
 */
bear::engine::layer::post_create_action
bear::engine::layer::mark_as_built( base_item& item )
{
  std::map<base_item*, post_create_action>::iterator it
    ( m_post_creation_action.find( &item ) );

  const post_create_action result( it->second );

  m_post_creation_action.erase( it );

  return result;
} // layer::mark_as_built()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if an item is currently being inserted in the layer.
 * \param item The item.
 */
bool bear::engine::layer::is_currently_building( base_item& item ) const
{
  return m_post_creation_action.find( &item ) != m_post_creation_action.end();
} // layer::is_currently_building()

/*----------------------------------------------------------------------------*/
/**
 * \brief Applies the changes that have been postponed until the end of the
 *        update.
 */
void bear::engine::layer::apply_post_update_changes()
{
  CLAW_PRECOND( !m_currently_updating );

  for ( std::list<base_item*>::const_iterator it=m_post_update_removal.begin();
        it != m_post_update_removal.end(); ++it )
    remove_item( **it );

  m_post_update_removal.clear();
} // layer::apply_post_update_changes()
