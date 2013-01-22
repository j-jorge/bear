/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A template class for item managing. Use it when you need to store
 *        items that can be added or deleted anytime but must wait if a
 *        process is running.
 * \author Julien Jorge.
 */
#ifndef __CONCEPT_ITEM_CONTAINER_HPP__
#define __CONCEPT_ITEM_CONTAINER_HPP__

#include <list>

namespace bear
{
  namespace concept
  {

    /**
     * \brief A template class for item managing. Use it when you need to store
     *        items that may want to be added or deleted anytime but should wait
     *        a signal to be really added or deleted.
     * \author Julien Jorge.
     */
    template<class ItemType>
    class item_container
    {
    public:
      /** \brief The type of the stored items. */
      typedef ItemType item_type;

    public:
      item_container();
      item_container( const item_container<item_type>& that );
      virtual ~item_container();

      void register_item( const item_type& who );
      void release_item( const item_type& who );

      bool locked() const;

    protected:
      virtual void add( const item_type& who ) = 0;
      virtual void remove( const item_type& who ) = 0;

      void lock();
      void unlock();

    private:
      /** \brief True if items can't be immediatly added. Items will be
          buffered. */
      bool m_locked;

      /** \brief The queue of the items to add. */
      std::list<item_type> m_life_queue;

      /** \brief The queue of the items to delete. */
      std::list<item_type> m_death_queue;

    }; // class item_container

  } // namespace concept
} // namespace bear

// template methods
#include "concept/impl/item_container.tpp"

#endif // __CONCEPT_ITEM_CONTAINER_HPP__
