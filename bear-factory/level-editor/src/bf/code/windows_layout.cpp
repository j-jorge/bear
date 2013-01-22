/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::windows_layout class.
 * \author Julien Jorge
 */
#include "bf/windows_layout.hpp"

#include "bf/main_frame.hpp"
#include "bf/properties_frame.hpp"
#include "bf/layer_list_frame.hpp"
#include "bf/ingame_view_frame.hpp"

#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param class_pool All the item classes.
 * \param mf The main window.
 * \param pf The window displaying informations on an item.
 * \param llf The windows displayong the list of layers in the current level.
 */
bf::windows_layout::windows_layout
( const item_class_pool& class_pool, main_frame& mf, properties_frame& pf,
  layer_list_frame& llf )
  : m_class_pool(class_pool), m_main_frame(mf), m_properties_frame(pf),
    m_layer_list(llf), m_current_level_view(NULL)
{

} // windows_layout::windows_layout()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the item_class_pool.
 */
const bf::item_class_pool& bf::windows_layout::get_item_class_pool() const
{
  return m_class_pool;
} // windows_layout::get_item_class_pool()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the main window.
 */
bf::main_frame& bf::windows_layout::get_main_frame()
{
  return m_main_frame;
} // windows_layout::get_main_frame()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the window displaying informations on an item.
 */
bf::properties_frame& bf::windows_layout::get_properties_frame()
{
  return m_properties_frame;
} // windows_layout::get_properties_frame()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the windows displaying the list of layers in the current level.
 */
bf::layer_list_frame& bf::windows_layout::get_layer_list_frame()
{
  return m_layer_list;
} // windows_layout::get_layer_list_frame()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the current level view.
 */
bf::ingame_view_frame* bf::windows_layout::get_current_level_view()
{
  return m_current_level_view;
} // windows_layout::get_current_level_view()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the current level.
 */
bool bf::windows_layout::set_current_level_view( ingame_view_frame& view )
{
  bool result = false;

  // the close event can happen before the activate event
  if ( m_level_view.find(&view) != m_level_view.end() )
    if ( m_current_level_view != &view )
      {
        m_current_level_view = &view;
        m_main_frame.set_active_level(m_current_level_view);
        result = true;
      }

  return result;
} // windows_layout::set_current_level_view()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a level view.
 * \param view The view to add.
 */
void bf::windows_layout::add_level_view( ingame_view_frame& view )
{
  if ( m_level_view.empty() )
    m_main_frame.set_active_level(&view);

  m_level_view.insert(&view);
} // windows_layout::add_level_view()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove a level view.
 * \param view The view to remove.
 */
void bf::windows_layout::remove_level_view( ingame_view_frame& view )
{
  m_level_view.erase(&view);

  if (m_current_level_view == &view)
    {
      m_current_level_view = NULL;
      m_main_frame.set_active_level(m_current_level_view);
    }
} // windows_layout::remove_level_view()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the list of level view.
 */
bf::windows_layout::iterator bf::windows_layout::begin()
{
  return iterator( m_level_view.begin() );
} // windows_layout::begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the end of the list of level view.
 */
bf::windows_layout::iterator bf::windows_layout::end()
{
  return iterator( m_level_view.end() );
} // windows_layout::end()
