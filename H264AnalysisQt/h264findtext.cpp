#include "h264findtext.h"
#include "h264analysisqt.h"

h264findtext::h264findtext(H264AnalysisQt *pH264AnalysisQt)
	: QMainWindow(pH264AnalysisQt)
{
	ui.setupUi(this);

	m_pH264AnalysisQt = pH264AnalysisQt;

	connect(ui.findNextPushButton, &QPushButton::clicked, this, &h264findtext::dealFindNext);
	connect(this, &h264findtext::msgFindText, pH264AnalysisQt, &H264AnalysisQt::dealFindText);
	connect(ui.closePushButton, &QPushButton::clicked, this, &h264findtext::close);
}

h264findtext::~h264findtext()
{

}

bool h264findtext::transformToHex( QString txt, QByteArray &outByteArry )
{
	size_t len = m_findTxt.size();
	if (len % 2)
		return false;
	QByteArray ret;
	ret.append(txt);
	outByteArry = QByteArray::fromHex(ret);
	return true;
}

void h264findtext::dealFindNext()
{
	m_findTxt = ui.findLineEdit->text().toLower();

	m_findTxt = m_findTxt.replace(" ", "");

	QByteArray byteArry;
	if (!transformToHex(m_findTxt, byteArry))
	{
		QMessageBox::warning(this, tr("����"), tr("�����ʽ����ȷ������������ƣ�����: 01 f2 1f(�пո���ʽ) ���� 01f21f(�޿ո���ʽ)"));
		return ;
	}

	if (!m_findTxt.isEmpty())
	{
		bool isContinueFindNext = ui.continueFindNextCheckBox->isChecked();
		if (ui.upRadioButton->isChecked())
			emit msgFindText(byteArry, isContinueFindNext, UP);
		else
			emit msgFindText(byteArry, isContinueFindNext, DOWN);
	}
}
