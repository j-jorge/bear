/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief This class contains the result of the check of an item instance.
 * \author Julien Jorge
 */
#ifndef __BF_ITEM_CHECK_RESULT_HPP__
#define __BF_ITEM_CHECK_RESULT_HPP__

#include "bf/check_error.hpp"

#include <list>

namespace bf
{
  /**
   * \brief This class contains the result of the check of an item instance.
   * \author Julien Jorge
   */
  class item_check_result
  {
  private:
    /** \brief The type of the list in which the problems are stored. */
    typedef std::list<check_error> error_list_type;

  public:
    /** \brief The type of teh iterator on the errors. */
    typedef error_list_type::const_iterator const_iterator;

  public:
    void add( const check_error& e );

    bool is_ok() const;

    const_iterator begin() const;
    const_iterator end() const;

  private:
    /** \brief The errors in the item. */
    error_list_type m_errors;

  }; // class item_check_result
} // namespace bf

#endif // __BF_ITEM_CHECK_RESULT_HPP__
