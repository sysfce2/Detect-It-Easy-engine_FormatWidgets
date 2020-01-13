// copyright (c) 2019 hors<horsicq@gmail.com>
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
#ifndef NEWIDGET_H
#define NEWIDGET_H

#include "../formatwidget.h"
#include "ne_defs.h"

namespace Ui
{
class NEWidget;
}

class NEWidget : public FormatWidget
{
    Q_OBJECT

public:
    NEWidget(QWidget *parent=nullptr);
    NEWidget(QIODevice *pDevice, FW_DEF::OPTIONS *pOptions, QWidget *parent=nullptr);
    ~NEWidget();
    virtual void clear();
    virtual void setData(QIODevice *pDevice,FW_DEF::OPTIONS *pOptions);
    virtual void reload();

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
    void on_pushButtonReload_clicked();
    void on_tableWidget_DOS_HEADER_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void on_tableWidget_OS2_HEADER_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

private:
    enum CB
    {
        CB_DOS_HEADER_e_magic=0,
        CB_OS2_HEADER_ne_magic,
        CB_OS2_HEADER_ne_flags,
        __CB_size
    };

    Ui::NEWidget *ui;

    XLineEditHEX *lineEdit_DOS_HEADER[N_DOS_HEADER::__data_size];
    XLineEditHEX *lineEdit_OS2_HEADER[N_OS2_HEADER::__data_size];
    XComboBoxEx *comboBox[__CB_size];

    bool bInit[SNE::__TYPE_size];

    SubDevice *subDevice[SNE::__TYPE_size];
};

#endif // NEWIDGET_H
