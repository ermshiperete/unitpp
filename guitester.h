#ifndef __UNITPP__GUITESTER_H
#define __UNITPP__GUITESTER_H
#ifdef GUI
#include "tester.h"
#include "gui.h"
#include <stack>
#include <vector>

namespace unitpp {
class g_setup : public visitor {
	gui* gp;
	bool running;
	int n_suites;
	int n_tests;
	std::stack<node*> branch;
	std::vector<node*> nodes, selected;
	std::map<QListViewItem*, node*> rev;
	void find_selected(QListViewItem*);
public:
	g_setup(gui* gp) : gp(gp), running(false), n_suites(0), n_tests(0) {}
	virtual void visit(test&);
	virtual void visit(suite&);
	virtual void visit(suite&, int);
public slots:
	void run();
};
}
#endif
#endif
