#include "GroqApi.hpp"

int main() {
	try {
		std::string apiKey = "groq_api_key";
		std::string model = "llama3-8b-8192";
		std::string userMessage = "Hello there!";

		std::string response = GroqApi::GetInstance().SendChatRequest(apiKey, model, userMessage);
		GroqApi::GetInstance().PrintChatResponse(response);
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}

	return 0;
}