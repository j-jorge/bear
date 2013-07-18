/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::model class.
 * \author Julien Jorge
 */
#include "engine/level_globals.hpp"

#include "engine/base_item.hpp"
#include "engine/layer/layer.hpp"
#include "engine/model/model_mark_item.hpp"
#include "engine/model/model_snapshot.hpp"
#include "engine/reference_point/model_mark_reference_point.hpp"

#include "universe/forced_movement/forced_tracking.hpp"

#include <boost/bind.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
template<class Base>
bear::engine::model<Base>::model()
  : m_action(NULL), m_sample(NULL), m_current_snapshot(NULL)
{

} // model::model()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 * \param that The instance to copy from.
 */
template<class Base>
bear::engine::model<Base>::model( const model<Base>& that )
  : super(that), m_actor(that.m_actor), m_action(NULL), m_sample(NULL),
    m_current_snapshot(NULL)
{

} // model::model()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor
 */
template<class Base>
bear::engine::model<Base>::~model()
{
  clear();
} // model::~model()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assignment operator.
 * \param that The instance to copy from.
 */
template<class Base>
bear::engine::model<Base>&
bear::engine::model<Base>::operator=( const model<Base>& that )
{
  model<Base> tmp(that);
  std::swap(tmp, *this);
  return *this;
} // model::operator=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do the progression of the model.
 * \param elapsed_time Elapsed time since the last call.
 */
template<class Base>
void bear::engine::model<Base>::progress( universe::time_type elapsed_time )
{
  super::progress( elapsed_time );
  progress_model(elapsed_time);
} // model::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the sprites representing the item.
 * \param visuals (out) The sprites of the item, and their positions.
 */
template<class Base>
void
bear::engine::model<Base>::get_visual( std::list<scene_visual>& visuals ) const
{
  if ( m_action!=NULL )
    if ( m_snapshot != m_action->snapshot_end() )
      {
        model_snapshot::const_mark_placement_iterator it;

        for ( it=m_snapshot->mark_placement_begin();
              it!=m_snapshot->mark_placement_end(); ++it )
          {
            const model_mark& mark( m_action->get_mark( it->get_mark_id() ) );

            if ( mark.has_animation() && it->is_visible() )
              visuals.push_front( get_mark_visual(mark, *it) );
          }
      }
} // model::get_visual()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the model actor.
 * \param actor The model_actor to copy from.
 */
template<class Base>
void bear::engine::model<Base>::set_model_actor( const model_actor& actor )
{
  clear();
  m_actor = actor;
} // model::set_model_actor()

/*----------------------------------------------------------------------------*/
/**
 * \brief Start an action.
 * \param name The name of the action to start.
 */
template<class Base>
void bear::engine::model<Base>::start_model_action( const std::string& name )
{
  model_action* a = m_actor.get_action( name );

  if ( a != NULL )
    {
      if ( a == m_action )
        reset_action(0);
      else
        {
          stop_action();
          m_action = a;
          m_action_name = name;
          start_action(0);
        }

      execute_snapshot();
    }
} // model::start_model_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Switch to an other action.
 * \param name The name of the action to start.
 * \remark The difference with start_model_action is that the date is not reset
 *         to zero. The new action begins at the date of the current action.
 */
template<class Base>
void
bear::engine::model<Base>::switch_to_model_action( const std::string& name )
{
  model_action* a = m_actor.get_action( name );

  if ( a != NULL )
    if ( a != m_action )
      {
        const universe::time_type d(m_date);
        stop_action();
        m_action = a;
        m_action_name = name;
        start_action(d);
      }
} // model::switch_to_model_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the name of the current action.
 */
template<class Base>
const std::string& bear::engine::model<Base>::get_current_action_name() const
{
  return m_action_name;
} // model::get_current_action_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an action from the model actor.
 * \param name The name of the action to get.
 */
template<class Base>
bear::engine::model_action const*
bear::engine::model<Base>::get_action( const std::string& name ) const
{
  return m_actor.get_action(name);
} // model::get_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the current action from the model actor.
 */
template<class Base>
bear::engine::model_action const*
bear::engine::model<Base>::get_current_action() const
{
  return m_action;
} // model::get_current_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the current action from the model actor.
 */
template<class Base>
const bear::engine::model_action::const_snapshot_iterator
bear::engine::model<Base>::get_current_snapshot() const
{
  CLAW_PRECOND( m_snapshot != model_action::const_snapshot_iterator() );

  return m_snapshot;
} // model::get_current_snapshot()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the position of a mark relatively to the world.
 * \param i The identifier of the mark.
 */
template<class Base>
bear::universe::position_type
bear::engine::model<Base>::get_mark_world_position( std::size_t i ) const
{
  CLAW_PRECOND( m_snapshot != model_action::const_snapshot_iterator() );
  CLAW_PRECOND( i < m_snapshot->get_mark_placements_count() );

  model_mark_placement m;
  m.set_mark_id(i);
  get_mark_placement(m);
  return m.get_position();
} // model::get_mark_world_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the position of a mark relatively to the world.
 * \param mark_name The name of the mark.
 */
template<class Base>
bear::universe::position_type
bear::engine::model<Base>::get_mark_world_position
( const std::string& mark_name ) const
{
  CLAW_PRECOND( m_snapshot != model_action::const_snapshot_iterator() );
  CLAW_PRECOND( m_action != NULL );

  return get_mark_world_position( m_action->get_mark_id(mark_name) );
} // model::get_mark_world_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the model mark item placed on a given mark in the active action.
 * \param name The name of the mark.
 */
template<class Base>
bear::engine::model_mark_item*
bear::engine::model<Base>::get_model_mark_item( const std::string& name ) const
{
  if ( m_action != NULL )
    {
      const std::size_t id = m_action->get_mark_id(name);

      if ( id != model_action::not_an_id )
        return &(m_action->get_mark(id).get_box_item());
    }

  return NULL;
} // model::get_model_mark_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the data associated to a mark placement, relatively to the world.
 * \param mark_name The name of the mark.
 * \param m (out) The mark placement.
 */
template<class Base>
bool bear::engine::model<Base>::get_mark_placement
( const std::string& mark_name, model_mark_placement& m ) const
{
  if ( m_action != NULL )
    return get_mark_placement( get_mark_id(mark_name), m );
  else
    return false;
} // model::get_mark_placement()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the data associated to a mark placement in the current state of
 *        the model and relatively to the model.
 * \param mark_name The name of the mark.
 * \param m (out) The mark placement.
 */
template<class Base>
bool bear::engine::model<Base>::get_current_local_mark_placement
( const std::string& mark_name, model_mark_placement& m ) const
{
  if ( m_action != NULL )
    return get_current_local_mark_placement( get_mark_id(mark_name), m );
  else
    return false;
} // model::get_current_local_mark_placement()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the visibility of a mark in the current action.
 * \param name The name of the mark.
 * \param b The visibility of the mark.
 */
template<class Base>
void bear::engine::model<Base>::set_mark_visibility_in_action
( const std::string& name, bool b )
{
  if ( m_action == NULL )
    return;

  const std::size_t id = m_action->get_mark_id(name);

  if ( id == model_action::not_an_id )
    return;

  typename model_action::snapshot_iterator its;
  for (its=m_action->snapshot_begin(); its!=m_action->snapshot_end(); ++its)
    {
      model_mark_placement m = its->get_mark_placement(id);
      m.set_visibility(b);
      its->set_mark_placement(m);
    }

  m_current_snapshot->get_mark_placement(id).set_visibility(b);
} // model::set_mark_visibility_in_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do the progression of the model.
 * \param elapsed_time Elapsed time since the last call.
 */
template<class Base>
void
bear::engine::model<Base>::progress_model( universe::time_type elapsed_time )
{
  if ( m_action==NULL )
    return;

  if ( m_snapshot==m_action->snapshot_end() )
    return;

  const universe::time_type initial_time(m_date);
  m_date += elapsed_time;
  model_action::const_snapshot_iterator
    eit( m_action->get_snapshot_at(m_date) );

  if (eit==m_snapshot)
    {
      progress_animations(initial_time, elapsed_time);
      update_sound_position();
      m_current_snapshot->update(elapsed_time);
      update_bounding_box();
      update_mark_items_positions();
    }
  else
    progress_to_snapshot(initial_time, elapsed_time, eit);
} // model::progress_model()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a substitute for the animation of all marks with a given name.
 * \param mark_name The name of the mark.
 * \param anim The new animation on the mark.
 */
template<class Base>
void bear::engine::model<Base>::set_global_substitute
( const std::string& mark_name, const model_animation& anim )
{
  m_actor.set_global_substitute(mark_name, anim);
} // model::set_global_substitute()

/*----------------------------------------------------------------------------*/
/**
 * \brief Restore the default animation of all marks with a given name.
 * \param mark_name The name of the mark.
 */
template<class Base>
void bear::engine::model<Base>::remove_global_substitute
( const std::string& mark_name )
{
  m_actor.remove_global_substitute(mark_name);
} // model::remove_global_substitute()

/*----------------------------------------------------------------------------*/
/**
 * \brief Force the angle of a mark in all the snapshots of the current action.
 * \param mark_name The name of the mark.
 * \param angle The angle to apply to the mark.
 */
template<class Base>
void bear::engine::model<Base>::set_mark_angle_in_action
( const std::string& mark_name, double angle )
{
  if ( m_action == NULL )
    return;

  const std::size_t id = m_action->get_mark_id(mark_name);

  if ( id == model_action::not_an_id )
    return;

  for ( model_action::snapshot_iterator it=m_action->snapshot_begin();
        it!=m_action->snapshot_end(); ++it )
    {
      model_mark_placement m=it->get_mark_placement(id);
      m.set_angle(angle);
      it->set_mark_placement(m);
    }

  m_current_snapshot->get_mark_placement(id).set_angle( angle );
} // model::set_mark_angle_in_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Force the position of a mark in all the snapshots of the current
 *        action.
 * \param mark_name The name of the mark.
 * \param position The position to apply to the mark.
 */
template<class Base>
void bear::engine::model<Base>::set_mark_position_in_action
( const std::string& mark_name, universe::position_type position )
{
  if ( m_action == NULL )
    return;

  std::size_t id = m_action->get_mark_id(mark_name);

  if ( id == model_action::not_an_id )
    return;

  for ( model_action::snapshot_iterator it=m_action->snapshot_begin();
        it!=m_action->snapshot_end(); ++it )
    {
      model_mark_placement m=it->get_mark_placement(id);
      m.set_position(position);
      it->set_mark_placement(m);
    }

  m_current_snapshot->get_mark_placement(id).set_position( position );
} // model::set_mark_position_in_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the identifier of a mark, given its name.
 * \param mark_name The name of the mark.
 */
template<class Base>
std::size_t bear::engine::model<Base>::get_mark_id
( const std::string& mark_name ) const
{
  if ( m_action != NULL )
    return m_action->get_mark_id(mark_name);
  else
    return model_action::not_an_id;
} // model::get_mark_id()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the identifier of a mark.
 * \param mark The mark for which we want the identifier.
 */
template<class Base>
std::size_t
bear::engine::model<Base>::get_mark_id( const model_mark& mark ) const
{
  return get_mark_id(mark.get_label());
} // model::get_mark_id()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the data associated to a mark placement, relatively to the world.
 * \param i The id of the mark.
 * \param m (out) The mark placement.
 */
template<class Base>
bool bear::engine::model<Base>::get_mark_placement
( std::size_t i, model_mark_placement& m ) const
{
  bool result = false;
  if ( m_action != NULL )
    {
      if ( i < m_action->get_marks_count() )
        {
          m.set_mark_id( i );
          result = get_mark_placement(m);
        }
    }

  return result;
} // model::get_mark_placement()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the data associated to a mark placement, relatively to the world.
 * \param m (in/out) The mark placement.
 *
 * \remark You must set the mark identifier of \a m before calling.
 */
template<class Base>
bool
bear::engine::model<Base>::get_mark_placement( model_mark_placement& m ) const
{
  bool result(false);

  if ( m_snapshot != m_action->snapshot_end() )
    {
      result = get_oriented_mark_placement(m);

      m.set_position( m.get_position() + this->get_bottom_left() );
      m.set_angle( m.get_angle() + this->get_visual_angle() );
      m.set_depth_position( m.get_depth_position() + this->get_z_position() );
    }

  return result;
} // model::get_mark_placement()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the data associated to a mark placement in the current state of
 *        the model and relatively to the model.
 * \param id The id of the mark.
 * \param m (out) The mark placement.
 */
template<class Base>
bool bear::engine::model<Base>::get_current_local_mark_placement
( std::size_t id, model_mark_placement& m ) const
{
  bool result(false);

  if ( ( m_action != NULL ) && ( id < m_action->get_marks_count() )
       && ( m_current_snapshot != NULL ) )
    {
      m = m_current_snapshot->get_mark_placement( id );
      result = true;
    }

  return result;
} // model::get_current_local_mark_placement()

/*----------------------------------------------------------------------------*/
/**
 * \brief Give a string representation of the item.
 * \param str (out) The result of the convertion.
 */
template<class Base>
void bear::engine::model<Base>::to_string( std::string& str ) const
{
  super::to_string(str);

  std::string action("\naction: ");
  str += action + m_action_name;
} // model::to_string()

/*----------------------------------------------------------------------------*/
/**
 * \brief Removes the items of the mark from the layer when this item leaves its
 *        layer.
 */
template<class Base>
void bear::engine::model<Base>::on_leaves_layer()
{
  super::on_leaves_layer();

  remove_all_mark_items_from_layer();
} // model::on_leaves_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do the progression up to a given snapshot.
 * \param initial_type The value of m_date before the call to progress.
 * \param elapsed_time Elapsed time since the last call.
 * \param eit The snapshot to reach.
 */
template<class Base>
void bear::engine::model<Base>::progress_to_snapshot
( universe::time_type initial_time, universe::time_type elapsed_time,
  const model_action::const_snapshot_iterator& eit )
{
  model_action::const_snapshot_iterator it(m_snapshot);
  for ( ++it; it != eit; ++it )
    {
      m_snapshot = it;
      execute_snapshot();
    }

  if ( eit != m_action->snapshot_end() )
    {
      m_snapshot = eit;
      create_tweeners();
      execute_snapshot();
      progress_animations(initial_time, elapsed_time);
      update_sound_position();
    }
  else
    {
      const std::string action_name( m_action->get_next_action() );
      const universe::time_type t( m_date - m_action->get_duration() );

      if ( action_name.empty() )
        {
          progress_animations(initial_time, elapsed_time);
          update_sound_position();
          update_mark_items_positions();
        }
      else
        {
          start_model_action( m_action->get_next_action() );
          progress_model( t );
        }
    }
} // model::progress_to_snapshot()

/*----------------------------------------------------------------------------*/
/**
 * \brief Execute a function associated with a snapshot.
 * \param name The name of the function.
 */
template<class Base>
void bear::engine::model<Base>::execute_function( const std::string& name )
{
  if ( !name.empty() )
    this->execute(name);
} // model::execute_function()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the sprite displayed on a given mark.
 * \param mark The mark for which we want the sprite.
 * \param p The placement of this mark.
 */
template<class Base>bear::engine::scene_visual
bear::engine::model<Base>::get_mark_visual
( const model_mark& mark, const model_mark_placement& p ) const
{
  CLAW_PRECOND( m_action!=NULL );
  CLAW_PRECOND( m_snapshot!=m_action->snapshot_end() );
  CLAW_PRECOND( mark.has_animation() && p.is_visible() );

  scene_visual v = this->get_scene_visual( *mark.get_animation() );
  model_mark_placement m(p);

  get_oriented_mark_placement(m);

  if ( mark.apply_angle_to_animation() )
    {
      double a
        ( v.scene_element.get_rendering_attributes().get_angle()
          + m.get_angle() );

      if ( this->get_rendering_attributes().is_mirrored() )
        a -= 3.14159;

      v.scene_element.get_rendering_attributes().set_angle(a);
    }

  v.z_position += m.get_depth_position();
  
  const visual::rectangle_type bounding_box
    ( v.scene_element.get_bounding_box() );

  v.scene_element.set_position
    ( m.get_position() + this->get_bottom_left()
      - bounding_box.size() / 2
      + (v.scene_element.get_position() - bounding_box.bottom_left() ));

  if ( this->get_rendering_attributes().is_mirrored() )
    v.scene_element.set_position
      ( v.scene_element.get_position().x - this->get_gap().x,
        v.scene_element.get_position().y );
  else
    v.scene_element.set_position
      ( v.scene_element.get_position().x + this->get_gap().x,
        v.scene_element.get_position().y );

  if ( this->get_rendering_attributes().is_flipped() )
    v.scene_element.set_position
      ( v.scene_element.get_position().x,
        v.scene_element.get_position().y - this->get_gap().y );
  else
    v.scene_element.set_position
      ( v.scene_element.get_position().x,
        v.scene_element.get_position().y + this->get_gap().y );

  return v;
} // model::get_mark_visual()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the data associated to a mark placement, relatively to the
 *        orientation of the model.
 * \param m (in/out) The mark placement.
 *
 * \remark You must set the mark identifier of \a m before calling.
 */
template<class Base>
bool
bear::engine::model<Base>::get_oriented_mark_placement
( model_mark_placement& m ) const
{
  bool result(false);

  if ( m_snapshot != m_action->snapshot_end() )
    if ( m.get_mark_id() < m_action->get_marks_count() )
      {
        result = true;
        m = m_current_snapshot->get_mark_placement( m.get_mark_id() );

        const double rot( this->get_visual_angle() );

        universe::coordinate_type dx(m.get_position().x);
        universe::coordinate_type dy(m.get_position().y);
        double a( m.get_angle() );

        if ( this->get_rendering_attributes().is_mirrored() )
          {
            dx = this->get_width() - dx;
            a = 3.14159265 - a;
          }

        if ( this->get_rendering_attributes().is_flipped() )
          {
            dy = this->get_height() - dy;
            a = -a;
          }

        dx -= this->get_width() / 2;
        dy -=  this->get_height() / 2;
        const universe::coordinate_type save_dx(dx);

        dx = dx * std::cos(rot) - dy * std::sin(rot) + this->get_width() / 2;
        dy = save_dx * std::sin(rot) + dy * std::cos(rot)
          + this->get_height() / 2;

        m.set_position( dx, dy );
        m.set_angle(a);
      }

  return result;
} // model::get_oriented_mark_placement()

/*----------------------------------------------------------------------------*/
/**
 * \brief Start the current action.
 * \param d The date at which the action is reset (new start).
 */
template<class Base>
void bear::engine::model<Base>::start_action( universe::time_type d )
{
  CLAW_PRECOND(m_action != NULL);

  delete m_sample;
  m_sample = NULL;

  const std::string s( m_action->get_sound_name() );

  if ( this->get_level_globals().sound_exists(s) )
    {
      m_sample = this->get_level_globals().new_sample(s);
      audio::sound_effect effect;
      if ( !m_action->sound_is_global() )
        effect.set_position( this->get_center_of_mass() );
      effect.set_loops(0);
      m_sample->play( effect );
    }

  reset_action(d);
} // model::start_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Stop the current action.
 */
template<class Base>
void bear::engine::model<Base>::stop_action()
{
  if ( m_action == NULL )
    return;

  if ( m_sample != NULL )
    {
      m_sample->stop();
      delete m_sample;
      m_sample = NULL;
    }

  remove_all_mark_items_from_layer();
  m_action = NULL;
  m_snapshot = model_action::snapshot_iterator();

  delete m_current_snapshot;
  m_current_snapshot = NULL;
} // model::stop_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Reset the current action.
 * \param d The date at which the action is reset (new start).
 */
template<class Base>
void bear::engine::model<Base>::reset_action( universe::time_type d )
{
  CLAW_PRECOND(m_action != NULL);

  universe::time_type remaining_time(0);

  if ( d <= m_action->get_duration() )
    m_date = d;
  else
    {
      remaining_time = d - m_action->get_duration();
      m_date = m_action->get_duration();
    }

  m_snapshot = m_action->get_snapshot_at(m_date);
  create_tweeners();

  model_action::mark_iterator it;

  for ( it=m_action->mark_begin(); it!=m_action->mark_end(); ++it )
    if ( it->has_animation() )
      if ( it->get_animation()->is_finite()
           || (it->get_animation()->get_first_index() != 0) )
        it->get_animation()->reset();

  progress_animations(0, m_date, true);

  if ( remaining_time != 0 )
    {
      const std::string action_name( m_action->get_next_action() );

      if ( action_name.empty() )
        {
          progress_animations(m_date, remaining_time);
          m_date = d;
          update_sound_position();
        }
      else
        {
          m_date = remaining_time;
          switch_to_model_action(action_name);
        }
    }
} // model::reset_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update the position of the sound emitted by the model.
 */
template<class Base>
void bear::engine::model<Base>::update_sound_position()
{
  if ( (m_sample != NULL) && (m_action!=NULL) )
    if ( !m_action->sound_is_global() )
      {
        audio::sound_effect effect( m_sample->get_effect() );
        effect.set_position( this->get_center_of_mass() );
        m_sample->set_effect( effect );
      }
} // model::update_sound_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do the progression of the animations of the marks.
 * \param initial_time The date at which the animations where stopped.
 * \param elapsed_time Elapsed time since \a initial_time.
 */
template<class Base>
void bear::engine::model<Base>::progress_animations
( universe::time_type initial_time, universe::time_type elapsed_time,
  bool only_finite )
{
  // We must pay attention not to call next() several times on the same
  // animation, in the case where an animation is shared by several marks.
  // Also, for the animations paused when the mark is hidden, we must save the
  // time during which the animation effectively evolves.
  typedef std::map<model_animation, universe::time_type> anim_map_type;
  anim_map_type anim;
  model_action::mark_iterator it;

  for ( it=m_action->mark_begin(); it!=m_action->mark_end(); ++it )
    if ( it->has_animation() )
      if ( !only_finite || it->get_animation()->is_finite() )
        {
          const model_animation a=it->get_animation();
          const anim_map_type::iterator ita=anim.find(a);
          const universe::time_type d
            ( m_action->accumulated_mark_visibility
              ( *it, initial_time, initial_time + elapsed_time ) );

          if ( ita == anim.end() )
            anim[a] = d;
          else if (ita->second < d )
            ita->second = d;
        }

  for ( ; !anim.empty(); anim.erase(anim.begin()) )
    anim.begin()->first->next(anim.begin()->second);
} // model::progress_animations()

/*----------------------------------------------------------------------------*/
/**
 * \brief Execute the current action.
 */
template<class Base>
void bear::engine::model<Base>::execute_snapshot()
{
  std::string s( m_snapshot->get_random_sound_name() );

  if ( this->get_level_globals().sound_exists(s) )
    {
      audio::sound_effect effect;

      if ( !m_snapshot->sound_is_global() )
        effect.set_position( this->get_center_of_mass() );

      effect.set_loops(1);
      this->get_level_globals().play_sound( s, effect );
    }

  update_bounding_box();
  update_mark_items();
  execute_function( m_snapshot->get_function() );
} // model::execute_snapshot()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the bounding box of the item according to the new action.
 */
template<class Base>
void bear::engine::model<Base>::update_mark_items()
{
  CLAW_PRECOND( m_snapshot != model_action::snapshot_iterator() );

  model_snapshot_tweener::const_mark_placement_iterator it;
  for ( it=m_current_snapshot->mark_placement_begin();
        it!=m_current_snapshot->mark_placement_end(); ++it )
    if ( !it->is_visible() || it->get_collision_function().empty()
         || (it->get_size().x == 0) || (it->get_size().y == 0) )
      m_action->get_mark( it->get_mark_id() ).drop_box_item_from_layer();
    else
      {
        add_mark_item_in_layer( it->get_mark_id() );

        model_mark_item& item =
          m_action->get_mark(it->get_mark_id()).get_box_item();

        item.set_collision_function( it->get_collision_function() );
      }

  update_mark_items_positions();
} // model::update_mark_items()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the position and the size of the mark items.
 */
template<class Base>
void bear::engine::model<Base>::update_mark_items_positions()
{
  CLAW_PRECOND( m_snapshot != model_action::snapshot_iterator() );

  for ( std::size_t i=0; i!=m_snapshot->get_mark_placements_count(); ++i )
    {
      model_mark_placement m;
      get_mark_placement(i, m);

      model_mark_item& item = m_action->get_mark(i).get_box_item();

      item.set_size( m.get_size() );
      item.set_center_of_mass( m.get_position() );
      item.set_z_position( m.get_depth_position() );
    }
} // model::update_mark_item_positions()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the bounding box of the item according to the new action.
 */
template<class Base>
void bear::engine::model<Base>::update_bounding_box()
{
  update_bounding_box_width();
  update_bounding_box_height();
} // model::update_bounding_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the width bounding box of the item according to the new action.
 */
template<class Base>
void bear::engine::model<Base>::update_bounding_box_width()
{
  universe::coordinate_type delta( m_snapshot->get_x_alignment_value() );
  universe::coordinate_type pos;
  model_snapshot::horizontal_alignment::value a
    ( m_snapshot->get_x_alignment() );

  if ( this->get_rendering_attributes().is_mirrored() )
    {
      if ( a == model_snapshot::horizontal_alignment::align_left )
        a = model_snapshot::horizontal_alignment::align_right;
      else if ( a == model_snapshot::horizontal_alignment::align_right )
        a = model_snapshot::horizontal_alignment::align_left;

      delta = -delta;
    }

  switch ( a )
    {
    case model_snapshot::horizontal_alignment::align_left:
      pos = this->get_left();
      this->set_width( m_snapshot->get_width() );
      this->set_left( pos + delta );
      break;
    case model_snapshot::horizontal_alignment::align_right:
      pos = this->get_right();
      this->set_width( m_snapshot->get_width() );
      this->set_right( pos + delta );
      break;
    case model_snapshot::horizontal_alignment::align_center:
      pos = this->get_horizontal_middle();
      this->set_width( m_snapshot->get_width() );
      this->set_horizontal_middle( pos + delta );
    }
} // model::update_bounding_box_width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the height bounding box of the item according to the new action.
 */
template<class Base>
void bear::engine::model<Base>::update_bounding_box_height()
{
  universe::coordinate_type delta( m_snapshot->get_y_alignment_value() );
  universe::coordinate_type pos;
  model_snapshot::vertical_alignment::value a( m_snapshot->get_y_alignment() );

  if ( this->get_rendering_attributes().is_flipped() )
    {
      if ( a == model_snapshot::vertical_alignment::align_bottom )
        a = model_snapshot::vertical_alignment::align_top;
      else if ( a == model_snapshot::vertical_alignment::align_top )
        a = model_snapshot::vertical_alignment::align_bottom;

      delta = -delta;
    }

  switch ( a )
    {
    case model_snapshot::vertical_alignment::align_bottom:
      pos = this->get_bottom();
      this->set_height( m_snapshot->get_height() );
      this->set_bottom( pos + delta );
      break;
    case model_snapshot::vertical_alignment::align_top:
      pos = this->get_top();
      this->set_height( m_snapshot->get_height() );
      this->set_top( pos + delta );
      break;
    case model_snapshot::vertical_alignment::align_center:
      pos = this->get_vertical_middle();
      this->set_height( m_snapshot->get_height() );
      this->set_vertical_middle( pos + delta );
    }
} // model::update_bounding_box_height()

/*----------------------------------------------------------------------------*/
/**
 * \brief Clear the model. Delete the functions, the sample.
 */
template<class Base>
void bear::engine::model<Base>::clear()
{
  delete m_sample;
  m_sample = NULL;

  remove_all_mark_items_from_layer();

  m_action = NULL;
  m_action_name.clear();
  m_snapshot = model_action::const_snapshot_iterator();

  delete m_current_snapshot;
  m_current_snapshot = NULL;
} // model::clear()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove the items associated with the mark from the layer where the
 *        model is.
 */
template<class Base>
void bear::engine::model<Base>::remove_all_mark_items_from_layer() const
{
  if ( m_action == NULL )
    return;

  for ( std::size_t i=0; i!=m_action->get_marks_count(); ++i )
    m_action->get_mark(i).drop_box_item_from_layer();
} // model::remove_all_mark_items_from_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add an item associated with a mark in the same layer than the model.
 * \param i The index of the mark whose item is added in the layer.
 */
template<class Base>
void bear::engine::model<Base>::add_mark_item_in_layer( std::size_t i )
{
  model_mark& mark( m_action->get_mark(i) );

  mark.add_box_item_in_layer( this->get_layer() );
  mark.get_box_item().set_model_item( *this );

  universe::forced_tracking mvt;
  mvt.set_distance( universe::position_type(0, 0) );
  mvt.set_reference_point
    ( model_mark_reference_point< model<Base> >( *this, mark.get_label() ) );

  mark.get_box_item().set_forced_movement( mvt );
} // model::add_mark_item_in_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the tweeners for the current snapshot.
 */
template<class Base>
void bear::engine::model<Base>::create_tweeners()
{
  CLAW_PRECOND( m_action != NULL );
  CLAW_PRECOND( m_snapshot != m_action->snapshot_end() );

  delete m_current_snapshot;
  m_current_snapshot = NULL;

  model_action::const_snapshot_iterator it(m_snapshot);
  ++it;

  if ( it != m_action->snapshot_end() )
    create_tweeners_to_snapshot( *it );
  else if ( !m_action->get_next_action().empty() )
    create_tweeners_to_action( *get_action(m_action->get_next_action()) );
  else
    m_current_snapshot = new model_snapshot_tweener(*m_snapshot);
} // model::create_tweeners()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the tweeners to an other snapshot of the current action.
 * \param s The other snapshot.
 */
template<class Base>
void bear::engine::model<Base>::create_tweeners_to_snapshot
( const model_snapshot& s )
{
  CLAW_PRECOND( m_action != NULL );
  CLAW_PRECOND( m_snapshot != m_action->snapshot_end() );

  m_current_snapshot =
      new model_snapshot_tweener
      ( *m_snapshot, s, *m_action, *m_action,
        s.get_date() - m_snapshot->get_date());
} // model::create_tweeners_to_snapshot()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the tweeners to the first snapshot of an other action.
 * \param a The other action.
 */
template<class Base>
void bear::engine::model<Base>::create_tweeners_to_action
( const model_action& a )
{
  CLAW_PRECOND( m_action != NULL );
  CLAW_PRECOND( m_snapshot != m_action->snapshot_end() );

  const universe::time_type d =
    std::max( 0.0, m_action->get_duration() - m_snapshot->get_date() );

  m_current_snapshot =
    new model_snapshot_tweener
    ( *m_snapshot, *a.snapshot_begin(), *m_action, a, d );
} // model::create_tweeners_to_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Export the methods of the class.
 */
template<class Base>
void bear::engine::model<Base>::init_exported_methods()
{
  TEXT_INTERFACE_CONNECT_METHOD_1
    ( model<Base>, start_model_action, void, const std::string& );
} // model::init_exported_methods()

/*----------------------------------------------------------------------------*/
TEXT_INTERFACE_IMPLEMENT_TEMPLATE_METHOD_LIST_1( bear::engine::model,
                                                 class, Base )
