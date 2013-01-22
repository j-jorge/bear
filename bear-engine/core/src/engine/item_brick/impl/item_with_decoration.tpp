/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::item_with_decoration class.
 * \author Sebastien Angibaud
 */

#include <limits>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
template<class Base>
bear::engine::item_with_decoration<Base>::item_with_decoration()
  : m_item_to_mimic(NULL), m_extend_on_bounding_box(false)
{

} // item_with_decoration::item_with_decoration()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the item.
 */
template<class Base>
void bear::engine::item_with_decoration<Base>::build()
{
  super::build();

  if ( (this->get_size().x == 0) && (this->get_size().y == 0) )
    auto_size();
} // item_with_decoration::build()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type visual::sprite.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 *
 * Valid values for the \a name and \a value parameters are :
 *  - "sprite", visual::sprite
 *  - anything supported by the parent class
 */
template<class Base>
bool bear::engine::item_with_decoration<Base>::set_sprite_field
( const std::string& name, const visual::sprite& value )
{
  bool ok = true;

  if (name == "item_with_decoration.sprite")
    m_animation = visual::animation(value);
  else
    ok = super::set_sprite_field(name, value);

  return ok;
} // item_with_decoration::set_sprite_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type visual::animation.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 *
 * Valid values for the \a name and \a value parameters are :
 *  - "animation", visual::animation
 *  - anything supported by the parent class
 */
template<class Base>
bool bear::engine::item_with_decoration<Base>::set_animation_field
( const std::string& name, const visual::animation& value )
{
  bool ok = true;

  if (name == "item_with_decoration.animation")
    m_animation = value;
  else
    ok = super::set_animation_field(name, value);

  return ok;
} // item_with_decoration::set_animation_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type engine::base_item.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
template<class Base>
bool bear::engine::item_with_decoration<Base>::set_item_field
( const std::string& name, base_item* value )
{
  bool ok = true;

  if ((name == "item_with_decoration.item_to_mimic") && (value != NULL))
    m_item_to_mimic = value;
  else
    ok = super::set_item_field(name, value);

  return ok;
} // item_with_decoration::set_animation_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type bool.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
template<class Base>
bool bear::engine::item_with_decoration<Base>::set_bool_field
( const std::string& name, bool value )
{
  bool ok = true;

  if (name == "item_with_decoration.extend_on_bounding_box")
    m_extend_on_bounding_box = value;
  else
    ok = super::set_bool_field(name, value);

  return ok;
} // item_with_decoration::set_bool_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do one iteration in the progression of the item.
 * \param elapsed_time Elapsed time since the last call.
 */
template<class Base>
void bear::engine::item_with_decoration<Base>::progress
( universe::time_type elapsed_time )
{
  super::progress(elapsed_time);

  if ( m_animation.is_valid() )
    {
      m_animation.next(elapsed_time);
      
      if ( m_extend_on_bounding_box )
        m_animation.set_size(this->get_width(), this->get_height());
    }
} // item_with_decoration::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the sprites representing the item.
 * \param visuals (out) The sprites of the item, and their positions.
 */
template<class Base>
void bear::engine::item_with_decoration<Base>::get_visual
( std::list<scene_visual>& visuals ) const
{
  super::get_visual(visuals);

  this->add_visual(m_animation, visuals);

  if (m_item_to_mimic != NULL)
    {
      std::list<scene_visual> mimic;
      m_item_to_mimic->get_visual(mimic);
      
      for ( ; !mimic.empty(); mimic.pop_front() )
        {
          if ( m_extend_on_bounding_box )
            mimic.front().scene_element.get_rendering_attributes().set_size
              (this->get_size());
          
          mimic.front().scene_element.set_position(0,0);
          this->add_visual( mimic.front().scene_element, visuals );
        } 
    }
} // item_with_decoration::get_visual()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the animation of the item.
 * \param anim The animation.
 */
template<class Base>
void bear::engine::item_with_decoration<Base>::set_animation
( const bear::visual::animation& anim )
{
  m_animation = anim;
} // item_with_decoration::set_animation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the sprite of the item.
 * \param spr The sprite.
 */
template<class Base>
void bear::engine::item_with_decoration<Base>::set_sprite
( const bear::visual::sprite& spr )
{
  std::vector<visual::sprite> images;
  std::vector<double> d(1, std::numeric_limits<double>::infinity());

  images.push_back(spr);

  m_animation = visual::animation(images, d);
} // item_with_decoration::set_sprite()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the animation of the item.
 */
template<class Base>
const bear::visual::animation&
bear::engine::item_with_decoration<Base>::get_animation() const
{
  return m_animation;
} // item_with_decoration::get_animation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the current sprite of the item.
 */
template<class Base>
bear::visual::sprite
bear::engine::item_with_decoration<Base>::get_sprite() const
{
  visual::sprite result(m_animation.get_sprite());
  result.set_angle( result.get_angle() + this->get_visual_angle() );

  return result;
} // item_with_decoration::get_animation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute the size of the item according to its decoration.
 */
template<class Base>
void bear::engine::item_with_decoration<Base>::auto_size()
{
  if ( m_animation.is_valid() )
    this->set_size( m_animation.get_max_size() );
  else if ( m_item_to_mimic != NULL )
    this->set_size( m_item_to_mimic->get_size() );
} // item_with_decoration::auto_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the animation of the item.
 */
template<class Base>
bear::visual::animation&
bear::engine::item_with_decoration<Base>::get_animation()
{
  return m_animation;
} // item_with_decoration::get_animation()
