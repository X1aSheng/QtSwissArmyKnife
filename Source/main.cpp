﻿/***************************************************************************************************
 * Copyright 2018-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "App/xExec.h"
#include "Common/xTools.h"

#include "Application.h"
#include "MainWindow.h"

int main(const int argc, char *argv[])
{
    QString version = xTools::xTools::version();
    return xTools::exec<MainWindow, MainWindow, Application>(argc, argv, QString("xTools"), version);
}
