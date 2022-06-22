#pragma once

class CINI {
private:
	FILE*			m_fIni;
	char			m_szWorkingPath[MAX_PATH];
	char			m_szFileName[512];
	bool			m_bInitialized;
public:
	CINI() {
		m_bInitialized = false;
	}
	~CINI() {
	}

	bool Initialize() {

		GetModuleFileNameA(NULL, m_szWorkingPath, MAX_PATH);
		std::string::size_type pos = std::string(m_szWorkingPath).find_last_of("\\/");

		snprintf(m_szFileName, sizeof(m_szFileName), "%s\\%s%s", std::string(m_szWorkingPath).substr(0, pos).c_str(), PROJECT_NAME, ".ini");

		m_bInitialized = true;
		return true;
	}

	float ReadFloatSetting(std::string key)
	{
		if (!m_bInitialized) return NULL;
		float value = 0.0f;
		char buffer[32];
		GetPrivateProfileStringA("Scarface", key.c_str(), "0.0", buffer, sizeof(buffer), m_szFileName);
		sscanf_s(buffer, "%f", &value);
		return value;
	}

	int ReadSetting(std::string key) {
		if (!m_bInitialized) return NULL;
		return GetPrivateProfileIntA("Scarface", key.c_str(), -1, m_szFileName);
	}

	bool WriteSetting(std::string key, int value)
	{
		if (!m_bInitialized) return NULL;
		char buffer[32];
		sprintf_s(buffer, "%d", value);
		return WritePrivateProfileStringA("Scarface", key.c_str(), buffer, m_szFileName) != FALSE;
	}

	bool WriteFloatSetting(std::string key, float value)
	{
		if (!m_bInitialized) return NULL;
		char buffer[32];
		sprintf_s(buffer, "%f", value);
		return WritePrivateProfileStringA("Scarface", key.c_str(), buffer, m_szFileName) != FALSE;
	}
};
