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
#ifndef __DICTIONARY_HPP_
#define __DICTIONARY_HPP_

#include <memory>
#include <unordered_map>
#include <iostream>
#include <utility>

#include "Types.hpp"

namespace template_engine {

class DictionaryList;
class Context;

/** \brief Define a pointer to a DictionaryList */
typedef std::shared_ptr<DictionaryList> DictionaryListPtr;

class Dictionary;
/** \brief Define a pointer to a Dictionary */
typedef std::shared_ptr<Dictionary> DictionaryPtr;

/** \brief Store and lookup of elements identified by string values.
 */
class Dictionary : public std::enable_shared_from_this<Dictionary>
{
	friend DictionaryList;
	friend Context;
private:

    /** \brief The element to be stored in the Dictionary
     */
	struct Element
	{

	    /** \brief An Element can contain: */
	    enum class element_t {
		Unknown,        ///< Invalid value, only temporarily used internally
		Value,          ///< The Element is a simple string value
		List            ///< The Element is a DictionaryList
	    };
	    element_t type;     ///< The Element variant.

	    /** Anonymous union holding the specific values */
	    union {
		std::shared_ptr<DictionaryList> list;       ///< Dictionary list
		std::shared_ptr<const te_string> value;     ///< Simple string value
	    };


        /** \brief Construct a simple string value based element
         *
         * \param value const te_string The string value to store.
         *
         */
	Element(const te_string value) :
	    type(element_t::Value),
	    value(std::make_shared<const te_string>(te_string(value)))
	{}

        /** \brief Construct a dictionary list based element
         *
         * \param value const te_string The dictionary list to store.
         *
         */
	Element(DictionaryListPtr value) :
	    type(element_t::List),
	    list(value)
	{}

        /** \brief Copy constructer. */
	Element(const Element& other) : type(element_t::Unknown), value(nullptr)
	{
	    type = other.type;
	    switch(other.type) {
		case element_t::Value:
		    value = other.value;
		    break;
		case element_t::List:
		    list = other.list;
		    break;
		case element_t::Unknown:
		default:
		    break;
	    }
	};

	Element& operator=(const Element&) = delete;

        /** \brief Destructor takes the \link element_t type \endlink into consideration while releasing the objects.
         */
	~Element()
	{
		switch (type)
		{
		    case element_t::List:
			list.~shared_ptr<DictionaryList>();
			break;
		    case element_t::Value:
			value.~shared_ptr<const te_string>();
			break;
		    case element_t::Unknown:
			default:
			break;
		    }
		}
	};

public:
    /** Construct an empty Dictionary */
	Dictionary();

	/** Release all consumed resources */
	virtual ~Dictionary();

	/** \brief Get a reference to the parent of this dictionary.
	*
	* \return A pointer to the parent, may be nullptr
	*/
	const DictionaryPtr getParent()
	{
		return _parent.lock();
	}
	
	/** \brief Can the name be found in the Dictionary hierarchy.
     *
     * \param name Element name to lookup in the Dictionary hierarchy.
     * \return true if the name exists, false otherwise.
     *
     */
	virtual bool exists(const te_string& name) const;

    /** \brief does the specified name represent a simple string value.
     *
     * \param name Element name to lookup in the Dictionary hierarchy.
     * \return true if the name represents a simple string value, false otherwise.
     * \throw TemplateException if the name cannot be found.
     */
	virtual bool isValue(const te_string& name) const;

    /** \brief Return the simple string value stored with the given key.
     *
     * \param name Element name to lookup in the Dictionary hierarchy.
     * \return The simple string value represented by the key.
     * \throw TemplateException if the name cannot be found or isn't a simple string value.
     */
	virtual const te_string&  getValue(const te_string& name) const;

    /** \brief does the specified name represent a DictionaryList.
     *
     * \param name Element name to lookup in the Dictionary hierarchy.
     * \return true if the name represents a DictionaryList, false otherwise.
     * \throw TemplateException if the name cannot be found.
     */
	virtual bool isList(const te_string& name) const;

    /** \brief Return the DictionaryList value stored with the given key.
     *
     * \param name Element name to lookup in the Dictionary hierarchy.
     * \return The DictionaryList value represented by the key.
     * \throw TemplateException if the name cannot be found or isn't a DictionaryList.
     */
	virtual const std::shared_ptr<DictionaryList>& getList(const te_string& name) const;

    /** \brief Add a simple UTF-16 string value to the Dictionary.
     *
     * \param name The key to the value
     * \param value The value to store
     */
	virtual void add(const te_string name, const te_string value);

    /** \brief Add a simple UTF-8 string value to the Dictionary.
     * Note! since UTF-8 values are stored in std::string's there is
     * a risk of polluting the Dictionary with non Unicode strings.
     * \param name The key to the value
     * \param value The value to store
     */
	virtual void add(const te_string name, const std::string& value);

    /** \brief Add a DictionaryList value to the Dictionary.
     *
     * \param name The key to the list
     * \param value The list to store
     */
	virtual void add(const te_string name, DictionaryListPtr value);

private:
    /** Simplify referencing the STL container */
	typedef std::unordered_map<te_string, Element> te_dict;

	/** STL collection backing the Dictionary */
	te_dict _map;

    /** \brief Perform recursive search of the dictionary hierarchy.
     *
     * \param name The key to search for
     * \return the element matching the specified key
     * \throws TemplateException if the name cannot be located in the hierarchy.
     */
	const Element& find(const te_string& name) const;
protected:
	std::weak_ptr<Dictionary> _parent;    ///< Reference to the parent scope/dictionary (may be nullptr)

    /** \brief Set the parent scope/dictionary of this dictionary.
     *
     * \param parent The parent scope/dictionary to assign.
     */
	void setParent(const DictionaryPtr& parent)
	{
	    _parent = parent;
	}
};

}
#endif // !__DICTIONARY_HPP_
