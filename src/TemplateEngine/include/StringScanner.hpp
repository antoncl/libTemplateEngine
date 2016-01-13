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
#ifndef __STRING_CHAR_READER_HPP
#define __STRING_CHAR_READER_HPP

#include "Scanner.hpp"

namespace template_engine
{

/** \brief Scanner operating on UTF-16 strings.
 */
class StringScanner :
	public Scanner
{
public:
	/** Initialize a string scanner with an constant string.
	 * @param t	string to be parsed, a null pointer will be converted to an empty string
	 */
	StringScanner(const te_char_t* const t) :
		_template(t ? te_string(t) : te_string(TE_TEXT(""))),
		_templateIterator(_template.begin())
	{};

	/** \copydoc StringScanner::StringScanner(const te_char_t* const) */
	StringScanner(const te_string& t) :
		_template(te_string(t)),
		_templateIterator(_template.begin())
	{};

	/** \copydoc Scanner::moveNext() */
	virtual bool moveNext();

    /** \copydoc Scanner::atEos() */
	virtual bool atEos() const;

    /** \copydoc Scanner::getChar() */
	virtual te_char_t getChar() const;

private:
	const te_string _template;
	te_string::const_iterator _templateIterator;
};

}
#endif // !__STRING_CHAR_READER_HPP
