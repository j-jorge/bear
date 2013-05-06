/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::decoration_layer class.
 * \author Julien Jorge
 */
#include "generic_items/layer/decoration_layer.hpp"

#include "engine/layer/export.hpp"

#include <claw/logger.hpp>
#include <set>

LAYER_EXPORT( decoration_layer, bear )

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param world_size The real size of the world.
 */
bear::decoration_layer::decoration_layer
( const universe::size_box_type& size )
  : layer( size ),
    m_items( (unsigned int)m_size.x + 1, (unsigned int)m_size.y + 1, 256 )
{

} // decoration_layer::decoration_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::decoration_layer::~decoration_layer()
{
  std::list<engine::base_item*> items;
  std::list<engine::base_item*>::const_iterator it;

  m_items.get_all_unique(items);

  for (it=items.begin(); it!=items.end(); ++it)
    delete *it;

  std::list<engine::base_item*>::const_iterator it_g;

  for(it_g=m_global_items.begin(); it_g!=m_global_items.end(); ++it_g)
    delete *it_g;
} // decoration_layer::~decoration_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Start the items in the layer.
 */
void bear::decoration_layer::start()
{
  log_statistics();
} // decoration_layer::start()

/*----------------------------------------------------------------------------*/
/**
 * \brief Progress the items in the active area.
 * \param active_area The area containing active items.
 * \param elapsed_time Elapsed time since the last call.
 */
void bear::decoration_layer::progress
( const region_type& active_area, universe::time_type elapsed_time  )
{
  std::list<engine::base_item*> items;

  m_items.get_areas_unique( active_area.begin(), active_area.end(), items );

  std::list<engine::base_item*>::const_iterator it;

  for (it=items.begin(); it!=items.end(); ++it)
    (*it)->progress( elapsed_time );

  for(it=m_global_items.begin(); it!=m_global_items.end(); ++it)
    (*it)->progress(elapsed_time);
} // decoration_layer::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Log layer's inside maps load.
 */
void bear::decoration_layer::log_statistics() const
{
  unsigned int empty_cells, min, max;
  double avg;

  empty_cells = m_items.empty_cells();
  m_items.cells_load(min, max, avg);

  claw::logger << claw::log_verbose << "layer[" << m_size.x << ":" << m_size.y
               << "]:items:empty=" << empty_cells << " min=" << min
               << " max=" << max << " avg=" << avg << std::endl;
} // decoration_layer::log_statistics()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the visible sprites of this layer.
 * \param visuals (out) The sprites in the visible area, and their positions.
 * \param visible_area The visible part of the layer.
 */
void bear::decoration_layer::do_get_visual
( std::list<engine::scene_visual>& visuals,
  const universe::rectangle_type& visible_area ) const
{
  std::list<engine::base_item*> items;

  m_items.get_area_unique( visible_area, items );

  std::list<engine::base_item*>::const_iterator it;

  for (it=items.begin(); it!=items.end(); ++it)
    visuals.push_back( (*it)->get_visual() );

  for(it=m_global_items.begin(); it!=m_global_items.end(); ++it)
    if ( visible_area.intersects( (*it)->get_bounding_box() ) )
      {
        const universe::rectangle_type r
          ( visible_area.intersection( (*it)->get_bounding_box() ) );

        if( (r.width() > 0) && (r.height() > 0) )
          visuals.push_back( (*it)->get_visual() );
      }
} // decoration_layer::do_get_visual()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add an item in the layer.
 * \param item The item to add. It will be deleted with the layer.
 */
void bear::decoration_layer::do_add_item( engine::base_item& item )
{
  if ( item.is_global() )
    m_global_items.push_front(&item);
  else
    m_items.insert( &item );
} // decoration_layer::do_add_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove an item from the layer.
 * \param that The item to remove.
 *
 * This method should never be called.
 */
void bear::decoration_layer::do_remove_item( engine::base_item& that )
{
  CLAW_PRECOND( false );
} // decoration_layer::do_remove_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove an item from the layer without deleting it.
 * \param that The item to remove.
 */
void bear::decoration_layer::do_drop_item( engine::base_item& that )
{
  CLAW_PRECOND( false );
} // decoration_layer::do_drop_item()
