/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A function object that reads a list of variable assignments as saved
 *        with a variable_saver.
 * \author Julien Jorge
 */
#ifndef __ENGINE_VARIABLE_LIST_READER_HPP__
#define __ENGINE_VARIABLE_LIST_READER_HPP__

#include "bear/engine/variable/var_map.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief A function object that reads a list of variable assignments as
     *        saved with a variable_saver.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT variable_list_reader
    {
    public:
      void operator()( std::istream& iss, var_map& output ) const;

    private:
      void apply
      ( var_map& v, const std::string& type, const std::string& name,
        const std::string& value ) const;
      std::string unescape( const std::string& s ) const;

      void add_string_variable
      ( var_map& v, const std::string& name, const std::string& value ) const;

      template<typename T>
      void add_variable
      ( var_map& v, const std::string& name, const std::string& value ) const;

    }; // class variable_list_reader

  } // namespace engine
} // namespace bear

#endif // __ENGINE_VARIABLE_LIST_READER_HPP__
