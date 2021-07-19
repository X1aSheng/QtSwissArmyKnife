﻿/*
 * Copyright 2020-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKDEBUGGERINPUTCONTROLLER_HH
#define SAKDEBUGGERINPUTCONTROLLER_HH

#include <QLabel>
#include <QTimer>
#include <QMutex>
#include <QObject>
#include <QTextEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QSettings>
#include <QPushButton>
#include <QListWidget>
#include <QWaitCondition>

class SAKInputDataFactory;
class SAKCommonCrcInterface;
class SAKInputDataPresetItem;
class SAKInputCrcSettingsDialog;
class SAKInputDataPresetItemManager;

/// @brief input module controller
class SAKDebuggerInput:public QObject
{
    Q_OBJECT
public:
    SAKDebuggerInput(QComboBox *regularlySending,
                     QComboBox *inputFormat,
                     QPushButton *more,
                     QPushButton *send,
                     QLabel *crc,
                     QTextEdit *input,
                     QSettings *settings,
                     const QString &settingsGroup,
                     QObject *parent = Q_NULLPTR);
    ~SAKDebuggerInput();

    struct InputParametersContext {
        bool appendCrc; // true: append crc value to the tail of data frame
        bool bigEndian; // true: crc value is big endian
        int cycleTime; // Auto send data interval
        int inputModel; // Input text format, such as bin, otc, dec, hex and so on
        int crcParametersModel; // The parameter model of crc value
        int startByte; // The start byte of crc section, the first byte is one
        int endByte; // The end byte of crc section, the last byte is one
    };

    /**
     * @brief showCrcSettingsDialog: popup crc setting dialog
     */
    void showCrcSettingsDialog();

    /**
     * @brief formattingInputText: formattting input text According to model
     * @param textEdit: Text source
     * @param model: text model
     */
    static void formattingInputText(QTextEdit *textEdit, int model);
private:
    QMenu *mWriteDataItemMenu;
private:
    QComboBox *mCyclingTimeComboBox;
    QComboBox *mInputModelComboBox;
    QPushButton *mMoreInputSettingsPushButton;
    QPushButton *mSendPushButton;
    QTextEdit *mInputTextEdit;
    QLabel *mCrcLabel;

    QTimer mCyclingWritingTimer;
    SAKInputDataFactory *mInputDataFactory;
    InputParametersContext mInputParameters;
    SAKCommonCrcInterface *mCrcInterface;
    SAKInputDataPresetItemManager *mInputDataItemManager;
    SAKInputCrcSettingsDialog *mCrcSettingsDialog;
    // The unit is ms.
    const int defaultCycle = 1000;

    // Variables about settings
    QSettings *mSettings;
    QString mSettingStringInputTextFromat;
private:
    void changeInputModel(const QString &text);
    void saveInputDataToFile();
    void readinFile();
    void inputTextEditTextChanged();
    void sendRawData();
    void sendOtherRawData(QString data, int textFormat);
    void changeCrcModel();
    void setPresetData();
    void initParameters();
    void cyclingWritingTimerTimeout();
    void updateCrc();
    void appendAction(SAKInputDataPresetItem *item);
    void removeAction(SAKInputDataPresetItem *item);
    void changeDescription(SAKInputDataPresetItem *item);
    void actionTriggered();
    void readinSettings();
signals:
    void rawDataChanged(QString rawData, InputParametersContext parameters);
};
Q_DECLARE_METATYPE(SAKDebuggerInput::InputParametersContext);

#endif
