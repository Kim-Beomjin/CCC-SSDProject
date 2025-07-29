#include <string>

#define interface struct

interface Executor {
	virtual bool execute(const char* command, const char* args) = 0;
};

class Writer : public Executor {
public:
	bool execute(const char* command, const char* args) override {
		return true;
	}
};

class Reader : public Executor {
public:
	bool execute(const char* command, const char* args) override {
		return true;
	}
};

class Helper : public Executor {
public:
	bool execute(const char* command, const char* args) override {
		return true;
	}
};

class Exitor : public Executor {
public:
	bool execute(const char* command, const char* args) override {
		// TODO: print exit message
		return true;
	}
};

class ExecutorFactory {
public:
	Executor* createExecutor(const std::string command) {
		if (command == "write" || command == "fullwrite") {
			return new Writer();
		} else if (command == "read" || command == "fullread") {
			return new Reader();
		} else if (command == "help") {
			return new Helper();
		} else if (command == "exit") {
			return new Exitor();
		}
		return nullptr;
	}
};
