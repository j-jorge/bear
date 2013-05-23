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
 * \brief Tells if the selection of a layer is not empty.
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
 * \brief Gets the selection in a layer.
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
 * \brief Gets the main selection in a layer.
 * \param layer_index The index of the layer.
 */
bf::item_instance*
bf::gui_level::get_main_selection( unsigned int layer_index ) const
{
  return get_selection( layer_index ).get_main_selection();
} // gui_level::get_main_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if an item is selected in its layer.
 * \param item The item to search.
 */
bool bf::gui_level::item_is_selected( item_instance const* item ) const
{
  const std::size_t layer_index( get_layer_by_item(*item) );

  return m_selection_by_layer[layer_index].is_selected( item );
} // gui_level::item_is_selected()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if an item is the main selection in its layer.
 * \param item The item to search.
 */
bool bf::gui_level::item_is_main_selection( item_instance const* item ) const
{
  return (item != NULL)
    && get_main_selection( get_layer_by_item(*item) ) == item;
} // gui_level::item_is_main_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adds a selection in the selection of the layers.
 * \param s The items to add.
 */
void bf::gui_level::add_to_selection
( const item_selection& s )
{
  for ( item_selection::const_iterator it( s.begin() ); it != s.end(); ++it )
    add_to_selection( *it );

  add_to_selection( s.get_main_selection(), true );
} // gui_level::add_to_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adds an item in the selection of its layer.
 * \param item The item to select.
 * \param main_selection Tells to set the item as the main selection.
 */
void bf::gui_level::add_to_selection
( item_instance* item, bool main_selection )
{
  if ( item == NULL )
    return;

  const std::size_t layer_index( get_layer_by_item( *item ) );
  
  m_selection_by_layer[layer_index].insert( item, main_selection );
} // gui_level::add_to_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Removes an item from the selection of its layer.
 * \param item The item to deselect.
 */
void bf::gui_level::remove_from_selection( item_instance* item )
{
  const std::size_t layer_index( get_layer_by_item( *item ) );

  m_selection_by_layer[layer_index].remove(item);
} // gui_level::remove_from_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Removes a selection from the selections of the layers.
 * \param s The items to remove.
 */
void bf::gui_level::remove_from_selection( const item_selection& s )
{
  for ( item_selection::const_iterator it( s.begin() ); it != s.end(); ++it )
    remove_from_selection( *it );
} // gui_level::remove_selection()

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
 * \brief Remove an item from the level.
 * \param layer_index The layer in which we remove the item.
 * \param item The item to remove.
 */
void bf::gui_level::remove_item( unsigned int layer_index, item_instance* item )
{
  CLAW_PRECOND( layer_index < layers_count() );

  remove_from_selection( item );
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
std::size_t bf::gui_level::get_active_layer_index() const
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
