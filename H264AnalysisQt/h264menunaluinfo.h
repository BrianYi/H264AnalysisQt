#ifndef H264MENUNALUINFO_H
#define H264MENUNALUINFO_H

#include <QMainWindow>
#include "ui_h264menunaluinfo.h"

class h264MenuNaluInfo : public QMainWindow
{
	Q_OBJECT

public:
	h264MenuNaluInfo(QMainWindow *parent = 0);
	~h264MenuNaluInfo();

private:
	Ui::h264MenuNaluInfo ui;
};

#endif // H264MENUNALUINFO_H
