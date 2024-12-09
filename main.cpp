#include <iostream>
#include <fstream>
#include <string>
#include <curl/curl.h>
#include <gumbo.h>

size_t writeCallback(void *ptr, size_t size, size_t nmemb, std::string* out) {
    size_t totalSize = size * nmemb;
    out->append(static_cast<char*>(ptr), totalSize);
    return totalSize;
}

void searchForEmailInput(GumboNode* node, std::string& emailField, const std::string& targetField) {
    if (node->type != GUMBO_NODE_ELEMENT) return;

    GumboAttribute* attr;
    if ((attr = gumbo_get_attribute(&node->v.element.attributes, "id")) &&
        std::string(attr->value) == targetField) {
        emailField = attr->value; // Extracted email ID
        }

    GumboVector* children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; ++i) {
        searchForEmailInput(static_cast<GumboNode*>(children->data[i]), emailField, targetField);
    }
}

int main() {
    CURL *curl;
    CURLcode res;
    std::string response;

    std::string loginUrl = "https://auth.assetdash.com/login";

    std::string email = "YourFUCKINGEMAILBITC@Hsdf.com";
    std::string password = "ijdfgdfl'ik4";

    std::string emailFieldName;
    std::string passFieldName;

    curl = curl_easy_init();

    // std::string username = getenv("ASSETDASH_USERNAME");
    // std::string password = getenv("ASSETDASH_USERNAME");
    //
    // if (username.empty() || password.empty()) {
    //     std::cerr << "Username and password must be specified\n";
    // }
    //
    // std::string postData = "username=" + username + "&password=" + password;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, loginUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        std::string postData = emailFieldName + "=" + email + "&" + passFieldName = "=" + password;

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, postData.size());

        curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "cookies.txt");
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "cookies.txt");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 1L);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            std::cerr << "Curl Error: " << curl_easy_strerror(res) << "\n";
            return 1;
        }

        std::cout << "[+] Login page fetched!\n";

        GumboOutput* output = gumbo_parse(response.c_str());

        searchForEmailInput(output->root, emailFieldName, "email");
        searchForEmailInput(output->root, passFieldName, "password");

        if (!passFieldName.empty() && !emailFieldName.empty()) {
            std::cout << "[+] Found password input field with ID: " << passFieldName << "\n";
            std::cout << "[+] Found email input field with ID: " << emailFieldName << "\n";

            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());
            response.clear();
            res = curl_easy_perform(curl);

            if (res != CURLE_OK) {
                std::cerr << "[-] Curl Error: " << curl_easy_strerror(res) << "\n";
            } else {
                std::cout << "[+] Sent credentials. Response:" << response << "\n";
            }

        } else if (emailFieldName.empty()) {
            std::cerr << "[-] Could not find email input field dynamically\n";
        } else if (passFieldName.empty()) {
            std::cerr << "[-] Could not find password input field dynamically\n";
        }

        gumbo_destroy_output(&kGumboDefaultOptions, output);

        curl_easy_cleanup(curl);
    } else {
        std::cerr << "[-] Failed to initialize CURL.\n";
    }
}
