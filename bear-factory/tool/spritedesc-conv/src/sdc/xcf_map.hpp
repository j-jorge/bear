/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The map associating the name of the xcf with their descriptions.
 * \author Julien Jorge
 */
#ifndef __SDC_XCF_MAP_HPP__
#define __SDC_XCF_MAP_HPP__

#include "xcf_info.hpp"

#include <string>
#include <map>

namespace sdc
{
  class xcf_info;

  /** \brief The map associating the name of the xcf with their
      descriptions. */
  typedef std::map<std::string, xcf_info> xcf_map;

} // namespace sdc

#endif // __SDC_XCF_MAP_HPP__
