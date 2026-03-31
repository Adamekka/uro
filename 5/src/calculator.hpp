#pragma once

#include <QString>
#include <cstdint>

enum class Operation : uint8_t {
    None,
    Add,
    Sub,
    Mul,
    Div,
};

class Calculator {
  public:
    Calculator() = default;

    auto digit(int32_t d) -> void;
    auto decimal() -> void;
    auto negate() -> void;
    auto percent() -> void;
    auto set_operation(Operation op) -> void;
    auto equals() -> void;
    auto clear() -> void;

    [[nodiscard]] auto display_string() const -> QString;
    [[nodiscard]] auto has_active_operation() const -> bool;
    [[nodiscard]] auto active_operation() const -> Operation;

  private:
    double m_stored = 0.0;
    Operation m_pending = Operation::None;
    QString m_display = QStringLiteral("0");
    bool m_fresh = true;
    bool m_has_decimal = false;

    [[nodiscard]] static auto compute(double lhs, Operation op, double rhs)
        -> double;
    auto set_display(double v) -> void;
};
