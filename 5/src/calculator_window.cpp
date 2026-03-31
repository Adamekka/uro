#include "calculator_window.hpp"
#include <QFont>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QSizePolicy>
#include <QVBoxLayout>
#include <cstdint>

// clang-format off
static constexpr const char* K_STYLE_GRAY_TOP =
    "QPushButton { background-color: #a5a5a5; color: white; font-size: 20px;"
    " font-weight: bold; border: none; }"
    "QPushButton:hover { background-color: #b5b5b5; }"
    "QPushButton:pressed { background-color: #c8c8c8; }";

static constexpr const char* K_STYLE_DIGIT =
    "QPushButton { background-color: #3a3a3a; color: white; font-size: 28px; border: none; }"
    "QPushButton:hover { background-color: #505050; }"
    "QPushButton:pressed { background-color: #666666; }";

// The 0 button spans two columns; its label is left-aligned to sit over the left cell
static constexpr const char* K_STYLE_DIGIT_ZERO =
    "QPushButton { background-color: #3a3a3a; color: white; font-size: 28px; border: none;"
    " text-align: left center; padding-left: 26px; }"
    "QPushButton:hover { background-color: #505050; }"
    "QPushButton:pressed { background-color: #666666; }";

static constexpr const char* K_STYLE_ORANGE =
    "QPushButton { background-color: #ff9500; color: white; font-size: 28px; border: none; }"
    "QPushButton:hover { background-color: #ffaa33; }"
    "QPushButton:pressed { background-color: #cc7700; }";

// Active operator: inverted colours (white bg, orange text) to match iOS highlight
static constexpr const char* K_STYLE_ORANGE_ACTIVE =
    "QPushButton { background-color: white; color: #ff9500; font-size: 28px; border: none; }"
    "QPushButton:hover { background-color: #eeeeee; }"
    "QPushButton:pressed { background-color: #dddddd; }";
// clang-format on

CalculatorWindow::CalculatorWindow(QWidget* parent): QWidget(parent) {
    setWindowTitle(QStringLiteral("Calculator"));
    setFixedSize(320, 500);
    setStyleSheet(QStringLiteral("background-color: black;"));

    // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
    auto* root = new QVBoxLayout(this);
    root->setSpacing(0);
    root->setContentsMargins(0, 0, 0, 0);

    // --- Display ---
    // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
    m_display = new QLabel(QStringLiteral("0"), this);
    m_display->setAlignment(Qt::AlignRight | Qt::AlignBottom);
    m_display->setStyleSheet(QStringLiteral(
        "background-color: #1c1c1c; color: white; padding: 10px 16px 14px 16px;"
    ));
    m_display->setFixedHeight(100);
    m_display->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QFont display_font;
    display_font.setPixelSize(64);
    m_display->setFont(display_font);
    root->addWidget(m_display);

    // --- Button grid ---
    // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
    auto* grid = new QGridLayout();
    grid->setSpacing(1);
    grid->setContentsMargins(0, 0, 0, 0);

    // Row 0: AC  +/-  %  ÷
    auto* btn_ac = make_button(QStringLiteral("AC"), K_STYLE_GRAY_TOP);
    auto* btn_neg = make_button(QStringLiteral("+/-"), K_STYLE_GRAY_TOP);
    auto* btn_pct = make_button(QStringLiteral("%"), K_STYLE_GRAY_TOP);
    m_btn_div = make_button(QStringLiteral("÷"), K_STYLE_ORANGE);
    grid->addWidget(btn_ac, 0, 0);
    grid->addWidget(btn_neg, 0, 1);
    grid->addWidget(btn_pct, 0, 2);
    grid->addWidget(m_btn_div, 0, 3);

    // Row 1: 7  8  9  ×
    auto* btn7 = make_button(QStringLiteral("7"), K_STYLE_DIGIT);
    auto* btn8 = make_button(QStringLiteral("8"), K_STYLE_DIGIT);
    auto* btn9 = make_button(QStringLiteral("9"), K_STYLE_DIGIT);
    m_btn_mul = make_button(QStringLiteral("×"), K_STYLE_ORANGE);
    grid->addWidget(btn7, 1, 0);
    grid->addWidget(btn8, 1, 1);
    grid->addWidget(btn9, 1, 2);
    grid->addWidget(m_btn_mul, 1, 3);

    // Row 2: 4  5  6  −
    auto* btn4 = make_button(QStringLiteral("4"), K_STYLE_DIGIT);
    auto* btn5 = make_button(QStringLiteral("5"), K_STYLE_DIGIT);
    auto* btn6 = make_button(QStringLiteral("6"), K_STYLE_DIGIT);
    m_btn_sub = make_button(QStringLiteral("−"), K_STYLE_ORANGE);
    grid->addWidget(btn4, 2, 0);
    grid->addWidget(btn5, 2, 1);
    grid->addWidget(btn6, 2, 2);
    grid->addWidget(m_btn_sub, 2, 3);

    // Row 3: 1  2  3  +
    auto* btn1 = make_button(QStringLiteral("1"), K_STYLE_DIGIT);
    auto* btn2 = make_button(QStringLiteral("2"), K_STYLE_DIGIT);
    auto* btn3 = make_button(QStringLiteral("3"), K_STYLE_DIGIT);
    m_btn_add = make_button(QStringLiteral("+"), K_STYLE_ORANGE);
    grid->addWidget(btn1, 3, 0);
    grid->addWidget(btn2, 3, 1);
    grid->addWidget(btn3, 3, 2);
    grid->addWidget(m_btn_add, 3, 3);

    // Row 4: 0 (wide)  .  =
    auto* btn0 = make_button(QStringLiteral("0"), K_STYLE_DIGIT_ZERO);
    auto* btn_dot = make_button(QStringLiteral("."), K_STYLE_DIGIT);
    auto* btn_eq = make_button(QStringLiteral("="), K_STYLE_ORANGE);
    grid->addWidget(btn0, 4, 0, 1, 2); // spans 2 columns
    grid->addWidget(btn_dot, 4, 2);
    grid->addWidget(btn_eq, 4, 3);

    for (int32_t col = 0; col < 4; ++col) {
        grid->setColumnStretch(col, 1);
    }
    for (int32_t row = 0; row < 5; ++row) {
        grid->setRowStretch(row, 1);
    }

    root->addLayout(grid);

    // --- Connections ---
    connect(
        btn_ac, &QPushButton::clicked, this, &CalculatorWindow::on_clear_clicked
    );
    connect(
        btn_neg,
        &QPushButton::clicked,
        this,
        &CalculatorWindow::on_negate_clicked
    );
    connect(
        btn_pct,
        &QPushButton::clicked,
        this,
        &CalculatorWindow::on_percent_clicked
    );

    connect(btn0, &QPushButton::clicked, this, [this] -> void {
        on_digit_clicked(0);
    });
    connect(btn1, &QPushButton::clicked, this, [this] -> void {
        on_digit_clicked(1);
    });
    connect(btn2, &QPushButton::clicked, this, [this] -> void {
        on_digit_clicked(2);
    });
    connect(btn3, &QPushButton::clicked, this, [this] -> void {
        on_digit_clicked(3);
    });
    connect(btn4, &QPushButton::clicked, this, [this] -> void {
        on_digit_clicked(4);
    });
    connect(btn5, &QPushButton::clicked, this, [this] -> void {
        on_digit_clicked(5);
    });
    connect(btn6, &QPushButton::clicked, this, [this] -> void {
        on_digit_clicked(6);
    });
    connect(btn7, &QPushButton::clicked, this, [this] -> void {
        on_digit_clicked(7);
    });
    connect(btn8, &QPushButton::clicked, this, [this] -> void {
        on_digit_clicked(8);
    });
    connect(btn9, &QPushButton::clicked, this, [this] -> void {
        on_digit_clicked(9);
    });

    connect(
        btn_dot,
        &QPushButton::clicked,
        this,
        &CalculatorWindow::on_decimal_clicked
    );

    connect(m_btn_div, &QPushButton::clicked, this, [this] -> void {
        on_operation_clicked(Operation::Div);
    });
    connect(m_btn_mul, &QPushButton::clicked, this, [this] -> void {
        on_operation_clicked(Operation::Mul);
    });
    connect(m_btn_sub, &QPushButton::clicked, this, [this] -> void {
        on_operation_clicked(Operation::Sub);
    });
    connect(m_btn_add, &QPushButton::clicked, this, [this] -> void {
        on_operation_clicked(Operation::Add);
    });

    connect(
        btn_eq,
        &QPushButton::clicked,
        this,
        &CalculatorWindow::on_equals_clicked
    );
}

auto CalculatorWindow::make_button(const QString& text, const char* const style)
    -> QPushButton* {
    // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
    auto* btn = new QPushButton(text, this);
    btn->setStyleSheet(QString::fromLatin1(style));
    btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    btn->setMinimumHeight(72);
    return btn;
}

auto CalculatorWindow::update_display() -> void {
    QString text = m_calc.display_string();
    m_display->setText(text);

    // Scale font down for long strings so nothing overflows
    auto len = static_cast<int32_t>(text.length());
    QFont font = m_display->font();
    if (len <= 6) {
        font.setPixelSize(64);
    } else if (len <= 9) {
        font.setPixelSize(48);
    } else {
        font.setPixelSize(32);
    }
    m_display->setFont(font);
}

auto CalculatorWindow::highlight_active_operation() -> void {
    // Reset all operator buttons to the standard orange style
    m_btn_div->setStyleSheet(QString::fromLatin1(K_STYLE_ORANGE));
    m_btn_mul->setStyleSheet(QString::fromLatin1(K_STYLE_ORANGE));
    m_btn_sub->setStyleSheet(QString::fromLatin1(K_STYLE_ORANGE));
    m_btn_add->setStyleSheet(QString::fromLatin1(K_STYLE_ORANGE));

    if (!m_calc.has_active_operation()) {
        return;
    }

    QPushButton* active = nullptr;
    switch (m_calc.active_operation()) {
        case Operation::Div: active = m_btn_div; break;
        case Operation::Mul: active = m_btn_mul; break;
        case Operation::Sub: active = m_btn_sub; break;
        case Operation::Add: active = m_btn_add; break;
        case Operation::None: break;
    }
    if (active) {
        active->setStyleSheet(QString::fromLatin1(K_STYLE_ORANGE_ACTIVE));
    }
}

auto CalculatorWindow::on_digit_clicked(const int32_t digit) -> void {
    m_calc.digit(digit);
    update_display();
    highlight_active_operation();
}

auto CalculatorWindow::on_decimal_clicked() -> void {
    m_calc.decimal();
    update_display();
}

auto CalculatorWindow::on_clear_clicked() -> void {
    m_calc.clear();
    update_display();
    highlight_active_operation();
}

auto CalculatorWindow::on_negate_clicked() -> void {
    m_calc.negate();
    update_display();
}

auto CalculatorWindow::on_percent_clicked() -> void {
    m_calc.percent();
    update_display();
}

auto CalculatorWindow::on_operation_clicked(const Operation op) -> void {
    m_calc.set_operation(op);
    update_display();
    highlight_active_operation();
}

auto CalculatorWindow::on_equals_clicked() -> void {
    m_calc.equals();
    update_display();
    highlight_active_operation();
}
