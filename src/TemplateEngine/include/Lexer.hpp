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
#ifndef __LEXER_HPP_
#define __LEXER_HPP_

#include <utility>
#include "Scanner.hpp"
#include "LookaheadScanner.hpp"

namespace template_engine
{

/** \brief Manages lexical analysis of a template definition stream.
 *
 * Parsing a template definition is a weird process where some parts
 * of the stream (simple text) is LL(0) and other parts of the stream
 * is LL(1). In order to support situations where we go from at LL(1)
 * situation to a LL(0) situation, the lexer supports putting/pushing
 * a token back via the putTokenBack() method.
 *
 * The lexer operates in two different states. Processing and non-
 * processing. In the processing state it will work.
 *
 * A word of warning! Most lexers have a stream or vector of
 * tokens. In order to keep the memory footprint down this lexer
 * only has one, which gets re-used over and over again.
 */
class Lexer
{
public:
    /** \internal
     * \brief Present one or more UTF-16 code units as a token.
     *
     * Some tokens, start/end tags and escape sequences requires that
     * two code points are examined. Fortunately both <code>{</code>,
     * <code>}</code> and <code>\\</code> are all in the BMP. (They can
     * be represented by a single char value.)
     *
     * There are two different kinds of token readers. One which only
     * demangle escape and tag sequences. And a more elaborate one which
     * will build named tokens from white space separated strings.
     */
	struct Token {
		friend class Lexer;

        /** \brief Default is to report end of stream */
		Token() :
			_type(token_t::Eos),
			_char(TE_TEXT('\0')),
			_name()
		{ }

        /** \brief The possible types of tokens */
		enum class token_t {
			Eos,        ///< End of stream reached.
			Char,       ///< Token is a UTF-16 code unit
			StartTag,   ///< Start of processing template <code>{{</code>
			EndTag,     ///< End of processing template <code>}}</code>
			Empty,      ///< Special value used inside lexer, should never be seen
			Name,       ///< Name parsed from within a template instruction.
		};

        /** \brief get the token type */
		inline token_t getType() const { return _type; }

        /** \brief Only valid if getType() is \link token_t::Name Char \endlink*/
		inline te_char_t getChar() const { return _char; }

        /** \brief Only valid if getType() type is \link token_t::Name Name \endlink */
		inline const te_string& getName() const { return _name; }

	private:
		token_t _type;      ///< Current token type
		te_char_t _char;    ///< May be garbage if _type != \link token_t::Char Char \endlink
		te_string _name;    ///< May be garbage if _type != \link token_t::Name Name \endlink
	};

	/** Initialize a lexer with the given scanner.
	 * @param scanner	the scanner to use while tokenizing the input
	*/
	Lexer(Scanner& scanner) :
		_scanner(scanner),
		_token(),
		_inProcessingInstruction(false)
	{};

	/** \brief Advance the scanner to the next logical token, and return the token.
	*  The returned token is reused in successive calls, if a reference is held
	*  to the returned value, the values of that token may/will be changed.
	*
	* @return the token parsed from the scanner
	*/
	const Token& getNextToken();

    /** \brief Push the current Token back onto the input stream
     *
     * \param token const Token& The token to push back
     */
	void putTokenBack(const Token& token);

protected:
	/** \internal Advance the scanner to the next logical token, and return the token.
	*
	* The returned token is reused in successive calls, if a reference is held
	* to the returned value, the values of that token may/will be changed.
	* This token reader will only return simple character/tag information.
	* More complex processing is delegated to public getToken() method.
	*
	* @return the token parsed from the scanner
	*/
	const Token& getNextSimpleToken();

	/** @internal Process the more structured information between start and end tags
	 * Anything between start and end tags is considered to be some kind of
	 * processing information and can be dealt with in a more structured manner;
	*/
	const Token& getNextProcessingToken(const Token& token);

	LookaheadScanner _scanner;      ///< the Scanner to read the input from.
	Token	_token;                 ///< the highly reused token, being passed around.
	bool _inProcessingInstruction;  ///<
};

}
#endif//__LEXER_HPP_
