/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::image_pool class.
 * \author Julien Jorge
 */
#include "bf/image_pool.hpp"
#include "bf/path_configuration.hpp"
#include "bf/workspace.hpp"
#include "bf/scan_dir.hpp"
#include "bf/wx_facilities.hpp"

#include <claw/assert.hpp>
#include <claw/logger.hpp>
#include <claw/string_algorithm.hpp>

#include <wx/image.h>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param t The map in which we store the thumbnails.
 * \param r The directory from which the search started.
 */
bf::image_pool::load_thumb_func::load_thumb_func
( image_map& t, const std::string& r )
  : thumb(t), m_root(r)
{

} // image_pool::load_thumb_func::load_thumb_func()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load the thumbnail of an image.
 * \param path The path to the image.
 */
void bf::image_pool::load_thumb_func::operator()( const std::string& path )
{
  if ( path.find_last_of('!') != std::string::npos )
    return;

  wxBitmap img = load(path);
  const wxString image_name( std_to_wx_string( path.substr(m_root.size()) ) );

  thumb[image_name] = img;
} // image_pool::load_thumb_func::operator()()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load the thumbnail of an image.
 * \param path The path to the image.
 */
wxBitmap bf::image_pool::load_thumb_func::load( const std::string& path )
{
  wxImage img( std_to_wx_string(path) );

  if ( (img.GetWidth() > s_thumb_size.x)
       || (img.GetHeight() > s_thumb_size.y) )
    {
      int w, h;

      if( img.GetWidth() > img.GetHeight() )
        {
          w = s_thumb_size.x;
          h = (img.GetHeight() * w) / img.GetWidth();
        }
      else
        {
          h = s_thumb_size.y;
          w = (img.GetWidth() * h) / img.GetHeight();
        }

      img.Rescale(w, h);
    }

  return wxBitmap(img);
} // image_pool::load_thumb_func::load()




/*----------------------------------------------------------------------------*/
const wxSize bf::image_pool::s_thumb_size( 150, 150 );

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bf::image_pool::image_pool()
{
  wxImage::AddHandler( new wxPNGHandler() );
  wxImage::AddHandler( new wxJPEGHandler() );
  wxImage::AddHandler( new wxTGAHandler() );
} // image_pool::image_pool()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param w The workspace to load.
 */
bf::image_pool::image_pool( const std::string& w )
{
  m_workspace = w;
  
  wxImage::AddHandler( new wxPNGHandler() );
  wxImage::AddHandler( new wxJPEGHandler() );
  wxImage::AddHandler( new wxTGAHandler() );

  if ( path_configuration::get_instance().has_workspace( w ) )
    {
      workspace::path_list_const_iterator it;
      const workspace& work
        ( path_configuration::get_instance().get_workspace( w ) );
      
      for ( it = work.data_begin(); 
            it != work.data_end(); ++it )
        scan_directory(*it);
    }
} // image_pool::image_pool()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the image with a given name.
 * \param image_name The name of the image we want.
 */
wxBitmap bf::image_pool::get_image( const wxString& image_name ) const
{
  image_map::const_iterator it = m_image.find(image_name);

  if ( it == m_image.end() )
    {
      load_image_data(image_name);
      it = m_image.find(image_name);
    }
  else if ( !it->second.IsOk() )
    load_image_data(image_name);

  return it->second;
} // image_pool::get_image()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the thumbnail with a given name.
 * \param image_name The name of the thumbnail we want.
 */
wxBitmap bf::image_pool::get_thumbnail( const wxString& image_name ) const
{
  CLAW_PRECOND( m_thumbnail.find(image_name) != m_thumbnail.end() );

  return m_thumbnail.find(image_name)->second;
} // image_pool::get_thumbnail()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the entries of the spritepos file associated with a given image.
 * \param image_name The name of the image.
 */
bf::image_pool::spritepos_entries
bf::image_pool::get_spritepos_entries( const wxString& image_name ) const
{
  load_spritepos_file( wx_to_std_string(image_name) );

  const spritepos_map::const_iterator it = m_spritepos.find(image_name);

  if ( it == m_spritepos.end() )
    return spritepos_entries();
  else
    return it->second;
} // image_pool::get_spritepos_entries()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the rectangle of a given spritepos entry.
 * \param image_name The name of the image to which the spritepos is associated.
 * \param entry The name of the entry to return.
 */
claw::math::rectangle<unsigned int>
bf::image_pool::get_spritepos_rectangle
( const wxString& image_name, const wxString& entry ) const
{
  const spritepos_entries r = get_spritepos_entries(image_name);
  const spritepos_entries::const_iterator it = r.find(entry);

  if ( it == r.end() )
    {
      std::clog << "Unknown spritepos entry '" << wx_to_std_string( entry )
                << "' in '" << wx_to_std_string( image_name ) << "'.\n";
      
      return claw::math::rectangle<unsigned int>(0, 0, 0, 0);
    }
  else
    return it->second;
} // image_pool::get_spritepos_rectangle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the name of a spritepos entry from a given file with the same
 *        rectangle than a given one.
 * \param image_name The name of the image to which the spritepos is associated.
 * \param r The rectangle for which we want the entry.
 */
wxString bf::image_pool::find_spritepos_name_from_size
( const wxString& image_name,
  const claw::math::rectangle<unsigned int>& r ) const
{
  const spritepos_entries e = get_spritepos_entries(image_name);

  for ( spritepos_entries::const_iterator it=e.begin(); it!=e.end(); ++it )
    if ( it->second == r )
      return it->first;

  return wxString();
} // image_pool::find_spritepos_name_from_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a constant iterator on the begining of the pool.
 */
bf::image_pool::const_iterator bf::image_pool::begin() const
{
  return const_iterator( m_thumbnail.begin() );
} // image_pool::begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a constant iterator just past the end of the pool.
 */
bf::image_pool::const_iterator bf::image_pool::end() const
{
  return const_iterator( m_thumbnail.end() );
} // image_pool::end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove all thumbnails and images.
 */
void bf::image_pool::clear()
{
  m_image.clear();
  m_thumbnail.clear();
} // image_pool::clear()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read all item files from a given directory and in its subdirectories.
 * \param dir_path The path to the directory to scan.
 */
void bf::image_pool::scan_directory( const std::string& dir_path )
{
  std::vector<std::string> ext(4);
  ext[0] = ".png";
  ext[1] = ".jpg";
  ext[2] = ".tga";
  ext[3] = ".bmp";

  std::string root( dir_path );
  if ( !root.empty() )
    if ( root[root.size() - 1] != '/' )
      root += '/';

  load_thumb_func f(m_thumbnail, root);
  scan_dir<load_thumb_func> scan;

  scan( root, f, ext.begin(), ext.end() );
} // image_pool::scan_directory()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load the spritepos entries associated with an image.
 * \param image_path The path to the image.
 */
void bf::image_pool::load_spritepos_file( const std::string& image_path ) const
{
  const wxString key = std_to_wx_string(image_path);

  if ( m_spritepos.find(key) != m_spritepos.end() )
    return;

  std::string::size_type pos = image_path.find_last_of('.');

  if ( pos != std::string::npos )
    {
      std::string std_name( image_path.substr(0, pos) + ".spritepos" );

      if ( path_configuration::get_instance().expand_file_name
           (std_name, 1, m_workspace) )
        {
          std::ifstream f( std_name.c_str() );

          if (f)
              m_spritepos[key] = read_spritepos_file(f);
          else
              m_spritepos[key] = spritepos_entries();
        }
    }
} // image_pool::load_spritepos_file()

/*----------------------------------------------------------------------------*/
/**
 * \brief Read the content of a spritepos file.
 * \param f The file to read from.
 */
bf::image_pool::spritepos_entries
bf::image_pool::read_spritepos_file( std::istream& f ) const
{
  claw::math::rectangle<unsigned int> r;
  std::string line;
  spritepos_entries result;

  while ( std::getline(f, line) )
    if ( !line.empty() )
      if ( line[0] != '#' )
        {
          std::vector<std::string> sp;

          claw::text::split(sp, line, ':');

          if (sp.size() != 1)
            {
              std::istringstream iss( sp.back() );

              if ( iss >> r.position.x >> r.position.y >> r.width >> r.height )
                result[std_to_wx_string(sp.front())] = r;
            }
        }

  return result;
} // image_pool::read_spritepos_file()

/*----------------------------------------------------------------------------*/
/**
 * \brief Load and store the data of an image.
 * \param name The name of the image to load.
 */
void bf::image_pool::load_image_data( const wxString& name ) const
{
  std::string std_name( wx_to_std_string(name) );

  if ( path_configuration::get_instance().expand_file_name
       (std_name, 1, m_workspace) )
    {
      if ( m_thumbnail.find(name) == m_thumbnail.end() )
        m_thumbnail[name] = load_thumb_func::load(std_name);

      m_image[name] = wxBitmap( wxImage(std_to_wx_string(std_name)) );
    }
  else
    {
      m_image[name] = wxBitmap();
      m_thumbnail[name] = wxBitmap();
    }
} // image_pool::load_image_data()
