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

#include <cstdlib>
#include <string>
#include <sstream>
#include "SemanticVersion.hpp"

namespace template_engine {

SemanticVersion::SemanticVersion(uint major, uint minor, uint patch, const std::string preRelease, const std::string build) :
	_major(major),
	_minor(minor),
	_patch(patch),
	_preRelease(preRelease),
	_build(build)
{
}

bool SemanticVersion::operator==(const SemanticVersion &other) const
{
	return _major == other._major &&
           _minor == other._minor &&
           _patch == other._patch &&
           _preRelease == other._preRelease;
}

bool SemanticVersion::operator>(const SemanticVersion &other) const
{
	if (_major > other._major)
	{
		return true;
	}
	else if (_major < other._major)
	{
		return false;
	}

	if (_minor > other._minor)
	{
		return true;
	}
	else if (_minor < other._minor)
	{
		return false;
	}

	if (_patch > other._patch)
	{
		return true;
	}
	else if (_patch < other._patch)
	{
		return false;
	}

	if (_preRelease > other._preRelease)
	{
		return true;
	}
	else if (_preRelease < other._preRelease)
	{
		return false;
	}

	return false;
}

void SemanticVersion::set(uint major, uint minor, uint patch, const std::string pre_release, const std::string build)
{
	_major = major;
	_minor = minor;
	_patch = patch;
	_preRelease = pre_release;
	_build = build;
}

std::string SemanticVersion::str() const
{
	std::ostringstream stream;

	stream << this->_major << '.' << this->_minor << '.' << this->_patch;

	if (this->_preRelease.length() > 0)
	{
		stream << '-' << this->_preRelease;
	}
	if (this->_build.length() > 0)
	{
		stream << '+' << this->_build;
	}

	return stream.str();
}

}
