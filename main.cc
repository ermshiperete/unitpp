// Copyright (C) 2001 Claus Dræby
// Terms of use are in the file COPYING
#include "main.h"
#include <algorithm>
using namespace std;
using namespace unitpp;
namespace { 
	bool verbose;
}
int main(int argc, const char* argv[])
{
	options_utils::optmap opts(argc, argv, "[ testids... ]");
	opts.add("v", new options_utils::opt_flag(verbose));
	opts.alias("verbose", "v");
	if (!opts.parse())
		opts.usage();
	bool res = true;
	if (opts.n() < argc)
		for (int i = opts.n(); i < argc; ++i)
			res = res && run_test(argv[i]);
	else
		res = run_test();
	return 0;
}

namespace unitpp {
bool run_test(const string& id)
{
	test* tp = suite::main().find(id);
	if (!tp) {
		return false;
	}
	return run_test(tp);
}
bool run_test(test* tp)
{
	tester tst(cout, verbose);
	tp->visit(&tst);
	tst.summary();
	res_cnt res(tst.res_tests());
	return res.n_err() == 0 && res.n_fail() == 0;
}

}

