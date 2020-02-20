/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::gui::callback_group class.
 * \author Julien Jorge
 */
#include "bear/gui/callback_group.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a copy of this callback.
 */
bear::gui::callback_group* bear::gui::callback_group::clone() const
{
  return new callback_group(*this);
} // callback_group::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Execute the callback.
 */
void bear::gui::callback_group::execute()
{
  for ( std::size_t i=0; i!=m_group.size(); ++i )
    m_group[i].execute();
} // callback_group::execute()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a callback in the group.
 * \param c The callback to add.
 */
void bear::gui::callback_group::add( const callback& c )
{
  m_group.push_back(c);
} // callback_group::execute()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if there is no callback in the group.
 */
bool bear::gui::callback_group::empty() const
{
  return m_group.empty();
} // callback_group::empty()
