#include "executorFactory.h"

shared_ptr<IExecutor> CachedExecutorFactory::createExecutor(const string& command)
{
	shared_ptr<IExecutor> executor = getExecutorFromCache(command);
	if (nullptr != executor) return executor;
	
	executor = getExecutorFromMap(command);
	if (nullptr != executor)
	{
		executorCache[command] = executor;
		return executor;
	}
	
	return nullptr;
}

shared_ptr<IExecutor> CachedExecutorFactory::getExecutorFromCache(const string& command)
{
	auto cacheIterator = executorCache.find(command);
	if (cacheIterator == executorCache.end()) return nullptr;

	return cacheIterator->second;
}

shared_ptr<IExecutor> CachedExecutorFactory::getExecutorFromMap(const string& command)
{
	const auto& factoryMap = getFactoryMap();

	auto mapIterator = factoryMap.find(command);
	if (mapIterator == factoryMap.end()) return nullptr;

	return mapIterator->second();
}

shared_ptr<IExecutor> DelegatedExecutorFactory::createExecutor(const string& command)
{
	for (const auto& factory : factories)
	{
		shared_ptr<IExecutor> executor = factory->createExecutor(command);
		if (executor != nullptr) return executor;
	}
	
	return nullptr;
}
