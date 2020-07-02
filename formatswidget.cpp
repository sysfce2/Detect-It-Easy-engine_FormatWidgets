// Copyright (c) 2020 hors<horsicq@gmail.com>
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
#include "formatswidget.h"
#include "ui_formatswidget.h"

FormatsWidget::FormatsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormatsWidget)
{
    ui->setupUi(this);
}

void FormatsWidget::setFileName(QString sFileName)
{
    this->sFileName=sFileName;

    ui->comboBoxType->clear();

    QSet<XBinary::FT> stTypes=XBinary::getFileTypes(sFileName);

    XFormats::filterFileTypes(&stTypes);

    QList<XBinary::FT> listFileTypes=XBinary::_getFileTypeListFromSet(stTypes);

    int nCount=listFileTypes.count();

    for(int i=0;i<nCount;i++)
    {
        XBinary::FT ft=listFileTypes.at(i);
        ui->comboBoxType->addItem(XBinary::fileTypeIdToString(ft),ft);
    }

    if(nCount)
    {
        ui->comboBoxType->setCurrentIndex(nCount-1);

        reload();
    }
}

FormatsWidget::~FormatsWidget()
{
    delete ui;
}

void FormatsWidget::on_comboBoxType_currentIndexChanged(int index)
{
    const QSignalBlocker blocker(ui->comboBoxType);

    reload();
}

void FormatsWidget::reload()
{
    // TODO
}
