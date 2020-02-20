/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::level_clipboard class.
 * \author Julien Jorge
 */
#include "bf/level_clipboard.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bf::level_clipboard::level_clipboard()
  : m_content_type( content_none )
{

} // level_clipboard::level_clipboard()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if this clipboard contains items that can be pasted.
 */
bool bf::level_clipboard::has_items() const
{
  return m_content_type == content_items;
} // level_clipboard::has_items()

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns the last items added to the clipboard.
 */
bf::item_copy bf::level_clipboard::get_items() const
{
  return m_items;
} // level_clipboard::get_items()

/*----------------------------------------------------------------------------*/
/**
 * \brief Stores a copy of a collection of items for future pastes.
 * \param copy The objects to store.
 */
void bf::level_clipboard::copy_items( item_copy copy )
{
  m_content_type = content_items;

  m_items = copy;
} // level_clipboard::copy_items()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if this clipboard contains a layer that can be pasted.
 */
bool bf::level_clipboard::has_layer() const
{
  return m_content_type == content_layer;
} // level_clipboard::has_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns the last layer copied in the clipboard.
 */
bf::layer bf::level_clipboard::get_layer() const
{
  return m_layer;
} // level_clipboard::get_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Stores a copy of a layer for future pastes.
 * \param layer The layer to store.
 */
void bf::level_clipboard::copy_layer( layer the_layer )
{
  m_content_type = content_layer;

  m_layer = the_layer;
} // level_clipboard::copy_layer()
