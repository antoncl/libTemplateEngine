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
#include "stdafx.h"
#include "LookaheadScanner.hpp"

namespace template_engine
{

LookaheadScanner::LookaheadScanner(Scanner& wrappedScanner) :
	_wrappedScanner(wrappedScanner),
	_lookAheadBuffer()
{
}

void LookaheadScanner::pushBack(te_char_t ch)
{
	_lookAheadBuffer.push_back(ch);
}

void LookaheadScanner::pushFront(te_char_t ch)
{
	_lookAheadBuffer.push_front(ch);
}

te_char_t LookaheadScanner::peek(size_t n)
{
	// simple case!, the buffer already contains the requested character
	if (_lookAheadBuffer.size() > n) {
		return _lookAheadBuffer.at(n);
	}

	// read characters from the scanner and put them on the buffer
	// until we have enough or we've reached the end of stream
	while (_lookAheadBuffer.size() <= n && !_wrappedScanner.atEos())
	{
		_lookAheadBuffer.push_back(_wrappedScanner.getChar());
		_wrappedScanner.moveNext();
	}

	// either the buffer has the character or the buffer was emptied
	if (_lookAheadBuffer.size() > n) {
		return _lookAheadBuffer.at(n);
	}

	// peek is not allowed to fail, and returns \0 beyond the end.
	return TE_TEXT('\0');
}


}