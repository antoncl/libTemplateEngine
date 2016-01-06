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
#include "DictionaryList.hpp"
#include "Exception.hpp"

namespace template_engine {

DictionaryList::DictionaryList() :
	_dictionaries(),
	_activeDictionary(0)
{
}

void DictionaryList::add(DictionaryPtr dict)
{
	dict->setParent(shared_from_this());

	_dictionaries.push_back(dict);

	// make the newly inserted dictionary the active one
	_activeDictionary = _dictionaries.size() - 1;
}

void DictionaryList::resetCursor()
{
	_activeDictionary = 0;
}

bool DictionaryList::advanceCursor()
{
	if (_activeDictionary < _dictionaries.size()) {
		++_activeDictionary;
		return true;
	}
	return false;
}

const DictionaryPtr& DictionaryList::getCurrent() const
{
	if (_activeDictionary < _dictionaries.size())
		return _dictionaries[_activeDictionary];

	throw TemplateException("Cursor outside of the valid range");
}

size_t DictionaryList::size()
{
	return _dictionaries.size();
}


void DictionaryList::add(const te_string name, const te_string value)
{
	Dictionary::add(name, value);
}

void DictionaryList::add(const te_string name, DictionaryListPtr value)
{
	value->setParent(shared_from_this());

	Dictionary::add(name, value);
}

}