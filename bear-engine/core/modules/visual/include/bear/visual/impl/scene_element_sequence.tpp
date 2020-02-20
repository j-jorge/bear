/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the visual::scene_element_sequence.
 * \author Julien Jorge
 */

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param first Iterator of the first element of the sequence.
 * \param last Iterator just after the last element of the sequence.
 */
template<typename Iterator>
bear::visual::scene_element_sequence::scene_element_sequence
( Iterator first, Iterator last )
  : base_scene_element(0, 0), m_element(first, last)
{

} // scene_element_sequence::scene_element_sequence()

