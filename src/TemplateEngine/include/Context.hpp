// Copyright (C) 2015,2016 Anton Lauridsen
//
// This file is part of libTemplateEngine.
//
// libTemplateEngine is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// libTemplateEngine is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with libTemplateEngine. If not, see <http://www.gnu.org/licenses/>.
#ifndef __CONTEXT_HPP_
#define __CONTEXT_HPP_

#include <memory>

#include "Types.hpp"
#include "Dictionary.hpp"

namespace template_engine {

  /**
   * @brief A context is conceptually at the root of the Dictionary hierarchy.
   *
   * The context manages all other subdictionaries used during the redering process.
   * a template tree may use several subdictionaries, but only one context.
   */
  class Context : public Dictionary
{
public:
  /**
   * @brief Instantiate the context used for rendering a template.
   *
   * During construction the global dictionary will be populated with the following values:
   * |Name   |  values
   * |:------|:---------
   * |APP    | The name of the application (libTemplateEngine)
   * |VERSION| The version number of libTemplateEngine
   * |TIME   | The date and time where the context was created/instantiated
   */
    Context();

    /** \brief Set the root dictionary.
     * The root dictionary will always be a simple dictionary. If the first
     * expansion instruction encountered is a repeat instruction. The dictionary
     * must *at least* have a single entry for that repeat instruction.
     *
     * \param dictionary const DictionaryPtr Pointer to the root dictionary.
     *
     */
    void setDictionary(const DictionaryPtr dictionary)
    {
        _dictionary = dictionary;
    }

    /** \brief Get a pointer to the root dictionary.
     *
     * \return const DictionaryPtr& A pointer to the root dictionary.
     *
     */
	const DictionaryPtr& getDictionary() const
	{
	    return _dictionary;
	}

private:
	DictionaryPtr _dictionary;      //<!@internal The current root dictionary.
};

}
#endif // !__CONTEXT_HPP_
