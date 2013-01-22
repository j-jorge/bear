/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::grid class.
 * \author Sebastien Angibaud
 */
#include "bf/grid.hpp"


/*----------------------------------------------------------------------------*/
const unsigned int bf::grid::s_step_x_default = 256;
const unsigned int bf::grid::s_step_y_default = 256;
const unsigned int bf::grid::s_magnetism_force_default = 10;

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bf::grid::grid()
  : m_offset(0, 0), m_step(s_step_x_default, s_step_y_default),
    m_magnetism_active(true), m_magnetism_force(s_magnetism_force_default)
{

} // grid::grid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the offset.
 * \param offset The new offset.
 */
void bf::grid::set_offset( const wxSize& offset )
{
  m_offset = offset;
} // grid::set_offset()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the step.
 * \param step The new step.
 */
void bf::grid::set_step( const wxSize& step )
{
  m_step = step;
} // grid::set_step()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the magnetism statut.
 * \param value The new statut.
 */
void bf::grid::set_magnetism_active(bool value)
{
  m_magnetism_active = value;
} // grid::set_magnetism_active()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the magnetism force.
 * \param force The new force.
 */
void bf::grid::set_magnetism_force(unsigned int force)
{
  m_magnetism_force = force;
} // grid::set_magnetism_force()


/*----------------------------------------------------------------------------*/
/**
 * \brief Return the offset.
 */
const wxSize& bf::grid::get_offset() const
{
  return m_offset;
} // grid::get_offset()


/*----------------------------------------------------------------------------*/
/**
 * \brief Return the step.
 */
const wxSize& bf::grid::get_step() const
{
  return m_step;
} // grid::get_step()

/*----------------------------------------------------------------------------*/
/**
 * \brief Return the step.
 */
bool bf::grid::get_magnetism_active() const
{
  return m_magnetism_active;
} // grid::get_magnetism_active()

/*----------------------------------------------------------------------------*/
/**
 * \brief Return the step.
 */
unsigned int bf::grid::get_magnetism_force() const
{
  return m_magnetism_force;
} // grid::get_magnetism_force()
