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
#ifndef ELFWIDGET_H
#define ELFWIDGET_H

#include "../formatwidget.h"
#include "elf_defs.h"

namespace Ui {
class ELFWidget;
}

class ELFWidget : public FormatWidget
{
    Q_OBJECT

public:
    ELFWidget(QWidget *parent = nullptr);
    ELFWidget(QIODevice *pDevice, OPTIONS *pOptions, QWidget *parent = nullptr);
    ~ELFWidget();
    virtual void clear();
    virtual void setData(QIODevice *pDevice,OPTIONS *pOptions);
protected:
    virtual bool _setValue(QVariant vValue,int nStype,int nNdata,int nVtype,int nPosition);
    virtual void setReadonly(bool bState);
    virtual void blockSignals(bool bState);
    virtual void adjustHeaderTable(int type,QTableWidget *pTableWidget);
private slots:
    virtual void reloadData();
    void widgetValueChanged(quint64 nValue);
    void on_treeWidgetNavi_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void on_checkBoxReadonly_toggled(bool checked);
    void on_tableWidget_Elf_Shdr_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

private:
    enum CB
    {
        CB_Elf_Ehdr_iclass=0,
        CB_Elf_Ehdr_idata,
        CB_Elf_Ehdr_iversion,
        CB_Elf_Ehdr_iosabi,
        CB_Elf_Ehdr_type,
        CB_Elf_Ehdr_machine,
        __CB_size
    };

    Ui::ELFWidget *ui;

    XLineEditHEX *lineEdit_Elf_Ehdr[N_Elf_Ehdr::__data_size];
    XComboBoxEx *comboBox[__CB_size];

    bool bInit[SELF::__TYPE_size];

    SubDevice *pSubDeviceSection;
    SubDevice *pSubDeviceProgram;
};

#endif // ELFWIDGET_H
