/* 
 * File:    XStr.cpp
 * Author:  Conor Finegan
 * Contact: conor_finegan@student.uml.edu
 * 
 * Created on November 11, 2015, 2:20 PM
 * 
 * UPDATE 11/26/15: Copy and movement have been removed and disabled because of 
 *                  concerns regarding the deletion of resources across DLL
 *                  boundaries.
 */

#include "XStr.h"

XERCES_CPP_NAMESPACE_USE

XStr::XStr(const char* const char_str)
{
    // Convert input string from local system encoding to unicode
    unicode_form = XMLString::transcode(char_str);
}

XStr::XStr(const std::string& char_str)
{
    unicode_form = XMLString::transcode(char_str.c_str());
}

XStr::~XStr() noexcept
{
    // De-allocate the unicode string
    // If this call fails, the program will call \c std::terminate
    XMLString::release(&unicode_form);
}

const XMLCh* XStr::get_unicode_form()
{
    // Return the unicode form of the managed string
    return unicode_form;
}