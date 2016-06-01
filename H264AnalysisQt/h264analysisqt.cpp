#include "h264analysisqt.h"

char tempstr[1000] = {0};
char outputstr[100000] = {0};

H264AnalysisQt::H264AnalysisQt(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	m_filePath = "../movie/500MTest.h264";
	ui.fileRouteLineEdit->setText(m_filePath);
	ui.NaluListTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); /// 设置不可编辑
	ui.NaluListTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); /// 设置单击选中整行 
	ui.NaluListTableWidget->setSelectionMode(QAbstractItemView::SingleSelection); /// 设置只能选中单行
	ui.NaluListTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.NaluListTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.curStatusLabel->setText(tr("当前选择的NALU信息: 无"));
	ui.NaluShowBinaryLabel->setText("            00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n");
	ui.NaluShowInfoLabel->setText("NALU信息");
	m_menuNaluInfo = new MenuNaluInfo(this);
	connect(ui.fileRoutePushButton, &QPushButton::clicked, this, [=](){
		m_filePath = QFileDialog::getOpenFileName(this, "选择H264文件路径");
		if (!m_filePath.isEmpty())
			ui.fileRouteLineEdit->setText(m_filePath);
	});
	connect(ui.startPushButton, &QPushButton::clicked, this, &H264AnalysisQt::showNaluList);
	connect(ui.closePushButton, &QPushButton::clicked, this, &H264AnalysisQt::close);
	connect(ui.NaluListTableWidget, &QTableWidget::itemDoubleClicked, this, &H264AnalysisQt::showNalu);
	connect(ui.actionNALU, &QAction::triggered, this->m_menuNaluInfo, &MenuNaluInfo::show);
}

H264AnalysisQt::~H264AnalysisQt()
{
	clearData();
	if (m_menuNaluInfo)
	{
		m_menuNaluInfo->destroyed();
		delete m_menuNaluInfo;
		m_menuNaluInfo = NULL;
	}
	m_analysis.closeFile();
}


void H264AnalysisQt::clearData()
{
	int rowCount = ui.NaluListTableWidget->rowCount();
	for (int i = 0; i < rowCount; i++)
	{
		QTableWidgetItem *pNumber = ui.NaluListTableWidget->item(i, 0);
		QTableWidgetItem *pNaluType = ui.NaluListTableWidget->item(i, 1);
		QTableWidgetItem *pFrameType = ui.NaluListTableWidget->item(i, 2);
		QTableWidgetItem *pNaluSize = ui.NaluListTableWidget->item(i, 3);
		if (pNumber)
			delete pNumber;
		if (pNaluType)
			delete pNaluType;
		if (pFrameType)
			delete pFrameType;
		if (pNaluSize)
			delete pNaluSize;
		ui.NaluListTableWidget->removeRow(i);
	}
	ui.NaluListTableWidget->clearContents();

	for (QList<PNalu>::iterator ite = m_NaluList.begin(); ite != m_NaluList.end(); ite++)
	{
		if (*ite)
		{
			delete *ite;
			*ite = NULL;
		}
	}
	m_NaluList.clear();
}

void H264AnalysisQt::showNaluList()
{
	clearData();
	ui.curStatusLabel->setText(tr("当前选择的NALU信息: 无"));
	// analysize
	int pCount = 0, bCount = 0, iCount = 0, siCount = 0, spCount = 0, spsCount = 0, ppsCount = 0, seiCount = 0, audCount = 0;
	int pSize = 0, bSize = 0, iSize = 0, siSize = 0, spSize = 0, spsSize = 0, ppsSize = 0, seiSize = 0, audSize = 0;
	int NaluCount = 0;
	int NaluSize = 0;
	int NaluTotalSize = 0;
	int startCodeLen = 0;
	char *naluData = NULL;
	m_analysis.getOpenFile(m_filePath.toStdString());
	while (NaluSize = m_analysis.nextNalu((char**)(&naluData)))
	{
		ui.NaluListTableWidget->insertRow(NaluCount);
		unsigned char nextByte = 0;
		PNalu pNalu = new Nalu;
		startCodeLen = m_analysis.scLen(naluData);
		nextByte = naluData[startCodeLen];
		pNalu->pos = m_analysis.m_pStreamBuf->tellgBase + m_analysis.m_pStreamBuf->pos - NaluSize;
		pNalu->size = NaluSize;
		m_NaluList.append(pNalu);
		UINT32 forbidden_zero_bit = B8_VAL_BASE_R(nextByte, 0, 1);
		UINT32 nal_ref_idc = B8_VAL_BASE_R(nextByte, 1, 2);
		UINT32 nal_unit_type = B8_VAL_BASE_R(nextByte, 3, 5);
		UINT32 first_mb_in_slice = 0;
		UINT32 slice_type = 0;
		UINT32 pic_parameter_set_id = 0;
		unsigned int egcDataPos = startCodeLen + 1;
		unsigned int egcDataLen = NaluSize - egcDataPos;
		unsigned int egcSize = 0;
		int len = 0;
		PNaluItem pNaluItem = new NaluItem;
		pNaluItem->numberItem = new QTableWidgetItem(QString::number(NaluCount));
		pNaluItem->naluSizeItem = new QTableWidgetItem(QString::number(NaluSize));
		switch (nal_unit_type)
		{
		case NAL_SLICE:
		case NAL_IDR_SLICE:
		case NAL_AUXILIARY_SLICE:
			if (nal_unit_type == NAL_SLICE)
				pNaluItem->naluTypeItem = new QTableWidgetItem(tr("NAL_SLICE"));
			else if (nal_unit_type == NAL_IDR_SLICE)
				pNaluItem->naluTypeItem = new QTableWidgetItem(tr("NAL_IDR_SLICE"));
			else if (nal_unit_type == NAL_AUXILIARY_SLICE)
				pNaluItem->naluTypeItem = new QTableWidgetItem(tr("NAL_AUXILIARY_SLICE"));
			
			if (m_analysis.ueDecode(&naluData[egcDataPos], egcDataLen, &first_mb_in_slice, &egcSize) == H264Analysis::Failed)
				break;
			if (m_analysis.ueDecode(&naluData[egcDataPos + egcSize], egcDataLen - egcSize, &slice_type, &egcSize) == H264Analysis::Failed)
				break;
			m_analysis.m_lastByte = 0;
			m_analysis.m_binPos = 0;
			pNaluItem->numberItem->setBackgroundColor(QColor("red"));
			pNaluItem->naluTypeItem->setBackgroundColor(QColor("red"));
			pNaluItem->naluSizeItem->setBackgroundColor(QColor("red"));

			switch (slice_type)
			{
			case SLICE_TYPE_P1:
			case SLICE_TYPE_P2:
				pNaluItem->frameTypeItem = new QTableWidgetItem(tr("P"));
				pNaluItem->frameTypeItem->setBackgroundColor(QColor("red"));

				break;
			case SLICE_TYPE_B1:
			case SLICE_TYPE_B2:
				pNaluItem->frameTypeItem = new QTableWidgetItem(tr("B"));
				pNaluItem->frameTypeItem->setBackgroundColor(QColor("red"));
				break;
			case SLICE_TYPE_I1:
			case SLICE_TYPE_I2:
				pNaluItem->frameTypeItem = new QTableWidgetItem(tr("I"));
				pNaluItem->frameTypeItem->setBackgroundColor(QColor("red"));
				break;
			case SLICE_TYPE_SP1:
			case SLICE_TYPE_SP2:
				pNaluItem->frameTypeItem = new QTableWidgetItem(tr("SP"));
				pNaluItem->frameTypeItem->setBackgroundColor(QColor("red"));
				break;
			case SLICE_TYPE_SI1:
			case SLICE_TYPE_SI2:
				pNaluItem->frameTypeItem = new QTableWidgetItem(tr("SI"));
				pNaluItem->frameTypeItem->setBackgroundColor(QColor("red"));
				break;
			default:
				pNaluItem->frameTypeItem = new QTableWidgetItem(tr("DON'T KNOW"));
				pNaluItem->frameTypeItem->setBackgroundColor(QColor("red"));
				break;
			}
			break;
		case NAL_DPA:
			pNaluItem->naluTypeItem = new QTableWidgetItem(tr("NAL_DPA"));
			pNaluItem->frameTypeItem = new QTableWidgetItem(tr("NULL"));
			break;
		case NAL_DPB:
			pNaluItem->naluTypeItem = new QTableWidgetItem(tr("NAL_DPB"));
			pNaluItem->frameTypeItem = new QTableWidgetItem(tr("NULL"));
			break;
		case NAL_DPC:
			pNaluItem->naluTypeItem = new QTableWidgetItem(tr("NAL_DPC"));
			pNaluItem->frameTypeItem = new QTableWidgetItem(tr("NULL"));
			break;
		case NAL_SEI:
			pNaluItem->naluTypeItem = new QTableWidgetItem(tr("NAL_SEI"));
			pNaluItem->frameTypeItem = new QTableWidgetItem(tr("NULL"));

			pNaluItem->numberItem->setBackgroundColor(QColor("blue"));
			pNaluItem->naluTypeItem->setBackgroundColor(QColor("blue"));
			pNaluItem->frameTypeItem->setBackgroundColor(QColor("blue"));
			pNaluItem->naluSizeItem->setBackgroundColor(QColor("blue"));
			break;
		case NAL_SPS:
			pNaluItem->naluTypeItem = new QTableWidgetItem(tr("NAL_SPS"));
			pNaluItem->frameTypeItem = new QTableWidgetItem(tr("NULL"));

			pNaluItem->numberItem->setBackgroundColor(QColor("yellow"));
			pNaluItem->naluTypeItem->setBackgroundColor(QColor("yellow"));
			pNaluItem->frameTypeItem->setBackgroundColor(QColor("yellow"));
			pNaluItem->naluSizeItem->setBackgroundColor(QColor("yellow"));
			break;
		case NAL_PPS:
			pNaluItem->naluTypeItem = new QTableWidgetItem(tr("NAL_PPS"));
			pNaluItem->frameTypeItem = new QTableWidgetItem(tr("NULL"));

			pNaluItem->numberItem->setBackgroundColor(QColor("green"));
			pNaluItem->naluTypeItem->setBackgroundColor(QColor("green"));
			pNaluItem->frameTypeItem->setBackgroundColor(QColor("green"));
			pNaluItem->naluSizeItem->setBackgroundColor(QColor("green"));
			break;
		case NAL_AUD:
			pNaluItem->naluTypeItem = new QTableWidgetItem(tr("NAL_AUD"));
			pNaluItem->frameTypeItem = new QTableWidgetItem(tr("NULL"));
			break;
		case NAL_END_SEQUENCE:
			pNaluItem->naluTypeItem = new QTableWidgetItem(tr("NAL_END_SEQUENCE"));
			pNaluItem->frameTypeItem = new QTableWidgetItem(tr("NULL"));
			break;
		case NAL_END_STREAM:
			pNaluItem->naluTypeItem = new QTableWidgetItem(tr("NAL_END_STREAM"));
			pNaluItem->frameTypeItem = new QTableWidgetItem(tr("NULL"));
			break;
		case NAL_FILLER_DATA:
			pNaluItem->naluTypeItem = new QTableWidgetItem(tr("NAL_FILLER_DATA"));
			pNaluItem->frameTypeItem = new QTableWidgetItem(tr("NULL"));
			break;
		case NAL_SPS_EXT:
			pNaluItem->naluTypeItem = new QTableWidgetItem(tr("NAL_SPS_EXT"));
			pNaluItem->frameTypeItem = new QTableWidgetItem(tr("NULL"));
			break;
		default:
			pNaluItem->naluTypeItem = new QTableWidgetItem(tr("DON'T KNOW"));
			pNaluItem->frameTypeItem = new QTableWidgetItem(tr("NULL"));
			break;
		}
		delete [] naluData;
		pNaluItem->numberItem->setTextAlignment(Qt::AlignCenter);
		pNaluItem->naluTypeItem->setTextAlignment(Qt::AlignCenter);
		pNaluItem->frameTypeItem->setTextAlignment(Qt::AlignCenter);
		pNaluItem->naluSizeItem->setTextAlignment(Qt::AlignCenter);
		ui.NaluListTableWidget->setItem(NaluCount, 0, pNaluItem->numberItem);
		ui.NaluListTableWidget->setItem(NaluCount, 1, pNaluItem->naluTypeItem);
		ui.NaluListTableWidget->setItem(NaluCount, 2, pNaluItem->frameTypeItem);
		ui.NaluListTableWidget->setItem(NaluCount, 3, pNaluItem->naluSizeItem);
		NaluCount++;
	}
	ui.NaluListTableWidget->setRowCount(NaluCount);
}


void H264AnalysisQt::showNalu(QTableWidgetItem *item)
{
	ui.NaluShowBinaryTextEdit->setText("");
	int itemRow = item->row();
	PNalu nalu = m_NaluList.at(itemRow);
	memset(outputstr, 0, 100000);
	QTableWidgetItem *pNumber = ui.NaluListTableWidget->item(itemRow, 0);
	QTableWidgetItem *pNaluType = ui.NaluListTableWidget->item(itemRow, 1);
	QTableWidgetItem *pFrameType = ui.NaluListTableWidget->item(itemRow, 2);
	QTableWidgetItem *pNaluSize = ui.NaluListTableWidget->item(itemRow, 3);
	QString statusStr = 
		tr("当前选择的NALU信息: ") + 
		tr("编号(") + pNumber->text() + 
		tr(")\tNALU类型(") + pNaluType->text() +
		tr(")\t帧类型(") + pFrameType->text() +
		tr(")\tNALU长度(") + pNaluSize->text() + tr(")");
	ui.curStatusLabel->setText(statusStr);
	int filePtrPos = m_analysis.m_fileStream.tellg();
	m_analysis.m_fileStream.seekg(nalu->pos);
	char *naluData = new char[nalu->size];
	if (m_analysis.readNextBytes(naluData, nalu->size) == H264Analysis::Failed)
		throw exception();
	m_analysis.m_fileStream.seekg(filePtrPos);
	h264_stream_t* h = h264_new();
	int startCodeLen = m_analysis.scLen(naluData);
	read_nal_unit(h, (uint8_t*)&naluData[startCodeLen], nalu->size - startCodeLen);
	debug_nal(h, h->nal);
	QString str = QString("%1\t").arg(0, 8, 16, QChar('0'));
	if (nalu->str.isEmpty())
	{
		for (int i = 0; i < nalu->size; i++)
		{
			if (i % 16 == 0 && i)
			{
				str += "\n";
				str += QString("%1\t").arg(i, 8, 16, QChar('0'));
			}
			str += QString("%1 ").arg((unsigned char)naluData[i], 2, 16, QChar('0'));
		}
		nalu->str = str;
	}
	ui.NaluShowInfoTextEdit->setText(outputstr);
	ui.NaluShowBinaryTextEdit->setText(nalu->str);
	delete [] naluData;
	h264_free(h);
}
