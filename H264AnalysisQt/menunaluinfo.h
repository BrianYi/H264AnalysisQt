#ifndef MENUNALUINFO_H
#define MENUNALUINFO_H

#include <QMainWindow>
#include "ui_menunaluinfo.h"

class MenuNaluInfo : public QMainWindow
{
	Q_OBJECT

public:
	MenuNaluInfo(QMainWindow *parent = 0);
	~MenuNaluInfo();

private:
	Ui::MenuNaluInfo ui;
};

#endif // MENUNALUINFO_H
