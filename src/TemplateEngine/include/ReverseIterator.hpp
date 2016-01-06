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
#ifndef __REVERSE_ITERATOR_HPP_
#define __REVERSE_ITERATOR_HPP_

namespace template_engine
{
/** \brief Rename reverse iterator to forward iterator.
 * The C++ for interator construct always picks the forward iterater
 */
template <typename T> struct reverse_range
{
private:
	/**\brief store a reference to the object */
	T& x_;

public:
	/**\brief Contruct the reverse renamer
	 * \param x Reference to the object which should be reverse iterated
	 */
	reverse_range(T& x) : x_(x) {}

	/**\brief get the renamed rbegin iterator */
	auto begin() const -> decltype (this->x_.rbegin())
	{
		return x_.rbegin();
	}

	/**\brief get the renamed rend iterator */
	auto end() const -> decltype (this->x_.rend())
	{
		return x_.rend();
	}
};

template <typename T> reverse_range<T> reverse(T& x)
{
	return reverse_range<T>(x);
}

}
#endif // !__REVERSE_ITERATOR_HPP_
