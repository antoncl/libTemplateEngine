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

size_t Template::_recursionLevel = 0;

void Template::resetParser()
{
	_recursionLevel = 0;
}

TemplatePtr Template::parse(Scanner& scanner)
{
	LookaheadScanner lookahead(scanner);
	Lexer lexer(lookahead);

	return parse(lexer);
}

TemplatePtr Template::parse(Lexer& lexer)
{
	std::shared_ptr<TemplateList> templ = std::make_shared<TemplateList>();

	Lexer::Token tok = lexer.getNextToken();
	while (tok.getType() != Lexer::Token::token_t::Eos) {
		size_t startRecursionLevel = _recursionLevel;

		TemplatePtr subTemplate(nullptr);

		if (Lexer::Token::token_t::Char == tok.getType())
			subTemplate = parseSimpleTemplate(tok, lexer);
		else if (Lexer::Token::token_t::StartTag == tok.getType())
			subTemplate = parseInstructionTemplate(lexer);

		if (subTemplate)
			templ->push_back(subTemplate);

		// if the recursion level has dropped, i.e. we parsed an end repeat
		// we exit and return what ever we've got
		if (startRecursionLevel > _recursionLevel)
			break;

		tok = lexer.getNextToken();
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

	return std::make_unique<SimpleTemplate>(value);
}

TemplatePtr Template::parseInstructionTemplate(Lexer& lexer)
{
	Lexer::Token t = lexer.getNextToken();

	if (Lexer::Token::token_t::Char == t.getType()) {
		switch (t.getChar())
		{
			case TE_TEXT('#'):
				return parseStartRepeatTemplate(lexer);
			case TE_TEXT('/'):
				return parseEndRepeatTemplate(lexer);
			default:
				break;		// fall through to the exception
		}
	}
	else if (Lexer::Token::token_t::Name == t.getType())
		return parseExpansionTemplate(t.getName(), lexer);

	throw TemplateException("Unknown processing instruction");
}

TemplatePtr Template::parseExpansionTemplate(const te_string& name, Lexer& lexer)
{
	const Lexer::Token& t = lexer.getNextToken();

	if (Lexer::Token::token_t::EndTag != t.getType())
		throw TemplateException("Missing end tag '}}' in name expansion");

	return std::make_unique<ExpansionTemplate>(name);
}

TemplatePtr Template::parseStartRepeatTemplate(Lexer& lexer)
{
	// repeat <name> }}
	Lexer::Token operationToken = lexer.getNextToken();

	if (Lexer::Token::token_t::Name != operationToken.getType())
		throw TemplateException("Operation name expected");

	// make sure the operation name is all in lower case
	te_string localCopy = operationToken.getName();
	std::transform(localCopy.begin(), localCopy.end(), localCopy.begin(), ::tolower);

	if(localCopy != TE_TEXT("repeat"))
		throw TemplateException("The only supported instruction is 'repeat'");

	Lexer::Token nameToken = lexer.getNextToken();
	if (Lexer::Token::token_t::Name != nameToken.getType())
		throw TemplateException("Malformed repeat instruction, a name was expected");

	Lexer::Token endToken = lexer.getNextToken();
	if (Lexer::Token::token_t::EndTag != endToken.getType())
		throw TemplateException("Malformed repeat instruction, an end tag was expected");

	// we've now parsed a valid start repeat instruction
	// recursively parse the template
	++_recursionLevel;
	TemplatePtr nestedTemplate = parse(lexer);

	return std::make_unique<RepeatTemplate>(nameToken.getName(), std::move(nestedTemplate));
}

TemplatePtr Template::parseEndRepeatTemplate(Lexer& lexer)
{
	// repeat <name> }}
	Lexer::Token operationToken = lexer.getNextToken();

	if (Lexer::Token::token_t::Name != operationToken.getType())
		throw TemplateException("Operation name expected");

	// make sure the operation name is all in lower case
	te_string localCopy = operationToken.getName();
	std::transform(localCopy.begin(), localCopy.end(), localCopy.begin(), ::tolower);

	if (localCopy != TE_TEXT("repeat"))
		throw TemplateException("The only supported instruction is 'repeat'");

	Lexer::Token endToken = lexer.getNextToken();
	if (Lexer::Token::token_t::EndTag != endToken.getType())
		throw TemplateException("Malformed end repeat instruction, an end tag was expected");

	// the end tag has no implementation
	--_recursionLevel;
	return TemplatePtr(nullptr);
}

}
