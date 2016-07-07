#ifndef H264MENUNALUINFO_H
#define H264MENUNALUINFO_H

#include <QMainWindow>
#include <Windows.h>
#include <QTableWidgetItem>
#include "ui_h264menunaluinfo.h"
#include "H264Analysis/H264PublicDef.h"
class h264menunaluinfo : public QMainWindow
{
	Q_OBJECT

public:
	typedef struct {
		QTableWidgetItem *nameItem;
		QTableWidgetItem *numItem;
		QTableWidgetItem *mbItem;
	} NaluItem, *PNaluItem;
public:
	h264menunaluinfo(QWidget *parent = 0);
	~h264menunaluinfo();
	void clearData();
public slots:
	void dealUpdateH264MenuNaluInfo(PMetaData pMetaData, unsigned int num);
	void showMenuNaluInfo();
private:
	Ui::h264menunaluinfo ui;
	PNaluItem m_pNaluItem;
};

#endif // H264MENUNALUINFO_H
