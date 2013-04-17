/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Important fields of the physical items.
 * \author Julien Jorge
 */
#ifndef __UNIVERSE_PHYSICAL_ITEM_STATE_HPP__
#define __UNIVERSE_PHYSICAL_ITEM_STATE_HPP__

#include "physical_item_attributes.hpp"

#include "universe/class_export.hpp"

namespace bear
{
  namespace universe
  {
    /**
     * \brief Important fields of the physical items.
     * \author Julien Jorge
     */
    class UNIVERSE_EXPORT physical_item_state:
      private physical_item_attributes
    {
    public:
      physical_item_state();
      physical_item_state( const physical_item_state& that );
      virtual ~physical_item_state();

      const size_box_type& get_size() const;
      size_type get_width() const;
      size_type get_height() const;

      void set_bounding_box( const universe::rectangle_type& r );
      universe::rectangle_type get_bounding_box() const;

      const force_type& get_acceleration() const;
      void set_acceleration( const force_type& a );

      force_type get_force() const;
      const force_type& get_internal_force() const;
      const force_type& get_external_force() const;
      void add_internal_force( const force_type& force );
      void add_external_force( const force_type& force );
      void set_internal_force( const force_type& f );
      void set_external_force( const force_type& f );

      double get_mass() const;
      void set_mass( double m );

      double get_density() const;
      void set_density( double d );

      const speed_type& get_speed() const;
      void set_speed( const speed_type& speed );
      void set_speed( double x, double y );

      double get_angular_speed() const;
      void set_angular_speed( double speed );
      void add_angular_speed( double speed );

      double get_friction() const;
      void set_friction( double f );

      double get_contact_friction() const;
      void set_contact_friction( double f );

      double get_elasticity() const;
      void set_elasticity( double e );

      double get_hardness() const;
      void set_hardness( double h );

      double get_system_angle() const;
      void set_system_angle( double a );
      void set_contact_angle( double a );

      bool has_free_system() const;
      void set_free_system( bool b );
      vector_type get_x_axis() const;

      void set_top( coordinate_type pos );
      void set_bottom( coordinate_type pos );
      void set_left( coordinate_type pos );
      void set_right( coordinate_type pos );
      void set_horizontal_middle( coordinate_type pos );
      void set_vertical_middle( coordinate_type pos );

      void set_top_left( const position_type& pos );
      void set_top_middle( const position_type& pos );
      void set_top_right( const position_type& pos );
      void set_bottom_left( const position_type& pos );
      void set_bottom_middle( const position_type& pos );
      void set_bottom_right( const position_type& pos );
      void set_left_middle( const position_type& pos );
      void set_right_middle( const position_type& pos );

      void set_top_left( const coordinate_type& x, const coordinate_type& y );
      void set_top_middle( const coordinate_type& x, const coordinate_type& y );
      void set_top_right( const coordinate_type& x, const coordinate_type& y );
      void set_bottom_left
        ( const coordinate_type& x, const coordinate_type& y );
      void set_bottom_middle
        ( const coordinate_type& x, const coordinate_type& y );
      void set_bottom_right
        ( const coordinate_type& x, const coordinate_type& y );
      void set_left_middle
        ( const coordinate_type& x, const coordinate_type& y );
      void set_right_middle
        ( const coordinate_type& x, const coordinate_type& y );

      coordinate_type get_left() const;
      coordinate_type get_top() const;
      coordinate_type get_right() const;
      coordinate_type get_bottom() const;
      coordinate_type get_horizontal_middle() const;
      coordinate_type get_vertical_middle() const;

      position_type get_top_left() const;
      position_type get_top_middle() const;
      position_type get_top_right() const;
      position_type get_bottom_left() const;
      position_type get_bottom_middle() const;
      position_type get_bottom_right() const;
      position_type get_left_middle() const;
      position_type get_right_middle() const;

      position_type get_center_of_mass() const;
      void set_center_of_mass
        ( const coordinate_type& x, const coordinate_type& y );
      void set_center_of_mass( const position_type& pos );
      void set_center_on( const physical_item_state& that );

      bool is_fixed() const;
      void fix();
      void remove_position_constraints();
      void remove_position_constraint_x();
      void remove_position_constraint_y();
      void add_position_constraints();
      void add_position_constraint_x();
      void add_position_constraint_y();

      bool can_move_items() const;
      void set_can_move_items(bool value);

      bool is_global() const;
      void set_global( bool global );

      bool has_left_contact() const;
      contact_range get_left_contact() const;

      bool has_right_contact() const;
      contact_range get_right_contact() const;

      bool has_top_contact() const;
      contact_range get_top_contact() const;

      bool has_bottom_contact() const;
      contact_range get_bottom_contact() const;

      bool has_middle_contact() const;
      bool has_contact() const;
      bool has_side_contact() const;

      void set_left_contact( bool contact = true );
      void set_left_contact( coordinate_type bottom, coordinate_type top );

      void set_right_contact( bool contact = true );
      void set_right_contact( coordinate_type bottom, coordinate_type top );

      void set_top_contact( bool contact = true );
      void set_top_contact( coordinate_type left, coordinate_type right );

      void set_bottom_contact( bool contact = true );
      void set_bottom_contact( coordinate_type left, coordinate_type right );

      void set_middle_contact( bool contact = true );

      void clear_contacts();

      void set_phantom( bool phantom );
      bool is_phantom() const;

      void set_artificial( bool a );
      bool is_artificial() const;

      void set_weak_collisions( bool w );
      bool has_weak_collisions() const;

      void set_size( const size_box_type& size );
      void set_size( size_type width, size_type height );
      void set_width( size_type width );
      void set_height( size_type height );

      void set_physical_state( const physical_item_state& s );

      virtual void to_string( std::string& str ) const;

    private:
      /**
       * \brief Tell if the item is fixed or not.
       *
       * If an item is fixed, its position, speed and acceleration won't be able
       * to change any more.
       */
      bool m_fixed;

    }; // class physical_item_state
  } // namespace universe
} // namespace bear

namespace std
{
  UNIVERSE_EXPORT std::ostream&
  operator<<
    ( std::ostream& os, const bear::universe::physical_item_state& item );
} // namespace std

#endif // __UNIVERSE_PHYSICAL_ITEM_STATE_HPP__
