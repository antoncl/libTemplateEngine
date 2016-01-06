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
#ifndef __SCANNER_HPP_
#define __SCANNER_HPP_

#include "Types.hpp"

namespace template_engine
{
/** \brief Abstract class, describing the interface for passing a template
 * description into the template engine.
 *
 * Scanners abstract away the complexities of handing input character streams.
 */
class Scanner
{
public:
    /** \brief Advance the cursor one UTF-16 code unit.
     *
     * \return virtual bool true if the move succeeded, false otherwise, typically due to reaching the end.
     */
	virtual bool moveNext() = 0;

    /** \brief Has the end of stream been reached?
     *
     * \return virtual bool false if more code units can be read.
     *
     */
	virtual bool atEos() const = 0;

    /** \brief Get the next UTF-16 code unit off the stream
     *
     * \return virtual The next code unit from the stream, or NUL (\\u0000) if at the end.
     */
	virtual te_char_t getChar() const = 0;
};

}
#endif//__SCANNER_HPP_
