/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::model class.
 * \author Julien Jorge
 */
#include "bf/model.hpp"

#include "bf/mark.hpp"
#include "bf/version.hpp"
#include "bf/custom_type.hpp"

#include <sstream>
#include <claw/assert.hpp>
#include <boost/lexical_cast.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bf::model::model()
{

} // model::model()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bf::model::~model()
{
  action_list::iterator it;

  for( it=m_actions.begin(); it!=m_actions.end(); ++it )
    delete *it;
} // model::~model()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the model has no action.
 */
bool bf::model::empty() const
{
  return m_actions.empty();
} // model::empty()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if there is an action with a given name.
 * \param name The name of the action.
 */
bool bf::model::has_action( const std::string& name ) const
{
  action_list::const_iterator it;
  bool result = false;

  for ( it = m_actions.begin(); ( it != m_actions.end() ) && !result; ++it )
    result = ( (*it)->get_name() == name );

  return result;
} // model::has_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Find an action name that is not already used.
 * \param base The base from which the name is computed.
 */
std::string
bf::model::create_unique_action_name( const std::string& base ) const
{
  if ( !has_action(base) )
    return base;

  const std::string::size_type p = base.find_last_not_of("0123456789-");
  std::string prefix;

  if ( p == std::string::npos )
    prefix = base;
  else
    prefix = base.substr(0, p+1);

  for ( std::size_t i=1; /* nothing */ ; ++i )
    {
      const std::string candidate
        ( prefix + '-' + boost::lexical_cast<std::string>(i) );

      if ( !has_action(candidate) )
        return candidate;
    }
} // model::create_unique_action_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add an action.
 * \param action_name The name of the new action.
 * \param duration The duration of the new action
 * \param sound The sound of the new action.
 * \param next_action The new next action.
 */
bf::action& bf::model::add_action
( const std::string& action_name, double duration,
  const sound_description& sound,
  const std::string& next_action)
{
  CLAW_PRECOND( !has_action(action_name) );

  action* a = new action(action_name, duration, sound, next_action);

  m_actions.push_back(a);

  return *a;
} // model::add_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Insert an action.
 * \param a The action to add.
 */
void bf::model::add_action( action* a )
{
  CLAW_PRECOND( !has_action(a->get_name()) );

  m_actions.push_back(a);
} // level::add_action)

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove an action.
 * \param name The name of the action to remove.
 */
bf::action* bf::model::remove_action( const std::string& name )
{
  CLAW_PRECOND( has_action(name) );

  action_list::iterator it;
  for ( it = m_actions.begin(); (*it)->get_name() != name; ++it )
    ;

  action* result = *it;
  m_actions.erase(it);

  return result;
} // model::remove_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an action from its name.
 * \param name The name of the action.
 * \pre has_action(name) == true
 */
bf::action& bf::model::get_action( const std::string& name )
{
  CLAW_PRECOND( has_action(name) );
  action_list::iterator it;

  for ( it = m_actions.begin(); (*it)->get_name() != name; ++it )
    ;

  return **it;
} // model::get_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an action from its name.
 * \param name The name of the action.
 * \pre has_action(name) == true
 */
const bf::action& bf::model::get_action( const std::string& name ) const
{
  CLAW_PRECOND( has_action(name) );

  action_list::const_iterator it;

  for ( it = m_actions.begin(); (*it)->get_name() != name; ++it )
    ;

  return **it;
} // model::get_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compile the model.
 * \param f The file in which we compile.
 * \param c The context in which the compilation is done.
 */
void bf::model::compile( compiled_file& f, compilation_context& c ) const
{
  f << BF_MAJOR_VERSION << BF_MINOR_VERSION << BF_RELEASE_NUMBER;

  const_action_iterator it;
  std::map<any_animation, std::size_t> anim_ref;
  std::list<any_animation> all_anims;

  for (it=action_begin(); it!=action_end(); ++it)
    {
      action::const_mark_iterator itm;
      for ( itm=it->mark_begin(); itm!=it->mark_end(); ++itm )
        if ( itm->has_animation() )
          if ( anim_ref.find( itm->get_animation() ) == anim_ref.end() )
            {
              anim_ref[ itm->get_animation() ] = all_anims.size();
              all_anims.push_back( itm->get_animation() );
            }
    }

  f << all_anims.size();

  std::list<any_animation>::const_iterator ita;
  for (ita=all_anims.begin(); ita!=all_anims.end(); ++ita)
    ita->compile(f, c);

  f << m_actions.size();
  for (it=action_begin(); it!=action_end(); ++it)
    it->compile(f, anim_ref);
} // model::compile()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the iterator on the beginning of the action list.
 */
bf::model::const_action_iterator bf::model::action_begin() const
{
  return const_action_iterator(m_actions.begin());
} // model::action_begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the iterator on the end of the action list.
 */
bf::model::const_action_iterator bf::model::action_end() const
{
  return const_action_iterator(m_actions.end());
} // model::action_end()
