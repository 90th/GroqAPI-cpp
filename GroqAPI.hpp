#ifndef GROQ_API_HPP
#define GROQ_API_HPP

#include <string>
#include <windows.h>
#include <winhttp.h>
#include <iostream>
#include "nlohmann/json.hpp"
#include <memory>
#include <functional>
#include <mutex>

#pragma comment(lib, "winhttp.lib")

using json = nlohmann::json;

namespace Groq {
	class GroqApi {
	public:
		static GroqApi& GetInstance() {
			static GroqApi instance;
			return instance;
		}

		std::string SendChatRequest(const std::string& apiKey, const std::string& model, const std::string& userMessage);

		void PrintChatResponse(const std::string& response);

	private:
		GroqApi() = default;

		std::wstring ConvertToWideString(const std::string& str);

		void SendRequest(HINTERNET hRequest, const std::wstring& headers, const std::string& requestBody);

		std::string GetResponse(HINTERNET hRequest);

		std::mutex mutex_;

		GroqApi(const GroqApi&) = delete;
		GroqApi& operator=(const GroqApi&) = delete;
	};
} // namespace Groq

#endif // GROQ_API_HPP