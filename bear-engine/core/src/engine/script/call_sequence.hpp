/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The sequence of the methods called in a script.
 * \author Julien Jorge
 */
#ifndef __ENGINE_CALL_SEQUENCE_HPP__
#define __ENGINE_CALL_SEQUENCE_HPP__

#include "engine/class_export.hpp"

#include "engine/script/method_call.hpp"
#include "universe/types.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief The sequence of the methods called in a script.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT call_sequence
    {
    public:
      /** \brief Informations on a call in the sequence. */
      struct call_info
      {
      public:
        /** \brief Compare two calls according to their dates, in increasing
            order. */
        struct compare_by_date
        {
          bool operator()( const call_info& a, const call_info& b ) const;
        }; // struct compare_by_date

      public:
        call_info( universe::time_type d, const method_call& c );

        /** \brief The date at which the call must be done. */
        universe::time_type date;

        /** \brief The call to do. */
        method_call call;

      }; // struct call_info

      /** \brief const iterator on the calls. */
      typedef std::vector<call_info>::const_iterator const_iterator;

    public:
      void add_call( universe::time_type date, const method_call& c );

      const_iterator begin() const;
      const_iterator end() const;

      bool empty() const;
      std::size_t size() const;

      universe::time_type get_last_date() const;

    private:
      /** \brief The calls to the methods. */
      std::vector<call_info> m_sequence;

    }; // class call_sequence

  } // namespace engine
} // namespace bear

#endif // __ENGINE_CALL_SEQUENCE_HPP__
