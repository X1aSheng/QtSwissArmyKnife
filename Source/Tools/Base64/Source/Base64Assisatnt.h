/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QWidget>

namespace Ui {
class Base64Assisatnt;
}

class Base64Assisatnt : public QWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE Base64Assisatnt(QWidget* parent = Q_NULLPTR);
    ~Base64Assisatnt();

private:
    Ui::Base64Assisatnt* ui;

private:
    void onImageClicked();
    void onEncryptClicked();
    void onDecryptClicked();
};
