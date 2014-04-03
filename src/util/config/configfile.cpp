// configfile.cpp
//
// Copyright (C) 2000, 2001, 2002, 2003, 2004, 2005 Matthew Flood
// See file AUTHORS for contact information
//
// This file is part of UtilConfig.
//
// UtilConfig is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.
// 
// UtilConfig is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with UtilConfig; (see COPYING) if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
// 02111-1307, USA.
//------------------------------------------------------------------------

#include "configfile.h"

#ifndef INCLUDED_UTIL_CONFIGIMPL_H
#include "ConfigImpl.h"
#endif

#ifndef INCLUDED_IOSTREAM
#include <iostream>
#define INCLUDED_IOSTREAM
#endif

#ifndef INCLUDED_STRING
#include <string>
#define INCLUDED_STRING
#endif

using namespace util::config;

namespace util{

//////////////////////////////////////////
// STATIC Methods
//////////////////////////////////////////

const char *ConfigFile::version()
{
	static std::string ver="3.";
	ver += ConfigImpl::version();
	return ver.c_str();
}
	
	void ConfigFile::setDefaultConfigFile(const char *filepath)
	{
		ConfigImpl::setDefaultConfigFile(filepath);
	}

	const char *ConfigFile::getDefaultConfigFile()
	{
		return ConfigImpl::getDefaultConfigFile();
	}

	void ConfigFile::setDefaultCommentCharacter(char c)
	{
		ConfigImpl::setDefaultCommentCharacter(c);
	}

	char ConfigFile::getDefaultCommentCharacter()
	{
		return ConfigImpl::getDefaultCommentCharacter();
	}

	void ConfigFile::setDefaultDelimiter(char c)
	{
		ConfigImpl::setDefaultDelimiter(c);
	}

	char ConfigFile::getDefaultDelimiter()
	{
		return ConfigImpl::getDefaultDelimiter();
	}

	void ConfigFile::setDefaultPreserveDeleted(bool shouldPreserve)
	{
		ConfigImpl::setDefaultPreserveDeleted(shouldPreserve);
	}

	bool ConfigFile::getDefaultPreserveDeleted()
	{
		return ConfigImpl::getDefaultPreserveDeleted();
	}

//////////////////////////////////////////
// INSTANCE Methods
//////////////////////////////////////////

ConfigFile::ConfigFile()
{
	d_implementation = new ConfigImpl();
}

ConfigFile::~ConfigFile()
{
	delete d_implementation;
	d_implementation = 0;
}

void ConfigFile::setConfigFile(const char *filepath)
{
	d_implementation->setConfigFile(filepath);
}

const char * ConfigFile::getConfigFile()
{
	return d_implementation->getConfigFile();
}

void ConfigFile::setCommentCharacter(char commentchar)
{
	d_implementation->setCommentCharacter(commentchar);
}

void ConfigFile::preserveDeletedData(bool shouldPreserve)
{
	d_implementation->preserveDeletedData(shouldPreserve);
}

void ConfigFile::setDelimiter(char delimiter)
{
	d_implementation->setDelimiter(delimiter);
}

bool ConfigFile::load()
{
	return d_implementation->load();
}

bool ConfigFile::load(const char *filename)
{
	return d_implementation->load(filename);
}


bool ConfigFile::save()
{
	return d_implementation->save();
}

bool ConfigFile::save(const char *filepath)
{
	return d_implementation->save(filepath);
}


void ConfigFile::clear()
{
	d_implementation->clear();
}


const char *ConfigFile::getError()
{
	return d_implementation->getError();
}


//////////////////////////////////////////
// Section Methods
//////////////////////////////////////////
int ConfigFile::getNumSections() const
{
	return d_implementation->getNumSections();
}

const char *ConfigFile::getSectionNameAt(int index) const
{
	return d_implementation->getSectionNameAt(index);
}

bool ConfigFile::setSection(const char *sectionname, bool shouldCreate)
{
	return d_implementation->setSection(sectionname, shouldCreate);
}

bool ConfigFile::setSection(const char *sectionname)
{
	return d_implementation->setSection(sectionname, true);
}

bool ConfigFile::deleteSection(const char *sectionname)
{
	return d_implementation->deleteSection(sectionname);
}

//////////////////////////////////////////
// Data Methods
//////////////////////////////////////////
int ConfigFile::getNumDataMembers() const
{
	return d_implementation->getNumDataMembers();
}

const char *ConfigFile::getDataNameAt(int index) const
{
	return d_implementation->getDataNameAt(index);
}

bool ConfigFile::exists(const char *name) const
{
	return d_implementation->exists(name);
}

void ConfigFile::setBoolValue(const char *name, bool value)
{
		d_implementation->setStringValue(name, ConfigImpl::boolToString(value));
	// d_implementation->setBoolValue(name, value);
}

bool ConfigFile::getBoolValue(const char *name) const
{
	return ConfigImpl::stringToBool(d_implementation->getStringValue(name));
	//return d_implementation->getBoolValue(name);
}

void ConfigFile::setIntValue(const char *name, int value)
{
	d_implementation->setStringValue(name, ConfigImpl::intToString(value));
	// d_implementation->setIntValue(name, value);
}

int ConfigFile::getIntValue(const char *name) const
{
	return ConfigImpl::stringToInt(d_implementation->getStringValue(name));
	//return d_implementation->getIntValue(name);
}

void ConfigFile::setDoubleValue(const char *name, double value)
{
	d_implementation->setStringValue(name, ConfigImpl::doubleToString(value));
	// d_implementation->setDoubleValue(name, value);
}

double ConfigFile::getDoubleValue(const char *name) const
{
	return ConfigImpl::stringToDouble(d_implementation->getStringValue(name));
	//return d_implementation->getDoubleValue(name);
}

void ConfigFile::setStringValue(const char *name, const char *value)
{
	d_implementation->setStringValue(name, value);
}

const char * ConfigFile::getStringValue(const char *name) const
{
	return d_implementation->getStringValue(name);
}

void ConfigFile::setValue(const char *name, const char *value)
{
	d_implementation->setStringValue(name, value);
}

const char * ConfigFile::getValue(const char *name) const
{
	return d_implementation->getStringValue(name);
}

bool ConfigFile::deleteData(const char *name)
{
	return d_implementation->deleteData(name);
}


} // end namespace util
