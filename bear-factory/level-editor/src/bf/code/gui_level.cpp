/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::gui_level class.
 * \author Julien Jorge
 */
#include "bf/gui_level.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param name The name of the level.
 * \param width The width of the level.
 * \param height The height of the level.
 * \param mus The default music in the level.
 */
bf::gui_level::gui_level
( const std::string& name, unsigned int width, unsigned int height,
  const std::string& mus )
  : level(name, width, height, mus), m_active_layer(0)
{

} // gui_level::gui_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the selection of a layer is not empty.
 * \param layer_index The layer to check.
 */
bool bf::gui_level::has_selection( unsigned int layer_index ) const
{
  CLAW_PRECOND( layer_index < layers_count() );
  CLAW_PRECOND( layers_count() == m_selection_by_layer.size() );

  return !m_selection_by_layer[layer_index].empty();
} // gui_level::has_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the selection of the layer on which we are working is not
 *        empty.
 */
bool bf::gui_level::has_selection( ) const
{
  if ( empty() )
    return false;
  else
    return has_selection( m_active_layer );
} // gui_level::has_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the selection in a layer.
 * \param layer_index The index of the layer.
 */
const bf::item_selection&
bf::gui_level::get_selection( unsigned int layer_index ) const
{
  CLAW_PRECOND( layer_index < layers_count() );
  CLAW_PRECOND( layers_count() == m_selection_by_layer.size() );

  return m_selection_by_layer[layer_index];
} // gui_level::get_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the selection in the layer on which we are working.
 */
const bf::item_selection& bf::gui_level::get_selection() const
{
  CLAW_PRECOND( m_active_layer < layers_count() );

  return get_selection( m_active_layer );
} // gui_level::get_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the main selection in a layer.
 * \param layer_index The index of the layer.
 */
bf::item_instance*
bf::gui_level::get_main_selection( unsigned int layer_index ) const
{
  CLAW_PRECOND( has_selection( layer_index ) );

  return get_selection( layer_index ).get_main_selection();
} // gui_level::get_main_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the main selection in the layer on which we are working.
 */
bf::item_instance* bf::gui_level::get_main_selection() const
{
  CLAW_PRECOND( has_selection() );

  return get_selection().get_main_selection();
} // gui_level::get_main_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the selection of a layer.
 * \param layer_index The index of the layer.
 * \param s The selection in the layer.
 * \pre All items in \a s are in the layer number \a layer_index.
 */
void bf::gui_level::set_selection
( unsigned int layer_index, const item_selection& s )
{
  CLAW_PRECOND( layer_index < layers_count() );
  CLAW_PRECOND( layers_count() == m_selection_by_layer.size() );

  m_selection_by_layer[layer_index] = s;
} // gui_level::set_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if an item is selected.
 * \param layer_index The layer to search the item in.
 * \param item The item to search.
 */
bool bf::gui_level::item_is_selected
( unsigned int layer_index, item_instance const* item ) const
{
  CLAW_PRECOND( layer_index < layers_count() );
  CLAW_PRECOND( layers_count() == m_selection_by_layer.size() );

  return m_selection_by_layer[layer_index].is_selected(item);
} // gui_level::item_is_selected()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if an item is selected in the layer in which we are working.
 * \param item The item to search.
 */
bool bf::gui_level::item_is_selected( item_instance const* item ) const
{
  CLAW_PRECOND( m_active_layer < layers_count() );

  return item_is_selected( m_active_layer, item );
} // gui_level::item_is_selected()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if an item is the main selection.
 * \param layer_index The layer to search the item in.
 * \param item The item to search.
 */
bool bf::gui_level::item_is_main_selection
( unsigned int layer_index, item_instance const* item ) const
{
  CLAW_PRECOND( layer_index < layers_count() );
  CLAW_PRECOND( layers_count() == m_selection_by_layer.size() );

  return m_selection_by_layer[layer_index].is_main_selection(item);
} // gui_level::item_is_main_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if an item is the main selection.
 * \param item The item to search.
 */
bool bf::gui_level::item_is_main_selection( item_instance const* item ) const
{
  CLAW_PRECOND( m_active_layer < layers_count() );

  return item_is_main_selection(m_active_layer, item);
} // gui_level::item_is_main_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a selection in the selection of a layer.
 * \param layer_index The layer in which we select the items.
 * \param s The items to add.
 */
void bf::gui_level::add_to_selection
( unsigned int layer_index, const item_selection& s )
{
  CLAW_PRECOND( layer_index < layers_count() );
  CLAW_PRECOND( layers_count() == m_selection_by_layer.size() );

  m_selection_by_layer[layer_index].insert(s);
} // gui_level::add_to_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add an item in the selection of a layer.
 * \param layer_index The layer in which we select the item.
 * \param item The item to select.
 * \param main_selection Set the item as the main selection.
 */
void bf::gui_level::add_to_selection
( unsigned int layer_index, item_instance* item, bool main_selection )
{
  CLAW_PRECOND( layer_index < layers_count() );

  m_selection_by_layer[layer_index].insert(item, main_selection);
} // gui_level::add_to_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add an item in the selection of the layer on which we are working.
 * \param item The item to select.
 * \param main_selection Set the item as the main selection.
 */
void bf::gui_level::add_to_selection( item_instance* item, bool main_selection )
{
  CLAW_PRECOND( m_active_layer < layers_count() );

  add_to_selection( m_active_layer, item, main_selection );
} // gui_level::add_to_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove an item from the selection of a layer.
 * \param layer_index The layer in which we deselect the item.
 * \param item The item to deselect.
 */
void bf::gui_level::remove_from_selection
( unsigned int layer_index, item_instance* item )
{
  CLAW_PRECOND( layer_index < layers_count() );
  CLAW_PRECOND( layers_count() == m_selection_by_layer.size() );

  m_selection_by_layer[layer_index].remove(item);
} // gui_level::remove_from_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove a selection from the selection of a layer.
 * \param layer_index The layer in which we deselect the items.
 * \param s The items to remove.
 */
void bf::gui_level::remove_from_selection
( unsigned int layer_index, const item_selection& s )
{
  CLAW_PRECOND( layer_index < layers_count() );
  CLAW_PRECOND( layers_count() == m_selection_by_layer.size() );

  m_selection_by_layer[layer_index].remove(s);
} // gui_level::remove_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove an item from the selection of the layer on which we are
 *        working.
 * \param item The item to deselect.
 */
void bf::gui_level::remove_from_selection( item_instance* item )
{
  CLAW_PRECOND( m_active_layer < layers_count() );

  remove_from_selection( m_active_layer, item );
} // gui_level::remove_from_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Clear the selection of a layer.
 * \param layer_index The layer in which we clear the selection.
 */
void bf::gui_level::clear_selection( unsigned int layer_index )
{
  CLAW_PRECOND( layer_index < layers_count() );
  CLAW_PRECOND( layers_count() == m_selection_by_layer.size() );

  m_selection_by_layer[layer_index].clear();
} // gui_level::clear_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Clear the selection in the layer in which we are working.
 */
void bf::gui_level::clear_selection()
{
  if ( m_active_layer < layers_count() )
    clear_selection( m_active_layer );
} // gui_level::clear_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove an item from the level.
 * \param layer_index The layer in which we remove the item.
 * \param item The item to remove.
 */
void bf::gui_level::remove_item( unsigned int layer_index, item_instance* item )
{
  CLAW_PRECOND( layer_index < layers_count() );

  remove_from_selection(layer_index, item);
  get_layer(layer_index).remove_item(item);
} // gui_level::remove_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a layer is visible.
 * \param layer_index The index of the layer.
 */
bool bf::gui_level::layer_is_visible( unsigned int layer_index ) const
{
  CLAW_PRECOND( layer_index < layers_count() );
  CLAW_PRECOND( layers_count() == m_layer_visibility.size() );

  return m_layer_visibility[layer_index];
} // gui_level::layer_is_visible()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the visibility of a layer.
 * \param layer_index The index of the layer.
 * \param b Visible or not?
 */
void bf::gui_level::set_layer_visibility( unsigned int layer_index, bool b )
{
  CLAW_PRECOND( layer_index < layers_count() );
  CLAW_PRECOND( layers_count() == m_layer_visibility.size() );

  m_layer_visibility[layer_index] = b;
} // gui_level::layer_is_visible()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the index of the layer to work on.
 * \param layer_index The index of the layer.
 */
void bf::gui_level::set_active_layer( unsigned int layer_index )
{
  CLAW_PRECOND( layer_index < layers_count() );

  m_active_layer = layer_index;
} // gui_level::set_active_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the layer on which we are working.
 */
bf::layer& bf::gui_level::get_active_layer() const
{
  CLAW_PRECOND( m_active_layer < layers_count() );

  return get_layer(m_active_layer);
} // gui_level::get_active_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the index of the layer on which we are working.
 */
unsigned int bf::gui_level::get_active_layer_index() const
{
  return m_active_layer;
} // gui_level::get_active_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a layer in the level, at the top of the layer stack.
 * \param class_name The name of the class of the layer.
 * \param name The name of the layer.
 * \param fit_level Tell if the layer size is the same than the level.
 * \param width The width of the layer.
 * \param height The height of the layer.
 */
bf::layer& bf::gui_level::add_layer
( const std::string& class_name, const std::string& name,
  bool fit_level, unsigned int width, unsigned int height )
{
  layer& result =
    level::add_layer( class_name, name, fit_level, width, height );

  m_active_layer = layers_count()-1;

  m_layer_visibility.push_back(true);
  m_selection_by_layer.push_back( item_selection() );

  return result;
} // gui_level::add_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a layer in the level.
 * \param lay The layer to add.
 * \param layer_index The position where the layer is added in the layer stack.
 */
void bf::gui_level::add_layer( layer* lay, unsigned int layer_index )
{
  CLAW_PRECOND( layer_index <= layers_count() );

  level::add_layer( lay, layer_index );

  m_active_layer = layer_index;

  m_layer_visibility.insert( m_layer_visibility.begin() + layer_index, true );
  m_selection_by_layer.insert
    ( m_selection_by_layer.begin() + layer_index, item_selection() );
} // gui_level::add_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove a layer from the level.
 * \param layer_index The position in the layer stack of the layer to remove.
 */
bf::layer* bf::gui_level::remove_layer( unsigned int layer_index )
{
  CLAW_PRECOND( layer_index < layers_count() );

  layer* result = level::remove_layer( layer_index );

  m_layer_visibility.erase( m_layer_visibility.begin() + layer_index );
  m_selection_by_layer.erase( m_selection_by_layer.begin() + layer_index );

  if ( ( m_active_layer == layers_count() ) && ( m_active_layer != 0 ) )
    --m_active_layer;

  return result;
} // gui_level::remove_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Move a layer toward the bottom of the layer stack.
 * \param layer_index The position in the layer stack of the layer to move.
 */
void bf::gui_level::move_backward( unsigned int layer_index )
{
  CLAW_PRECOND( layer_index < layers_count() );

  if ( layer_index > 0 )
    {
      level::move_backward( layer_index );

      const bool tmp(m_layer_visibility[layer_index]);
      m_layer_visibility[layer_index] = m_layer_visibility[layer_index-1];
      m_layer_visibility[layer_index-1] = tmp;

      std::swap
        ( m_selection_by_layer[layer_index],
          m_selection_by_layer[layer_index-1] );

      if ( m_active_layer == layer_index )
        --m_active_layer;
    }
} // gui_level::move_backward()

/*----------------------------------------------------------------------------*/
/**
 * \brief Move a layer toward the top of the layer stack.
 * \param layer_index The position in the layer stack of the layer to move.
 */
void bf::gui_level::move_forward( unsigned int layer_index )
{
  CLAW_PRECOND( layer_index < layers_count() );

  if ( layer_index + 1 < layers_count() )
    {
      level::move_forward( layer_index );

      const bool tmp(m_layer_visibility[layer_index]);
      m_layer_visibility[layer_index] = m_layer_visibility[layer_index+1];
      m_layer_visibility[layer_index+1] = tmp;

      std::swap
        ( m_selection_by_layer[layer_index],
          m_selection_by_layer[layer_index+1] );

      if ( m_active_layer == layer_index )
        ++m_active_layer;
    }
} // gui_level::move_forward()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check if items are in the level.
 */
bool bf::gui_level::check_item_position() const
{
  bool result = true;

  for ( unsigned int i=0; i != layers_count() && result; ++i)
    result = get_layer(i).check_item_position();

  return result;  
} // gui_level::check_item_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Pick the first item found near a given point.
 * \param pos The position of the point.
 */
bf::item_instance*
bf::gui_level::first_item( const position_type& pos ) const
{
  item_instance* result( NULL );
  layer::item_iterator it;
  coordinate_type min_max_dist( std::numeric_limits<coordinate_type>::max() );

  if ( !empty() )
    {
      layer& the_layer( get_active_layer() );

      for ( it = the_layer.item_begin(); it != the_layer.item_end(); ++it )
        {
          const rectangle_type box( get_visual_box( *it ) );

          if ( box.includes(pos) )
            {
              const coordinate_type dist =
                std::max
                ( std::max( pos.x - box.left(), box.right() - pos.x),
                  std::max( pos.y - box.top(), box.bottom() - pos.y) );

              if ( dist < min_max_dist )
                {
                  result = &(*it);
                  min_max_dist = dist;
                }
            }
        }
    }

  return result;
} // gui_level::pick_first_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the first item in the active selection that overlaps a given
 *        position.
 * \param pos The point to test.
 */
bf::item_instance* bf::gui_level::first_selected_item( position_type pos ) const
{
  if ( empty() )
    return NULL;

  item_selection::const_iterator it;
  const item_selection& selection( get_selection() );

  for ( it=selection.begin(); it!=selection.end(); ++it )
    {
      const rectangle_type box( get_visual_box( **it ) ); 

      if ( box.includes( pos ) )
        return *it;
    }

  return NULL;
} // gui_level::first_selected_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Finds all the items of the active layer that overlaps a given point.
 * \param pos The point.
 */
std::vector<bf::item_instance*>
bf::gui_level::find_items_at( position_type pos ) const
{
  std::vector<item_instance*> result;

  if ( !empty() )
    {
      const layer& the_layer( get_active_layer() );

      for ( layer::item_iterator it = the_layer.item_begin();
            it != the_layer.item_end(); ++it )
        {
          const rectangle_type box( get_visual_box( *it ) );

          if ( box.includes( pos ) )
            result.push_back( &(*it) );
        }
    }

  return result;
} // gui_level::find_items_at()

/*----------------------------------------------------------------------------*/
/**
 * \brief Finds all the items intersecting a given box.
 * \param box The box where to pick the items.
 */
std::vector<bf::item_instance*>
bf::gui_level::pick_items( rectangle_type box ) const
{
  std::vector<item_instance*> result;

  if ( !empty() )
    {
      const layer& the_layer( get_active_layer() );

      for ( layer::item_iterator it = the_layer.item_begin();
            it != the_layer.item_end(); ++it)
        if ( box.intersects( get_visual_box( *it ) ) )
          result.push_back( &(*it) );
    }

  return result;
} // gui_level::pick_items()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the visual box of an item.
 * \param item The item to bound.
 */
bf::rectangle_type
bf::gui_level::get_visual_box( const item_instance& item ) const
{
  double x( item.get_rendering_parameters().get_left() );
  double y( item.get_rendering_parameters().get_bottom() );
  double w( item.get_rendering_parameters().get_width() );
  double h( item.get_rendering_parameters().get_height() );

  if ( w == 0 )
    {
      x -= 10;
      w = 20;
    }

  if ( h == 0 )
    {
      y -= 10;
      h = 20;
    }

  return rectangle_type( x, y, x + w, y + h );
} // gui_level::get_visual_box()
