#include "calculator.hpp"
#include <cmath>
#include <cstdint>
#include <limits>

auto Calculator::digit(const int32_t d) -> void {
    if (m_fresh) {
        m_display = (d == 0) ? QStringLiteral("0") : QString::number(d);
        m_fresh = false;
        m_has_decimal = false;
        return;
    }

    // Suppress leading zeros (handles both "0" and "-0")
    bool is_zero
        = (m_display == QStringLiteral("0")
           || m_display == QStringLiteral("-0"));
    if (is_zero) {
        if (d == 0) {
            return;
        }
        bool neg = m_display.startsWith(QLatin1Char('-'));
        m_display = neg ? (QStringLiteral("-") + QString::number(d))
                        : QString::number(d);
        return;
    }

    // Cap digit count at 9 (excluding '-' and '.')
    int32_t digit_count = 0;
    for (QChar c : m_display) {
        if (c.isDigit()) {
            ++digit_count;
        }
    }
    if (digit_count >= 9) {
        return;
    }

    m_display += QString::number(d);
}

auto Calculator::decimal() -> void {
    if (m_fresh) {
        m_display = QStringLiteral("0.");
        m_fresh = false;
        m_has_decimal = true;
        return;
    }
    if (!m_has_decimal) {
        m_display += QLatin1Char('.');
        m_has_decimal = true;
    }
}

auto Calculator::negate() -> void {
    bool is_special
        = (m_display == QStringLiteral("Error")
           || m_display == QStringLiteral("∞")
           || m_display == QStringLiteral("-∞")
           || m_display == QStringLiteral("0"));
    if (is_special) {
        return;
    }

    if (m_display.startsWith(QLatin1Char('-'))) {
        m_display.remove(0, 1);
    } else {
        m_display.prepend(QLatin1Char('-'));
    }
}

auto Calculator::percent() -> void {
    set_display(m_display.toDouble() / 100.0);
    m_fresh = false;
}

auto Calculator::set_operation(const Operation op) -> void {
    double current = m_display.toDouble();

    // Chain operations if the user entered a second operand
    if (m_pending != Operation::None && !m_fresh) {
        double result = compute(m_stored, m_pending, current);
        m_stored = result;
        set_display(result);
    } else {
        m_stored = current;
    }

    m_pending = op;
    m_fresh = true;
}

auto Calculator::equals() -> void {
    if (m_pending == Operation::None) {
        return;
    }

    double result = compute(m_stored, m_pending, m_display.toDouble());
    m_pending = Operation::None;
    m_fresh = true;
    set_display(result);
}

auto Calculator::clear() -> void {
    m_stored = 0.0;
    m_pending = Operation::None;
    m_display = QStringLiteral("0");
    m_fresh = true;
    m_has_decimal = false;
}

auto Calculator::display_string() const -> QString {
    return m_display;
}

auto Calculator::has_active_operation() const -> bool {
    return m_pending != Operation::None;
}

auto Calculator::active_operation() const -> Operation {
    return m_pending;
}

auto Calculator::compute(const double lhs, const Operation op, const double rhs)
    -> double {
    switch (op) {
        case Operation::Add: return lhs + rhs;
        case Operation::Sub: return lhs - rhs;
        case Operation::Mul: return lhs * rhs;
        case Operation::Div:
            return (rhs != 0.0) ? (lhs / rhs)
                                : std::numeric_limits<double>::quiet_NaN();
        case Operation::None: return rhs;
    }
    Q_UNREACHABLE();
}

auto Calculator::set_display(const double v) -> void {
    if (std::isnan(v)) {
        m_display = QStringLiteral("Error");
        m_has_decimal = false;
        return;
    }
    if (std::isinf(v)) {
        m_display = (v > 0.0) ? QStringLiteral("∞") : QStringLiteral("-∞");
        m_has_decimal = false;
        return;
    }
    // Show integer results without a decimal point
    if (v == std::trunc(v) && std::abs(v) < 1.0e12) {
        m_display = QString::number(static_cast<int64_t>(v));
        m_has_decimal = false;
    } else {
        m_display = QString::number(v, 'g', 10);
        m_has_decimal = m_display.contains(QLatin1Char('.'));
    }
}
