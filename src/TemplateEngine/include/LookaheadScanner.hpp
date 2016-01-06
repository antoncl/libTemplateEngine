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
#ifndef __LOOKAHEAD_SCANNER_HPP_
#define __LOOKAHEAD_SCANNER_HPP_

#include <queue>
#include "Scanner.hpp"

namespace template_engine
{

/** \brief Internal wrapper and adapter around scanners, enables peeking ahead into the stream.
 */
class LookaheadScanner : public Scanner
{
public:
    /** \brief Wrap the given scanner in an adapter, which supports peeking.
     *
     * \param wrappedScanner Scanner& The scanner to wrap/adapt.
     */
	LookaheadScanner(Scanner& wrappedScanner);

    /** \copydoc Scanner::moveNext() */
	inline virtual bool moveNext()
	{
		if(_lookAheadBuffer.empty())
			return _wrappedScanner.moveNext();

		_lookAheadBuffer.pop_front();

		// return false if both are empty
		return !(_lookAheadBuffer.empty() && _wrappedScanner.atEos());
	}

    /** \copydoc Scanner::atEos() */
	inline virtual bool atEos() const
	{
		if (_lookAheadBuffer.empty())
			return _wrappedScanner.atEos();

		return false;
	}

    /** \copydoc Scanner::getChar() */
	inline virtual te_char_t getChar() const
	{
		if (_lookAheadBuffer.empty())
			return _wrappedScanner.getChar();

		return _lookAheadBuffer.front();
	}

    /** \brief Push the UTF-16 code unit back into the stream
     *
     * \param ch te_char_t The UTF-16 code unit to push back.
     */
	void pushBack(te_char_t ch);

    /** \brief Push the UTF-16 code unit back into the stream
     *
     * \param ch te_char_t The UTF-16 code unit to push back.
     */
	void pushFront(te_char_t ch);


    /** \brief Look n character ahead into the stream.
     * There is no magic going on here, the only thing peek() does
     * is to cache n number of code units. Potentially caching
     * the complete input stream if n is sufficiently large.
     * \param n size_t Number of chars to look ahead.
     *
     * peek() is not allowed to fail. If n reaches beyond the
     * end of the stream NUL (\\u0000) is returned.
     * \return te_char_t The n'th code unit relative to the current position.
     */
	te_char_t peek(size_t n);

private:
	Scanner& _wrappedScanner;                   //!< The actual scanner being read from.
	std::deque<te_char_t> _lookAheadBuffer;     //!< Cache of code units which were peeked.
};

}
#endif // __LOOKAHEAD_SCANNER_HPP_
