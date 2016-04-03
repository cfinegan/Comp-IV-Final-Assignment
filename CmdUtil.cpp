/* 
 * File:    CmdUtil.cpp
 * Author:  Conor Finegan
 * Contact: conor_finegan@student.uml.edu
 * 
 * Created on December 4, 2015, 11:28 PM
 */

#include "CmdUtil.h"

using namespace std;

bool CmdUtil::YesNoPrompt(const std::string& message)
{
    constexpr char KEY_PROMPT[] = " (y/n) ";
    regex reYesCommand("^\\s*y(es)?\\s*$", regex::icase);
    regex reNoCommand("^\\s*no?\\s*$", regex::icase);
    
    cout << message << KEY_PROMPT;
    
    string strUserInput;
    
    for (;;) // For ever...
    {                
        // Get input from user
        getline(cin, strUserInput);
        
        // Throw error if stream fails while waiting for valid input
        if (cin.bad()) 
        { 
            throw CmdFailureException(); 
        }
        
        // Return true if user selects yes
        if (regex_match(strUserInput, reYesCommand))
        {
            return true;
        }
        
        // Return false if user selects no
        else if (regex_match(strUserInput, reNoCommand))
        {
            return false;
        }
        
        // Print an error if the user's input is neither
        cout << "Invalid input. " << message << KEY_PROMPT;
    }   
}

void CmdUtil::Output2ColumnTable(
        const std::vector<std::string>& left_strings,
        const std::vector<std::string>& right_strings,
        size_t left_width, size_t right_width)
{
    // Initialize iterators for left and right sides
    auto litr = left_strings.begin(), lend = left_strings.end();
    auto ritr = right_strings.begin(), rend = right_strings.end();   
    
    // While there are still strings
    while (litr != lend || ritr != rend)
    {
        // Vector inputs for helper function default to empty
        vector<string> left_toks, right_toks;
        
        // Fill left vector if there are mores strings
        if (litr != lend)
        {
            left_toks = jmhUtilities::StringSplit(*litr, ' ');
        }
        
        // Fill right vector if there are more strings
        if (ritr != rend)
        {
            right_toks = jmhUtilities::StringSplit(*ritr, ' ');
        }
        
        // Helper function to output this row
        OutputTableRow(left_toks, right_toks, left_width, right_width);
        
        // Increment iterators
        if (litr != lend) { ++ litr; }
        if (ritr != rend) { ++ ritr; }
    }
}

void CmdUtil::OutputTableRow(const std::vector<std::string>& left_vec, 
        const std::vector<std::string>& right_vec, 
        size_t left_width, size_t right_width)
{
    // Error message that fires if any of the tokens processed are too long
    //      to fit in their respective columns.
    // TODO: make this not crash, implement line-wrapping or something
    constexpr char TOK_TOO_LONG_ERR[] =
            "CmdUtil::OutputTableRow(): String token was too long to format";
    
    size_t lpos = 0, rpos = 0;
    
    // Initialize iterators for left and right sides
    auto litr = left_vec.begin(), lend  = left_vec.end();
    auto ritr = right_vec.begin(), rend = right_vec.end();
    
    // Outer loop: while strings still exist
    while (litr != lend || ritr != rend)
    {
        // First inner loop: print strings on left side until 
        //      there is no more space.
        while (litr != lend && (left_width - lpos) > litr->size())
        {
            // Throw if token is too long to fit
            if (litr->size() > left_width)
            {
                throw std::runtime_error(TOK_TOO_LONG_ERR);
            }
            
            // Output token and increase size/increment
            cout << *litr << ' ';
            lpos += litr->size() + 1;
            ++ litr;
        }
        
        // Fill rest of left column with white space
        while (lpos  <= left_width)
        {
            cout << ' ';
            ++ lpos;
        }
        
        // Second inner loop: print strings on left side 
        //      until there is no more space
        while (ritr != rend && (right_width - rpos) > ritr->size())
        {
            // Throw if token is too long to fit
            if (ritr->size() > right_width)
            {
                throw std::runtime_error(TOK_TOO_LONG_ERR);
            }
            
            // Output token and increase size/increment
            cout << *ritr << ' ';
            rpos += ritr->size() + 1;
            ++ ritr;
        }
        
        // Reset counters and go to next line
        lpos = 0;
        rpos = 0;
        cout << endl;
    }
}