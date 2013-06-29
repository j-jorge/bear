/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The base class for all classes for which we want to be able to call
 *        methods from a text_interface file.
 * \author Julien Jorge.
 */
#ifndef __TEXT_AUTO_CONVERTER_MAKER_HPP__
#define __TEXT_AUTO_CONVERTER_MAKER_HPP__

#include "text_interface/auto_converter.hpp"

namespace bear
{
  namespace text_interface
  {
    template<typename A0>
    auto_converter auto_converter_maker( A0 arg0 );

    template<typename A0, typename A1>
    auto_converter auto_converter_maker( A0 arg0, A1 arg1 );

    template<typename A0, typename A1, typename A2>
    auto_converter auto_converter_maker( A0 arg0, A1 arg1, A2 arg2 );

  } // namespace text_interface
} // namespace bear

#include "text_interface/impl/auto_converter_maker.tpp"

#endif // __TEXT_AUTO_CONVERTER_MAKER_HPP__
