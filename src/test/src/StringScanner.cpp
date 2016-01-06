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
using namespace template_engine;

BOOST_AUTO_TEST_SUITE(ScannerTest);

BOOST_AUTO_TEST_CASE(empty_string)
{
	StringScanner reader(TE_TEXT(""));

	// on an empty string moveNext should return false on the first iteration
	BOOST_CHECK(false == reader.moveNext());
}

BOOST_AUTO_TEST_CASE(null_string)
{
	StringScanner reader(nullptr);

	// the null pointer should behave just like the empty string
	BOOST_CHECK(false == reader.moveNext());
}

BOOST_AUTO_TEST_CASE(simple_string)
{
	StringScanner reader(TE_TEXT("Abc"));

	BOOST_CHECK(TE_TEXT('A') == reader.getChar());
	BOOST_CHECK(true == reader.moveNext());
	BOOST_CHECK(TE_TEXT('b') == reader.getChar());
	BOOST_CHECK(true == reader.moveNext());
	BOOST_CHECK(TE_TEXT('c') == reader.getChar());
	BOOST_CHECK(false == reader.moveNext());
}

BOOST_AUTO_TEST_CASE(multiple_line_string)
{
	StringScanner reader(TE_TEXT("Abc\ndef"));

	BOOST_CHECK(TE_TEXT('A') == reader.getChar());
	BOOST_CHECK(true == reader.moveNext());
	BOOST_CHECK(TE_TEXT('b') == reader.getChar());
	BOOST_CHECK(true == reader.moveNext());
	BOOST_CHECK(TE_TEXT('c') == reader.getChar());
	BOOST_CHECK(true == reader.moveNext());
	BOOST_CHECK(TE_TEXT('\n') == reader.getChar());
	BOOST_CHECK(true == reader.moveNext());
	BOOST_CHECK(TE_TEXT('d') == reader.getChar());
	BOOST_CHECK(true == reader.moveNext());
	BOOST_CHECK(TE_TEXT('e') == reader.getChar());
	BOOST_CHECK(true == reader.moveNext());
	BOOST_CHECK(TE_TEXT('f') == reader.getChar());
	BOOST_CHECK(false == reader.moveNext());
}

BOOST_AUTO_TEST_SUITE_END()
