/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class that create the adequate dialog for editing a field.
 * \author Julien Jorge
 */
#ifndef __BF_DIALOG_MAKER_HPP__
#define __BF_DIALOG_MAKER_HPP__

#include "bf/value_editor_dialog.hpp"
#include "bf/set_edit.hpp"
#include "bf/interval_edit.hpp"

namespace bf
{
  /**
   * \brief A class that create the adequate dialog for editing a field.
   *
   * \b Template \b parameters:
   * - \a Control The control used for selecting the value (free_edit, set_edit,
   *   sprite_edit and so on),
   * - \a Type The type of the edited value (integer_type,
   *   std::list<integer_type> and so on).
   *
   * \author Julien Jorge
   */
  template<typename Control, typename Type>
  class dialog_maker
  {
  public:
    typedef value_editor_dialog<Control, Type> dialog_type;
    typedef Type value_type;

  public:
    static dialog_type*
    create( wxWindow& parent, const wxString& type, const type_field& f,
            const value_type& v );

    static dialog_type*
    create( wxWindow& parent, const wxString& type, const type_field& f,
            const value_type& v, workspace_environment* env );

  }; // class dialog_maker

  /**
   * \brief Specialization of the bf::dialog_maker class for bf::set_edit<> type
   *        controls.
   *
   * \b Template \b parameters:
   * - \a T The type of the value edited with the set_edit (among the base
   *   types: integer_type, real_type and so on),
   * - \a Type The type of the edited value (integer_type,
   *   std::list<integer_type> and so on).
   *
   * \author Julien Jorge
   */
  template<typename T, typename Type>
  class dialog_maker<set_edit<T>, Type>
  {
  public:
    typedef value_editor_dialog<set_edit<T>, Type> dialog_type;
    typedef Type value_type;

  public:
    static dialog_type*
    create( wxWindow& parent, const wxString& type, const type_field& f,
            const value_type& v );

  }; // class dialog_maker [set_edit]

  /**
   * \brief Specialization of the bf::dialog_maker class for
   *        bf::interval_edit<> type controls.
   *
   * \b Template \b parameters:
   * - \a T The type of the value edited with the interval_edit (among the base
   *   types: integer_type, real_type and so on),
   * - \a Type The type of the edited value (integer_type,
   *   std::list<integer_type> and so on).
   *
   * \author Julien Jorge
   */
  template<typename T, typename Type>
  class dialog_maker<interval_edit<T>, Type>
  {
  public:
    typedef value_editor_dialog<interval_edit<T>, Type> dialog_type;
    typedef Type value_type;

  public:
    static dialog_type*
    create( wxWindow& parent, const wxString& type, const type_field& f,
            const value_type& v );

  }; // class dialog_maker [interval_edit]
} // namespace bf

#include "bf/impl/dialog_maker.tpp"

#endif // __BF_DIALOG_MAKER_HPP__
