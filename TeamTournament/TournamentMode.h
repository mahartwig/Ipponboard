﻿// Copyright 2010-2013 Florian Muecke. All rights reserved.
// http://www.ipponboard.info (ipponboardinfo at googlemail dot com)
//
// THIS FILE IS PART OF THE IPPONBOARD PROJECT.
// IT MAY NOT BE DISTRIBUTED TO OR SHARED WITH THE PUBLIC IN ANY FORM!
//

#ifndef TOURNAMENTMODE_H
#define TOURNAMENTMODE_H

#include <QString>
#include <vector>

class QSettings;
class TestTournamentMode;

class TournamentMode
{
    friend class TestTournamentMode;

public:
	TournamentMode();

	// ascii strings
	static QString const& str_Title;
	static QString const& str_SubTitle;
	static QString const& str_Weights;
	static QString const& str_Template;
	static QString const& str_FightTimeOverrides;
	static QString const& str_Rounds;
	static QString const& str_FightTimeInSeconds;
	static QString const& str_WeightsAreDoubled;

	static bool ReadModes(
		const QString& filename,
		std::vector<TournamentMode>& modes,
		QString& errorMsg);

	bool operator< (TournamentMode const& other) const;

	QString FullTitle() const;
    int FightsPerRound() const;

private:
    static bool parse_current_group(
        QSettings const& config,
        TournamentMode& tm,
        QString& errorMsg);

public: // nothing to encapsulate here
	QString name;
	QString title;
	QString subTitle;
	QString weights;
	QString listTemplate;
	QString fightTimeOverrides;
	int nRounds;
	int fightTimeInSeconds;
	bool weightsAreDoubled;
};

#endif // TOURNAMENTMODE_H
