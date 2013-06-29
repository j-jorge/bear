/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A star.
 * \author Julien Jorge
 */
#ifndef __VISUAL_STAR_HPP__
#define __VISUAL_STAR_HPP__

#include "visual/types.hpp"

#include "visual/class_export.hpp"
#include <vector>

namespace bear
{
  namespace visual
  {
    /**
     * \brief A star.
     * \author Julien Jorge
     */
    class VISUAL_EXPORT star
    {
    public:
      star( std::size_t branches, double inside_ratio );

      double get_ratio() const;
      void set_ratio( double r );

      std::size_t get_branches() const;
      void set_branches( std::size_t b );

      const std::vector<position_type>& get_coordinates() const;

    private:
      void compute_coordinates( std::size_t branches, double inside_ratio );

    private:
      /** \brief The coordinates of the branches. */
      std::vector<position_type> m_coordinates;

    }; // class star
  } // namespace visual
} // namespace bear

#endif // __VISUAL_STAR_HPP__
