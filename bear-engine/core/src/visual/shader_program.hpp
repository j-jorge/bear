/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A class that hides the implementation of the shader programs.
 * \author Julien Jorge
 */
#ifndef __VISUAL_SHADER_PROGRAM_HPP__
#define __VISUAL_SHADER_PROGRAM_HPP__

#include <string>

#include <claw/smart_ptr.hpp>

#include "visual/base_shader_program.hpp"

namespace bear
{
  namespace visual
  {
    /**
     * \brief A class that hides the implementation of the shader programs.
     * \author Julien Jorge
     */
    class VISUAL_EXPORT shader_program
    {
    private:
      typedef
        claw::memory::smart_ptr<base_shader_program> base_shader_program_ptr;

    public:
      shader_program();
      shader_program( std::istream& program_code );

      void clear();
      void restore( std::istream& program_code );

      bool is_valid() const;

      const base_shader_program* get_impl() const;

    private:
      /**
       * \brief The implementation of the shader_program.
       *
       * The implementation is shared among the instances, thus if one of them
       * is restored or replaced then all instances of program becomes clean.
       */
      claw::memory::smart_ptr<base_shader_program_ptr> m_impl;

    }; // class shader_program
  } // namespace visual
} // namespace bear

#endif // __VISUAL_SHADER_PROGRAM_HPP__
