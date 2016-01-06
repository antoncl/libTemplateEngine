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
#ifndef __VERSION__HPP_
#define __VERSION__HPP_

#include <string>

namespace template_engine {

typedef unsigned int uint;

/** \brief Utility class used for reporting the current version of the library.
 * Versioning is based on semantic versioning. http://semver.org/
 */
class SemanticVersion
{
public:
    /** \brief Create an/the instance of the library version
     *
     * \param major      uint Incremented when the API changes
     * \param minor      uint Incremented with new backwards compatible features.
     * \param patch      uint Incremented with backwards compatible bugfixes.
     * \param preRelease std::string Text tag, e.g. "alpha", "beta", "rc", etc.
     * \param build      std::string Any tag, e.g. "mt", "st" or a hex-string
     */
	SemanticVersion(uint major, uint minor, uint patch, const std::string preRelease = "", const std::string build = "");

    /** \brief Are two different semantic versions identical?
     * see http://semver.org/ for comparison rules.
     *
     * \param other const SemanticVersion& The right hand side of the operator.
     * \return bool true if they are identical, false otherwise.
     */
	bool operator==(const SemanticVersion& other) const;

    /** \brief Is this > other ?
     *
     * \param other const SemanticVersion& The right hand side of the operator
     * \return bool true if this is > than other.
     *
     */
	bool operator>(const SemanticVersion& other) const;

    /** \brief Assign a new value to this version
     *
     * \param major uint Incremented when the API changes
     * \param minor uint Incremented with new backwards compatible features.
     * \param patch uint Incremented with backwards compatible bugfixes.
     * \param preRelease const std::string Text tag, e.g. "alpha", "beta", "rc", etc.
     * \param build      const std::string Any tag, e.g. "mt", "st" or a hex-string
     */
	void set(uint major, uint minor, uint patch, const std::string preRelease = "", const std::string build = "");

	std::string str(void) const;

private:
	uint _major;                //!<Incremented when the API changes
	uint _minor;                //!<Incremented with new backwards compatible features.
	uint _patch;                //!<Incremented with backwards compatible bugfixes.
	std::string _preRelease;    //!<Text tag, e.g. "alpha", "beta", "rc", etc.
	std::string _build;         //!<Any tag, e.g. "mt", "st" or a hex-string
};

}
#endif // !__VERSION__HPP_

