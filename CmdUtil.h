/* 
 * File:    CmdUtil.h
 * Author:  Conor Finegan
 * Contact: conor_finegan@student.uml.edu
 *
 * Created on December 4, 2015, 11:28 PM
 */

#ifndef CMDUTIL_H
#define	CMDUTIL_H

#include "jmhUtilities.h"

#include <iostream>
#include <string>
#include <regex>
#include <stdexcept>
#include <iomanip>

/**
 * The \c CmdUtil class is a repository of static methods and class/type
 *      definitions that may be of use when developing software that continually
 *      polls for user input using \c stdin. 
 * @author Conor Finegan<br>conor_finegan@student.uml.edu
 * @version 1.0
 */
class CmdUtil final
{
public:    
    
    /**
     * Prompts the user with a user-defined message and the prompt "(y/n)". 
     *      The user must enter either 'y' or 'yes' to exit the loop with a 
     *      return value of \c true, or enter 'n' or 'no' to exit the loop with
     *      a return value of \c false. The prompt is not case-sensitive. If
     *      the user enters an invalid string, the prompt will output "Invalid
     *      input", then repeat \c message. 
     * @param message The prompt to display to the user. This string should
     *      make sense in context as a "yes or no" proposition. The string 
     *      " (y/n) " is appended to the end of message when printing, so no
     *      white-space formatting at the end of \c message is required. 
     * @return \c true if the user answers "yes", or \c false if the user
     *      answers "no".
     */
    static bool YesNoPrompt(const std::string& message);
    
    /**
     * Output a formatted 2-column table to standard out. This function will
     *      automatically provides formatting so that words don't incorrectly
     *      wrap around to the next line. Note that the behavior of this
     *      function if it encounters a word longer than its column length
     *      is undefined. 
     * @param left_strings  vector of strings to display on the LHS
     * @param right_strings vector of strings to display on the RHS
     * @param left_width    width of the LHS column
     * @param right_width   width of the RHS column
     * @throw \c std::runtime_exception (from \c OutputTableRow If the method 
     *      encounters a string too long to format.
     */
    static void Output2ColumnTable(const std::vector<std::string>& left_strings,
            const std::vector<std::string>& right_strings, 
            const size_t left_width, const size_t right_width);
        
    /**
     * Most errors in C++ stream system are not considered critical failures,
     *      but some situations, failure to process input should be considered
     *      exceptional behavior. This class is a pre-made exception,
     *      deriving from \c std::runtime_error, that contains a message explaining 
     *      that the system encountered an error parsing for user input. This class 
     *      is most useful in situations where stream failure can't be 
     *      recovered from by more benign means (returning null, breaking a loop, etc).
     */
    class CmdFailureException : public std::runtime_error
    {
    public: 
        /**
         * Default constructor, instantiates the object with \c what()
         *      indicating that a stream error occurred.
         */
        inline CmdFailureException() : std::runtime_error(
                "The system experienced a failure parsing for user input") {}
    };
    
private:
    /**
     * Construction of \c CmdUtil objects is prohibited, as this class contains
     *      only static data.
     */
    CmdUtil() = delete;
    
    /**
     * Helper function for Output2ColumnTable. Prints the individual tokens
     *      for a row in the table.
     * @param left_vec    string tokens to print on LHS
     * @param right_vec   string tokens to print on RHS
     * @param left_width  width of the LHS column
     * @param right_width width of the RHS column
     * @throw \c std::runtime_exception If the method encounters a string too
     *      long to format.
     */
    static void OutputTableRow( 
            const std::vector<std::string>& left_vec, 
            const std::vector<std::string>& right_vec, 
            size_t left_width, size_t right_width);
};

#endif	/* CMDUTIL_H */

