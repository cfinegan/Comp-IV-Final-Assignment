/* 
 * File:   main.cpp
 * Author: Jesse Heines, heines@cs.uml.edu
 *
 * Created on November 23, 2015, 10:23 AM
 */

// NOTE: Most of the starter code from this file was moved to the InputParser
//      class. See InputParser.h and InputParser.cpp for more information.

#include <iostream>  // for cout and friends
#include <sstream>   // for string streams
#include <string>    // for the STL string class
#include <regex>     // for regular expressions

#include "jmhUtilities.h"
#include "InputParser.h"

/**
 * The standard C++ main function.
 * @param argc number of command line arguments
 * @param argv array of command line arguments
 * @return 
 */
int main(int argc, char** argv)
{
    using namespace std;
    XERCES_CPP_NAMESPACE_USE
            
    // Initialize the XML4C2 system
    try
    {
        XMLPlatformUtils::Initialize();
    }

    // Print an error message and quit gracefully if initialization fails
    catch (const XMLException& err)
    {
        cerr << "Error during Xerces-C Initialization." << endl
                << "Exception: " << LSTR(err.getMessage());
        return EXIT_FAILURE;
    }
   
    // Create parser object and enter program loop
    try
    {
        // Initialize parser
        InputParser parser;

        // display program title if input object initializes
        cout << "Assignment No. 7:" << endl;
        cout << "Parsing a Command Line Using Regular Expressions" << endl;
        cout << "Author:  Conor Finegan" << endl;
        cout << "Contact: conor_finegan@student.uml.edu" << endl;
        cout << "Type \"help\" for command reference" << endl;
        
        // Run parser with command list if available
        // Autoexec is specified by argv[1]]
        if (argc > 1)
        {
            parser.Run(argv[1]);
        }
        
        // Otherwise use default Run method
        else
        {
            parser.Run();
        }
    }
    
    // Catch out-of-memory exceptions from Xerces-C
    catch (const OutOfMemoryException& err)
    {
        cerr << "Error: OutOfMemoryException" << endl;
        cerr << LSTR(err.getMessage()) << endl;
        return EXIT_FAILURE;
    }
    
    // Catch and output any other errors from Xerces-C
    catch (const DOMException& err)
    {
        cerr << "Error: DOMException code is: " << err.code << endl;
        cerr << LSTR(err.msg) << endl;
        return EXIT_FAILURE;
    }
    
    // Print diagnostic information from any STL exceptions
    catch (const std::exception& err)
    {
        cerr << "std::exception: " << err.what() << endl;
        return EXIT_FAILURE;
    }
    
    // catch any unspecified exceptions before they reach the user
    catch (...)
    {
        cerr << "Error: An unspecified exception was thrown" << endl;
        return EXIT_FAILURE;
    }
    
    // Terminates the Xerces-C runtime
    // Technically this call is optional (hence 
    //      why it's ignored in the catch blocks above)
    XMLPlatformUtils::Terminate();
    
    return EXIT_SUCCESS;
}