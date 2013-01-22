/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A class that handles the things that can be copied and pasted in a
 *        level.
 * \author Julien Jorge
 */
#ifndef __BF_LEVEL_CLIPBOARD_HPP__
#define __BF_LEVEL_CLIPBOARD_HPP__

#include "bf/item_copy.hpp"
#include "bf/layer.hpp"

namespace bf
{
  /**
   * \brief A class that handles the things that can be copied and pasted in a
   *        level.
   *
   * The data is not available for other applications, nor for an other
   * instance of this program.
   *
   * \author Julien Jorge
   */
  struct level_clipboard
  {
  private:
    /**
     * \brief The type of the data effectively stored in the clipboard.
     */
    enum content_type
      {
        /** \brief No data has been copied. */
        content_none,

        /** \brief An item collection is the most recently stored data. */
        content_items,

        /** \brief A layer is the most recently stored data. */
        content_layer
      }; // enum content_type

  public:
    level_clipboard();

    bool has_items() const;
    item_copy get_items() const;
    void copy_items( item_copy copy );

    bool has_layer() const;
    layer get_layer() const;
    void copy_layer( layer the_layer );

  private:
    /** \brief The The type of the most recently stored data. */
    content_type m_content_type;

    /** \brief The last item collection added in the clipboard. */
    item_copy m_items;

    /** \brief The last layer added in the clipboard. */
    layer m_layer;

  }; // struct level_clipboard
} // namespace bf

#endif // __BF_LEVEL_CLIPBOARD_HPP__
