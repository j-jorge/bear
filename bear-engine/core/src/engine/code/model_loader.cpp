/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::model_loader class.
 * \author Julien Jorge
 */
#include "engine/model_loader.hpp"

#include "engine/level_globals.hpp"
#include "engine/sprite_loader.hpp"
#include "engine/model/model_action.hpp"
#include "engine/model/model_snapshot.hpp"
#include "universe/types.hpp"

#include "easing.hpp"

#include <claw/assert.hpp>
#include <claw/exception.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param f The file from which we load the model.
 * \param glob The level_globals in which we load the resources.
 *
 * \a f must live longer than this instance of model_loader.
 */
bear::engine::model_loader::model_loader
( std::istream& f, level_globals& glob )
  : m_file(f, true), m_level_globals(glob)
{

} // model_loader::model_loader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Build the model
 */
bear::engine::model_actor* bear::engine::model_loader::run()
{
  unsigned int maj(0), min(0), rel(0);

  m_file >> maj >> min >> rel;

  if ( !m_file )
    throw claw::exception("Can't read the version of the model file.");
  else if ( (maj != 0) || (min < 6) )
    throw claw::exception("This version of the model file is not supported.");
  else
    {
      anim_map_type anim_map;
      load_animations( anim_map );

      model_actor* result = new model_actor;
      load_actions(*result, anim_map);
      return result;
    }
} // model_loader::run()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load all the actions of a model.
 * \param m The model.
 * \param anim_map All the animations of the model.
 */
void bear::engine::model_loader::load_actions
( model_actor& m, const anim_map_type& anim_map )
{
  std::size_t count;

  if ( m_file >> count )
    {
      for ( std::size_t i=0; i!=count; ++i )
        load_action(m, anim_map);
    }
  else
    claw::logger << claw::log_error << "No action found in the model."
                 << std::endl;
} // model_loader::load_actions()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load an action of a model.
 * \param m The model.
 * \param anim_map All the animations of the model.
 */
void bear::engine::model_loader::load_action
( model_actor& m, const anim_map_type& anim_map )
{
  std::string name, autonext;
  universe::time_type duration;

  if ( m_file >> name >> duration >> autonext )
    {
      std::vector<std::string> sounds;
      bool glob;
      load_sound( sounds, glob );

      std::string sound_name;
      if ( !sounds.empty() )
        sound_name = sounds[0];

      std::size_t n;
      m_file >> n;

      model_action a( n, duration, autonext, sound_name, glob );

      load_marks(a, anim_map);
      load_snapshots(a);
      m.add_action( name, a );
    }
  else
    claw::logger << claw::log_error << "Invalid action." << std::endl;
} // model_loader::load_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Loads a set of sounds with their properties.
 * \param sounds (out) The sounds.
 * \param glob (out) Tells if the the sounds are played globally.
 */
void bear::engine::model_loader::load_sound
( std::vector<std::string>& sounds, bool& glob )
{
  std::size_t count;

  if ( m_file >> glob >> count )
    {
      sounds.resize( count );

      for ( std::size_t i=0; i!=count; ++i )
        {
          m_file >> sounds[i];
          m_level_globals.load_sound( sounds[i] );
        }
    }
} // model_loader::load_sound()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load the marks of an action.
 * \param a The action.
 * \param anim_map All the animations of the model.
 */
void bear::engine::model_loader::load_marks
( model_action& a, const anim_map_type& anim_map )
{
  for ( std::size_t i=0; i!=a.get_marks_count(); ++i )
    {
      std::string label;
      bool apply_angle;
      bool pause_hidden;
      std::size_t anim_index;

      if ( m_file >> label >> apply_angle >> pause_hidden >> anim_index )
        {
          model_animation anim;

          if ( anim_index < anim_map.size() )
            anim = anim_map[anim_index];

          a.get_mark(i) = model_mark( label, anim, apply_angle, pause_hidden );
        }
      else
        claw::logger << claw::log_error << "The mark is incomplete."
                     << std::endl;
    }
} // model_loader::load_marks()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load the snpashots of an action.
 * \param a The action.
 */
void bear::engine::model_loader::load_snapshots( model_action& a )
{
  std::size_t n;
  m_file >> n;

  for ( ; n!=0; --n )
    load_snapshot(a);
} // model_loader::load_snapshots()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load a snapshot of an action.
 * \param a The action.
 */
void bear::engine::model_loader::load_snapshot( model_action& a )
{
  universe::time_type date;
  std::string func;

  std::string x_align, y_align;
  universe::coordinate_type x_align_value, y_align_value;
  universe::size_type width, height;

  if ( m_file >> date >> func >> width >> height >> x_align
       >> y_align >> x_align_value >> y_align_value )
    {
      bool glob;
      std::vector<std::string> sounds;
      load_sound(sounds, glob);

      model_snapshot s(date, a.get_marks_count(), func, sounds, glob);
      s.set_size(width, height);
      s.set_x_alignment
        (model_snapshot::horizontal_alignment::from_string(x_align));
      s.set_y_alignment
        (model_snapshot::vertical_alignment::from_string(y_align));
      s.set_x_alignment_value(x_align_value);
      s.set_y_alignment_value(y_align_value);

      load_mark_placements(s);
      a.add_snapshot(s);
    }
  else
    claw::logger << claw::log_error << "The snapshot is incomplete."
                 << std::endl;
} // model_loader::load_snapshot()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load the mark placements of a snapshot.
 * \param s The snapshot.
 */
void bear::engine::model_loader::load_mark_placements( model_snapshot& s )
{
  for ( std::size_t i=0; i!=s.get_mark_placements_count(); ++i )
    load_mark_placement(s);
} // model_loader::load_mark_placements()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load a mark placements of a snapshot.
 * \param s The snapshot.
 */
void bear::engine::model_loader::load_mark_placement( model_snapshot& s )
{
  unsigned int id;
  universe::coordinate_type x, y, w, h;
  int z;
  double angle;
  bool visible;
  std::string collision_function;

  bear::easing::function::function_type
    x_func, y_func, width_func, height_func, angle_func;
  bear::easing::direction::direction_type
    x_dir, y_dir, width_dir, height_dir, angle_dir;

  if ( m_file >> id >> x >> x_func >> x_dir
       >> y >> y_func >> y_dir
       >> w >> width_func >> width_dir
       >> h >> height_func >> height_dir
       >> z
       >> angle >> angle_func >> angle_dir
       >> visible >> collision_function )
    {
      model_mark_placement m
        (id, x, y, w, h, z, angle, visible, collision_function);

      m.set_x_position_easing
        ( easing(x_func, x_dir).to_claw_easing_function() );
      m.set_y_position_easing
        ( easing(y_func, y_dir).to_claw_easing_function() );
      m.set_width_easing
        ( easing(width_func, width_dir).to_claw_easing_function() );
      m.set_height_easing
        ( easing(height_func, height_dir).to_claw_easing_function() );
      m.set_angle_easing
        ( easing(angle_func, angle_dir).to_claw_easing_function() );

      s.set_mark_placement(m);
    }
  else
    claw::logger << claw::log_error << "The mark_placement is incomplete."
                 << std::endl;
} // model_loader::load_mark_placement()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load all the animations used in the model.
 * \param anim_map All the animations of the model.
 */
void bear::engine::model_loader::load_animations( anim_map_type& anim_map )
{
  std::size_t n;
  m_file >> n;

  anim_map.resize(n);

  for (std::size_t i=0; i!=n; ++i)
    {
      visual::animation val =
        sprite_loader::load_any_animation(m_file, m_level_globals);

      anim_map[i] = new visual::animation(val);
    }
} // model_loader::load_animation()
