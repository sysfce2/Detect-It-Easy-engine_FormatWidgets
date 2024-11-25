/* Copyright (c) 2017-2024 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef DIALOGPROCESSDATA_H
#define DIALOGPROCESSDATA_H

#include <QDialog>
#include <QMessageBox>
#include <QThread>

#include "processdata.h"
#include "xdialogprocess.h"

class DialogProcessData : public XDialogProcess {
    Q_OBJECT

public:
    explicit DialogProcessData(QWidget *pParent);
    ~DialogProcessData();
    void setData(QStandardItemModel **ppModel, QList<FW_DEF::HEADER_RECORD> *pListHeaderRecords, FW_DEF::CWOPTIONS *pCwOptions);

private:
    ProcessData *g_pProcessData;
    QThread *g_pThread;
};

#endif  // DIALOGPROCESSDATA_H