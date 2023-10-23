#include "lint.hpp"

// Track one linter for FinRL competition
namespace nutc {
    namespace lint_track_one {
        std::string
        lint(const std::string& uid, const std::string& algo_id)
        {
            std::optional<std::string> algo_code = nutc::client::get_algo(uid, algo_id);
            if (!algo_code.has_value()) {
                return "Could not find algorithm";
            }

            const bool e = nutc::pywrapper_track_one::create_api_module(nutc::mock_api::getMarketFunc());
            if (!e) {
                log_e(linting, "Failed to create API module");
                nutc::client::set_lint_result(uid, algo_id, false);
                return "Unexpected error: failed to create API module";
            }

            std::optional<std::string> err = nutc::pywrapper_track_one::import_py_code(algo_code.value());
            if (err.has_value()) {
                log_e(linting, "{}", err.value());
                nutc::client::set_lint_result(uid, algo_id, false);
                nutc::client::set_lint_failure(uid, algo_id, err.value());
                return err.value();
            }

            err = nutc::pywrapper_track_one::run_initialization();
            if (err.has_value()) {
                log_e(linting, "{}", err.value());
                nutc::client::set_lint_result(uid, algo_id, false);
                nutc::client::set_lint_failure(uid, algo_id, err.value());
                return err.value();
            }

            err = nutc::pywrapper_track_one::trigger_callbacks();
            if (err.has_value()) {
                log_e(linting, "{}", err.value());
                nutc::client::set_lint_result(uid, algo_id, false);
                nutc::client::set_lint_failure(uid, algo_id, err.value());
                return err.value();
            }

            nutc::client::set_lint_result(uid, algo_id, true);

            return "Lint succeeded!";
        }
    } // namespace lint_track_one
} // namespace nutc
