#ifdef GUI
#include "optmap.h"
#include "main.h"
#include "guitester.h"
#include "gui_hook.h"
#include <qapplication.h>

using namespace unitpp;
using namespace std;

class gui_test_runner : public test_runner {
	virtual bool run_tests(int argc, const char** argv, int)
	{
		QApplication a(argc, const_cast<char**>(argv));
		gui g(a);
		g_setup setup(&g);
		suite::main().visit(&setup);
		a.setMainWidget(&g);
		g.show();
		return a.exec();
	}
};
class gui_flag : public options_utils::optmap::cmd {
	bool do_cmd(options_utils::optmap* om)
	{
		static gui_test_runner gtr;
		set_tester(&gtr);
		return true;
	}
};

gui_hook::gui_hook()
{
	options().add("g", new gui_flag());
	options().alias("gui", "g");
}

void g_setup::visit(test& t)
{
	++n_tests;
	nodes.push_back(new node(branch.top(), t));
}
void g_setup::visit(suite& t)
{
	++n_suites;
	node* np = branch.size() ? new node(branch.top(), t) : new node(gp, t);
	branch.push(np);
	nodes.push_back(np);
}
void g_setup::visit(suite& t, int)
{
	branch.pop();
	if (!branch.size()) {
		gp->totSuites(n_suites);
		gp->totTests(n_tests);
	}
}

void g_setup::run()
{
	if (running)
		return;
	running = true;
	selected.clear();
	find_selected(gp->test_tree()->firstChild());
	for (vector<node*>::iterator p = selected.begin(); p!=selected.end(); ++p) {
		(*p)->run();
		gp->processEvents(20);
		if (!running)
			break;
	}
	running = false;
}

void g_setup::find_selected(QListViewItem* lvi)
{
	if (!lvi)
		return;
	if (lvi->isSelected())
		selected.push_back(rev[lvi]);
	else
		find_selected(lvi->firstChild());
	find_selected(lvi->nextSibling());
}

void node::run()
{
	try {
		t();
		emit ok();
	} catch (assertion_error& e) {
		emit fail(e);
	} catch (exception& e) {
		emit error(e);
	} catch (...) {
		emit error(runtime_error("Unknown exception type"));
	}
}
#endif
