/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class for writing source level files.
 * \author Julien Jorge
 */
#ifndef __BF_XML_VALUE_TO_XML_HPP__
#define __BF_XML_VALUE_TO_XML_HPP__

#include "bf/any_animation.hpp"
#include "bf/color.hpp"
#include "bf/custom_type.hpp"
#include "bf/font.hpp"
#include "bf/sample.hpp"
#include "bf/libeditor_export.hpp"

#include <iostream>
#include <wx/xml/xml.h>

namespace bf
{
  namespace xml
  {
    /**
     * \brief A class to convert a value into an XML representation.
     * \author Julien Jorge
     */
    template<typename Type>
    class value_to_xml
    {
    public:
      static void write
      ( std::ostream& os, const std::string& node_name, const Type& v );
    }; // class value_to_xml

    /**
     * \brief A class for common code in value_to_xml<animation> and
     *        value_to_xml<sprite>.
     * \author Julien Jorge
     */
    class BEAR_EDITOR_EXPORT base_sprite_animation_value_to_xml
    {
    protected:
      static void bitmap_rendering_attributes_xml
      ( std::ostream& os, const bitmap_rendering_attributes& att );

    }; // class base_sprite_animation_value_to_xml

    /**
     * \brief A class to convert a value into an XML representation.
     *        Specialisation for the bf::sprite class.
     * \author Julien Jorge
     */
    template<>
    class BEAR_EDITOR_EXPORT value_to_xml<sprite>:
      public base_sprite_animation_value_to_xml
    {
    public:
      static void write( std::ostream& os, const sprite& spr );
    }; // class value_to_xml [sprite]

    /**
     * \brief A class to convert a value into an XML representation.
     *        Specialisation for the bf::animation class.
     * \author Julien Jorge
     */
    template<>
    class BEAR_EDITOR_EXPORT value_to_xml<animation>:
      public base_sprite_animation_value_to_xml
    {
    public:
      static void write( std::ostream& os, const animation& anim );
    }; // class value_to_xml [animation]

    /**
     * \brief A class to convert a value into an XML representation.
     *        Specialisation for the bf::animation_file_type class.
     * \author Julien Jorge
     */
    template<>
    class BEAR_EDITOR_EXPORT value_to_xml<animation_file_type>:
      public base_sprite_animation_value_to_xml
    {
    public:
      static void write( std::ostream& os, const animation_file_type& anim );
    }; // class value_to_xml [animation_file_type]

    /**
     * \brief A class to convert a value into an XML representation.
     *        Specialisation for the bf::any_animation class.
     * \author Julien Jorge
     */
    template<>
    class BEAR_EDITOR_EXPORT value_to_xml<any_animation>
    {
    public:
      static void write( std::ostream& os, const any_animation& anim );
    }; // class value_to_xml [any_animation]

    /**
     * \brief A class to convert a value into an XML representation.
     *        Specialisation for the bf::sample class.
     * \author Julien Jorge
     */
    template<>
    class BEAR_EDITOR_EXPORT value_to_xml<sample>
    {
    public:
      static void write( std::ostream& os, const sample& s );
    }; // class value_to_xml [sample]

    /**
     * \brief A class to convert a value into an XML representation.
     *        Specialisation for the bf::font class.
     * \author Julien Jorge
     */
    template<>
    class BEAR_EDITOR_EXPORT value_to_xml<font>
    {
    public:
      static void write( std::ostream& os, const font& s );
    }; // class value_to_xml [font]

    /**
     * \brief A class to convert a value into an XML representation.
     *        Specialisation for the bf::color class.
     * \author Julien Jorge
     */
    template<>
    class BEAR_EDITOR_EXPORT value_to_xml<color>
    {
    public:
      static void write( std::ostream& os, const color& f );
    }; // class value_to_xml [color]

    /**
     * \brief A class to convert a value into an XML representation.
     *        Specialisation for the bf::easing_type class.
     * \author Julien Jorge
     */
    template<>
    class BEAR_EDITOR_EXPORT value_to_xml<easing_type>
    {
    public:
      static void write( std::ostream& os, const easing_type& f );
    }; // class value_to_xml [easing_type]

  } // namespace xml
} // namespace bf

#include "bf/xml/impl/value_to_xml.tpp"

#endif // __BF_XML_VALUE_TO_XML_HPP__
