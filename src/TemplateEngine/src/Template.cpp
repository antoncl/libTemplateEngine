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

#include <cstring>
#include <memory>
#include <algorithm>

#include "Template.hpp"
#include "TemplateList.hpp"
#include "SimpleTemplate.hpp"
#include "ExpansionTemplate.hpp"
#include "RepeatTemplate.hpp"
#include "LookaheadScanner.hpp"
#include "Lexer.hpp"
#include "Exception.hpp"

namespace template_engine
{

TemplatePtr Template::parse(Scanner& scanner)
{
	LookaheadScanner lookahead(scanner);
	Lexer lexer(lookahead);

	return parse(lexer);
}

TemplatePtr Template::parse(Lexer& lexer)
{
	std::shared_ptr<TemplateList> templ = std::make_shared<TemplateList>();

	bool streamIsOk = true;
	while (streamIsOk) {
		const Lexer::Token& tok = lexer.getNextToken();
		if (Lexer::Token::token_t::Eos != tok.getType()) {
			TemplatePtr subTemplate(nullptr);

			if (Lexer::Token::token_t::Char == tok.getType())
				subTemplate = parseSimpleTemplate(tok, lexer);
			else if (Lexer::Token::token_t::StartTag == tok.getType())
				subTemplate = parseInstructionTemplate(lexer);

			if (subTemplate)
				templ->push_back(subTemplate);
			else
				return templ;
		}
		else
			streamIsOk = false;
	}

	return templ;
}

TemplatePtr Template::parseSimpleTemplate(const Lexer::Token& token, Lexer& lexer)
{
	Lexer::Token t = token;
	te_string value;

	while (t.getType() == Lexer::Token::token_t::Char) {
		value += t.getChar();
		t = lexer.getNextToken();
	}

	// while looking for the end of the simple template we
	// read beyond the end of that template.
	lexer.putTokenBack(t);

	return std::make_shared<SimpleTemplate>(value);
}

TemplatePtr Template::parseInstructionTemplate(Lexer& lexer)
{
	// {{? <name> }}
	//   ^

	const Lexer::Token& instructionMarker = lexer.getNextToken();

	if (Lexer::Token::token_t::Char == instructionMarker.getType()) {
		switch (instructionMarker.getChar())
		{
			case TE_TEXT('#'):
				{
					lexer.skipWhiteSpace();
					const Lexer::Token& instructionStart = lexer.getNextToken();
					if (Lexer::Token::token_t::Name == instructionStart.getType())
					{
						if (instructionStart.isName(TE_TEXT("repeat")))
							return parseRepeatTemplate(lexer);
						else {
							te_converter converter;
							throw new TemplateException("Unknown processing instruction: '" +
								converter.to_bytes(instructionStart.getName()) + "'");
						}
					}
				}
				break;
			case TE_TEXT('/'):
				{
					lexer.skipWhiteSpace();
					const Lexer::Token& instructionEnd = lexer.getNextToken();
					if (Lexer::Token::token_t::Name == instructionEnd.getType())
					{
						if (instructionEnd.isName(TE_TEXT("repeat"))) {
							return parseEndRepeatTemplate(lexer);
						}
						else {
							te_converter converter;
							throw new TemplateException("Unknown processing instruction: '" +
								converter.to_bytes(instructionEnd.getName()) + "'");
						}
					}
				}
				break;
			default:
				break;		// fall through to the exception
		}
	}

	return parseExpansionTemplate(instructionMarker, lexer);
}

TemplatePtr Template::parseExpansionTemplate(const Lexer::Token& token, Lexer& lexer)
{
	// {{ (:)*<name> }}
	//     ^

	const Lexer::Token* t = &token;
	uint8_t colonCount = 0;

	while (Lexer::Token::token_t::Char == t->getType() && TE_TEXT(':') == t->getChar()) {
		++colonCount;
		t = &lexer.getNextToken();
	}

	// {{ (:)*<name> }}
	//        ^
	te_string name;
	if (Lexer::Token::token_t::Name == t->getType())
		name = t->getName();
	else 
		throw TemplateException("Malformed name expansion");

	// {{ (:)*<name> }}
	//              ^

	lexer.skipWhiteSpace();
	t = &lexer.getNextToken();

	if (Lexer::Token::token_t::EndTag != t->getType())
		throw TemplateException("Missing end tag '}}' in name expansion");

	return std::make_shared<ExpansionTemplate>(name, colonCount);
}

TemplatePtr Template::parseRepeatTemplate(Lexer& lexer)
{
	// repeat <name> }} <text> {{/repeat}}
	te_string name = parseStartRepeatTemplate(lexer);
	TemplatePtr nestedTemplate = parse(lexer);

	TemplatePtr templ = std::make_shared<RepeatTemplate>(name, nestedTemplate);

	return templ;
}

te_string Template::parseStartRepeatTemplate(Lexer& lexer)
{
	//{{# repeat  <name> }}
	//           ^

	// skip the repeat name
	lexer.getNextToken();

	// <name>
	lexer.skipWhiteSpace();
	const Lexer::Token& nameToken = lexer.getNextToken();
	if (Lexer::Token::token_t::Name != nameToken.getType())
		throw TemplateException("Malformed repeat instruction, a name was expected");

	te_string name = nameToken.getName();

	// }}
	lexer.skipWhiteSpace();
	const Lexer::Token& endToken = lexer.getNextToken();
	if (Lexer::Token::token_t::EndTag != endToken.getType())
		throw TemplateException("Malformed repeat instruction, an end tag was expected");

	return name;
}

TemplatePtr Template::parseEndRepeatTemplate(Lexer& lexer)
{
	// {{/ repeat }}
	//           ^

	// }}
	lexer.skipWhiteSpace();
	const Lexer::Token& endTag = lexer.getNextToken();
	if (Lexer::Token::token_t::EndTag != endTag.getType())
		throw TemplateException("Malformed end repeat instruction, an end tag was expected");

	// signal end of recursion
	return nullptr;
}

}
