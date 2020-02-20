/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::gui::visual_component class.
 * \author Julien Jorge
 */
#include "bear/gui/visual_component.hpp"

#include "bear/input/key_info.hpp"
#include "bear/input/finger_event.hpp"

#include "bear/visual/scene_line.hpp"
#include "bear/visual/scene_rectangle.hpp"

#include <algorithm>
#include <claw/assert.hpp>
#include <claw/functional.hpp>
#include <claw/real_number.hpp>

#include <climits>
#include <limits>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::gui::visual_component::visual_component()
  : m_box(0, 0, 0, 0), m_owner(NULL), m_focused_component(-1), m_visible(true),
    m_input_priority(false), m_enabled(true),
    m_top_left_border_color(0, 0, 0, 0),
    m_bottom_right_border_color(0, 0, 0, 0), m_background_color(0, 0, 0, 0)
{

} // visual_component::visual_component()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::gui::visual_component::~visual_component()
{
  clear();
} // visual_component::~visual_component()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set this component as the owner of an other component.
 * \param child The component that becomes a child of this component.
 */
void bear::gui::visual_component::insert( visual_component* child )
{
  if ( child->m_owner != NULL )
    child->m_owner->remove(child);

  m_components.push_back(child);
  child->m_owner = this;

  if (m_focused_component < 0)
    m_focused_component = 0;

  child->stay_in_owner();
  on_child_inserted(child);
} // visual_component::insert()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove a child component of this component.
 * \param child The component to remove.
 * \pre child is effectively a child of this component.
 * \remark The child will not be deleted.
 */
void bear::gui::visual_component::remove( visual_component* child )
{
  CLAW_PRECOND( std::find( m_components.begin(), m_components.end(), child )
                != m_components.end() );

  m_components.erase
    ( std::find( m_components.begin(), m_components.end(), child ) );

  if ( m_focused_component >= (int)m_components.size() )
    --m_focused_component;

  on_child_removed(child);
} // visual_component::remove()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove all children components of this component.
 * \remark The children will not be deleted.
 */
void bear::gui::visual_component::remove_all()
{
  while ( !m_components.empty() )
    remove( m_components.back() );
} // visual_component::remove_all()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the beginning of the children.
 */
bear::gui::visual_component::iterator
bear::gui::visual_component::begin() const
{
  return m_components.begin();
} // visual_component::begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the beginning of the children.
 */
bear::gui::visual_component::iterator
bear::gui::visual_component::end() const
{
  return m_components.end();
} // visual_component::end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Draw the component and its sub components on a screen.
 * \param e The scene elements of the component and its sub components.
 */
void bear::gui::visual_component::render
( std::list<visual::scene_element>& e ) const
{
  if (!m_visible)
    return;

  const scene_element_list result( get_scene_elements() );
  e.insert( e.end(), result.begin(), result.end() );
} // visual_component::render()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the component that a key had been pressed.
 * \param key The value of the pressed key.
 */
bool bear::gui::visual_component::key_pressed( const input::key_info& key )
{
  if ( !is_enabled() )
    return false;

  bool result;

  if (m_input_priority)
    {
      result = on_key_press(key);

      if ( !result )
        if ( m_focused_component >= 0 )
          result = m_components[m_focused_component]->key_pressed(key);
    }
  else if ( m_focused_component >= 0 )
    {
      result = m_components[m_focused_component]->key_pressed(key);

      if ( !result )
        result = on_key_press(key);
    }
  else
    result = on_key_press(key);

  return result;
} // visual_component::key_pressed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the component that a character had been entered.
 * \param key The value of the pressed key.
 */
bool bear::gui::visual_component::char_pressed( const input::key_info& key )
{
  if ( !is_enabled() )
    return false;

  bool result;

  if (m_input_priority)
    {
      result = on_char_pressed(key);

      if ( !result )
        if ( m_focused_component >= 0 )
          result = m_components[m_focused_component]->char_pressed(key);
    }
  else if ( m_focused_component >= 0 )
    {
      result = m_components[m_focused_component]->char_pressed(key);

      if ( !result )
        result = on_char_pressed(key);
    }
  else
    result = on_char_pressed(key);

  return result;
} // visual_component::char_pressed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the component that a joystick button had been pressed.
 * \param button The value of the pressed button.
 * \param joy_index The index of the joystick.
 */
bool bear::gui::visual_component::button_pressed
( input::joystick::joy_code button, unsigned int joy_index )
{
  if ( !is_enabled() )
    return false;

  bool result;

  if (m_input_priority)
    {
      result = on_button_press(button, joy_index);

      if ( !result )
        if ( m_focused_component >= 0)
          result = m_components[m_focused_component]->button_pressed
            (button, joy_index);
    }
  else if ( m_focused_component >= 0)
    {
      result = m_components[m_focused_component]->button_pressed
        (button, joy_index);

      if ( !result )
        result = on_button_press(button, joy_index);
    }
  else
    result = on_button_press(button, joy_index);

  return result;
} // visual_component::button_pressed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the component that a mouse button had been pressed.
 * \param key The value of the pressed button.
 * \param pos The current position of the cursor.
 */
bool bear::gui::visual_component::mouse_pressed
( input::mouse::mouse_code key,
  const claw::math::coordinate_2d<unsigned int>& pos )
{
  if ( !is_enabled() )
    return false;

  bool result;

  if (m_input_priority)
    {
      result = on_mouse_press(key, pos);

      if ( !result )
        result = broadcast_mouse_press(key, pos);
    }
  else
    {
      result = broadcast_mouse_press(key, pos);

      if ( !result )
        result = on_mouse_press(key, pos);
    }

  return result;
} // visual_component::mouse_pressed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the component that a mouse button had been released.
 * \param key The value of the released button.
 * \param pos The current position of the cursor.
 */
bool bear::gui::visual_component::mouse_released
( input::mouse::mouse_code key,
  const claw::math::coordinate_2d<unsigned int>& pos )
{
  if ( !is_enabled() )
    return false;

  bool result;

  if (m_input_priority)
    {
      result = on_mouse_released(key, pos);

      if ( !result )
        result = broadcast_mouse_released(key, pos);
    }
  else
    {
      result = broadcast_mouse_released(key, pos);

      if ( !result )
        result = on_mouse_released(key, pos);
    }

  return result;
} // visual_component::mouse_released()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the component that a mouse button had been maintained.
 * \param key The value of the maintained button.
 * \param pos The current position of the cursor.
 */
bool bear::gui::visual_component::mouse_maintained
( input::mouse::mouse_code key,
  const claw::math::coordinate_2d<unsigned int>& pos )
{
  if ( !is_enabled() )
    return false;

  bool result;

  if (m_input_priority)
    {
      result = on_mouse_maintained(key, pos);

      if ( !result )
        result = broadcast_mouse_maintained(key, pos);
    }
  else
    {
      result = broadcast_mouse_maintained(key, pos);

      if ( !result )
        result = on_mouse_maintained(key, pos);
    }

  return result;
} // visual_component::mouse_maintained()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the component that the mouse has been moved.
 * \param pos The new position of the cursor.
 */
bool bear::gui::visual_component::mouse_move
( const claw::math::coordinate_2d<unsigned int>& pos )
{
  if ( !is_enabled() )
    return false;

  bool result;

  if (m_input_priority)
    {
      result = on_mouse_move(pos);

      if ( !result )
        result = broadcast_mouse_move( pos );
    }
  else
    {
      result = broadcast_mouse_move( pos );

      if ( !result )
        result = on_mouse_move(pos);
    }

  return result;
} // visual_component::mouse_move()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the component that the finger has been used.
 * \param event The event dispatched by the finger.
 */
bool
bear::gui::visual_component::finger_action( const input::finger_event& event )
{
  if ( !is_enabled() )
    return false;

  bool result;

  if (m_input_priority)
    {
      result = on_finger_action(event);

      if ( !result )
        result = broadcast_finger_action( event );
    }
  else
    {
      result = broadcast_finger_action( event );

      if ( !result )
        result = on_finger_action(event);
    }

  return result;
} // visual_component::finger_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adjust the size of the component to fit its sub components.
 * \param margin The margin around the children.
 */
void bear::gui::visual_component::fit( visual::size_type margin )
{
  size_type min_x( std::numeric_limits<size_type>::max() );
  size_type min_y( std::numeric_limits<size_type>::max() );
  size_type max_x( std::numeric_limits<size_type>::min() );
  size_type max_y( std::numeric_limits<size_type>::min() );

  component_list::const_iterator it;
  for ( it=m_components.begin(); it!=m_components.end(); ++it )
    {
      min_x = std::min( min_x, (*it)->left() );
      min_y = std::min( min_y, (*it)->bottom() );
      max_x = std::max( max_x, (*it)->right() );
      max_y = std::max( max_y, (*it)->top() );
    }

  const size_type delta_x = min_x - margin;
  const size_type delta_y = min_y - margin;

  for ( it=m_components.begin(); it!=m_components.end(); ++it )
    (*it)->set_bottom_left
      ( (*it)->left() - delta_x, (*it)->bottom() - delta_y );

  set_size( max_x - delta_x + margin, max_y - delta_y + margin );
} // visual_component::fit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the size of the component to its maximum size.
 */
void bear::gui::visual_component::set_size_maximum()
{
  set_size( std::numeric_limits<size_type>::max(),
            std::numeric_limits<size_type>::max() );
} // visual_component::set_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the size of the component.
 * \param size The new size.
 */
void bear::gui::visual_component::set_size( const size_box_type& size )
{
  set_size( size.x, size.y );
} // visual_component::set_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the size of the component.
 * \param w The new width.
 * \param h The new height.
 */
void bear::gui::visual_component::set_size( size_type w, size_type h )
{
  const size_type old_w = m_box.width();
  const size_type old_h = m_box.height();

  m_box.set( left(), bottom(), m_box.left() + w, m_box.bottom() + h );

  stay_in_owner();

  if ( (old_w != m_box.width()) || (old_h != m_box.height()) )
    on_resized();
} // visual_component::set_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the width of the component.
 * \param w The new width.
 */
void bear::gui::visual_component::set_width( size_type w )
{
  set_size( w, height() );
} // visual_component::set_width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the height of the component.
 * \param h The new height.
 */
void bear::gui::visual_component::set_height( size_type h )
{
  set_size( width(), h );
} // visual_component::set_height()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the position of the component in its owner.
 * \param pos The new position.
 */
void bear::gui::visual_component::set_position
( const position_type& pos )
{
  set_position( pos.x, pos.y );
} // visual_component::set_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the position of the component.
 * \param x The new x-coordinate.
 * \param y The new y-coordinate.
 */
void  bear::gui::visual_component::set_position
( coordinate_type x, coordinate_type y )
{
  set_bottom_left(x, y);
} // visual_component::set_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Show or hide the component.
 * \param b The new visibility.
 */
void bear::gui::visual_component::set_visible( bool b )
{
  m_visible = b;
} // visual_component::set_visible()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the position of this component when tabbing.
 * \param v The new position.
 */
void bear::gui::visual_component::set_tab_order( unsigned int v )
{
  if (m_owner)
    m_owner->change_tab_position( this, v );
} // visual_component::set_tab_order()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set if the current component must process key_ press event before its
 *        children.
 * \param this_first True to set this compoment as priority.
 */
void bear::gui::visual_component::set_input_priority( bool this_first )
{
  m_input_priority = this_first;
} // visual_component::set_input_priority()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the left position of the component in its owner.
 * \param x The new position.
 */
void bear::gui::visual_component::set_left( coordinate_type x )
{
  set_bottom_left( x, bottom() );
} // visual_component::set_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the right position of the component in its owner.
 * \param x The new position.
 */
void bear::gui::visual_component::set_right( coordinate_type x )
{
  set_bottom_left( x - width(), bottom() );
} // visual_component::set_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the top position of the component in its owner.
 * \param y The new position.
 */
void bear::gui::visual_component::set_top( coordinate_type y )
{
  set_bottom_left( left(), y - height() );
} // visual_component::set_top()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the bottom position of the component in its owner.
 * \param x The new position.
 */
void bear::gui::visual_component::set_bottom( coordinate_type y )
{
  set_bottom_left( left(), y );
} // visual_component::set_bottom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the top-left position of the component in its owner.
 * \param x The new x-position.
 * \param y The new y-position.
 */
void bear::gui::visual_component::set_top_left
( coordinate_type x, coordinate_type y )
{
  set_bottom_left( x, y - height() );
} // visual_component::set_top_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the bottom-left position of the component in its owner.
 * \param x The new x-position.
 * \param y The new y-position.
 */
void bear::gui::visual_component::set_bottom_left
( coordinate_type x, coordinate_type y )
{
  const coordinate_type w = m_box.width();
  const coordinate_type h = m_box.height();

  m_box.left(x);
  m_box.bottom(y);
  stay_in_owner();

  if ( std::abs( w - m_box.width() ) > 0.000001 || 
       std::abs( h - m_box.height() ) > 0.000001 )
    on_resized();
} // visual_component::set_bottom_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the top-right position of the component in its owner.
 * \param x The new x-position.
 * \param y The new y-position.
 */
void bear::gui::visual_component::set_top_right
( coordinate_type x, coordinate_type y )
{
  set_bottom_left( x - width(), y - height() );
} // visual_component::set_top_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the bottom-right position of the component in its owner.
 * \param x The new x-position.
 * \param y The new y-position.
 */
void bear::gui::visual_component::set_bottom_right
( coordinate_type x, coordinate_type y )
{
  set_bottom_left( x - width(), y );
} // visual_component::set_bottom_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the width of the component.
 */
bear::gui::size_type bear::gui::visual_component::width() const
{
  return m_box.width();
} // visual_component::width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the height of the component.
 */
bear::gui::size_type bear::gui::visual_component::height() const
{
  return m_box.height();
} // visual_component::height()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the x-coordinate of the component's left edge.
 */
bear::gui::coordinate_type bear::gui::visual_component::left() const
{
  return m_box.left();
} // visual_component::left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the x-coordinate of the component's right edge.
 */
bear::gui::coordinate_type bear::gui::visual_component::right() const
{
  return m_box.right();
} // visual_component::right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the y-coordinate of the component's bottom edge.
 */
bear::gui::coordinate_type bear::gui::visual_component::bottom() const
{
  return m_box.bottom();
} // visual_component::bottom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the y-coordinate of the component's top edge.
 */
bear::gui::coordinate_type bear::gui::visual_component::top() const
{
  return m_box.top();
} // visual_component::top()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the coordinates of the component's top left corner.
 */
bear::gui::position_type bear::gui::visual_component::top_left() const
{
  return m_box.top_left();
} // visual_component::top_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the coordinates of the component's top right corner.
 */
bear::gui::position_type bear::gui::visual_component::top_right() const
{
  return m_box.top_right();
} // visual_component::top_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the coordinates of the component's bottom left corner.
 */
bear::gui::position_type bear::gui::visual_component::bottom_left() const
{
  return m_box.bottom_left();
} // visual_component::bottom_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the coordinates of the component's bottom right corner.
 */
bear::gui::position_type bear::gui::visual_component::bottom_right() const
{
  return m_box.bottom_right();
} // visual_component::bottom_right()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the size of the component.
 */
bear::gui::size_box_type bear::gui::visual_component::get_size() const
{
  return m_box.size();
} // visual_component::get_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the position of the component in its owner.
 */
bear::gui::position_type bear::gui::visual_component::get_position() const
{
  return m_box.bottom_left();
} // visual_component::get_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the rectangle bounding this component.
 */
const bear::gui::rectangle_type&
bear::gui::visual_component::get_rectangle() const
{
  return m_box;
} // visual_component::get_rectangle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the component is visible.
 */
bool bear::gui::visual_component::get_visible() const
{
  return m_visible;
} // visual_component::get_visible()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the size of the borders.
 */
bear::gui::size_type bear::gui::visual_component::get_border_size() const
{
  return 1;
} // visual_component::get_border_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the color of the border.
 * \param clr The new color.
 */
void bear::gui::visual_component::set_border_color( const color_type& clr )
{
  set_top_left_border_color( clr );
  set_bottom_right_border_color( clr );
} // visual_component::set_border_color()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the color of the top and left borders.
 * \param clr The new color.
 */
void
bear::gui::visual_component::set_top_left_border_color( const color_type& clr )
{
  m_top_left_border_color = clr;
} // visual_component::set_top_left_border_color()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the color of the top and left borders.
 */
const bear:: gui::color_type&
bear::gui::visual_component::get_top_left_border_color() const
{
  return m_top_left_border_color;
} // visual_component::get_top_left_border_color()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the color of the bottom and right borders.
 * \param clr The new color.
 */
void bear::gui::visual_component::set_bottom_right_border_color
( const color_type& clr )
{
  m_bottom_right_border_color = clr;
} // visual_component::set_bottom_right_border_color()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the color of the bottom and right borders.
 */
const bear::gui::color_type&
bear::gui::visual_component::get_bottom_right_border_color() const
{
  return m_bottom_right_border_color;
} // visual_component::get_bottom_right_border_color()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the color of the background.
 * \param clr The new color.
 */
void bear::gui::visual_component::set_background_color( const color_type& clr )
{
  m_background_color = clr;
} // visual_component::set_background_color()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the color of the background.
 * \param clr The new color.
 */
const bear::gui::color_type&
bear::gui::visual_component::get_background_color() const
{
  return m_background_color;
} // visual_component::get_background_color()

/*----------------------------------------------------------------------------*/
/**
 * \brief Disable this component.
 */
void bear::gui::visual_component::disable()
{
  m_enabled = false;
} // visual_component::disable()

/*----------------------------------------------------------------------------*/
/**
 * \brief Enable this component.
 */
void bear::gui::visual_component::enable()
{
  m_enabled = true;
} // visual_component::enable()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if this compontent is enabled.
 */
bool bear::gui::visual_component::is_enabled() const
{
  return m_enabled;
} // visual_component::is_enabled()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the focus on this component.
 */
void bear::gui::visual_component::set_focus()
{
  const visual_component* foc = get_focus();
  if ( foc != this )
    {
      // all parent controls up to the top component
      std::list<visual_component*> s;
      visual_component* c;

      for ( c = this; c!=NULL; c=c->m_owner )
        s.push_front(c);

      std::list<visual_component*>::const_iterator it_parent(s.begin());
      std::list<visual_component*>::const_iterator it_child(it_parent);
      ++it_child;

      for ( ; it_child!=s.end(); ++it_parent, ++it_child )
        (*it_parent)->set_focus(*it_child);

      for ( it_parent=s.begin(); it_parent!=s.end(); ++it_parent )
        (*it_parent)->on_focused();
    }
} // visual_component::set_focus()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the child component having the focus.
 */
bear::gui::visual_component* bear::gui::visual_component::get_focus() const
{
  if ( m_focused_component < 0 )
    return NULL;
  else
    return m_components[m_focused_component];
} // visual_component::get_focus()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a callback called when the component receives the focus.
 * \param c The callback.
 */
void bear::gui::visual_component::add_focus_callback( const callback& c )
{
  m_focus_callback.add(c);
} // visual_component::add_focus_callback()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove all the children of the component.
 */
void bear::gui::visual_component::clear()
{
  std::for_each( m_components.begin(), m_components.end(),
                 claw::delete_function<visual_component*>() );
  m_components.clear();
  m_focused_component = -1;
  on_clear();
} // visual_component::clear()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the rendering attributes.
 */
bear::visual::bitmap_rendering_attributes&
bear::gui::visual_component::get_rendering_attributes()
{
  return m_rendering_attributes;
} // visual_component::get_rendering_attributes()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the rendering attributes.
 */
const bear::visual::bitmap_rendering_attributes&
bear::gui::visual_component::get_rendering_attributes() const
{
  return m_rendering_attributes;
} // visual_component::get_rendering_attributes()

/*----------------------------------------------------------------------------*/
/**
 * \brief Change the position of a component when tabbing.
 * \param that The component to move.
 * \param pos The new position.
 * \remark The item which was at the position \a pos will be placed at the old
 *         position of \a that.
 */
void bear::gui::visual_component::change_tab_position
( const visual_component* that, unsigned int pos )
{
  CLAW_PRECOND( std::find(m_components.begin(), m_components.end(), that)
                != m_components.end() );

  if ( pos > m_components.size() - 1 )
    pos = m_components.size() - 1;

  std::swap( m_components[pos], *std::find(m_components.begin(),
                                           m_components.end(), that) );
} // visual_component::change_tab_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Method called after the component has been resized.
 */
void bear::gui::visual_component::on_resized()
{
  // nothing to do
} // visual_component::on_resized()

/*----------------------------------------------------------------------------*/
/**
 * \brief Method called after calling clear function.
 */
void bear::gui::visual_component::on_clear()
{
  // nothing to do
} // visual_component::on_clear()

/*----------------------------------------------------------------------------*/
/**
 * \brief Method called after the insertion of a child.
 * \param child The child inserted in this component.
 */
void bear::gui::visual_component::on_child_inserted( visual_component* child )
{
  // nothing to do
} // visual_component::on_child_inserted()

/*----------------------------------------------------------------------------*/
/**
 * \brief Method called after the removal of a child.
 * \param child The child removed from this component.
 */
void bear::gui::visual_component::on_child_removed( visual_component* child )
{
  // nothing to do
} // visual_component::on_child_removed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the scene elements of the component.
 * \param e (out) The scene elements.
 */
void bear::gui::visual_component::display( scene_element_list& e ) const
{
  // nothing to do
} // visual_component::display()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell the focused component that a key has been pressed.
 * \param key The pressed key.
 * \return true if the key has been processed.
 */
bool bear::gui::visual_component::on_key_press( const input::key_info& key )
{
  bool result = false;

  if ( key.is_tab() )
    {
      if ( !m_components.empty() )
        {
          result = true;
          m_focused_component = (m_focused_component + 1) % m_components.size();
        }
    }

  return result;
} // visual_component::on_key_press()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell the focused component that a character has been entered.
 * \param key The pressed key.
 * \return true if the key has been processed.
 */
bool bear::gui::visual_component::on_char_pressed( const input::key_info& key )
{
  return false;
} // visual_component::on_char_pressed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell the component that a joystick button has been pressed.
 * \param button The value of the pressed button.
 * \param joy_index The index of the joystick.
 * \return true if the button has been processed.
 */
bool bear::gui::visual_component::on_button_press
( input::joystick::joy_code button, unsigned int joy_index )
{
  return false;
} // visual_component::on_button_press()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell the focused component that a mouse button has been pressed.
 * \param key The pressed key.
 * \param pos The current position of the cursor.
 * \return true if the key has been processed.
 */
bool bear::gui::visual_component::on_mouse_press
( input::mouse::mouse_code key,
  const claw::math::coordinate_2d<unsigned int>& pos )
{
  return false;
} // visual_component::on_mouse_press()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell the focused component that a mouse button has been released.
 * \param key The pressed key.
 * \param pos The current position of the cursor.
 * \return true if the key has been processed.
 */
bool bear::gui::visual_component::on_mouse_released
( input::mouse::mouse_code key,
  const claw::math::coordinate_2d<unsigned int>& pos )
{
  return false;
} // visual_component::on_mouse_released()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell the focused component that a mouse button has been maintained.
 * \param key The pressed key.
 * \param pos The current position of the cursor.
 * \return true if the key has been processed.
 */
bool bear::gui::visual_component::on_mouse_maintained
( input::mouse::mouse_code key,
  const claw::math::coordinate_2d<unsigned int>& pos )
{
  return false;
} // visual_component::on_mouse_maintained()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells the component that the mouse has been moved.
 * \param pos The new position of the cursor.
 * \return true if the button has been processed.
 */
bool bear::gui::visual_component::on_mouse_move
( const claw::math::coordinate_2d<unsigned int>& pos )
{
  return false;
} // visual_component::on_mouse_move()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells the component that the finger has been used.
 * \param event The event dispatched by the finger.
 */
bool bear::gui::visual_component::on_finger_action
( const input::finger_event& event )
{
  return false;
} // visual_component::on_finger_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell the component he received the focus.
 */
void bear::gui::visual_component::on_focused()
{
  m_focus_callback.execute();
} // visual_component::on_focused()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the sub components that the mouse has been moved.
 * \param pos The new position of the cursor.
 */
bool bear::gui::visual_component::broadcast_mouse_move
( const claw::math::coordinate_2d<unsigned int>& pos )
{
  bool result(false);
  component_list::iterator it;

  for (it=m_components.begin(); !result && (it!=m_components.end()); ++it)
    if ( (*it)->m_box.includes(pos) )
      result = (*it)->mouse_move( pos - (*it)->get_position() );

  return result;
} // visual_component::broadcast_mouse_move()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the sub components that a mouse button has been pressed.
 * \param button The pressed key.
 * \param pos The current position of the cursor.
 * \return true if the key has been processed.
 */
bool bear::gui::visual_component::broadcast_mouse_press
( input::mouse::mouse_code button,
  const claw::math::coordinate_2d<unsigned int>& pos )
{
  bool result(false);
  component_list::iterator it;

  for (it=m_components.begin(); !result && (it!=m_components.end()); ++it)
    if ( (*it)->m_box.includes(pos) )
      result = (*it)->mouse_pressed( button, pos - (*it)->get_position() );

  return result;
} // visual_component::broadcast_mouse_press()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the sub components that a mouse button has been released.
 * \param button The pressed key.
 * \param pos The current position of the cursor.
 * \return true if the key has been processed.
 */
bool bear::gui::visual_component::broadcast_mouse_released
( input::mouse::mouse_code button,
  const claw::math::coordinate_2d<unsigned int>& pos )
{
  bool result(false);
  component_list::iterator it;

  for (it=m_components.begin(); !result && (it!=m_components.end()); ++it)
    if ( (*it)->m_box.includes(pos) )
      result = (*it)->mouse_released( button, pos - (*it)->get_position() );

  return result;
} // visual_component::broadcast_mouse_released()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the sub components that a mouse button has been maintained.
 * \param button The pressed key.
 * \param pos The current position of the cursor.
 * \return true if the key has been processed.
 */
bool bear::gui::visual_component::broadcast_mouse_maintained
( input::mouse::mouse_code button,
  const claw::math::coordinate_2d<unsigned int>& pos )
{
  bool result(false);
  component_list::iterator it;

  for (it=m_components.begin(); !result && (it!=m_components.end()); ++it)
    if ( (*it)->m_box.includes(pos) )
      result = (*it)->mouse_maintained( button, pos - (*it)->get_position() );

  return result;
} // visual_component::broadcast_mouse_maintained()

/*----------------------------------------------------------------------------*/
/**
 * \brief Informs the sub components that the finger has been used.
 * \param event The event dispatched by the finger.
 */
bool bear::gui::visual_component::broadcast_finger_action
( const input::finger_event& event )
{
  bool result(false);
  component_list::iterator it;
  const position_type pos( event.get_position() );

  for (it=m_components.begin(); !result && (it!=m_components.end()); ++it)
    if ( (*it)->m_box.includes(pos) )
      result =
        (*it)->finger_action
        ( event.at_position( pos - (*it)->get_position() ) );

  return result;
} // visual_component::broadcast_finger_action()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adjust the width and the height of the component to stay in the owner.
 */
void bear::gui::visual_component::stay_in_owner()
{
  if (m_owner)
    {
      coordinate_type n_left(m_box.left());
      coordinate_type n_right(m_box.right());
      coordinate_type n_bottom(m_box.bottom());
      coordinate_type n_top(m_box.top());

      if ( n_left >= m_owner->width() )
        n_left = m_owner->width();

      if ( n_bottom >= m_owner->height() )
        n_bottom = m_owner->height();

      if ( n_right >= m_owner->width() )
        n_right = m_owner->width();

      if ( n_top >= m_owner->height() )
        n_top = m_owner->height();

      m_box.set( n_left, n_bottom, n_right, n_top );
    }
} // visual_component::stay_in_owner()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the focus on a given component.
 * \param c The component on which the focus is set.
 */
void bear::gui::visual_component::set_focus( visual_component* c )
{
  int i(0);
  bool stop(false);
  m_focused_component = -1;

  for ( component_list::const_iterator it=m_components.begin();
        !stop && (it != m_components.end()); ++it, ++i )
    if ( *it == c )
      {
        m_focused_component = i;
        stop = true;
      }
} // visual_component::set_focus()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the scene elements representing this component.
 */
bear::gui::visual_component::scene_element_list
bear::gui::visual_component::get_scene_elements() const
{
  scene_element_list result;

  // display the current component
  display( result );

  scene_element_list sub_e;

  // display the sub components
  for ( component_list::const_iterator it=m_components.begin();
        it!=m_components.end();
        ++it )
    (*it)->render( sub_e );

  // the scene elements are placed in the current component, so we adjust
  // their position according to our position.
  for ( scene_element_list::iterator it=sub_e.begin(); it!=sub_e.end(); ++it )
    it->set_position( it->get_position() + m_box.bottom_left() );
      
  result.insert( result.end(), sub_e.begin(), sub_e.end() );

  render_faces( result );

  for ( scene_element_list::iterator it=result.begin(); it!=result.end(); ++it )
    it->get_rendering_attributes().combine(m_rendering_attributes);

  return result;
} // visual_component::get_scene_elements()

/*----------------------------------------------------------------------------*/
/**
 * \brief Draw the background and the borders of the component.
 * \param e The scene elements of the borders and the background.
 */
void bear::gui::visual_component::render_faces
( std::list<visual::scene_element>& e ) const
{
  // and we draw the borders and the background of the current component
  visual::rectangle_type box
    (visual::position_type(0, 0), visual::position_type( width(), height() ));

  if ( get_border_size() != 0 )
    {
      std::vector<visual::position_type> line(3);
      line[0] = box.bottom_left();
      line[1] = box.bottom_right();
      line[2] = box.top_right();

      if ( m_bottom_right_border_color.components.alpha != 0 )
        e.push_front
          ( visual::scene_line
            (left(), bottom(), m_bottom_right_border_color, line,
             get_border_size()) );

      line[1] = box.top_left();

      if ( m_top_left_border_color.components.alpha != 0 )
        e.push_front
          ( visual::scene_line
            (left(), bottom(), m_top_left_border_color, line,
             get_border_size()) );
    }

  if ( m_background_color.components.alpha != 0 )
    e.push_front
      ( visual::scene_rectangle(left(), bottom(), m_background_color, box) );
} // visual_component::render_faces()
