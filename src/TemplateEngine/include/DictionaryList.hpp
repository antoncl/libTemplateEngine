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
#ifndef __DICTIONARY_LIST_HPP_
#define __DICTIONARY_LIST_HPP_

#include <memory>
#include <vector>

#include "Dictionary.hpp"

namespace template_engine {

class DictionaryList;
typedef std::shared_ptr<DictionaryList> DictionaryListPtr;

/** \brief A Dictionary in it's own right, but also stores a - potentially empty - list/vector of sub-dictionaries.
 * The sub-dictionaries can be iterated over using a simple cursoring mechanism, sorry no fancy STL iterator.
 */
class DictionaryList : public Dictionary
{
	friend Dictionary;
public:
    /** Construct an empty dictionary list. */
	DictionaryList();

    /** Release all consumed resources. */
	virtual ~DictionaryList() {};

    //
	// Dictionary management and cursors
    //

    /** \brief Add a Dictionary as a sub-dictionary.
     *
     * \param dict The Dictionary to add.
     */
	void add(DictionaryPtr dict);

    /** \brief Restart the cursor at the first sub-dictionary */
	void resetCursor();

    /** \brief Advance the cursor to the next sub-dictionary */
	bool advanceCursor();

    /** \brief Get the cursor pointed to by the cursor position.
     *
     * \return The Dictionary pointed to by the cursor
     * \throw TemplateException if the list is empty or the cursor has advanced beyond the end.
     */
	const DictionaryPtr& getCurrent() const;

    /** Get the number of sub-dictionaries contained in the list */
	size_t size();

    //
	// add regular elements
	//
    /** \copydoc Dictionary::add(const te_string, const te_string) */
	virtual void add(const te_string name, const te_string value);

    /** \copydoc Dictionary::add(const te_string, DictionaryListPtr) */
	virtual void add(const te_string name, DictionaryListPtr value);

private:
	std::vector<DictionaryPtr> _dictionaries;   ///< STL container storing the sub-dictionaries.
	size_t _activeDictionary;                   ///< current cursor position.
};

// iterator class for the dictionary list
//template <typename ValueType> class DictionaryListIterator :
//	std::iterator<std::input_iterator_tag, ValueType>
//{
//public:
//    // iterator class definition goes here
//	// basic iterator
//	DictionaryListIterator(const DictionaryListIterator&);
//	~DictionaryListIterator();
//	DictionaryListIterator& operator=(const DictionaryListIterator&);
//	DictionaryListIterator& operator++(); //prefix increment
//	reference operator*() const;
//	friend void swap(DictionaryListIterator& lhs, DictionaryListIterator& rhs); //C++11 I think
//
//	// input iterator
//	DictionaryListIterator operator++(int); //postfix increment
//	ValueType operator*() const;
//	ValueType* operator->() const;
//	friend bool operator==(const DictionaryListIterator& lhs, const DictionaryListIterator& rhs);
//	friend bool operator!=(const DictionaryListIterator& lhs, const DictionaryListIterator& rhs);
//};
//
//typedef DictionaryListIterator<Dictionary> iterator_type;
//typedef DictionaryListIterator<const Dictionary> const_iterator_type;

}
#endif // !__DICTIONARY_LIST_HPP_
