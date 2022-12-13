#include <gtest.h>
#include "translator.h"
TEST(Translator, can_parse_digits_expression) {
    string str = "   2/4 -4.0 + 0.4 * 0.4 / 3 - 4      - 7";
    ASSERT_NO_THROW(Translator expression(str));
}

TEST(Translator, can_parse_brackets_expression) {
    string str = " (  ((2)/4) -(4.0*4.1 + 0.4) * (0.4 / 3) - 7)";
    ASSERT_NO_THROW(Translator expression(str));
}

TEST(Translator, throws_when_empty_string) {
    string str = "";
    ASSERT_ANY_THROW(Translator expression(str));
}
TEST(Translator, throws_when_empty_brackets) {
    string str = " 7 + 3*6 + () - 4";
    ASSERT_ANY_THROW(Translator expression(str));
}
TEST(Translator, throws_when_unclosed_bracket) {
    string str = "( (7 + (3*6) - 4) + 2";
    ASSERT_ANY_THROW(Translator expression(str));
}
TEST(Translator, throws_when_unopened_bracket) {
    string str = "(7 + (3*6) - 4) + 2 )";
    ASSERT_ANY_THROW(Translator expression(str));
}

TEST(Translator, throws_when_double_operation) {
    string str = "42 + 24 ++ 12";
    ASSERT_ANY_THROW(Translator expression(str));
}
TEST(Translator, throws_when_invalid_symbols) {
    string str = "2 + 3*5^ + 4";
    ASSERT_ANY_THROW(Translator expression(str));
}

TEST(Translator, can_get_infix) {
    Translator expression("(  ((2)/4) -(4.0*4.1 + 0.4) * (0.4 / 3) - 7)");
    EXPECT_EQ("(((2)/4)-(4.0*4.1+0.4)*(0.4/3)-7)", expression.getInfix());
}

TEST(Translator, can_get_postfix_equal_priority) {
    Translator expression("1   + 2 - 3    - 4");
    EXPECT_EQ("12+3-4-", expression.getPostfix());
}
TEST(Translator, can_get_postfix_different_priority) {
    Translator expression("  1+2*3 -  4  - 5*6");
    EXPECT_EQ("123*+4-56*-", expression.getPostfix());
}
TEST(Translator, can_get_postfix_brackets) {
    Translator expression("  1+2*(3 -  4)  - 5*6 + (7+8*(9-10))");
    EXPECT_EQ("1234-*+56*-78910-*++", expression.getPostfix());
}
TEST(Translator, can_calculate_subtraction_digits_only) {
    Translator expression(" 1 - 2 - (3 - 4)");
    double result = expression.Calculate();
    double expected = 1 - 2 - (3 - 4);
    EXPECT_EQ(expected, result);
}
TEST(Translator, can_calculate_multiplication_digits_only) {
    Translator expression(" 1 * 2 * (3 * 4)");
    double result = expression.Calculate();
    double expected = 1 * 2 * (3 * 4);
    EXPECT_EQ(expected, result);
}
TEST(Translator, can_calculate_division_digits_only) {
    Translator expression(" 1.0 / 2.0 / (3.0 / 4.0)");
    double result = expression.Calculate();
    double expected = 1.0 / 2.0 / (3.0 / 4.0);
    EXPECT_EQ(expected, result);
}
TEST(Translator, can_calculate_digits_only) {
    Translator expression("(1.0 / 2.0 - (3.0 / 4.0) * 5.0 + 0.6 - 0.7*0.7) / (0.1 + 0.2)");
    double result = expression.Calculate();
    double expected = (1.0 / 2.0 - (3.0 / 4.0) * 5.0 + 0.6 - 0.7 * 0.7) / (0.1 + 0.2);
    EXPECT_EQ(expected, result);
}

TEST(Translator, can_understand_that_it_is_unary_minus) {
    Translator expression("-45+5*(-2)--1");
    double result = expression.Calculate();
    double expected = -45 + 5 * (-2) - (-1);
    EXPECT_EQ(expected, result);
}
TEST(Translator, can_calculate_difficult_expression) {
    Translator expression("((4.0*4.1 + 0.4) * (0.4 / 3) - 7*(-1.1))");
    double result = expression.Calculate();
    double expected = ((4.0 * 4.1 + 0.4) * (0.4 / 3) - 7*(-1.1));
    EXPECT_EQ(expected, result);
}