/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::gui::picture class.
 * \author Julien Jorge
 */
#include "gui/picture.hpp"

#include "visual/scene_sprite.hpp"

#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param pict The sprite to display.
 */
bear::gui::picture::picture( const visual::sprite& pict )
  : scene_element(visual::scene_sprite(0, 0, pict))
{

} // picture::picture()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the displayed picture.
 * \param pict The picture to display.
 */
void bear::gui::picture::set_picture( const visual::sprite& pict )
{
  set_scene_element( visual::scene_sprite(0, 0, pict) );
} // picture::set_picture()
