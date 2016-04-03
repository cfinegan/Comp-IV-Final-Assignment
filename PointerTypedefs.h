/* 
 * File:    PointerTypedefs.h
 * Author:  Conor Finegan
 * Contact: conor_finegan@student.uml.edu
 *
 * Created on November 25, 2015, 6:55 PM
 * 
 * This header is a small library of typedefs designed to ease the burden of
 * using Xerces-C DOM objects by tying their release methods to a smart
 * pointer's destructor.
 */


#ifndef POINTERTYPEDEFS_H
#define	POINTERTYPEDEFS_H

#include <memory>

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMNode.hpp>

/**
 * Function operator that takes a DOM object and calls \c release on it. 
 *      This function template is primarily used in the declaration of
 *      Xerces-C-compatible smart pointers. This method acts as a custom
 *      deleter, allowing DOM objects to have their destruction managed
 *      automatically. \b CAUTION: be careful only to extend this template
 *      to classes that actually have a \c release method, otherwise you'll
 *      get bizarre compiler errors.
 * @param ptr Pointer to the DOM object that is going to be released.
 * @author Conor Finegan<br>conor_finegan@student.uml.edu
 * @version 1.0
 */
template <typename DOMObjectType>
struct DOMDeleter : public std::unary_function<DOMObjectType*, void>
{
    /**
     * Method invoked when the DOM smart pointer leaves scope and is destroyed
     * @param ptr Pointer to the object owned by the smart pointer.
     */
    inline void operator()(DOMObjectType* ptr) noexcept
    {
        ptr->release();
    }
};

/**
 * Declaration for a self-releasing \c DOMDocument unique pointer
 */
typedef std::unique_ptr<
        XERCES_CPP_NAMESPACE::DOMDocument, 
        DOMDeleter<XERCES_CPP_NAMESPACE::DOMDocument>
        > DocumentPtr;

/**
 * Declaration for a self-releasing \c DOMTreeWalker unique pointer
 */
typedef std::unique_ptr<
        XERCES_CPP_NAMESPACE::DOMTreeWalker, 
        DOMDeleter<XERCES_CPP_NAMESPACE::DOMTreeWalker>
        > TreeWalkerPtr;

/**
 * Declaration for a self-releasing \c DOMLSSerializer unique pointer
 */
typedef std::unique_ptr<
        XERCES_CPP_NAMESPACE::DOMLSSerializer,
        DOMDeleter<XERCES_CPP_NAMESPACE::DOMLSSerializer>
        > LSSerializerPtr;

/**
 * Declaration for a self-releasing \c DOMLSOutput unique pointer
 */
typedef std::unique_ptr<
        XERCES_CPP_NAMESPACE::DOMLSOutput,
        DOMDeleter<XERCES_CPP_NAMESPACE::DOMLSOutput>
        > LSOutputPtr;

#endif	/* POINTERTYPEDEFS_H */

