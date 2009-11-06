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

#ifndef XML_ASSOCIATE_XSD_H
#define XML_ASSOCIATE_XSD_H

#include <vector>
#include <expat.h>
#include <string>
#include <map>
#include <memory>
#include "wrapexpat.h"

struct AssociateXsdData : public ParserData
{
	std::string buffer, path, namespaceAttribute;
	bool rootElementSeen;
	std::map<std::string, std::string> namespaceMap;
};

class XmlAssociateXsd : public WrapExpat
{
	public:
		XmlAssociateXsd (
		    const std::string& path = "",
		    size_t size = BUFSIZ );
		virtual ~XmlAssociateXsd();
		std::string getBuffer()
		{
			return d->buffer;
		}
	private:
		std::auto_ptr<AssociateXsdData> d;
		std::string encoding;
		std::map<std::string, std::string> namespaceMap;
		static void XMLCALL start (
		    void *data,
		    const XML_Char *el,
		    const XML_Char **attr );
		static void XMLCALL end (
		    void *data,
		    const XML_Char *el );
		static void XMLCALL defaulthandler (
		    void *data,
		    const XML_Char *s,
		    int len );
};

#endif
