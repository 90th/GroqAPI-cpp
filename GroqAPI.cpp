#include "GroqAPI.hpp"

namespace {
	const std::wstring kUserAgent = L"GroqAPI-CPP/1.1";
	const std::wstring kServer = L"api.groq.com";
	const std::wstring kEndpoint = L"/openai/v1/chat/completions";
	const std::wstring kContentType = L"application/json";
}

namespace Groq {
	std::string GroqApi::SendChatRequest(const std::string& apiKey, const std::string& model, const std::string& userMessage) {
		std::lock_guard<std::mutex> lock(mutex_);

		HINTERNET hSession = WinHttpOpen(kUserAgent.c_str(), WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
		if (!hSession) {
			throw std::runtime_error("WinHttpOpen failed with error: " + std::to_string(GetLastError()));
		}
		auto sessionGuard = std::unique_ptr<void, decltype(&WinHttpCloseHandle)>(hSession, WinHttpCloseHandle);

		HINTERNET hConnect = WinHttpConnect(hSession, kServer.c_str(), INTERNET_DEFAULT_HTTPS_PORT, 0);
		if (!hConnect) {
			throw std::runtime_error("WinHttpConnect failed with error: " + std::to_string(GetLastError()));
		}
		auto connectGuard = std::unique_ptr<void, decltype(&WinHttpCloseHandle)>(hConnect, WinHttpCloseHandle);

		HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"POST", kEndpoint.c_str(), NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
		if (!hRequest) {
			throw std::runtime_error("WinHttpOpenRequest failed with error: " + std::to_string(GetLastError()));
		}
		auto requestGuard = std::unique_ptr<void, decltype(&WinHttpCloseHandle)>(hRequest, WinHttpCloseHandle);

		json requestBodyJson = {
			{"messages", json::array({{{"role", "user"}, {"content", userMessage}}})},
			{"model", model}
		};

		std::string requestBody = requestBodyJson.dump();
		std::wstring headers = L"Authorization: Bearer " + ConvertToWideString(apiKey) + L"\r\nContent-Type: " + kContentType + L"\r\n";

		SendRequest(hRequest, headers, requestBody);
		return GetResponse(hRequest);
	}

	void GroqApi::PrintChatResponse(const std::string& response) {
		std::lock_guard<std::mutex> lock(mutex_);

		json responseJson = json::parse(response);
		std::string content = responseJson["choices"][0]["message"]["content"];
		int completionTokens = responseJson["usage"]["completion_tokens"];
		double completionTime = responseJson["usage"]["completion_time"];
		double promptTime = responseJson["usage"]["prompt_time"];

		std::cout << "Content: " << content << std::endl;
		std::cout << "Completion Tokens: " << completionTokens << std::endl;
		std::cout << "Completion Time: " << completionTime << std::endl;
		std::cout << "Prompt Time: " << promptTime << std::endl;
	}

	std::wstring GroqApi::ConvertToWideString(const std::string& str) {
		int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.size()), NULL, 0);
		std::wstring wstrTo(size_needed, 0);
		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.size()), &wstrTo[0], size_needed);
		return wstrTo;
	}

	void GroqApi::SendRequest(HINTERNET hRequest, const std::wstring& headers, const std::string& requestBody) {
		BOOL bResults = WinHttpSendRequest(hRequest, headers.c_str(), static_cast<DWORD>(headers.length()), (LPVOID)requestBody.c_str(), static_cast<DWORD>(requestBody.length()), static_cast<DWORD>(requestBody.length()), 0);
		if (!bResults) {
			throw std::runtime_error("WinHttpSendRequest failed with error: " + std::to_string(GetLastError()));
		}

		bResults = WinHttpReceiveResponse(hRequest, NULL);
		if (!bResults) {
			throw std::runtime_error("WinHttpReceiveResponse failed with error: " + std::to_string(GetLastError()));
		}
	}

	std::string GroqApi::GetResponse(HINTERNET hRequest) {
		DWORD dwSize = 0;
		DWORD dwDownloaded = 0;
		std::string response;

		do {
			dwSize = 0;
			if (!WinHttpQueryDataAvailable(hRequest, &dwSize)) {
				throw std::runtime_error("WinHttpQueryDataAvailable failed with error: " + std::to_string(GetLastError()));
			}

			if (dwSize == 0)
				break;

			std::unique_ptr<char[]> buffer(new char[dwSize + 1]);
			ZeroMemory(buffer.get(), dwSize + 1);

			if (!WinHttpReadData(hRequest, (LPVOID)buffer.get(), dwSize, &dwDownloaded)) {
				throw std::runtime_error("WinHttpReadData failed with error: " + std::to_string(GetLastError()));
			}

			response.append(buffer.get(), dwDownloaded);
		} while (dwSize > 0);

		return response;
	}
} // namespace Groq