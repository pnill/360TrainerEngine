#include <vector>
//#include "main.h"
using namespace std;

class Logging
{
public:
	std::vector<DWORD> TexList;

	bool Add(std::vector<DWORD> &List, DWORD tex);
	bool Remove(std::vector<DWORD> &List, DWORD m_Tex);
	void Log(std::vector<DWORD> &List);
};

bool Logging::Add(std::vector<DWORD> &List, DWORD tex)
{
	for (unsigned int i = 0; i < List.size(); i++)
	{
		if (tex == List[i]) return false;
	}
	List.push_back(tex);
	return true;
}

bool Logging::Remove(std::vector<DWORD> &List, DWORD tex)
{
	if (!List.empty())
	{
		for (unsigned int i = 0; i < List.size(); i++)
		{
			if (tex == List[i])
			{
				List.erase(List.begin() + i);
				return true;
			}
		}
	}
	return false;
}

void Logging::Log(std::vector<DWORD> &List)
{
	for(unsigned int i=0; i<List.size(); i++)
	{
		DbgPrint("crc32: 0x%x\r\n",List[i]);

		//add_log("crc32: 0x%x", List[i]);
	}
}