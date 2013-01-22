/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::path_trace class.
 * \author Julien Jorge
 */
#include "generic_items/path_trace.hpp"

#include "visual/scene_polygon.hpp"
#include <limits>

BASE_ITEM_EXPORT(path_trace, bear)

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::path_trace::path_trace()
: m_progress(&path_trace::progress_void),
  m_fill_color(claw::graphic::black_pixel), m_opacity(1), m_fade_out_speed(1),
  m_maximal_length( std::numeric_limits<universe::time_type>::max() ),
  m_tail_ratio(1), m_use_horizontal_axis(false)
{
  set_artificial(true);
  set_phantom(true);
  set_can_move_items(false);
} // path_trace::path_trace()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param ref The item to trace.
 */
bear::path_trace::path_trace( const base_item& ref )
  : m_progress(&path_trace::progress_alive),
    m_fill_color(claw::graphic::black_pixel), m_opacity(1), m_fade_out_speed(1),
    m_maximal_length( std::numeric_limits<universe::time_type>::max() ),
    m_tail_ratio(1), m_use_horizontal_axis(false)
{
  set_artificial(true);
  set_phantom(true);
  set_can_move_items(false);
  set_global( true );

  set_item( ref );
} // path_trace::path_trace()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the color of the trace.
 * \param c The new color.
 */
void bear::path_trace::set_fill_color( const visual::color_type& c )
{
  m_fill_color = c;
} // path_trace::set_fill_color()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if the trace uses the horizontal axis of the moving item
 *        for its computations, instead of the vertical axis.
 * \param b The new value of the flag.
 */
void bear::path_trace::set_use_horizontal_axis( bool b )
{
  m_use_horizontal_axis = b;
} // path_trace::set_use_horizontal_axis()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the ratio of the size of the end of the trace relatively to its
 *        head.
 * \param r The new ratio.
 * \remark Values of r lesser than zero are ignored.
 */
void bear::path_trace::set_tail_ratio( double r )
{
  if ( r >= 0 )
    m_tail_ratio = r;
} // path_trace::set_tail_ratio()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the item to trace.
 * \param ref The item to trace.
 */
void bear::path_trace::set_item( const base_item& ref )
{
  set_z_position(ref.get_z_position() - 1);
  set_bounding_box( ref.get_bounding_box() );
  set_movement_reference( &ref );

  m_previous_top.clear();
  m_previous_bottom.clear();
  m_previous_date.clear();

  m_date = 0;
  m_item = ref;
  m_progress = &path_trace::progress_alive;

  set_auto_axis();

  push_position();
} // path_trace::set_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the speed of the fade out of the trace when the traced item is
 *        dead.
 * \param s Units of opacity lost per second.
 */
void bear::path_trace::set_fade_out_speed( double s )
{
  m_fade_out_speed = (s > 0) ? s : 0;
} // path_trace::set_fade_out_speed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the maximal length of the trace.
 * \param length of the trace.
 */
void bear::path_trace::set_length( universe::time_type length )
{
  m_maximal_length = length;
} // path_trace::set_length()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do one step in the progression of the item.
 * \param elapsed_time Elapsed time since the last call.
 */
void bear::path_trace::progress( universe::time_type elapsed_time )
{
  super::progress(elapsed_time);

  m_date += elapsed_time;

  (this->*m_progress)( elapsed_time );

  update_length();
} // path_trace::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the visuals of this item.
 * \param visuals (out) The visuals.
 */
void
bear::path_trace::get_visual( std::list<engine::scene_visual>& visuals ) const
{
  CLAW_PRECOND( m_previous_bottom.size() == m_previous_top.size() );

  if ( m_previous_top.empty() )
    return;

  position_list::const_iterator bottom_it = m_previous_bottom.begin();
  position_list::const_iterator top_it = m_previous_top.begin();

  position_list::const_iterator next_top(top_it);
  ++next_top;

  std::size_t i( m_previous_top.size() );

  while ( next_top != m_previous_top.end() )
    {
      // The four corners of the polygon.
      const visual::position_type first_bottom( *bottom_it );
      ++bottom_it;
      const visual::position_type second_bottom( *bottom_it );

      const visual::position_type first_top( *top_it );
      ++top_it;
      const visual::position_type second_top( *top_it );

      next_top = top_it;
      ++next_top;

      --i;
      const std::vector<visual::position_type> p
        ( get_polygon
          ( i, first_bottom, second_bottom, first_top, second_top ) );

      if ( !p.empty() )
        {
          visual::scene_polygon e(0, 0, m_fill_color, p);
          e.get_rendering_attributes().set_opacity(m_opacity);
          visuals.push_back( engine::scene_visual(e) );
        }
    }
} // path_trace::get_visual()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update the position of the item.
 *
 * This function overrides the default move and place the trace such that the
 * last computed points are placed on the moving item.
 *
 * \param elapsed_time Elapsed time since the last call.
 */
void bear::path_trace::move( universe::time_type elapsed_time )
{
  if ( m_previous_bottom.empty() || (m_item == (physical_item*)NULL) )
    return;

  const universe::position_type last_point( m_previous_bottom.back() );

  if ( m_use_horizontal_axis )
    set_bottom_left
      ( get_left() + (m_item->get_left() - last_point.x),
        get_bottom() + (m_item->get_vertical_middle() - last_point.y) );
  else
    set_bottom_left
      ( get_left() + (m_item->get_horizontal_middle() - last_point.x),
        get_bottom() + (m_item->get_bottom() - last_point.y) );
} // path_trace::move()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do one step in the progression of this item, when there is no traced
 *        item.
 * \param elapsed_time Elapsed time since the last call.
 */
void bear::path_trace::progress_void( universe::time_type elapsed_time )
{
  // nothing to do
} // path_trace::progress_void()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do one step in the progression of this item, while the traced item is
 *        alive.
 * \param elapsed_time Elapsed time since the last call.
 */
void bear::path_trace::progress_alive( universe::time_type elapsed_time )
{
  if ( m_item == NULL )
    {
      m_progress = &path_trace::progress_dead;
      return;
    }

  push_position();

  set_bounding_box( get_bounding_box().join(m_item->get_bounding_box()) );
} // path_trace::progress_alive()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do one step in the progression of this item, once the traced item is
 *        dead.
 * \param elapsed_time Elapsed time since the last call.
 */
void bear::path_trace::progress_dead( universe::time_type elapsed_time )
{
  m_opacity -= m_fade_out_speed * elapsed_time;

  if ( m_opacity < 0 )
    kill();
} // path_trace::progress_dead()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update the length.
 */
void bear::path_trace::update_length()
{
  bool stop(false);

  while ( !stop )
    if ( m_previous_date.empty() )
      stop = true;
    else if ( m_previous_date.front() + m_maximal_length <= m_date )
      {
        m_previous_date.pop_front();
        m_previous_top.pop_front();
        m_previous_bottom.pop_front();
      }
    else
      stop = true;
} // path_trace::update_length()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute the polygon of a part of the trace.
 * \param i The index of the polygon relatively to the head of the trace.
 * \param first_bottom The first point stored on the bottom edge.
 * \param second_bottom The second point stored on the bottom edge.
 * \param first_top The first point stored on the top edge.
 * \param second_top The second point stored on the top edge.
 */
std::vector<bear::visual::position_type>
bear::path_trace::get_polygon
( std::size_t i, universe::position_type first_bottom,
  universe::position_type second_bottom, universe::position_type first_top,
  universe::position_type second_top ) const
{
  if ( m_use_horizontal_axis )
    return get_horizontal_polygon
      ( i, first_bottom, second_bottom, first_top, second_top );
  else
    return get_vertical_polygon
      ( i, first_bottom, second_bottom, first_top, second_top );
} // path_trace::get_polygon()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute the polygon of a part of the trace in the case where we store
 *        the points on the vertical axis.
 * \param i The index of the polygon relatively to the head of the trace.
 * \param first_bottom The first point stored on the bottom edge.
 * \param second_bottom The second point stored on the bottom edge.
 * \param first_top The first point stored on the top edge.
 * \param second_top The second point stored on the top edge.
 */
std::vector<bear::visual::position_type>
bear::path_trace::get_vertical_polygon
( std::size_t i, universe::position_type first_bottom,
  universe::position_type second_bottom, universe::position_type first_top,
  universe::position_type second_top ) const
{
  std::size_t count( m_previous_top.size() );
  std::vector<visual::position_type> p(4);

  // Adjust the height of the polygon according to the distance from the
  // head of the trace.
  const double first_dy
    ( ( (1 - m_tail_ratio) * (double)(i+1) / count )
      * (first_top.y - first_bottom.y) / 2 );
  const double second_dy
    ( ( (1 - m_tail_ratio) * (double)(i) / count )
      * (second_top.y - second_bottom.y) / 2 );

  p[0] = visual::position_type( first_bottom.x, first_bottom.y + first_dy );
  p[1] = visual::position_type( second_bottom.x, second_bottom.y + second_dy );
  p[2] = visual::position_type( second_top.x, second_top.y - second_dy );
  p[3] = visual::position_type( first_top.x, first_top.y - first_dy );

  return p;
} // path_trace::get_vertical_polygon()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute the polygon of a part of the trace in the case where we store
 *        the points on the horizontal axis.
 * \param i The index of the polygon relatively to the head of the trace.
 * \param first_left The first point stored on the left edge.
 * \param second_left The second point stored on the left edge.
 * \param first_right The first point stored on the right edge.
 * \param second_right The second point stored on the right edge.
 */
std::vector<bear::visual::position_type>
bear::path_trace::get_horizontal_polygon
( std::size_t i, universe::position_type first_left,
  universe::position_type second_left, universe::position_type first_right,
  universe::position_type second_right ) const
{
  std::size_t count( m_previous_top.size() );
  std::vector<visual::position_type> p(4);

  // Adjust the width of the polygon according to the distance from the
  // head of the trace.
  const double first_dx
    ( ( (1 - m_tail_ratio) * (double)(i+1) / count )
      * (first_right.x - first_left.x) / 2 );
  const double second_dx
    ( ( (1 - m_tail_ratio) * (double)(i) / count )
      * (second_right.x - second_left.x) / 2 );

  p[0] = visual::position_type( first_left.x + first_dx, first_left.y );
  p[1] = visual::position_type( second_left.x + second_dx, second_left.y );
  p[2] = visual::position_type( second_right.x - second_dx, second_right.y );
  p[3] = visual::position_type( first_right.x - first_dx, first_right.y );

  return p;
} // path_trace::get_horizontal_polygon()

/*----------------------------------------------------------------------------*/
/**
 * \brief Automatically sets the set_use_horizontal_axis flag in order to have
 *        the best rendering.
 * \param b The new value of the flag.
 */
void bear::path_trace::set_auto_axis()
{
  if ( m_item == (universe::physical_item*)NULL )
    return;

  const double vertical_angle_threshold( 3.14159 / 4 );
  const double pi_div_2( 3.14159 / 2 );
  const double a( m_item->get_system_angle() );

  if ( ( (pi_div_2 - vertical_angle_threshold <= a)
         && (a <= pi_div_2 + vertical_angle_threshold ) )
       || ( (-pi_div_2 - vertical_angle_threshold <= a)
            && (a <= -pi_div_2 + vertical_angle_threshold ) ) )
    set_use_horizontal_axis(false);
  else
    set_use_horizontal_axis(true);
} // path_trace::set_auto_axis()

/*----------------------------------------------------------------------------*/
/**
 * \brief Stores the position of the trace on m_item.
 */
void bear::path_trace::push_position()
{
  if ( m_item == NULL )
    return;

  if ( m_use_horizontal_axis )
    {
      m_previous_top.push_back( m_item->get_right_middle() );
      m_previous_bottom.push_back( m_item->get_left_middle() );
    }
  else
    {
      m_previous_top.push_back( m_item->get_top_middle() );
      m_previous_bottom.push_back( m_item->get_bottom_middle() );
    }

  m_previous_date.push_back( m_date );
} // path_trace::push_position()
