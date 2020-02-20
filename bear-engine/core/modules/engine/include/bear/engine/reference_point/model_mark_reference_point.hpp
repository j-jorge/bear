/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Instances of model_mark_reference_point compute the reference point
 *        of in forced movements as the mark position of an item.
 * \author Sébastien Angibaud
 */
#ifndef __ENGINE_MODEL_MARK_REFERENCE_POINT_HPP__
#define __ENGINE_MODEL_MARK_REFERENCE_POINT_HPP__

#include "bear/engine/model.hpp"
#include "bear/universe/forced_movement/base_reference_point.hpp"
#include "bear/universe/derived_item_handle.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief Instances of model_mark_reference_point compute the reference
     *        point of in forced movements as the mark position of an item.
     *
     * \author Sébastien Angibaud
     */
    template<class Base>
    class model_mark_reference_point:
      public bear::universe::base_reference_point
    {
    private:
      /** The type of the handle on the item. */
      typedef bear::universe::derived_item_handle<Base> item_handle_type;
      
    public:
      explicit model_mark_reference_point
      ( bear::universe::physical_item& item, const std::string& mark_name );
      
      virtual bear::universe::base_reference_point* clone() const;
      
      virtual bool is_valid() const;
      virtual bear::universe::position_type get_point() const;
      
      virtual bool has_item() const;
      virtual bear::universe::physical_item& get_item() const;
      
    private:
      /** \brief The item on which we take the mark position. */
      item_handle_type m_item;
      
      /** \brief The item on which we take the mark position. */
      std::string m_mark_name;
    }; // class model_mark_reference_point
  } // namespace engine
} // namespace bear

#include "bear/engine/reference_point/impl/model_mark_reference_point.tpp"

#endif // __ENGINE_MODEL_MARK_REFERENCE_POINT_HPP__
