/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::runtime_settings_layer class.
 * \author Julien Jorge
 */
#include "generic_items/layer/runtime_settings_layer.hpp"

#include "generic_items/debug/runtime_setting.hpp"

#include "gui/callback_function.hpp"
#include "gui/horizontal_flow.hpp"
#include "gui/static_text.hpp"
#include "gui/text_input.hpp"

#include <claw/multi_type_map_visitor.hpp>
#include <claw/string_algorithm.hpp>

#include <boost/bind.hpp>
#include <boost/function.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief 
 */
bear::runtime_settings_layer::setting_visitor::setting_visitor
( runtime_settings_layer& layer )
  : m_layer(layer)
{

} // runtime_settings_layer::setting_visitor::setting_visitor()

/*----------------------------------------------------------------------------*/
/**
 * \brief 
 */
template<typename T>
void bear::runtime_settings_layer::setting_visitor::operator()
  ( std::string name, T var ) const
{
  m_layer.process_setting( name, var );
} // runtime_settings_layer::setting_visitor::operator()()




/*----------------------------------------------------------------------------*/
const bear::gui::size_type bear::runtime_settings_layer::s_margin(10);
const bear::gui::color_type
bear::runtime_settings_layer::s_cursor_color( claw::graphic::red_pixel );

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param f The font to use in the labels.
 */
bear::runtime_settings_layer::runtime_settings_layer( visual::font f )
  : m_toggle_key(input::keyboard::kc_F9), m_font(f), m_root_component(NULL),
    m_pattern(NULL), m_settings(NULL)
{

} // runtime_settings_layer::runtime_settings_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::runtime_settings_layer::~runtime_settings_layer()
{
  clear();
} // runtime_settings_layer::~runtime_settings_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief 
 */
void bear::runtime_settings_layer::render( scene_element_list& e ) const
{
  if ( m_root_component != NULL )
    m_root_component->render( e );
} // runtime_settings_layer::render()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the layer that a keyboard key has been pressed.
 * \param key The value of the pressed key.
 */
bool bear::runtime_settings_layer::key_pressed( const input::key_info& key )
{
  bool result = true;

  if ( key.get_code() == m_toggle_key )
    {
      if ( m_root_component == NULL )
        {
          build_components();
          update();
        }
      else
        clear();
    }
  else
    result = false;

  return result;
} // runtime_settings_layer::key_pressed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Creates the components that will display the settings, but does not
 *        fill them.
 */
void bear::runtime_settings_layer::build_components()
{
  m_root_component = new gui::visual_component();
  m_root_component->set_size( get_size() );

  gui::static_text* const pattern_text = new gui::static_text( m_font );
  pattern_text->set_text( "Pattern:" );
  pattern_text->set_auto_size(true);
  pattern_text->set_position( s_margin, s_margin );
  m_root_component->insert( pattern_text );

  m_pattern = new gui::text_input( m_font, s_cursor_color );
  m_pattern->set_position( pattern_text->right() + s_margin, s_margin );
  m_pattern->set_size
    ( get_size().x - 3 * s_margin - pattern_text->width(),
      pattern_text->height() );
  m_pattern->set_text( "*" );
  m_pattern->set_border_color( claw::graphic::black_pixel );
  m_root_component->insert( m_pattern );

  m_settings = new gui::horizontal_flow();
  
  m_settings->set_position( s_margin, pattern_text->top() + s_margin );
  m_settings->set_border_color( claw::graphic::white_pixel );

  m_settings->set_size
    ( m_root_component->width() - 2 * s_margin,
      m_root_component->height() - pattern_text->top() - 2 * s_margin );
  m_root_component->insert( m_settings );

  set_root_component( m_root_component );
} // runtime_settings_layer::build_components()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destroys all the components created by build_components()
 */
void bear::runtime_settings_layer::clear()
{
  set_root_component( NULL );

  delete m_root_component;
  m_root_component = NULL;
  m_pattern = NULL;
  m_settings = NULL;
} // runtime_settings_layer::clear()

/*----------------------------------------------------------------------------*/
/**
 * \brief Updates the visible settings according to the current pattern.
 */
void bear::runtime_settings_layer::update()
{
  const setting_visitor visitor( *this );
  claw::multi_type_map_visitor process;

  process.run( g_global_settings, visitor );
} // runtime_settings_layer::update()

/*----------------------------------------------------------------------------*/
/**
 * \brief 
 */
template<typename T>
void bear::runtime_settings_layer::process_setting
( std::string name, T* var ) const
{
  if ( !claw::text::glob_match( m_pattern->get_text(), name ) )
    return;

  gui::visual_component* const component = new gui::visual_component();
  component->set_size( get_size() );

  gui::static_text* const label = new gui::static_text( m_font );
  label->set_text( name );
  label->set_auto_size( true );
  component->insert( label );
  
  gui::text_input* const value = new gui::text_input( m_font, s_cursor_color );
  std::ostringstream oss;
  oss << *var;
  value->set_text( oss.str() );
  value->set_size( label->get_size() );
  value->set_position( label->right() + s_margin, label->bottom() );
  value->set_border_color( claw::graphic::black_pixel );

  value->add_enter_callback
    ( gui::callback_function< boost::function<void ()> >
      ( boost::bind
        ( &runtime_settings_layer::on_enter<T>, this, var, value ) ) );
  component->insert( value );

  component->fit();

  m_settings->insert( component );
} // runtime_settings_layer::process_setting()

/*----------------------------------------------------------------------------*/
/**
 * \brief 
 */
template<typename T>
void bear::runtime_settings_layer::on_enter
( T* var, gui::text_input* value ) const
{
  std::istringstream iss( value->get_text() );
  iss >> *var;
} // runtime_settings_layer::on_enter()
