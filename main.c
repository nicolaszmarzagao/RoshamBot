#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#define TELEGRAM_URL "https://api.telegram.org/bot"
static char* TOKEN = NULL;

size_t write_callback(void* contents, size_t size, size_t nmemb, char** response) {
    size_t total_size = size * nmemb;
    *response = realloc(*response, total_size + 1);
    memcpy(*response, contents, total_size);
    (*response)[total_size] = '\0';

    return total_size;
}


char* get_request_url(const char* path) {
    char* request_url = malloc(257);
    if (!request_url) return NULL;
    
    snprintf(request_url, 257, "%s%s%s", TELEGRAM_URL, TOKEN, path);
    
    return request_url;
}


CURLcode make_get_request(CURL* curl, char** response) {
    char* request_url = get_request_url("/getUpdates");

    curl_easy_setopt(curl, CURLOPT_URL, request_url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);

    return curl_easy_perform(curl);
}

CURLcode make_post_request(CURL* curl, char** response) {
    char* request_url = get_request_url("/sendMessage");

    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "chat_id=123&text=hello");

    return curl_easy_perform(curl);
}

int main(void) {
    char* env_token = getenv("TELEGRAM_TOKEN");
    
    if (env_token == NULL) {
        fprintf(stderr, "Error: TELEGRAM_TOKEN not set\n");
        return 0;
    }
    
    TOKEN = strdup(env_token);
    if (TOKEN == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return 0;
    }

    CURL* curl = curl_easy_init();

    char* response = malloc(1);
    if (!response) return -1;
    response[0] = '\0';

    CURLcode res;


    if (curl) {
        res = make_get_request(curl, &response);
        if(res == CURLE_OK) {
            printf("Response:\n%s\n", response);
        } else {
            fprintf(stderr, "curl failed: %s\n", curl_easy_strerror(res));
        }


        res = make_post_request(curl, &response);
        if(res == CURLE_OK) {
            printf("Response:\n%s\n", response);
        } else {
            fprintf(stderr, "curl failed: %s\n", curl_easy_strerror(res));
        }
        
        if (TOKEN != NULL) {
            free(TOKEN);
            TOKEN = NULL;
        }
        free(response);
        curl_easy_cleanup(curl); 

    }

    return 0;
}
