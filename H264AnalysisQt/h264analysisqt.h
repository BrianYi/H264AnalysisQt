#ifndef H264ANALYSISQT_H
#define H264ANALYSISQT_H

#pragma execution_character_set("utf-8")
#include <QtWidgets/QMainWindow>
#include "ui_h264analysisqt.h"
#include "H264Analysis/H264Analysis.h"
#include "h264bitstream/h264_stream.h"
#include "menunaluinfo.h"
#include <QString>
#include <QTableWidgetItem>
#include <QFileDialog>
#include <QPushButton>
#include <QList>
using namespace std;


class H264AnalysisQt : public QMainWindow
{
	Q_OBJECT

public:
	typedef struct {
		QTableWidgetItem *numberItem;
		QTableWidgetItem *naluTypeItem;
		QTableWidgetItem *frameTypeItem;
		QTableWidgetItem *naluSizeItem;
	} NaluItem, *PNaluItem;
	typedef struct {
		size_t pos;
		size_t size;
		QString str;
	} Nalu, *PNalu;
public:
	H264AnalysisQt(QWidget *parent = 0);
	~H264AnalysisQt();
	void clearData();
public slots:
	void showNaluList();
	void showNalu(QTableWidgetItem *item);
private:
	Ui::H264AnalysisQtClass ui;
	QString m_filePath;
	H264Analysis m_analysis;
	QList<PNalu> m_NaluList;
	QLabel m_statusLabel;
	MenuNaluInfo *m_menuNaluInfo;
};

#endif // H264ANALYSISQT_H
