/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::level_globals class.
 * \author Julien Jorge
 */
#include "engine/level_globals.hpp"

#include "engine/bitmap_font_loader.hpp"
#include "engine/model_loader.hpp"
#include "engine/resource_pool.hpp"
#include "engine/shader_loader.hpp"
#include "engine/sprite_loader.hpp"
#include "engine/spritepos.hpp"

#include <sstream>
#include <cassert>
#include <claw/logger.hpp>
#include <claw/exception.hpp>

#include <boost/algorithm/string/predicate.hpp>

/*----------------------------------------------------------------------------*/
double bear::engine::level_globals::s_sound_volume(1);
double bear::engine::level_globals::s_music_volume(1);
bool bear::engine::level_globals::s_sound_muted(false);
bool bear::engine::level_globals::s_music_muted(false);

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::engine::level_globals::level_globals()
  : m_shared_resources(NULL), m_frozen(false)
{
  constructor_default();
} // level_globals::level_globals()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param shared Another level_globals from which we can take the resources
 *        instead of building new ones.
 */
bear::engine::level_globals::level_globals( const level_globals* shared )
  : m_shared_resources( shared ), m_frozen(false)
{
  constructor_default();
} // level_globals::level_globals()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load an image.
 * \param file_name The name of the file to load the image from.
 */
void bear::engine::level_globals::load_image( const std::string& file_name )
{
  if ( !image_exists(file_name) )
    {
      claw::logger << claw::log_verbose << "loading image '" << file_name
                   << "'." << std::endl;

      std::stringstream f;
      resource_pool::get_instance().get_file(file_name, f);

      if (f)
        m_image_manager.load_image(file_name, f);
      else
        claw::logger << claw::log_error << "can not open file '" << file_name
                     << "'." << std::endl;
    }
} // level_globals::load_image()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load a sound.
 * \param file_name The name of the file to load the sound from.
 */
void bear::engine::level_globals::load_sound( const std::string& file_name )
{
  // The sounds cannot be shared between the level globals, thus we search only
  // in this instance.
  if ( !m_sound_manager.sound_exists(file_name) )
    {
      claw::logger << claw::log_verbose << "loading sound '" << file_name
                   << "'." << std::endl;

      std::stringstream f;
      resource_pool::get_instance().get_file(file_name, f);

      if (f)
        m_sound_manager.load_sound(file_name, f);
      else
        claw::logger << claw::log_error << "can not open file '" << file_name
                     << "'." << std::endl;
    }
} // level_globals::load_sound()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load a model.
 * \param file_name The name of the file to load the model from.
 */
void bear::engine::level_globals::load_model( const std::string& file_name )
{
  if ( !model_exists(file_name) )
    {
      claw::logger << claw::log_verbose << "loading model '" << file_name
                   << "'." << std::endl;

      std::stringstream f;
      resource_pool::get_instance().get_file(file_name, f);

      if (f)
        {
          model_loader ldr( f, *this );
          model_actor* m = ldr.run();
          m_model[file_name] = *m;
          delete m;
        }
      else
        claw::logger << claw::log_error << "can not open file '" << file_name
                     << "'." << std::endl;
    }
} // level_globals::load_model()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load an animation.
 * \param file_name The name of the file to load the animation from.
 */
void bear::engine::level_globals::load_animation( const std::string& file_name )
{
  if ( !animation_exists(file_name) )
    {
      claw::logger << claw::log_verbose << "loading animation '" << file_name
                   << "'." << std::endl;

      std::stringstream f;
      resource_pool::get_instance().get_file(file_name, f);

      if (f)
        {
          compiled_file cf(f, true);
          sprite_loader ldr;
          m_animation[file_name] = ldr.load_animation( cf, *this );
        }
      else
        claw::logger << claw::log_error << "can not open file '" << file_name
                     << "'." << std::endl;
    }
} // level_globals::load_animation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load a font.
 * \param file_name Path to the font file.
 * \todo Implement the support of true type fonts.
 */
void bear::engine::level_globals::load_font( const std::string& file_name )
{
  if ( !font_exists(file_name) )
    {
      claw::logger << claw::log_verbose << "loading font '" << file_name
                   << "'." << std::endl;

      std::stringstream f;
      resource_pool::get_instance().get_file(file_name, f);

      if (f)
        {
          if ( boost::algorithm::ends_with( file_name, ".fnt" ) )
            {
              bitmap_font_loader ldr( f, *this );
              m_font_manager.load_font( file_name, ldr.run() );
            }
          else
            m_font_manager.load_font( file_name, f );
        }
      else
        claw::logger << claw::log_error << "can not open file '" << file_name
                     << "'." << std::endl;
    }
} // level_globals::load_font()

/*----------------------------------------------------------------------------*/
/**
 * \brief Loads a shader program.
 * \param file_name The name of the file to load the shader from.
 */
void bear::engine::level_globals::load_shader( const std::string& file_name )
{
  if ( !shader_exists(file_name) )
    {
      claw::logger << claw::log_verbose << "loading shader '" << file_name
                   << "'." << std::endl;

      std::stringstream f;
      shader_loader::parse_shader_file( f, file_name );

      if (f)
        m_image_manager.load_shader_program(file_name, f);
      else
        claw::logger << claw::log_error << "can not open file '" << file_name
                     << "'." << std::endl;
    }
} // level_globals::load_shader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets an image.
 * \param name The name of the image to get.
 * \pre There is an image named \a name.
 */
bear::visual::image
bear::engine::level_globals::get_image( const std::string& name )
{
  if ( !image_exists(name) )
    {
      warn_missing_ressource( name );
      load_image(name);
    }

  return get_existing_image( name );
} // level_globals::get_image()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets an image with a given name. The image must exist in this instance
 *        or in the shared resources.
 * \pre There is an image named \a name.
 */
bear::visual::image
bear::engine::level_globals::get_existing_image( const std::string& name ) const
{
  CLAW_PRECOND( image_exists( name ) );

  if ( m_image_manager.exists( name ) )
    return m_image_manager.get_image( name );
  else
    return m_shared_resources->get_existing_image( name );
} // level_globals::get_existing_image()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets a model.
 * \param name The name of the model to get.
 */
const bear::engine::model_actor&
bear::engine::level_globals::get_model( const std::string& name )
{
  if ( !model_exists(name) )
    {
      warn_missing_ressource( name );
      load_model(name);
    }

  return get_existing_model( name );
} // level_globals::get_model()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets a model with a given name. The model must exist in this instance
 *        or in the shared resources.
 * \param name The name of the model to get.
 */
const bear::engine::model_actor&
bear::engine::level_globals::get_existing_model( const std::string& name ) const
{
  CLAW_PRECOND( model_exists( name ) );

  if ( m_model.find( name ) != m_model.end() )
    return m_model.find( name )->second;
  else
    return m_shared_resources->get_existing_model( name );
} // level_globals::get_existing_model()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an animation.
 * \param name The name of the animation to get.
 */
const bear::visual::animation&
bear::engine::level_globals::get_animation( const std::string& name )
{
  if ( !animation_exists(name) )
    {
      warn_missing_ressource( name );
      load_animation(name);
    }

  return get_existing_animation( name );
} // level_globals::get_animation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets a animation with a given name. The animation must exist in this
 *        instance or in the shared resources.
 * \param name The name of the animation to get.
 */
const bear::visual::animation&
bear::engine::level_globals::get_existing_animation
( const std::string& name ) const
{
  CLAW_PRECOND( animation_exists( name ) );

  if ( m_animation.find( name ) != m_animation.end() )
    return m_animation.find( name )->second;
  else
    return m_shared_resources->get_existing_animation( name );
} // level_globals::get_existing_animation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets a font, loads it if it does not exist yet.
 * \param name The name of the font.
 * \param size The size of the font.
 */
bear::visual::font
bear::engine::level_globals::get_font( const std::string& name, double size )
{
  if ( !font_exists(name) )
    {
      warn_missing_ressource( name );
      load_font(name);
    }

  return get_existing_font( name, size );
} // level_globals::get_font()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets a font with a given name. The font must exist in this instance
 *        or in the shared resources.
 * \param name The name of the font to get.
 * \param size The size of the font.
 */
bear::visual::font
bear::engine::level_globals::get_existing_font
( const std::string& name, double size ) const
{
  CLAW_PRECOND( font_exists( name ) );

  if ( m_font_manager.exists( name ) )
    return m_font_manager.get_font( name, size );
  else
    return m_shared_resources->get_existing_font( name, size );
} // level_globals::get_existing_font()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets a shader.
 * \param name The name of the shader to get.
 * \pre There is a shader named \a name.
 */
bear::visual::shader_program
bear::engine::level_globals::get_shader( const std::string& name )
{
  if ( !shader_exists(name) )
    {
      warn_missing_ressource( name );
      load_shader(name);
    }

  return get_existing_shader( name );
} // level_globals::get_shader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets a shader with a given name. The shader must exist in this
 *        instance or in the shared resources.
 * \pre There is an shader named \a name.
 */
bear::visual::shader_program
bear::engine::level_globals::get_existing_shader
( const std::string& name ) const
{
  CLAW_PRECOND( shader_exists( name ) );

  if ( m_image_manager.has_shader_program( name ) )
    return m_image_manager.get_shader_program( name );
  else
    return m_shared_resources->get_existing_shader( name );
} // level_globals::get_existing_shader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the name of the spritepos file associated with a given image.
 * \param image_name The name of the image.
 */
std::string bear::engine::level_globals::get_spritepos_path
( const std::string& image_name ) const
{
  std::size_t pos( image_name.find_last_of('.') );

  if ( pos != std::string::npos )
    {
      const std::string candidate( image_name.substr(0, pos) + ".spritepos" );

      if ( resource_pool::get_instance().exists( candidate ) )
        return candidate;
    }

  return std::string();
} // level_globals::get_spritepos_path()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a sprite by reading its position and its size in the spritepos
 *        file associated with the image.
 * \param image_name The name of the image.
 * \param sprite_name The name of the sprite in the spritepos file.
 * \remark The spritepos file in opened in each call and read until the sprite
 *         is found.
 */
bear::visual::sprite bear::engine::level_globals::auto_sprite
( const std::string& image_name, const std::string& sprite_name )
{
  const std::string spritepos_file( get_spritepos_path(image_name) );
  visual::sprite result;

  if ( !spritepos_file.empty() )
    {
      std::stringstream f;
      resource_pool::get_instance().get_file( spritepos_file, f );

      if (f)
        {
          const spritepos s(f);
          const spritepos::const_iterator it( s.find( sprite_name ) );

          if ( it != s.end() )
            {
              const visual::image& img( get_image(image_name) );
              result = visual::sprite( img, it->get_clip() );
            }
          else
            claw::logger << claw::log_error << "can not find a valid sprite '"
                         << sprite_name << "' in the spritepos file of '"
                         << image_name << "'." << std::endl;
        }
      else
        claw::logger << claw::log_error << "can not open spritepos file for '"
                     << image_name << "'." << std::endl;
    }

  return result;
} // level_globals::auto_sprite()

/*----------------------------------------------------------------------------*/
/**
 * \brief Start to play a sound.
 * \param name The name of the sound to play.
 */
void bear::engine::level_globals::play_sound( const std::string& name )
{
  // The sounds cannot be shared between the level globals, thus we search only
  // in this instance.
  if ( !m_sound_manager.sound_exists(name) )
    {
      warn_missing_ressource( name );
      load_sound(name);
    }

  m_sound_manager.play_sound( name );
} // level_globals::play_sound()

/*----------------------------------------------------------------------------*/
/**
 * \brief Start to play the sound, with an effect.
 * \param name The name of the sound to play.
 * \param effect The effect applied to the sound.
 */
void bear::engine::level_globals::play_sound
( const std::string& name, const audio::sound_effect& effect )
{
  // The sounds cannot be shared between the level globals, thus we search only
  // in this instance.
  if ( !m_sound_manager.sound_exists(name) )
    {
      warn_missing_ressource( name );
      load_sound(name);
    }

  m_sound_manager.play_sound( name, effect );
} // level_globals::play_sound()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a new sample of a sound.
 * \param name The name of the sound to get.
 */
bear::audio::sample*
bear::engine::level_globals::new_sample( const std::string& name )
{
  // The sounds cannot be shared between the level globals, thus we search only
  // in this instance.
  if ( !m_sound_manager.sound_exists(name) )
    {
      warn_missing_ressource( name );
      load_sound(name);
    }

  return m_sound_manager.new_sample(name);
} // level_globals::new_sample()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a copy of a sample.
 * \param s The sample to copy.
 */
bear::audio::sample*
bear::engine::level_globals::new_sample( const audio::sample& s )
{
  // The sounds cannot be shared between the level globals, thus we search only
  // in this instance.
  return m_sound_manager.new_sample(s);
} // level_globals::new_sample()

/*----------------------------------------------------------------------------*/
/**
 * \brief Play a music.
 * \param name The name of the music to play.
 * \param loops How many times the music loops (zero means infinite).
 * \return The identifier of the music.
 * \remark identifiers are strictly greater than zero.
 */
std::size_t bear::engine::level_globals::play_music
( const std::string& name, unsigned int loops )
{
  // The sounds cannot be shared between the level globals, thus we search only
  // in this instance.
  if ( !m_sound_manager.sound_exists(name) )
    {
      warn_missing_ressource( name );
      load_sound(name);
    }

  return m_sound_manager.play_music(name, loops);
} // level_globals::play_music()

/*----------------------------------------------------------------------------*/
/**
 * \brief Stop the music.
 * \param id The identifier of the music.
 * \param fadeout The duration of the fadeout, if any.
 */
void bear::engine::level_globals::stop_music( std::size_t id, double fadeout )
{
  m_sound_manager.stop_music(id, fadeout);
} // level_globals::stop_music()

/*----------------------------------------------------------------------------*/
/**
 * \brief Stop all musics.
 * \param fadeout The duration of the fadeout, if any.
 */
void bear::engine::level_globals::stop_all_musics( double fadeout )
{
  m_sound_manager.stop_all_musics( fadeout );
} // level_globals::stop_all_musics()

/*----------------------------------------------------------------------------*/
/**
 * \brief Pause all music and sounds.
 */
void bear::engine::level_globals::pause_audio()
{
  m_sound_manager.pause_all();
} // level_globals::pause_audio()

/*----------------------------------------------------------------------------*/
/**
 * \brief Resume all music and sounds.
 */
void bear::engine::level_globals::resume_audio()
{
  m_sound_manager.resume_all();
} // level_globals::resume_audio()

/*----------------------------------------------------------------------------*/
/**
 * \brief Mute/unmute the sounds.
 * \param m The mute status.
 */
void bear::engine::level_globals::global_set_sound_muted( bool m )
{
  s_sound_muted = m;
} // level_globals::global_set_sound_muted()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the sound is muted.
 */
bool bear::engine::level_globals::global_get_sound_muted()
{
  return s_sound_muted;
} // level_globals::global_get_sound_muted()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the volume of the sounds.
 * \param v The volume.
 */
void bear::engine::level_globals::global_set_sound_volume( double v )
{
  s_sound_volume = v;
} // level_globals::global_set_sound_volume()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the volume of the sounds.
 */
double bear::engine::level_globals::global_get_sound_volume()
{
  return s_sound_volume;
} // level_globals::global_get_sound_volume()

/*----------------------------------------------------------------------------*/
/**
 * \brief Mute/unmute the music.
 * \param m The mute status.
 */
void bear::engine::level_globals::global_set_music_muted( bool m )
{
  s_music_muted = m;
} // level_globals::global_set_music_muted()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the music is muted.
 */
bool bear::engine::level_globals::global_get_music_muted()
{
  return s_music_muted;
} // level_globals::global_get_music_muted()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the volume of the musics.
 * \param v The volume.
 */
void bear::engine::level_globals::global_set_music_volume( double v )
{
  s_music_volume = v;
} // level_globals::global_set_music_volume()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the volume of the musics.
 */
double bear::engine::level_globals::global_get_music_volume()
{
  return s_music_volume;
} // level_globals::global_get_music_volume()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the volume of the sounds.
 * \param v The new volume.
 */
void bear::engine::level_globals::set_sound_volume( double v )
{
  global_set_sound_volume(v);

  if (!s_sound_muted)
    m_sound_manager.set_sound_volume(v);
} // level_globals::set_sound_volume()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the volume of the music.
 * \param v The new volume.
 */
void bear::engine::level_globals::set_music_volume( double v )
{
  global_set_music_volume(v);

  if (!s_music_muted)
    m_sound_manager.set_music_volume(v);
} // level_globals::set_music_volume()

/*----------------------------------------------------------------------------*/
/**
 * \brief Mute/unmute the sound and the music.
 * \param m The mute status.
 */
void bear::engine::level_globals::mute( bool m )
{
  mute_sound(m);
  mute_music(m);
} // level_globals::mute()

/*----------------------------------------------------------------------------*/
/**
 * \brief Mute/unmute the sounds.
 * \param m The mute status.
 */
void bear::engine::level_globals::mute_sound( bool m )
{
  global_set_sound_muted(m);

  if ( global_get_sound_muted() )
    m_sound_manager.set_sound_volume(0);
  else
    m_sound_manager.set_sound_volume( global_get_sound_volume() );
} // level_globals::mute_sound()

/*----------------------------------------------------------------------------*/
/**
 * \brief Mute/unmute the music.
 * \param m The mute status.
 */
void bear::engine::level_globals::mute_music( bool m )
{
  global_set_music_muted(m);

  if ( global_get_music_muted() )
    m_sound_manager.set_music_volume(0);
  else
    m_sound_manager.set_music_volume( global_get_music_volume() );
} // level_globals::mute_music()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if there is an image with a given name.
 * \param name The name of the image to check.
 */
bool bear::engine::level_globals::image_exists( const std::string& name ) const
{
  if ( m_image_manager.exists(name) )
    return true;
  else if ( m_shared_resources != NULL )
    return m_shared_resources->image_exists( name );
  else
    return false;
} // level_globals::image_exists()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if there is a sound with a given name.
 * \param name The name of the sound to check.
 */
bool bear::engine::level_globals::sound_exists( const std::string& name ) const
{
  // The sounds cannot be shared between the level globals, thus we search only
  // in this instance.
  if ( m_sound_manager.sound_exists(name) )
    return true;
  else
    return false;
} // level_globals::sound_exists()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if there is a model with a given name.
 * \param name The name of the model to check.
 */
bool bear::engine::level_globals::model_exists( const std::string& name ) const
{
  // The sounds of the model cannot be shared between the level globals, thus
  // we search the model only in this instance until we find a new solution.
  if ( m_model.find(name) != m_model.end() )
    return true;
  else
    return false;
} // level_globals::model_exists()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if there is an animation with a given name.
 * \param name The name of the animation to check.
 */
bool
bear::engine::level_globals::animation_exists( const std::string& name ) const
{
  if ( m_animation.find(name) != m_animation.end() )
    return true;
  else if ( m_shared_resources != NULL )
    return m_shared_resources->animation_exists( name );
  else
    return false;
} // level_globals::animation_exists()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if there is a font with a given name.
 * \param name The name of the font to check.
 */
bool bear::engine::level_globals::font_exists( const std::string& name ) const
{
  if ( m_font_manager.exists(name) )
    return true;
  else if ( m_shared_resources != NULL )
    return m_shared_resources->font_exists( name );
  else
    return false;
} // level_globals::font_exists()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if there is a shader with a given name.
 * \param name The name of the shader to check.
 */
bool bear::engine::level_globals::shader_exists( const std::string& name ) const
{
  if ( m_image_manager.has_shader_program(name) )
    return true;
  else if ( m_shared_resources != NULL )
    return m_shared_resources->shader_exists( name );
  else
    return false;
} // level_globals::shader_exists()

/*----------------------------------------------------------------------------*/
/**
 * \brief Register an item in the post office.
 * \param item The item to register.
 */
void bear::engine::level_globals::register_item
( communication::messageable& item )
{
  m_post_office.register_item( &item );
} // level_globals::register_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Release an item from the post office.
 * \param item The item to release.
 */
void bear::engine::level_globals::release_item
( communication::messageable& item )
{
  m_post_office.release_item( &item );
} // level_globals::release_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Send a message to an item via the post office.
 * \param name The name of the item to contact.
 * \param msg The message to send to this item.
 */
bool bear::engine::level_globals::send_message
( const std::string& target, communication::message& msg ) const
{
  return m_post_office.send_message( target, msg );
} // level_globals::send_message()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the position of the ears.
 * \param position The new position.
 */
void bear::engine::level_globals::set_ears_position
( const claw::math::coordinate_2d<double>& position )
{
  m_sound_manager.set_ears_position(position);
} // level_globals::set_ears_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the distance under which sounds are at maximum volume.
 * \param d The distance.
 */
void bear::engine::level_globals::set_full_volume_distance( double d )
{
  m_sound_manager.set_full_volume_distance( d );
} // level_globals::set_full_volume_distance()

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns the distance under which sounds are at maximum volume.
 */
double bear::engine::level_globals::get_full_volume_distance() const
{
  return m_sound_manager.get_full_volume_distance();
} // level_globals::get_full_volume_distance()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the distance from which we can't hear a sound.
 * \param d The distance.
 */
void bear::engine::level_globals::set_silence_distance( double d )
{
  m_sound_manager.set_silence_distance( d );
} // level_globals::set_silence_distance()

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns the distance from which we can't hear a sound.
 */
double bear::engine::level_globals::get_silence_distance() const
{
  return m_sound_manager.get_silence_distance();
} // level_globals::get_silence_distance()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the unit of the measures applied in the computation of the
 *        distances for the sound effects.
 * \param d The unit.
 * \remark The function has no effect if d <= 0;
 */
void bear::engine::level_globals::set_distance_unit( double d )
{
  m_sound_manager.set_distance_unit( d );
} // level_globals::set_distance_unit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns the unit of the measures applied in the computation of the
 *        distances for the sound effects.
 */
double bear::engine::level_globals::get_distance_unit() const
{
  return m_sound_manager.get_distance_unit();
} // level_globals::get_distance_unit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Reload the images.
 */
void bear::engine::level_globals::restore_images()
{
  std::vector<std::string> names;

  m_image_manager.get_image_names(names);

  // Clearing the images will delete the textures. All images must be cleared
  // before restoring any image, otherwise one of the older images may delete a
  // texture whose id is used by a restored image.
  m_image_manager.clear_images();

  if ( m_shared_resources != NULL )
    const_cast<level_globals*>( m_shared_resources )->restore_images();

  for (unsigned int i=0; i!=names.size(); ++i)
    {
      claw::logger << claw::log_verbose << "restoring image '" << names[i]
                   << "'." << std::endl;

      std::stringstream f;
      resource_pool::get_instance().get_file(names[i], f);

      if (f)
        m_image_manager.restore_image(names[i], f);
      else
        claw::logger << claw::log_error << "cannot open file '" << names[i]
                     << "'." << std::endl;
    }
} // level_globals::restore_images()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells that no more resources are supposed to be created.
 */
void bear::engine::level_globals::freeze()
{
  m_frozen = true;
} // level_globals::freeze()

/*----------------------------------------------------------------------------*/
/**
 * \brief Prints a warning telling that a resource was not preloaded.
 * \param name The name of the resource.
 */
void
bear::engine::level_globals::warn_missing_ressource( std::string name ) const
{
  if ( m_frozen )
    claw::logger << claw::log_warning
                 << "The following resource was not preloaded: '" << name
                 << "'." << std::endl;
} // level_globals::warn_missing_ressource()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do the initializations that must be done in each constructor.
 */
void bear::engine::level_globals::constructor_default()
{
  if (s_sound_muted)
    m_sound_manager.set_sound_volume(0);
  else
    m_sound_manager.set_sound_volume(s_sound_volume);

  if (s_music_muted)
    m_sound_manager.set_music_volume(0);
  else
    m_sound_manager.set_music_volume(s_music_volume);
} // level_globals::constructor_default()
