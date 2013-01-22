/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The description of the content of an xcf file.
 * \author Julien Jorge
 */
#ifndef __SDC_XCF_INFO_HPP__
#define __SDC_XCF_INFO_HPP__

#include "layer_info.hpp"

#include <map>

namespace sdc
{
  /**
   * \brief The informations of a xcf file.
   * \author Julien Jorge
   */
  struct xcf_info
  {
    /**
     * \brief The map containing the informations about the layers. The key is
     *        the name of the layer.
     */
    typedef std::map<std::string, layer_info> layer_map;

    /** \brief The width of the image. */
    std::size_t width;

    /** \brief The height of the image. */
    std::size_t height;

    /** \brief The layers in the image. */
    layer_map layers;

  }; // struct xcf_info
} // namespace sdc

#endif // __SDC_XCF_INFO_HPP__
