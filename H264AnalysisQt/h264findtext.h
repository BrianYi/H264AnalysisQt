#ifndef H264FINDTEXT_H
#define H264FINDTEXT_H

#include <QMainWindow>
#include <QMessageBox>
#include "ui_h264findtext.h"
class H264AnalysisQt;
class h264FindText : public QMainWindow
{
	Q_OBJECT
public:
	enum FindDirection{ DOWN = 0, UP = 1 }; // ���ϣ����²���
public:
	h264FindText(H264AnalysisQt *pH264AnalysisQt);
	~h264FindText();
	bool transformToBinary(QString txt, QByteArray &outByteArry );
signals:
	void msgFindText(QByteArray findTxt, bool isContinueFindNext, h264FindText::FindDirection findDirection);
public slots:
	void dealFindNext();
private:
	Ui::h264FindText ui;
	QString m_findTxt;
	H264AnalysisQt *m_pH264AnalysisQt;
};

#endif // H264FINDTEXT_H