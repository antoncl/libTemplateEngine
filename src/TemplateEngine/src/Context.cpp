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
#include "Context.hpp"
#include "Version.hpp"

#include <locale>

namespace template_engine {

Context::Context() : _dictionary(nullptr)
{
	add(TE_TEXT("APP"), PROJECT_NAME);
	add(TE_TEXT("VERSION"), Version.str());

	std::time_t timeNow = std::time(nullptr);		// grab time
	std::tm* tmNow = std::gmtime(&timeNow);			// convert to GMT (UTC)

	char mbstr[100];
	if (std::strftime(mbstr, sizeof(mbstr), "%c", tmNow))
	    add(TE_TEXT("TIME"), mbstr);
	else
	    add(TE_TEXT("TIME"), "");
}

}
