#include "executor.h"
#include "compositeExecutor.h"

const unordered_map<string, CompositeExecutorFactory::Creator> CompositeExecutorFactory::factoryMap = {
	{FIRST_SCRIPT_SHORT_NAME,  []() { return new FullWriteAndReadCompare(new Writer(), new Comparer()); }},
	{FIRST_SCRIPT_FULL_NAME,   []() { return new FullWriteAndReadCompare(new Writer(), new Comparer()); }},
	{SECOND_SCRIPT_SHORT_NAME, []() { return new PartialLBAWrite(new Writer(), new Comparer()); }},
	{SECOND_SCRIPT_FULL_NAME,  []() { return new PartialLBAWrite(new Writer(), new Comparer()); }},
	{THIRD_SCRIPT_SHORT_NAME,  []() { return new WriteReadAging(new Writer(), new Comparer()); }},
	{THIRD_SCRIPT_FULL_NAME,   []() { return new WriteReadAging(new Writer(), new Comparer()); }},
	{FOURTH_SCRIPT_SHORT_NAME, []() { return new EraseAndWriteAging(new Writer(), new Comparer(), new Eraser()); }},
	{FOURTH_SCRIPT_FULL_NAME,  []() { return new EraseAndWriteAging(new Writer(), new Comparer(), new Eraser()); }},
};

IExecutor* IExecutorFactory::getExecutorFromCache(const string& command) {
	auto cacheIterator = executorCache.find(command);
	if (cacheIterator != executorCache.end()) {
		return cacheIterator->second.get();
	}

	return nullptr;
}

IExecutor* CompositeExecutorFactory::createExecutor(const string& command) {
	IExecutor* executor = getExecutorFromCache(command);
	if (nullptr != executor) return executor;

	executor = getExecutorFromMap(command);
	if (nullptr != executor) return executor;

	return nullptr;
}

IExecutor* CompositeExecutorFactory::getExecutorFromMap(const string& command) {
	auto mapIterator = factoryMap.find(command);
	if (mapIterator != factoryMap.end()) {
		std::unique_ptr<IExecutor> executor(mapIterator->second());
		IExecutor* rawPtr = executor.get();
		executorCache[command] = std::move(executor);
		return rawPtr;
	}

	return nullptr;
}

const unordered_map<string, ExecutorFactory::Creator> ExecutorFactory::factoryMap = {
	{WRITE_CMD,                []() { return new OuputDecoratedWriter(); }},
	{FULL_WRITE_CMD,           []() { return new FullWriter(); }},
	{READ_CMD,                 []() { return new OuputDecoratedReader(); }},
	{FULL_READ_CMD,            []() { return new FullReader(); }},
	{ERASE_CMD,                []() { return new SizeEraser(); }},
	{ERASE_RANGE_CMD,          []() { return new RangeEraser(); }},
	{HELP_CMD,                 []() { return new Helper(); }},
	{EXIT_CMD,                 []() { return new Exiter(); }},
	{FLUSH_CMD,                []() { return new Flusher(); }},

	{FIRST_SCRIPT_SHORT_NAME,  []() { return new FullWriteAndReadCompare(new Writer(), new Comparer()); }},
	{FIRST_SCRIPT_FULL_NAME,   []() { return new FullWriteAndReadCompare(new Writer(), new Comparer()); }},
	{SECOND_SCRIPT_SHORT_NAME, []() { return new PartialLBAWrite(new Writer(), new Comparer()); }},
	{SECOND_SCRIPT_FULL_NAME,  []() { return new PartialLBAWrite(new Writer(), new Comparer()); }},
	{THIRD_SCRIPT_SHORT_NAME,  []() { return new WriteReadAging(new Writer(), new Comparer()); }},
	{THIRD_SCRIPT_FULL_NAME,   []() { return new WriteReadAging(new Writer(), new Comparer()); }},
	{FOURTH_SCRIPT_SHORT_NAME, []() { return new EraseAndWriteAging(new Writer(), new Comparer(), new Eraser()); }},
	{FOURTH_SCRIPT_FULL_NAME,  []() { return new EraseAndWriteAging(new Writer(), new Comparer(), new Eraser()); }},
};

IExecutor* ExecutorFactory::createExecutor(const string& command) {
	IExecutor* executor = getExecutorFromCache(command);
	if (nullptr != executor) return executor;

	executor = getExecutorFromMap(command);
	if (nullptr != executor) return executor;

	return nullptr;
}

IExecutor* ExecutorFactory::getExecutorFromMap(const string& command) {
	auto mapIterator = factoryMap.find(command);
	if (mapIterator != factoryMap.end()) {
		std::unique_ptr<IExecutor> executor(mapIterator->second());
		IExecutor* rawPtr = executor.get();
		executorCache[command] = std::move(executor);
		return rawPtr;
	}

	return nullptr;
}