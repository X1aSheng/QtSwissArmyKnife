﻿/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded in "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source code
 * directory.
 **************************************************************************************************/
#include "../xTools.h"

#include <QAction>
#include <QActionGroup>
#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QDesktopServices>
#include <QFile>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QJsonDocument>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPainter>
#include <QProcess>
#include <QScreen>
#include <QStyle>
#include <QStyleFactory>
#include <QStyleHints>
#include <QSvgRenderer>
#include <QTimer>
#include <QTranslator>
#include <QUrl>

#include "xTools_p.h"

namespace xTools {

xTools::xTools(QObject *parent)
    : QObject(*new xToolsPrivate, parent)
{

}

xTools &xTools::signleton()
{
    Q_ASSERT_X(qApp, "xTools", "The xTools object must be created after application object.");

    static xTools instance;
    return instance;
}

bool xTools::enableSplashScreen()
{
    Q_D(xTools);
    return d->m_enableSplashScreen;
}

void xTools::setEnableSplashScreen(bool enable)
{
    Q_D(xTools);
    d->m_enableSplashScreen = enable;
}

void xTools::setSplashScreenMessage(const QString &msg)
{
    Q_D(xTools);
    d->m_splashScreen.showMessage(msg);
}

QSplashScreen *xTools::splashScreen()
{
    Q_D(xTools);
    return &d->m_splashScreen;
}

QString xTools::appFriendlyName()
{
    Q_D(xTools);
    return d->m_appFriendlyName;
}

void xTools::setAppFriendlyName(const QString &name)
{
    Q_D(xTools);
    d->m_appFriendlyName = name;
}

QString xTools::appVersion()
{
    return QApplication::applicationVersion();
}

QString xTools::defaultAppLanguage()
{
    Q_D(xTools);
    QSettings *settings = d->m_settings;
    QString language = settings->value("language").toString();
    if (language.isEmpty()) {
        language = QLocale::system().name();
    }

    if (!d->m_languageFlagNameMap.contains(language)) {
        qWarning() << "The language name is not supported:" << language << ". \"en\" will be used.";
        language = "en";
    }

    return d->m_languageFlagNameMap.value(language);
}

QStringList xTools::supportedAppLanguages()
{
    Q_D(xTools);
    return d->m_languageFlagNameMap.values();
}

QStringList xTools::supportedAppLanguagePrefixes()
{
    Q_D(xTools);
    // Such as "xToolsCore", "xApp"
    return d->m_appSupportedLanguagePrefixes;
}

void xTools::setSupportedAppLanguagePrefixes(const QStringList &prefixes)
{
    Q_D(xTools);
    d->m_appSupportedLanguagePrefixes = prefixes;
}

void xTools::setupAppLanguageWithPrefix(const QString &language, const QString &prefix)
{
    Q_D(xTools);
    QString key = d->m_languageFlagNameMap.key(language);
    if (language.isEmpty()) {
        key = QLocale::system().name();
        qWarning() << "The language is not specified, system language will be used:" << key;
    }

    if (!d->m_languageFlagNameMap.contains(key)) {
        auto info = QString("The language(%1) isn't supported. English 'll be used.").arg(language);
        qWarning() << qPrintable(info);
        key = "en";
    }

    static QMap<QString, QTranslator *> fileTranslatorMap;
    QString fileName = QString(":/resources/Translations/%1_%2.qm").arg(prefix, key);
    if (!fileTranslatorMap.contains(prefix)) {
        QTranslator *translator = new QTranslator();
        fileTranslatorMap.insert(prefix, translator);
    } else {
        bool ret = QCoreApplication::removeTranslator(fileTranslatorMap.value(prefix));
        const QString status = ret ? "success" : "failed";
        auto info = QString("The translator(%1) removing: %2.").arg(prefix, status);
        qInfo() << qPrintable(info);
    }

    QTranslator *translator = fileTranslatorMap.value(prefix);
    if (!translator->load(fileName)) {
        auto info = QString("The file(%1) can not be load to translator.").arg(fileName);
        qWarning() << qPrintable(info);
        return;
    }

    if (QCoreApplication::installTranslator(translator)) {
        const QString name = d->m_languageFlagNameMap.value(key);
        auto info = QString("The language(%1) has been setup, current language is:").arg(fileName);
        qInfo() << qPrintable(info) << name;
        emit languageChanged();
    } else {
        translator->deleteLater();
        translator = Q_NULLPTR;
        auto info = QString("The translator(%1) install failed(qm file is empty?).").arg(fileName);
        qInfo() << qPrintable(info);
        return;
    }
}

void xTools::setupAppLanguage(const QString &language)
{
    Q_D(xTools);
    QString tmp = language;
    if (tmp.isEmpty()) {
        tmp = d->m_settings->value("language").toString();
        if (tmp.isEmpty()) {
            tmp = defaultAppLanguage();
        }
    }

    QStringList prefixes = supportedAppLanguagePrefixes();
    for (const auto &prefix : prefixes) {
        setupAppLanguageWithPrefix(tmp, prefix);
    }
}

QString xTools::xToolsVersion()
{
    QString version = "0.0.0";
#if defined(X_TOOLS_GIT_TAG)
    auto tag = QString(X_TOOLS_GIT_TAG);
    if (tag == QString("continuous")) {
        version = "9.9.9";
    } else {
        tag = tag.remove("v");
        if (tag.split('.').length() == 3) {
            version = tag;
        }
    }
#endif
    return version;
}

QString xTools::xToolsLastCommit()
{
#if defined(X_TOOLS_GIT_COMMIT)
    return X_TOOLS_GIT_COMMIT;
#else
    return "Unknown";
#endif
}

QString xTools::xToolsLastCommitTime()
{
#if defined(X_TOOLS_GIT_COMMIT_TIME)
    return X_TOOLS_GIT_COMMIT_TIME;
#else
    return "Unknown";
#endif
}

QVariantList xTools::supportedHdpiPolicies()
{
    QMetaEnum metaEnum = QMetaEnum::fromType<Qt::HighDpiScaleFactorRoundingPolicy>();
    QVariantList list;
    for (int i = 0; i < metaEnum.keyCount(); i++) {
        list.append(metaEnum.value(i));
    }
    return list;
}

QString xTools::hdpiPolicyName(int policy)
{
    typedef Qt::HighDpiScaleFactorRoundingPolicy Policy;
    static QMap<Policy, QString> policyMap;
    if (policyMap.isEmpty()) {
        policyMap.insert(Policy::Unset, tr("System"));
        policyMap.insert(Policy::Round, tr("Round up for .5 and above"));
        policyMap.insert(Policy::Ceil, tr("Always round up"));
        policyMap.insert(Policy::Floor, tr("Always round down"));
        policyMap.insert(Policy::RoundPreferFloor, tr("Round up for .75 and above"));
        policyMap.insert(Policy::PassThrough, tr("Don't round"));
    }

    return policyMap.value(static_cast<Policy>(policy), "Unknown");
}

bool xTools::isValidHdpiPolicy(int policy)
{
    auto policies = supportedHdpiPolicies();
    return policies.contains(QVariant(policy));
}

QString xTools::stringToHexString(const QString &str)
{
    return QString::fromLatin1(str.toUtf8().toHex());
}

QString xTools::hexStringToString(const QString &str)
{
    QByteArray arr = QByteArray::fromHex(str.toUtf8());
    return QString::fromUtf8(arr);
}

QByteArray xTools::byteArray2Hex(const QByteArray &source, char separator)
{
    if (source.isEmpty()) {
        return source;
    }

    auto toHex = [](quint8 value) -> char { return "0123456789abcdef"[value & 0xF]; };
    const int length = separator ? (source.size() * 3 - 1) : (source.size() * 2);
    QByteArray hex(length, Qt::Uninitialized);
    char *hexData = hex.data();
    const uchar *data = reinterpret_cast<const uchar *>(source.constData());
    for (int i = 0, o = 0; i < source.size(); ++i) {
        hexData[o++] = toHex(data[i] >> 4);
        hexData[o++] = toHex(data[i] & 0xf);

        if ((separator) && (o < length))
            hexData[o++] = separator;
    }
    return hex;
}

QString xTools::dateTimeString(const QString &format)
{
    return QDateTime::currentDateTime().toString(format);
}

QDateTime xTools::buildDateTime()
{
    QString dateString = QString(__DATE__);
    QString timeString = QString(__TIME__);
    dateString = dateString.replace(QString("  "), " 0");
    QString dateTimeString = dateString + " " + timeString;
    QDateTime dateTime = QLocale(QLocale::English).toDateTime(dateTimeString, "MMM dd yyyy hh:mm:ss");
    return dateTime;
}

QString xTools::buildDateTimeString(const QString &format)
{
    return buildDateTime().toString(format);
}

QString xTools::systemDateFormat()
{
    return QLocale::system().dateFormat();
}

QString xTools::systemTimeFormat()
{
    return QLocale::system().timeFormat();
}

QString xTools::desktopPath()
{
    return QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
}

QString xTools::clipboardText()
{
    return QGuiApplication::clipboard()->text();
}

void xTools::setClipboardText(const QString &text)
{
    QGuiApplication::clipboard()->setText(text);
}

void xTools::openUrl(const QString &url)
{
    QDesktopServices::openUrl(QUrl(url));
}

QIcon xTools::toThemeIcon(const QIcon &icon)
{
    const QString color = qApp->palette().windowText().color().name();
    return cookedIcon(icon, color);
}

QIcon xTools::cookedIconFile(const QString &iconFile, const QString &color)
{
    QIcon icon(iconFile);
    return cookedIcon(icon, color);
}

QIcon xTools::cookedIcon(const QIcon &icon, const QString &color)
{
    QPixmap pixmap = icon.pixmap(QSize(128, 128));
    QPainter painter(&pixmap);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(pixmap.rect(), QColor(color));
    QIcon colorIcon = QIcon(pixmap);
    return colorIcon;
}

void xTools::settingsOpenSettingsFileDir()
{
    QDesktopServices::openUrl(settingsPath());
}

QString xTools::settingsPath()
{
    Q_D(xTools);
    QString settingsFile(d->settingsPath());
    QString path = settingsFile.left(settingsFile.lastIndexOf("/"));
    return path;
}

int xTools::settingsHdpiPolicy()
{
    Q_D(xTools);
    auto var = d->m_settings->value("hdpiPolicy");
    if (var.isValid()) {
        return var.toInt();
    }

    return int(QGuiApplication::highDpiScaleFactorRoundingPolicy());
}

void xTools::settingsSetHdpiPolicy(int policy)
{
    Q_D(xTools);
    d->m_settings->setValue("hdpiPolicy", policy);
}

QString xTools::settingsAppStyle()
{
    Q_D(xTools);
    auto var = d->m_settings->value("appStyle");
    if (var.isValid()) {
        var.toString();
    }

    if (QApplication::instance()) {
        return QApplication::style()->name();
    }

    return QString("");
}

void xTools::settingsSetAppStyle(const QString &style)
{
    Q_D(xTools);
    d->m_settings->setValue("appStyle", style);
}

QString xTools::settingsLanguage()
{
    Q_D(xTools);
    auto var = d->m_settings->value("language");
    if (var.isValid()) {
        return var.toString();
    }

    return QString("");
}

void xTools::settingsSetLanguage(const QString &lan)
{
    Q_D(xTools);
    d->m_settings->setValue("language", lan);
}

bool xTools::settingsClearSettings()
{
    Q_D(xTools);
    return d->m_settings->value("clearSettings").toBool();
}

void xTools::settingsSetClearSettings(bool clear)
{
    Q_D(xTools);
    d->m_settings->setValue("clearSettings", clear);
}

int xTools::settingsColorScheme()
{
    Q_D(xTools);
    auto var = d->m_settings->value("colorScheme");
    if (var.isValid()) {
        return var.toInt();
    }

    return 0;
}

void xTools::settingsSetColorScheme(const int colorScheme)
{
    Q_D(xTools);
    d->m_settings->setValue("colorScheme", colorScheme);
}

QVariant xTools::settingsValue(const QString &key, const QVariant &value) const
{
    Q_D(const xTools);
    return d->m_settings->value(key, value);
}

void xTools::settingsSetValue(const QString &key, const QVariant &value)
{
    Q_D(xTools);
    d->m_settings->setValue(key, value);
}

void xTools::settingsSetJsonObjectStringValue(const QString &key, const QString &value)
{
    Q_D(xTools);
    QJsonDocument doc = QJsonDocument::fromJson(value.toUtf8());
    if (doc.isNull()) {
        return;
    }

    d->m_settings->setValue(key, doc.toVariant());
}

QMainWindow *xTools::mainWindow()
{
    for (const auto &widget : qApp->topLevelWidgets()) {
        auto mainWindow = qobject_cast<QMainWindow *>(widget);
        if (mainWindow) {
            return mainWindow;
        }
    }

    return nullptr;
}

void xTools::moveToScreenCenter(QWidget *widget)
{
    QRect screenRect = QApplication::primaryScreen()->geometry();
    bool tooWidth = (widget->width() > screenRect.width());
    bool tooHeight = (widget->height() > screenRect.height());
    if (tooWidth || tooHeight) {
        widget->showMaximized();
        qInfo() << "The screen is too small.";
    } else {
        widget->move((screenRect.width() - widget->width()) / 2,
                     (screenRect.height() - widget->height()) / 2);
    }
}

void xTools::tryToReboot()
{
    int ret = QMessageBox::information(
        nullptr,
        QObject::tr("Neet to Reboot"),
        QObject::tr("The operation need to reboot to effectived, reboot the applicaion now?"),
        QMessageBox::Ok | QMessageBox::Cancel);
    if (ret == QMessageBox::Ok) {
        QProcess::startDetached(QApplication::applicationFilePath(), QStringList());
        qApp->closeAllWindows();
    }
}

void xTools::tryToClearSettings()
{
    Q_D(xTools);
    if (!d->m_settings->value("clearSettings").toBool()) {
        return;
    }

    d->m_settings->setValue("value", false);
    QString path = d->settingsPath();
    QDir dir(path);
    if (dir.exists()) {
        if (dir.removeRecursively()) {
            qInfo() << "The settings path is removed.";
        } else {
            qWarning() << "The operation(remove settings path) failed!";
        }
    }
}

} // namespace xTools
