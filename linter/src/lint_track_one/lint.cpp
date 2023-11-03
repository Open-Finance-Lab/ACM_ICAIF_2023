#include <pybind11/eval.h>
#include "lint.hpp"

// Track one linter for FinRL competition
namespace nutc::lint_track_one {
    std::string
    lint(const std::string& uid, const std::string& algo_id)
    {
        const auto maybe_algo_files = nutc::client::get_algo_files(uid, algo_id);
        if (!maybe_algo_files.has_value()) {
            return "Could not find algorithm";
        }

        auto algo_files = maybe_algo_files.value();

        for (auto const& [key, _] : algo_files) {
            log_i(main, "got file: {}", key);
        }

        bool contains_zip_file = false;
        for (auto const& [file_name, _] : algo_files) {
            if (file_name.ends_with(".zip")) {
                contains_zip_file = true;
                break;
            }
        }
        if (!contains_zip_file || !algo_files.contains("test.py") || !algo_files.contains("readme.md")) {
            return "Missing required file(s).";
        }

        if (algo_files.contains("requirements.txt")) {
            auto txt = algo_files["requirements.txt"];
        }

        std::optional<std::string> err = nutc::pywrapper_track_one::import_py_code(algo_files["test.py"]);

        if (err.has_value()) {
            log_e(linting, "{}", err.value());
            nutc::client::set_lint_result(uid, algo_id, false);
            nutc::client::set_lint_failure(uid, algo_id, err.value());
            return err.value();
        }
//
//        err = nutc::pywrapper_track_one::run_initialization();
//        if (err.has_value()) {
//            log_e(linting, "{}", err.value());
//            nutc::client::set_lint_result(uid, algo_id, false);
//            nutc::client::set_lint_failure(uid, algo_id, err.value());
//            return err.value();
//        }
//
//        err = nutc::pywrapper_track_one::trigger_callbacks();
//        if (err.has_value()) {
//            log_e(linting, "{}", err.value());
//            nutc::client::set_lint_result(uid, algo_id, false);
//            nutc::client::set_lint_failure(uid, algo_id, err.value());
//            return err.value();
//        }
//
//        nutc::client::set_lint_result(uid, algo_id, true);

        return "Lint succeeded!";
    }
} // namespace nutc::lint_track_one
