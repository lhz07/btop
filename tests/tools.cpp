// SPDX-License-Identifier: Apache-2.0

#include <vector>

#include <gtest/gtest.h>

#include "btop_tools.hpp"

TEST(tools, string_split) {
	EXPECT_EQ(Tools::ssplit(""), std::vector<std::string> {});
	EXPECT_EQ(Tools::ssplit("foo"), std::vector<std::string> { "foo" });
	{
		auto actual = Tools::ssplit("foo       bar         baz    ");
		auto expected = std::vector<std::string> { "foo", "bar", "baz" };
		EXPECT_EQ(actual, expected);
	}

	{
		auto actual = Tools::ssplit("foobo  oho  barbo  bo  bazbo", 'o');
		auto expected = std::vector<std::string> { "f", "b", "  ", "h", "  barb", "  b", "  bazb" };
		EXPECT_EQ(actual, expected);
	}
}

TEST(term, ansi_optimizer_removes_redundant_same_position_chunk) {
	const std::string esc = "\x1b[";
	const std::string input = esc + "5;10fABC" + esc + "5;10fXYZ";
	const std::string expected = esc + "5;10fXYZ";
	EXPECT_EQ(Term::ANSIOptimizer::optimize(input), expected);
}

TEST(term, ansi_optimizer_keeps_shorter_overwrite_without_clear) {
	const std::string esc = "\x1b[";
	const std::string input = esc + "2;2fABCDE" + esc + "2;2fAB";
	EXPECT_EQ(Term::ANSIOptimizer::optimize(input), input);
}

TEST(term, ansi_optimizer_collapses_clear_then_redraw) {
	const std::string esc = "\x1b[";
	const std::string input = esc + "3;4fABCDE" + esc + "3;4f" + esc + "2K" + esc + "3;4fNEW";
	const std::string expected = esc + "3;4fNEW";
	EXPECT_EQ(Term::ANSIOptimizer::optimize(input), expected);
}
