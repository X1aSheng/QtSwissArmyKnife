/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded in "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source code
 * directory.
 **************************************************************************************************/
#pragma once

#include <QActionGroup>
#include <QApplication>
#include <QMainWindow>
#include <QMenu>
#include <QStyleFactory>

namespace xTools {

class Application;
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = Q_NULLPTR);
    static QIcon cookedIcon(const QString& svgFileName);

    virtual void updateWindowTitle();

protected:
    Application* m_xToolsApp;

    QMenu* m_fileMenu;
    QMenu* m_optionMenu;
    QMenu* m_languageMenu;
    QMenu* m_helpMenu;

    QAction* m_exitAction;

    QAction* m_gitHubAction;
    QAction* m_giteeAction;
    QAction* m_qqGroupAction;
    QAction* m_aboutAction;
    QAction* m_aboutQtAction;

protected:
    static QString qtConfFileName();

private:
    QActionGroup* m_appStyleActionGroup;
    QActionGroup* m_languageActionGroup;
    QActionGroup* m_appPaletteActionGroup;

private:
    void initMenuFile();
    void initMenuOption();
    void initMenuLanguage();
    void initMenuHelp();

    void initOptionMenuAppStyleMenu();
    void initOptionMenuAppPaletteMenu();
    void initOptionMenuSettingsMenu();
    void initOptionMenuHdpiPolicy();
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
    void initOptionMenuColorScheme();
#endif

    void onHdpiPolicyActionTriggered(int policy);
    void onAboutActionTriggered();
    void onImportActionTriggered();
    void onExportActionTriggered();

    bool tryToReboot();
    void createQtConf();
    void showQqQrCode();
    void setPalette(const QString& fileName);
};

} // namespace xTools
