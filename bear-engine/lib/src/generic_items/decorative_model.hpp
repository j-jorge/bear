/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief An item used to decorative a model. It will just display the model.
 * \author Julien Jorge
 */
#ifndef __BEAR_DECORATIVE_MODEL_HPP__
#define __BEAR_DECORATIVE_MODEL_HPP__

#include "engine/base_item.hpp"
#include "engine/model.hpp"
#include "engine/export.hpp"

#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief An item used to decorative a model. It will just display the model.
   *
   * The valid fields for this item are
   *  - \a model_file: (string) the path to the model file.
   *  - \a initial_action: (string) the initial action.
   *  - any field supported by the parent classes.
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT decorative_model:
    public engine::model<engine::base_item>
  {
    DECLARE_BASE_ITEM(decorative_model);

  public:
    typedef engine::model<engine::base_item> super;

  public:
    bool is_valid() const;

    void build();

    bool set_string_field( const std::string& name, const std::string& value );

  private:
    /** \brief The path to the model to load. */
    std::string m_model_path;   

    /** \brief The initial action. */
    std::string m_initial_action;
  }; // class decorative_model
} // namespace bear

#endif // __BEAR_DECORATIVE_MODEL_HPP__
