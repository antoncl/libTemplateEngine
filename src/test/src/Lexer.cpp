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
#include <boost/test/unit_test.hpp>
namespace utf = boost::unit_test;


#include <TemplateEngine.hpp>
using namespace template_engine;

// Boost test doesn't know these types and cannot output their value
BOOST_TEST_DONT_PRINT_LOG_VALUE(te_string);
BOOST_TEST_DONT_PRINT_LOG_VALUE(Lexer::Token::token_t);

BOOST_AUTO_TEST_SUITE(LexerTest); // , *utf::disabled());

BOOST_AUTO_TEST_CASE(lexer01)
{
	StringScanner s(TE_TEXT(""));
	Lexer lexer(s);

	BOOST_CHECK(Lexer::Token::token_t::Eos == lexer.getNextToken().getType());
}

BOOST_AUTO_TEST_CASE(lexer02)
{
	StringScanner s(TE_TEXT("ABC"));
	Lexer lexer(s);

	BOOST_CHECK(Lexer::Token::token_t::Char == lexer.getNextToken().getType());
	BOOST_CHECK(Lexer::Token::token_t::Char == lexer.getNextToken().getType());
	BOOST_CHECK(Lexer::Token::token_t::Char == lexer.getNextToken().getType());
	BOOST_CHECK(Lexer::Token::token_t::Eos == lexer.getNextToken().getType());
}

BOOST_AUTO_TEST_CASE(lexer03)
{
	StringScanner s(TE_TEXT("{{"));
	Lexer lexer(s);

	BOOST_CHECK(Lexer::Token::token_t::StartTag == lexer.getNextToken().getType());
	BOOST_CHECK(Lexer::Token::token_t::Eos == lexer.getNextToken().getType());
}

BOOST_AUTO_TEST_CASE(lexer04)
{
	StringScanner s(TE_TEXT("}}"));
	Lexer lexer(s);

	BOOST_CHECK(Lexer::Token::token_t::EndTag == lexer.getNextToken().getType());
	BOOST_CHECK(Lexer::Token::token_t::Eos == lexer.getNextToken().getType());
}

BOOST_AUTO_TEST_CASE(lexer05)
{
	StringScanner s(uR"(\\)");
	Lexer lexer(s);

	BOOST_CHECK(TE_TEXT('\\') == lexer.getNextToken().getChar());
	BOOST_CHECK(TE_TEXT('\\') == lexer.getNextToken().getChar());
	BOOST_CHECK(Lexer::Token::token_t::Eos == lexer.getNextToken().getType());
}

BOOST_AUTO_TEST_CASE(lexer06)
{
	StringScanner s(uR"(\{)");
	Lexer lexer(s);

	BOOST_CHECK(TE_TEXT('\\') == lexer.getNextToken().getChar());
	BOOST_CHECK(TE_TEXT('{') == lexer.getNextToken().getChar());
	BOOST_CHECK(Lexer::Token::token_t::Eos == lexer.getNextToken().getType());
}

BOOST_AUTO_TEST_CASE(lexer07)
{
	StringScanner s(uR"(\)");
	Lexer lexer(s);

	BOOST_CHECK(TE_TEXT('\\') == lexer.getNextToken().getChar());
	BOOST_CHECK(Lexer::Token::token_t::Eos == lexer.getNextToken().getType());
}

BOOST_AUTO_TEST_CASE(lexer08)
{
	StringScanner s(uR"(\aaa)");
	Lexer lexer(s);

	BOOST_CHECK(TE_TEXT('\\') == lexer.getNextToken().getChar());
	BOOST_CHECK(TE_TEXT('a') == lexer.getNextToken().getChar());
	BOOST_CHECK(TE_TEXT('a') == lexer.getNextToken().getChar());
	BOOST_CHECK(TE_TEXT('a') == lexer.getNextToken().getChar());
	BOOST_CHECK(Lexer::Token::token_t::Eos == lexer.getNextToken().getType());
}

BOOST_AUTO_TEST_CASE(lexer09)
{
	StringScanner s(uR"(\{{ab)");
	Lexer lexer(s);

	BOOST_CHECK(TE_TEXT('{') == lexer.getNextToken().getChar());
	BOOST_CHECK(TE_TEXT('{') == lexer.getNextToken().getChar());
	BOOST_CHECK(TE_TEXT('a') == lexer.getNextToken().getChar());
	BOOST_CHECK(TE_TEXT('b') == lexer.getNextToken().getChar());
	BOOST_CHECK(Lexer::Token::token_t::Eos == lexer.getNextToken().getType());
}

BOOST_AUTO_TEST_CASE(lexer10)
{
	StringScanner s(uR"(\}}ab)");
	Lexer lexer(s);

	BOOST_CHECK(TE_TEXT('}') == lexer.getNextToken().getChar());
	BOOST_CHECK(TE_TEXT('}') == lexer.getNextToken().getChar());
	BOOST_CHECK(TE_TEXT('a') == lexer.getNextToken().getChar());
	BOOST_CHECK(TE_TEXT('b') == lexer.getNextToken().getChar());
	BOOST_CHECK(Lexer::Token::token_t::Eos == lexer.getNextToken().getType());
}

BOOST_AUTO_TEST_CASE(lexer11)
{
	StringScanner s(TE_TEXT("{{   TEST}}"));
	Lexer lexer(s);

	BOOST_CHECK(Lexer::Token::token_t::StartTag == lexer.getNextToken().getType());
	BOOST_CHECK(Lexer::Token::token_t::Name == lexer.getNextToken().getType());
	BOOST_CHECK(Lexer::Token::token_t::EndTag == lexer.getNextToken().getType());
	BOOST_CHECK(Lexer::Token::token_t::Eos == lexer.getNextToken().getType());
}

BOOST_AUTO_TEST_CASE(lexer12)
{
	StringScanner s(TE_TEXT("{{TEST}}a"));
	Lexer lexer(s);

	BOOST_CHECK(Lexer::Token::token_t::StartTag == lexer.getNextToken().getType());
	BOOST_CHECK(Lexer::Token::token_t::Name == lexer.getNextToken().getType());
	BOOST_CHECK(Lexer::Token::token_t::EndTag == lexer.getNextToken().getType());
	BOOST_CHECK(TE_TEXT('a') == lexer.getNextToken().getChar());
	BOOST_CHECK(Lexer::Token::token_t::Eos == lexer.getNextToken().getType());
}

BOOST_AUTO_TEST_CASE(lexer13)
{
	StringScanner s(TE_TEXT("{{TEST"));
	Lexer lexer(s);

	BOOST_CHECK(Lexer::Token::token_t::StartTag == lexer.getNextToken().getType());
	BOOST_CHECK(Lexer::Token::token_t::Name == lexer.getNextToken().getType());
	BOOST_CHECK(Lexer::Token::token_t::Eos == lexer.getNextToken().getType());
}

BOOST_AUTO_TEST_CASE(lexer14)
{
	StringScanner s(TE_TEXT("{{}}"));
	Lexer lexer(s);

	BOOST_CHECK(Lexer::Token::token_t::StartTag == lexer.getNextToken().getType());
	BOOST_CHECK(Lexer::Token::token_t::EndTag == lexer.getNextToken().getType());
	BOOST_CHECK(Lexer::Token::token_t::Eos == lexer.getNextToken().getType());
}

BOOST_AUTO_TEST_CASE(lexer15)
{
	StringScanner s(TE_TEXT("{{#repeat name}}"));
	Lexer lexer(s);

	BOOST_CHECK(Lexer::Token::token_t::StartTag == lexer.getNextToken().getType());
	BOOST_CHECK(Lexer::Token::token_t::Char == lexer.getNextToken().getType());

	const Lexer::Token& t1 = lexer.getNextToken();
	BOOST_CHECK(Lexer::Token::token_t::Name == t1.getType());
	BOOST_CHECK(t1.getName() == TE_TEXT("repeat"));

	const Lexer::Token& t2 = lexer.getNextToken();
	BOOST_CHECK(Lexer::Token::token_t::Name == t2.getType());
	BOOST_CHECK(t2.getName() == TE_TEXT("name"));

	BOOST_CHECK(Lexer::Token::token_t::EndTag == lexer.getNextToken().getType());
	BOOST_CHECK(Lexer::Token::token_t::Eos == lexer.getNextToken().getType());
}

BOOST_AUTO_TEST_CASE(lexer16)
{
	StringScanner s(TE_TEXT("{{-}}"));
	Lexer lexer(s);

	BOOST_CHECK(Lexer::Token::token_t::Eos == lexer.getNextToken().getType());
}

BOOST_AUTO_TEST_CASE(lexer17)
{
	StringScanner s(uR"({{- this is a test and \{{ \}})");
	Lexer lexer(s);

	BOOST_CHECK(Lexer::Token::token_t::Eos == lexer.getNextToken().getType());
}

BOOST_AUTO_TEST_CASE(lexer18)
{
	StringScanner s(uR"(a{{- this is a test and \{{ \}}b)");
	Lexer lexer(s);

	BOOST_CHECK(TE_TEXT('a') == lexer.getNextToken().getChar());
	BOOST_CHECK(TE_TEXT('b') == lexer.getNextToken().getChar());
	BOOST_CHECK(Lexer::Token::token_t::Eos == lexer.getNextToken().getType());
}

BOOST_AUTO_TEST_CASE(lexer19)
{
	StringScanner s(uR"(a{{- this is a test and )");
	Lexer lexer(s);

	BOOST_CHECK(TE_TEXT('a') == lexer.getNextToken().getChar());
	BOOST_REQUIRE_THROW(lexer.getNextToken().getType(), TemplateException);
}

BOOST_AUTO_TEST_CASE(lexer20)
{
	StringScanner s(uR"(a\{{\}}\b)");
	Lexer lexer(s);

	BOOST_CHECK(TE_TEXT('a') == lexer.getNextToken().getChar());
	BOOST_CHECK(TE_TEXT('{') == lexer.getNextToken().getChar());
	BOOST_CHECK(TE_TEXT('{') == lexer.getNextToken().getChar());
	BOOST_CHECK(TE_TEXT('}') == lexer.getNextToken().getChar());
	BOOST_CHECK(TE_TEXT('}') == lexer.getNextToken().getChar());
	BOOST_CHECK(TE_TEXT('\\') == lexer.getNextToken().getChar());
	BOOST_CHECK(TE_TEXT('b') == lexer.getNextToken().getChar());
	BOOST_CHECK(Lexer::Token::token_t::Eos == lexer.getNextToken().getType());
}

BOOST_AUTO_TEST_CASE(lexer21)
{
	StringScanner s(TE_TEXT("{{APP}} {{NAME}}"));
	Lexer lexer(s);

	BOOST_CHECK(Lexer::Token::token_t::StartTag == lexer.getNextToken().getType());
	const Lexer::Token& t1 = lexer.getNextToken();
	BOOST_CHECK(Lexer::Token::token_t::Name == t1.getType());
	BOOST_CHECK(t1.getName() == TE_TEXT("APP"));
	BOOST_CHECK(Lexer::Token::token_t::EndTag == lexer.getNextToken().getType());

	BOOST_CHECK(TE_TEXT(' ') == lexer.getNextToken().getChar());

	BOOST_CHECK(Lexer::Token::token_t::StartTag == lexer.getNextToken().getType());
	const Lexer::Token& t2 = lexer.getNextToken();
	BOOST_CHECK(Lexer::Token::token_t::Name == t2.getType());
	BOOST_CHECK(t2.getName() == TE_TEXT("NAME"));
	BOOST_CHECK(Lexer::Token::token_t::EndTag == lexer.getNextToken().getType());

	BOOST_CHECK(Lexer::Token::token_t::Eos == lexer.getNextToken().getType());
}

BOOST_AUTO_TEST_SUITE_END()


