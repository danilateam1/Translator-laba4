#include <gtest.h>
#include "translator.h"
TEST(Translator, can_get_delete_spaces) {
    Translator expression("1   + 2 - 3    - 4");
    EXPECT_EQ("1+2-3-4", expression.getInfix());
}
TEST(Translator, can_parse_digits_expression) {
    string str = "   2/4 -4.0 + 0.4 * 0.4 / 3 - 4      - 7";
    ASSERT_NO_THROW(Translator expression(str));
}

TEST(Translator, can_parse_brackets_expression) {
    string str = " (  ((2)/4) -(4.0*4.1 + 0.4) * (0.4 / 3) - 7)";
    ASSERT_NO_THROW(Translator expression(str));
}

TEST(Translator, throws_when_brackets_are_in_reverse_order) {
    string str = "1-)20-21(";
    ASSERT_ANY_THROW(Translator expression(str));
}

TEST(Translator, throws_when_more_one_operations_together) {
    string str = "5.1*/2";
    ASSERT_ANY_THROW(Translator expression(str));
}

TEST(Translator, throws_when_more_one_point_in_the_number) {
    string str = "21-54+2.1.1";
    ASSERT_ANY_THROW(Translator expression(str));
}

TEST(Translator, throws_when_divided_by_zero) {
    string str = "5.1/0";
    Translator expression(str);
    ASSERT_ANY_THROW(expression.Calculate());
}
TEST(Translator, no_throws_when_zero_divied_by_number) {
    Translator expression("0.0/2.0");
    EXPECT_EQ(0.0, expression.Calculate());
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

TEST(Translator, can_find_unary_minus) {
    Translator expression("-3");
    EXPECT_EQ(-3, expression.Calculate());
}

TEST(Translator, can_multiply_negative_number_by_positive_number) {
    Translator expression("-2*8");
    EXPECT_EQ(expression.Calculate(), -16.0);
}

TEST(Translator, can_multiply_positive_number_by_negative_number) {
    Translator expression("2*(-8)");
    EXPECT_EQ(expression.Calculate(), -16.0);
}

TEST(Translator, can_multiply_negative_number_by_negative_number) {
    Translator expression("-2*(-8)");
    EXPECT_EQ(expression.Calculate(), 16.0);
}

TEST(Translator, can_binary_subtraction_operation) {
    Translator expression(" 32 - 54");
    double result = expression.Calculate();
    double expected = 32.0 - 54.0;
    EXPECT_EQ(expected, result);
}

TEST(Translator, can_binary_addition_operation) {
    Translator expression(" 32 + 54");
    double result = expression.Calculate();
    double expected = 32.0 + 54.0;
    EXPECT_EQ(expected, result);
}

TEST(Translator, can_binary_multiply_operation) {
    Translator expression(" 32 * 54");
    double result = expression.Calculate();
    double expected = 32.0 * 54.0;
    EXPECT_EQ(expected, result);
}

TEST(Translator, can_binary_divided_operation) {
    Translator expression(" 32 / 54");
    double result = expression.Calculate();
    double expected = 32.0/54.0;
    EXPECT_EQ(expected, result);
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

TEST(Translator, can_understand_where_unary_minus_where_simple_minus) {
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

TEST(Translator, can_calculate_difficult_expression_still) {
    Translator expression("(472*21-58)/(82*(-2.3)+32*(54-32))");
    double result = expression.Calculate();
    double expected = (472 * 21 - 58) / (82 * (-2.3) + 32 * (54 - 32));
    EXPECT_EQ(expected, result);
}

