﻿/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QDir>
#include <QMap>
#include <QFile>
#include <QDebug>
#include <QMetaEnum>
#ifdef SAK_IMPORT_COM_MODULE
#include <QSerialPort>
#endif
#include <QHostAddress>
#include <QApplication>
#include <QMapIterator>
#include <QStandardItem>
#include <QStandardPaths>
#ifdef SAK_IMPORT_COM_MODULE
#include <QSerialPortInfo>
#endif
#include <QNetworkInterface>
#include <QStandardItemModel>

#include "SAKGlobal.hh"
#include "SAKDataStruct.hh"
#include "SAKCRCInterface.hh"

#ifdef SAK_IMPORT_COM_MODULE
Q_DECLARE_METATYPE(QSerialPortInfo)
#endif
SAKGlobal::SAKGlobal(QObject* parent)
    :QObject (parent)
{

}

QString SAKGlobal::logFile()
{
    QString fileName = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);

    QDir dir;
    if (!dir.exists(fileName)){
        SAKGlobal::mkMutiDir(fileName);
    }    

    fileName.append("/");
    fileName.append("QtSwissArmyKnife.txt");

    return fileName;
}

QString SAKGlobal::dataPath()
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    path.insert(path.lastIndexOf("/"),  QString("/Qter"));

    QDir dir;
    if (!dir.exists(path)){
        mkMutiDir(path);
    }

    return path;
}

QString SAKGlobal::mkMutiDir(const QString path){

    QDir dir(path);
    if (dir.exists(path)){
        return path;
    }

    QString parentDir = mkMutiDir(path.mid(0,path.lastIndexOf('/')));
    QString dirname = path.mid(path.lastIndexOf('/') + 1);
    QDir parentPath(parentDir);

    if ( !dirname.isEmpty() ){
        parentPath.mkpath(dirname);
    }

    return parentDir + "/" + dirname;
}

QString SAKGlobal::debugPageNameFromType(int type)
{
    QString name;
    switch (type) {
    case SAKDataStruct::DebugPageTypeUdpClient:
        name = tr("UDP client");
        break;
    case SAKDataStruct::DebugPageTypeTCPClient:
        name = tr("TCP client");
        break;
    case SAKDataStruct::DebugPageTypeTCPServer:
        name = tr("TCP server");
        break;
#ifdef SAK_IMPORT_SCTP_MODULE
    case SAKDataStruct::DebugPageTypeSCTPClient:
        name = tr("SCTP client");
        break;
    case SAKDataStruct::DebugPageTypeSCTPServer:
        name = tr("SCTP server");
        break;
#endif
#ifdef SAK_IMPORT_WEBSOCKET_MODULE
    case SAKDataStruct::DebugPageTypeWebSocketClient:
        name = tr("WebSocket client");
        break;
    case SAKDataStruct::DebugPageTypeWebSocketServer:
        name = tr("WebSocket server");
        break;
#endif
#ifdef SAK_IMPORT_COM_MODULE
    case SAKDataStruct::DebugPageTypeCOM:
        name = tr("COM debugger");
        break;
#endif
#ifdef SAK_IMPORT_HID_MODULE
    case SAKDataStruct::DebugPageTypeHID:
        name = tr("HID debugger");
        break;
#endif
#ifdef SAK_IMPORT_USB_MODULE
    case SAKDataStruct::DebugPageTypeUSB:
        name = tr("USB debugger");
        break;
#endif
#ifdef SAK_IMPORT_BLUETOOTH_MODULE
    case SAKDataStruct::DebugPageTypeBluetoothClient:
        name = tr("Bluetooth client");
        break;
    case SAKDataStruct::DebugPageTypeBluetoothServer:
        name = tr("Bluetooth server");
        break;
#endif
    default:
        Q_ASSERT_X(false, __FUNCTION__, "Unknow debug page type");
        name = QString("Unknow");
        break;
    }

    return name;
}

QString SAKGlobal::toolNameFromType(int type)
{
    QString name("Unknow name of tool");
    switch (type) {
#ifdef SAK_IMPORT_FILECHECKER_MODULE
    case SAKDataStruct::ToolTypeFileChecker:
        name = QString("文件校验器");
        break;
#endif
    case SAKDataStruct::ToolTypeCRCCalculator:
        name = QString("CRC计算器");
        break;
#ifdef SAK_IMPORT_QRCODE_MODULE
    case SAKDataStruct::ToolTypeQRCodeCreator:
        name = QString("二维码生成器");
        break;
#endif
    }

    return name;
}

#ifdef SAK_IMPORT_COM_MODULE
void SAKGlobal::initComComboBox(QComboBox *comboBox)
{
    if (comboBox){
        comboBox->clear();
        QList<QSerialPortInfo> coms = QSerialPortInfo::availablePorts();
        for(auto var:coms){
            comboBox->addItem(var.portName());
        }
    }
}
#endif

#ifdef SAK_IMPORT_COM_MODULE
void SAKGlobal::initBaudRateComboBox(QComboBox *comboBox)
{
    if (comboBox){
        comboBox->clear();
        QList<qint32> bd = QSerialPortInfo::standardBaudRates();
        for (auto var:bd) {
            comboBox->addItem(QString::number(var), QVariant::fromValue(var));
        }

        comboBox->setCurrentText("9600");
    }
}
#endif

#ifdef SAK_IMPORT_COM_MODULE
void SAKGlobal::initDataBitsComboBox(QComboBox *comboBox)
{
    if (comboBox){
        comboBox->clear();
        comboBox->addItem("8", QVariant::fromValue(QSerialPort::Data8));
        comboBox->addItem("7", QVariant::fromValue(QSerialPort::Data7));
        comboBox->addItem("6", QVariant::fromValue(QSerialPort::Data6));
        comboBox->addItem("5", QVariant::fromValue(QSerialPort::Data5));
    }
}
#endif

#ifdef SAK_IMPORT_COM_MODULE
void SAKGlobal::initStopBitsComboBox(QComboBox *comboBox)
{
    if (comboBox){
        comboBox->clear();
        comboBox->addItem("1", QVariant::fromValue(QSerialPort::OneStop));
#ifdef Q_OS_WINDOWS
        comboBox->addItem("1.5", QVariant::fromValue(QSerialPort::OneAndHalfStop));
#endif
        comboBox->addItem("2", QVariant::fromValue(QSerialPort::TwoStop));
    }
}
#endif

#ifdef SAK_IMPORT_COM_MODULE
void SAKGlobal::initParityComboBox(QComboBox *comboBox)
{
    if (comboBox){
        comboBox->clear();
        comboBox->addItem(tr("无"), QVariant::fromValue(QSerialPort::NoParity));
        comboBox->addItem(tr("偶校验"), QVariant::fromValue(QSerialPort::EvenParity));
        comboBox->addItem(tr("奇校验"), QVariant::fromValue(QSerialPort::OddParity));
        comboBox->addItem(tr("空格检验"), QVariant::fromValue(QSerialPort::SpaceParity));
        comboBox->addItem(tr("标记校验"), QVariant::fromValue(QSerialPort::MarkParity));
    }
}
#endif

#ifdef SAK_IMPORT_COM_MODULE
void SAKGlobal::initFlowControlComboBox(QComboBox *comboBox)
{
    if (comboBox){
        comboBox->clear();
        comboBox->addItem(tr("无"), QVariant::fromValue(QSerialPort::NoFlowControl));
        comboBox->addItem(tr("硬件流控"), QVariant::fromValue(QSerialPort::HardwareControl));
        comboBox->addItem(tr("软件流控"), QVariant::fromValue(QSerialPort::SoftwareControl));
    }
}
#endif

void SAKGlobal::initIpComboBox(QComboBox *comboBox, bool appendHostAny)
{
    QString localHost("127.0.0.1");
    if (comboBox){
        comboBox->clear();
        comboBox->addItem(QString("::"));
        comboBox->addItem(QString("::1"));
        comboBox->addItem(QString("0.0.0.0"));
        comboBox->addItem(localHost);
        QList<QHostAddress> addresses = QNetworkInterface::allAddresses();
        for(auto var:addresses){
            if (var.protocol() == QAbstractSocket::IPv4Protocol) {
                if (var.toString().compare(localHost) == 0){
                    continue;
                }
                comboBox->addItem(var.toString());
            }
        }

        comboBox->addItem(QString("255.255.255.255"));
        if (appendHostAny){
            comboBox->addItem(QString(SAK_HOST_ADDRESS_ANY));
        }
        comboBox->setCurrentText(localHost);
    }
}

void SAKGlobal::initInputTextFormatComboBox(QComboBox *comboBox)
{
    if (comboBox){
        comboBox->clear();

        QMap<int, QString> formatMap;
        formatMap.insert(SAKDataStruct::InputFormatBin, tr("Bin"));
        formatMap.insert(SAKDataStruct::InputFormatOct, tr("Dec"));
        formatMap.insert(SAKDataStruct::InputFormatDec, tr("Hex"));
        formatMap.insert(SAKDataStruct::InputFormatHex, tr("Ascii"));
        formatMap.insert(SAKDataStruct::InputFormatAscii, tr("Utf8"));
        formatMap.insert(SAKDataStruct::InputFormatUtf8, tr("Utf16"));
        formatMap.insert(SAKDataStruct::InputFormatLocal, tr("System"));

        QMapIterator<int, QString> mapIterator(formatMap);
        QStandardItemModel *itemModel = new QStandardItemModel(comboBox);
        while (mapIterator.hasNext()) {
            mapIterator.next();
            QStandardItem *item = new QStandardItem(mapIterator.value());
            item->setToolTip(mapIterator.value());
            itemModel->appendRow(item);
        }
        comboBox->setModel(itemModel);
        comboBox->setCurrentText(formatMap.value(SAKDataStruct::InputFormatLocal));
    }
}

void SAKGlobal::initOutputTextFormatComboBox(QComboBox *comboBox)
{
    if (comboBox){
        comboBox->clear();

        QMap<int, QString> formatMap;
        formatMap.insert(SAKDataStruct::OutputFormatBin, tr("Bin"));
        formatMap.insert(SAKDataStruct::OutputFormatDec, tr("Dec"));
        formatMap.insert(SAKDataStruct::OutputFormatHex, tr("Hex"));
        formatMap.insert(SAKDataStruct::OutputFormatAscii, tr("Ascii"));
        formatMap.insert(SAKDataStruct::OutputFormatUtf8, tr("Utf8"));
        formatMap.insert(SAKDataStruct::OutputFormatUtf16, tr("Utf16"));
        formatMap.insert(SAKDataStruct::OutputFormatUcs4, tr("Ucs4"));
        formatMap.insert(SAKDataStruct::OutputFormatStdwstring, tr("Wstring"));
        formatMap.insert(SAKDataStruct::OutputFormatLocal, tr("System"));

        QMapIterator<int, QString> mapIterator(formatMap);
        QStandardItemModel *itemModel = new QStandardItemModel(comboBox);
        while (mapIterator.hasNext()) {
            mapIterator.next();
            QStandardItem *item = new QStandardItem(mapIterator.value());
            item->setToolTip(mapIterator.value());
            itemModel->appendRow(item);
        }
        comboBox->setModel(itemModel);
        comboBox->setCurrentText(formatMap.value(SAKDataStruct::OutputFormatLocal));
    }
}

void SAKGlobal::initCRCComboBox(QComboBox *comboBox)
{
    if (comboBox){
        comboBox->clear();
        QMetaEnum enums = QMetaEnum::fromType<SAKCRCInterface::CRCModel>();
        QStandardItemModel *itemModel = new QStandardItemModel(comboBox);
        for (int i = 0; i < enums.keyCount(); i++){
            const QString key = enums.key(i);
            // There may be a bug, I do not know whether will the itemModel take ownership of the item
            // if not, a memory leak will occur after comboBox is destroyed.
            QStandardItem *item = new QStandardItem(key);
            item->setToolTip(key);
            itemModel->appendRow(item);
        }
        comboBox->setModel(itemModel);
    }
}

void SAKGlobal::initWebSocketSendingTypeComboBox(QComboBox *comboBox)
{
    if (comboBox){
        comboBox->addItem(tr("文本发送方式"), SAKDataStruct::WebSocketSendingTypeText);
        comboBox->addItem(tr("二进制发送方式"), SAKDataStruct::WebSocketSendingTypeBin);
    }
}
