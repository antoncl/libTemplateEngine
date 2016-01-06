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
#ifndef __REPEAT_TEMPLATE_HPP_
#define __REPEAT_TEMPLATE_HPP_

#include "Template.hpp"
#include "Lexer.hpp"

namespace template_engine
{

/** \brief Repeat all templates and text, zero or more times.
 * A repeat template instruction repeats the template(s) enclosed
 * between start and end tags a variable number of times. The
 * number of times it is repeated depends on the DictionaryList
 * contained in the Dictionary it is passed during rendering.
 *
 * The format of a repeat is as follows:
 * <tt>\verbatim{{#REPEAT <name>}}<body>{{/REPEAT}}\endverbatim</tt>
 *
 * If the found DictionaryList contains three entries the body is
 * repeated three times. If the list is empty
 * an empty string will be produced.
 *
 * Repeats may be nested any number of times.
 */
class RepeatTemplate :
	public Template
{
public:
    /** \brief Construct a repeat template.
     *
     * \param name te_string                    Name of this repeat instruction.
     * \param templ std::shared_ptr<Template>   The template to repeat during rendering.
     */
	RepeatTemplate(te_string name, std::shared_ptr<Template> templ);

protected:
    /** \brief Repeat the text a number of timed, depending on the length of the supplied Dictionary
     *
     * \param dictionary const DictionaryPtr&   The dictionary to use when looking for <code>name</code>
     * \param filter TemplateFilter             ignored.
     * \return virtual te_string                String to replace the template instruction with.
     * \throws TemplateException If the Dictionary doesn't contain a DictionaryList with the name of the repeat instruction.
     */
	virtual te_string render(const DictionaryPtr& dictionary, TemplateFilter filter) const;

private:
	te_string _name;                    //<! Name of the repeat instruction
	std::shared_ptr<Template> _templ;   //<! The template to repeat
};

}
#endif // !__REPEAT_TEMPLATE_HPP_

