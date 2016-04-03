/* 
 * File:    DOMUtil.h
 * Author:  Conor Finegan
 * Contact: conor_finegan@student.uml.edu
 *
 * Created on December 6, 2015, 10:50 PM
 */

#ifndef DOMUTIL_H
#define	DOMUTIL_H

#include "XStr.h"
#include "LocalStr.h"
#include "CmdUtil.h"

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/framework/StdOutFormatTarget.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/util/XMLString.hpp>

#include <cassert>
#include <iostream>
#include <vector>


/**
 * The \c DOMUtil is a repository of static methods that don't belong to any
 *      particular class, but are useful when manipulating document data   
 *      using Xerces-C.
 * @author Conor Finegan<br>conor_finegan@student.uml.edu
 * @version 1.0
 */
class DOMUtil
{
public:
    
    /**
     * Prompts the user to select a specific \c DOMNode out of the given
     *      container. First checks the size of the container, returning null
     *      if it's empty. This method will only prompt the user if there is
     *      ambiguity as to which node should be selected - that is, if the 
     *      container only contains one node, then that node will be returned
     *      without any prompt. When prompted, the user selects the desired
     *      container by inputing its index. Indexes are adjusted by +1 so that
     *      the first node in the container requires an input of "1", etc. The 
     *      user will be trapped until he enters a valid integer within range 
     *      of the container's size.
     * @param param_nodes \c DOMNodeList* of input nodes
     * @return The Node selected by the user, or \c nullptr if the input
     *      container was empty.
     */    
    static XERCES_CPP_NAMESPACE::DOMNode* SelectNodeFromList(
            XERCES_CPP_NAMESPACE::DOMNodeList* param_nodes);
    
    
    /**
     * Outputs a description of the specified node, listing its name, parent
     *      name, and content.  This method is \b not use the same formatting 
     *      as StdoutPrintCommandHelper(). It's meant to be used for menu
     *      selections, so it doesn't take into account things like tree
     *      depth when formatting the output.
     * @param node Node to be output. The method will fail an assert if this
     *      parameter is null.
     */
    static void OutputNodeDescription(XERCES_CPP_NAMESPACE::DOMNode* node);
    
    /**
     * Recursive function to output an element and all of its children. This
     *      method first prints a node's name, then it's content if applicable.
     *      The node's attributes are then printed, followed by a recursive call
     *      to this method for each of node's children.
     * @param depth Curent depth in the tree structure. Mainly used for
     *      formatting purposes.
     * @param root  Pointer to the current node being output. This method will
     *      also be called recursively for each of <code>node</code>'s children.
     */
    static void OutputDocSubTree( const size_t depth,
            XERCES_CPP_NAMESPACE::DOMNode* root);
    
    /**
     * This method will, given a pointer to a \c DOMNode, attempt to remove the
     *      target from its parent. If the node is successfully removed, it will
     *      then be freed from memory. This method will return true if the 
     *      node is successfully removed, false if an error occurs (such as
     *      the node not having a parent, or the \c removeChild() method
     *      failing).
     * @param target Node to be removed
     * @return \c true if the node was successfully removed, \c false if it was not.
     */
    static bool RemoveNodeFromParent(XERCES_CPP_NAMESPACE::DOMNode* target);
        
private:
    
    /**
     * Construction of \c DOMUtil objects is prohibited, as this class contains
     *      only static data.
     */
    DOMUtil() = delete;
    
    /**
     * This method, given a maximum value, will poll the user for input in the
     *      range [1, range_max] until a valid integer within the range is 
     *      read. The value, upon being returned, is decremented by 1 so that
     *      it is a valid index in the range [0, range_max). This method is 
     *      private to prevent misuse, as it mostly exists as a helper function
     *      for SelectNodeFromList().
     * @param range_max The maximum integer value that is considered valid.
     * @return The value input by the user, minus 1.
     */
    static XMLSize_t GetPositiveIndex(const XMLSize_t range_max);
};

#endif	/* DOMUTIL_H */

