/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::level class.
 * \author Julien Jorge
 */
#include "engine/level.hpp"

#include <algorithm>
#include <claw/functional.hpp>

#include "engine/game.hpp"
#include "engine/level_globals.hpp"
#include "engine/layer/gui_layer.hpp"
#include "engine/variable/base_variable.hpp"
#include "universe/const_item_handle.hpp"

#include "debug/scoped_time_measure.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param name The name of the level.
 * \param filename The filename of the level.
 * \param level_size The size of the level.
 * \param level_music The default music of the level.
 * \param shared_resources A level_globals from which we can take the resources
 *        instead of building new ones in the new level.
 */
bear::engine::level::level
( const std::string& name, const std::string& filename,
  const universe::size_box_type& level_size, const std::string& level_music,
  const level_globals* shared_resources )
  : m_name(name),  m_filename(filename), m_camera(NULL),
    m_level_size(level_size),
    m_level_globals( new level_globals(shared_resources) ),
    m_music(level_music), m_music_id(0), m_paused(0),
    m_overview_activated(false)
{
  set_pause();

  if ( !m_music.empty() )
    m_level_globals->load_sound(m_music);
} // level::level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::engine::level::~level()
{
  stop();
  clear();
} // level::~level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Start the level.
 */
void bear::engine::level::start()
{
  m_level_globals->freeze();

  unset_pause();

  for (unsigned int i=0; i!=m_layers.size(); ++i)
    m_layers[i]->start();

  play_music();
} // level::start()

/*----------------------------------------------------------------------------*/
/**
 * \brief Stop the level.
 */
void bear::engine::level::stop()
{
  stop_music();
} // level::stop()

/*----------------------------------------------------------------------------*/
/**
 * \brief Progress all layers in the active area.
 * \param elapsed_time Elapsed time since the last call.
 */
void bear::engine::level::progress( universe::time_type elapsed_time )
{
  BEAR_CREATE_SCOPED_TIMELOG( std::string("progress level") );

  if ( !is_paused() )
    {
      region_type active_regions;

      get_active_regions( active_regions );

      for (unsigned int i=0; i!=m_layers.size(); ++i)
        {
          region_type areas(active_regions);
          get_layer_region(i, areas);
          m_layers[i]->update( areas, elapsed_time );
        }

      if ( m_ears != universe::item_handle(NULL) )
        m_level_globals->set_ears_position( m_ears->get_center_of_mass() );
      else
        m_level_globals->set_ears_position( get_camera_center() );

      set_sound_distance_unit();
    }

  m_gui.progress( elapsed_time );

  m_progress_done_signal();
} // level::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adds a function to call when the progress of the level is over.
 * \param f The function to call.
 */
boost::signals::connection
bear::engine::level::on_progress_done( boost::function<void ()> f )
{
  return m_progress_done_signal.connect( f );
} // bear::engine::level::on_progress_done()

/*----------------------------------------------------------------------------*/
/**
 * \brief Draw the visible part of the level on the screen.
 * \param screen The screen on which we draw.
 */
void bear::engine::level::render( visual::screen& screen ) const
{
  BEAR_CREATE_SCOPED_TIMELOG( std::string("render level") );

  render_layers(screen);
  render_gui(screen);
} // level::render()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the position and the scale factor of a scene_element relatively to
 *        the screen.
 * \param e The scene element in the level coordinates.
 */
bear::visual::scene_element bear::engine::level::element_to_screen_coordinates
( const visual::scene_element& e ) const
{
  const universe::rectangle_type area( get_camera_focus() );

  return element_to_screen_coordinates
    ( e, area.bottom_left(),
      (double)game::get_instance().get_screen_size().x / get_camera_size().x,
      (double)game::get_instance().get_screen_size().y / get_camera_size().y );
} // level::element_to_screen_coordinates()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a big picture of the level.
 * \param screen The screen on which we do the captured drawings.
 * \param img The image in which we save the resulting capture.
 */
void bear::engine::level::shot
( visual::screen& screen, claw::graphic::image& img ) const
{
  img.set_size( get_size().x, get_size().y );
  claw::logger << claw::log_verbose << "Level image allocated." << std::endl;

  claw::graphic::image tmp(screen.get_size().x, screen.get_size().y);
  const double ratio = 1;
  const visual::color_type c = screen.get_background_color();
  screen.set_background_color( claw::graphic::white_pixel );

  for ( unsigned int y=0; y<img.height(); y+=tmp.height() )
    for ( unsigned int x=0; x<img.width(); x+=tmp.width() )
      {
        const universe::rectangle_type area
          ( x, y, x+screen.get_size().x, y+screen.get_size().y );

        claw::logger << claw::log_verbose << "rendering level x=" << x
                     << " y=" << y << std::endl;

        screen.begin_render();
        for (unsigned int i=0; i!=m_layers.size(); ++i)
          if ( m_layers[i]->get_size() == get_size() )
            {
              // get four times the area covered by the camera
              region_type r;
              add_region( r, area, area.size() );

              universe::rectangle_type active( r.front() );
              std::list<scene_visual> visuals;

              m_layers[i]->get_visual( visuals, active );
              visuals.sort( scene_visual::z_position_compare() );

              render( visuals, area.bottom_left(), screen, ratio, ratio );
            }
        screen.end_render();

        screen.shot(tmp);
        tmp.flip();
        img.partial_copy(tmp, claw::math::coordinate_2d<int>(x, y));
      }

  img.flip();
  screen.set_background_color(c);
} // level::shot()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the rendering of the level in overview mode.
 * \param b True if the overview mode is activated.
 */
void bear::engine::level::set_overview_activated( bool b )
{
  m_overview_activated = b;
} // level::set_overview_activated()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the rendering of the level is in overview mode.
 */
bool bear::engine::level::get_overview_activated() const
{
  return m_overview_activated;
} // level::get_overview_activated()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the size of the level.
 */
const bear::universe::size_box_type& bear::engine::level::get_size() const
{
  return m_level_size;
} // level::get_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the number of layers.
 */
unsigned int bear::engine::level::get_depth() const
{
  return m_layers.size();
} // level::get_depth()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the name of the level.
 */
const std::string& bear::engine::level::get_name() const
{
  return m_name;
} // level::get_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the filename of the level.
 */
const std::string& bear::engine::level::get_filename() const
{
  return m_filename;
} // level::get_filename()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the globals resources of the level.
 */
bear::engine::level_globals& bear::engine::level::get_globals()
{
  return *m_level_globals;
} // level::get_globals()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the item to use as the camera.
 * \param cam The camera.
 */
void bear::engine::level::set_camera( base_item& cam )
{
  m_camera = cam;
} // level::set_camera()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add an item around which we want to call progress().
 * \param item The item.
 */
void bear::engine::level::add_interest_around( const base_item* item )
{
  add_interest_around
    ( item,
      universe::size_box_type
      ( game::get_instance().get_active_area_margin(),
        game::get_instance().get_active_area_margin() ) );
} // level::add_interest_around()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add an item around which we want to call progress().
 * \param item The item.
 * \param s The size of the active area around the item.
 */
void bear::engine::level::add_interest_around
( const base_item* item, const universe::size_box_type& s )
{
  if ( item != NULL )
    m_activity[universe::const_item_handle(item)] = s;
} // level::add_interest_around()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a layer in front of the others.
 * \param the_layer The layer to add.
 * \pre the_layer != NULL
 */
void bear::engine::level::push_layer( layer* the_layer )
{
  CLAW_PRECOND( the_layer != NULL );
  m_layers.push_back( the_layer );
  the_layer->set_level(*this);
} // level::push_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a layer in front of the layers of the interface.
 * \param the_layer The layer to add.
 * \pre the_layer != NULL
 */
void bear::engine::level::push_layer( gui_layer* the_layer )
{
  CLAW_PRECOND( the_layer != NULL );

  m_gui.push_layer(the_layer);

  the_layer->set_level(*this);
  the_layer->pre_cache();
  the_layer->build();
} // level::push_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets an iterator on the beginning of the sequence of layers.
 */
bear::engine::level::layer_iterator bear::engine::level::layer_begin()
{
  return m_layers.begin();
} // level::layer_begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets an iterator on the end of the sequence of layers.
 */
bear::engine::level::layer_iterator bear::engine::level::layer_end()
{
  return m_layers.end();
} // level::layer_end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets an iterator on the beginning of the sequence of layers.
 */
bear::engine::level::const_layer_iterator
bear::engine::level::layer_begin() const
{
  return m_layers.begin();
} // level::layer_begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets an iterator on the end of the sequence of layers.
 */
bear::engine::level::const_layer_iterator bear::engine::level::layer_end() const
{
  return m_layers.end();
} // level::layer_end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Pause the level.
 */
void bear::engine::level::set_pause()
{
  m_level_globals->pause_audio();
  ++m_paused;
} // level::set_pause()

/*----------------------------------------------------------------------------*/
/**
 * \brief Unpause the level.
 *
 * The level is unpaused if the number of calls to this method is greater or the
 * number of calls to set_pause().
 */
void bear::engine::level::unset_pause()
{
  if ( m_paused != 0 )
    {
      --m_paused;

      if ( !is_paused() )
        m_level_globals->resume_audio();
    }
  else
    claw::logger << claw::log_warning << "level::unset_pause: not paused."
                 << std::endl;
} // level::unset_pause()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the level is paused.
 */
bool bear::engine::level::is_paused() const
{
  return m_paused != 0;
} // level::is_paused()

/*----------------------------------------------------------------------------*/
/**
 * \brief Play the default music of the level.
 */
void bear::engine::level::play_music()
{
  if ( m_level_globals->sound_exists( m_music ) )
    m_music_id = m_level_globals->play_music( m_music );
} // level::play_music()

/*----------------------------------------------------------------------------*/
/**
 * \brief Stop the music of the level.
 * \param fade_duration The length of the fade out of the music.
 */
void bear::engine::level::stop_music( universe::time_type fade_duration )
{
  m_level_globals->stop_music( m_music_id, fade_duration );
} // level::stop_music()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the current camera.
 */
bear::universe::item_handle bear::engine::level::get_camera()
{
  return m_camera;
} // level::get_camera()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the size of the camera.
 */
bear::universe::size_box_type bear::engine::level::get_camera_size() const
{
  if ( m_camera == NULL )
    return game::get_instance().get_screen_size();
  else
    return m_camera->get_size();
} // level::get_camera_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the focus of the camera.
 */
bear::universe::rectangle_type bear::engine::level::get_camera_focus() const
{
  if ( m_camera == NULL )
    return universe::rectangle_type
      ( 0, 0, game::get_instance().get_screen_size().x,
        game::get_instance().get_screen_size().y );
  else
    return m_camera->get_bounding_box();
} // level::get_camera_focus()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the position of the center of the camera.
 */
bear::universe::position_type bear::engine::level::get_camera_center() const
{
  if ( m_camera == NULL )
    return game::get_instance().get_screen_size() / 2;
  else
    return m_camera->get_center_of_mass();
} // level::get_camera_center()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the position of the ears in the sound system.
 * \param ears The item receiving the sounds.
 */
void bear::engine::level::set_ears( universe::item_handle ears )
{
  m_ears = ears;
} // set_ears()

/*----------------------------------------------------------------------------*/
/**
 * \brief Converts a position from the screen's coordinates into the coordinates
 *        of the level.
 * \param p The position in the screen's coordinates.
 */
bear::universe::position_type
bear::engine::level::screen_to_level( visual::position_type p ) const
{
  const visual::size_box_type screen_size
    ( game::get_instance().get_window_size() );
  const universe::rectangle_type camera_focus( get_camera_focus() );

  return universe::position_type
    ( camera_focus.left() + p.x * camera_focus.width() / screen_size.x,
      camera_focus.bottom() + p.y * camera_focus.height() / screen_size.y );
} // level::screen_to_level()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the value of a global variable.
 * \param val (in/out) The variable for which we want the value.
 */
void bear::engine::level::get_level_variable( base_variable& val ) const
{
  val.get_value_from(m_level_variables);
} // level::get_level_variable()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the value of a global variable.
 * \param val (in/out) The variable for which we set the value.
 */
void bear::engine::level::set_level_variable( const base_variable& val )
{
  val.assign_value_to(m_level_variables);
} // level::set_level_variable()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a given variable exists in the level.
 * \param val (in/out) The variable to check.
 */
bool
bear::engine::level::level_variable_exists( const base_variable& val ) const
{
  return val.exists(m_level_variables);
} // level::level_variable_exists()

/*----------------------------------------------------------------------------*/
/**
 * \brief Draw the visible part of the level layers on the screen.
 * \param screen The screen on which we draw.
 */
void bear::engine::level::render_layers( visual::screen& screen ) const
{
  universe::rectangle_type view;

  if ( m_overview_activated )
    view.set( 0, 0, get_size().x, get_size().y );
  else
    view = get_camera_focus();

  render_layers( screen, view );
} // level::render_layers()

/*----------------------------------------------------------------------------*/
/**
 * \brief Draw the visible part of the level layers on the screen.
 * \param screen The screen on which we draw.
 * \param view The region in the level from which the elements are taken.
 */
void bear::engine::level::render_layers
( visual::screen& screen, const universe::rectangle_type& view ) const
{
  const double r_x = (double)screen.get_size().x / view.width();
  const double r_y = (double)screen.get_size().y / view.height();

  for (unsigned int i=0; i!=m_layers.size(); ++i)
    {
      // get four times the area covered by the camera
      region_type r;
      add_region( r, view, get_camera_size() / 2 );

      universe::rectangle_type active( r.front() );
      get_layer_area(i, active); // the active area scaled in the layer

      std::list<scene_visual> visuals;
      m_layers[i]->get_visual( visuals, active );

      universe::rectangle_type area( view );
      get_layer_area(i, area);   // the camera scaled in the layer
      render( visuals, area.bottom_left(), screen, r_x, r_y );
    }
} // level::render_layers()

/*----------------------------------------------------------------------------*/
/**
 * \brief Draw the visible part of the level layers on the screen.
 * \param screen The screen on which we draw.
 */
void bear::engine::level::render_gui( visual::screen& screen ) const
{
  gui_layer_stack::scene_element_list vis;
  m_gui.render(vis);

  for ( ; !vis.empty(); vis.pop_front() )
    screen.render( vis.front() );
} // level::render_gui()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the visible sprites.
 * \param visuals The sprites to display and their coordinates in the world.
 * \param cam_pos The position of the camera.
 * \param screen The screen on which we draw.
 * \param r_w Ratio on the width of the sprites.
 * \param r_h Ratio on the height of the sprites.
 */
void bear::engine::level::render
( const std::list<scene_visual>& visuals,
  const universe::position_type& cam_pos, visual::screen& screen,
  double r_w, double r_h ) const
{
  std::list<scene_visual>::const_iterator it;

  for ( it=visuals.begin(); it!=visuals.end(); ++it )
    screen.render
      ( element_to_screen_coordinates(it->scene_element, cam_pos, r_w, r_h) );
} // level::render()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the position and the scale factor of a scene_visual relatively to
 *        the screen.
 * \param e The scene element in the level coordinates.
 * \param cam_pos The position of the camera.
 * \param r_w Ratio on the width of the elements.
 * \param r_h Ratio on the height of the elements.
 */
bear::visual::scene_element bear::engine::level::element_to_screen_coordinates
( const visual::scene_element& e, const universe::position_type& cam_pos,
  double r_w, double r_h ) const
{
  visual::scene_element result(e);

  // the y-axis of the screen is in reversed direction
  universe::position_type pos( result.get_position() - cam_pos );

  result.set_position(pos.x * r_w, pos.y * r_h);
  result.set_scale_factor
    ( e.get_scale_factor_x() * r_w, e.get_scale_factor_y() * r_h);

  return result;
} // level::element_to_screen_coordinates()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove all the layers.
 */
void bear::engine::level::clear()
{
  m_gui.clear();

  std::for_each
    ( m_layers.begin(), m_layers.end(), claw::delete_function<layer*>() );

  m_layers.clear();

  delete m_level_globals;
  m_level_globals = NULL;
} // layer::clear()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the repositioned region in a layer from the area in the
 *        level/action layer.
 * \param layer_index The index of the layer in which we want the repositioned
 *        region.
 * \param the_region (in) The region in the action layer, (out) the repositioned
 *        region.
 */
void bear::engine::level::get_layer_region
( unsigned int layer_index, region_type& the_region ) const
{
  region_type::iterator it;

  for (it=the_region.begin(); it!=the_region.end(); ++it)
    get_layer_area( layer_index, *it );
} // level::get_layer_region()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the repositioned area in a layer from the area in the level/action
 *        layer.
 * \param layer_index The index of the layer in which we want the repositioned
 *        area.
 * \param area (in) The area in the level, (out) the repositioned area.
 */
void bear::engine::level::get_layer_area
( unsigned int layer_index, universe::rectangle_type& area ) const
{
  if ( area.width() > m_layers[layer_index]->get_size().x )
    area.left(0);
  else if ( m_level_size.x > area.width() )
    area.left
      (( area.left() * ( m_layers[layer_index]->get_size().x - area.width() ) )
       / ( m_level_size.x - area.width() ));

  if ( area.height() > m_layers[layer_index]->get_size().y )
    area.bottom(0);
  else if ( m_level_size.y > area.height() )
    area.bottom
      (( area.bottom()
         * ( m_layers[layer_index]->get_size().y - area.height() ) )
       / ( m_level_size.y - area.height() ));
} // level::get_layer_area()

/*----------------------------------------------------------------------------*/
/**
 * \brief Find the active regions in the level.
 * \param active_regions (out) The active regions in the level.
 */
void
bear::engine::level::get_active_regions( region_type& active_regions )
{
  activity_map_type::iterator it(m_activity.begin());

  while ( it!=m_activity.end() )
    if ( it->first == NULL )
      {
        const activity_map_type::iterator tmp(it);
        ++it;
        m_activity.erase(tmp);
      }
    else
      {
        add_region
          ( active_regions, it->first->get_bounding_box(), it->second );
        ++it;
      }

  add_region
    ( active_regions, get_camera_focus(),
      universe::size_box_type
      ( game::get_instance().get_active_area_margin(),
        game::get_instance().get_active_area_margin() ) );
} // level::get_active_regions()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adds an active region centered on a point in a list of regions.
 * \param active_regions (out) The active regions.
 * \param r The bounding box of the item around which there is an activity.
 * \param s The size of the area around the center.
 */
void
bear::engine::level::add_region
( region_type& active_regions, const universe::rectangle_type& r,
  const universe::size_box_type& s ) const
{
  universe::size_box_type box_size( r.size() );
  const universe::position_type center
    ( r.left() + r.width() / 2, r.bottom() + r.height() / 2 );
  box_size.x += 2 * s.x;
  box_size.y += 2 * s.y;

  universe::position_type bottom_left(0, 0);

  // set bottom-left position so we stay in the level
  if (center.x >= box_size.x / 2)
    bottom_left.x = center.x - box_size.x / 2;
  else
    box_size.x -= box_size.x / 2 - center.x;

  if (center.y >= box_size.y / 2)
    bottom_left.y = center.y - box_size.y / 2;
  else
    box_size.y -= box_size.y / 2 - center.y;

  // set size for the same reason
  if (bottom_left.x + box_size.x >= get_size().x)
    box_size.x = get_size().x - bottom_left.x;

  if (bottom_left.y + box_size.y >= get_size().y)
    box_size.y = get_size().y - bottom_left.y;

  active_regions.push_front
    ( universe::rectangle_type(bottom_left, bottom_left + box_size) );
} // level::add_region()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sets the distance unit in the sound system according to the size of
 *        the screen and the size of the camera.
 */
void bear::engine::level::set_sound_distance_unit() const
{
  const universe::rectangle_type view( get_camera_focus() );
  const universe::size_box_type screen_size =
    game::get_instance().get_screen_size();

  const double u =
    std::max( view.width() / screen_size.x, view.height() / screen_size.y );

  m_level_globals->set_distance_unit( u );
} // level::set_sound_distance_unit()
