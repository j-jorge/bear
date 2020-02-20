/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A structure describing the data required to copy and paste a
 *        collection of items.
 * \author Julien Jorge
 */
#ifndef __BF_ITEM_COPY_HPP__
#define __BF_ITEM_COPY_HPP__

#include "bf/item_instance.hpp"

namespace bf
{
  /**
   * \brief This structure describes the data required to copy and paste a
   *        collection of items.
   * \author Julien Jorge
   */
  struct item_copy
  {
    /** \brief The x-position to take as a reference when placing the copied
        items. */
    double x;

    /** \brief The y-position to take as a reference when placing the copied
        items. */
    double y;

    /** \brief The copied items. */
    std::list<item_instance> items;

  }; // struct item_copy
} // namespace bf

#endif // __BF_ITEM_COPY_HPP__
