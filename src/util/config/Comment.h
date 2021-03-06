// Comment.h
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


#ifndef INCLUDED_Comment_H
#define INCLUDED_Comment_H

#ifndef INCLUDED_DataLine_H
#include "DataLine.h"
#endif

#ifndef INCLUDED_STRING
#include <string>
#define INCLUDED_STRING
#endif

namespace util{
namespace config{

class Comment: public DataLine{

	std::string d_comment;

public:

	Comment(const char *comment);
	
	void acceptWriter(AbstractWriter& writer) const;

	const char *getComment() const;
	
};

}} // end namespace util::config

#endif

