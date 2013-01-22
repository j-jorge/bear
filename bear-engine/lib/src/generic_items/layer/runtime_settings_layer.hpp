/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief This layer displays the runtime settings with some components to edit
 *        their values.
 * \author Julien Jorge
 */
#ifndef __BEAR_RUNTIME_SETTINGS_LAYER_HPP__
#define __BEAR_RUNTIME_SETTINGS_LAYER_HPP__

#include "engine/layer/base_debugging_layer.hpp"
#include "gui/text_input.hpp"
#include "gui/visual_component.hpp"

#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief This layer displays the runtime settings with some components to
   *        edit their values.
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT runtime_settings_layer:
    public engine::gui_layer
  {
  private:
    /**
     * \brief The class to pass to the visitor of the global settings in order
     *        to get the settings.
     */
    class setting_visitor
    {
    public:
      setting_visitor( runtime_settings_layer& layer );

      template<typename T>
      void operator()( std::string name, T var ) const;

    private:
      /** \brief The layer to which we pass the settings. */
      runtime_settings_layer& m_layer;

    }; // class setting_visitor

  public:
    runtime_settings_layer( visual::font f );
    ~runtime_settings_layer();

    void render( scene_element_list& e ) const;

    bool key_pressed( const input::key_info& key );

  private:
    void build_components();
    void clear();
    void update();

    template<typename T>
    void process_setting( std::string name, T* var ) const;

    template<typename T>
    void on_enter( T* var, gui::text_input* value ) const;

  private:
    /** \brief The value of the key that changes the visibility of the
        layer. */
    const input::key_code m_toggle_key;

    /** \brief The font to use to display the labels and the values. */
    visual::font m_font;

    /** \brief The container of all components. */
    gui::visual_component* m_root_component;

    /** \brief The component in which the user can enter a pattern to filter the
        settings. */
    gui::text_input* m_pattern;

    /** \brief The component that displays the settings. */
    gui::visual_component* m_settings;

    /** \brief The margin between the components. */
    static const gui::size_type s_margin;

    /** \brief The cursor of the cursor in the text input components. */
    static const gui::color_type s_cursor_color;

  }; // class runtime_settings_layer
} // namespace bear

#endif // __BEAR_RUNTIME_SETTINGS_LAYER_HPP__
