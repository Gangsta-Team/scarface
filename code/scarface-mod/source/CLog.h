#pragma once

#include <fstream>
using namespace std;

enum MessageType {
	INFO_MSG,
	WARNING_MSG,
	ERROR_MSG
};

class CLog {
private:
	FILE*			m_fLog;
	char			m_szWorkingPath[MAX_PATH];
	char			m_szFileName[512];
	bool			m_bInitialized;
public:

	CLog() {
		m_bInitialized = false;
	}
	~CLog() {
		if (m_bInitialized) {
			fclose(m_fLog);
			m_fLog = NULL;
		}
	}

	bool Initialize() {
		GetModuleFileNameA(NULL, m_szWorkingPath, MAX_PATH);
		std::string::size_type pos = std::string(m_szWorkingPath).find_last_of("\\/");

		snprintf(m_szFileName, sizeof(m_szFileName), "%s\\%s%s", std::string(m_szWorkingPath).substr(0, pos).c_str(), PROJECT_NAME, ".log");

		m_fLog = fopen(m_szFileName, "a");
		if (m_fLog == NULL)
			return false;
		
		m_bInitialized = true;
		return true;
	}

	std::string GetMessageType(MessageType msg_type) {

		switch (msg_type)
		{
		case WARNING_MSG:
			return "WARNING";
			break;
		case ERROR_MSG:
			return "ERROR";
			break;
		case INFO_MSG:
			return "INFO";
			break;
		default:
			return "UNKNOWN";
			break;
		}
		return "-";
	}

	void LogMessage(MessageType msg_type, const char* format, ...) {
		if (!m_bInitialized || m_fLog == NULL)
			return;
		SYSTEMTIME	time;
		va_list		ap;

		GetLocalTime(&time);
		fprintf(m_fLog, "[%02d-%02d-%02d || %02d:%02d:%02d.%03d]::[%s] ", time.wDay, time.wMonth, time.wYear, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, GetMessageType(msg_type).c_str());
		va_start(ap, format);
		vfprintf(m_fLog, format, ap);
		va_end(ap);
		fprintf(m_fLog, "\n");
		fflush(m_fLog);
	}
};
