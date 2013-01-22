/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief This class contains the result of the check of a layer.
 * \author Julien Jorge
 */
#ifndef __BF_LAYER_CHECK_RESULT_HPP__
#define __BF_LAYER_CHECK_RESULT_HPP__

#include "bf/item_instance.hpp"
#include "bf/item_check_result.hpp"

namespace bf
{
  /**
   * \brief This class contains the result of the check of a layer.
   * \author Julien Jorge
   */
  class layer_check_result
  {
  private:
    /** \brief The type of the map in which we store the result of the check of
        the items. */
    typedef std::map<item_instance*, item_check_result> item_result_map_type;

  public:
    /** \brief The type of the iterators on the results of the check of the
        items. */
    typedef item_result_map_type::const_iterator item_iterator;

  public:
    void check( item_instance* item, const std::set<std::string>& map_id );

    void add( item_instance* item, const check_error& e );

    bool is_ok() const;

    bool contains( item_instance* item ) const;

    item_iterator item_begin() const;
    item_iterator item_end() const;

    const bf::item_check_result& get_item_result( item_instance* item ) const;

  private:
    /** \brief The result of the check of the items. */
    item_result_map_type m_items;

  }; // class layer_check_result
} // namespace bf

#endif // __BF_LAYER_CHECK_RESULT_HPP__
