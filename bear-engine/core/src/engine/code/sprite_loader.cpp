/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::sprite_loader class.
 * \author Julien Jorge
 */
#include "engine/sprite_loader.hpp"

#include "engine/compiled_file.hpp"
#include "engine/game.hpp"
#include "engine/i18n/translator.hpp"
#include "engine/level_globals.hpp"

#include <claw/exception.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Load a sprite.
 * \param f The file from which we will read the sprite.
 * \param glob The level globals from which we take the images.
 */
bear::visual::sprite bear::engine::sprite_loader::load_sprite
( compiled_file& f, level_globals& glob )
{
  std::string image_name;
  claw::math::coordinate_2d<unsigned int> size;

  claw::math::rectangle<unsigned int> clip;
  visual::rectangle_type opacity;

  f >> image_name >> clip.position.x >> clip.position.y >> clip.width
    >> clip.height >> opacity.first_point.x >> opacity.first_point.y
    >> opacity.second_point.x >> opacity.second_point.y;

  const visual::image& resource = 
    glob.get_image
    ( engine::game::get_instance().get_translator().get( image_name ) );
  visual::sprite result( resource, clip, opacity );

  load_bitmap_rendering_attributes(f, result);

  return result;
} // sprite_loader::load_sprite()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load an animation.
 * \param f The file from which we will read the animation.
 * \param glob The level globals from which we take the images.
 */
bear::visual::animation bear::engine::sprite_loader::load_animation
( compiled_file& f, level_globals& glob )
{
  unsigned int maj(0), min(0), rel(0);

  f >> maj >> min >> rel;

  if ( (maj != 0) || (min <= 4) )
    throw claw::exception
      ( "This version of the animation file is not supported." );
  else
    return load_animation_v0_5(f, glob);
} // sprite_loader::load_animation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load any animation.
 * \param f The file from which we will read the animation.
 * \param glob The level globals from which we take the images.
 *
 * This function expects the content of f to be wether a complete animation or
 * a path to an animation file, with extra rendering attributes in the latter
 * case.
 *
 */
bear::visual::animation bear::engine::sprite_loader::load_any_animation
( compiled_file& f, level_globals& glob )
{
  std::string content_type;
  visual::animation result;

  f >> content_type;

  if ( content_type == "content_file" )
    {
      std::string path;
      f >> path;
      result = 
        glob.get_animation
        ( engine::game::get_instance().get_translator().get(path) );
      load_bitmap_rendering_attributes(f, result);
    }
  else if ( content_type == "content_animation" )
    result = load_animation_data(f, glob);
  else
    throw claw::exception
      ( "Unknown animation content type: '" + content_type + "'." );

  return result;
} // sprite_loader::load_any_animation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load an animation.
 * \param f The file from which we will read the animation.
 * \param glob The level globals from which we take the images.
 */
bear::visual::animation bear::engine::sprite_loader::load_animation_data
( compiled_file& f, level_globals& glob )
{
  return load_animation_v0_5(f, glob);
} // sprite_loader::load_animation_data()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load a bitmap_rendering_attributes.
 * \param f The file from which we will read the attributes.
 * \param result The attributes to set.
 */
void bear::engine::sprite_loader::load_bitmap_rendering_attributes
( compiled_file& f, visual::bitmap_rendering_attributes& result )
{
  claw::math::coordinate_2d<unsigned int> size;
  bool flip, mirror;
  double alpha;
  double r, g, b;
  double angle;

  f >> size.x >> size.y >> mirror >> flip >> alpha >> r >> g >> b >> angle;

  result.flip( flip );
  result.mirror( mirror );
  result.set_opacity( alpha );
  result.set_size( size );
  result.set_intensity( r, g, b );
  result.set_angle(angle);
} // sprite_loader::load_bitmap_rendering_attributes()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load an animation.
 * \param f The file from which we will read the animation.
 * \param glob The level globals from which we take the images.
 */
bear::visual::animation bear::engine::sprite_loader::load_animation_v0_5
( compiled_file& f, level_globals& glob )
{
  unsigned int frames_count;

  f >> frames_count;

  std::vector<visual::sprite> frames(frames_count);
  std::vector<double> duration(frames_count);

  for ( unsigned int i=0; i!=frames_count; ++i)
    {
      f >> duration[i];
      frames[i] = load_sprite(f, glob);
    }

  unsigned int loops, first_index, last_index;
  bool loop_back;

  f >> loops >> loop_back >> first_index >> last_index;

  visual::animation result( frames, duration );

  load_bitmap_rendering_attributes(f, result);

  result.set_loops( loops );
  result.set_loop_back( loop_back );
  result.set_first_index(first_index);
  result.set_last_index(last_index);

  return result;
} // sprite_loader::load_animation_v0_5()
