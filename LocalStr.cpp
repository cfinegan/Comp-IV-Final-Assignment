/* 
 * File:    LocalStr.cpp
 * Author:  Conor Finegan
 * Contact: conor_finegan@student.uml.edu
 * 
 * Created on November 26, 2015, 1:05 PM
 */

#include "LocalStr.h"

XERCES_CPP_NAMESPACE_USE

LocalStr::LocalStr(const XMLCh* const xml_str)
{
    // Convert input string from unicode to local system encoding
    local_form = XMLString::transcode(xml_str);
}

LocalStr::~LocalStr() noexcept
{
    // De-allocate the local string
    // If this call fails, the program will call \c std::terminate
    XMLString::release(&local_form);
}

const char* LocalStr::get_local_form()
{
    // Return the locally-encoded form of the managed string
    return local_form;
}

LocalStr::operator bool()
{
    // Return whether the underlying string variable is valid or not
    return (local_form != nullptr);
}

std::ostream& operator<<(std::ostream& out, const LocalStr& lstr)
{
    // operator is a friend function; can access lstr's private data
    out << lstr.local_form;
    return out;
}