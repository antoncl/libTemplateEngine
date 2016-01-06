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
#ifndef __SIMPLE_TEMPLATE_HPP_
#define __SIMPLE_TEMPLATE_HPP_

#include "Types.hpp"
#include "Template.hpp"

namespace template_engine
{

/** \brief Simple, plain text template, with no substitutions.
 */
class SimpleTemplate :
	public Template
{
public:
    /** \brief Construct a simple template with a constant string value.
     * \param value te_string The constant string to pass on to the output buffer during rendering.
     */
	SimpleTemplate(te_string value);

protected:
    /** \brief Copy constant string onto the output buffer.
     *
     * \param dictionary const DictionaryPtr&   Ignored
     * \param filter TemplateFilter             Ignored.
     * \return virtual te_string                Copy of the string used when the template was constructed.
     */
	virtual te_string render(const DictionaryPtr& dictionary, TemplateFilter filter) const;

private:
	te_string _value;   //!< The string to output.
};

}
#endif // __SIMPLE_TEMPLATE_HPP_
