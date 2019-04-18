// copyright (c) 2017-2019 hors<horsicq@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#include "machwidget.h"
#include "ui_machwidget.h"

MACHWidget::MACHWidget(QWidget *parent) :
    FormatWidget(parent),
    ui(new Ui::MACHWidget)
{
    ui->setupUi(this);
}

MACHWidget::MACHWidget(QIODevice *pDevice, OPTIONS *pOptions, QWidget *parent) :
    FormatWidget(pDevice,pOptions,parent),
    ui(new Ui::MACHWidget)
{
    ui->setupUi(this);

    setData(pDevice,pOptions);
}

MACHWidget::~MACHWidget()
{
    delete ui;
}

void MACHWidget::clear()
{
    memset(bInit,0,sizeof bInit);
    memset(lineEdit_mach_header,0,sizeof lineEdit_mach_header);
    memset(comboBox,0,sizeof comboBox);

    ui->checkBoxReadonly->setChecked(true);

    ui->widgetHex->enableHeader(true);
    ui->widgetHex->enableReadOnly(false);

    ui->treeWidgetNavi->clear();
}

void MACHWidget::setData(QIODevice *pDevice, FormatWidget::OPTIONS *pOptions)
{
    FormatWidget::setData(pDevice,pOptions);

    XMACH mach(pDevice,getOptions()->bIsImage,getOptions()->nImageAddress);

    if(mach.isValid())
    {
        // mb TODO 32/64
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SMACH::TYPE_HEX,"HEX"));
        ui->treeWidgetNavi->addTopLevelItem(createNewItem(SMACH::TYPE_mach_header,"mach_header"));

        // TODO Commands

        ui->treeWidgetNavi->expandAll();

        ui->treeWidgetNavi->setCurrentItem(ui->treeWidgetNavi->topLevelItem(1));
    }
}

bool MACHWidget::_setValue(QVariant vValue, int nStype, int nNdata, int nVtype, int nPosition)
{
    bool bResult=false;

    blockSignals(true);

    quint64 nValue=vValue.toULongLong();

    if(getDevice()->isWritable())
    {
        XMACH mach(getDevice(),getOptions()->bIsImage,getOptions()->nImageAddress);
        if(mach.isValid())
        {
            switch(nStype)
            {
            case SMACH::TYPE_mach_header:
                switch(nNdata)
                {
                case N_mach_header::magic:
                    // TODO reload all data
                    comboBox[CB_mach_header_magic]->setValue(nValue);
                    mach.setHeader_magic((quint32)nValue);
                    break;
                case N_mach_header::cputype:
                    comboBox[CB_mach_header_cputype]->setValue(nValue);
                    mach.setHeader_cputype((qint32)nValue);
                    break;
                case N_mach_header::cpusubtype:
                    mach.setHeader_cpusubtype((qint32)nValue);
                    break;
                case N_mach_header::filetype:
                    comboBox[CB_mach_header_filetype]->setValue(nValue);
                    mach.setHeader_filetype((quint32)nValue);
                    break;
                case N_mach_header::ncmds:
                    mach.setHeader_ncmds((quint32)nValue);
                    break;
                case N_mach_header::sizeofcmds:
                    mach.setHeader_sizeofcmds((quint32)nValue);
                    break;
                case N_mach_header::flags:
                    comboBox[CB_mach_header_flags]->setValue(nValue);
                    mach.setHeader_flags((quint32)nValue);
                    break;
                case N_mach_header::reserved:
                    mach.setHeader_reserved((quint32)nValue);
                    break;
                }
                break;
            }
            bResult=true;
        }
    }

    blockSignals(false);

    return bResult;
}

void MACHWidget::setReadonly(bool bState)
{
    setLineEditsReadOnly(lineEdit_mach_header,N_mach_header::__data_size,bState);

    setComboBoxesReadOnly(comboBox,__CB_size,bState);

    ui->widgetHex->setReadonly(bState);
}

void MACHWidget::blockSignals(bool bState)
{
    _blockSignals((QObject **)lineEdit_mach_header,N_mach_header::__data_size,bState);

    _blockSignals((QObject **)comboBox,__CB_size,bState);
}

void MACHWidget::adjustHeaderTable(int type, QTableWidget *pTableWidget)
{
    int nSymbolWidth=getSymbolWidth();

    switch(type)
    {
    case SMACH::TYPE_mach_header:
        pTableWidget->setColumnWidth(0,nSymbolWidth*8);
        pTableWidget->setColumnWidth(1,nSymbolWidth*10);
        pTableWidget->setColumnWidth(2,nSymbolWidth*10);
        pTableWidget->setColumnWidth(3,nSymbolWidth*26);
        break;
    }
}

void MACHWidget::reloadData()
{
    int nData=ui->treeWidgetNavi->currentItem()->data(0,Qt::UserRole).toInt();
    ui->stackedWidgetInfo->setCurrentIndex(nData);

    XMACH mach(getDevice(),getOptions()->bIsImage,getOptions()->nImageAddress);
    if(mach.isValid())
    {
        if(nData==SMACH::TYPE_HEX)
        {
            if(!bInit[nData])
            {
                QHexView::OPTIONS options={0};

                options.nBaseAddress=getOptions()->nImageAddress;
                options.sBackupFileName=getOptions()->sBackupFileName;

                ui->widgetHex->setData(getDevice(),&options);

                bInit[nData]=true;
            }
        }
        else if(nData==SMACH::TYPE_mach_header)
        {
            if(!bInit[nData])
            {
                if(!mach.is64())
                {
                    bInit[nData]=createHeaderTable(SMACH::TYPE_mach_header,ui->tableWidget_mach_header,N_mach_header::records32,lineEdit_mach_header,N_mach_header::__data_size-1,0);
                }
                else
                {
                    bInit[nData]=createHeaderTable(SMACH::TYPE_mach_header,ui->tableWidget_mach_header,N_mach_header::records64,lineEdit_mach_header,N_mach_header::__data_size,0);
                }

                comboBox[CB_mach_header_magic]=createComboBox(ui->tableWidget_mach_header,XMACH::getHeaderMagicsS(),SMACH::TYPE_mach_header,N_mach_header::magic,XComboBoxEx::CBTYPE_NORMAL);
                comboBox[CB_mach_header_cputype]=createComboBox(ui->tableWidget_mach_header,XMACH::getHeaderCpuTypesS(),SMACH::TYPE_mach_header,N_mach_header::cputype,XComboBoxEx::CBTYPE_NORMAL);
                comboBox[CB_mach_header_filetype]=createComboBox(ui->tableWidget_mach_header,XMACH::getHeaderFileTypesS(),SMACH::TYPE_mach_header,N_mach_header::filetype,XComboBoxEx::CBTYPE_NORMAL);
                comboBox[CB_mach_header_flags]=createComboBox(ui->tableWidget_mach_header,XMACH::getHeaderFlagsS(),SMACH::TYPE_mach_header,N_mach_header::flags,XComboBoxEx::CBTYPE_FLAGS);
            }

            blockSignals(true);

            lineEdit_mach_header[N_mach_header::magic]->setValue(mach.getHeader_magic());
            lineEdit_mach_header[N_mach_header::cputype]->setValue(mach.getHeader_cputype());
            lineEdit_mach_header[N_mach_header::cpusubtype]->setValue(mach.getHeader_cpusubtype());
            lineEdit_mach_header[N_mach_header::filetype]->setValue(mach.getHeader_filetype());
            lineEdit_mach_header[N_mach_header::ncmds]->setValue(mach.getHeader_ncmds());
            lineEdit_mach_header[N_mach_header::sizeofcmds]->setValue(mach.getHeader_sizeofcmds());
            lineEdit_mach_header[N_mach_header::flags]->setValue(mach.getHeader_flags());

            if(mach.is64())
            {
                lineEdit_mach_header[N_mach_header::reserved]->setValue(mach.getHeader_reserved());
            }

            comboBox[CB_mach_header_magic]->setValue(mach.getHeader_magic());
            comboBox[CB_mach_header_cputype]->setValue((quint32)mach.getHeader_cputype());
            comboBox[CB_mach_header_filetype]->setValue((quint32)mach.getHeader_filetype());
            comboBox[CB_mach_header_flags]->setValue((quint32)mach.getHeader_flags());

            blockSignals(false);
        }

        setReadonly(ui->checkBoxReadonly->isChecked());
    }
}

void MACHWidget::widgetValueChanged(quint64 nValue)
{
    QWidget *pWidget=qobject_cast<QWidget *>(sender());
    int nStype=pWidget->property("STYPE").toInt();
    int nNdata=pWidget->property("NDATA").toInt();

    switch(nStype)
    {
    case SMACH::TYPE_mach_header:
        switch(nNdata)
        {
        case N_mach_header::magic:
            lineEdit_mach_header[N_mach_header::magic]->setValue((quint32)nValue);
            break;
        case N_mach_header::cputype:
            lineEdit_mach_header[N_mach_header::cputype]->setValue((quint32)nValue);
            break;
        case N_mach_header::filetype:
            lineEdit_mach_header[N_mach_header::filetype]->setValue((quint32)nValue);
            break;
        case N_mach_header::flags:
            lineEdit_mach_header[N_mach_header::flags]->setValue((quint32)nValue);
            break;
        }
        break;
    }
}

void MACHWidget::on_treeWidgetNavi_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    if(current)
    {
        reloadData();
    }
}

void MACHWidget::on_checkBoxReadonly_toggled(bool checked)
{
    setReadonly(checked);
}
