// Copyright (C) 2001 Claus Dræby
// Terms of use are in the file COPYING
#include "asserter.h"

void unitpp::assertion_error::out(ostream& os) const
{
	os << msg << " [assertion failed]";
}
