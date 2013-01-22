/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A camera shared among objects.
 * \author Sébastien Angibaud
 */
#ifndef __BEAR_CAMERA_ON_OBJECT_HPP__
#define __BEAR_CAMERA_ON_OBJECT_HPP__

#include "communication/typed_message.hpp"
#include "engine/messageable_item.hpp"
#include "generic_items/camera.hpp"
#include "universe/derived_item_handle.hpp"

#include "generic_items/class_export.hpp"
#include "engine/export.hpp"

namespace bear
{
  /**
   * \brief A camera shared among objects.
   * \author S�bastien Angibaud
   */
  class GENERIC_ITEMS_EXPORT camera_on_object:
    public camera
  {
    DECLARE_BASE_ITEM(camera_on_object);

    TEXT_INTERFACE_DECLARE_METHOD_LIST(super, init_exported_methods)

  public:
    typedef bear::camera super;

  private:
    /** \brief The type of an handle on a object. */
    typedef universe::derived_item_handle<base_item> handle_type;

    /** \brief The type of a list of objects. */
    typedef std::list<handle_type> handle_list;

  public:
    camera_on_object();
    void progress( bear::universe::time_type elapsed_time );

    bool set_item_list_field
    ( const std::string& name, const std::vector<base_item*>& value );
    bool set_bool_field( const std::string& name, bool value );

    void add_item(base_item* item);
    void remove_item(base_item* item);

  private:
    void progress_center( bear::universe::time_type elapsed_time );
    void progress_fit_items( bear::universe::time_type elapsed_time );

    static void init_exported_methods();

  private:
    /** \brief The objects on which camera is focused. */
    handle_list m_objects;

    /** \brief Indicates if camera is fit on items. */
    bool m_fit_items;

  }; // class camera_on_object
} // namespace bear

#endif // __BEAR_CAMERA_ON_OBJECT_HPP__
