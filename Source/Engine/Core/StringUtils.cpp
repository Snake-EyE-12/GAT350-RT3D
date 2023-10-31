#include "StringUtils.h"
#include <cstring>
namespace nc
{
	unsigned int StringUtils::id = 0;
	std::string StringUtils::ToUpper(std::string s)
	{
		string newString = "";
		for (int i = 0; i < s.length(); i++) {
			newString += toupper(s.at(i));
		}
		return newString;
	}

	std::string StringUtils::ToLower(std::string s)
	{
		string newString = "";
		for (int i = 0; i < s.length(); i++) {
			newString += tolower(s.at(i));
		}
		return newString;
	}

	bool StringUtils::IsEqualIgnoreCase(std::string s1, std::string s2)
	{
		return ToLower(s1) == ToLower(s2);
	}

	std::string StringUtils::CreateUnique(std::string s)
	{
		id++;
		return s + std::to_string(id);
	}
}