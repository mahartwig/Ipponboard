﻿// Copyright 2010-2013 Florian Muecke. All rights reserved.
// http://www.ipponboard.info (ipponboardinfo at googlemail dot com)
//
// THIS FILE IS PART OF THE IPPONBOARD PROJECT.
// IT MAY NOT BE DISTRIBUTED TO OR SHARED WITH THE PUBLIC IN ANY FORM!
//

#include "TournamentMode.h"

#include <QString>
#include <QStringList>
#include <QFile>
#include <QSettings>

QString const& TournamentMode::str_Title("Title");
QString const& TournamentMode::str_SubTitle("SubTitle");
QString const& TournamentMode::str_Weights("Weights");
QString const& TournamentMode::str_Template("Template");
QString const& TournamentMode::str_FightTimeOverrides("FightTimeOverrides");
QString const& TournamentMode::str_Rounds("Rounds");
QString const& TournamentMode::str_FightTimeInSeconds("FightTimeInSeconds");
QString const& TournamentMode::str_WeightsAreDoubled("WeightsAreDoubled");

TournamentMode::TournamentMode()
	: name()
	, title()
	, subTitle()
	, weights()
	, listTemplate()
	, fightTimeOverrides()
	, nRounds(0)
	, fightTimeInSeconds(0)
	, weightsAreDoubled(false)
{
}


bool TournamentMode::ReadModes(
	const QString& filename,
	std::vector<TournamentMode>& modes,
	QString& errorMsg)
{
	errorMsg.clear();

	QFile file(filename);
	if (!file.exists())
	{
		errorMsg = QString("%1 not found!").arg(filename);
		return false;
	}

	QSettings config(filename, QSettings::IniFormat, nullptr);
	QStringList groups = config.childGroups();

    if (groups.isEmpty())
    {
        errorMsg = QString("%1 does not contain any mode definitions!").arg(filename);
        return false;
    }

	std::vector<TournamentMode> _modes;
	Q_FOREACH(QString const & group, groups)
	{
        TournamentMode tm;

        config.beginGroup(group);
        bool readSuccess = parse_current_group(config, tm, errorMsg);
        config.endGroup();

        if (!readSuccess)
        {
            errorMsg = QString("Error parsing file %1. %2").arg(errorMsg);
            return false;
        }

		_modes.push_back(tm);
	}

	std::sort(begin(modes), end(modes));

	// all Ok, swap to internal
	modes.swap(_modes);
	return true;
}

bool TournamentMode::operator<(TournamentMode const& other) const
{
	return FullTitle() < other.FullTitle();
}

QString TournamentMode::FullTitle() const
{
	return subTitle.isEmpty() ?
		   title :
                QString("%1 - %2").arg(title, subTitle);
}

int TournamentMode::FightsPerRound() const
{
    const auto nWeights = weights.split(';').count();
    return weightsAreDoubled ? nWeights * 2 : nWeights;
}

bool TournamentMode::parse_current_group(
        QSettings const& config,
        TournamentMode& tm,
        QString& errorMsg)
{
    const QString err = "The key [%1] in section [%2] is empty";

    tm.name = config.group();
    tm.title = config.value(TournamentMode::str_Title).toString();
    tm.subTitle = config.value(TournamentMode::str_SubTitle).toString();
    tm.weights = config.value(TournamentMode::str_Weights).toString();
    tm.listTemplate = config.value(TournamentMode::str_Template).toString();
    tm.nRounds = config.value(TournamentMode::str_Rounds).toUInt();
    tm.fightTimeInSeconds = config.value(TournamentMode::str_FightTimeInSeconds).toUInt();
    tm.weightsAreDoubled = config.value(TournamentMode::str_WeightsAreDoubled, false).toBool();
    tm.fightTimeOverrides = config.value(TournamentMode::str_FightTimeOverrides).toString();

    if (tm.weights.isEmpty())
    {
        errorMsg = err.arg(TournamentMode::str_Weights, config.group());
        return false;
    }

    if (tm.listTemplate.isEmpty())
    {
        errorMsg = err.arg(TournamentMode::str_Template, config.group());
        return false;
    }
    else
    {
        QFile listTemplate(tm.listTemplate);

        if (!listTemplate.exists())
        {
            errorMsg = QString("The list template for [%2] is not valid: \"%1\"")
                       .arg(tm.listTemplate, config.group());

            return false;
        }
    }

    if (tm.nRounds == 0)
    {
        errorMsg = err.arg(TournamentMode::str_Rounds, config.group());
        return false;
    }

    if (tm.fightTimeInSeconds == 0)
    {
        errorMsg = err.arg(TournamentMode::str_FightTimeInSeconds, config.group());
        return false;
    }

    if (tm.title.isEmpty())
    {
        errorMsg = err.arg(TournamentMode::str_Title, config.group());
        return false;
    }

    if (!tm.fightTimeOverrides.isEmpty())
    {
        // TODO parse and validate...
    }

    return true;
}
