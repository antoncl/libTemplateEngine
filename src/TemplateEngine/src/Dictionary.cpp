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
#include "Types.hpp"
#include "Exception.hpp"
#include "Dictionary.hpp"
#include "DictionaryList.hpp"

namespace template_engine {

Dictionary::Dictionary() :
	_map(),
	_parent()
{
}

Dictionary::~Dictionary()
{
}

const Dictionary::Element& Dictionary::find(const te_string& name) const
{
	te_dict::const_iterator it = _map.find(name);
	if (it != _map.end())
		return it->second;
	else if (!_parent.expired()) {
		return _parent.lock()->find(name);
	}

	te_converter converter;
	throw TemplateException("Attempt to find unknown dictionary entry '" + converter.to_bytes(name) + "'");
}

bool Dictionary::exists(const te_string& name) const
{
	te_dict::const_iterator it = _map.find(name);
	if (it != _map.end())
		return true;
	else if (!_parent.expired())
		return _parent.lock()->exists(name);

	return false;
}

bool Dictionary::isValue(const te_string& name) const
{
	const Element& e = find(name);

	return Element::element_t::Value == e.type;
}

const te_string& Dictionary::getValue(const te_string& name) const
{
	if (isValue(name)) {
		const Element& e = find(name);

		return *e.value;
	}

	te_converter converter;
	throw TemplateException("Attempt to get '" + converter.to_bytes(name) + "' as a value");
}

bool Dictionary::isList(const te_string& name) const
{
	const Element& e = find(name);

	return Element::element_t::List == e.type;
}

const std::shared_ptr<DictionaryList>& Dictionary::getList(const te_string& name) const
{
	if (isList(name)) {
		const Element& e = find(name);

		return e.list;
	}

	te_converter converter;
	throw TemplateException("Attempt to get '" + converter.to_bytes(name) + "' as a list");
}

void Dictionary::add(const te_string name, const te_string value)
{
	_map.insert({ name, Element(value) });
}

void Dictionary::add(const te_string name, const std::string& value)
{
	te_converter converter;
	_map.insert({ name, Element(converter.from_bytes(value)) });
}

void Dictionary::add(const te_string name, DictionaryListPtr value)
{
	_map.insert({ name, Element(value) });
	value->setParent(shared_from_this());
}

}
