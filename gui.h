#ifndef __UNITPP_GUI_H
#define __UNITPP_GUI_H
#ifdef GUI
#include "tester.h"
#include <exception>
#include <qwidget.h>
#include <qcolor.h>
#include <qframe.h>
#include <qlabel.h>
#include <qlistview.h>
#include <qprogressbar.h>
#include <qhbox.h>
#include <qvbox.h>
#include <qpixmap.h>
#include <qpushbutton.h>
#include <qapplication.h>

namespace unitpp {
class cnt_item : public QHBox
{
	Q_OBJECT
private:
	QLabel* val;
	QLabel* label;
public:
	cnt_item(QWidget* par, const QString& txt, const QColor& col = black,
			const char* nam = 0);
public slots:
	void value(int v);
};

class cnt_line : public QHBox
{
	Q_OBJECT
private:
	QLabel* label;
	cnt_item* cnts[4];
public slots:
	void set(const res_cnt& r) {}
	void max(int v);
public:
	cnt_line(const QString& txt, QWidget* par = 0, const char* name = 0);
};

class res_stack : public QVBox
{
	Q_OBJECT
private:
	cnt_line* cnts;
	QProgressBar* bar;
public slots:
	void max(int max);
public:
	res_stack(const QString& txt, QWidget* par=0, const char* name=0);
};
class gui : public QVBox
{
	Q_OBJECT
public:
	gui(QApplication& app, QWidget* par = 0, const char* name = 0);
	virtual ~gui();
	QListView* test_tree() { return tree; }
	void processEvents(int t);
public slots:
	void totSuites(int v);
	void totTests(int v);
private:
	QApplication& app;
	QListView* tree;
	res_stack* suites;
	res_stack* tests;
	QPushButton* run;
	QPushButton* stop;
	QPushButton* quit;
};
class node : public QObject
{
	Q_OBJECT
public:
	enum state { none, is_ok, is_fail, is_error };
	node(node* par, test&);
	node(gui* par, test&);
	QListViewItem* lvi() { return item; }
signals:
	void ok();
	void fail(assertion_error& err);
	void error(const std::exception& err);
public slots:
	void run();
private:
	QListViewItem* item;
	test& t;
	state st;
	void setImg();
};
}
#endif
#endif
