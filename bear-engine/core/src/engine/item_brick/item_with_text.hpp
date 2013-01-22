/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief An item with a text inside.
 * \author Julien Jorge
 */
#ifndef __ENGINE_ITEM_WITH_TEXT_HPP__
#define __ENGINE_ITEM_WITH_TEXT_HPP__

#include "engine/scene_visual.hpp"
#include "engine/item_brick/with_text.hpp"
#include "universe/types.hpp"
#include "visual/font/font.hpp"
#include "visual/writing.hpp"

#include <list>

namespace bear
{
  namespace engine
  {
    /**
     * \brief An item with a text inside.
     *
     * \b template \b parameters :
     * - \a Base: the base class for this item. Must inherit from
     *    basic_renderable_item.
     *
     * The custom fields of this class are:
     * - text: string, the text in the item (default = empty string),
     * - font: font, the font to use to display the text (default = none),
     * - text_inside: boolean, tell if the text is drawn only in the bounds of
     *   the item (default = false),
     * - stretched_text: boolean, tell if the text has to be stretched to fit
     *   the bounds of the item (default = false),
     * - scale_to_fit: boolean, tell if the text has to be scaled to fit
     *   the bounds of the item (default = false),
     * - any field supported by the parent classes.
     *
     * \author Julien Jorge
     */
    template<class Base>
    class item_with_text:
      public Base,
      public with_text
    {
      typedef Base super;

    public:
      item_with_text();

      void build();

      bool set_string_field
      ( const std::string& name, const std::string& value );
      bool set_bool_field( const std::string& name, bool value );
      bool set_font_field( const std::string& name, visual::font value );

      void progress( universe::time_type elapsed_time );
      void get_visual( std::list<scene_visual>& visuals ) const;

      void set_text_inside( bool b );
      void set_scale_to_fit( bool b );
      void set_stretched_text( bool b );
      void fit_to_text();

    private:
      bear::visual::position_type 
      get_gap_for_scale_to_fit( const visual::scene_writing& e ) const;

    private:
      /** \brief Tell if the text is drawn only in the bounds of the item. */
      bool m_text_inside;

      /** \brief Tell if the text has to be stretched to fit the bounds of the
          item. */
      bool m_stretched_text;

      /** \brief Tell if the text has to be scaled to fit the bounds of the
          item. */
      bool m_scale_to_fit;

    }; // class item_with_text
  } // namespace engine
} // namespace bear

#include "engine/item_brick/impl/item_with_text.tpp"

#endif // __ENGINE_ITEM_WITH_TEXT_HPP__
