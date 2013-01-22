/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief This class contains the result of the check of a level.
 * \author Julien Jorge
 */
#ifndef __BF_LEVEL_CHECK_RESULT_HPP__
#define __BF_LEVEL_CHECK_RESULT_HPP__

#include "bf/layer_check_result.hpp"

#include <map>

namespace bf
{
  class layer;

  /**
   * \brief This class contains the result of the check of a level.
   * \author Julien Jorge
   */
  class level_check_result
  {
  private:
    /** \brief The type of the map in which we store the result of the check of
        the layers. */
    typedef std::map<layer*, layer_check_result> layer_result_map_type;

  public:
    /** \brief The type of the iterators on the results of the check of the
        layers. */
    typedef layer_result_map_type::const_iterator layer_iterator;

  public:
    void check( layer* lay );

    void clear();
    bool is_ok() const;

    bool contains( layer* lay, item_instance* item ) const;

    layer_iterator layer_begin() const;
    layer_iterator layer_end() const;

    const layer_check_result& get_layer_result( layer* lay ) const;

  private:
    /** \brief The result of the check of the layers. */
    layer_result_map_type m_layers;

  }; // class level_check_result
} // namespace bf

#endif // __BF_LEVEL_CHECK_RESULT_HPP__
