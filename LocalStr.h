/* 
 * File:    LocalStr.h
 * Author:  Conor Finegan
 * Contact: conor_finegan@student.uml.edu
 *
 * Created on November 26, 2015, 1:05 PM
 * 
 * This class is mostly inspired by XStr.h. It allows the user to quickly
 *      instantiate local-form C-Strings from XMLCh* strings. Clean-up is
 *      automatic when the LocalStr falls out of scope.
 */

#ifndef LOCALSTR_H
#define	LOCALSTR_H

#include <iostream>
#include <xercesc/util/XMLString.hpp>

/**
 * Wrapper class for system-local C-string that will, upon destruction,
 *      automatically deallocate its member string using the DLL-safe 
 *      \c XMLString::release method.
 * @param xml_str \c Unicode string of type \c XMLCh* to be transcoded and stored.
 * @author Conor Finegan<br>conor_finegan@student.uml.edu
 * @version 1.0
 */
class LocalStr final
{
public:
    /**
     * Constructs a \c LocalStr from a sequence of XML-formatted characters.
     * Creates and owns a copy of the input string.
     * @param xml_str XML-String to encode
     */
    LocalStr(const XMLCh* const xml_str);
        
    /**
     * Disabled copy constructor \n
     * Copying is disabled because of concerns regarding the management of
     *      resources across DLL boundaries.
     */
    LocalStr(const LocalStr&) = delete;
    
    /**
     * Disabled copy-assignment operator \n
     * Copying is disabled because of concerns regarding the management of
     *      resources across DLL boundaries. 
     */
    void operator=(const LocalStr&) = delete;
    
    /**
     * Destructor \n
     * De-allocates the managed string with a call to \c XMLString::release.\n
     * @throw The exception safety of XMLString::release is not specified in the
     *      documentation, so \c noexcept is used to enforce a no-throw 
     *      guarantee.
     */
    ~LocalStr() noexcept;
    
    /**
     * Accessor method \n
     * @return a const C-String representing the system-local form of the 
     *      managed string.
     */
    const char* get_local_form();
    
    /**
     * This operator allows a \c LocalStr variable to be referenced as a boolean
     *      expression. The return value of this operator indicates whether the
     *      underlying local string implementation is null.
     * @return \c true if this string owns a valid local string, \c false if
     *      the underlying string variable is \c null.
     */
    explicit operator bool();
    
    /**
     * Insertion operator is overloaded for easy compatibility with 
     *      \c std::ostream. Calls get_local_form() and outputs the value to 
     *      param \c out.
     * @param out  The stream being output to.
     * @param lstr The \c LocalStr to be output.
     * @return     The \c out parameter is returned by reference.
     */
    friend std::ostream& operator<<(std::ostream& out, const LocalStr& lstr);
    
private:

    /**
     * Pointer to the system-local interpretation of the string.
     */
    char* local_form = nullptr;
};

/**
 * Pre-processor macro that allows the user to quickly instantiate
 *      one-time-use, exception-safe local strings from XMLCh* literals
 *      or variables.
 */
#define LSTR(xstr_literal) LocalStr(xstr_literal).get_local_form()

#endif	/* LOCALSTR_H */

