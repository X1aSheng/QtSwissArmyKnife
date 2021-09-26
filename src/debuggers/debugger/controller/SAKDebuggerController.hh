﻿/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#ifndef SAKDEBUGGERCONTROLLER_H
#define SAKDEBUGGERCONTROLLER_H

#include <QWidget>
#include <QSettings>

/// @brief Device controller
class SAKDebuggerController:public QWidget
{
    Q_OBJECT
public:
    SAKDebuggerController(QSettings *settings,
                          const QString &settingsGroup,
                          QWidget *parent = Q_NULLPTR);
    ~SAKDebuggerController();

    /**
     * @brief setUiEnable: Set ui state.
     * @param opened: If the device is opened, the value of the parameters is true.
     */
    virtual void updateUiState(bool opened) = 0;

    /**
     * @brief refreshDevice: Refresh the device list.
     */
    virtual void refreshDevice() = 0;

    /**
     * @brief parametersContext: Get parameters context of device.
     * @return Parameters context of device.
     */
    virtual QVariant parametersContext() = 0;
protected:
    QSettings *mSettings;
    const QString mSettingsGroup;
signals:
    void messageChanged(QString msg, bool isError = true);
    void parametersContextChanged();
};

#endif
