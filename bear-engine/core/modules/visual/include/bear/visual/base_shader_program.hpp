/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Base class for all shader program implementations.
 * \author Julien Jorge
 */
#ifndef __VISUAL_BASE_SHADER_PROGRAM_HPP__
#define __VISUAL_BASE_SHADER_PROGRAM_HPP__

#include <claw/non_copyable.hpp>

#include "bear/visual/class_export.hpp"

namespace bear
{
  namespace visual
  {
    /**
     * \brief Base class for all shader program implementations.
     * \author Julien Jorge
     */
    class VISUAL_EXPORT base_shader_program:
      public claw::pattern::non_copyable
    {
    public:
      virtual ~base_shader_program() {}

    }; // class base_shader_program
  } // namespace visual
} // namespace bear

#endif // __VISUAL_BASE_SHADER_PROGRAM_HPP__
