/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::mark_placement class.
 * \author Julien Jorge
 */
#include "bf/mark_placement.hpp"

#include "bf/compiled_file.hpp"
#include "bf/mark.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param m The identifier of mark concerned by this placement.
 */
bf::mark_placement::mark_placement( const mark* m )
  : m_mark(m), m_x(0), m_y(0), m_width(0), m_height(0), m_depth(0), m_angle(0),
    m_visible(true)
{

} // mark_placement::mark_placement()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assign an other placement to the mark of this placement.
 * \param that The instance to copy from.
 */
void bf::mark_placement::copy( const mark_placement& that )
{
  m_x = that.m_x;
  m_y = that.m_y;
  m_width = that.m_width;
  m_height = that.m_height;
  m_depth = that.m_depth;
  m_angle = that.m_angle;
  m_visible = that.m_visible;
  m_collision_function = that.m_collision_function;

  m_x_easing = that.m_x_easing;
  m_y_easing = that.m_y_easing;
  m_width_easing = that.m_width_easing;
  m_height_easing = that.m_height_easing;
  m_angle_easing = that.m_angle_easing;
} // mark_placement::copy()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the x-position of the mark.
 * \param x The new position.
 */
void bf::mark_placement::set_x_position( double x )
{
  m_x = x;
} // mark_placement::set_x_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the y-position of the mark.
 * \param y The new position.
 */
void bf::mark_placement::set_y_position( double y )
{
  m_y = y;
} // mark_placement::set_y_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the position of the mark
 * \param x The new x-position.
 * \param y The new y-position.
 */
void bf::mark_placement::set_position( double x, double y )
{
  set_x_position(x);
  set_y_position(y);
} // mark_placement::set_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the x-position of the mark.
 */
double bf::mark_placement::get_x_position() const
{
  return m_x;
} // mark_placement::get_x_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the y-position of the mark.
 */
double bf::mark_placement::get_y_position() const
{
  return m_y;
} // mark_placement::get_y_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the width of the box around the mark.
 * \param w The width.
 */
void bf::mark_placement::set_width( double w )
{
  m_width = std::max( .0, w );
} // mark_placement::set_width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the height of the box around the mark.
 * \param h The height.
 */
void bf::mark_placement::set_height( double h )
{
  m_height = std::max( .0, h );
} // mark_placement::set_height()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the size of the box around the mark.
 * \param w The width.
 * \param h The height.
 */
void bf::mark_placement::set_size( double w, double h )
{
  set_width(w);
  set_height(h);
} // mark_placement::set_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the width of the box around the mark.
 */
double bf::mark_placement::get_width() const
{
  return m_width;
} // mark_placement::get_width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the height of the box around the mark.
 */
double bf::mark_placement::get_height() const
{
  return m_height;
} // mark_placement::get_height()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the name of the function called when there is a collision in the
 *        box around the mark.
 * \param f The name of the function.
 */
void bf::mark_placement::set_collision_function( const std::string& f )
{
  m_collision_function = f;
} // mark_placement::set_collision_function()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the name of the function called when there is a collision in the
 *        box around the mark.
 */
const std::string& bf::mark_placement::get_collision_function() const
{
  return m_collision_function;
} // mark_placement::get_collision_function()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the visibility of the mark
 * \param v Tell if the mark is visible or not.
 */
void bf::mark_placement::set_visibility( bool v )
{
  m_visible = v;
} // mark_placement::set_visibility()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the visibility of the mark
 */
bool bf::mark_placement::is_visible() const
{
  return m_visible;
} // mark_placement::is_visible()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the depth-position of the mark.
 * \param p The new position.
 */
void bf::mark_placement::set_depth_position( int p )
{
  m_depth = p;
} // mark_placement::set_depth_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the depth-position of the mark.
 */
int bf::mark_placement::get_depth_position() const
{
  return m_depth;
} // mark_placement::get_depth_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the angle of the mark.
 * \param a The new angle.
 */
void bf::mark_placement::set_angle( double a )
{
  m_angle = a;
} // mark_placement::set_angle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the angle of the mark.
 */
double bf::mark_placement::get_angle() const
{
  return m_angle;
} // mark_placement::get_angle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the mark concerned by this placement.
 */
const bf::mark* bf::mark_placement::get_mark() const
{
  return m_mark;
} // mark_placement::get_mark()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the easing for the x-position.
 * \param e The easing.
 */
void bf::mark_placement::set_x_easing( const bear::easing& e )
{
  m_x_easing = e;
} // mark_placement::set_x_easing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the easing for the x-position.
 */
const bear::easing& bf::mark_placement::get_x_easing() const
{
  return m_x_easing;
} // mark_placement::get_x_easing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the easing for the y-position.
 * \param e The easing.
 */
void bf::mark_placement::set_y_easing( const bear::easing& e )
{
  m_y_easing = e;
} // mark_placement::set_y_easing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the easing for the y-position.
 */
const bear::easing& bf::mark_placement::get_y_easing() const
{
  return m_y_easing;
} // mark_placement::get_y_easing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the easing for the width.
 * \param e The easing.
 */
void bf::mark_placement::set_width_easing( const bear::easing& e )
{
  m_width_easing = e;
} // mark_placement::set_width_easing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the easing for the width.
 */
const bear::easing& bf::mark_placement::get_width_easing() const
{
  return m_width_easing;
} // mark_placement::get_width_easing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the easing for the height.
 * \param e The easing.
 */
void bf::mark_placement::set_height_easing( const bear::easing& e )
{
  m_height_easing = e;
} // mark_placement::set_height_easing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the easing for the height.
 */
const bear::easing& bf::mark_placement::get_height_easing() const
{
  return m_height_easing;
} // mark_placement::get_height_easing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the easing for the angle.
 * \param e The easing.
 */
void bf::mark_placement::set_angle_easing( const bear::easing& e )
{
  m_angle_easing = e;
} // mark_placement::set_angle_easing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the easing for the angle.
 */
const bear::easing& bf::mark_placement::get_angle_easing() const
{
  return m_angle_easing;
} // mark_placement::get_angle_easing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compile the mark_placement.
 * \param f The file in which we compile.
 */
void bf::mark_placement::compile( compiled_file& f ) const
{
  f << m_x << m_x_easing.get_function() << m_x_easing.get_direction()
    << m_y << m_y_easing.get_function() << m_y_easing.get_direction()
    << m_width
    << m_width_easing.get_function() << m_width_easing.get_direction()
    << m_height
    << m_height_easing.get_function() << m_height_easing.get_direction()
    << m_depth
    << m_angle
    << m_angle_easing.get_function() << m_angle_easing.get_direction()
    << m_visible << m_collision_function;
} // mark_placement::compile()
