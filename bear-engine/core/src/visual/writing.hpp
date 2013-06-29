/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A text written with some font.
 * \author Julien Jorge
 */
#ifndef __VISUAL_WRITING_HPP__
#define __VISUAL_WRITING_HPP__

#include "visual/class_export.hpp"

#include "visual/font/font.hpp"
#include "visual/sequence_effect.hpp"
#include "visual/text_align.hpp"

namespace bear
{
  namespace visual
  {
    class base_screen;
    class bitmap_writing;
    class scene_writing;

    /**
     * \brief A text written with some font.
     * \author Julien Jorge
     */
    class VISUAL_EXPORT writing
    {
    private:
      typedef bitmap_writing writing_type;

    public:
      typedef writing_type& writing_reference;
      typedef writing_type const& const_writing_reference;
      typedef writing_type* writing_pointer;
      typedef writing_type const* const_writing_pointer;

    public:
      writing();
      writing( const writing& that );
      writing
        ( const font& f, const std::string& str,
          text_align::horizontal_align h = text_align::align_left,
          text_align::vertical_align v = text_align::align_top );
      writing
        ( const font& f, const std::string& str, const size_box_type& s,
          text_align::horizontal_align h = text_align::align_left,
          text_align::vertical_align v = text_align::align_top );

      ~writing();

      writing& operator=( const writing& that );
      void swap( writing& that );

      void set_effect( sequence_effect e );
      void update( double t );

      coordinate_type get_width() const;
      coordinate_type get_height() const;
      const size_box_type& get_size() const;

      writing_reference operator*();
      const_writing_reference operator*() const;
      writing_pointer operator->();
      const_writing_pointer operator->() const;

      const_writing_pointer get_impl() const;

      void create
        ( const font& f, const std::string& str,
          text_align::horizontal_align h = text_align::align_left,
          text_align::vertical_align v = text_align::align_top );
      void create
        ( const font& f, const std::string& str, const size_box_type& s,
          text_align::horizontal_align h = text_align::align_left,
          text_align::vertical_align v = text_align::align_top );

      void call_render( const scene_writing& s, base_screen& scr ) const;

    private:
      /** \brief The writing on which we work. */
      writing_pointer m_writing;

      /** \brief Tell how many \b other instances of this class share the same
          m_writing. */
      std::size_t* m_counter;

    }; // class writing
  } // namespace visual
} // namespace bear

namespace std
{
  void swap( bear::visual::writing& a, bear::visual::writing& b );
} // namespace std

#endif // __VISUAL_WRITING_HPP__
