﻿/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#ifndef SAKSERIALPORTDEVICET_HH
#define SAKSERIALPORTDEVICET_HH

#include <QMutex>
#include <QSerialPort>
#include <QWaitCondition>

#include "SAKDebuggerDevice.hh"
#include "SAKCommonDataStructure.hh"

class SAKSerialPortDebugger;
class SAKSerialPortController;
class SAKSerialPortDevice : public SAKDebuggerDevice
{
    Q_OBJECT
public:
    SAKSerialPortDevice(QSettings *settings,
                        const QString &settingsGroup,
                        QWidget *uiParent,
                        QObject *parent = Q_NULLPTR);
    void setParametersCtx(
            SAKCommonDataStructure::SAKStructSerialPortParametersContext ctx
            );

    bool initialize() final;
    QByteArray read() final;
    QByteArray write(const QByteArray &bytes) final;
    void uninitialize() final;


private:
    QSerialPort *mSerialPort;
    SAKCommonDataStructure::SAKStructSerialPortParametersContext mInnerParametersContext;
    QMutex mInnerParametersContextMutex;


private:
    SAKCommonDataStructure::SAKStructSerialPortParametersContext parametersCtx();
};

#endif
