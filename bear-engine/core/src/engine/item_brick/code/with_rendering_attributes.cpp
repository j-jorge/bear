/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::with_rendering_attributes class.
 * \author Julien Jorge
 */
#include "engine/item_brick/with_rendering_attributes.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::engine::with_rendering_attributes::~with_rendering_attributes()
{
  // Nothing to do.
} // with_rendering_attributes::~with_rendering_attributes()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the rendering attributes.
 */
bear::visual::bitmap_rendering_attributes&
bear::engine::with_rendering_attributes::get_rendering_attributes()
{
  return m_rendering_attributes;
} // with_rendering_attributes::get_rendering_attributes()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the rendering attributes.
 */
const bear::visual::bitmap_rendering_attributes&
bear::engine::with_rendering_attributes::get_rendering_attributes() const
{
  return m_rendering_attributes;
} // with_rendering_attributes::get_rendering_attributes()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the rendering attributes.
 * \param attr The rendering attributes.
 */
void bear::engine::with_rendering_attributes::set_rendering_attributes
( const visual::bitmap_rendering_attributes& attr )
{
  m_rendering_attributes = attr;
} // with_rendering_attributes::set_rendering_attributes()
