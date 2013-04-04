/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A layer from our level.
 * \author Julien Jorge
 */
#ifndef __BF_LAYER_HPP__
#define __BF_LAYER_HPP__

#include "bf/item_instance.hpp"

#include "bf/item_filter/item_filter.hpp"

#include <claw/functional.hpp>
#include <claw/iterator.hpp>

#include <set>
#include <list>
#include <map>
#include <string>

namespace bf
{
  class layer_check_result;

  /**
   * \brief A layer from our level.
   * \author Julien Jorge
   */
  class layer
  {
  public:
    typedef std::set<item_instance*> item_set_type;
    typedef std::list<item_instance*> item_list_type;

    typedef claw::wrapped_iterator
    < item_instance,
      item_set_type::iterator,
      claw::dereference<item_instance>
    >::iterator_type item_iterator;

    typedef std::list<item_filter> filter_list;
    typedef filter_list::const_iterator const_filter_iterator;

  public:
    explicit layer( const std::string& layer_type = "decoration_layer",
                    const std::string& layer_name = "", bool fit_level = true,
                    unsigned int width = 1, unsigned int height = 1 );
    layer( const layer& that );
    ~layer();

    void set_fit_level( bool f );
    bool fits_level() const;

    void resize( unsigned int width, unsigned int height );
    void set_class_name( const std::string& class_name );
    void set_name( const std::string& name );

    void set_tag( const std::string& tag );
    std::string get_tag() const;

    void add_item( item_instance* item );
    void remove_item( item_instance* item );

    item_iterator find_item_by_id( const std::string& id ) const;
    std::list<item_instance*> get_identified_items() const;

    void prioritize( item_instance* item );
    void prioritize( item_instance* item, std::size_t p );
    void deprioritize( item_instance* item );
    bool is_prioritized( item_instance* item ) const;
    std::size_t get_priority( item_instance* item ) const;
    std::vector<item_instance*> get_prioritized_items() const;

    const std::string& get_class_name() const;
    const std::string& get_name() const;
    std::string get_displayed_name() const;
    unsigned int get_width() const;
    unsigned int get_height() const;
    std::size_t get_items_count() const;

    bool empty() const;
    bool contains( const item_instance& item ) const;

    void check( layer_check_result& result ) const;

    void compile( compiled_file& f, compilation_context& c ) const;

    const_filter_iterator filter_begin() const;
    const_filter_iterator filter_end() const;

    item_iterator item_begin() const;
    item_iterator item_end() const;

    item_iterator item_begin_no_filter() const;
    item_iterator item_end_no_filter() const;

    layer& operator=( const layer& that );

    void add_filter( const item_filter& filter );
    void remove_filter( const item_filter& filter );

    bool check_item_position() const;

  private:
    void clear();
    void assign( const layer& that );
    void sort_and_identify
    ( std::list<item_instance*>& referenced, compilation_context& c ) const;
    void sort_by_dependency
    ( item_instance* p, std::map<std::string, item_instance*>& items,
      std::list<item_instance*>& referenced ) const;

    bool evaluate_filters_on_item( const item_instance& item ) const;

    void check_priorities( layer_check_result& result ) const;
    void check_unique_identifiers
    ( layer_check_result& result, std::set<std::string>& identifiers ) const;

  private:
    /** \brief Tell if the width and the height are automatically set to the
        size of the level. */
    bool m_fit_level;

    /** \brief The width of the layer. */
    unsigned int m_width;

    /** \brief The height of the layer. */
    unsigned int m_height;

    /** \brief The type of the layer. */
    std::string m_layer_type;

    /** \brief The name of the layer. */
    std::string m_layer_name;

    /** \brief The items in this layer. */
    item_set_type m_item;

    /** \brief The items returned by item_begin()/item_end(). */
    item_set_type m_filtered_item;

    /** \brief The filters on the items returned by item_begin()/item_end(). */
    filter_list m_filters;

    /** \brief The prioritized items. */
    item_list_type m_priority;

    /** \brief The tag of the layer. */
    std::string m_tag;

  }; // class layer
} // namespace bf

#endif // __BF_LAYER_HPP__
