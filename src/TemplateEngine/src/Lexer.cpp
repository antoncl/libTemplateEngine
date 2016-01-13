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

#include <cctype>

#include "Lexer.hpp"
#include "Exception.hpp"
#include "ReverseIterator.hpp"

namespace template_engine
{

const Lexer::Token& Lexer::getNextSimpleToken()
{
	// grab a character from the scanner
	te_char_t ch = _scanner.getChar();

	// is not a special character
	if( !(TE_TEXT('{') == ch || TE_TEXT('}') == ch || TE_TEXT('\\') == ch) )
	{
		_scanner.moveNext();				// eat ch;
		_token._char = ch;
		_token._type = Token::token_t::Char;
		return _token;
	}

	// not a simple char copy, so peek at the next chars
	te_char_t ch2 = _scanner.peek(1);
	te_char_t ch3 = _scanner.peek(2);

	// Comment tag? '{{-'
	if ((TE_TEXT('{') == ch && TE_TEXT('{') == ch2 && TE_TEXT('-') == ch3)) {
		_scanner.moveNext();				// eat ch
		_scanner.moveNext();				// eat ch2
		_scanner.moveNext();				// eat ch3

		_currentState = states_t::Comment;
		return getNextToken();
	}

	// Escape tag? '\{{' or '\}}'
	if ( (TE_TEXT('\\') == ch && TE_TEXT('{') == ch2 && TE_TEXT('{') == ch3) ||
		 (TE_TEXT('\\') == ch && TE_TEXT('}') == ch2 && TE_TEXT('}') == ch3)
	   ) {
		_scanner.moveNext();				// eat the backslash

		_currentState = states_t::Escape;
		return getNextEscapeToken();
	}

	// two '{{' in a row
	if ((TE_TEXT('{') == ch && TE_TEXT('{') == ch2)) {
		_currentState = states_t::Instruction;

		_scanner.moveNext();				// eat ch
		_scanner.moveNext();				// eat ch2
		_token._type = Token::token_t::StartTag;

		return _token;
	}

	// two '}}' in a row
	// If the template is syntactically correct this cannot happen
	// We check, in order to provide better errror reporting
	if ((TE_TEXT('}') == ch && TE_TEXT('}') == ch2)) {
		_scanner.moveNext();				// eat ch
		_scanner.moveNext();				// eat ch2

		_token._type = Token::token_t::EndTag;
		return _token;
	}

	// despite appearances ch is really just a simple char, i.e. a single backslash
	// or curly parenthesis
	_scanner.moveNext();				// eat ch;
	_token._char = ch;
	_token._type = Token::token_t::Char;
	return _token;
}

const Lexer::Token& Lexer::getNextToken()
{
	if (_scanner.atEos()) {
		_token._type = Token::token_t::Eos;
		return _token;
	}

	switch (_currentState)
	{
		case states_t::Simple:
			return getNextSimpleToken();
		case states_t::Instruction:
			return getNextInstructionToken();
		case states_t::Comment:
			return getNextCommentToken();
		case states_t::Escape:
			return getNextEscapeToken();
		default:
			break;
	}

	throw TemplateException("Unknown internal lexer state");
}

const Lexer::Token& Lexer::getNextInstructionToken()
{
	// eat any whitespace
	te_char_t ch = _scanner.getChar();
	while (std::isspace(ch)) {
		_scanner.moveNext();
		if (_scanner.atEos())
			throw new TemplateException("Unexpected end of stream while parsing an instruction");

		ch = _scanner.getChar();
	}

	// is the current token a charcter and one of the special processing characters?
	if (TE_TEXT('#') == ch || TE_TEXT('/') == ch || TE_TEXT('-') == ch) {
		_token._type = Token::token_t::Char;
		_token._char = ch;

		_scanner.moveNext();	// eat ch
		return _token;
	}

	// End of instruction tag? '}}'
	te_char_t ch2 = _scanner.peek(1);
	if (TE_TEXT('}') == ch && TE_TEXT('}') == ch2) {
		_scanner.moveNext();				// eat ch
		_scanner.moveNext();				// eat ch2

		_token._type = Token::token_t::EndTag;

		_currentState = states_t::Simple;
		return _token;
	}

	// we've eliminated everything but a name token,
	// so this must be a name token.
	_token._name.clear();
	_token._type = Token::token_t::Name;
	while (!_scanner.atEos()) {
		if (std::isalnum(ch) || TE_TEXT('_') == ch) {
			_token._name += ch;
			_scanner.moveNext();

			// we shouldn't be able to reach eos here, but it might happen
			if (_scanner.atEos())
				break;
		}
		else
			break;
		ch = _scanner.getChar();
	}

	return _token;
}

const Lexer::Token& Lexer::getNextCommentToken()
{
	while (!_scanner.atEos())
	{
		te_char_t ch = _scanner.getChar();
		if (TE_TEXT('}') == ch) {
			te_char_t ch2 = _scanner.peek(1);
			if (TE_TEXT('}') == ch2) {
				_scanner.moveNext();		// eat ch
				_scanner.moveNext();		// eat ch2

				_currentState = states_t::Simple;
				return getNextToken();
			}
		}
		_scanner.moveNext();
	}
	
	throw TemplateException("Runaway comment detected");
}

const Lexer::Token& Lexer::getNextEscapeToken()
{
	static uint32_t count = 3;

	if (--count) {
		_token._char = _scanner.getChar();
		_token._type = Token::token_t::Char;
		_scanner.moveNext();
		return _token;
	}

	_currentState = states_t::Simple;
	count = 3;
	return getNextToken();
}


void Lexer::putTokenBack(const Token& token)
{
	// only insert tokens which has a representation in the input stream
	switch (token.getType())
	{
        case Token::token_t::Char:
            _scanner.pushFront(token.getChar());

            // special case for escaping the backslash
            if (TE_TEXT('\\') == token.getChar())
                _scanner.pushFront(TE_TEXT('\\'));
            break;
        case Token::token_t::EndTag:
            _scanner.pushFront(TE_TEXT('}'));
            _scanner.pushFront(TE_TEXT('}'));
			// if we scanned an end tag, we entered the simple state
			// and need to return to the instruction state
			_currentState = states_t::Instruction;
			_inProcessingInstruction = false;
			break;
        case Token::token_t::StartTag:
            _scanner.pushFront(TE_TEXT('{'));
            _scanner.pushFront(TE_TEXT('{'));

			// if we scanned a start tag, we entered the instruction state
			// and need to leave that state
			_inProcessingInstruction = false;
			_currentState = states_t::Simple;
			break;
        case Token::token_t::Name:
            for (te_char_t ch : reverse(token.getName()))
                _scanner.pushFront(ch);
            break;
        case Token::token_t::Eos:       // don't put anything back for these
        case Token::token_t::Empty:
            break;
	}
}

}
