/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A sequence of scene elements.
 * \author Julien Jorge
 */
#ifndef __VISUAL_SCENE_ELEMENT_SEQUENCE_HPP__
#define __VISUAL_SCENE_ELEMENT_SEQUENCE_HPP__

#include "bear/visual/base_scene_element.hpp"

#include "bear/visual/class_export.hpp"

namespace bear
{
  namespace visual
  {
    /**
     * \brief A sequence of scene elements on the screen.
     * \author Julien Jorge
     */
    class VISUAL_EXPORT scene_element_sequence:
      public base_scene_element
    {
    private:
      /** \brief The type of the container for the sub elements. */
      typedef std::list<scene_element> sequence_type;

    public:
      scene_element_sequence();

      template<typename Iterator>
      scene_element_sequence( Iterator first, Iterator last );

      base_scene_element* clone() const;

      void push_back( const scene_element& e );
      void push_front( const scene_element& e );

      rectangle_type get_opaque_box() const;
      rectangle_type get_bounding_box() const;

      void burst
      ( const rectangle_list& boxes, scene_element_list& output ) const;

      void render( base_screen& scr ) const;

      bool always_displayed() const;

    private:
      /** \brief The element_sequence. */
      sequence_type m_element;

    }; // class scene_element_sequence
  } // namespace visual
} // namespace bear

#include "bear/visual/impl/scene_element_sequence.tpp"

#endif // __VISUAL_SCENE_ELEMENT_SEQUENCE_HPP__
