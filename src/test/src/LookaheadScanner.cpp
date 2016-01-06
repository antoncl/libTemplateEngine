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

#include <iostream>
#include <TemplateEngine.hpp>

BOOST_AUTO_TEST_SUITE(LookaheadScannerTest);
using namespace template_engine;

BOOST_AUTO_TEST_CASE(empty_string)
{
	StringScanner s(TE_TEXT(""));
	LookaheadScanner l(s);

	// on an empty string moveNext should return false on the first iteration
	BOOST_CHECK(false == l.moveNext());
}

BOOST_AUTO_TEST_CASE(null_string)
{
	StringScanner s(nullptr);
	LookaheadScanner l(s);

	// the null pointer should behave just like the empty string
	BOOST_CHECK(false == l.moveNext());
}

BOOST_AUTO_TEST_CASE(simple_string)
{
	StringScanner s(TE_TEXT("Abc"));
	LookaheadScanner l(s);

	// start scan
	BOOST_CHECK(TE_TEXT('A') == l.getChar());
	BOOST_CHECK(true == l.moveNext());
	BOOST_CHECK(TE_TEXT('b') == l.getChar());
	BOOST_CHECK(true == l.moveNext());
	BOOST_CHECK(TE_TEXT('c') == l.getChar());
	BOOST_CHECK(false == l.moveNext());
}

BOOST_AUTO_TEST_CASE(multiple_line_string)
{
	StringScanner s(TE_TEXT("Abc\ndef"));
	LookaheadScanner l(s);

	BOOST_CHECK(TE_TEXT('A') == l.getChar());
	BOOST_CHECK(true == l.moveNext());
	BOOST_CHECK(TE_TEXT('b') == l.getChar());
	BOOST_CHECK(true == l.moveNext());
	BOOST_CHECK(TE_TEXT('c') == l.getChar());
	BOOST_CHECK(true == l.moveNext());
	BOOST_CHECK(TE_TEXT('\n') == l.getChar());
	BOOST_CHECK(true == l.moveNext());
	BOOST_CHECK(TE_TEXT('d') == l.getChar());
	BOOST_CHECK(true == l.moveNext());
	BOOST_CHECK(TE_TEXT('e') == l.getChar());
	BOOST_CHECK(true == l.moveNext());
	BOOST_CHECK(TE_TEXT('f') == l.getChar());
	BOOST_CHECK(false == l.moveNext());
}

BOOST_AUTO_TEST_CASE(push_front_01)
{
	StringScanner s(TE_TEXT("Abc"));
	LookaheadScanner l(s);

	BOOST_CHECK(TE_TEXT('A') == l.getChar());
	BOOST_CHECK(true == l.moveNext());
	BOOST_CHECK(TE_TEXT('b') == l.getChar());
	BOOST_CHECK(true == l.moveNext());
	BOOST_CHECK(TE_TEXT('c') == l.getChar());
	BOOST_CHECK(false == l.moveNext());

	BOOST_CHECK(true == l.atEos());		// should be at eos now
	l.pushBack(TE_TEXT('Y'));
	BOOST_CHECK(false == l.atEos());		// adding a char should clear the eos scenario
	l.pushFront(TE_TEXT('X'));

	BOOST_CHECK(TE_TEXT('X') == l.getChar());
	BOOST_CHECK(true == l.moveNext());
	BOOST_CHECK(TE_TEXT('Y') == l.getChar());
	BOOST_CHECK(false == l.moveNext());
	BOOST_CHECK(true == l.atEos());		// should be at eos again
}

BOOST_AUTO_TEST_CASE(push_front_02)
{
	StringScanner s(TE_TEXT("A"));
	LookaheadScanner l(s);
	l.pushFront(TE_TEXT('Y'));
	l.pushFront(TE_TEXT('X'));

	BOOST_CHECK(TE_TEXT('X') == l.getChar());
	BOOST_CHECK(true == l.moveNext());
	BOOST_CHECK(TE_TEXT('Y') == l.getChar());
	BOOST_CHECK(true == l.moveNext());
	BOOST_CHECK(TE_TEXT('A') == l.getChar());
	BOOST_CHECK(false == l.moveNext());
}

BOOST_AUTO_TEST_CASE(peek_01)
{
	StringScanner s(TE_TEXT("A"));
	LookaheadScanner l(s);

	BOOST_CHECK(TE_TEXT('A') == l.peek(0));
	BOOST_CHECK(TE_TEXT('\0') == l.peek(12));
	BOOST_CHECK(TE_TEXT('A') == l.getChar());
	BOOST_CHECK(false == l.moveNext());
	BOOST_CHECK(true == l.atEos());
}

BOOST_AUTO_TEST_CASE(peek_02)
{
	StringScanner s(TE_TEXT(""));
	LookaheadScanner l(s);

	l.pushBack(TE_TEXT('A'));

	BOOST_CHECK(TE_TEXT('A') == l.peek(0));
	BOOST_CHECK(TE_TEXT('\0') == l.peek(12));
	BOOST_CHECK(TE_TEXT('A') == l.getChar());
	BOOST_CHECK(false == l.moveNext());
	BOOST_CHECK(true == l.atEos());
}
BOOST_AUTO_TEST_SUITE_END()
