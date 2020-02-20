/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the specialisations of the
 *        bear::engine::type_to_string class.
 * \author Julien Jorge
 */
#include "bear/engine/variable/type_to_string.hpp"

const char* const bear::engine::type_to_string<int>::value = "int";
const char* const bear::engine::type_to_string<unsigned int>::value = "uint";
const char* const bear::engine::type_to_string<bool>::value = "bool";
const char* const bear::engine::type_to_string<std::string>::value = "string";
const char* const bear::engine::type_to_string<double>::value = "real";
