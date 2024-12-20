/* Copyright (c) 2019-2024 hors<horsicq@gmail.com>
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
#ifndef MULTISEARCH_H
#define MULTISEARCH_H

#include <QElapsedTimer>
#include <QFuture>
#include <QIODevice>
#include <QSemaphore>
#include <QStandardItemModel>
#include <QtConcurrent>
// #include <QtConcurrentMap>

#include "xformats.h"
#include "xoptions.h"
// #include "xinfodb.h"

class MultiSearch : public QObject {
    Q_OBJECT

public:
    enum TYPE {
        TYPE_STRINGS = 0,
        TYPE_SIGNATURES,
        TYPE_VALUES,
        TYPE_STRINGS_XINFODB
    };

    enum COLUMN_STRING {
        COLUMN_STRING_NUMBER,
        COLUMN_STRING_OFFSET,
        COLUMN_STRING_ADDRESS,
        COLUMN_STRING_REGION,
        COLUMN_STRING_SIZE,
        COLUMN_STRING_TYPE,
        COLUMN_STRING_VALUE,
        __COLUMN_STRING_SIZE
    };

    enum COLUMN_VALUE {
        COLUMN_VALUE_NUMBER,
        COLUMN_VALUE_OFFSET,
        COLUMN_VALUE_ADDRESS,
        COLUMN_VALUE_REGION,
        COLUMN_VALUE_VALUE,
        __COLUMN_VALUE_SIZE
    };

    enum USERROLE {
        USERROLE_SIZE = 0,
        USERROLE_OFFSET,
        USERROLE_ADDRESS,
        USERROLE_STRING,
        USERROLE_INFO,
        USERROLE_TYPE
    };

    struct SIGNATURE_RECORD {
        qint32 nNumber;
        QString sName;
        XBinary::ENDIAN endian;
        QString sSignature;
    };

    struct OPTIONS {
        XBinary::_MEMORY_MAP memoryMap;
        bool bAnsi;
        // bool bUTF8;     // TODO
        bool bUnicode;
        bool bNullTerminated;
        // QString sANSICodec;
        bool bLinks;
        QString sMask;
        XBinary::ENDIAN endian;
        qint32 nMinLenght;
        QList<SIGNATURE_RECORD> *pListSignatureRecords;
        QVariant varValue;
        XBinary::VT valueType;
        bool bMenu_Hex;  // TODO remove
    };

    const qint32 N_MAX = 100000;
    const qint32 N_MAXNUMBEROFTHREADS = 8;

    explicit MultiSearch(QObject *pParent = nullptr);
    ~MultiSearch();

    void setSearchData(QIODevice *pDevice, QList<XBinary::MS_RECORD> *pListRecords, OPTIONS options, TYPE type, XBinary::PDSTRUCT *pPdStruct);
    void setModelData(QList<XBinary::MS_RECORD> *pListRecords, QStandardItemModel **ppModel, OPTIONS options, TYPE type, XBinary::PDSTRUCT *pPdStruct);

    static QList<SIGNATURE_RECORD> loadSignaturesFromFile(const QString &sFileName);
    static SIGNATURE_RECORD createSignature(const QString &sName, const QString &sSignature);
    void processSignature(SIGNATURE_RECORD signatureRecord);

signals:
    void errorMessage(const QString &sText);
    void completed(qint64 nElapsed);

public slots:
    void processSearch();
    void processModel();

private:
    QIODevice *g_pDevice;
    QList<XBinary::MS_RECORD> *g_pListRecords;
    OPTIONS g_options;
    TYPE g_type;
    QStandardItemModel **g_ppModel;
    QSemaphore *g_pSemaphore;
    QMutex g_mutex;
    XBinary::PDSTRUCT *g_pPdStruct;
    qint32 g_nFreeIndex;
};

#endif  // MULTISEARCH_H
