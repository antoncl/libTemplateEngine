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
#ifndef __TEMPLATELIST_HPP_
#define __TEMPLATELIST_HPP_

#include <vector>
#include "Template.hpp"

namespace template_engine
{

/** \brief An ordered list of templates.
 * When a template definition is parsed, it it broken up into a number of
 * sub-templates. Typically simple text templates separated by various
 * kinds of instruction templates.
 *
 * The "root" template is a list template, as well as the template between
 * start and end repeat tags.
 *
 * The implementation is pretty bare, since almost all functionality is
 * handled by a STL container.
 */
class TemplateList : public std::vector<std::shared_ptr<const Template>>,
	public Template
{
protected:
    /** \brief Repeat the template length of dictionary list times.
     *
     * \param dictionary const DictionaryPtr&   Must be a DictionaryListPtr
     * \param filter TemplateFilter             Ignored.
     * \return virtual te_string                Expanded template length() times.
     */
	virtual te_string render(const DictionaryPtr& dictionary, TemplateFilter) const;
};

}
#endif // !__TEMPLATELIST_HPP_
