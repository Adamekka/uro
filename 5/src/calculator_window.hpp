#pragma once

#include "calculator.hpp"
#include <QWidget>
#include <cstdint>

class QLabel;
class QPushButton;

class CalculatorWindow: public QWidget {
    Q_OBJECT

  public:
    explicit CalculatorWindow(QWidget* parent = nullptr);

  private:
    Calculator m_calc;
    QLabel* m_display = nullptr;
    QPushButton* m_btn_div = nullptr;
    QPushButton* m_btn_mul = nullptr;
    QPushButton* m_btn_sub = nullptr;
    QPushButton* m_btn_add = nullptr;

    [[nodiscard]] auto make_button(const QString& text, const char* style)
        -> QPushButton*;
    auto update_display() -> void;
    auto highlight_active_operation() -> void;

    auto on_digit_clicked(int32_t digit) -> void;
    auto on_decimal_clicked() -> void;
    auto on_clear_clicked() -> void;
    auto on_negate_clicked() -> void;
    auto on_percent_clicked() -> void;
    auto on_operation_clicked(Operation op) -> void;
    auto on_equals_clicked() -> void;
};
