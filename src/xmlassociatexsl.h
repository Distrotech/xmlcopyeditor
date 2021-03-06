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

#ifndef XML_ASSOCIATE_XSL_H
#define XML_ASSOCIATE_XSL_H

#include <cstdio>
#include <vector>
#include <expat.h>
#include <string>
#include <memory>
#include "wrapexpat.h"

struct XslData : public ParserData
{
	std::string buffer, path;
	bool rootElementSeen;
};

class XmlAssociateXsl : public WrapExpat
{
	public:
		XmlAssociateXsl (
		    const std::string& path = "",
		    size_t size = BUFSIZ );
		virtual ~XmlAssociateXsl();
		std::string getBuffer()
		{
			return d->buffer;
		}
	private:
		std::auto_ptr<XslData> d;
		std::string encoding;
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
		static void XMLCALL processinghandler (
		    void *data,
		    const XML_Char *target,
		    const XML_Char *datastring );
};

#endif
