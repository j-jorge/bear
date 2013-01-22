/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief An item existing only for a decoration purpose.
 * \author Sebastien Angibaud
 */
#ifndef __BEAR_DECORATIVE_ITEM_HPP__
#define __BEAR_DECORATIVE_ITEM_HPP__

#include "engine/base_item.hpp"
#include "engine/item_brick/basic_renderable_item.hpp"
#include "engine/item_brick/item_with_decoration.hpp"
#include "engine/item_brick/item_with_text.hpp"

#include "generic_items/class_export.hpp"

#include "engine/export.hpp"

namespace bear
{
  /**
   * \brief An item existing only for a decoration purpose.
   * \author Sebastien Angibaud
   * The fields of this item are
   *  - \a kill_on_contact: (bool) \c Indicates if the item kills himself when
   * he has a contact (default = false).
   *  - \a kill_when_leaving: (bool) \c Indicates if the item kills himself when
   * he leaves the active region (default = false).
   *  - \a kill_when finished: (bool) \c Indicates if the item kills himself
   * when the animation is finished (default = false).
   *
   */
  class GENERIC_ITEMS_EXPORT decorative_item:
    public engine::item_with_decoration
      < engine::item_with_text
        < engine::basic_renderable_item<engine::base_item> > >
  {
    DECLARE_BASE_ITEM(decorative_item);
    
    /** \brief The type of the parent class. */
    typedef engine::item_with_decoration
      < engine::item_with_text
        < engine::basic_renderable_item<engine::base_item> > > super;
    
  private:
    /**
     * \brief The item loader for the decorative item.
     */
    class loader:
      public engine::item_loader_base
    {
    private:
      /** \brief The type of the parent class. */
      typedef engine::item_loader_base super;
      
    public:
      explicit loader( decorative_item& item );

      loader* clone() const;

      bool set_field( const std::string& name, bool value );
      bool set_field( const std::string& name, double value );

    private:
      /** \brief The instance loaded by this loader. */
      decorative_item& m_item;

    }; // class loader

  public:
    decorative_item();

    void progress( universe::time_type elapsed_time );
    void get_visual( std::list<engine::scene_visual>& visuals ) const;

    void set_kill_when_finished(bool value);
    void set_kill_on_contact(bool value);

    bool get_kill_when_leaving() const;
    void set_kill_when_leaving(bool value);

    void set_shadow_x( double v );
    void set_shadow_y( double v );

  protected:
    void populate_loader_map( engine::item_loader_map& m );

  private:
    void leaves_active_region();

  private:
    /** \brief Indicates if the item kills himself when the animation is
        finished. */
    bool m_kill_when_finished;

    /** \brief Indicates if the item kills himself when he has a contact. */
    bool m_kill_on_contact;

    /** \brief Indicates if the item stops
        himself when he has a bootom contact. */
    bool m_stop_on_bottom_contact;

    /** \brief Tell if the item kills himself when he leaves the active
        region. */
    bool m_kill_when_leaving;

    /** \brief The offset of the shadow of the visuals on the x-axis. */
    visual::coordinate_type m_shadow_x;

    /** \brief The offset of the shadow of the visuals on the y-axis. */
    visual::coordinate_type m_shadow_y;

  }; // class decorative_item
} // namespace bear

#endif // __BEAR_DECORATIVE_ITEM_HPP__
