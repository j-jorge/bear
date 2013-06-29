/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The true_type_memory_file maps a true type font file into the memory.
 * \author Julien Jorge
 */
#ifndef __VISUAL_TRUE_TYPE_MEMORY_FILE_HPP__
#define __VISUAL_TRUE_TYPE_MEMORY_FILE_HPP__

#include "visual/class_export.hpp"

#include <boost/shared_array.hpp>
#include <string>

namespace bear
{
  namespace visual
  {
    /**
     * \brief The true_type_memory_file maps a true type font file into the
     *        memory.
     * \author Julien Jorge
     */
    class VISUAL_EXPORT true_type_memory_file
    {
    private:
      /** \brief The type of the pointer to the file's content. The data is
          shared among the copies of the instance and will be deleted with the
          last instance. */
      typedef boost::shared_array<const unsigned char> content_pointer_type;

    public:
      true_type_memory_file( std::istream& is );

      const unsigned char* begin() const;
      const unsigned char* end() const;

    private:
      /** \brief The content of the file. */
      content_pointer_type m_content;

      /** \brief The number of bytes in the file. */
      std::size_t m_size;

    }; // class true_type_memory_file

  } // namespace visual
} // namespace bear

#endif // __VISUAL_TRUE_TYPE_MEMORY_FILE_HPP__
