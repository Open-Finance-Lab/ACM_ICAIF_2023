#include "runtime_track_one.hpp"

namespace py = pybind11;

class PyStdErrOutStreamRedirect {
    py::object _stdout;
    py::object _stderr;
    py::object _stdout_buffer;
    py::object _stderr_buffer;
public:
    PyStdErrOutStreamRedirect() {
        auto sysm = py::module::import("sys");
        _stdout = sysm.attr("stdout");
        _stderr = sysm.attr("stderr");
        auto stringio = py::module::import("io").attr("StringIO");
        _stdout_buffer = stringio();  // Other filelike object can be used here as well, such as objects created by pybind11
        _stderr_buffer = stringio();
        sysm.attr("stdout") = _stdout_buffer;
        sysm.attr("stderr") = _stderr_buffer;
    }
    std::string stdoutString() {
        _stdout_buffer.attr("seek")(0);
        return py::str(_stdout_buffer.attr("read")());
    }
    std::string stderrString() {
        _stderr_buffer.attr("seek")(0);
        return py::str(_stderr_buffer.attr("read")());
    }
    ~PyStdErrOutStreamRedirect() {
        auto sysm = py::module::import("sys");
        sysm.attr("stdout") = _stdout;
        sysm.attr("stderr") = _stderr;
    }
};

namespace nutc {
namespace pywrapper_track_one {

std::optional<std::string>
import_py_code(const std::string& code)
{
    log_i(mock_runtime, "Importing algorithm code into python interpreter");

    PyStdErrOutStreamRedirect pyOutputRedirect{};
    try {
        py::exec("__name__ = '__main__'");
        py::exec(code);
        log_i(main, "result: \"{}\"", pyOutputRedirect.stdoutString());
        log_i(main, "stderr: \"{}\"", pyOutputRedirect.stderrString());
    } catch (const std::exception& e) {
        log_i(main, "result: \"{}\"", pyOutputRedirect.stdoutString());
        log_i(main, "stderr: \"{}\"", pyOutputRedirect.stderrString());
        return fmt::format("Failed to import code: {}", e.what());
    }



    return std::nullopt;
}

std::optional<std::string>
run_initialization()
{
    log_i(mock_runtime, "Running initialization code");
    try {
        py::exec("strategy = Strategy()");
    }

    catch (const std::exception& e) {
        return fmt::format("Failed to run initialization: {}", e.what());
    }

    try {
        py::object main_module = py::module_::import("__main__");
        py::dict main_dict = main_module.attr("__dict__");
        py::object on_trade_update = main_dict["strategy"].attr("on_trade_update");
        py::object on_orderbook_update =
            main_dict["strategy"].attr("on_orderbook_update");
        py::object on_account_update = main_dict["strategy"].attr("on_account_update");
    } catch (py::error_already_set& e) {
        return fmt::format("Failed to import callback functions: {}", e.what());
    }

    return std::nullopt;
}

std::optional<std::string>
trigger_callbacks()
{
    log_i(mock_runtime, "Triggering callbacks");
    try {
        py::exec(R"(place_market_order("BUY", "A", 1.0, 1.0))");
    } catch (const std::exception& e) {
        return fmt::format("Failed to run place_market_order: {}", e.what());
    }

    try {
        py::exec(R"(strategy.on_orderbook_update("A","BUY",1.0,1.0))");
    } catch (const std::exception& e) {
        return fmt::format("Failed to run on_orderbook_update: {}", e.what());
    }
    try {
        py::exec(R"(strategy.on_trade_update("A","BUY",1.0,1.0))");
    } catch (const std::exception& e) {
        return fmt::format("Failed to run on_trade_update: {}", e.what());
    }

    try {
        py::exec(R"(strategy.on_account_update("A","BUY",1.0,1.0,1.0))");
    } catch (const std::exception& e) {
        return fmt::format("Failed to run on_account_update: {}", e.what());
    }

    try {
        py::exec(R"(strategy.on_orderbook_update("B","BUY",1.0,1.0))");
    } catch (const std::exception& e) {
        return fmt::format("Failed to run on_orderbook_update: {}", e.what());
    }
    try {
        py::exec(R"(strategy.on_trade_update("B","BUY",1.0,1.0))");
    } catch (const std::exception& e) {
        return fmt::format("Failed to run on_trade_update: {}", e.what());
    }

    try {
        py::exec(R"(strategy.on_account_update("B","BUY",1.0,1.0,1.0))");
    } catch (const std::exception& e) {
        return fmt::format("Failed to run on_account_update: {}", e.what());
    }

      try {
        py::exec(R"(strategy.on_orderbook_update("C","BUY",1.0,1.0))");
    } catch (const std::exception& e) {
        return fmt::format("Failed to run on_orderbook_update: {}", e.what());
    }
    try {
        py::exec(R"(strategy.on_trade_update("C","BUY",1.0,1.0))");
    } catch (const std::exception& e) {
        return fmt::format("Failed to run on_trade_update: {}", e.what());
    }

    try {
        py::exec(R"(strategy.on_account_update("C","BUY",1.0,1.0,1.0))");
    } catch (const std::exception& e) {
        return fmt::format("Failed to run on_account_update: {}", e.what());
    }




    return std::nullopt;
}

} // namespace pywrapper_track_one
} // namespace nutc
