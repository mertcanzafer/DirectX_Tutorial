#pragma once
#include "FixedWin.h"

#include <string>
#include <vector>

class DxgiInfoManager
{
public:
	DxgiInfoManager();

	DxgiInfoManager(const DxgiInfoManager& copy) = delete;
	DxgiInfoManager& operator =(const DxgiInfoManager& copy) = delete;

	void Set()noexcept;
	std::vector<std::string> GetMessage()const;

	~DxgiInfoManager();
private:
	uint64_t next;
	struct IDXGIInfoQueue* pDxgiInfoQueue;
};

