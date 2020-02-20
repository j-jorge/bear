/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This class is an argument converter used to statically associate a
 *        name with each of the elements of a list of arguments.
 * \author Julien Jorge.
 */
#ifndef __TEXT_AUTO_CONVERTER_HPP__
#define __TEXT_AUTO_CONVERTER_HPP__

#include "bear/text_interface/argument_converter.hpp"

#include <map>

namespace bear
{
  namespace text_interface
  {
    /**
     * \brief This class is an argument converter used to statically associate a
     *        name with each of the elements of a list of arguments.
     *
     * You will typicall use this converter when the name of the function to
     * call is unknown but its arguments are known.
     *
     * \author Julien Jorge
     */
    class auto_converter:
      public argument_converter
    {
    private:
      /** \brief The type of the map associating the arguments with their
          names. */
      typedef std::map<std::string, converted_argument> arg_map_type;

    public:
      std::vector<std::string> get_arguments() const;

      converted_argument do_convert_argument
      ( const std::string& arg, const std::type_info& type ) const;

      void push( converted_argument argv );

    private:
      /** \brief The map associating the arguments with their names. */
      arg_map_type m_args;

    }; // class auto_converter

  } // namespace text_interface
} // namespace bear

#endif // __TEXT_AUTO_CONVERTER_HPP__

