// copyright (c) 2019-2021 hors<horsicq@gmail.com>
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
#include "dialogmodelinfo.h"
#include "ui_dialogmodelinfo.h"

DialogModelInfo::DialogModelInfo(QWidget *pParent) :
    QDialog(pParent),
    ui(new Ui::DialogModelInfo)
{
    ui->setupUi(this);

    g_pDevice=0;
}

DialogModelInfo::~DialogModelInfo()
{
    delete ui;
}

void DialogModelInfo::setData(QIODevice *pDevice, QString sTitle, QStandardItemModel *pModel)
{
    g_pDevice=pDevice;
    g_sTitle=sTitle;

    setWindowTitle(sTitle);

    int nNumberOfRecords=pModel->rowCount();

    // TODO columns
    for(int i=0;i<nNumberOfRecords;i++)
    {
        QString sText=pModel->item(i)->text();

        ui->textEdit->append(sText);
    }
}

void DialogModelInfo::on_pushButtonOK_clicked()
{
    this->close();
}

void DialogModelInfo::on_pushButtonSave_clicked()
{
    QString sFileName=XBinary::getResultFileName(g_pDevice,QString("%1.txt").arg(g_sTitle));
    sFileName=QFileDialog::getSaveFileName(this, tr("Save file"),sFileName, QString("%1 (*.txt);;%2 (*)").arg(tr("Text files"),tr("All files")));

    if(!sFileName.isEmpty())
    {
        XOptions::saveTextEdit(ui->textEdit,sFileName);
    }
}