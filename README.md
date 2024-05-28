# GroqAPI
![GitHub Downloads (all assets, all releases)](https://img.shields.io/github/downloads/90th/GroqAPI-cpp/total) ![GitHub last commit](https://img.shields.io/github/last-commit/90th/GroqAPI-cpp)


GroqAPI-cpp (Groq API Wrapper) is a minimalistic C++ library for easy interaction with the Groq API. It simplifies sending requests, processing responses, and supports multithreading. Ideal for integrating Groq's language processing into C++ applications efficiently. Works seamlessly with C++14 and later versions. 

## Features

- Send chat requests to the Groq API with ease.
- Print chat responses directly to the console.
- Thread-safe singleton pattern ensures safe concurrent access.
- Utilizes Windows HTTP Services for network communication.
- Supports C++14 and later versions.

## Usage

```cpp
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
```
## Dependencies

- Windows SDK (for WinHttp)
- nlohmann/json (included as a header-only library)
- C++14 compiler or later

## Installation

Simply include the GroqApi.hpp header file in your project and link against the Windows HTTP Services library (winhttp.lib).