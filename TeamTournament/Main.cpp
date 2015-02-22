﻿#include "MainWindowTeam.h"
#include "../core/TournamentMode.h"
#include "../Widgets/Countdown.h"
#include "../Widgets/SplashScreen.h"
#include "../base/versioninfo.h"
#include "../util/path_helpers.h"

#include <QtGui/QApplication>
#include <QTranslator>
#include <QMessageBox>
#include <QSettings>
#include <QFile>
#include <QLocale>
#include <QtextCodec>


void LangNotFound(const QString& fileName)
{
	QMessageBox::critical(nullptr,
						  QCoreApplication::applicationName(),
						  "Unable to read language file: " + fileName +
						  "\nThe default language is being used.");
}

void SetTranslation(QApplication& app, QTranslator& translator, QTranslator& coreTranslator, QString const& langStr)
{
    if (langStr == QString("en"))
    {
        return; // default
    }

    if (langStr == QString("de") || langStr == QString("nl"))
    {
        const QString& langPath =
            QCoreApplication::applicationDirPath() + QString("/lang");

        if (translator.load("Ipponboard_team_" + langStr, langPath))
        {
            app.installTranslator(&translator);
        }
        else
        {
            LangNotFound("Ipponboard_team_" + langStr);
        }

        if (coreTranslator.load("core_" + langStr, langPath))
        {
            app.installTranslator(&coreTranslator);
        }
        else
        {
            LangNotFound("core_" + langStr);
        }
    }
}

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

	QCoreApplication::setApplicationVersion(VersionInfo::VersionStr);
	QCoreApplication::setOrganizationName("Florian Mücke");
	QCoreApplication::setOrganizationDomain("ipponboard.info");
	QCoreApplication::setApplicationName("Ipponboard (Team Edition)");

	MainWindowTeam mainWnd;
	mainWnd.setWindowTitle(
		QCoreApplication::applicationName() + " v" +
		QCoreApplication::applicationVersion());

	// read language code
	QString langStr = QLocale::system().name();
	langStr.truncate(langStr.lastIndexOf('_'));

	const QString ini(
		QString::fromStdString(
			fm::GetSettingsFilePath(
				mainWnd.GetConfigFileName().toAscii())));

	QSettings settings(ini, QSettings::IniFormat, &a);
	settings.beginGroup(str_tag_Main);

	if (settings.contains(str_tag_Language))
    {
		langStr = settings.value(str_tag_Language).toString();
    }

	settings.endGroup();

    QTranslator translator;  // Note: this object needs to remain in scope.
    QTranslator coreTranslator; // Note: this object needs to remain in scope

    SetTranslation(a, translator, coreTranslator, langStr);

	//
	// load tournament modes
	//
	QString errMsg;
    Ipponboard::TournamentMode::List modes;

    if (!Ipponboard::TournamentMode::ReadModes(MainWindowTeam::ModeConfigurationFileName(), modes, errMsg))
	{
		QMessageBox::critical(0,
                              QCoreApplication::tr("Error reading mode configurations"),
							  errMsg);

		return 0;
	}

	mainWnd.SetModes(modes);


    auto eulaText1 = QCoreApplication::tr("Dies ist keine freie Version. Kopieren und Weitergeben ist untersagt.");
    auto eulaText2 = QCoreApplication::tr("Bitte unterstützen Sie die Weitereintwicklung und zukünftige Pflege von Ipponboard durch eine kleine Spende.");
    auto eulaText3 = QCoreApplication::tr("Spenden können Sie einfach über den Link auf der Homepage oder Sie kontaktieren uns einfach.");
    auto eulaText4 = QCoreApplication::tr("Falls Sie Verbesserungsvorschläge bzgl. der Anzeige (Darstellung, Bedienung) oder der Steuerung haben, oder aber finden, dass es noch irgendwo hakt, dann teilen Sie uns dies bitte mit - wir freuen uns, wenn wir unsere Software noch besser machen können. Hierzu gibt es einen Online-Fragebogen, erreichbar über <em>Menü&rarr;Über&rarr;Feedback</em> (dauert wirklich nicht lange).");
    auto eulaText5 = QCoreApplication::tr("Die aktuellste Version finden Sie auf der Homepage:");
    auto eulaText6 = QCoreApplication::tr("Vielen Dank!");
    auto edition = "Team Edition";

    const QString text = QString(
                "<html><body><h2><span style=\"color:#336699\">Ipponboard</span> %7 <small>v%8</small></h2>"\
                "<!--<p><strong>%1</strong></p>-->"\
                "<p><span style=\"color:blue\"><em>%2</em></span></p>"\
                "<p>%3</p>"\
                "<p>%4</p>"\
                "<p>%5 <a href=\"http://www.ipponboard.info\">www.ipponboard.info</a></p>"\
                "<p><br/><em>%6</em></p>"\
                "</body></html>").arg(eulaText1, eulaText2, eulaText3, eulaText4, eulaText5, eulaText6, edition, VersionInfo::VersionStrShort);

	SplashScreen::Data splashData;
	splashData.text = text;
	splashData.info = QCoreApplication::applicationName()
					  + " v" + QCoreApplication::applicationVersion()
					  + "\n"
					  + "Build: " + VersionInfo::Date;
	SplashScreen splash(splashData);
	splash.SetImageStyleSheet("image: url(:/res/images/logo_team.png);");
	splash.resize(480, 410);

	if (QDialog::Accepted != splash.exec())
    {
		return 0;
    }

	mainWnd.Init();
	mainWnd.show();

	return a.exec();
}
