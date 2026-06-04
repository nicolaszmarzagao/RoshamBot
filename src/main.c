#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <jansson.h>

#include "roshambo.h"

#define TELEGRAM_URL "https://api.telegram.org/bot"
static char* TOKEN = NULL;

size_t write_callback(void* contents, size_t size, size_t nmemb, char** response) {
	size_t total_size = size * nmemb;
	*response = realloc(*response, total_size + 1);
	memcpy(*response, contents, total_size);
	(*response)[total_size] = '\0';

	return total_size;
}

char* set_request_url(const char* path) {
	char* request_url = malloc(257);
	if (!request_url) return NULL;

	snprintf(request_url, 257, "%s%s%s", TELEGRAM_URL, TOKEN, path);

	return request_url;
}

CURLcode make_get_updates(CURL* curl, char** response) {
	char* request_url = set_request_url("/getUpdates");

	curl_easy_setopt(curl, CURLOPT_URL, request_url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);

	return curl_easy_perform(curl);
}

CURLcode send_message(CURL* curl, char** response, long long chat_id, const char* text) {
	char* request_url = set_request_url("/sendMessage");

	char body[256];
	sprintf(body, "chat_id=%lld&text=%s\n", chat_id, text);

	curl_easy_setopt(curl, CURLOPT_URL, request_url);
	curl_easy_setopt(curl, CURLOPT_POST, 1L);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);

	return curl_easy_perform(curl);
}

void setup_env_token() {
	char* env_token = getenv("TELEGRAM_TOKEN");

	if (env_token == NULL) {
		fprintf(stderr, "Error: TELEGRAM_TOKEN not set\n");
		exit(1);
	}

	TOKEN = strdup(env_token);
	if (TOKEN == NULL) {
		fprintf(stderr, "Error: Memory allocation failed\n");
		exit(1);
	}
}

long long get_chat_id(CURL* curl, char** response) {
	CURLcode res;
	// add error handling
	res = make_get_updates(curl, response);
	if(res == CURLE_OK) {
		printf("Response:\n%s\n", response[0]);
	} else {
		fprintf(stderr, "curl failed: %s\n", curl_easy_strerror(res));
	}

	const char* response_str = response[0];

	if (strstr(response_str, "\"id\"")) {
	printf("This string contains ID\n");
	}
	json_error_t error;
	json_t* root = json_loads(response_str, 0, &error);
	json_t* r = json_object_get(root, "result");
	json_t* head = json_array_get(r, 0);
	json_t* m = json_object_get(head, "message");
	json_t* chat = json_object_get(m, "chat");
	json_t* id = json_object_get(chat, "id");
	return json_integer_value(id);

}

char* intro_message()
{
	char* intro = 
		"\t Let's play roshambo! \t\n\n"
		"Here are your options...\n\n"
		"/rock\n"
		"/paper\n"
		"/scissors\n\n"
		"Ready?\n\n";
	return intro;
}

int main(void) {
	setup_env_token();

	CURL* curl = curl_easy_init();
	char* response = malloc(1);
	if (!response) return -1;
	response[0] = '\0';
	CURLcode res;
	if (curl) {

		long long chat_id = get_chat_id(curl, &response);

		res = send_message(curl, &response, chat_id, intro_message());
		if(res == CURLE_OK) {
			printf("Response:\n%s\n", response);
		} 
		else {
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
