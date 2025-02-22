
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

// Callback function to write response data
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t totalSize = size * nmemb;
    char** response_ptr = (char**)userp;

    // Allocate or expand memory for the response
    *response_ptr = realloc(*response_ptr, strlen(*response_ptr) + totalSize + 1);
    if (*response_ptr == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 0;
    }

    strncat(*response_ptr, contents, totalSize);
    return totalSize;
}

int main() {
    CURL* curl;
    CURLcode res;
    char* response = calloc(1, sizeof(char));  // Allocate initial memory for response

    if (!response) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        const char* url = "https://jsonplaceholder.typicode.com/posts/1";

        // Set cURL options
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Perform the GET request
        res = curl_easy_perform(curl);

        // Error handling
        if (res != CURLE_OK) {
            fprintf(stderr, "cURL Error: %s\n", curl_easy_strerror(res));
        } else {
            printf("API Response:\n%s\n", response);
        }

        // Cleanup
        curl_easy_cleanup(curl);
    } else {
        fprintf(stderr, "Failed to initialize cURL\n");
    }

    curl_global_cleanup();
    free(response);  // Free allocated memory
    return 0;
}
