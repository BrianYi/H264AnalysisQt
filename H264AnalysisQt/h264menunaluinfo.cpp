#include "h264menunaluinfo.h"

h264menunaluinfo::h264menunaluinfo(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.naluTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); /// 设置不可编辑
	ui.naluTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); /// 设置单击选中整行 
	ui.naluTableWidget->setSelectionMode(QAbstractItemView::SingleSelection); /// 设置只能选中单行
	ui.naluTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.naluTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	m_pNaluItem = NULL;
}

h264menunaluinfo::~h264menunaluinfo()
{

}

void h264menunaluinfo::clearData()
{
	int rowCount = ui.naluTableWidget->rowCount();
	for (int i = 0; i < rowCount; i++)
	{
		QTableWidgetItem *pName = ui.naluTableWidget->item(i, 0);
		QTableWidgetItem *pNum = ui.naluTableWidget->item(i, 1);
		QTableWidgetItem *pMB = ui.naluTableWidget->item(i, 2);
		if (pName)
			delete pName;
		if (pNum)
			delete pNum;
		if (pMB)
			delete pMB;
		ui.naluTableWidget->removeRow(i);
	}
	ui.naluTableWidget->clearContents();
}

void h264menunaluinfo::dealUpdateH264MenuNaluInfo( PMetaData pMetaData, unsigned int num )
{
	clearData();
	if (!m_pNaluItem)
		m_pNaluItem = new NaluItem[num];

	for (int i = 0; i < num; i++)
	{
		ui.naluTableWidget->insertRow(i);
		QString name = pMetaData[i].name;
		QString num = QString::number(pMetaData[i].num);
		QString mb = QString::number(pMetaData[i].bytes / 1024 / 1024);
		m_pNaluItem[i].nameItem= new QTableWidgetItem(name);
		m_pNaluItem[i].numItem = new QTableWidgetItem(num);
		m_pNaluItem[i].mbItem = new QTableWidgetItem(mb);
		m_pNaluItem[i].nameItem->setTextAlignment(Qt::AlignCenter);
		m_pNaluItem[i].numItem->setTextAlignment(Qt::AlignCenter);
		m_pNaluItem[i].mbItem->setTextAlignment(Qt::AlignCenter);
		ui.naluTableWidget->setItem(i, 0, m_pNaluItem[i].nameItem);
		ui.naluTableWidget->setItem(i, 1, m_pNaluItem[i].numItem);
		ui.naluTableWidget->setItem(i, 2, m_pNaluItem[i].mbItem);
	}
	
	ui.naluTableWidget->setRowCount(num);
}

void h264menunaluinfo::showMenuNaluInfo()
{

	this->show();
}
