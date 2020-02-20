/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This class contains the informations about the program to use to run a
 *        level.
 * \author Julien Jorge
 */
#ifndef __BF_RUN_CONFIGURATION_HPP__
#define __BF_RUN_CONFIGURATION_HPP__

#include "bf/item_instance.hpp"

#include <string>
#include <list>

namespace bf
{
  /**
   * \brief This class contains the informations about the program to use to run
   *        a level.
   * \author Julien Jorge
   */
  class run_configuration
  {
  public:
    const std::string& get_program_path() const;
    void set_program_path( const std::string& p );

    std::string get_argument( std::size_t i ) const;
    std::size_t get_arguments_count() const;
    void append_argument( const std::string& arg );

    item_instance get_item( std::size_t i ) const;
    std::size_t get_items_count() const;
    void append_item( const item_instance& item );

  private:
    /** \brief The path to the program to run to test the level. */
    std::string m_program_path;

    /** \brief The arguments to pass to the program. */
    std::list<std::string> m_arguments;

    /** \brief The items to add to the level before launching the program. */
    std::list<item_instance> m_items;

  }; // class run_configuration_frame
} // namespace bf

#endif // __BF_RUN_CONFIGURATION_FRAME_HPP__
