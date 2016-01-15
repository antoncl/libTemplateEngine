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
#include "ExpansionTemplate.hpp"
#include "Exception.hpp"
#include "Types.hpp"

namespace template_engine
{

ExpansionTemplate::ExpansionTemplate(const te_string& name, uint8_t scopeWalk) :
	_name(name),
	_scopeWalk(scopeWalk)
{
}


te_string ExpansionTemplate::render(const DictionaryPtr& dictionary, TemplateFilter filter) const
{
	DictionaryPtr currentDictionary = dictionary;
	// handle scoping
	for (uint8_t i = 0; i < _scopeWalk; i++) {
		currentDictionary = currentDictionary->getParent();
		if (nullptr == currentDictionary)
			throw TemplateException("Access to non existing parent scope");
	}
	
	// do the actual lookup
	if(currentDictionary->exists(_name) && currentDictionary->isValue(_name)) {
		if(filter)
			return filter(currentDictionary->getValue(_name));
		else
			return currentDictionary->getValue(_name);
	}

	te_converter converter;

	throw TemplateException("The name '" + converter.to_bytes(_name) + "' could not be found");
}

}
