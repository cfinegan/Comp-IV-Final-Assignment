/* 
 * File:    XStr.h
 * Author:  Conor Finegan
 * Contact: conor_finegan@student.uml.edu
 *
 * Created on November 11, 2015, 2:20 PM
 * 
 * This class is mostly copy/pasted from the "CreateDOMDocument.cpp" file 
 *      handed out in class (see footnote 1 for online source).
 * Documentation has been altered to conform with Javadoc syntax. Multiple
 *      constructors have also been added, to support the STL string class
 *      and also avoid copy/move errors
 * 
 * UPDATE 11/26/15: Copy and movement have been removed and disabled because of 
 *                  concerns regarding the deletion of resources across DLL
 *                  boundaries.
 */

#ifndef XSTR_H
#define	XSTR_H

#include <xercesc/util/XMLString.hpp>
#include <string>

/**
 * Wrapper class for a Unicode-form C-String that will, upon destruction,
 *      automatically deallocate its member string using the DLL-safe 
 *      \c XMLString::release method.
 * @param char_str system-local string of type \c char* to be transcoded and stored.
 * @author Conor Finegan<br>conor_finegan@student.uml.edu
 * @version 1.1
 */
class XStr final
{
public:
    /**
     * Constructs an \c XStr from a sequence of system-local characters. Creates
     *      and owns a copy of the input string.
     * @param char_str C-String to encode
     */
    XStr(const char* const char_str);
    
    /**
     * Alternative constructor that takes an \c std::string as an argument.
     *      Equivalent to calling \c char_str.c_str() .
     * @param char_str String to encode
     */
    XStr(const std::string& char_str);
    
    /**
     * Disabled copy constructor \n
     * Copying is disabled because of concerns regarding the management of 
     *      resources across DLL boundaries.
     */
    XStr(const XStr&) = delete;
    
    /**
     * Disabled copy-assignment operator \n
     * Copying is disabled because of concerns regarding the management of 
     *      resources across DLL boundaries.
     */
    void operator=(const XStr&) = delete;
    
    /**
     * Destructor \n
     * De-allocates the managed string with a call to \c XMLString::release.\n
     * @throw The exception safety of \c XMLString::release is not specified in the 
     *      documentation, so \c noexcept is used to enforce a no-throw
     *      guarantee.
     */
    ~XStr() noexcept;
   
    /**
     * Accessor method \n
     * @return a const XML C-String representing the unicode form of the 
     *      managed string.
     */
    const XMLCh* get_unicode_form();
    
private:
    
    /**
     * Pointer to the Xerces-c compatible Unicode interpretation of the string.
     */
    XMLCh* unicode_form = nullptr;
};

/**
 * Pre-processor macro that allows the user to quickly instantiate
 *      one-time-use, exception-safe XML Strings from char* literals
 *      or variables.
 */
#define XSTR(str_literal) XStr(str_literal).get_unicode_form()

#endif	/* XSTR_H */

// Footnotes:
// [1]: https://teaching.cs.uml.edu/~heines/91.204/91.204-2014-15f/204-lecs/code/XercesSamples/CreateDOMDocument/CreateDOMDocument-listing.txt