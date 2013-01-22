/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file rocket.hpp
 * \brief The class describing a rocket.
 * \author Sebastien Angibaud
 */
#ifndef __BEAR_rocket_HPP__
#define __BEAR_rocket_HPP__

#include "generic_items/decorative_item.hpp"
#include "engine/export.hpp"
#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief The class describing a rocket.
   * The valid fields for this item are
   *  - any field supported by the parent classes.
   *
   * \author Sebastien Angibaud
   */
  class  GENERIC_ITEMS_EXPORT rocket :
    public decorative_item
  {
    DECLARE_BASE_ITEM(rocket);
    
  public:
    /** \brief The type of the parent class. */
    typedef decorative_item super;
    
  public:
    rocket();
    rocket( const rocket& that );
    ~rocket();
    
    void on_enters_layer();
    void progress( universe::time_type elapsed_time );
    
    void set_force_factor( double min_factor_force, double max_factor_force );
    void set_initial_angle( double angle );
    void set_angle( double min_angle, double max_angle );
    void set_rocket_count( unsigned int min_nb, unsigned int max_nb );
    void set_explosion_rocket_count( unsigned int min_nb, unsigned int max_nb );
    void set_explosion_date
    ( universe::time_type min_date, universe::time_type max_date );
    void set_trace
      ( universe::time_type length, const visual::color_type& color,
        double opacity );
    void set_fade_out_speed
    ( universe::time_type min_s, universe::time_type max_s );
    void add_rocket( const rocket& r );
    void set_explosion_sound_name( std::string name );

  private:
    void explose();
    void create_rocket(double initial_angle);
    void duplicate_rocket();
    void create_trace();
    void play_explosion_sound();
    void throw_rocket();
    
    template<class T> 
    T get_rand(T min, T max) const;
    
  private:
    /** \brief The time since the beginning. */
    universe::time_type m_date;

    /** \brief The minimal initial force factor to apply. */
    double m_min_force_factor;

    /** \brief The maximal initial force factor to apply. */
    double m_max_force_factor;

    /** \brief The initial angle to give. */
    double m_initial_angle;

    /** \brief The minimal initial angle to give. */
    double m_min_angle;

    /** \brief The maximal initial angle to give. */
    double m_max_angle;

    /** \brief The number of current rocket to create at the beginning. */
    unsigned int m_nb_rockets;

    /** \brief The minimal number of current rocket to create 
       at the beginning. */
    unsigned int m_min_nb_rockets;

    /** \brief The maximal number of current rocket to create 
       at the beginning. */
    unsigned int m_max_nb_rockets;

    /** \brief The number of rocket to create when the item exploses. */
    unsigned int m_nb_explose_rockets;

    /** \brief The minimal number of rocket to create when the item exploses. */
    unsigned int m_min_nb_explose_rockets;

    /** \brief The maximal number of rocket to create when the item exploses. */
    unsigned int m_max_nb_explose_rockets;

    /** \brief The date of the explosion. */
    universe::time_type m_explosion_date;

    /** \brief The minimal date of the explosion. */
    universe::time_type m_min_explosion_date;

    /** \brief The maximal date of the explosion. */
    universe::time_type m_max_explosion_date;

    /** \brief The length of the trace. */
    universe::time_type m_trace_length;

    /** \brief The color of the trace. */
    visual::color_type m_trace_color;
    
    /** \brief The speed of the fade out when the item exploses. */
    universe::time_type m_fade_out_speed;
    
    /** \brief The minimum speed of the fade when the item explose. */
    universe::time_type m_min_fade_out_speed;

    /** \brief The maximum speed of the fade when the item explose. */
    universe::time_type m_max_fade_out_speed;
    
    /** \brief The list of rockets that can be created when the item
        exploses. */
    std::vector<rocket*> m_rockets;

    /** \brief The name of the sound to play when the rocket exploses. */
    std::string m_explosion_sound_name;

    /** \brief The force to give. */
    const static bear::universe::force_type s_force;

  }; // class rocket
} // namespace rp

#endif // __BEAR_rocket_HPP__
