#include "h264analysisqt.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	H264AnalysisQt w;
	w.show();
	return a.exec();
}
