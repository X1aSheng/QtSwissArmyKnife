/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKUDPTRANSMITTERTOOLUI_HH
#define SAKUDPTRANSMITTERTOOLUI_HH

#include "SAKSocketClientTransmitterToolUi.hh"

class SAKUdpTransmitterToolUi : public SAKSocketClientTransmitterToolUi
{
    Q_OBJECT
public:
    SAKUdpTransmitterToolUi(QWidget *parent = nullptr);
};

#endif // SAKUDPTRANSMITTERTOOLUI_HH
