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
#ifndef __EXPANSION_TEMPLATE_HPP_
#define __EXPANSION_TEMPLATE_HPP_

#include "Template.hpp"

namespace template_engine
{

/** \brief Simple value <code>{{Name}}</code> template instruction.
 */
class ExpansionTemplate :
	public Template
{
public:
    /** \brief Construct an expansion template instruction
     *
     * \param name te_string    The name to be expanded, enclosed in <code>{{</code> and <code>}}</code>
     * \param scopeWalk uint8_t	The name refers to a dictionary this many steps away on the stack
     */
	ExpansionTemplate(const te_string& name, uint8_t scopeWalk);

protected:

    /** \brief Look up the name of this template in the given dictionary
     *
     * \param dictionary const DictionaryPtr&   The dictionary to use when looking up the value.
     * \param filter TemplateFilter             Call this function with the found value, and put the result onto the output buffer
     * \return virtual te_string                String to replace the template instruction with.
     * \throws TemplateException If the name isn't found or isn't a simple string value.
     */
	virtual te_string render(const DictionaryPtr& dictionary, TemplateFilter filter) const;

private:
	te_string _name;        //<! Name of the expansion instruction.
	uint8_t  _scopeWalk;	///< how far to break out of the current scope
};

}
#endif // !__EXPANSION_TEMPLATE_HPP_
