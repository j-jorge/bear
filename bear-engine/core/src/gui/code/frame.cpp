/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::gui::frame class.
 * \author Julien Jorge
 */
#include "gui/frame.hpp"

#include "visual/scene_writing.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::gui::frame::frame()
  : visual_component(), m_content_frame(new visual_component()), m_font_size(0)
{
  insert(m_content_frame);
} // frame::frame()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param title The title of the frame.
 */
bear::gui::frame::frame( const std::string& title )
  : visual_component(), m_content_frame(new visual_component()), m_font_size(0)
{
  insert(m_content_frame);
  set_title( title );
} // frame::frame()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param title The title of the frame.
 * \param f The font used to display the title.
 * \param font_size The size of the font.
 */
bear::gui::frame::frame
( const std::string& title, const visual::font& f, visual::size_type font_size )
  : visual_component(), m_content_frame(new visual_component())
{
  insert(m_content_frame);
  set_font(f);
  set_font_size(font_size);
  set_title( title );
} // frame::frame()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the title of the frame.
 * \param t The new title.
 */
void bear::gui::frame::set_title( const std::string& t )
{
  m_title = t;
  update_displayed_title();
} // frame::()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the title of the frame.
 */
const std::string& bear::gui::frame::get_title() const
{
  return m_title;
} // frame::get_title()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the font used to display the title.
 * \param f The font.
 */
void bear::gui::frame::set_font( const visual::font& f )
{
  m_font = f;
  set_font_size(m_font_size);
  update_displayed_title();
} // frame::set_font()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the font used to display the title.
 */
const bear::visual::font& bear::gui::frame::get_font() const
{
  return m_font;
} // frame::get_font()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the size applied to the font when rendering the title.
 * \param s The size.
 */
void bear::gui::frame::set_font_size( visual::size_type s )
{
  if ( s <= 0 )
    m_font_size = m_font.get_size();
  else
    m_font_size = s;
} // frame::set_font_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the size applied to the font when rendering the title.
 */
bear::visual::size_type bear::gui::frame::get_font_size() const
{
  return m_font_size;
} // frame::get_font_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Call this method when you close the frame.
 * \return True if the frame has been closed.
 */
bool bear::gui::frame::close()
{
  return on_close();
} // frame::close()

/*----------------------------------------------------------------------------*/
/**
 * \brief Method called before closing the frame.
 * \return True if the frame can be closed.
 */
bool bear::gui::frame::on_close()
{
  return true;
} // frame::on_close()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the component in which to add the children of the frame.
 */
bear::gui::visual_component& bear::gui::frame::get_content()
{
  return *m_content_frame;
} // frame::get_content()

/*----------------------------------------------------------------------------*/
/**
 * \brief Call this method to make the component fitting its children.
 * \param margin The margin around the content frame.
 */
void bear::gui::frame::fit( size_type margin )
{
  m_content_frame->fit( margin );

  set_size( std::max(m_content_frame->width(), m_displayed_title.get_width())
            + 2 * get_border_size(),
            m_content_frame->height() + compute_title_height()
            + 2 * get_border_size() );
} // frame::fit()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the scene elements of the component.
 * \param e (out) The scene elements.
 */
void bear::gui::frame::display( std::list<visual::scene_element>& e ) const
{
  visual::scene_writing t
    ( left() + get_border_size(),
      top() - compute_title_height() - get_border_size(), m_displayed_title );

  const double factor = m_font_size / m_font.get_line_spacing();
  t.set_scale_factor( factor, factor );

  e.push_back(t);
} // frame::display()

/*----------------------------------------------------------------------------*/
/**
 * \brief Method called after the component has been resized.
 */
void bear::gui::frame::on_resized()
{
  m_content_frame->set_size
    ( width() - 2 * get_border_size(),
      height() - 2 * get_border_size() - compute_title_height() );
} // frame::on_resized()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the scene elements for the background of the frame.
 * \param e (out) The scene elements.
 */
bear::visual::size_type bear::gui::frame::compute_title_height() const
{
  return
    (m_displayed_title.get_height() / m_font.get_size())
    * m_font_size;
} // frame::compute_title_height()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update the writing used to display the text.
 */
void bear::gui::frame::update_displayed_title()
{
  m_displayed_title.create( m_font, m_title );
} // frame::update_displayed_title()
