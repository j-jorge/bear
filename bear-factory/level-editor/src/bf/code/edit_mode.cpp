/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::edit_mode class.
 * \author Julien Jorge
 */
#include "bf/edit_mode.hpp"

#include "bf/gui_level.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructs a new instance initialized to apply the operations to the
 *        active layer.
 */
bf::edit_mode::edit_mode()
  : m_current_mode( active_layer )
{

} // edit_mode::edit_mode()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells to which items the operations are applied.
 */
bf::edit_mode::value_type bf::edit_mode::get_value() const
{
  return m_current_mode;
} // edit_mode::get_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets to which items the operations are applied.
 * \param v The new edit mode.
 */
void bf::edit_mode::set_value( value_type v )
{
  m_current_mode = v;
} // edit_mode::set_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the items to which the operations must be done.
 * \param lvl The level from which the items are taken.
 */
bf::item_selection
bf::edit_mode::get_selection( const gui_level& lvl ) const
{
  return get_selections_by_layer_index
    ( lvl, get_edit_layers( lvl ), lvl.get_active_layer_index() );
} // edit_mode::get_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the selection of the active layer.
 * \param lvl The level from which the items are taken.
 */
std::vector<std::size_t>
bf::edit_mode::get_edit_layers( const gui_level& lvl ) const
{
  std::vector<std::size_t> result;

  if ( lvl.empty() )
    return result;

  switch( m_current_mode )
    {
    case active_layer:
      result.push_back( lvl.get_active_layer_index() );
      break;

    case layers_by_tag:
      result = get_layer_indices_by_tag( lvl );
      break;

    case all_layers:
      result = get_all_layers_indices( lvl );
    }

  return result;
} // edit_mode::get_edit_layers()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if an item is owned by a layer concerned by the current edit
 *        mode.
 * \param lvl The level from which the layers are taken.
 * \param item The item to check.
 */
bool bf::edit_mode::is_in_edited_layer
( const gui_level& lvl, item_instance* item ) const
{
  const std::vector<std::size_t> layers( get_edit_layers( lvl ) );

  return std::find
    ( layers.begin(), layers.end(), lvl.get_layer_by_item( *item ) )
    != layers.end();
} // edit_mode::is_in_edited_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Picks the first item found near a given point in the edited layers.
 * \param lvl The level from which we take the items.
 * \param pos The position of the point.
 */
bf::item_instance* bf::edit_mode::first_item
( const gui_level& lvl, const position_type& pos ) const
{
  item_instance* result( NULL );
  coordinate_type min_max_dist( std::numeric_limits<coordinate_type>::max() );

  const std::vector<std::size_t> layers( get_edit_layers( lvl ) );

  for ( std::size_t i( layers.size() ); i != 0; --i )
    {
      const layer& the_layer( lvl.get_layer( layers[ i - 1 ] ) );

      for ( layer::item_iterator it( the_layer.item_begin() );
            it != the_layer.item_end(); ++it )
        {
          const rectangle_type box( lvl.get_visual_box( *it ) );

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
} // edit_mode::pick_first_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the first item in the edited layers that overlaps a given
 *        position.
 * \param lvl The level from which we take the items.
 * \param pos The point to test.
 */
bf::item_instance* bf::edit_mode::first_selected_item
( const gui_level& lvl, position_type pos ) const
{
  const std::vector<std::size_t> layers( get_edit_layers( lvl ) );

  for ( std::size_t i( layers.size() ); i != 0; --i )
    {
      const item_selection& selection( lvl.get_selection( layers[ i - 1 ] ) );

      for ( item_selection::const_iterator it( selection.begin() );
            it!=selection.end(); ++it )
        {
          const rectangle_type box( lvl.get_visual_box( **it ) ); 

          if ( box.includes( pos ) )
            return *it;
        }
    }

  return NULL;
} // edit_mode::first_selected_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Finds all the items of the edited layers that overlap a given point.
 * \param lvl The level from which we take the items.
 * \param pos The point.
 */
std::vector<bf::item_instance*>
bf::edit_mode::find_items_at( const gui_level& lvl, position_type pos ) const
{
  std::vector<item_instance*> result;

  const std::vector<std::size_t> layers( get_edit_layers( lvl ) );

  for ( std::size_t i( layers.size() ); i != 0; --i )
    {
      const layer& the_layer( lvl.get_layer( layers[ i - 1 ] ) );

      for ( layer::item_iterator it( the_layer.item_begin() );
            it != the_layer.item_end(); ++it )
        {
          const rectangle_type box( lvl.get_visual_box( *it ) );

          if ( box.includes( pos ) )
            result.push_back( &(*it) );
        }
    }

  return result;
} // edit_mode::find_items_at()

/*----------------------------------------------------------------------------*/
/**
 * \brief Finds all the items of the edited layers intersecting a given box.
 * \param lvl The level from which we take the items.
 * \param box The box where to pick the items.
 */
std::vector<bf::item_instance*>
bf::edit_mode::pick_items( const gui_level& lvl, rectangle_type box ) const
{
  std::vector<item_instance*> result;

  const std::vector<std::size_t> layers( get_edit_layers( lvl ) );

  for ( std::size_t i( layers.size() ); i != 0; --i )
    {
      const layer& the_layer( lvl.get_layer( layers[ i - 1 ] ) );

      for ( layer::item_iterator it( the_layer.item_begin() );
            it != the_layer.item_end(); ++it)
        if ( box.intersects( lvl.get_visual_box( *it ) ) )
          result.push_back( &(*it) );
    }

  return result;
} // edit_mode::pick_items()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the indices of all layers having the same tag than the active
 *        layer.
 * \param lvl The level from which the layers are taken.
 */
std::vector<std::size_t>
bf::edit_mode::get_layer_indices_by_tag( const gui_level& lvl ) const
{
  std::vector<std::size_t> layers;

  const std::string ref_tag( lvl.get_active_layer().get_tag() );

  for ( std::size_t i(0); i!=lvl.layers_count(); ++i )
    if ( lvl.get_layer( i ).get_tag() == ref_tag )
      layers.push_back( i );

  return layers;
} // edit_mode::get_layer_indices_by_tag()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the indices of all layers.
 * \param lvl The level from which the layers are taken.
 */
std::vector<std::size_t>
bf::edit_mode::get_all_layers_indices( const gui_level& lvl ) const
{
  std::vector<std::size_t> layers;

  for ( std::size_t i(0); i!=lvl.layers_count(); ++i )
    layers.push_back( i );

  return layers;
} // edit_mode::get_all_layers_indices()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the selection in several layers.
 * \param lvl The level from which the items are taken.
 * \param layers The indices of the layers.
 * \param main_index The index of the layer from which the main selection is
 *        taken. If there is no selection in this layer then the main selection
 *        is taken from the first layer having a selection.
 */
bf::item_selection bf::edit_mode::get_selections_by_layer_index
( const gui_level& lvl, std::vector<std::size_t> layers,
  std::size_t main_index ) const
{
  item_selection result;

  if ( main_index == lvl.layers_count() )
    return result;

  for ( std::size_t i(0); i!=layers.size(); ++i )
    result.insert( lvl.get_selection( layers[i] ) );

  // This will for the main selection to be the one of main_index, if this layer
  // has a selection.
  result.insert( lvl.get_selection( main_index ) );

  return result;
} // edit_mode::get_selections_by_layer_index()
