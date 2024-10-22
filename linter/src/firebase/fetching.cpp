#include "fetching.hpp"
#include <unordered_map>

namespace nutc {
namespace client {

void
print_algo_info(const glz::json_t& algo, const std::string& algo_id)
{
    log_i(firebase, "Running {}", algo["name"].get<std::string>());
    log_i(firebase, "Description: {}", algo["description"].get<std::string>());
    log_i(firebase, "Upload date: {}", algo["uploadDate"].get<std::string>());
    log_i(firebase, "Algo id: {}", algo_id);
}

void
set_lint_result(const std::string& uid, const std::string& algo_id, bool succeeded)
{
    std::string success = "\"success\"";
    std::string failure = "\"failure\"";
    glz::json_t res = firebase_request(
        "PUT",
        fmt::format(
            "{}/users/{}/algos/{}/lintResults.json",
            std::string(FIREBASE_URL),
            uid,
            algo_id
        ),
        succeeded ? success : failure
    );
}

std::string
replaceDisallowedValues(const std::string& input)
{
    std::regex newlinePattern("\\n");
    std::string input2 = std::regex_replace(input, newlinePattern, "\\n");
    std::regex disallowedPattern("[.$#\\[\\]]");

    return std::regex_replace(input2, disallowedPattern, "");
}

void
set_lint_success(
    const std::string& uid, const std::string& algo_id, const std::string& success
)
{
    std::string json_success = "\"" + replaceDisallowedValues(success) + "\"";
    log_e(main, "Seeing lint success: {}", json_success);
    glz::json_t res = firebase_request(
        "PUT",
        fmt::format(
            "{}/users/{}/algos/{}/lintSuccessMessage.json",
            std::string(FIREBASE_URL),
            uid,
            algo_id
        ),
        json_success
    );
    glz::json_t res2 = firebase_request(
        "PUT",
        fmt::format("{}/users/{}/latestAlgoId.json", std::string(FIREBASE_URL), uid),
        "\"" + algo_id + "\""
    );
}

void
set_lint_failure(
    const std::string& uid, const std::string& algo_id, const std::string& failure
)
{
    std::string json_failure = "\"" + replaceDisallowedValues(failure) + "\"";
    log_e(main, "Seeing lint failure: {}", json_failure);
    glz::json_t res = firebase_request(
        "PUT",
        fmt::format(
            "{}/users/{}/algos/{}/lintFailureMessage.json",
            std::string(FIREBASE_URL),
            uid,
            algo_id
        ),
        json_failure
    );
}

glz::json_t
get_user_info(const std::string& uid)
{
    auto url = fmt::format("{}/users/{}.json", std::string(FIREBASE_URL), uid);
    return firebase_request("GET", url);
}

static size_t
write_callback(void* contents, size_t size, size_t nmemb, void* userp)
{
    auto* str = reinterpret_cast<std::string*>(userp);
    auto* data = static_cast<char*>(contents);

    str->append(data, size * nmemb);
    return size * nmemb;
}

std::string
storage_request(const std::string& firebase_url)
{
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, firebase_url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res)
                      << std::endl;
        }

        curl_easy_cleanup(curl);
    }

    return readBuffer;
}

std::optional<std::string>
get_algo(const std::string& uid, const std::string& algo_id)
{
    glz::json_t user_info = get_user_info(uid);
    // if not has "algos"
    if (!user_info.contains("algos")) {
        log_w(
            firebase, "User {} has no algos. Will not participate in simulation.", uid
        );
        return std::nullopt;
    }
    if (!user_info["algos"].contains(algo_id)) {
        log_w(firebase, "User {} does not have algo id {}.", uid, algo_id);
        return std::nullopt;
    }
    glz::json_t algo_info = user_info["algos"][algo_id];
    std::string downloadURL = algo_info["0"]["downloadURL"].get<std::string>();
    print_algo_info(algo_info, algo_id);
    std::string algo_file = storage_request(downloadURL);
    return algo_file;
}

std::optional<std::unordered_map<std::string, std::string>>
get_algo_files(const std::string& uid, const std::string& algo_id)
{
    glz::json_t user_info = get_user_info(uid);

    if (!user_info.contains("algos")) {
        log_w(firebase, "User {} has no algos. Will not participate in simulation.", uid);
        return std::nullopt;
    }

    if (!user_info["algos"].contains(algo_id)) {
        log_w(firebase, "User {} does not have algo id {}.", uid, algo_id);
        return std::nullopt;
    }
    glz::json_t algo_info = user_info["algos"][algo_id];

    std::unordered_map<std::string, std::string> algo_files_map;
    size_t file_idx = 0;
    while (algo_info.contains(std::to_string(file_idx))) {
        glz::json_t file = algo_info[std::to_string(file_idx)];
        log_i(main, "index: {}, file: {}", file_idx, glz::prettify(glz::write_json(file)));
        file_idx++;

        std::string file_name = file["fileName"].get<std::string>();
        std::string download_url = file["downloadURL"].get<std::string>();
        std::string file_contents = storage_request(download_url);

        algo_files_map.insert(std::make_pair(file_name, file_contents));
    }

    return algo_files_map;
}

glz::json_t
firebase_request(
    const std::string& method, const std::string& url, const std::string& data
)
{
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        if (method == "POST") {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
        }
        else if (method == "PUT") {
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
        }
        else if (method == "DELETE") {
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
        }

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            log_e(firebase, "curl_easy_perform() failed: {}", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
    }

    glz::json_t json{};
    auto error = glz::read_json(json, readBuffer);
    if (error) {
        std::string descriptive_error = glz::format_error(error, readBuffer);
        log_e(firebase, "glz::read_json() failed: {}", descriptive_error);
    }
    return json;
}
} // namespace client
} // namespace nutc
