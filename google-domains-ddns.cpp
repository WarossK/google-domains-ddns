#include "google-domains-ddns.h"

std::string user{};
std::string password{};
std::string address{};
uint64_t refresh_minutes{};

bool load_yaml()
{
	try
	{
		auto config = YAML::LoadFile("ddns_conf.yaml");
		user = config["config"]["user"].as<std::string>();
		password = config["config"]["password"].as<std::string>();
		address = config["config"]["address"].as<std::string>();
		refresh_minutes = config["config"]["refresh_minutes"].as<uint64_t>();
	}
	catch (YAML::Exception & exception)
	{
		fmt::print("{}\n", exception.what());
		return false;
	}

	return true;
}

bool execute()
{
	try
	{
		std::string global_ip{};
		{
			curlpp::Cleanup cleaner{};
			curlpp::Easy request{};
			request.setOpt(curlpp::options::Url("https://domains.google.com/checkip"));
			request.setOpt(curlpp::options::WriteFunction(
				curlpp::Types::WriteFunctionFunctor([&global_ip](char* pstr, size_t size, size_t nmemb) -> size_t
				{
					size_t allsize = size * nmemb;
					global_ip.append(static_cast<const char*>(pstr), allsize);
					return allsize;
				}))
			);

			request.perform();
		}

		{
			curlpp::Cleanup cleaner{};
			curlpp::Easy request{};
			request.setOpt(curlpp::options::Url("https://" + user + ":" + password + "@domains.google.com/nic/update?hostname=" + address + "&myip=" + global_ip));
			request.setOpt(curlpp::options::WriteFunction(
				curlpp::Types::WriteFunctionFunctor([&global_ip](char* pstr, size_t size, size_t nmemb) -> size_t
				{
					size_t allsize = size * nmemb;
					std::string response{};
					response.append(static_cast<const char*>(pstr), allsize);
					fmt::print(response + "\n");
					return allsize;
				}))
			);
			request.perform();
		}
	}
	catch (std::exception & exception)
	{
		fmt::print("{}\n", exception.what());
		return false;
	}

	return true;
}

int main()
{
	return (load_yaml() && execute()) ? EXIT_SUCCESS : EXIT_FAILURE;
}
