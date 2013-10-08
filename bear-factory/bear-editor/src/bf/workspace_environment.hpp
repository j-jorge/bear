/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief An workspace environment.
 * \author Sebastien Angibaud
 */
#ifndef __BF_WORKSPACE_ENVIRONMENT_HPP__
#define __BF_WORKSPACE_ENVIRONMENT_HPP__

#include "bf/image_pool.hpp"
#include "bf/item_class_pool.hpp"
#include <string>

namespace bf
{
  /**
   * \brief A workspace environment.
   *
   * \author Sebastien Angibaud
   */
  class workspace_environment
  {
  public:
    workspace_environment();
    workspace_environment( const std::string& n );
    
    const std::string& get_name() const;
    const image_pool& get_image_pool() const;
    const item_class_pool& get_item_class_pool() const;      

  private:
    /** \brief The name of the workspace. */
    std::string name;

    /** \brief The image pool. */
    image_pool m_image_pool;

    /** \brief The item class pool. */
    item_class_pool m_item_class_pool;
  }; // workspace_environment
} // namespace bf

#endif // __BF_WORKSPACE_ENVIRONMENT_HPP__
