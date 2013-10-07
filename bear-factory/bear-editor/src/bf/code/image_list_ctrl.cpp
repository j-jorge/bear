/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::image_list_ctrl class.
 * \author Julien Jorge
 */
#include "bf/image_list_ctrl.hpp"

#include "bf/image_pool.hpp"

#include <wx/dcbuffer.h>
#include <wx/image.h>
#include <wx/sizer.h>
#include <wx/tokenzr.h>

#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
const wxSize bf::image_list_ctrl::s_margin( 10, 50 );



/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The window containing this view.
 *
 */
bf::image_list_ctrl::list_view::list_view( image_list_ctrl& parent )
  : wxWindow(&parent, wxID_ANY), m_parent(parent)
{

} // image_list_ctrl::list_view::list_view()

/*----------------------------------------------------------------------------*/
/**
 * \brief The user have clicked the window.
 * \param event The event.
 */
void bf::image_list_ctrl::list_view::on_left_up( wxMouseEvent& event )
{
  m_parent.select_item( event.GetPosition() - GetPosition() );
} // image_list_ctrl::list_view::on_left_up()

/*----------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(bf::image_list_ctrl::list_view, wxWindow)
  EVT_LEFT_UP(bf::image_list_ctrl::list_view::on_left_up)
END_EVENT_TABLE()




/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param parent The window containing this list.
 * \param pool The image pool to use.
 */
bf::image_list_ctrl::image_list_ctrl( wxWindow& parent, image_pool* pool )
: wxPanel(&parent), m_selection(0), m_image_pool(pool)
{
  create_controls();
} // image_list_ctrl::image_list_ctrl()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the list of images.
 * \param img The new list of images.
 */
void bf::image_list_ctrl::set_list( const std::list<wxString>& img )
{
  wxString current_selection = get_selection();

  m_image = img;
  set_selection(current_selection);

  set_scrollbar_values();
  render();
} // image_list_ctrl::set_list()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the current selection.
 * \param i The index of the selected image.
 */
void bf::image_list_ctrl::set_selection( int i )
{
  if ( i >= (int)m_image.size() )
    m_selection = (int)m_image.size() - 1;
  else
    m_selection = i;

  render();
} // image_list_ctrl::set_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the current selection.
 * \param s The name of the selected image.
 */
void bf::image_list_ctrl::set_selection( const wxString& s )
{
  int i=0;
  bool found = false;
  std::list<wxString>::const_iterator it;

  for (it=m_image.begin(); !found && (it!=m_image.end()); ++it)
    if ( *it == s )
      found = true;
    else
      ++i;

  if (found)
    set_selection(i);
  else
    set_selection(-1);
} // image_list_ctrl::set_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the string of the current selection.
 */
wxString bf::image_list_ctrl::get_selection() const
{
  wxString result;

  if ( m_selection >= 0)
    if ( m_selection < (int)m_image.size() )
      {
        std::list<wxString>::const_iterator it = m_image.begin();
        std::advance(it, m_selection);
        result = *it;
      }

  return result;
} // image_list_ctrl::get_selection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the controls of this window.
 */
void bf::image_list_ctrl::create_controls()
{
  m_image_list = new list_view(*this);
  m_bar = new wxScrollBar
    (this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSB_VERTICAL );
  wxBoxSizer* sizer = new wxBoxSizer( wxHORIZONTAL );

  sizer->Add( m_image_list, 1, wxEXPAND );
  sizer->Add( m_bar, 0, wxEXPAND );

  SetSizer(sizer);
} // image_list_ctrl::create_controls()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the position and the range of the scrollbar.
 */
void bf::image_list_ctrl::set_scrollbar_values()
{
  const wxSize thumb_size = image_pool::s_thumb_size;

  // count of images in the width of the list
  const int cw =
    (m_image_list->GetSize().x - s_margin.x) / (thumb_size.x + s_margin.x);

  // count of images in the height of the list
  const int ch =
    (m_image_list->GetSize().y - s_margin.y) / (thumb_size.y + s_margin.y);

  // count of image lines
  int lines = 0;
  int pos = 0;

  if ( cw == 0 )
    {
      lines = 1;
      pos = 0;
    }
  else
    {
      if (m_selection > 0)
        pos = m_selection / cw;

      lines = m_image.size() / cw;

      if ( (m_image.size() % cw) != 0 )
        ++lines;
    }

  m_bar->SetScrollbar( pos, ch, lines, ch, true );
} // image_list_ctrl::set_scrollbar_values()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the list.
 */
void bf::image_list_ctrl::render()
{
  if( IsShown() )
    {
      wxBufferedPaintDC dc( m_image_list );

      dc.SetBackground(*wxBLACK_BRUSH);
      dc.Clear();

      dc.SetBrush(*wxTRANSPARENT_BRUSH);

      dc.SetFont
        ( wxFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL,
                 wxFONTWEIGHT_NORMAL) );

      render_list(dc);
    }
} // image_list_ctrl::render()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the list of images.
 * \param dc The device context on which we draw.
 */
void bf::image_list_ctrl::render_list( wxDC& dc )
{
  const wxSize thumb_size = image_pool::s_thumb_size;

  // count of images in the width of the list
  const int cw =
    (m_image_list->GetSize().x - s_margin.x) / (thumb_size.x + s_margin.x);

  std::list<wxString>::iterator it = m_image.begin();
  const std::list<wxString>::iterator eit = m_image.end();

  int i = m_bar->GetThumbPosition() * cw;
  std::advance( it, i );

  // x, x is ok
  wxPoint pos( s_margin.x, s_margin.x );

  for ( ; (it!=eit) && (pos.y < m_image_list->GetSize().y); ++it, ++i )
    {
      render_name(dc, *it, pos, i);
      if ( m_image_pool )
        render_thumb(dc, m_image_pool->get_thumbnail(*it), pos, i);
    }
} // image_list_ctrl::render_list()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the name of an image.
 * \param dc The device context on which we draw.
 * \param name The name of the image.
 * \param pos The position where the thumbnail is drawn.
 * \param i the index of the thumbnail.
 */
void bf::image_list_ctrl::render_name
( wxDC& dc, const wxString& name, wxPoint& pos, int i ) const
{
  const wxSize thumb_size = image_pool::s_thumb_size;

  if ( i==m_selection )
    dc.SetTextForeground(*wxRED);
  else
    dc.SetTextForeground(*wxWHITE);

  wxString formatted_name;
  wxStringTokenizer tokenizer(name, wxT("/\\"));

  while ( tokenizer.HasMoreTokens() )
    {
      const wxString token( tokenizer.GetNextToken() );
      const wxString candidate
        ( formatted_name.IsEmpty() ? token :
          (formatted_name + wxT("/") + token) );

      if ( dc.GetTextExtent(candidate).x > thumb_size.x )
        {
          formatted_name += wxT("/\n") + token;

          while ( tokenizer.HasMoreTokens() )
            formatted_name += wxT("/") + tokenizer.GetNextToken();
        }
      else
        formatted_name = candidate;
    }

  dc.DrawText( formatted_name, pos.x, pos.y + thumb_size.y + s_margin.x );
} // image_list_ctrl::render_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render an image.
 * \param dc The device context on which we draw.
 * \param bmp The thumbnail to render.
 * \param pos (in) The position where the thumbnail is drawn. (out) The position
 *            of the next thumbnail.
 * \param i the index of the thumbnail.
 */
void bf::image_list_ctrl::render_thumb
( wxDC& dc, wxBitmap bmp, wxPoint& pos, int i ) const
{
  const wxSize thumb_size = image_pool::s_thumb_size;

  if ( bmp.IsOk() )
    {
      wxMemoryDC mem_dc(bmp);
      dc.Blit( pos.x, pos.y, bmp.GetWidth(), bmp.GetHeight(),
               &mem_dc, 0, 0, wxCOPY, true );
    }

  render_border(dc, pos, wxSize(bmp.GetWidth(), bmp.GetHeight()), i);

  pos.x += thumb_size.x + s_margin.x;

  if ( pos.x + thumb_size.x > m_image_list->GetSize().x )
    {
      pos.x = s_margin.x;
      pos.y += thumb_size.y + s_margin.y;
    }
} // image_list_ctrl::render_thumb()

/*----------------------------------------------------------------------------*/
/**
 * \brief Render the border around the thumbnail.
 * \param dc The device context on which we draw.
 * \param pos The position of the thumbnail.
 * \param s The size of thumbnail.
 * \param i the index of the thumbnail.
 */
void bf::image_list_ctrl::render_border
( wxDC& dc, const wxPoint& pos, const wxSize& s, int i ) const
{
  wxPoint p[4];

  p[0] = wxPoint( pos.x-1, pos.y-1 );
  p[1] = wxPoint( pos.x + s.x, p[0].y );
  p[2] = wxPoint( p[1].x, pos.y + s.y );
  p[3] = wxPoint( p[0].x, p[2].y );

  if ( i == m_selection )
    dc.SetPen( *wxRED_PEN );
  else
    dc.SetPen( *wxGREEN_PEN );

  dc.DrawPolygon(4, p);
} // image_list_ctrl::render_border()

/*----------------------------------------------------------------------------*/
/**
 * \brief Select the item at a given position.
 * \param pos The position where we pick the item.
 */
void bf::image_list_ctrl::select_item( const wxPoint& pos )
{
  const wxSize thumb_size = image_pool::s_thumb_size;

  // count of images in the width of the list
  const int cw =
    (m_image_list->GetSize().x - s_margin.x) / (thumb_size.x + s_margin.x);

  int i =
    ( m_bar->GetThumbPosition() + pos.y / (thumb_size.y + s_margin.y) ) * cw;

  i += (pos.x - s_margin.x) / (thumb_size.x + s_margin.x);

  set_selection(i);
} // image_list_ctrl::select_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Draw the content of the window.
 * \param event The paint event.
 */
void bf::image_list_ctrl::on_paint( wxPaintEvent& WXUNUSED(event) )
{
  render();
} // image_list_ctrl::on_paint()

/*----------------------------------------------------------------------------*/
/**
 * \brief The window is resized.
 * \param event The paint event.
 */
void bf::image_list_ctrl::on_size( wxSizeEvent& event )
{
  set_scrollbar_values();
  event.Skip();
} // image_list_ctrl::on_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Answer to a mouse wheel event.
 * \param event The event.
 */
void bf::image_list_ctrl::on_wheel( wxMouseEvent& event )
{
  if ( event.GetWheelRotation() > 0 )
    m_bar->SetThumbPosition( m_bar->GetThumbPosition() - 1 );
  else if ( event.GetWheelRotation() < 0 )
    m_bar->SetThumbPosition( m_bar->GetThumbPosition() + 1 );
} // image_list_ctrl::on_wheel()

/*----------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(bf::image_list_ctrl, wxPanel)
  EVT_PAINT(bf::image_list_ctrl::on_paint)
  EVT_SIZE(bf::image_list_ctrl::on_size)
  EVT_MOUSEWHEEL(bf::image_list_ctrl::on_wheel)
END_EVENT_TABLE()
