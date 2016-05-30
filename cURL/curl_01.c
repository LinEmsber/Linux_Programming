/* A sample of cURL */

#include <stdio.h>
#include <curl/curl.h>
#include <curl/easy.h>

int main(){

	CURL *curl;
	CURLcode res;

	curl = curl_easy_init();

	if(curl) {

		// Start a libcurl easy session.
		curl_easy_setopt(curl, CURLOPT_URL, "https://tw.yahoo.com/");

		// Perform the request, res will get the return code.
		res = curl_easy_perform(curl);

		// Check for errors
		if(res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

		// Cleanup
		curl_easy_cleanup(curl);
	}
	return 0;
}
