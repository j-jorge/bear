/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Macros and function useful for class exportation.
 * \author Julien Jorge
 */
#ifndef __ENGINE_EXPORT_FUNCTION_HPP__
#define __ENGINE_EXPORT_FUNCTION_HPP__

#include <string>

namespace bear
{
  namespace engine
  {
    class base_item;
    class layer;

    /**
     * \brief Get the name of the exported function made for a class.
     * \param class_name The name of the class that has been exported.
     */
    inline std::string export_function_name( const std::string& class_name )
    {
      // prefix must be the string used for prefix in BASE_ITEM_EXPORT
      return "create_" + class_name;
    } // export_function_name()

    typedef base_item* (*base_export_function_type)();

    /**
     * \brief Get the name of the exported function made for a class.
     * \param class_name The name of the class that has been exported.
     */
    inline std::string
    layer_export_function_name( const std::string& class_name )
    {
      // prefix must be the string used for prefix in LAYER_EXPORT
      return "create_layer_" + class_name;
    } // layer_export_function_name()

    typedef
    layer* (*layer_export_function_type)(const universe::size_box_type& s);

  } // namespace engine
} // namespace bear

#endif // __ENGINE_EXPORT_FUNCTION_HPP__
