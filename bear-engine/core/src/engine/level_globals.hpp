/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Some global classes in a level.
 * \author Julien Jorge
 */
#ifndef __ENGINE_LEVEL_GLOBALS_HPP__
#define __ENGINE_LEVEL_GLOBALS_HPP__

#include "audio/sound_manager.hpp"
#include "visual/animation.hpp"
#include "visual/image_manager.hpp"
#include "visual/font/font_manager.hpp"
#include "communication/post_office.hpp"
#include "engine/model/model_actor.hpp"

#include "engine/class_export.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief Some global classes in a level: the image_manager, the
     *        sound_manager and the post office.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT level_globals
    {
    private:
      /** \brief The type of the key identifying a sprite loaded from a
          spritepos file. */
      typedef std::pair<std::string, std::string> spritepos_entry;

      /** \brief The type of the map storing the sprites loaded from the
          spritepos files. */
      typedef std::map<spritepos_entry, visual::sprite> auto_sprite_cache;

    public:
      level_globals();
      level_globals
        ( const level_globals* shared,
          const level_globals* temporary_resources );

      void add_image
        ( const std::string& file_name, const bear::visual::image& image );

      void load_image( const std::string& file_name );
      void load_sound( const std::string& file_name );
      void load_model( const std::string& file_name );
      void load_animation( const std::string& file_name );
      void load_font( const std::string& file_name );
      void load_shader( const std::string& file_name );

      visual::image get_image( const std::string& name );
      visual::image get_existing_image( const std::string& name ) const;

      const model_actor& get_model( const std::string& name );
      const model_actor& get_existing_model( const std::string& name ) const;

      const visual::animation& get_animation( const std::string& name );
      const visual::animation&
        get_existing_animation( const std::string& name ) const;

      visual::font get_font( const std::string& name, double size );
      visual::font get_existing_font
        ( const std::string& name, double size ) const;

      visual::shader_program get_shader( const std::string& name );
      visual::shader_program get_existing_shader
        ( const std::string& name ) const;

      std::string get_spritepos_path( const std::string& image_name ) const;
      visual::sprite auto_sprite
      ( const std::string& image_name, const std::string& sprite_name );

      void play_sound( const std::string& name );
      void play_sound
      ( const std::string& name, const audio::sound_effect& effect );

      audio::sample* new_sample( const std::string& name );
      audio::sample* new_sample( const audio::sample& s );
      std::size_t play_music( const std::string& name, unsigned int loops = 0 );
      void stop_music( std::size_t id, double fadeout = 0 );
      void stop_all_musics( double fadeout = 0 );

      void pause_audio();
      void resume_audio();

      static void global_set_sound_muted( bool m );
      static bool global_get_sound_muted();
      static void global_set_music_muted( bool m );
      static bool global_get_music_muted();

      static void global_set_sound_volume( double m );
      static double global_get_sound_volume();
      static void global_set_music_volume( double m );
      static double global_get_music_volume();

      void set_sound_volume( double v );
      void set_music_volume( double v );

      void mute( bool m );
      void mute_sound( bool m );
      void mute_music( bool m );

      bool image_exists( const std::string& name ) const;
      bool sound_exists( const std::string& name ) const;
      bool model_exists( const std::string& name ) const;
      bool animation_exists( const std::string& name ) const;
      bool font_exists( const std::string& name ) const;
      bool shader_exists( const std::string& name ) const;

      void register_item( communication::messageable& item );
      void release_item( communication::messageable& item );
      bool send_message
      ( const std::string& target, communication::message& msg ) const;

      void set_ears_position
        ( const claw::math::coordinate_2d<double>& position );

      void set_full_volume_distance( double d );
      double get_full_volume_distance() const;

      void set_silence_distance( double d );
      double get_silence_distance() const;

      void set_distance_unit( double d );
      double get_distance_unit() const;

      void restore_resources();

      void freeze();

    private:
      void warn_missing_ressource( std::string name ) const;

      void restore_images();
      void restore_shader_programs();

      void constructor_default();

    private:
      /** \brief Another level_globals from which we can take the resources
          instead of building new ones. */
      const level_globals* m_shared_resources;

      /** \brief Another level_globals from which we can temporarily take the
          resources instead of building new ones. */
      const level_globals* m_temporary_resources;

      /** \brief The image resources in the level. */
      visual::image_manager m_image_manager;

      /** \brief The font resources in the level. */
      visual::font_manager m_font_manager;

      /** \brief The sound resources in the level. */
      audio::sound_manager m_sound_manager;

      /** \brief The post office in the level. */
      communication::post_office m_post_office;

      /** \brief The models of the items in the level. */
      std::map<std::string, model_actor> m_model;

      /** \brief The animations in the level. */
      std::map<std::string, visual::animation> m_animation;

      /** \brief This map stores the sprites read from spritepos files. */
      auto_sprite_cache m_auto_sprite_cache;

      /** \brief Tells if no more resources are supposed to be created. */
      bool m_frozen;

      /** \brief The volume of the sounds of the game. */
      static double s_sound_volume;

      /** \brief The volume of the music of the game. */
      static double s_music_volume;

      /** \brief Tell if the sounds are muted. */
      static bool s_sound_muted;

      /** \brief Tell if the music is muted. */
      static bool s_music_muted;

    }; // struct level_globals
  } // namespace engine
} // namespace bear

#endif // __ENGINE_LEVEL_GLOBALS_HPP__
