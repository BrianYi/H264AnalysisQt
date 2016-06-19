#include "h264findtext.h"
#include "h264analysisqt.h"

h264FindText::h264FindText(H264AnalysisQt *pH264AnalysisQt)
	: QMainWindow(pH264AnalysisQt)
{
	ui.setupUi(this);

	m_pH264AnalysisQt = pH264AnalysisQt;

	connect(ui.findNextPushButton, &QPushButton::clicked, this, &h264FindText::dealFindNext);
	connect(this, &h264FindText::msgFindText, pH264AnalysisQt, &H264AnalysisQt::dealFindText);
	connect(ui.closePushButton, &QPushButton::clicked, this, &h264FindText::close);
}

h264FindText::~h264FindText()
{

}

bool h264FindText::transformToBinary( QString txt, QByteArray &outByteArry )
{
	size_t len = m_findTxt.size();
	if (len % 2)
		return false;
	QByteArray ret;
	ret.append(txt);
	outByteArry = QByteArray::fromHex(ret);
	return true;
}

void h264FindText::dealFindNext()
{
	m_findTxt = ui.findLineEdit->text().toLower();

	m_findTxt = m_findTxt.replace(" ", "");

	QByteArray byteArry;
	if (!transformToBinary(m_findTxt, byteArry))
	{
		QMessageBox::warning(this, tr("警告"), tr("输入格式不正确，请输入二进制，例如: 01 f2 1f(有空格形式) 或者 01f21f(无空格形式)"));
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
