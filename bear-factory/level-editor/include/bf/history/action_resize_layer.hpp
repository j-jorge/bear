/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The action of resizing a layer.
 * \author Julien Jorge
 */
#ifndef __BF_ACTION_RESIZE_LAYER_HPP__
#define __BF_ACTION_RESIZE_LAYER_HPP__

#include "bf/history/level_action.hpp"

#include <string>

namespace bf
{
  /**
   * \brief The action of resizing a layer.
   * \author Julien Jorge
   */
  class action_resize_layer:
    public level_action
  {
  public:
    /**
     * \brief Constructor.
     * \param fit_level Tell if the layer has the same size than the level.
     * \param width The new width of the layer.
     * \param height The new height of the layer.
     * \param class_name The name of the class of the layer.
     * \param name The name of the layer.
     * \param tag The tag of the layer.
     * \param lay The layer to resize.
     */
    action_resize_layer
    ( bool fit_level, unsigned int width, unsigned int height,
      const std::string& class_name, const std::string& name,
      const std::string& tag, unsigned int lay );

    /**
     * \remark Calling execute() two times will restore the initial size.
     */
    void execute( gui_level& lvl );
    void undo( gui_level& lvl );

    bool is_identity( const gui_level& gui ) const;
    wxString get_description() const;

  private:
    /** \brief Tell if the layer has the same size than the level. */
    bool m_fit_level;

    /** \brief The new width of the layer. */
    unsigned int m_width;

    /** \brief The new height of the layer. */
    unsigned int m_height;

    /** \brief The name of the class of the layer. */
    std::string m_class;

    /** \brief The name of the layer. */
    std::string m_name;

    /** \brief The tag of the layer. */
    std::string m_tag;

    /** \brief The layer to resize. */
    unsigned int m_layer;

  }; // class action_resize_layer
} // namespace bf

#endif // __BF_ACTION_RESIZE_LAYER_HPP__
