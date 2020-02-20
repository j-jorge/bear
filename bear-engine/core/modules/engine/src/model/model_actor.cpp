/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::model_actor class.
 * \author Julien Jorge
 */
#include "bear/engine/model/model_actor.hpp"

#include "bear/engine/model/model_action.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::engine::model_actor::model_actor()
{
  // nothing to do
} // model_actor::model_actor()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 * \param that The instance to copy from.
 */
bear::engine::model_actor::model_actor( const model_actor& that )
{
  action_map::const_iterator it;

  for (it=that.m_actions.begin(); it!=that.m_actions.end(); ++it)
    m_actions[it->first] = new model_action(*it->second);

  // The animations of the marks are shared among the marks of a model, but not
  // among two models. So, we must replace the animations of the copies with
  // copies of the animations and make them shared among the new marks.

  // this associates the new animation to each animation in \a that
  typedef std::map<model_animation, model_animation> anim_map_type;

  anim_map_type anim_map;

  anim_map[NULL] = NULL;

  for ( action_map::iterator ita=m_actions.begin(); ita!=m_actions.end(); ++ita)
    for ( model_action::mark_iterator itm=ita->second->mark_begin();
          itm != ita->second->mark_end(); ++itm )
      {
        const model_animation main_anim( itm->get_main_animation() );
        const model_animation substitute( itm->get_substitute() );

        // get or create a copy of the main animation
        anim_map_type::iterator anim( anim_map.find( main_anim ) );

        if ( anim == anim_map.end() )
          {
            model_animation a( new visual::animation(*main_anim) );
            anim = anim_map.insert( std::make_pair( main_anim, a ) ).first;
          }

        itm->set_main_animation(anim->second);

        // get or create a copy of the substitute
        anim = anim_map.find( itm->get_substitute() );

        if ( anim == anim_map.end() )
          {
            model_animation a( new visual::animation(*substitute) );
            anim = anim_map.insert( std::make_pair( main_anim, a ) ).first;
          }

        itm->set_substitute(anim->second);
    }
} // model_actor::model_actor()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::engine::model_actor::~model_actor()
{
  action_map::const_iterator it;

  for (it=m_actions.begin(); it!=m_actions.end(); ++it)
    delete it->second;
} // model_actor::~model_actor()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assignment operator.
 * \param that The instance to copy from.
 */
bear::engine::model_actor&
bear::engine::model_actor::operator=( model_actor that )
{
  swap(that);
  return *this;
} // model_actor::operator=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Swap two actors.
 * \param that The instance to swap with.
 */
void bear::engine::model_actor::swap( model_actor& that ) throw()
{
  std::swap(m_actions, that.m_actions);
} // model_actor::swap()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an action of the model.
 * \param action_name The name of the action.
 */
bear::engine::model_action*
bear::engine::model_actor::get_action( const std::string& action_name ) const
{
  CLAW_PRECOND( m_actions.find(action_name) != m_actions.end() );

  return m_actions.find(action_name)->second;
} // model_actor::get_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add an action to the model.
 * \param name The name of the action.
 * \param a The action to add.
 */
void bear::engine::model_actor::add_action
( const std::string& name, const model_action& a )
{
  action_map::iterator it( m_actions.find(name) );

  if ( it == m_actions.end() )
    m_actions[name] = new model_action(a);
  else
    {
      *it->second = a;
    }
} // model_actor::add_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a substitute for the animation of all marks with a given name.
 * \param mark_name The name of the marks.
 * \param anim The new animation on the marks.
 */
void bear::engine::model_actor::set_global_substitute
( const std::string& mark_name, const model_animation& anim )
{
  action_map::iterator it;

  for( it=m_actions.begin(); it!=m_actions.end(); ++it )
    {
      const std::size_t i( it->second->get_mark_id(mark_name) );

      if ( i!=model_action::not_an_id )
        it->second->get_mark(i).set_substitute(anim);
    }
} // model_actor::set_global_substitute()

/*----------------------------------------------------------------------------*/
/**
 * \brief Restore the default animation of all marks with a given name.
 * \param mark_name The name of the marks.
 */
void bear::engine::model_actor::remove_global_substitute
( const std::string& mark_name )
{
  action_map::iterator it;

  for( it=m_actions.begin(); it!=m_actions.end(); ++it )
    {
      const std::size_t i( it->second->get_mark_id(mark_name) );

      if ( i!=model_action::not_an_id )
        it->second->get_mark(i).remove_substitute();
    }
} // model_actor::remove_global_substitute()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the beginning of the actions.
 */
bear::engine::model_actor::const_action_iterator
bear::engine::model_actor::action_begin() const
{
  return m_actions.begin();
} // model_actor::action_begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the end of the actions.
 */
bear::engine::model_actor::const_action_iterator
bear::engine::model_actor::action_end() const
{
  return m_actions.end();
} // model_actor::action_end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Swap two actors.
 * \param a The first actor.
 * \param b The second actor.
 */
void
bear::engine::swap( engine::model_actor& a, engine::model_actor& b ) throw()
{
  a.swap(b);
} // bear::swap()

/*----------------------------------------------------------------------------*/
/**
 * \brief Swap two actors.
 * \param a The first actor.
 * \param b The second actor.
 */
template<>
void std::swap( bear::engine::model_actor& a, bear::engine::model_actor& b )
{
  a.swap(b);
} // std::swap()
