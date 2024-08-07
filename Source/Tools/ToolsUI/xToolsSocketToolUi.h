﻿/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "xToolsCommunicationToolUi.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class xToolsSocketToolUi;
}
QT_END_NAMESPACE

class xToolsSocketToolUi : public xToolsCommunicationToolUi
{
    Q_OBJECT
public:
    xToolsSocketToolUi(QWidget *parent = nullptr);
    ~xToolsSocketToolUi();

    virtual QVariantMap save() const;
    virtual void load(const QVariantMap &parameters);

signals:
    void currentClientChanged(const QString &flag);
    void invokeDisconnectAll();

protected:
    void setClientWidgetsVisible(bool visible);
    void setServerWidgetsVisible(bool visible);
    void setChannelWidgetsVisible(bool visible);
    void setAuthenticationWidgetsVisible(bool visible);
    void setWriteToWidgetsVisible(bool visible);

    void setClientWidgetsEnabled(bool enabled);
    void setServerWidgetsEnabled(bool enabled);
    void setChannelWidgetsEnabled(bool enabled);
    void setAuthenticationWidgetsEnabled(bool enabled);
    void setWriteToWidgetsEnabled(bool enabled);

    void setupClients(const QStringList &clients);

private:
    Ui::xToolsSocketToolUi *ui;
};
