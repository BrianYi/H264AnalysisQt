#ifndef H264FINDTEXT_H
#define H264FINDTEXT_H

#include <QMainWindow>
#include <QMessageBox>
#include "ui_h264findtext.h"
class H264AnalysisQt;
class h264findtext : public QMainWindow
{
	Q_OBJECT
public:
	enum FindDirection{ DOWN = 0, UP = 1 }; // 向上，向下查找
public:
	h264findtext(H264AnalysisQt *pH264AnalysisQt);
	~h264findtext();
	bool transformToHex(QString txt, QByteArray &outByteArry );
signals:
	void msgFindText(QByteArray findTxt, bool isContinueFindNext, h264findtext::FindDirection findDirection);
	public slots:
		void dealFindNext();
private:
	Ui::h264findtext ui;
	QString m_findTxt;
	H264AnalysisQt *m_pH264AnalysisQt;
};

#endif // H264FINDTEXT_H

