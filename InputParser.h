/* 
 * File:    InputParser.h
 * Author:  Conor Finegan
 * Contact: conor_finegan@student.uml.edu
 *
 * Created on November 24, 2015, 12:19 PM
 */

#ifndef INPUTPARSER_H
#define	INPUTPARSER_H

#include "PointerTypedefs.h"
#include "jmhUtilities.h"
#include "XStr.h"
#include "LocalStr.h"
#include "CmdUtil.h"
#include "DOMUtil.h"

#include <sstream>
#include <string>
#include <regex>
#include <iostream>
#include <stdexcept>    // for runtime_error
#include <cassert>      // for cassert
#include <iomanip>      // for setw
#include <fstream>      // for ifstream--

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/framework/StdOutFormatTarget.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/util/XMLString.hpp>

/**
 * This class continually prompts the user for commands, processing each one
 *      using regular expressions. These commands allow the user to manipulate
 *      a DOM document, and are implemented using the Xerces-C library. To test
 *      the result of many commands quickly, call Run() with a \c std::ostream 
 *      (or \c std::string naming a text file) containing these commands. 
 *      They will be executed automatically.
 * @author Conor Finegan<br>conor_finegan@student.uml.edu
 * @version 1.0
 */
class InputParser
{
public:
    
    /**
     * Default constructor. Calls Initialize().
     * @throw DOMException: Exceptions thrown by 
     *      \c DOMImplementation::createDocument, see:
     *      https://xerces.apache.org/xerces-c/apiDocs-3/classDOMImplementation.htm
     * @throw std::runtime_error: If the factory methods for \c impl or \c doc 
     *      return null.
     */
    InputParser();
    
    /**
     * First of 3 variants of the \c Run method. When called with no
     *      parameters, the \c Run method repeatedly prompts the user for
     *      commands, processing them and reporting the results. This method
     *      returns when the user enters the "Quit" command.
     */
    void Run();
    
    /**
     * Second of the 3 \c Run variants. When called with an \c std::ostream as
     *      an argument, the \c Run method will process lines of input from the
     *      stream until it fails or reaches EOF. Control will then be given
     *      to the user with a parameterless call to \c Run. This method will
     *      return early if a "Quit" command is read from the stream.
     * @param exec the \c std::istream to read commands from
     */
    void Run(std::istream& exec);
    
    /**
     * Third version of \c Run. When called with an \c std::string as an
     *      argument, the \c Run method will attempt to open a file at the path
     *      specified in \c exec_filename. If the stream is constructed in a
     *      valid state, it will be passed to \c Run(std::istream&) to be used
     *      as an Autoexec. Otherwise \c Run is called with no parameters. A
     *      message is output indicating whether the file was successfully 
     *      opened.
     * @param exec_filename the path to the file being opened
     */
    void Run(const std::string& exec_filename);
    
private:
    
    /**
     * Initializes the \c InputReader by calling the factory methods for
     *      \c impl and \c doc.
     * @throw DOMException: Exceptions thrown by 
     *      \c DOMImplementation::createDocument, see:
     *      https://xerces.apache.org/xerces-c/apiDocs-3/classDOMImplementation.html
     * @throw std::runtime_error: If the factory methods for \c impl or \c doc 
     *      return null.
     */
    void Initialize();
    
    /**
     * Parses an individual command and processes it by calling 
     *      \c ParseCommandString
     * @param strUserEntry The command to be parsed
     * @return \c false if a valid "Quit" command was processed, \c true otherwise.
     *      Note that this method still returns \c true for invalid commands, the
     *      return value of this method indicates whether the user would like to 
     *      stay in the command loop.
     */
    bool ParseCommand(const std::string& strUserEntry);
    
    /**
     * Check for a valid basic command. If the command can't be identified
     *      as any of the basic types, a simple help message will be printed.
     * @param strUserEntry command string entered by the user
     */
    void ParseCommandString(const std::string& strUserEntry) ;
    
    /**
     * Handle an add command entered by the user. This function distinguishes
     *      the "element" commands from the "attribute" commands, and will also
     *      output an error message if the "add" command fits neither valid
     *      category.
     * @param strUserEntry command string entered by the user
     */
    void ProcessAddCommand(const std::string& strUserEntry) ;
    
    /**
     * Handle an add element command entered by the user. This method pulls
     *      the individual arguments out of the string and passes them to
     *      <code>AddElement</code>. An error message will be printed if the
     *      input string is not well-formed.
     * @param strUserEntry command string entered by the user
     */
    void ProcessAddElementCommand(const std::string& strUserEntry) ;
    
    /**
     * Handle an add attribute command entered by the user. This method pulls
     *      the individual arguments out of the string and passes them to
     *      <code>AddAttribute</code>. An error message will be printed if the
     *      input string is not well-formed.
     * @param strUserEntry command string entered by the user
     */
    void ProcessAddAttributeCommand(const std::string& strUserEntry);
    
    /**
     * Analyzes a "delete" command entered by the user to determine if it's
     *      well-formed or not. If it is well formed, the element name
     *      parameter is passed to DeleteElement().
     * @param strUserEntry command string entered by the user.
     */
    void ProcessDeleteCommand(const std::string& strUserEntry);
    
    /**
     * Analyzes a "print" command to determine if it is valid.
     *      If it is valid, check for an argument for the root node
     *      and display the appropriate document or document sub-tree to
     *      standard out.
     * @param strUserEntry command string entered by the user.
     */
    void ProcessPrintCommand(const std::string& strUserEntry);
    
    /**
     * Analyzes a "write" command to determine if it's well-formed. If it is,
     * the file path parameter is passed to WriteToFile().
     * @param strUserEntry command string entered by the user.
     */
    void ProcessWriteCommand(const std::string& strUserEntry);
    
    /**
     * Uses \c DOMTreeWalker to print the current document to standard out. This
     *      method visits each direct child of the root and uses them to call
     *      the recursive helper method <code>StdoutPrintCommand</code>. If the
     *      document is empty, an error message will be printed.
     */
    void OutputDocument() ;  
    
    /**
     * Add an individual element to the tree structure. This method is called
     *      after the regular expression from the user has been split into
     *      individual arguments, but before those arguments have been
     *      associated with specific nodes in the DOM structure. If no node with 
     *      a name matching \c parent_name can be found in the the structure,
     *      then this method will print an error message and return. If one or
     *      more nodes are found, then the behavior at this time to append the
     *      child node to the first parent found. If no child content was specified
     *      by the user, then \c child_content will be empty, and a \c DOMText
     *      object will not be appended to the child.
     * @param parent_name   \c std::string specifying the name of the parent
     * @param child_name    \c std::string specifying the name of the child
     * @param child_content \c std::string specifying the content of the child 
     */
    void AddElement(const std::string& parent_name, 
            const std::string& child_name, const std::string& child_content);
        
    /**
     * Attempts to output the current document to a file located at the path   
     *      specified by <code>filename</code>. This method will print a message
     *      indicating whether the output operation succeeded or failed.
     * @param filename Path to the file being output to.
     * @throw std::runtime_error If the factory methods for the serializer or
     *      output description return null.
     */
    void WriteToFile(const std::string& filename);
    
    /**
     * Add an individual attribute to an element of the tree structure. This
     *      method is called after the regular expression from the user has been
     *      split into individual arguments, but before those arguments have
     *      been associated with specific nodes in the DOM structure. If no
     *      element with a name matching \c parent_name can be found, then this
     *      method will print an error message and return. If one or more elements 
     *      are found, then the behavior at this time to append the attribute
     *      to the first element found.
     * @param parent_name \c std::string specifying the name of the parent.
     * @param attr_name   \c std::string specifying the key of the attribute.
     * @param attr_val    \c std::string specifying the value of the attribute.
     */
    void AddAttribute(const std::string& parent_name, 
            const std::string& attr_name, const std::string& attr_val);
    
    /**
     * Prints a menu detailing the various commands available for use.
     */
    void ProcessHelpCommand();
    
    /**
     * Given a node name, this method will find any nodes with matching names,
     *      prompt the user to select one (if more than one node matches),
     *      then attempt to delete that node from the document by invoking
     *      \c DOMUtil::RemoveNodeFromParent(). If no node can be found matching
     *      the input name, or if there is an error attempting to remove the
     *      target node from the document, then this method will output an 
     *      error message informing the user.
     * @param node_name Name of the Element to find and delete.
     */
    void DeleteElement(const std::string& node_name);
    
    /**
     * Given a node name, this method will find any nodes with matching names,
     *      prompt the user to select one (if more than one node matches),
     *      then recursively output the node and its children to standard
     *      output by invoking \c DOMUtil::OutputDocSubTree(). If a node with 
     *      the specified name cannot be found, then this method will output
     *      an error message informing the user.
     * @param root_name Name of the root element of the sub-tree.
     */
    void PrintSubTree(const std::string& root_name);
    
    /**
     * Individual DOM Implementations are represented by a singleton. 
     */
    XERCES_CPP_NAMESPACE::DOMImplementation* impl = nullptr;
    
    /**
     * The pointer representing the document is owned by a smart pointer that
     *      will automatically call \c release on the document upon destruction.
     */
    DocumentPtr doc = nullptr;
    
};

#endif	/* INPUTPARSER_H */

