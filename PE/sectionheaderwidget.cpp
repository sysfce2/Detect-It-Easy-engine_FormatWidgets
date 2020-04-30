// copyright (c) 2017-2020 hors<horsicq@gmail.com>
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
#include "sectionheaderwidget.h"
#include "ui_sectionheaderwidget.h"

SectionHeaderWidget::SectionHeaderWidget(QWidget *parent):
    FormatWidget(parent),
    ui(new Ui::SectionHeaderWidget)
{
    ui->setupUi(this);
}

SectionHeaderWidget::SectionHeaderWidget(QIODevice *pDevice, FW_DEF::OPTIONS *pOptions, quint32 nNumber, qint64 nOffset, qint32 nType, QWidget *parent):
    FormatWidget(pDevice,pOptions,nNumber,nOffset,nType,parent),
    ui(new Ui::SectionHeaderWidget)
{
    ui->setupUi(this);

    setData(pDevice,pOptions,nNumber,nOffset,nType);

    ppLinedEdit=0;
    nLineEditSize=0;
    ppComboBox=0;
    nComboBoxSize=0;
    ppInvWidget=0;
    nInvWidgetSize=0;

    if(nType==SPE::TYPE_IMAGE_SECTION_HEADER)
    {
        nLineEditSize=N_IMAGE_SECTION_HEADER::__data_size+1;
        nComboBoxSize=N_IMAGE_SECTION_HEADER::__CB_size;
        nInvWidgetSize=N_IMAGE_SECTION_HEADER::__INV_size;
    }

    if(nLineEditSize)
    {
        ppLinedEdit=new PXLineEditHEX[nLineEditSize];
    }

    if(nComboBoxSize)
    {
        ppComboBox=new PXComboBoxEx[nComboBoxSize];
    }

    if(nInvWidgetSize)
    {
        ppInvWidget=new PInvWidget[nInvWidgetSize];
    }
}

SectionHeaderWidget::~SectionHeaderWidget()
{
    if(ppLinedEdit)
    {
        delete[] ppLinedEdit;
    }

    if(ppComboBox)
    {
        delete[] ppComboBox;
    }

    if(ppInvWidget)
    {
        delete[] ppInvWidget;
    }

    delete ui;
}

void SectionHeaderWidget::clear()
{
    reset();

    memset(ppLinedEdit,0,nLineEditSize*sizeof(XLineEditHEX *));
    memset(ppComboBox,0,nComboBoxSize*sizeof(XComboBoxEx *));
    memset(ppInvWidget,0,nInvWidgetSize*sizeof(InvWidget *));

    pSubDevice=nullptr;

    ui->checkBoxReadonly->setChecked(true);
}

void SectionHeaderWidget::cleanup()
{

}

void SectionHeaderWidget::reset()
{
    bInit=false;
}

void SectionHeaderWidget::reload()
{
    clear();

    ui->checkBoxReadonly->setEnabled(!isReadonly());

    reloadData();
}

bool SectionHeaderWidget::_setValue(QVariant vValue, int nStype, int nNdata, int nVtype,int nPosition,qint64 nOffset)
{
    Q_UNUSED(nVtype)
    Q_UNUSED(nOffset)

    bool bResult=false;

    quint64 nValue=vValue.toULongLong();
    QString sValue=vValue.toString();

    if(getDevice()->isWritable())
    {
        XPE pe(getDevice(),getOptions()->bIsImage,getOptions()->nImageBase);

        if(pe.isValid())
        {
            switch(nStype)
            {
                case SPE::TYPE_IMAGE_SECTION_HEADER:
                    switch(nNdata)
                    {
                        case N_IMAGE_SECTION_HEADER::VirtualAddress:        ppInvWidget[N_IMAGE_SECTION_HEADER::INV_VirtualAddress]->setAddressAndSize(&pe,pe.getBaseAddress()+(quint32)nValue,0);        break;
                        case N_IMAGE_SECTION_HEADER::PointerToRawData:      ppInvWidget[N_IMAGE_SECTION_HEADER::INV_PointerToRawData]->setOffsetAndSize(&pe,(quint32)nValue,0);                           break;
                    }

                    break;
            }

            switch(nStype)
            {
                case SPE::TYPE_IMAGE_SECTION_HEADER:
                    switch(nNdata)
                    {
                        case N_IMAGE_SECTION_HEADER::Name:                  pe.setSection_NameAsString((quint32)nPosition,sValue);                  break;
                        case N_IMAGE_SECTION_HEADER::VirtualSize:           pe.setSection_VirtualSize((quint32)nPosition,(quint32)nValue);          break;
                        case N_IMAGE_SECTION_HEADER::VirtualAddress:        pe.setSection_VirtualAddress((quint32)nPosition,(quint32)nValue);       break;
                        case N_IMAGE_SECTION_HEADER::SizeOfRawData:         pe.setSection_SizeOfRawData((quint32)nPosition,(quint32)nValue);        break;
                        case N_IMAGE_SECTION_HEADER::PointerToRawData:      pe.setSection_PointerToRawData((quint32)nPosition,(quint32)nValue);     break;
                        case N_IMAGE_SECTION_HEADER::PointerToRelocations:  pe.setSection_PointerToRelocations((quint32)nPosition,(quint32)nValue); break;
                        case N_IMAGE_SECTION_HEADER::PointerToLinenumbers:  pe.setSection_PointerToLinenumbers((quint32)nPosition,(quint32)nValue); break;
                        case N_IMAGE_SECTION_HEADER::NumberOfRelocations:   pe.setSection_NumberOfRelocations((quint32)nPosition,(quint16)nValue);  break;
                        case N_IMAGE_SECTION_HEADER::NumberOfLinenumbers:   pe.setSection_NumberOfLinenumbers((quint32)nPosition,(quint16)nValue);  break;
                        case N_IMAGE_SECTION_HEADER::Characteristics:       pe.setSection_Characteristics((quint32)nPosition,(quint32)nValue);      break;
                    }

                    ui->widgetHex->reload();

                    break;
            }

            bResult=true;
        }
    }

    return bResult;
}
void SectionHeaderWidget::setReadonly(bool bState)
{
    setLineEditsReadOnly(ppLinedEdit,nLineEditSize,bState);

    setComboBoxesReadOnly(ppComboBox,nComboBoxSize,bState);
}

void SectionHeaderWidget::blockSignals(bool bState)
{
    _blockSignals((QObject **)ppLinedEdit,nLineEditSize,bState);

    _blockSignals((QObject **)ppComboBox,nComboBoxSize,bState);
}

void SectionHeaderWidget::adjustHeaderTable(int type, QTableWidget *pTableWidget)
{
    int nSymbolWidth=XLineEditHEX::getSymbolWidth(this);

    switch(type)
    {
        case SPE::TYPE_IMAGE_SECTION_HEADER:
            pTableWidget->setColumnWidth(HEADER_COLUMN_OFFSET,nSymbolWidth*4);
            pTableWidget->setColumnWidth(HEADER_COLUMN_TYPE,nSymbolWidth*6);
            pTableWidget->setColumnWidth(HEADER_COLUMN_NAME,nSymbolWidth*16);
            pTableWidget->setColumnWidth(HEADER_COLUMN_VALUE,nSymbolWidth*8);
            pTableWidget->setColumnWidth(HEADER_COLUMN_INFO,nSymbolWidth*16);
            break;
    }
}

void SectionHeaderWidget::on_checkBoxReadonly_toggled(bool checked)
{
    setReadonly(checked);
}

void SectionHeaderWidget::reloadData()
{
    int nType=getType();

    XPE pe(getDevice(),getOptions()->bIsImage,getOptions()->nImageBase);

    if(pe.isValid())
    {
        if(!bInit)
        {
            if(nType==SPE::TYPE_IMAGE_SECTION_HEADER)
            {
                bInit=createHeaderTable(SPE::TYPE_IMAGE_SECTION_HEADER,ui->tableWidget,N_IMAGE_SECTION_HEADER::records,ppLinedEdit,N_IMAGE_SECTION_HEADER::__data_size+1,getNumber());
                ppComboBox[N_IMAGE_SECTION_HEADER::CB_CHARACTERISTICS]=createComboBox(ui->tableWidget,XPE::getImageSectionHeaderFlagsS(),SPE::TYPE_IMAGE_SECTION_HEADER,N_IMAGE_SECTION_HEADER::Characteristics,XComboBoxEx::CBTYPE_FLAGS);
                ppComboBox[N_IMAGE_SECTION_HEADER::CB_ALIGH]=createComboBox(ui->tableWidget,XPE::getImageSectionHeaderAlignsS(),SPE::TYPE_IMAGE_SECTION_HEADER,N_IMAGE_SECTION_HEADER::Characteristics+1,XComboBoxEx::CBTYPE_EFLAGS,XPE_DEF::S_IMAGE_SCN_ALIGN_MASK);

                ppInvWidget[N_IMAGE_SECTION_HEADER::INV_VirtualAddress]=createInvWidget(ui->tableWidget,SPE::TYPE_IMAGE_SECTION_HEADER,N_IMAGE_SECTION_HEADER::VirtualAddress,InvWidget::TYPE_HEX);
                ppInvWidget[N_IMAGE_SECTION_HEADER::INV_PointerToRawData]=createInvWidget(ui->tableWidget,SPE::TYPE_IMAGE_SECTION_HEADER,N_IMAGE_SECTION_HEADER::PointerToRawData,InvWidget::TYPE_HEX);

                blockSignals(true);

                XPE_DEF::IMAGE_SECTION_HEADER ish=pe.getSectionHeader(getNumber());

                //        lineEdit_IMAGE_SECTION_HEADER[N_IMAGE_SECTION_HEADER::e_magic]->setValue(ish.);

                QString sName=QString((char *)ish.Name);
                sName.resize(qMin(sName.length(),XPE_DEF::S_IMAGE_SIZEOF_SHORT_NAME));

                ppLinedEdit[N_IMAGE_SECTION_HEADER::Name]->setStringValue(sName,XPE_DEF::S_IMAGE_SIZEOF_SHORT_NAME); // TODO Check
                ppLinedEdit[N_IMAGE_SECTION_HEADER::VirtualSize]->setValue(ish.Misc.VirtualSize);
                ppLinedEdit[N_IMAGE_SECTION_HEADER::VirtualAddress]->setValue(ish.VirtualAddress);
                ppLinedEdit[N_IMAGE_SECTION_HEADER::SizeOfRawData]->setValue(ish.SizeOfRawData);
                ppLinedEdit[N_IMAGE_SECTION_HEADER::PointerToRawData]->setValue(ish.PointerToRawData);
                ppLinedEdit[N_IMAGE_SECTION_HEADER::PointerToRelocations]->setValue(ish.PointerToRelocations);
                ppLinedEdit[N_IMAGE_SECTION_HEADER::PointerToLinenumbers]->setValue(ish.PointerToLinenumbers);
                ppLinedEdit[N_IMAGE_SECTION_HEADER::NumberOfRelocations]->setValue(ish.NumberOfRelocations);
                ppLinedEdit[N_IMAGE_SECTION_HEADER::NumberOfLinenumbers]->setValue(ish.NumberOfLinenumbers);
                ppLinedEdit[N_IMAGE_SECTION_HEADER::Characteristics]->setValue(ish.Characteristics);

                ppComboBox[N_IMAGE_SECTION_HEADER::CB_CHARACTERISTICS]->setValue(ish.Characteristics);
                ppComboBox[N_IMAGE_SECTION_HEADER::CB_ALIGH]->setValue(ish.Characteristics);

                ppInvWidget[N_IMAGE_SECTION_HEADER::INV_VirtualAddress]->setAddressAndSize(&pe,pe.getBaseAddress()+ish.VirtualAddress,0);
                ppInvWidget[N_IMAGE_SECTION_HEADER::INV_PointerToRawData]->setOffsetAndSize(&pe,ish.PointerToRawData,0);

                qint64 nOffset=pe.getSectionHeaderOffset(getNumber());
                qint64 nSize=pe.getSectionHeaderSize();
                qint64 nAddress=pe.offsetToRelAddress(nOffset);

                loadHexSubdevice(nOffset,nSize,nAddress,&pSubDevice,ui->widgetHex);

                blockSignals(false);
            }
        }

        setReadonly(ui->checkBoxReadonly->isChecked());
    }
}

void SectionHeaderWidget::widgetValueChanged(quint64 nValue)
{
    QWidget *pWidget=qobject_cast<QWidget *>(sender());
    int nStype=pWidget->property("STYPE").toInt();
    int nNdata=pWidget->property("NDATA").toInt();

    switch(nStype)
    {
        case SPE::TYPE_IMAGE_SECTION_HEADER:
            switch(nNdata)
            {
                case N_IMAGE_SECTION_HEADER::Characteristics:
                    ppLinedEdit[N_IMAGE_SECTION_HEADER::Characteristics]->setValue((quint32)nValue);
                    ppComboBox[N_IMAGE_SECTION_HEADER::CB_CHARACTERISTICS]->setValue(nValue);
                    break;

                case N_IMAGE_SECTION_HEADER::Characteristics+1:
                    ppLinedEdit[N_IMAGE_SECTION_HEADER::Characteristics]->setValue((quint32)nValue);
                    ppComboBox[N_IMAGE_SECTION_HEADER::CB_ALIGH]->setValue(nValue);
                    break;
            }

            break;
    }
}

void SectionHeaderWidget::on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(currentRow)
    Q_UNUSED(currentColumn)
    Q_UNUSED(previousRow)
    Q_UNUSED(previousColumn)

    setHeaderTableSelection(ui->widgetHex,ui->tableWidget);
}
