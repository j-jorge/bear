/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A level, with its layers.
 * \author Julien Jorge
 */
#ifndef __BF_LEVEL_HPP__
#define __BF_LEVEL_HPP__

#include "bf/layer.hpp"
#include <vector>
#include <list>

namespace bf
{
  class level_check_result;

  /**
   * \brief A level, with its layers.
   * \author Julien Jorge
   */
  class level
  {
  public:
    level( const std::string& name, unsigned int width, unsigned int height,
           const std::string& mus );
    level( const level& that );
    ~level();

    layer& get_layer( unsigned int index ) const;

    layer* remove_layer( unsigned int index );

    layer& add_layer
    ( const std::string& class_name, const std::string& name,
      bool fit_level = true,
      unsigned int width = 1, unsigned int height = 1 );
    void add_layer( layer* lay, unsigned int index );

    void move_backward( unsigned int layer_index );
    void move_forward( unsigned int layer_index );

    bool empty() const;
    std::size_t layers_count() const;

    unsigned int get_width() const;
    unsigned int get_height() const;
    void set_size( unsigned int width, unsigned int height );

    const std::string& get_music() const;
    void set_music( const std::string& music );

    const std::string& get_name() const;
    void set_name( const std::string& name );

    std::pair<bool, layer::item_iterator>
    find_item_by_id( const std::string& id ) const;
    std::size_t get_layer_by_item( const item_instance& item ) const;

    void generate_valid_id( std::string& id ) const;
    void generate_valid_id
    ( std::string& id, const std::set<std::string>& avoid ) const;
    void fix_identifiers
    ( const std::vector<item_instance*>& new_items ) const;

    level& operator=( const level& that );

    void check( level_check_result& result );

    void compile( compiled_file& f, compilation_context& c ) const;

    void add_filter( const item_filter& filter );
    void remove_filter( const item_filter& filter );

  private:
    void clear();
    void assign( const level& that );

    void check_unique_identifiers
    ( unsigned int i, std::set<std::string>& identifiers ) const;

  private:
    /** \brief The name of the level. */
    std::string m_name;

    /** \brief The layers. */
    std::vector<layer*> m_layer;

    /** \brief The width of the level. */
    unsigned int m_width;

    /** \brief The height of the level. */
    unsigned int m_height;

    /** \brief The music to play in this level. */
    std::string m_music;

  }; // class level
} // namespace bf

#endif // __BF_LEVEL_HPP__
