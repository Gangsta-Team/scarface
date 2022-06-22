#pragma once

class CConsole {
public:
	CConsole() {
	}
	bool Initialize() {
		AllocConsole();
		FILE* fDummy;
		freopen_s(&fDummy, "CONIN$", "r", stdin);
		freopen_s(&fDummy, "CONOUT$", "w", stderr);
		freopen_s(&fDummy, "CONOUT$", "w", stdout);

		return true;
	}
	~CConsole() {
		FreeConsole();
	}
};
