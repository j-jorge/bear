/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The description of a slope in the game.
 * \author Julien Jorge
 */
#ifndef __BF_SLOPE_HPP__
#define __BF_SLOPE_HPP__

#include "bf/types.hpp"

namespace bf
{
  class item_instance;

  /**
   * \brief The description of a slope in the game.
   * \author Julien Jorge
   */
  class slope
  {
  public:
    slope();

    void read_from( const item_instance& item );

    void scale( double ratio );

  private:
    double read_item_field( const item_instance& item, std::string f ) const;
    
  public:
    /** \brief The difference in height between the beginning and the end of the
        slope. */
    coordinate_type steepness;
    
    /** \brief The position of the control point for the left side. */
    position_type left_handle;

    /** \brief The position of the control point for the right side. */
    position_type right_handle;

  }; // class slope
} // namespace bf

#endif // __BF_SLOPE_HPP__
