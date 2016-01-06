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
#include "RepeatTemplate.hpp"
#include "DictionaryList.hpp"
#include "Exception.hpp"
#include "Types.hpp"

namespace template_engine
{

RepeatTemplate::RepeatTemplate(te_string name, std::shared_ptr<Template> templ) :
	_name(name),
	_templ(templ)
{
}

te_string RepeatTemplate::render(const DictionaryPtr& dictionary, TemplateFilter filter) const
{
	te_string result;

	if (!(dictionary->exists(_name) && dictionary->isList(_name))) {
		te_converter converter;

		throw TemplateException("The list '" + converter.to_bytes(_name) + "'could not be found");
	}

	DictionaryListPtr list = dictionary->getList(_name);

	list->resetCursor();

	for (size_t i = 0; i < list->size(); i++) {
		result += _templ->render(list->getCurrent(), filter);
		list->advanceCursor();
	}

	return result;
}

}