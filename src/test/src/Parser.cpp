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

struct Fixture {
	Fixture() : dict(std::make_shared<Dictionary>()), ctx(Context::BuildContext())
	{
		ctx->setDictionary(dict);
		dict->add(TE_TEXT("TEST"), TE_TEXT("<TEST>"));

		DictionaryListPtr list = std::make_shared<DictionaryList>();
		dict->add(TE_TEXT("section"), list);

		DictionaryPtr child = std::make_shared<Dictionary>();
		list->add(child);
		child->add(TE_TEXT("B"), TE_TEXT("b"));

		child = std::make_shared<Dictionary>();
		list->add(child);
		child->add(TE_TEXT("B"), TE_TEXT("b"));

		BOOST_TEST_MESSAGE("setup fixture");
	}
	~Fixture() { BOOST_TEST_MESSAGE("teardown fixture"); }

	DictionaryPtr dict;
	ContextPtr ctx;
};


BOOST_FIXTURE_TEST_SUITE(ParserTest, Fixture); // , *utf::disabled());

BOOST_AUTO_TEST_CASE(empty_parser)
{
	StringScanner s(TE_TEXT(""));
	std::shared_ptr<Template> t = Template::parse(s);

	// must return non nullptr
	BOOST_CHECK_EQUAL((bool)t, true);
}

BOOST_AUTO_TEST_CASE(simple_text_parser)
{
	StringScanner s(TE_TEXT("hello world"));
	std::shared_ptr<Template> t = Template::parse(s);

	// must return non nullptr
	BOOST_CHECK_EQUAL((bool)t, true);

	te_string result = t->render(ctx);

	BOOST_CHECK_EQUAL(result, TE_TEXT("hello world"));
}

BOOST_AUTO_TEST_CASE(simple_replacement_parser)
{
	StringScanner s(TE_TEXT("{{TEST}}"));
	std::shared_ptr<Template> t = Template::parse(s);

	// must return non nullptr
	BOOST_CHECK_EQUAL((bool)t, true);

	te_string result = t->render(ctx);

	BOOST_CHECK_EQUAL(result, TE_TEXT("<TEST>"));
}

BOOST_AUTO_TEST_CASE(simple_replacement_parser_error_01)
{
	StringScanner s(TE_TEXT("{{TEST TEST}}"));
	BOOST_REQUIRE_THROW(Template::parse(s), TemplateException);
}

BOOST_AUTO_TEST_CASE(simple_replacement_parser_error_02)
{
	StringScanner s(TE_TEXT("{{TEST{{TEST}}"));
	BOOST_REQUIRE_THROW(Template::parse(s), TemplateException);
}

BOOST_AUTO_TEST_CASE(simple_replacement_parser_error_03)
{
	StringScanner s(TE_TEXT("{{TEST"));
	BOOST_REQUIRE_THROW(Template::parse(s), TemplateException);
}

BOOST_AUTO_TEST_CASE(simple_replacement_parser_error_04)
{
	StringScanner s(TE_TEXT("{{TEST\\\\"));
	BOOST_REQUIRE_THROW(Template::parse(s), TemplateException);
}

BOOST_AUTO_TEST_CASE(comment_parser_01)
{
	StringScanner s(TE_TEXT("{{- this is a comment}}"));
	std::shared_ptr<Template> t = Template::parse(s);

	// must return non nullptr
	BOOST_CHECK_EQUAL((bool)t, true);

	te_string result = t->render(ctx);

	BOOST_CHECK_EQUAL(result, TE_TEXT(""));
}

BOOST_AUTO_TEST_CASE(comment_parser_02)
{
	StringScanner s(TE_TEXT("{{- this is a comment"));
	BOOST_REQUIRE_THROW(Template::parse(s), TemplateException);
}

BOOST_AUTO_TEST_CASE(comment_parser_03)
{
	// strictly speaking this is a bug, but the effort required to
	// correct this is too big.
	StringScanner s(TE_TEXT("{{- comment \\ with an embedded single backslash}}"));
	BOOST_REQUIRE_THROW(Template::parse(s), TemplateException);
}

BOOST_AUTO_TEST_CASE(comment_parser_04)
{
	StringScanner s(TE_TEXT("A{{- this is a comment}}"));
	std::shared_ptr<Template> t = Template::parse(s);

	// must return non nullptr
	BOOST_CHECK_EQUAL((bool)t, true);

	te_string result = t->render(ctx);

	BOOST_CHECK_EQUAL(result, TE_TEXT("A"));
}

BOOST_AUTO_TEST_CASE(comment_parser_05)
{
	StringScanner s(TE_TEXT("{{- this is a comment}}B"));
	std::shared_ptr<Template> t = Template::parse(s);

	// must return non nullptr
	BOOST_CHECK_EQUAL((bool)t, true);

	te_string result = t->render(ctx);

	BOOST_CHECK_EQUAL(result, TE_TEXT("B"));
}

BOOST_AUTO_TEST_CASE(comment_parser_06)
{
	StringScanner s(TE_TEXT("A{{- this is a comment}}B"));
	std::shared_ptr<Template> t = Template::parse(s);

	// must return non nullptr
	BOOST_CHECK_EQUAL((bool)t, true);

	te_string result = t->render(ctx);

	BOOST_CHECK_EQUAL(result, TE_TEXT("AB"));
}

BOOST_AUTO_TEST_CASE(repeat_parser_01)
{
	StringScanner s(TE_TEXT("{{#repeat section}}B{{/repeat}}"));
	std::shared_ptr<Template> t = Template::parse(s);

	// must return non nullptr
	BOOST_CHECK_EQUAL((bool)t, true);

	te_string result = t->render(ctx);

	BOOST_CHECK_EQUAL(result, TE_TEXT("BB"));
}

BOOST_AUTO_TEST_CASE(repeat_parser_02) // , *utf::enabled())
{
	StringScanner s(TE_TEXT("A{{#repeat section}}B{{/repeat}}C"));
	std::shared_ptr<Template> t = Template::parse(s);

	// must return non nullptr
	BOOST_CHECK_EQUAL((bool)t, true);

	te_string result = t->render(ctx);

	BOOST_CHECK_EQUAL(result, TE_TEXT("ABBC"));
}

BOOST_AUTO_TEST_CASE(repeat_parser_03) // , *utf::enabled())
{
	StringScanner s(TE_TEXT("A{{#repeat section}}{{B}}{{/repeat}}C"));
	std::shared_ptr<Template> t = Template::parse(s);

	// must return non nullptr
	BOOST_CHECK_EQUAL((bool)t, true);

	te_string result = t->render(ctx);

	BOOST_CHECK_EQUAL(result, TE_TEXT("AbbC"));
}

BOOST_AUTO_TEST_SUITE_END()
