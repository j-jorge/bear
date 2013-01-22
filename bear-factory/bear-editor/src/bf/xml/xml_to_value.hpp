/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A class for reading source xml files.
 * \author Julien Jorge
 */
#ifndef __BF_XML_XML_TO_VALUE_HPP__
#define __BF_XML_XML_TO_VALUE_HPP__

#include "bf/any_animation.hpp"
#include "bf/color.hpp"
#include "bf/font.hpp"
#include "bf/sample.hpp"
#include "bf/libeditor_export.hpp"

#include <wx/xml/xml.h>

namespace bf
{
  namespace xml
  {
    /**
     * \brief A class to convert an XML node into real data.
     * \author Julien Jorge
     */
    template<typename Type>
    class xml_to_value
    {
    public:
      void operator()( Type& v, const wxXmlNode* node ) const;

    }; // class xml_to_value

    /**
     * \brief A class for common code in
     *        xml_to_value<something with bitmap_rendering_attributes>.
     * \author Julien Jorge
     */
    class BEAR_EDITOR_EXPORT bitmap_rendering_attributes_xml_to_value
    {
    protected:
      void load_rendering_attributes
        ( bitmap_rendering_attributes& att, const wxXmlNode* node  ) const;

    }; // class bitmap_rendering_attributes_xml_to_value

    /**
     * \brief A class to convert an XML node into real data. Specialisation for
     *        the bf::sprite class.
     */
    template<>
    class BEAR_EDITOR_EXPORT xml_to_value<sprite>:
      public bitmap_rendering_attributes_xml_to_value
    {
    public:
      void operator()( sprite& v, const wxXmlNode* node ) const;

    }; // class xml_to_value [sprite]

    /**
     * \brief A class to convert an XML node into real data. Specialisation for
     *        the bf::animation class.
     */
    template<>
    class BEAR_EDITOR_EXPORT xml_to_value<animation>:
      public bitmap_rendering_attributes_xml_to_value
    {
    public:
      static bool supported_node( const wxString& node_name );
      void operator()( animation& v, const wxXmlNode* node ) const;

    private:
      void load_frames( animation& anim, const wxXmlNode* node ) const;
      void load_frame( animation& anim, const wxXmlNode* node ) const;

    }; // class xml_to_value [animation]

    /**
     * \brief A class to convert an XML node into real data. Specialisation for
     *        the bf::animation_file_type class.
     */
    template<>
    class BEAR_EDITOR_EXPORT xml_to_value<animation_file_type>:
      public bitmap_rendering_attributes_xml_to_value
    {
    public:
      static bool supported_node( const wxString& node_name );

      void operator()( animation_file_type& v, const wxXmlNode* node ) const;

    }; // class xml_to_value [animation_file_type]

    /**
     * \brief A class to convert an XML node into real data. Specialisation for
     *        the bf::any_animation class.
     */
    template<>
    class BEAR_EDITOR_EXPORT xml_to_value<any_animation>
    {
    public:
      static bool supported_node( const wxString& node_name );

      void operator()( any_animation& v, const wxXmlNode* node ) const;

    }; // class xml_to_value [any_animation]

    /**
     * \brief A class to convert an XML node into real data. Specialisation for
     *        the bf::sample class.
     */
    template<>
    class BEAR_EDITOR_EXPORT xml_to_value<sample>
    {
    public:
      void operator()( sample& v, const wxXmlNode* node ) const;

    }; // class xml_to_value [sample]

    /**
     * \brief A class to convert an XML node into real data. Specialisation for
     *        the bf::font class.
     */
    template<>
    class BEAR_EDITOR_EXPORT xml_to_value<font>
    {
    public:
      void operator()( font& v, const wxXmlNode* node ) const;

    }; // class xml_to_value [font]

    /**
     * \brief A class to convert an XML node into real data. Specialisation for
     *        the bf::color class.
     */
    template<>
    class BEAR_EDITOR_EXPORT xml_to_value<color>
    {
    public:
      void operator()( color& v, const wxXmlNode* node ) const;

    }; // class xml_to_value [color]
  } // namespace xml
} // namespace bf

#include "bf/xml/impl/xml_to_value.tpp"

#endif // __BF_XML_XML_TO_VALUE_HPP__
