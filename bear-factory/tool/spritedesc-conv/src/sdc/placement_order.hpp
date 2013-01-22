/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The heuristic to use to place the sprites.
 * \author Julien Jorge
 */
#ifndef __SDC_PLACEMENT_ORDER_HPP__
#define __SDC_PLACEMENT_ORDER_HPP__

namespace sdc
{
  /**
   * \brief The heuristic to use to place the sprites.
   * \author Julien Jorge
   */
  enum placement_order
    {
      /** \brief The sprites with the largest area are placed first. */
      decreasing_area,
      
      /** \brief The tallest sprites are placed first. */
      decreasing_height,

      /** \brief The sprites are placed in the order they are defined. */
      declaration_order
      
    }; // enum placement_order

} // namespace sdc

#endif // __SDC_PLACEMENT_ORDER_HPP__
