/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The classes related to the loading of the items in the level files.
 * \author Julien Jorge
 */
#ifndef __ENGINE_ITEM_LOADER_BASE_HPP__
#define __ENGINE_ITEM_LOADER_BASE_HPP__

#include <string>

#include "bear/engine/class_export.hpp"

#include "bear/audio/sample.hpp"
#include "bear/visual/animation.hpp"
#include "bear/visual/sprite.hpp"
#include "bear/visual/color.hpp"
#include "bear/visual/font/font.hpp"

#include "bear/engine/easing.hpp"

namespace bear
{
  namespace engine
  {
    class base_item;

    /**
     * \brief Base class for the loaders of the items.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT item_loader_base
    {
    public:
      explicit item_loader_base( const std::string& name );

      virtual ~item_loader_base();

      virtual item_loader_base* clone() const = 0;
      const std::string& get_name() const;

      virtual bool set_field( const std::string& name, unsigned int value );
      virtual bool set_field( const std::string& name, int value );
      virtual bool set_field( const std::string& name, double value );
      virtual bool set_field( const std::string& name, bool value );
      virtual bool set_field
      ( const std::string& name, const std::string& value );
      virtual bool set_field( const std::string& name, base_item* value );
      virtual bool set_field
      ( const std::string& name, const visual::sprite& value );
      virtual bool set_field
      ( const std::string& name, const visual::animation& value );
      virtual bool set_field( const std::string& name, audio::sample* value );
      virtual bool set_field( const std::string& name, visual::font value );
      virtual bool set_field( const std::string& name, visual::color value );
      virtual bool set_field( const std::string& name, easing_function value );

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
      virtual bool set_field
        ( const std::string& name, const std::vector<easing_function>& value );

    private:
      /** The name assigned to this loader. */
      std::string m_name;

    }; // class item_loader_base

  } // namespace engine
} // namespace bear

#endif // __ENGINE_ITEM_LOADER_BASE_HPP__
