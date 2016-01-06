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
	if (_scanner.atEos()) {
		_token._type = Token::token_t::Eos;
		return _token;
	}

	// grab a character from the scanner
	te_char_t ch = _scanner.getChar();

	// is not a special character
	if( !(TE_TEXT('{') == ch || TE_TEXT('}') == ch || TE_TEXT('\\') == ch) )
	{
		_scanner.moveNext();			// eat the char;
		_token._char = ch;
		_token._type = Token::token_t::Char;
		return _token;
	}


	te_char_t ch2 = _scanner.peek(1);

	// two '{{' in a row
	if ((TE_TEXT('{') == ch && TE_TEXT('{') == ch2)) {
		_scanner.moveNext();				// eat ch
		_scanner.moveNext();				// eat ch2
		_token._type = Token::token_t::StartTag;
		return _token;
	}

	// two '}}' in a row
	if ((TE_TEXT('}') == ch && TE_TEXT('}') == ch2)) {
		_scanner.moveNext();				// eat ch
		_scanner.moveNext();				// eat ch2
		_token._type = Token::token_t::EndTag;
		return _token;
	}

	// two '\\' in a row
	if ((TE_TEXT('\\') == ch && TE_TEXT('\\') == ch2)) {
		_scanner.moveNext();				// eat ch
		_scanner.moveNext();				// eat ch2
		_token._type = Token::token_t::Char;
		_token._char = TE_TEXT('\\');
		return _token;
	}

	// an escaped start tag '\{'
	if ((TE_TEXT('\\') == ch && TE_TEXT('{') == ch2)) {
		_scanner.moveNext();				// eat ch
		_scanner.moveNext();				// eat ch2
		_token._type = Token::token_t::Char;
		_token._char = TE_TEXT('{');
		return _token;
	}

	// an escaped end tag '\}'
	if ((TE_TEXT('\\') == ch && TE_TEXT('}') == ch2)) {
		_scanner.moveNext();				// eat ch
		_scanner.moveNext();				// eat ch2
		_token._type = Token::token_t::Char;
		_token._char = TE_TEXT('}');
		return _token;
	}

	_scanner.moveNext();				// eat ch
	throw TemplateException("Unmatched escape '\\' character");
}

const Lexer::Token& Lexer::getNextToken()
{
	const Token& tok = getNextSimpleToken();

	if (Token::token_t::EndTag == tok.getType())
		_inProcessingInstruction = false;

	if (_inProcessingInstruction && Token::token_t::Eos != tok.getType())
		return getNextProcessingToken(tok);


	if (Token::token_t::StartTag == tok.getType())
		_inProcessingInstruction = true;

	return tok;
}

const Lexer::Token& Lexer::getNextProcessingToken(const Token& token)
{
	static Token compoundToken;
	compoundToken._type = token.getType();
	compoundToken._char = token.getChar();
	compoundToken._name.clear();

	// processing instructions can either be:
	// {{ <name> }}
	// or
	// {{ # repeat <name>}}
	// or
	// {{ / repeat }}
	// or
	// {{ - <comment> }}

	Token t = token;

	if(Token::token_t::StartTag == t.getType())
		throw TemplateException("Start tag '{{' encountered within processing instruction");

	// eat any whitespace
	while (Token::token_t::Char == t.getType() && std::isspace(t.getChar()))
		t = getNextSimpleToken();

	// is the current token a charcter and one of the special processing characters?
	if (Token::token_t::Char == t.getType() && (TE_TEXT('#') == t.getChar() || TE_TEXT('/') == t.getChar() || TE_TEXT('-') == t.getChar())) {
		compoundToken._type = Token::token_t::Char;
		compoundToken._char = t.getChar();
		return compoundToken;
	}

	// we'll - hopefully - break out of the loop before the end of stream
	// when breaking out, care must be taken to restore the input stream
	// to a sane state.
	//
	// This method looks one token ahead into the stream
	// when we finish lexing processing instruction
	// we need to put the scanned token back into the
	// input stream
	while (Token::token_t::Eos != t.getType()) {
		if (Token::token_t::Char == t.getType()) {
			if (std::isalnum(t.getChar()) || TE_TEXT('_') == t.getChar()) {
				compoundToken._type = Token::token_t::Name;
				compoundToken._name += t.getChar();
			}
			else {
				putTokenBack(t);
				return compoundToken;
			}
		} else if (Token::token_t::EndTag == t.getType()) {
			putTokenBack(t);
			return compoundToken;
		}
		else if (Token::token_t::StartTag == t.getType()) {
			putTokenBack(t);
			return compoundToken;
		}

		t = getNextSimpleToken();
	}

	return compoundToken;
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
            break;
        case Token::token_t::StartTag:
            _scanner.pushFront(TE_TEXT('{'));
            _scanner.pushFront(TE_TEXT('{'));

            // if we scanned a start tag, we entered the instruction state
            _inProcessingInstruction = false;
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
