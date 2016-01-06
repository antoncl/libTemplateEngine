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
#ifndef __EXCEPTION_HPP_
#define __EXCEPTION_HPP_

#include <exception>
#include <string>

namespace template_engine
{

/** \brief Exception used for all error conditions.
 * As a convenience all exception messages are formatted as
 * standard stl strings <code>std::string</code>. This makes
 * debugging using standard tools easier.
 *
 * The error description can be extracted with what(),
 */
class TemplateException : public std::runtime_error
{
public:
    /** \brief Construct an exception with the given message.
     *
     * \param message const std::string& The error message as a stl string.
     */
	explicit TemplateException(const std::string& message) :
		std::runtime_error(message.c_str())
	{
	}

    /** \brief Construct an exception with the given message.
     *
     * \param message const std::string& The error message as a char pointer.
     */
	explicit TemplateException(const char* message) :
		std::runtime_error(message)
	{
	}

};

}
#endif//__EXCEPTION_HPP_
