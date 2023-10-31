#pragma once
#include <string>
using namespace std;
namespace nc
{
	static class StringUtils
	{
	public:

		static std::string ToUpper(std::string s);
		static std::string ToLower(std::string s);
		static bool IsEqualIgnoreCase(std::string s1, std::string s2);
		static std::string CreateUnique(std::string s);
	private:
		static unsigned int id;
	};
}