/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::model_mark class.
 * \author Julien Jorge
 */
#include "bear/engine/model/model_mark.hpp"

#include "bear/engine/model/model_mark_item.hpp"
#include "bear/engine/layer/layer.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::engine::model_mark::model_mark()
  : m_apply_angle_to_animation(false), m_pause_when_hidden(false),
    m_reset_animation_with_action(true),
    m_box_item( new model_mark_item ), m_item_in_layer(false)
{

} // model_mark::model_mark()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param label The label of the mark.
 * \param anim The animation displayed on the mark.
 * \param apply_angle Tells if the angle of the mark is applied to the
 *        animation.
 * \param pause_hidden Tells if the animation must be paused when hidden.
 * \param reset_animation_with_action Tells if the animation must be reset when
 *        the action begins.
 */
bear::engine::model_mark::model_mark
( const std::string& label, const model_animation& anim, bool apply_angle,
  bool pause_hidden, bool reset_animation_with_action )
  : m_label(label), m_animation(anim), m_apply_angle_to_animation(apply_angle),
    m_pause_when_hidden(pause_hidden),
    m_reset_animation_with_action(reset_animation_with_action),
    m_box_item( new model_mark_item ),
    m_item_in_layer(false)
{

} // model_mark::model_mark()

/*----------------------------------------------------------------------------*/
/**
 * Copy constructor.
 * \param that The instance to copy from.
 */
bear::engine::model_mark::model_mark( const model_mark& that )
  : m_label(that.m_label), m_animation( that.m_animation ),
    m_substitute( that.m_substitute ),
    m_apply_angle_to_animation( that.m_apply_angle_to_animation ),
    m_pause_when_hidden( that.m_pause_when_hidden ),
    m_reset_animation_with_action( that.m_reset_animation_with_action ),
    m_box_item(that.m_box_item->clone()), m_item_in_layer(false)
{

} // model_mark::model_mark()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::engine::model_mark::~model_mark()
{
  // If the item is in a layer, it will be deleted with it. Thus we do not have
  // to delete it here.

  if ( m_item_in_layer )
    drop_box_item_from_layer();

  delete m_box_item;
} // model_mark::~model_mark()

/*----------------------------------------------------------------------------*/
/**
 * Assignment.
 * \param that The instance to assign from.
 */
bear::engine::model_mark& bear::engine::model_mark::operator=( model_mark that )
{
  swap(that);
  return *this;
} // model_mark::operator=()

/*----------------------------------------------------------------------------*/
/**
 * Swap the mark with an other mark.
 * \param that The mark to swap with.
 */
void bear::engine::model_mark::swap( model_mark& that ) throw()
{
  std::swap(m_label, that.m_label);
  std::swap(m_animation, that.m_animation);
  std::swap(m_substitute, that.m_substitute);
  std::swap(m_apply_angle_to_animation, that.m_apply_angle_to_animation);
  std::swap(m_pause_when_hidden, that.m_pause_when_hidden);
  std::swap(m_reset_animation_with_action, that.m_reset_animation_with_action);
  std::swap(m_box_item, that.m_box_item);
  std::swap(m_item_in_layer, that.m_item_in_layer);
} // model_mark::swap()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the label of the mark.
 */
const std::string& bear::engine::model_mark::get_label() const
{
  return m_label;
} // model_mark::get_label()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if there is an animation on the mark.
 */
bool bear::engine::model_mark::has_animation() const
{
 if (m_substitute != NULL)
    return m_substitute->is_valid();
 else if (m_animation != NULL)
   return m_animation->is_valid();
 else
   return false;
} // model_mark::has_animation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the animation on the mark.
 */
bear::engine::model_animation& bear::engine::model_mark::get_animation()
{
  if ( m_substitute != NULL )
    return m_substitute;
  else
    return m_animation;
} // model_mark::get_animation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the animation on the mark.
 */
const bear::engine::model_animation&
bear::engine::model_mark::get_animation() const
{
  if ( m_substitute != NULL )
    return m_substitute;
  else
    return m_animation;
} // model_mark::get_animation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the animation on the mark.
 */
const bear::engine::model_animation&
bear::engine::model_mark::get_main_animation() const
{
  return m_animation;
} // model_mark::get_main_animation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the main animation for the mark.
 * \param anim The new animation on this mark.
 */
void bear::engine::model_mark::set_main_animation( const model_animation& anim )
{
  m_animation = anim;
} // model_mark::set_main_animation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the angle is applied to the animation.
 */
bool bear::engine::model_mark::apply_angle_to_animation() const
{
  return m_apply_angle_to_animation;
} // model_mark::apply_angle_to_animation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the animation must be paused when the mark is hidden.
 */
bool bear::engine::model_mark::pause_when_hidden() const
{
  return m_pause_when_hidden;
} // model_mark::pause_when_hidden()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the substitute for the animation of the mark.
 */
const bear::engine::model_animation&
bear::engine::model_mark::get_substitute() const
{
  return m_substitute;
} // model_mark::get_substitute()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a substitute for the animation of this mark.
 * \param anim The new animation on this mark.
 */
void bear::engine::model_mark::set_substitute( const model_animation& anim )
{
  m_substitute = anim;
} // model_mark::set_substitute()

/*----------------------------------------------------------------------------*/
/**
 * \brief Restore the default animation.
 */
void bear::engine::model_mark::remove_substitute()
{
  m_substitute = NULL;
} // model_mark::remove_substitute()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if the animation of the mark must be reset when the action
 *        begins.
 */
bool bear::engine::model_mark::reset_animation_with_action() const
{
  return m_reset_animation_with_action
    &&  has_animation()
    && ( get_animation()->is_finite()
         || (get_animation()->get_first_index() != 0) );
} // model_mark::reset_animation_with_action()

/*----------------------------------------------------------------------------*/
/**
 * Get the item to keep centered on this mark.
 */
bear::engine::model_mark_item& bear::engine::model_mark::get_box_item() const
{
  return *m_box_item;
} // model_mark::get_box_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inserts the box item in a given layer. The item will not be deleted if
 *        the mark is destroyed.
 * \param the_layer The layer in which the item must be inserted.
 */
void bear::engine::model_mark::add_box_item_in_layer( layer& the_layer )
{
  if ( m_box_item->is_in_layer( the_layer ) )
    {
      CLAW_PRECOND( m_item_in_layer );
      return;
    }

  CLAW_PRECOND( !m_item_in_layer );

  m_item_in_layer = true;
  the_layer.add_item( *m_box_item );
} // model_mark::add_box_item_in_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Removes the item from its containing layer. The item will be deleted
 *        if the mark is destroyed.
 */
void bear::engine::model_mark::drop_box_item_from_layer()
{
  if ( !m_item_in_layer )
    return;

  m_item_in_layer = false;
  m_box_item->get_layer().drop_item( *m_box_item );
} // model_mark::drop_box_item_from_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Swap two marks.
 * \param a The first mark.
 * \param b The second mark.
 */
void bear::engine::swap( engine::model_mark& a, engine::model_mark& b ) throw()
{
  a.swap(b);
} // bear::swap()

/*----------------------------------------------------------------------------*/
/**
 * \brief Swap two marks.
 * \param a The first mark.
 * \param b The second mark.
 */
template<>
void std::swap( bear::engine::model_mark& a, bear::engine::model_mark& b )
{
  a.swap(b);
} // std::swap()
