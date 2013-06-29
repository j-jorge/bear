/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The type of the items kept on the marks of the items.
 * \author Julien Jorge
 */
#ifndef __ENGINE_MODEL_MARK_ITEM_HPP__
#define __ENGINE_MODEL_MARK_ITEM_HPP__

#include "engine/base_item.hpp"
#include "universe/derived_item_handle.hpp"

#include "engine/export.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief The type of the items kept on the marks of the items.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT model_mark_item:
      public base_item
    {
      DECLARE_BASE_ITEM(model_mark_item);

    private:
      /** \brief The type of the handle on the model of which this is a mark. */
      typedef universe::derived_item_handle<base_item> handle_type;

    public:
      model_mark_item();
      model_mark_item( const model_mark_item& that );

      // Not implemented
      model_mark_item& operator=( const model_mark_item& that );

      void set_model_item( base_item& item );
      base_item& get_model_item();
      void set_collision_function( const std::string& f );

    private:
      void collision( base_item& that, universe::collision_info& info );

    private:
      /** \brief The model object owning this mark object. */
      handle_type m_model_item;

      /** \brief The function to call on m_model_item when a collision occurs on
          this item. */
      std::string m_collision_function;

    }; // class model_mark_item

  } // namespace engine
} // namespace bear

#endif // __ENGINE_MODEL_MARK_ITEM_HPP__
