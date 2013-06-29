/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This is tool class that helps us to get the alignment class
 *        corresponding to a given zone.
 * \author Julien Jorge
 */
#ifndef __UNIVERSE_ALIGNMENT_TYPE_HPP__
#define __UNIVERSE_ALIGNMENT_TYPE_HPP__

#include "universe/zone.hpp"
#include "universe/alignment/align_top_left.hpp"
#include "universe/alignment/align_top.hpp"
#include "universe/alignment/align_top_right.hpp"
#include "universe/alignment/align_left.hpp"
#include "universe/alignment/align_right.hpp"
#include "universe/alignment/align_bottom_left.hpp"
#include "universe/alignment/align_bottom.hpp"
#include "universe/alignment/align_bottom_right.hpp"

namespace bear
{
  namespace universe
  {
    /**
     * \brief This is tool class that helps us to get the alignment class
     *        corresponding to a given zone.
     * \author Julien Jorge
     */
    template<zone::position Z>
    class alignment_type
    {
      // nothing
    }; // class alignment_type

    /**
     * \brief Specialization for the top left zone.
     * \author Julien Jorge
     */
    template<>
    class alignment_type<zone::top_left_zone>
    {
    public:
      typedef align_top_left type;
    }; // class alignment_type [top_left_zone]

    /**
     * \brief Specialization for the top zone.
     * \author Julien Jorge
     */
    template<>
    class alignment_type<zone::top_zone>
    {
    public:
      typedef align_top type;
    }; // class alignment_type [top_zone]

    /**
     * \brief Specialization for the top right zone.
     * \author Julien Jorge
     */
    template<>
    class alignment_type<zone::top_right_zone>
    {
    public:
      typedef align_top_right type;
    }; // class alignment_type [top_right_zone]

    /**
     * \brief Specialization for the middle left zone.
     * \author Julien Jorge
     */
    template<>
    class alignment_type<zone::middle_left_zone>
    {
    public:
      typedef align_left type;
    }; // class alignment_type [middle_left_zone]

    /**
     * \brief Specialization for the middle right zone.
     * \author Julien Jorge
     */
    template<>
    class alignment_type<zone::middle_right_zone>
    {
    public:
      typedef align_right type;
    }; // class alignment_type [middle_right_zone]

    /**
     * \brief Specialization for the bottom left zone.
     * \author Julien Jorge
     */
    template<>
    class alignment_type<zone::bottom_left_zone>
    {
    public:
      typedef align_bottom_left type;
    }; // class alignment_type [bottom_left_zone]

    /**
     * \brief Specialization for the bottom zone.
     * \author Julien Jorge
     */
    template<>
    class alignment_type<zone::bottom_zone>
    {
    public:
      typedef align_bottom type;
    }; // class alignment_type [bottom_zone]

    /**
     * \brief Specialization for the bottom right zone.
     * \author Julien Jorge
     */
    template<>
    class alignment_type<zone::bottom_right_zone>
    {
    public:
      typedef align_bottom_right type;
    }; // class alignment_type [bottom_right_zone]

  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_ALIGNMENT_TYPE_HPP__
