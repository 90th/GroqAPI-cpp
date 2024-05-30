#include "GroqAPI.hpp"

int main() {
	try {
		std::string apiKey = "groq-api-key";
		std::string model = "llama3-8b-8192";
		std::string userMessage = "Hello there!";

		std::string response = Groq::GroqApi::GetInstance().SendChatRequest(apiKey, model, userMessage);
		Groq::GroqApi::GetInstance().PrintChatResponse(response);
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}
	return 0;
}