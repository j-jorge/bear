/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class that loads a level file.
 * \author Julien Jorge
 */
#ifndef __ENGINE_LEVEL_LOADER_HPP__
#define __ENGINE_LEVEL_LOADER_HPP__

#include "audio/sample.hpp"
#include "universe/types.hpp"
#include "visual/color.hpp"
#include "visual/font/font.hpp"

#include "engine/easing.hpp"

#include "engine/class_export.hpp"
#include <vector>

namespace bear
{
  namespace engine
  {
    class base_item;
    class compiled_file;
    class item_loader_map;
    class layer;
    class level;
    class level_globals;

    /**
     * \brief This class loads a level from a compiled level file.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT level_loader
    {
    public:
      explicit level_loader
        ( compiled_file& f, const std::string& path,
          const level_globals* shared_resources = NULL );

      ~level_loader();

      unsigned int get_item_index() const;
      unsigned int get_items_count() const;

      level* drop_level();

      void complete_run();
      bool one_step();

    private:
      bool one_step_item();
      bool one_step_level();

      void load_layer();

      void validate_current_item();

      void load_item_declaration();
      void load_item_definition();
      void load_item();

      void load_item_field_list();

      void load_item_field_int();
      void load_item_field_u_int();
      void load_item_field_real();
      void load_item_field_bool();
      void load_item_field_string();
      void load_item_field_sprite();
      void load_item_field_animation();
      void load_item_field_item();
      void load_item_field_sample();
      void load_item_field_font();
      void load_item_field_color();
      void load_item_field_easing();

      void load_item_field_int_list();
      void load_item_field_u_int_list();
      void load_item_field_real_list();
      void load_item_field_bool_list();
      void load_item_field_string_list();
      void load_item_field_sprite_list();
      void load_item_field_animation_list();
      void load_item_field_item_list();
      void load_item_field_sample_list();
      void load_item_field_font_list();
      void load_item_field_color_list();
      void load_item_field_easing_list();

      base_item* create_item_from_string( const std::string& name ) const;
      layer* create_layer_from_string
      ( const std::string& name, const universe::size_box_type& s ) const;

      void escape( std::string& str ) const;

      audio::sample* load_sample_data() const;
      visual::font load_font_data() const;
      visual::color load_color_data() const;
      easing_function load_easing_data() const;

      template<typename T>
      std::string load_list( std::vector<T>& v );

    private:
      /** \brief The code of the next thing to read. */
      unsigned int m_next_code;

      /** \brief The level that we are building. */
      level* m_level;

      /** \brief The current layer. */
      layer* m_layer;

      /** \brief The file that we are reading. */
      compiled_file& m_file;

      /** \brief The item we are currently loading, if any. */
      base_item* m_current_item;

      /** \brief The loaders for the current item. */
      item_loader_map* m_current_loader;

      /** \brief Referenced items. */
      std::vector<base_item*> m_referenced;

      /** \brief Count of items in the level. */
      unsigned int m_items_count;

      /** \brief Index of the currently built item. */
      unsigned int m_item_index;

      /** \brief Index of the next item definition to read. */
      unsigned int m_referenced_index;

      /** \brief The major version of the level. */
      unsigned int m_maj;

      /** \brief The major version of the level. */
      unsigned int m_min; 

      /** \brief The release version of the level. */
      unsigned int m_rel;

    }; // class level_loader
  } // namespace engine
} // namespace bear

#endif // __ENGINE_LEVEL_LOADER_HPP__
