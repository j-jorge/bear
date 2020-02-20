/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief One level in the game.
 * \author Julien Jorge
 */
#ifndef __ENGINE_LEVEL_HPP__
#define __ENGINE_LEVEL_HPP__

#include <vector>

#include "bear/engine/layer/gui_layer_stack.hpp"
#include "bear/engine/layer/layer.hpp"
#include "bear/engine/variable/var_map.hpp"
#include "bear/visual/screen.hpp"

#include "bear/engine/class_export.hpp"

#include <claw/multi_type_map.hpp>
#include <claw/meta/type_list.hpp>
#include <claw/functional.hpp>
#include <claw/iterator.hpp>

#include <boost/signals2.hpp>

namespace bear
{
  namespace engine
  {
    class base_variable;
    class level_globals;
    class level_loader;

    /**
     * \brief One level in the game.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT level
    {
    private:
      typedef layer::region_type region_type;
      typedef std::map<universe::const_item_handle, universe::size_box_type>
      activity_map_type;
      
      /** \brief The type of the list containing the layers on the level. */
      typedef std::vector<layer*> layer_vector;

    public:
      /** \brief The type of the iterators on the layers of the level. */
      typedef claw::wrapped_iterator
        < const layer,
          layer_vector::const_iterator,
          claw::const_dereference<layer> >
        ::iterator_type const_layer_iterator;

      /** \brief The type of the iterators on the layers of the level. */
      typedef claw::wrapped_iterator
        < layer,
          layer_vector::iterator,
          claw::dereference<layer> >
        ::iterator_type layer_iterator;

    public:
      level
      ( const std::string& name, const std::string& filename,
        const universe::size_box_type& level_size,
        const std::string& level_music,
        const level_globals* shared_resources,
        const level_globals* resource_source );
      ~level();

      void start();
      boost::signals2::connection on_started( boost::function<void ()> f );
      void stop();

      void progress( universe::time_type elapsed_time );
      boost::signals2::connection
        on_progress_done( boost::function<void ()> f );

      void render( visual::screen& screen ) const;
      visual::scene_element
      element_to_screen_coordinates( const visual::scene_element& e ) const;

      void shot( visual::screen& screen, claw::graphic::image& img ) const;

      void set_overview_activated( bool b );
      bool get_overview_activated() const;

      const universe::size_box_type& get_size() const;
      unsigned int get_depth() const;
      const std::string& get_name() const;
      const std::string& get_filename() const;
      level_globals& get_globals();

      void set_camera( base_item& cam );
      void add_interest_around( const base_item* item );
      void add_interest_around
      ( const base_item* item, const universe::size_box_type& s );

      void push_layer( layer* the_layer );
      void push_layer( gui_layer* the_layer );

      void move_layer( const layer& the_layer, int distance );

      layer_iterator layer_begin();
      layer_iterator layer_end();

      const_layer_iterator layer_begin() const;
      const_layer_iterator layer_end() const;

      void set_pause();
      void unset_pause();
      bool is_paused() const;

      void play_music();
      void stop_music( universe::time_type fade_duration = 0 );

      universe::item_handle get_camera();
      universe::size_box_type get_camera_size() const;
      universe::rectangle_type get_camera_focus() const;
      universe::position_type get_camera_center() const;

      void set_ears( universe::item_handle ears );

      universe::position_type screen_to_level( visual::position_type p ) const;
      
      void get_level_variable( base_variable& val ) const;
      void set_level_variable( const base_variable& val );
      bool level_variable_exists( const base_variable& val ) const;

    private:
      void render_layers( visual::screen& screen ) const;
      void render_layers
        ( visual::screen& screen, const universe::rectangle_type& view ) const;

      void render_gui( visual::screen& screen ) const;
      void render
      ( const std::list<scene_visual>& visuals,
        const universe::position_type& cam_pos, visual::screen& screen,
        double r_w, double r_h ) const;
      visual::scene_element element_to_screen_coordinates
      ( const visual::scene_element& e, const universe::position_type& cam_pos,
        double r_w, double r_h ) const;

      void clear();

      void get_layer_region
      ( unsigned int layer_index, region_type& the_region ) const;
      void get_layer_area
      ( unsigned int layer_index, universe::rectangle_type& area ) const;

      void get_active_regions( region_type& active_regions );

      void add_region
      ( region_type& active_regions, const universe::rectangle_type& r,
        const universe::size_box_type& s ) const;

      void set_sound_distance_unit() const;

    private:
      /** \brief The name of the level. */
      const std::string m_name;

      /** \brief The filename of the level. */
      const std::string m_filename;

      /** \brief Visible/active part of the level. */
      universe::item_handle m_camera;

      /** \brief A set of items around which the activity is. */
      activity_map_type m_activity;

      /** \brief The layers of the level, from the back to the front. */
      layer_vector m_layers;

      /** \brief The size of the level. */
      universe::size_box_type m_level_size;

      /** \brief Resources of the level. */
      level_globals* m_level_globals;

      /** \brief The default music to play in the level. */
      std::string m_music;

      /** \brief The identifier of the music when played. */
      std::size_t m_music_id;

      /** \brief The layers of the interface. */
      gui_layer_stack m_gui;

      /** \brief How many times this level is currently paused. */
      unsigned int m_paused;

      /** \brief Variables global to the level. */
      var_map m_level_variables;

      /** \brief Tell to render the whole level in the screen. */
      bool m_overview_activated;

      /** \brief The item to use to set the ears in the sound manager. */
      universe::item_handle m_ears;

      /** \brief The signal emitted when the level starts. */
      boost::signals2::signal<void ()> m_started_signal;

      /** \brief The signal emitted when the progress of the whole level is
          done. */
      boost::signals2::signal<void ()> m_progress_done_signal;

    }; // class level
  } // namespace engine
} // namespace bear

#endif // __ENGINE_LEVEL_HPP__
