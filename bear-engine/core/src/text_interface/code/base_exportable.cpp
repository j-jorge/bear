/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::text_interface::base_exportable class.
 * \author Julien Jorge.
 */
#include "text_interface/base_exportable.hpp"

#include "text_interface/auto_converter.hpp"
#include "text_interface/method_caller.hpp"

#include <claw/logger.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::text_interface::base_exportable::method_list::method_list()
  : parent(NULL)
{
  // nothing to do
} // base_exportable::method_list::method_list()




/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::text_interface::base_exportable::~base_exportable()
{
  // nothing to do
} // base_exportable::~base_exportable()

/*----------------------------------------------------------------------------*/
/**
 * \brief Export the methods of the class.
 */
void bear::text_interface::base_exportable::init_method_list()
{
  // nothing to do
} // base_exportable::init_method_list()

/*----------------------------------------------------------------------------*/
/**
 * \brief Execute a method from the class from its name and its arguments as
 *        passed as strings.
 * \param n The name of the method to call.
 * \param args The string representation of the value of the arguments of the
 *        method.
 * \remark The call is make without any context, which mean that some argument
 *         conversion may not work.
 */
void bear::text_interface::base_exportable::execute
( const std::string& n, const std::vector<std::string>& args )
{
  method_caller const* f = find_function(n);

  if (f!=NULL)
    f->execute(this, args, argument_converter());
} // base_exportable::execute()

/*----------------------------------------------------------------------------*/
/**
 * \brief Execute a method from the class from its name and its arguments as
 *        passed as strings.
 * \param n The name of the method to call.
 * \param c The argument_converter used to convert the arguments.
 */
void bear::text_interface::base_exportable::execute
( const std::string& n, const auto_converter& c )
{
  method_caller const* f = find_function(n);

  if (f!=NULL)
    f->execute(this, c.get_arguments(), c);
} // base_exportable::execute()

/*----------------------------------------------------------------------------*/
/**
 * \brief Execute a method from the class from its name and its arguments as
 *        passed as strings.
 * \param n The name of the method to call.
 * \param args The string representation of the value of the arguments of the
 *        method.
 * \param c The argument_converter used to convert the arguments.
 */
void bear::text_interface::base_exportable::execute
( const std::string& n, const std::vector<std::string>& args,
  const argument_converter& c )
{
  method_caller const* f = find_function(n);

  if (f!=NULL)
    f->execute(this, args, c);
} // base_exportable::execute()

/*----------------------------------------------------------------------------*/
/**
 * \brief Execute a method from the class from its name and its arguments as
 *        passed as strings.
 * \param n The name of the method to call.
 * \param args The string representation of the value of the arguments of the
 *        method.
 * \param context The context in which these methods are called.
 */
bear::text_interface::method_caller const*
bear::text_interface::base_exportable::find_function
( const std::string& n ) const
{
  method_caller const* result=NULL;

  // Get the callers from the lowest class in the hierarchy.
  const method_list* m( get_method_list() );
  method_list_data_type::const_iterator it(m->data.find(n));
  bool stop(false);

  while( !stop ) // there is at least the null-terminating entry in m.data
    if ( it == m->data.end() )
      {
        // We are at the end of the caller tables. We will now look in the
        // table of the parent class, if there is such a class.
        if ( m->parent != NULL )
          {
            m = m->parent;
            it = m->data.find(n);
          }
        else
          {
            claw::logger << claw::log_warning << "Method '" << n
                         << "' not found." << std::endl;
            stop = true;
          }
      }
    else
      {
        // We found the method. Execute.
        stop = true;
        result = it->second;
      }

  return result;
} // base_exportable::find_function()

/*----------------------------------------------------------------------------*/
/** Initialisation of the static member variables used for the caller table in
    bear::text_interface::base_exportable. */
TEXT_INTERFACE_IMPLEMENT_ROOT_METHOD_LIST
