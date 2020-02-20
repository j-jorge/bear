/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The item_loader_fallback loader passes the field values to a
 *        base_item.
 * \author Julien Jorge
 */
#ifndef __ENGINE_ITEM_LOADER_FALLBACK_HPP__
#define __ENGINE_ITEM_LOADER_FALLBACK_HPP__

#include "bear/engine/loader/item_loader_base.hpp"

namespace bear
{
  namespace engine
  {
    class base_item;

    /**
     * \brief The item_loader_fallback loader passes the field values to a
     *        base_item.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT item_loader_fallback:
      public item_loader_base
    {
    public:
      item_loader_fallback( base_item& item );

      virtual item_loader_fallback* clone() const;

      virtual bool set_field
      ( const std::string& name, unsigned int value );
      virtual bool set_field( const std::string& name, int value );
      virtual bool set_field( const std::string& name, double value );
      virtual bool set_field( const std::string& name, bool value );
      virtual bool set_field
      ( const std::string& name, const std::string& value );
      virtual bool set_field
      ( const std::string& name, base_item* value );
      virtual bool set_field
      ( const std::string& name, const visual::sprite& value );
      virtual bool set_field
      ( const std::string& name, const visual::animation& value );
      virtual bool set_field
      ( const std::string& name, audio::sample* value );
      virtual bool set_field
        ( const std::string& name, visual::font value );
      virtual bool set_field
        ( const std::string& name, visual::color value );

      virtual bool set_field
      ( const std::string& name, const std::vector<unsigned int>& value );
      virtual bool set_field
      ( const std::string& name, const std::vector<int>& value );
      virtual bool set_field
      ( const std::string& name, const std::vector<double>& value );
      virtual bool set_field
      ( const std::string& name, const std::vector<bool>& value );
      virtual bool set_field
      ( const std::string& name, const std::vector<std::string>& value );
      virtual bool set_field
      ( const std::string& name, const std::vector<base_item*>& value );
      virtual bool set_field
      ( const std::string& name, const std::vector<visual::sprite>& value );
      virtual bool set_field
      ( const std::string& name, const std::vector<visual::animation>& value );
      virtual bool set_field
      ( const std::string& name, const std::vector<audio::sample*>& value );
      virtual bool set_field
        ( const std::string& name, const std::vector<visual::font>& value );
      virtual bool set_field
        ( const std::string& name, const std::vector<visual::color>& value );

    private:
      /** \brief The item to which the field values are passed. */
      base_item& m_item;

    }; // class item_loader_fallback

  } // namespace engine
} // namespace bear

#endif // __ENGINE_ITEM_LOADER_FALLBACK_HPP__
