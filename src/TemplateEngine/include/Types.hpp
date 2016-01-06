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
#ifndef __TEMPLATE_ENGINE_TYPES_HPP_
#define __TEMPLATE_ENGINE_TYPES_HPP_

#include <string>
#include <locale>
#include <codecvt>

namespace template_engine
{
#define TE_TEXT(STR)	u##STR

typedef char16_t		te_char_t;
typedef std::u16string	te_string;

// MSVC has some serious issues linking codecvt and friends.
#ifndef _WIN32
/** \brief Convert between UTF-8 and UTF-16 */
typedef std::wstring_convert<std::codecvt_utf8_utf16<te_char_t>, te_char_t> te_converter;
#else
/** \brief Convert between UTF-8 and UTF-16 */
struct te_converter
{
	/** \brief Convert a UTF-16 string to UTF-8
	 *  \param wstr A UTF-16 string
	 *  \return the same string as UTF-8
	 */
	std::string to_bytes(const te_string &wstr);

	/** \brief Convert a UTF-8 string to UTF-16
	 *  \param str A UTF-8 string
	 *  \return the same string as UTF-16
	 */
	te_string from_bytes(const std::string &str);
};
#endif

}
#endif // !__TEMPLATE_ENGINE_TYPES_HPP_
