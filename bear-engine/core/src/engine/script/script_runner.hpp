/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A class that runs a script.
 * \author Julien Jorge
 */
#ifndef __ENGINE_SCRIPT_RUNNER_HPP__
#define __ENGINE_SCRIPT_RUNNER_HPP__

#include "engine/base_item.hpp"
#include "engine/script/call_sequence.hpp"
#include "engine/script/script_context.hpp"

#include "engine/class_export.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief A class that runs a script.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT script_runner :
      public text_interface::base_exportable
    {
    public:
       typedef text_interface::base_exportable super;

    private:
      TEXT_INTERFACE_DECLARE_METHOD_LIST(super, init_exported_methods)

    public:
      void set_actor_item( const std::string& name, base_item* item );
      void set_actor
      ( const std::string& name, text_interface::base_exportable* item );

      bool load_script( const std::string& s );
      void reset();
      bool finished() const;
      void play( universe::time_type duration );

      universe::time_type get_script_duration() const;

    protected:
      const script_context& get_context() const;
      virtual void on_script_started();
      virtual void on_script_stopped();

    private:
      void end();
      void play_action();

    private:
       static void init_exported_methods();

    private:
      /** \brief The calls of the script. */
      call_sequence m_sequence;

      /** \brief The context in which the script is executed. */
      script_context m_context;

      /** \brief The elapsed time since the beginning of the script. */
      universe::time_type m_date;

      /** \brief The next call to do. */
      call_sequence::const_iterator m_current_call;

    }; // class script_runner

  } // namespace engine
} // namespace bear

#endif // __ENGINE_SCRIPT_RUNNER_HPP__
