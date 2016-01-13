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
#ifndef __TEMPLATE_HPP_
#define __TEMPLATE_HPP_

#include <memory>
#include <functional>

#include "Scanner.hpp"
#include "Lexer.hpp"
#include "Context.hpp"

namespace template_engine
{

/** \brief Filter expansion prior to the result being pushed to the output buffer.
 * A filter function is responsible for filtering illegal characters
 * from a dictionary entry prior to the expanded value being pushed
 * onto the output buffer.
 *
 * It can e.g. be used to filter out &gt; and &lt; characters from
 * an XML formatted output buffer.
 *
 * *Note!* the filter is *only* applied on simple value expansions.
 */
typedef std::function<te_string(const te_string&)> TemplateFilter;

class Template;
typedef std::shared_ptr<Template> TemplatePtr;  //<! Pointer to a Template

/** \brief Abstract class describing every possible kind of template used.
 * This class is capable of parsing a template definition text, and instantiating
 * a valid hierarchy of templates, based on that definition.
 */
class Template
{
	friend class TemplateList;
	friend class RepeatTemplate;
public:
	Template() {};


    /** \brief Virtual destructor in order to ensure correct destruction of sub-classes.
     */
	virtual ~Template() {};

    /** \brief Scan the given input, and construct a template hierarchy from that definition.
     *
     * \param s Scanner&            The scanner to read the input configuration from.
     * \return TemplatePtr          Template hierarchy generated from the input.
     * \throws TemplateException    All parse errors are reported as exceptions.
     */
	static TemplatePtr parse(Scanner& s);

    /** \brief The parser is *not* re-entrant and must be reset between invocations.
     */
	static void resetParser();

    /** \brief Render the template, based on the specified dictionary/context.
     *
     * \param context const Context&    The context to use when expanding values.
     * \param filter TemplateFilter     Optional filter to apply when expanding values.
     * \return virtual te_string        String where values from the dictionaries have been expanded.
     * \throws TemplateException        All and all errors encountered, e.g. missing dictionary entries,
     */
	virtual te_string render(const ContextPtr context, TemplateFilter filter = nullptr) const
	{
		return render(context->getDictionary(), filter);
	}

protected:

    /** \brief Similar to the public render, except the dictionary to use has been resolved.
     *
     * \param dictionary    The context to use when expanding values.
     * \param filter        Optional filter to apply when expanding values.
     * \return              String where values from the dictionaries have been expanded.
     * \throws TemplateException    All and all errors encountered, e.g. missing dictionary entries,
     */
	virtual te_string render(const DictionaryPtr& dictionary, TemplateFilter filter) const = 0;

private:
	static TemplatePtr parse(Lexer& l);
	static TemplatePtr parseSimpleTemplate(const Lexer::Token& token, Lexer& lexer);
	static TemplatePtr parseInstructionTemplate(Lexer& lexer);
	static TemplatePtr parseStartRepeatTemplate(Lexer& lexer);
	static TemplatePtr parseEndRepeatTemplate(Lexer& lexer);
	static TemplatePtr parseExpansionTemplate(const te_string& name, Lexer& lexer);

	static size_t _recursionLevel;  //<! @internal Keep track of nesting level of repeats.
};

}
#endif // !__TEMPLATE_HPP_

