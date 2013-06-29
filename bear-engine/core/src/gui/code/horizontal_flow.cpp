/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::gui::horizontal_flow class.
 * \author Julien Jorge
 */
#include "gui/horizontal_flow.hpp"

#include "visual/scene_rectangle.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param horizontal_margin The horizontal margin between the children
 *        components.
 * \param vertical_margin The vertical margin between the children components.
 * \param selected color Color used to display rectangle around selected
 * childrens.
 */
bear::gui::horizontal_flow::horizontal_flow
( size_type horizontal_margin, size_type vertical_margin,
  bear::visual::color_type selected_color )
  : visual_component(), m_horizontal_margin(horizontal_margin),
    m_vertical_margin(vertical_margin), m_selected_children(NULL),
    m_selected_color(selected_color)
{
  // nothing to do
} // horizontal_flow::horizontal_flow()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the horizontal margin between the children components.
 * \param m The horizontal margin between the children components.
 */
void bear::gui::horizontal_flow::set_horizontal_margin( size_type m )
{
  set_margins( m, m_vertical_margin );
} // horizontal_flow::set_horizontal_margin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the vertical margin between the children components.
 * \param m The vertical margin between the children components.
 */
void bear::gui::horizontal_flow::set_vertical_margin( size_type m )
{
  set_margins( m_horizontal_margin, m );
} // horizontal_flow::set_vertical_margin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the margins between the children components.
 * \param horizontal The horizontal margin between the chidren components.
 * \param vertical The vertical margin between the children components.
 */
void bear::gui::horizontal_flow::set_margins
( size_type horizontal, size_type vertical )
{
  m_horizontal_margin = horizontal;
  m_vertical_margin = vertical;
  adjust_children_positions();
} // horizontal_flow::set_margins()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the scene elements of the component.
 * \param e (out) The scene elements.
 */
void bear::gui::horizontal_flow::display
( std::list<visual::scene_element>& e ) const
{
  if ( m_selected_children != NULL )
    {
      visual::rectangle_type rect
        (0, 0, m_selected_children->get_size().x + 2,
         m_selected_children->get_size().y + 2 );

      e.push_back
        ( visual::scene_rectangle
          (left() + m_selected_children->left() - 1,
           bottom() + m_selected_children->bottom() - 1,
           m_selected_color, rect, false, 2) );
    }
} // horizontal_flow::display()

/*----------------------------------------------------------------------------*/
/**
 * \brief Method called when a mouse moves over the frame.
 * \param pos The position of the mouse.
 */
bool bear::gui::horizontal_flow::process_mouse_move
( const claw::math::coordinate_2d<unsigned int>& pos )
{
  highlight_control_at(pos);

  return false;
} // frame::process_mouse_move()

/*----------------------------------------------------------------------------*/
/**
 * \brief Function called when a key is pressed.
 * \param key The pressed key.
 */
bool bear::gui::horizontal_flow::on_key_press( const input::key_info& key )
{
  return special_code( key );
} // horizontal_flow::on_key_press()

/*----------------------------------------------------------------------------*/
/**
 * \brief Method called when a mouse moves over the frame.
 * \param pos The position of the mouse.
 */
bool bear::gui::horizontal_flow::on_mouse_move
( const claw::math::coordinate_2d<unsigned int>& pos )
{
  return process_mouse_move(pos);
} // frame::on_mouse_move()

/*----------------------------------------------------------------------------*/
/**
 * \brief Highlight a control at a given position.
 * \param pos A position where the control is.
 */
bool bear::gui::horizontal_flow::highlight_control_at
( const claw::math::coordinate_2d<unsigned int>& pos )
{
  bool stop(false);
  
  for ( iterator it = begin(); !stop && (it != end()); ++it )
    if ( it->get_rectangle().includes(pos) && it->get_visible() )
      {
        stop = true;
        m_selected_children = &(*it);
        it->set_focus();
      }

  return stop;
} // frame::highlight_control_at()

/*----------------------------------------------------------------------------*/
/**
 * \brief Process a special key code.
 * \param key The code of the key.
 * \return True if the code has been processed.
 */
bool bear::gui::horizontal_flow::special_code( const input::key_info& key )
{
  bool result = false;

  if ( key.is_left() )
    result = move_left();
  else if ( key.is_right() )
    result = move_right();
  else if ( key.is_up() )
     result = move_up();
  else if ( key.is_down() )
    result = move_down();

  return result;
} // horizontal_flow::special_code()

/*----------------------------------------------------------------------------*/
/**
 * \brief Give focus at a chidren at top of a given position.
 * \param line The line of current focused children.
 * \param column Column of current focused children.
 * \return True if children at left exists.
 */
bool bear::gui::horizontal_flow::children_at_top
(unsigned int line, unsigned int column)
{
  bool result(false);

  if ( line > 0 )
    {
      line--;

      if ( !m_childrens_array[line].empty() )
        {
          if ( column >= m_childrens_array[line].size() )
            column = m_childrens_array[line].size() - 1;

          m_selected_children = m_childrens_array[line][column];
          m_childrens_array[line][column]->set_focus();
          result = true;
        }
    }

  return result;
} // children_at_top()

/*----------------------------------------------------------------------------*/
/**
 * \brief Give focus at a chidren at bottom of a given position.
 * \param line The line of current focused children.
 * \param column Column of current focused children.
 * \return True if children at left exists.
 */
bool bear::gui::horizontal_flow::children_at_bottom
(unsigned int line, unsigned int column)
{
  bool result(false);

  line++;

  if ( line < m_childrens_array.size() )
    {
      if ( !m_childrens_array[line].empty() )
        {
          if ( column >= m_childrens_array[line].size() )
            column = m_childrens_array[line].size() - 1;

          m_selected_children = m_childrens_array[line][column];
          m_childrens_array[line][column]->set_focus();
          result = true;
        }
    }

  return result;
} // children_at_bottom()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the iterator on selected children.
 */
bear::gui::visual_component::iterator
bear::gui::horizontal_flow::get_selected_children()
{
  iterator it=begin();
  bool stop(false);

  if ( m_selected_children != NULL )
    while ( ( it != end() ) && ( !stop ) )
      {
        if ( &(*it) == m_selected_children )
          stop = true;
        else
          ++it;
      }
  else
    it = end();

  return it;
} // get_selected_children()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute position of selected children in childrens array.
 * \param line The line of selected children (out).
 * \param column Column of selected chldren (out).
 * \return True if found.
 */
bool bear::gui::horizontal_flow::get_selected_children_in_array
(unsigned int & line, unsigned int & column)
{
  bool stop(false);

  if (m_selected_children != NULL )
    for (unsigned int i = 0; (i < m_childrens_array.size()) && !stop; ++i )
      for (unsigned int j = 0; (j<m_childrens_array[i].size()) && !stop; ++j )
        if ( m_childrens_array[i][j] == m_selected_children )
          {
            line = i;
            column = j;
            stop = true;
          }

  return stop;
} // get_selected_children()

/*----------------------------------------------------------------------------*/
/**
 * \brief Move the cursor one character to the left.
 * \return True if the code has been processed.
 */
bool bear::gui::horizontal_flow::move_left()
{
  bool result(false);

  iterator it=get_selected_children();

  if ( it!=end() && it!=begin() )
    {
      --it;
      m_selected_children = &(*it);
      it->set_focus();
      result = true;
    }

  return result;
} // horizontal_flow::move_left()

/*----------------------------------------------------------------------------*/
/**
 * \brief Move the cursor one character to the right.
 * \return True if the code has been processed.
 */
bool bear::gui::horizontal_flow::move_right()
{
  bool result(false);

  iterator it=get_selected_children();

  if ( it!=end() )
    {
      ++it;
      if ( it!=end() )
        if ( it->get_visible() )
          {
            m_selected_children = &(*it);
            it->set_focus();
            result = true;
          }
    }

  return result;
} // horizontal_flow::move_right()


/*----------------------------------------------------------------------------*/
/**
 * \brief Move the cursor one character to the top.
 * \return True if the code has been processed.
 */
bool bear::gui::horizontal_flow::move_up()
{
  bool result(false);

  unsigned int line, column;

  if ( get_selected_children_in_array(line, column) )
    result = children_at_top(line, column);

  return result;
} // horizontal_flow::move_up()

/*----------------------------------------------------------------------------*/
/**
 * \brief Move the cursor one character to the bottom.
 * \return True if the code has been processed.
 */
bool bear::gui::horizontal_flow::move_down()
{
  bool result(false);

  unsigned int line, column;

  if ( get_selected_children_in_array(line, column) )
    result = children_at_bottom(line, column);

  return result;
} // horizontal_flow::move_down()

/*----------------------------------------------------------------------------*/
/**
 * \brief Method called after the component has been resized. The position of
 *        the children are updated to fill the component.
 */
void bear::gui::horizontal_flow::on_resized()
{
  adjust_children_positions();
} // horizontal_flow::on_resized()

/*----------------------------------------------------------------------------*/
/**
 * \brief Method called after calling clear function.
 */
void bear::gui::horizontal_flow::on_clear()
{
  m_selected_children = NULL;
} // horizontal_flow::on_clear()

/*----------------------------------------------------------------------------*/
/**
 * \brief Method called after the insertion of a child.
 * \param child The child inserted in this component.
 */
void bear::gui::horizontal_flow::on_child_inserted( visual_component* child )
{
  adjust_children_positions();

  if ( m_selected_children == NULL )
    m_selected_children = child;
} // horizontal_flow::on_child_inserted()

/*----------------------------------------------------------------------------*/
/**
 * \brief Method called after the removal of a child.
 * \param child The child removed from this component.
 */
void bear::gui::horizontal_flow::on_child_removed( visual_component* child )
{
  adjust_children_positions();
} // horizontal_flow::on_child_removed()

/*----------------------------------------------------------------------------*/
/**
 * \brief The focus has been set on the frame or one of its children.
 */
void bear::gui::horizontal_flow::on_focused()
{
  iterator it(get_selected_children());

  if ( it != end() )
    m_selected_children = &(*it);
  else if ( begin() != end() )
    {
      m_selected_children = &(*(begin()));
      m_selected_children->set_focus();
    }
} // gui::horizontal_flow::on_focused()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adjust he position of the children to fill the component.
 */
void bear::gui::horizontal_flow::adjust_children_positions()
{
  iterator first=begin();
  coordinate_type top = height() - m_vertical_margin;
  unsigned int line=0;
  m_childrens_array.clear();

  while ( first != end() )
    {
      bool stop = false;
      size_type line_width = 2 * m_horizontal_margin;
      size_type line_height = 0;
      iterator last = first;

      while ( !stop && last!=end() )
        if ( line_width + last->width() > width() )
          stop = true;
        else
          {
            line_width += last->width() + m_horizontal_margin;
            line_height = std::max( line_height, last->height() );
            ++last;
          }

      if ( top < line_height ) // no more child will be visible.
        for ( ; first!=end(); ++first )
          first->set_visible(false);
      else
        {
          if ( first!=last )
            m_childrens_array.push_back(std::vector<visual_component*>());

          coordinate_type x = m_horizontal_margin;
          for ( ; first!=last; ++first )
            {
              first->set_visible(true);
              first->set_position
                ( x, top - line_height + (line_height - first->height()) / 2 );
              x += first->width() + m_horizontal_margin;
              m_childrens_array[line].push_back(&(*first));
            }
        }

      top -= line_height + m_vertical_margin;
      line++;
    }
} // horizontal_flow::adjust_children_positions()
