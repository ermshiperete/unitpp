#include "asserter.h"

void unitpp::assertion_error::out(ostream& os) const
{
	os << msg << " [assertion failed]";
}
