/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A set of classes for comparing instances of item_instance.
 * \author Julien Jorge
 */
#ifndef __BF_ITEM_COMPARATOR_HPP__
#define __BF_ITEM_COMPARATOR_HPP__

#include "bf/libeditor_export.hpp"

namespace bf
{
  class item_instance;

  /**
   * \brief A set of classes for comparing instances of item_instance.
   * \author Julien Jorge
   */
  class BEAR_EDITOR_EXPORT item_comparator
  {
  public:
    /**
     * \brief Do a lexicographic comparison of two items according to their
     *        position and their size.
     * \author Julien Jorge
     */
    class BEAR_EDITOR_EXPORT by_place
    {
    public:
      /**
       * \brief Do a lexicographic comparison of two items according to their
       *        position and their size.
       * \param a The left operand.
       * \param b The right operand.
       * \return a < b : position(a) lex< position(b) && size(a) lex< size(b)
       */
      bool operator()( const item_instance& a, const item_instance& b ) const;

    }; // class by_place

  }; // class item_comparator

} // namespace bf

#endif // __BF_ITEM_COMPARATOR_HPP__
