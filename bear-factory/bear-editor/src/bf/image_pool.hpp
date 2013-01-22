/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A singleton containing all image resources.
 * \author Julien Jorge
 */
#ifndef __BF_IMAGE_POOL_HPP__
#define __BF_IMAGE_POOL_HPP__

#include "bf/libeditor_export.hpp"

#include <claw/basic_singleton.hpp>
#include <claw/iterator.hpp>
#include <claw/functional.hpp>
#include <claw/rectangle.hpp>
#include <map>

#include <wx/bitmap.h>

namespace bf
{
  /**
   * \brief A singleton containing all image resources.
   * \author Julien Jorge
   */
  class BEAR_EDITOR_EXPORT image_pool
    : public claw::pattern::basic_singleton<image_pool>
  {
  private:
    typedef std::map<wxString, wxBitmap> image_map;

    /** \brief Function object passed to bf::scan_dir to load the thumbnails. */
    struct load_thumb_func
    {
    public:
      load_thumb_func( image_map& t, const std::string& r );

      void operator()( const std::string& path );

      static wxBitmap load( const std::string& path );

    public:
      /** \brief The thumbnails of the images. */
      image_map& thumb;

    private:
      /** \brief The directory from which the search started. */
      std::string m_root;

    }; // struct load_thumb_func

  public:
    typedef
      std::map< wxString, claw::math::rectangle<unsigned int> >
      spritepos_entries;

  private:
    typedef std::map<wxString, spritepos_entries> spritepos_map;

  public:
    typedef claw::wrapped_iterator
    < const wxString,
      image_map::const_iterator,
      claw::const_pair_first<image_map::value_type> >
    ::iterator_type const_iterator;

  public:
    image_pool();

    void clear();
    void scan_directory( const std::string& dir_path );

    wxBitmap get_image( const wxString& image_name ) const;
    wxBitmap get_thumbnail( const wxString& image_name ) const;

    spritepos_entries get_spritepos_entries( const wxString& image_name ) const;

    claw::math::rectangle<unsigned int>
      get_spritepos_rectangle
      ( const wxString& image_name, const wxString& entry ) const;
    wxString find_spritepos_name_from_size
      ( const wxString& image_name,
        const claw::math::rectangle<unsigned int>& r ) const;

    const_iterator begin() const;
    const_iterator end() const;

  private:
    void load_spritepos_file( const std::string& image_path ) const;
    spritepos_entries read_spritepos_file( std::istream& f ) const;

    void load_image_data( const wxString& name ) const;

  public:
    /** \brief The maximum size of the thumbnails. */
    static const wxSize s_thumb_size;

  private:
    /** \brief The images used in the application. */
    mutable image_map m_image;

    /** \brief The thumbnails of the images. */
    mutable image_map m_thumbnail;

    /** \brief The spritepos entries for each image. */
    mutable spritepos_map m_spritepos;

  }; // class image_pool
} // namespace bf

#endif // __BF_IMAGE_POOL_HPP__
