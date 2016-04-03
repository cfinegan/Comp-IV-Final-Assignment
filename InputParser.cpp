/* 
 * File:    InputParser.cpp
 * Author:  Conor Finegan
 * Contact: conor_finegan@student.uml.edu
 * 
 * Created on November 24, 2015, 12:19 PM
 */

#include "InputParser.h"

XERCES_CPP_NAMESPACE_USE
using namespace std;

InputParser::InputParser()
{
    Initialize();
}

void InputParser::Run()
{
    string strUserEntry;
    
    do
    {
        cout << "\nYour command: ";
        getline(cin, strUserEntry);
        
        // Throw exception if input loop is interrupted by stream failure
        if (cin.bad()){ throw CmdUtil::CmdFailureException(); }
    }
    while (ParseCommand(strUserEntry));
}

void InputParser::Run(std::istream& exec)
{
    string execLine;

    while (getline(exec, execLine))
    {
        // Skip empty lines
        if (execLine.size() > 1)
        {
            // output line to console then parse it
            // A quit command from the autoexec will halt the program
            cout << "\nAutoexec: " << execLine << endl;
            if (!ParseCommand(execLine)) { return; }
        }
    }
    
    // Go to user input loop after processing all commands from file
    Run();
}

void InputParser::Run(const std::string& exec_filename)
{
    // Open input file for reading only
    ifstream fin(exec_filename.c_str(), ios::in);
    
    cout << endl;
    
    if (fin) // Stream was opened successfully
    {
        cout << "Autoexec successfully loaded from file: " 
                << exec_filename << endl;
        
        Run(fin);
    }
    
    else // There was a problem opening the stream
    {
        cout << "Error: Failed to load autoexec from file: "
                << exec_filename << endl;
        
        Run();
    }
}

/////////////////////////////////
// Private InputParser Methods //
/////////////////////////////////

void InputParser::Initialize()
{
    // Get pointer to DOM implementation
    impl = DOMImplementationRegistry::getDOMImplementation(XSTR("Core"));
    
    if (!impl)
    {
        // Throw an exception if the factory method for \d impl returns null
        throw std::runtime_error("Requested feature level not supported");
    }
        
    // Create DOM document with pre-assigned root node
    doc.reset(impl->createDocument(0, XSTR("Assignment7"), 0));
    
    if (!doc)
    {
        // Throw an exception if the factory method for \c doc returns null. 
        //      InputParser cannot be constructed in valid state if doc is
        //      not constructed
        throw std::runtime_error("An unspecified error occurred creating the document.");
    }
}

bool InputParser::ParseCommand(const std::string& strUserEntry)
{
    // Regex identifying a quit command. "Exit" is also accepted as a keyword
    regex reQuit("^\\s*(quit|exit)\\s*$", regex::icase);
    
    // if the user didn't enter 'quit', parse the command string
    //      and return true
    if (!regex_match(strUserEntry, reQuit))
    {
        ParseCommandString(jmhUtilities::trim(strUserEntry));
        return true;
    }
    
    // Return false if it's time to quit
    return false;
}

void InputParser::ParseCommandString(const std::string& strUserEntry)
{    
    cmatch what;
    
    //regular expressions for basic commands
    regex reBasicAddCommand("\\s*add($|(\\s+.*))", regex::icase);
    regex reBasicPrintCommand("\\s*print($|(\\s+.*))", regex::icase);
    regex reBasicWriteCommand("\\s*write($|(\\s+.*))", regex::icase);
    regex reBasicHelpCommand("\\s*help\\s*", regex::icase);
    regex reBasicDeleteCommand("\\s*del(ete)?($|(\\s+.*))", regex::icase);
    
    // test for each basic command in turn
    if (regex_match(strUserEntry, reBasicAddCommand))
    {
        // Add
        ProcessAddCommand(strUserEntry);
    }
    
    else if (regex_match(strUserEntry, reBasicPrintCommand))
    {
        // Print
        ProcessPrintCommand(strUserEntry);
    }
    
    else if (regex_match(strUserEntry, reBasicWriteCommand))
    {
        // Write to file
        ProcessWriteCommand(strUserEntry);
    }
    
    else if (regex_match(strUserEntry, reBasicHelpCommand))
    {
        // Help
        ProcessHelpCommand();
    }
    
    else if (regex_match(strUserEntry, reBasicDeleteCommand))
    {
        // Delete
        ProcessDeleteCommand(strUserEntry);
    }
    
    else
    {
        cout << "Invalid command.  Acceptable commands are \"add\", \"print\", "
                "\"write\", \"delete\", \"help\", and \"quit\"." << endl;
    }
}

void InputParser::ProcessAddCommand(const std::string& strUserEntry)
{
    // regular expressions for the second parameter in the add command
    regex reAddElementCommand("\\s*add\\s+element($|(\\s.*))", regex::icase);
    regex reAddAttributeCommand("\\s*add\\s+attribute($|(\\s.*))", regex::icase);

    // test for each possible second parameter in turn
    if (regex_match(strUserEntry, reAddElementCommand))
    {
        ProcessAddElementCommand(strUserEntry);
    }
    
    else if (regex_match(strUserEntry, reAddAttributeCommand))
    {
        ProcessAddAttributeCommand(strUserEntry);
    }
    
    else // Output error if command is invalid
    {
        cout << "Invalid \"add\" command: 2nd parameter must be \"element\" "
                "or \"attribute\"." << endl;
        cout << "Valid signatures for add are:" << endl;
        cout << "> add element [parent name] [child name]" << endl;
        cout << "> add element [parent name] [child name] "
                "[child content]" << endl;
        cout << "> add attribute [parent name] [key] [value]" << endl;
    }
}

void InputParser::ProcessAddElementCommand(const std::string& strUserEntry)
{
    // the what variable is actually an array that will be populated by the 
    //      regex_match function when matched groups are found
    cmatch what;
    // what[0] contains the entire matched string
    // what[1] contains the first matched group
    // what[2] contains the second matched group
    // what[3] etc.

    // regular expression to pick out the name of the parent to which the new 
    //      element is to be added and the name of the new element itself
    regex reAddElementCmd(
            "^\\s*add\\s*element\\s*(\\w+)\\s*(\\w+)(\\s*((\\w|\\d)+))?\\s*$", 
            regex::icase);

    // note that the following variant of the regex_match command requires 
    //      a C string, not an STL string
    if (regex_match(strUserEntry.c_str(), what, reAddElementCmd))
    {        
        AddElement(what[1], what[2], what[4]);
    }
    
    else // Output error if command is invalid
    {        
        cout << "Invalid \"add element\" command. Valid signatures for "
                "add element are:" << endl;
        cout << "> add element [parent name] [child name]" << endl;
        cout << "> add element [parent name] [child name] "
                "[child content]" << endl;
    }
}

void InputParser::ProcessAddAttributeCommand(const std::string& strUserEntry)
{
    // the what variable is actually an array that will be populated by the 
    //    regex_match function when matched groups are found
    cmatch what;
    // what[0] contains the entire matched string
    // what[1] contains the first matched group
    // what[2] contains the second matched group
    // what[3] etc.

    // regular expression to pick out the name of the element to which the 
    //     new attribute is to be added, the name of the new attribute, and the 
    //     value of that attribute
    regex reAddAttributeCmd(
            "^\\s*add\\s*attribute\\s*(\\w+)\\s*(\\w+)\\s*(\\w+)\\s*$", 
            regex::icase);

    // note that the following variant of the regex_match command requires 
    //      a C string, not an STL string
    if (regex_match(strUserEntry.c_str(), what, reAddAttributeCmd))
    {
        AddAttribute(what[1], what[2], what[3]);
    }
    
    else // Output error if command is invalid
    {        
        cout << "Invalid \"add attribute\" command. Valid signature for "
                "add attribute is:" << endl;
        cout << "> add attribute [parent name] [key] [value]" << endl;
    }
}

void InputParser::ProcessDeleteCommand(const std::string& strUserEntry)
{
    // Regex for a valid delete command requires exactly one argument
    regex reFullDeleteCommand("^\\s*del(ete)?\\s+(\\w+)\\s*$");
    cmatch what;
    
    if (regex_match(strUserEntry.c_str(), what, reFullDeleteCommand))
    {
        DeleteElement(what[2]);
    }
    
    else // Output error if command is invalid
    {
        cout << "Invalid \"delete\" command. Valid signature for delete is:" << endl;
        cout << "> delete [element name]" << endl;
    }
}

void InputParser::ProcessPrintCommand(const std::string& strUserEntry)
{    
    // Print command requires either zero or one argument, with certain
    //      non-word characters allowed for the second argument (characters
    //      commonly used in file names).
    regex rePrintFullCommand("^\\s*print(\\s+(\\w+))?\\s*$", regex::icase);
    cmatch what;
    
    // If match is found, then it is a valid command
    if (regex_match(strUserEntry.c_str(), what, rePrintFullCommand))
    {        
        // what[2] is the match for the optional element name.
        if (what[2].str().empty())
        {
            OutputDocument();
        }
        
        else
        {
            PrintSubTree(what[2]);
        }             
    }
    
    // Else print helper message for 'print' syntax
    else
    {
        cout << "Invalid print command. The optional parameter specifies "
                "the node to treat as the root of the output." << endl;
        cout << "Valid signatures for print are:" <<  endl;
        cout << "> print" << endl;
        cout << "> print [root name]" << endl;
    }
}

void InputParser::ProcessWriteCommand(const std::string& strUserEntry)
{
    // Full command for write. Allows periods and underscores in addition to
    //      word characters for file names.
    regex reWriteFullCommand("^\\s*write\\s+((\\w|\\.|_)+)\\s*$");
    cmatch what;
    
    if (regex_match(strUserEntry.c_str(), what, reWriteFullCommand))
    {
        WriteToFile(what[1]);
    }
    
    else
    {
        cout << "Invalid Write Command. Valid signature for write is:" << endl;
        cout << "> write [file path]" << endl;
        
    }
}

void InputParser::OutputDocument()
{
    using namespace std;
    
    // Get root from document
    DOMElement* root = doc->getDocumentElement();
    
    // Create tree walker that will visit all nodes
    TreeWalkerPtr walker(doc->createTreeWalker(
            root, DOMNodeFilter::SHOW_ALL, nullptr, true));
    
    // Initial call to nextNode() as required by the documentation
    DOMNode* node = walker->nextNode();
    
    // Check for empty tree before proceeding
    if (!node)
    {
        cout << "Document is empty." << endl;
        return;
    }
    
    // While there are still nodes...
    while (node != nullptr)
    {
        // Each sibling is treated as a node and passed to the recursive
        //      helper function
        DOMUtil::OutputDocSubTree(0, node);
        node = walker->nextSibling();
    }
}

void InputParser::AddElement(const std::string& parent_name, 
        const std::string& child_name, const std::string& child_content)
{
    // Special syntax used to specify the root. As a side-effect, elements with
    //      the name "root" are not supported.
    regex rootElementSpecifier("^\\s*root\\s*$", regex::icase);
    
    // Root node of the current document
    DOMNode* root = doc->getDocumentElement();
    
    // Node being appended to. Initialized to null to indicate that 
    //      there is no default parent
    DOMNode* parent = nullptr;
    
    // If user specified root, set parent to root
    if (regex_match(parent_name.c_str(), rootElementSpecifier))
    {
        parent = root;
    }

    // Else search the tree for elements matching the specified name
    else
    {        
        DOMNodeList* elements = doc->getElementsByTagName(XSTR(parent_name));
        
        parent = DOMUtil::SelectNodeFromList(elements);
    }
    
    // If parent points to valid node, create child and append it
    if (parent)
    {
        // Create and append child
        DOMElement* child = doc->createElement(XSTR(child_name));
        parent->appendChild(child);
        
        cout << "Element \"" << child_name << "\" ";
        
        // Only add content to child if there is content to add
        if (!child_content.empty())
        {
            //string trim_content = jmhUtilities::trim(child_content);
            DOMText* child_text = doc->createTextNode(XSTR(child_content));
            child->appendChild(child_text);
            cout << "with content \"" << child_content << "\" ";
        }
        
        // If there is no content, reflect this in the output message
        else
        {
            cout << "(with no content) ";
        }
                
        cout << "was successfully appended to parent Element \""
                << LSTR(parent->getNodeName()) << "\"." << endl;
    }
    
    // Print error message if parent can't be found
    else
    {
        cout << "No element with the name \"" << parent_name
                << "\" could be found." << endl;
    }
}

void InputParser::WriteToFile(const std::string& filename)
{
    // Get serializer from DOM implementation
    LSSerializerPtr serializer(impl->createLSSerializer());
    
    // Get output description from DOM implementation
    LSOutputPtr output_desc(impl->createLSOutput());
        
    // Operation result defaults to false (in the case that 
    //      serializer-write() is never called)
    bool write_result = false;
    
    // Only attempt to write to the file if the serializer and output 
    //      description were successfully constructed.
    if (serializer && output_desc)
    {
        // Set serializer to output to the file specified
        unique_ptr<XMLFormatTarget> format_target(
                new LocalFileFormatTarget(filename.c_str()));

        output_desc->setByteStream(format_target.get());
        
        // Write doc to output
        write_result = serializer->write(doc.get(), output_desc.get());
    }
       
    // Output whether serialization was successful or not    
    cout << (write_result ? 
            "The document was successfully written to the file: \"" :
            "There was an error writing the document to the file: \"") 
            << filename << "\"" << endl;
}

void InputParser::AddAttribute( const std::string& parent_name,
        const std::string& attr_name, const std::string& attr_val)
{    
    // Get collection of elements
    DOMNodeList* elements = doc->getElementsByTagName(XSTR(parent_name));
    
    // Cast result to a DOMElement so that we can access addAttribute()
    DOMElement* parent = 
            static_cast<DOMElement*>(DOMUtil::SelectNodeFromList(elements));
    
    // Only append if parent could be found
    if (parent)
    {  
        parent->setAttribute(XSTR(attr_name), XSTR(attr_val));
        
        cout << "Attribute \"" << attr_name << "\" with value \"" << attr_val
                << "\" was successfully added to Element \"" << parent_name
                << "\"." << endl;
    }
    
    // If element couldn't be found, print error message
    else
    {
        cout << "No element with the name \"" << parent_name
                << "\" could be found." << endl;
    }
}

void InputParser::ProcessHelpCommand()
{               
    // Collection of command names
    vector<string> cmd_names =
    {
        "add element",
        "add attribute",
        "print",
        "write",
        "delete",
        "help",
        "print"
    };
    
    // Collection of command descriptions
    vector<string> cmd_desc = 
    {
        "Add Element [parent name] [child name] <child content?>: Adds an  "
        "element with the specified name to the specified parent. Optionally, "
        "element content can be specified. \"Specify \"root\" as the parent "
        "name to add a child to the root."
        ,
        "Add Attribute [parent name] [key] [value]: Adds an attribute with the "
        "specified key/value pair to the specified parent."
        ,
        "Print <node name?>: Outputs the document to standard out. If "
        "a    <node name> is specified, it will be used as the root of the "
        "output document."
        ,
        "Write [file path]: Outputs the current document to a file "
        "specified by [file path]."
        ,
        "Delete [element name]: Finds the element specified by [element name], "
        "removes it from the document, then frees it from memory."
        ,
        "Help: Displays this help menu."
        ,
        "Quit: \"quit\" or \"exit\" may be used to quit the program."
    };
    
    // Always set this to the longest string
    size_t lhs_size = sizeof("add attribute");
    
    // Invoke output column method
    // Print assumes an 80 character wide display for standard out
    CmdUtil::Output2ColumnTable(cmd_names, cmd_desc, lhs_size, 80 - lhs_size);
}

void InputParser::DeleteElement(const std::string& node_name)
{    
    // Get all elements matching the input name
    DOMNodeList* elements = doc->getElementsByTagName(XSTR(node_name));
    
    // If there are multiple nodes of the same name, user chooses one
    DOMNode* target = DOMUtil::SelectNodeFromList(elements);
    
    // If node is valid, attempt to delete it
    if (target)
    {        
        // Give user chance to opt out before permanently deleting data
        bool del = CmdUtil::YesNoPrompt(
                "Are you sure you would like to delete this node?");
                
        if (del) // If user wishes to delete...
        {
            if (DOMUtil::RemoveNodeFromParent(target))
            {
                cout << "Successfully deleted element \"" 
                        << node_name << "\"" << endl;
            }
            else
            {
                cout << "Error: Element \"" << node_name << "\" could not be "
                        "removed from the document." << endl;
            }
        }
    }
        
    else // If no nodes with name node_name
    {
        cout << "No element with the name \"" << node_name
                << "\" could be found." << endl;
    }
}

void InputParser::PrintSubTree(const std::string& root_name)
{    
    // Get collection of elements matching root_name
    DOMNodeList* elements = doc->getElementsByTagName(XSTR(root_name));

    // Get input from user if there are multiple matches
    DOMNode* target = DOMUtil::SelectNodeFromList(elements);

    if (target)
    {
        DOMUtil::OutputDocSubTree(0, target);
    }
    
    else // Display error if target cannot be found
    {
        cout << "No element with the name \"" << root_name
                << "\" could be found." << endl;
    }
}