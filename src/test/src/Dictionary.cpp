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

struct Config {
	Config()
	{
#if defined(_WIN32) && defined(_DEBUG)
		_CrtSetDbgFlag(0);
#endif
		BOOST_TEST_MESSAGE("setup fixture");
	}
	~Config()
	{
#if defined(_WIN32) && defined(_DEBUG)
		//_CrtDumpMemoryLeaks();
#endif
		BOOST_TEST_MESSAGE("teardown fixture");
	}

};

BOOST_GLOBAL_FIXTURE(Config);

BOOST_AUTO_TEST_SUITE(DictionaryTest);

BOOST_AUTO_TEST_CASE(Dictionary01)
{
	// simple test that nothing is thrown by construction/destruction
	DictionaryPtr dict = std::make_shared<Dictionary>();
}

BOOST_AUTO_TEST_CASE(Dictionary02)
{
	DictionaryPtr dict = std::make_shared<Dictionary>();

	dict->add(TE_TEXT("NAME"), TE_TEXT("VALUE"));

	BOOST_CHECK(dict->exists(TE_TEXT("NAME")));
	BOOST_CHECK(dict->isValue(TE_TEXT("NAME")));
	BOOST_CHECK(!dict->isList(TE_TEXT("NAME")));
	BOOST_CHECK(dict->getValue(TE_TEXT("NAME")) == TE_TEXT("VALUE"));
}

BOOST_AUTO_TEST_CASE(Dictionary03)
{
	DictionaryPtr dict = std::make_shared<Dictionary>();
	DictionaryListPtr list = std::make_shared<DictionaryList>();

	dict->add(TE_TEXT("NAME"), TE_TEXT("VALUE"));
	list->add(TE_TEXT("NAME2"), TE_TEXT("VALUE2"));
	dict->add(TE_TEXT("LIST"), list);

	BOOST_CHECK(dict->exists(TE_TEXT("NAME")));
	BOOST_CHECK(dict->isValue(TE_TEXT("NAME")));
	BOOST_CHECK(!dict->isList(TE_TEXT("NAME")));
	BOOST_CHECK(dict->getValue(TE_TEXT("NAME")) == TE_TEXT("VALUE"));

	BOOST_CHECK(dict->exists(TE_TEXT("LIST")));
	BOOST_CHECK(!dict->isValue(TE_TEXT("LIST")));
	BOOST_CHECK(dict->isList(TE_TEXT("LIST")));
	BOOST_CHECK(dict->getList(TE_TEXT("LIST")) == list);

	BOOST_CHECK(list->exists(TE_TEXT("NAME2")));
	BOOST_CHECK(list->isValue(TE_TEXT("NAME2")));
	BOOST_CHECK(!list->isList(TE_TEXT("NAME2")));
	BOOST_CHECK(list->getValue(TE_TEXT("NAME2")) == TE_TEXT("VALUE2"));

	// test the scoping rules work as expected
	BOOST_CHECK(list->exists(TE_TEXT("NAME")));
	BOOST_CHECK(list->isValue(TE_TEXT("NAME")));
	BOOST_CHECK(!list->isList(TE_TEXT("NAME")));
	BOOST_CHECK(list->getValue(TE_TEXT("NAME")) == TE_TEXT("VALUE"));
}

BOOST_AUTO_TEST_CASE(Dictionary04)
{
	DictionaryPtr child = std::make_shared<Dictionary>();
	child->add(TE_TEXT("NAME"), TE_TEXT("CHILD1"));

	DictionaryListPtr list = std::make_shared<DictionaryList>();
	list->add(TE_TEXT("NAME2"), TE_TEXT("LIST"));
	list->add(child);

	DictionaryPtr root = std::make_shared<Dictionary>();
	root->add(TE_TEXT("NAME"), TE_TEXT("ROOT"));
	root->add(TE_TEXT("LIST"), list);

	// Get values from the child
	BOOST_CHECK(child->exists(TE_TEXT("NAME")));
	BOOST_CHECK(child->isValue(TE_TEXT("NAME")));
	BOOST_CHECK(!child->isList(TE_TEXT("NAME")));
	BOOST_CHECK(child->getValue(TE_TEXT("NAME")) == TE_TEXT("CHILD1"));
	BOOST_REQUIRE_THROW(child->getList(TE_TEXT("NAME")), TemplateException);

	// The child should have access to the list dictionary
	BOOST_CHECK(list->exists(TE_TEXT("NAME2")));
	BOOST_CHECK(list->isValue(TE_TEXT("NAME2")));
	BOOST_CHECK(!list->isList(TE_TEXT("NAME2")));
	BOOST_CHECK(list->getValue(TE_TEXT("NAME2")) == TE_TEXT("LIST"));
	BOOST_REQUIRE_THROW(list->getList(TE_TEXT("NAME2")), TemplateException);

	// The child should have access to the root dictionary
	BOOST_CHECK(child->exists(TE_TEXT("LIST")));
	BOOST_CHECK(!child->isValue(TE_TEXT("LIST")));
	BOOST_CHECK(child->isList(TE_TEXT("LIST")));
	BOOST_REQUIRE_THROW(child->getValue(TE_TEXT("LIST")), TemplateException);
	BOOST_CHECK(child->getList(TE_TEXT("LIST")) == list);

	// cursoring tests
	list->resetCursor();
	uint32_t count = 0;
	while (list->advanceCursor())
		count++;
	BOOST_CHECK(count == 1);

	child = std::make_shared<Dictionary>();
	child->add(TE_TEXT("NAME"), TE_TEXT("CHILD2"));
	list->add(child);

	list->resetCursor();
	count = 0;
	while (list->advanceCursor())
		count++;
	BOOST_CHECK(count == 2);

	// iterating through the list and look up values;
	list->resetCursor();
	BOOST_CHECK(list->getCurrent()->getValue(TE_TEXT("NAME")) == TE_TEXT("CHILD1"));
	list->advanceCursor();
	BOOST_CHECK(list->getCurrent()->getValue(TE_TEXT("NAME")) == TE_TEXT("CHILD2"));
	list->advanceCursor();
	BOOST_REQUIRE_THROW(list->getCurrent()->getValue(TE_TEXT("NAME")), TemplateException);
}

BOOST_AUTO_TEST_CASE(Dictionary05)
{
    // Template to compile
    StringScanner reader(TE_TEXT("{{APP}} {{NAME}}"));
    
    // Build the dictionary
    DictionaryPtr root = std::make_shared<Dictionary>();
    root->add(TE_TEXT("NAME"), TE_TEXT("<NAME>"));
    
    // Setup the template context
    ContextPtr context = Context::BuildContext();

    // Add root dictionary to context
    context->setDictionary(root);
    
    // compile the template
    TemplatePtr compiledTemplate = Template::parse(reader);

    // render the template
    te_string result = compiledTemplate->render(context);

    BOOST_CHECK(result == TE_TEXT("libTemplateEngine <NAME>"));
}

BOOST_AUTO_TEST_SUITE_END()
