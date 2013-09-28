/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::mark class.
 * \author Julien Jorge
 */
#include "bf/mark.hpp"

#include "bf/compiled_file.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bf::mark::mark()
  : m_apply_angle_to_animation(false), m_pause_animation_when_hidden(false),
    m_reset_animation_with_action(true)
{

} // mark::mark()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the label of the mark.
 * \param label The label of the mark.
 */
void bf::mark::set_label( const std::string& label )
{
  m_label = label;
} // mark::set_label()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the label of the mark.
 */
const std::string& bf::mark::get_label() const
{
  return m_label;
} // mark::get_label()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the animation used for this mark.
 * \param anim The animation.
 */
void bf::mark::set_animation( const any_animation& anim )
{
  m_animation = anim;
} // mark::set_animation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the animation file used for this mark.
 */
const bf::any_animation& bf::mark::get_animation() const
{
  return m_animation;
} // mark::get_animation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if this mark has an animation associated to it.
 */
bool bf::mark::has_animation() const
{
  return !get_animation_data().empty();
} // mark::has_animation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the animation associated with this mark.
 */
bf::animation bf::mark::get_animation_data() const
{
  return m_animation.get_current_animation();
} // mark::get_animation_data()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if the angle applied to the mark is also applied to the
 *        animation.
 * \param b Apply it or not.
 */
void bf::mark::apply_angle_to_animation(bool b)
{
  m_apply_angle_to_animation = b;
} // mark::apply_angle_to_animation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if the angle applied to the mark is also applied to the
 *        animation.
 */
bool bf::mark::apply_angle_to_animation() const
{
  return m_apply_angle_to_animation;
} // mark::apply_angle_to_animation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if the animation is paused when the mark is hidden.
 * \param b Pause or not.
 */
void bf::mark::pause_animation_when_hidden(bool b)
{
  m_pause_animation_when_hidden = b;
} // mark::pause_animation_when_hidden()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if the animation is paused when the mark is hidden.
 */
bool bf::mark::pause_animation_when_hidden() const
{
  return m_pause_animation_when_hidden;
} // mark::pause_animation_when_hidden()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if the animation must be reset when an action begins.
 * \param b Reset or not.
 */
void bf::mark::reset_animation_with_action(bool b)
{
  m_reset_animation_with_action = b;
} // mark::reset_animation_with_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if the animation must be reset when an action begins.
 */
bool bf::mark::reset_animation_with_action() const
{
  return m_reset_animation_with_action;
} // mark::reset_animation_with_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compiles the mark.
 * \param f The file in which we compile.
 * \param anim_ref The references to the animations common to several marks.
 */
void bf::mark::compile
( compiled_file& f,
  const std::map<any_animation, std::size_t>& anim_ref ) const
{
  f << m_label << m_apply_angle_to_animation << m_pause_animation_when_hidden
    << m_reset_animation_with_action;

  if ( has_animation() )
    f << anim_ref.find(get_animation())->second;
  else
    f << anim_ref.size();
} // mark::compile()
