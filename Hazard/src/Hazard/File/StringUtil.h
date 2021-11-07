
#include "Hazard/Core/Core.h"

namespace Hazard {
	class StringUtil {

	public:
		static std::vector<std::string> SplitString(std::string string, char delim);
		static std::string RemoveAtEnd(std::string& source, char val);
		static std::string BytesToString(const size_t& bytes);
		static std::string Replace(const std::string& value, const std::string& find, const std::string& replace);

		template<typename T>
		static std::string FormatNumber(T number) {
			std::string num = std::to_string(number);
			int n = num.length() - 3;

			while (n > 0) {
				num.insert(n, ",");
				n -= 3;
			}
			return num;
		}
	};
}