#include "calculator_window.hpp"
#include <QApplication>

auto main(int argc, char* argv[]) -> int32_t {
    QApplication app(argc, argv);

    CalculatorWindow window;
    window.show();

    return QApplication::exec();
}
