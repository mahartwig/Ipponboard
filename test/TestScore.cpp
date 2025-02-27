// Copyright 2018 Florian Muecke. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE.txt file.

#include "../util/catch2/catch.hpp"
#include "../core/Score.h"
#include "../core/Enums.h"
#include "../core/Rules.h"
#include "../core/Fight.h"

#include <iostream>

using namespace Ipponboard;
using Point = Score::Point;

bool IsScoreLess(std::shared_ptr<Ipponboard::AbstractRules> pRules, Score const& lhs, Score const& rhs)
{
	Fight f { lhs, rhs };
	return pRules->CompareScore(f) > 0;
}

TEST_CASE("[Score] Shido rules for fights")
{
	auto empty = Score();
	auto shido = Score().Add(Point::Shido);
	auto twoShido = Score().Add(Point::Shido).Add(Point::Shido);
	auto yukoWithShido = Score().Add(Point::Yuko).Add(Point::Shido);
	auto yukoWithTwoShido = Score().Add(Point::Yuko).Add(Point::Shido).Add(Point::Shido);
	auto wazaari = Score().Add(Point::Wazaari);
	auto wazaariWithShido = Score().Add(Point::Wazaari).Add(Point::Shido);
	auto wazaariWithTwoShido = Score().Add(Point::Wazaari).Add(Point::Shido).Add(Point::Shido);
	auto wazaariWithThreeShido = Score().Add(Point::Wazaari).Add(Point::Shido).Add(Point::Shido).Add(Point::Shido);

	auto rules2013 = std::make_shared<Ipponboard::Rules2013>();
	REQUIRE_FALSE(IsScoreLess(rules2013, shido, shido));
	REQUIRE(IsScoreLess(rules2013, shido, empty));
	REQUIRE(IsScoreLess(rules2013, twoShido, shido));
	REQUIRE(IsScoreLess(rules2013, shido, yukoWithTwoShido));
	REQUIRE(IsScoreLess(rules2013, twoShido, yukoWithTwoShido));
	REQUIRE(IsScoreLess(rules2013, empty, yukoWithTwoShido));
	REQUIRE(IsScoreLess(rules2013, wazaariWithShido, wazaari));
	REQUIRE(IsScoreLess(rules2013, wazaariWithTwoShido, wazaari));
	REQUIRE(IsScoreLess(rules2013, wazaariWithTwoShido, wazaariWithShido));
	REQUIRE(IsScoreLess(rules2013, wazaariWithThreeShido, wazaariWithShido));
	REQUIRE(IsScoreLess(rules2013, wazaariWithThreeShido, wazaariWithTwoShido));

	// classic rules
	auto classicRules = std::make_shared<Ipponboard::ClassicRules>();
	REQUIRE_FALSE(IsScoreLess(classicRules, shido, empty));
	REQUIRE_FALSE(IsScoreLess(classicRules, shido, shido));
	REQUIRE(IsScoreLess(classicRules, twoShido, yukoWithShido));
	REQUIRE(IsScoreLess(classicRules, shido, yukoWithTwoShido));
	REQUIRE(IsScoreLess(classicRules, twoShido, yukoWithTwoShido));
	REQUIRE(IsScoreLess(classicRules, empty, yukoWithTwoShido));
}

//TEST_CASE("4th shido sets hansokumake")
//{
//	Score one;
//	Score two;
//
//	one.Add(Point::Shido);
//	one.Add(Point::Shido);
//	one.Add(Point::Shido);
//	one.Add(Point::Shido);
//	two.Add(Hansokumake);
//
//	one.Hansokumake
//	REQUIRE_FALSE(one.IsLess(two));
//	REQUIRE_FALSE(two.IsLess(one));
//}

TEST_CASE("[Score] Each fighter can have Hansokumake")
{
	Score score1;
	Score score2;
	auto classicRules = std::make_shared<Ipponboard::ClassicRules>();
	auto rules2013 = std::make_shared<Ipponboard::Rules2013>();

	score1.Add(Score::Point::Hansokumake);

	REQUIRE(IsScoreLess(classicRules, score1, score2));
	REQUIRE_FALSE(IsScoreLess(classicRules, score2, score1));

	REQUIRE(IsScoreLess(rules2013, score1, score2));
	REQUIRE_FALSE(IsScoreLess(rules2013, score2, score1));

	score2.Add(Score::Point::Hansokumake);

	REQUIRE_FALSE(IsScoreLess(classicRules, score1, score2));
	REQUIRE_FALSE(IsScoreLess(classicRules, score2, score1));

	REQUIRE_FALSE(IsScoreLess(rules2013, score1, score2));
	REQUIRE_FALSE(IsScoreLess(rules2013, score2, score1));
}

TEST_CASE("[Score] is awasette ippon")
{
	Score score;
	auto classicRules = std::make_shared<Ipponboard::ClassicRules>();
	auto rules2013 = std::make_shared<Ipponboard::Rules2013>();
	auto rules2017 = std::make_shared<Ipponboard::Rules2017>();
	auto rules2018 = std::make_shared<Ipponboard::Rules2018>();
	auto rules2025 = std::make_shared<Ipponboard::Rules2025>();

	REQUIRE_FALSE(classicRules->IsAwaseteIppon(score));
	REQUIRE_FALSE(rules2013->IsAwaseteIppon(score));
	REQUIRE_FALSE(rules2017->IsAwaseteIppon(score));
	REQUIRE_FALSE(rules2018->IsAwaseteIppon(score));
	REQUIRE_FALSE(rules2025->IsAwaseteIppon(score));

	score.Add(Score::Point::Wazaari);

	REQUIRE_FALSE(classicRules->IsAwaseteIppon(score));
	REQUIRE_FALSE(rules2013->IsAwaseteIppon(score));
	REQUIRE_FALSE(rules2017->IsAwaseteIppon(score));
	REQUIRE_FALSE(rules2018->IsAwaseteIppon(score));
	REQUIRE_FALSE(rules2025->IsAwaseteIppon(score));

	score.Add(Score::Point::Wazaari);

	REQUIRE(classicRules->IsAwaseteIppon(score));
	REQUIRE(rules2013->IsAwaseteIppon(score));
	REQUIRE_FALSE(rules2017->IsAwaseteIppon(score));
	REQUIRE(rules2018->IsAwaseteIppon(score));
	REQUIRE(rules2025->IsAwaseteIppon(score));

	//TODO
	//FIXME
	/* not correct, yet
	score.Add(Score::Point::Wazaari);

	std::cout << "ippon: " << score.Ippon() << ", ";
	std::cout << "wazaari: " << score.Wazaari();
	REQUIRE(classicRules->IsAwaseteIppon(score));
	REQUIRE(rules2013->IsAwaseteIppon(score));
	REQUIRE_FALSE(rules2017->IsAwaseteIppon(score));
	REQUIRE(rules2018->IsAwaseteIppon(score));
	*/
}

TEST_CASE("[Score] rules 2017: only first shido does not count")
{
	auto rules = std::make_shared<Ipponboard::Rules2017>();

	Score s1;
	REQUIRE_FALSE(IsScoreLess(rules, s1, Score()));

	s1.Add(Point::Shido);
	REQUIRE_FALSE(IsScoreLess(rules, s1, Score()));

	s1.Add(Point::Shido);
	REQUIRE_FALSE(IsScoreLess(rules, s1, Score()));  // as yuko has to be added manually
}

TEST_CASE("[Score] rules 2017: first shido does count in golden score")
{
	auto rules = std::make_shared<Ipponboard::Rules2017>();

	Fight f;
	f.SetGoldenScore(true);

	REQUIRE(rules->CompareScore(f) == 0);

	f.GetScore(FighterEnum::First).Add(Point::Shido);
	REQUIRE(rules->CompareScore(f) > 0);

	f.GetScore(FighterEnum::First).Add(Point::Shido);
	REQUIRE(rules->CompareScore(f) > 0);
}

TEST_CASE("[Score] rules 2025: yuko is always less than wazaari")
{
	auto rules = std::make_shared<Ipponboard::Rules2025>();

	Score yukoScore, wazaariScore; 
	yukoScore.Add(Point::Yuko);
	INFO("empty score is less than yuko");
	REQUIRE(IsScoreLess(rules, Score(), yukoScore));

	wazaariScore.Add(Point::Wazaari);
	INFO("yuko is less than wazaari");
	REQUIRE(IsScoreLess(rules, yukoScore, wazaariScore));

	yukoScore.Add(Point::Yuko);
	yukoScore.Add(Point::Yuko);
	INFO("3 yuko is less than wazaari");
	REQUIRE(IsScoreLess(rules, yukoScore, wazaariScore));

	for (int i = 0; i < 97; ++i)
	{
		yukoScore.Add(Point::Yuko);
	}
	INFO("100 yuko is less than wazaari");
	REQUIRE(IsScoreLess(rules, yukoScore, wazaariScore));
}
