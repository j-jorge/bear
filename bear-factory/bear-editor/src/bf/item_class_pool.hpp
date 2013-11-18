/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A singleton containing all item classes.
 * \author Julien Jorge
 */
#ifndef __BF_ITEM_CLASS_POOL_HPP__
#define __BF_ITEM_CLASS_POOL_HPP__

#include <boost/filesystem/convenience.hpp>
#include <map>

#include "bf/item_class.hpp"
#include "bf/libeditor_export.hpp"

namespace bf
{
  /**
   * \brief A class containing all item classes.
   * \author Julien Jorge
   */
  class BEAR_EDITOR_EXPORT item_class_pool
  {
  private:
    /** \brief The map associating item classes with their name. */
    typedef std::map<std::string, item_class*> item_class_map;

    /** \brief Function object passed to scan_dir to get the item class
        files. */
    struct open_item_class_file
    {
      open_item_class_file( std::map<std::string, std::string>& c );

      void operator()( const std::string& path );

      /** \brief The files found in the directory. */
      std::map<std::string, std::string>& class_files;

    }; // struct open_item_class_file

  public:
    /** \brief Iterator on all the classes. */
    typedef claw::wrapped_iterator
    < const item_class,
      item_class_map::const_iterator,
      claw::unary_compose
      < claw::const_dereference<item_class>,
        claw::const_pair_second<item_class_map::value_type> > >
    ::iterator_type const_iterator;

  public:
    item_class_pool(const std::string& w);
    ~item_class_pool();

    bool has_item_class( const std::string& class_name ) const;
    const item_class& get_item_class( const std::string& class_name ) const;
    const item_class* get_item_class_ptr( const std::string& class_name ) const;

    const_iterator begin() const;
    const_iterator end() const;

  private:
    void scan_directory( const std::string& dir_path );

    void load_class
    ( const std::string& name, std::map<std::string, std::string>& files );

    void field_unicity_test();

    // not implemented
    item_class_pool& operator=( const item_class_pool& that );
    item_class_pool( const item_class_pool& that );

  private:
    /** \brief The item classes. */
    item_class_map m_item_class;
  }; // class item_class_pool
} // namespace bf

#endif // __BF_ITEM_CLASS_POOL_HPP__
