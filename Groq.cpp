#include "GroqAPI.hpp"

int main() {
	try {
		std::string apiKey = "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";
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