/* 
 * File:   jmhUtilities.cpp
 * Author: Jesse Heines, heines@cs.uml.edu
 * 
 * Created on November 23, 2015, 10:46 AM
 * updated by JMH on December 4, 2015 at 4:11 PM to add the StringSplit function
 */

#include "jmhUtilities.h"
#include <string>
using namespace std ;

/**
 * NetBeans-supplied default constructor.
 */
jmhUtilities::jmhUtilities() {
}

/**
 * NetBeans-supplied copy constructor.
 */
jmhUtilities::jmhUtilities(const jmhUtilities& orig) {
}

/**
 * NetBeans-supplied destructor.
 */
jmhUtilities::~jmhUtilities() {
}


/**
 * Trim leading and trailing white space (spaces, tabs, and newlines) from the
 *    string passed as an argument and return the trimmed string.
 * This version is more sophisticated than the one above.  It uses iterators
 *    and an improved search technique.
 * @param str string to trim (a copy, so that the original is not destroyed)
 * @return a copy of the original string with leading and trailing white space removed
 */
string jmhUtilities::trim( string str ) {
  // define the iterator to be used in both loops
  string::iterator it ;

  // search for leading white space characters
  // Edited on 12/5/15 to include the '\r' character
  it = str.begin() ;
  while ( *it == ' ' || *it == '\t' || *it == '\n' || *it == '\r' ) {
    str.erase( it ) ;   // erase the found whitespace character
    it = str.begin() ;  // reposition the iterator to the first character in the string
  }

  // search for trailing white space characters
  // Edited on 12/5/15 to include the '\r' character
  it = str.end() - 1 ;
  while ( *it == ' ' || *it == '\t' || *it == '\n' || *it == '\r' ) {
    str.erase( it ) ;     // erase the found whitespace character
    it = str.end() - 1 ;  // reposition the iterator to the last character in the string
  }

  // return the result
  return str ;
}


/**
 * Compare two characters in a case-insensitive manner by converting them both to
 *    uppercase and then testing if they are equal.
 * @param a the first character to compare
 * @param b the second character to compare
 * @return true if the two uppercase characters are equal, false otherwise
   * @see https://www.safaribooksonline.com/library/view/c-cookbook/0596007612/ch04s14.html
 */
inline bool jmhUtilities::caseInsCharCompareN(char a, char b) {
  return ( toupper( a ) == toupper( b ) ) ;
}

/**
 * Compare two strings in a case-insensitive manner using the companion case-insensitive
 *    character comparison helper function.
 * @param s1 the first string to compare
 * @param s2 the second string to compare
 * @return true if the two uppercase strings are equal, false otherwise
   * @see https://www.safaribooksonline.com/library/view/c-cookbook/0596007612/ch04s14.html
 */
bool jmhUtilities::caseInsCompare( const string& s1, const string& s2 ) {
   return ( ( s1.size() == s2.size() ) &&
             equal( s1.begin(), s1.end(), s2.begin(), caseInsCharCompareN ) ) ;
}

/**
 * This function splits a string on space delimiters and returns the individual tokens
 * found in a vector.
 * @param  str    string to split on spaces
 * @param  c      character to use to split the string
 * @return vector containing string tokens found
 */
vector<string> jmhUtilities::StringSplit( string str, char c ) {
  vector<string> vec ;  // result vector to return
  
  // trim leading and trailing spaces from the string
  // str = jmhUtilities::trim( str ) ;
  
  // position of first space character found
  size_t firstSpacePosition = str.find( c ) ;

  // save each word found
  while ( firstSpacePosition != string::npos ) {
    string strExtracted = str.substr( 0, firstSpacePosition ) ;
    if ( strExtracted.length() != 0 ) {
      vec.push_back( strExtracted ) ;
    }
    str = str.substr( firstSpacePosition + 1 ) ;  // chop off the token found
    firstSpacePosition = str.find( c ) ;        // find the next space character
  }
  // push the last token onto the vector
  if ( str.compare( "" ) != 0 ) {
    vec.push_back( str ) ;
  }
  
  // return the resultant vector
  return vec ;
}