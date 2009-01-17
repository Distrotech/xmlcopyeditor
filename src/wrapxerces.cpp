/*
 * Copyright 2005-2007 Gerald Schmidt.
 *
 * This file is part of Xml Copy Editor.
 *
 * Xml Copy Editor is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * Xml Copy Editor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Xml Copy Editor; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "wrapxerces.h"
#define XERCES_TMPLSINC

#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/util/XMLUni.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <sstream>
#include <utility>
#include <stdexcept>

using namespace xercesc;

WrapXerces::WrapXerces( std::string catalogPath, std::string catalogUtilityPath )
{
	try
	{
		XMLPlatformUtils::Initialize();
	}
	catch ( XMLException& e )
	{
		throw std::runtime_error ( "Cannot initialize Xerces" );
	}
	errorPosition = std::make_pair ( 1, 1 );
	catalogResolver = new XercesCatalogResolver( catalogPath, catalogUtilityPath );
}

WrapXerces::~WrapXerces()
{
	delete catalogResolver;
	XMLPlatformUtils::Terminate();	
}

bool WrapXerces::validate ( const std::string& fileName )
{
	SAX2XMLReader *parser = XMLReaderFactory::createXMLReader();

	parser->setFeature ( XMLUni::fgSAX2CoreNameSpaces, true );
	parser->setFeature ( XMLUni::fgSAX2CoreValidation, true );
	parser->setFeature ( XMLUni::fgXercesDynamic, false );
	parser->setFeature ( XMLUni::fgXercesSchema, true );
	parser->setFeature ( XMLUni::fgXercesSchemaFullChecking, true );
	parser->setFeature ( XMLUni::fgXercesValidationErrorAsFatal, true );
	parser->setFeature ( XMLUni::fgXercesLoadExternalDTD, true );


	DefaultHandler handler;
	MySAX2Handler mySAX2Handler;
	parser->setContentHandler ( &handler );
	parser->setErrorHandler ( &mySAX2Handler );

	//DefaultHandler handler;
	//parser->setEntityResolver ( &handler );
	parser->setEntityResolver ( catalogResolver );

	try
	{
		parser->parse ( fileName.c_str() );
	}
	catch ( XMLException& e )
	{
		delete parser;
		char *err = XMLString::transcode ( e.getMessage() );
		lastError = err;
		XMLString::release ( &err );
		return false;
	}
	catch ( SAXParseException& e )
	{
		delete parser;
		char *err = XMLString::transcode ( e.getMessage() );
		std::stringstream ss;
		ss << "Validation stopped at line " << e.getLineNumber() << ", column " << e.getColumnNumber() << ": " << err;
		lastError = ss.str();
		errorPosition = std::make_pair ( e.getLineNumber(), e.getColumnNumber() );
		XMLString::release ( &err );
		return false;
	}
	catch ( ... )
	{
		delete parser;
		lastError = "Unexpected validation error";
		return false;
	}
	delete parser;
	return true;
}

// tbd: cache grammar
bool WrapXerces::validateMemory (
	const char *buffer,
	const char *system,
	unsigned len )
{
	SAX2XMLReader *parser = XMLReaderFactory::createXMLReader();

	parser->setFeature ( XMLUni::fgSAX2CoreNameSpaces, true );
	parser->setFeature ( XMLUni::fgSAX2CoreValidation, true );
	parser->setFeature ( XMLUni::fgXercesDynamic, true );
	parser->setFeature ( XMLUni::fgXercesSchema, true );
	//parser->setFeature ( XMLUni::fgXercesSchemaFullChecking, true );
	parser->setFeature ( XMLUni::fgXercesValidationErrorAsFatal, true );
	parser->setFeature ( XMLUni::fgXercesLoadExternalDTD, true );
	
	DefaultHandler handler;
	MySAX2Handler mySAX2Handler;
	parser->setContentHandler ( &handler );
	parser->setErrorHandler ( &mySAX2Handler );
	//parser->setEntityResolver ( &handler );
	parser->setEntityResolver ( catalogResolver );

	XMLByte* xmlBuffer = (XMLByte*) buffer;
        MemBufInputSource source (
		xmlBuffer,
                len,
		system );
  
	try
	{
		parser->parse ( source );
	}
	catch ( XMLException& e )
	{
		delete parser;
		lastError = "";
		return false;
	}
	catch ( SAXParseException& e )
	{
		delete parser;
		char *err = XMLString::transcode ( e.getMessage() );
		std::stringstream ss;
		ss << "Ln " << e.getLineNumber() << " Col " << e.getColumnNumber() << ": " << err;
		lastError = ss.str();
		errorPosition = std::make_pair ( e.getLineNumber(), e.getColumnNumber() );
		XMLString::release ( &err );
		return false;
	}
	catch ( ... )
	{
		delete parser;
		lastError = "";
		return false;
	}
	delete parser;
	return true;
}

std::string WrapXerces::getLastError()
{
	char *rawError, *it;
	rawError = (char *)lastError.c_str();
	it = strstr ( rawError, "Message:" );
	if ( it )
	{
		lastError = it + 8;
	}

	return lastError;
}

std::pair<int, int> WrapXerces::getErrorPosition()
{
	return errorPosition;
}
