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
#ifndef __TEMPLATE_ENGINE_HPP_
#define __TEMPLATE_ENGINE_HPP_

/* MSVC  linker settings. */
#if defined(_MSC_VER)
#    ifdef _DEBUG
#        pragma comment(lib, "TemplateEngined.lib")
#    else
#        pragma comment(lib, "TemplateEngine.lib")
#    endif
#    pragma comment(lib, "Kernel32.lib")
#endif

#include "Types.hpp"
#include "Exception.hpp"
#include "StringScanner.hpp"
#include "LookaheadScanner.hpp"
#include "Template.hpp"
#include "Dictionary.hpp"
#include "DictionaryList.hpp"


/** \brief All code in libTemplateEngine is contained within this namespace, there
 * are no nested namespaces.
 */
namespace template_engine {
}

#endif // !__TEMPLATE_ENGINE_HPP_
