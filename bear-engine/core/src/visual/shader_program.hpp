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

#include <claw/multi_type_map.hpp>
#include <claw/multi_type_map_visitor.hpp>
#include <claw/meta/type_list.hpp>

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
      /**
       * \brief The types of the variables that can be passed to the program.
       */
      typedef claw::meta::type_list_maker<int, bool, double>::result var_types;

      /**
       * \brief The type of the map storing the values of the inputs of the
       *       program.
       */
      typedef claw::multi_type_map<std::string, var_types> input_variable_map;

      /**
       * \brief The type of the visitor to use to access the variables of the
       *        program.
       */
      typedef claw::multi_type_map_visitor variable_visitor_type;

    public:
      shader_program();
      shader_program( std::istream& program_code );

      void clear();
      void restore( std::istream& program_code );

      bool is_valid() const;

      const base_shader_program* get_impl() const;

      template<typename T>
        void set_variable( std::string name, T value );

      input_variable_map get_variables() const;

    private:
      /**
       * \brief The implementation of the shader_program.
       *
       * The implementation is shared among the instances, thus if one of them
       * is restored or replaced then all instances of program becomes clean.
       */
      claw::memory::smart_ptr<base_shader_program_ptr> m_impl;

      /** The values of the variables passed to the program. */
      input_variable_map m_input_variable;

    }; // class shader_program
  } // namespace visual
} // namespace bear

#include "visual/impl/shader_program.tpp"

#endif // __VISUAL_SHADER_PROGRAM_HPP__
