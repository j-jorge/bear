/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A radio_group is a group of radio_button ensuring that at most one of
 *        them is checked at once.
 * \author Julien Jorge
 */
#ifndef __GUI_RADIO_GROUP_HPP__
#define __GUI_RADIO_GROUP_HPP__

#include "gui/visual_component.hpp"

#include "gui/class_export.hpp"

namespace bear
{
  namespace gui
  {
    class radio_button;

    /**
     * \brief A radio_group is a group of radio_button ensuring that at most one
     *        of them is checked at once.
     * \author Julien Jorge
     */
    class GUI_EXPORT radio_group:
      public visual_component
    {
    public:
      void add_button( radio_button* b, visual::size_type margin );
      const radio_button* get_selection() const;

    private:
      void on_check( std::size_t i ) const;

    private:
      /** \brief The buttons in this group. */
      std::vector<radio_button*> m_buttons;

    }; // class radio_group
  } // namespace gui
} // namespace bear

#endif // __GUI_RADIO_GROUP_HPP__
