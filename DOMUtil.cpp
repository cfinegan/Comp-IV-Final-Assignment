/* 
 * File:    DOMUtil.cpp
 * Author:  Conor Finegan
 * Contact: conor_finegan@student.uml.edu
 * 
 * Created on December 6, 2015, 10:50 PM
 */

#include "DOMUtil.h"

using namespace std;
XERCES_CPP_NAMESPACE_USE

DOMNode* DOMUtil::SelectNodeFromList(DOMNodeList* param_nodes)
{
    const XMLSize_t param_nodes_size = param_nodes->getLength();
    
    // Return null if input container is empty
    if (!param_nodes_size)
    {
        return nullptr;
    }
    
    // If there is only 1 result, it will be used by default
    else if (param_nodes_size == 1)
    {
        return param_nodes->item(0);
    }
    
    // Else prompt user to select node
    cout << endl;
    cout << "You have entered an Element name that matches more than one element." << endl;
    
    // Output description of each node
    for (XMLSize_t i = 0; i < param_nodes_size; ++ i)
    {
        cout << i+1 << ") ";
        OutputNodeDescription(param_nodes->item(i));        
    }

    // Get index of target node from user
    XMLSize_t user_selection = GetPositiveIndex(param_nodes_size);

    // Sanity check: final index must be within range
    assert(user_selection < param_nodes_size);
    
    return param_nodes->item(user_selection);
}

void DOMUtil::OutputNodeDescription(DOMNode* node)
{
    // Cannot continue if node is not valid
    assert(node != nullptr);
    
    cout << "Element \"" << LSTR(node->getNodeName()) << "\"";
    
    // Get Parent
    DOMNode* parent = node->getParentNode(); 
    if (parent)
    {
        cout << " with parent \"" << LSTR(parent->getNodeName()) << "\"";
    }
    
    // Get node text
    DOMNode* tnode = node->getFirstChild();
    if (tnode && tnode->getNodeType() == DOMNode::TEXT_NODE)
    {
        LocalStr node_text(tnode->getNodeValue());
        if (node_text) 
        { 
            cout << " and content \"" << node_text << "\""; 
        } 
    }
    cout << "." << endl;
}

void DOMUtil::OutputDocSubTree( const size_t depth, DOMNode* root)
{
    // Amount of spaces in a tab. Could make configurable later.
    constexpr size_t tab_size = 4;
           
    // Print node name
    for(size_t i = 0; i < depth * tab_size; ++ i) { cout << " "; }
    cout << "[Node] " << LSTR(root->getNodeName());
    
    // Get node text
    DOMNode* tnode = root->getFirstChild();
    if (tnode && tnode->getNodeType() == DOMNode::TEXT_NODE)
    {
        LocalStr node_text(tnode->getNodeValue());
        if (node_text) 
        { 
            cout << ": " << node_text; 
        } 
    }
    
    cout << endl;
    
    // Print node value if it exists
    LocalStr node_val(root->getNodeValue());
    if (node_val)
    {
        for (size_t i = 0; i < depth * tab_size + tab_size; ++ i) { cout << " " ; }
        cout << "[Value] " << node_val;
    }
    
    // Print attributes
    DOMNamedNodeMap* attr = root->getAttributes();
    if (attr != nullptr)
    {
        // Enumerate attributes
        XMLSize_t attr_len = attr->getLength();
        for (XMLSize_t i = 0; i < attr_len; ++ i)
        {
            // Load attribute
            DOMNode* attr_node = attr->item(i);          
            
            // Print white-space buffer of parent + 1
            for (size_t i = 0; i < depth * tab_size + tab_size; ++ i) { cout << " "; }
            
            // Output attribute information
            cout << "[Attribute] " << LSTR(attr_node->getNodeName()) << ": " 
                    << LSTR(attr_node->getNodeValue()) << endl;
        }
    }
        
    // Recursively call helper for each child
    DOMNodeList* children = root->getChildNodes();
    if (children != nullptr)
    {
        // Enumerate children
        XMLSize_t children_len = children->getLength();
        for (XMLSize_t i = 0; i < children_len; ++i)
        {
            // Load child
            DOMNode* child = children->item(i);
            
            // Only recursively call for non-text nodes
            if (child->getNodeType() != DOMNode::TEXT_NODE)
            {
                OutputDocSubTree(depth + 1, child);
            }
        }
    }
}

bool DOMUtil::RemoveNodeFromParent(DOMNode* target)
{
    // Cache target name for later output
    LocalStr target_name(target->getNodeName());

    // Attempt to get parent from target
    DOMNode* parent = target->getParentNode();

    // pointer to node after it has been removed from the document.
    // removed being null indicates that the node was not removed.
    DOMNode* removed = nullptr;

    // Only attempt to remove if target has a valid parent
    if (parent) { removed = parent->removeChild(target); }

    if (removed) // If node is no longer in document
    {
        // Release the node from memory
        removed->release();

        // Report success
        return true;
    }

    // If the node could not be removed, report failure
    return false;
}

XMLSize_t DOMUtil::GetPositiveIndex(const XMLSize_t range_max)
{
    // Sanity check: a range_max of zero would trap the user in an
    //      inescapable loop.
    assert(range_max != 0);
    
    // Get index of target node from user
    string user_input;
    cout << "Select by index: ";
    
    for (;;) // For ever...
    {
        getline(cin, user_input);
        
        // Throw error if stream fails waiting for valid input
        if (cin.bad()) { throw CmdUtil::CmdFailureException(); }
        
        // Get integer value from string
        char* endptr = nullptr;
        errno = 0;
        unsigned long index = strtoul(user_input.c_str(), &endptr, 0);
        
        // To be valid, a conversion must:
        //      Not return zero (either because of an error or because it's 
        //          what the user entered).
        //      Not be outside the range_max.
        //      Not abort because the string was too long to parse.
        //      Not contain any characters after the index value.
        if (index != 0 && index <= range_max && 
                errno != ERANGE && endptr != nullptr && *endptr == 0)
        {
            // Abort loop if index is correct
            // Return the adjusted value so that it is a valid index in the
            //      range [0, range_max)
            return static_cast<XMLSize_t>(index - 1);
        }
        
        // Output error if conversion failed
        cout << "Invalid input, please try again: ";
    }
}