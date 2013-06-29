/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This class launchs the default browser with an url.
 * \author Sebastien Angibaud
 */
#ifndef __BEAR_BROWSER_LAUNCHER_TOGGLE_HPP__
#define __BEAR_BROWSER_LAUNCHER_TOGGLE_HPP__

#include "engine/item_brick/item_with_toggle.hpp"
#include "engine/base_item.hpp"
#include "engine/export.hpp"
#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief This item launchs the default browser with an url.
   *
   * The custom fields of this class are :
   * - \a url: string, the url to open (default = none).
   * - any field supported by the parent class.
   *
   * \author Sebastien Angibaud
   */
  class GENERIC_ITEMS_EXPORT browser_launcher_toggle:
    public engine::item_with_toggle<engine::base_item>
  {
    DECLARE_BASE_ITEM(browser_launcher_toggle);

  public:
    /** \brief The type of the parent class. */
    typedef engine::item_with_toggle<engine::base_item> super;

  public:
    browser_launcher_toggle();

    bool set_string_field( const std::string& name, const std::string& value );

  private:
    void on_toggle_on( engine::base_item* activator );

  private:
    /** \brief The url to open. */
    std::string m_url;

  }; // class browser_launcher_toggle
} // namespace bear

#endif // __BEAR_BROWSER_LAUNCHER_TOGGLE_HPP__
