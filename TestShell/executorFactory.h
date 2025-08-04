#pragma once
#include <unordered_map>
#include <functional>

#include "global_config.h"
#include "interface.h"
#include "executor.h"
#include "compositeExecutor.h"

using std::unordered_map;
using std::function;

class CachedExecutorFactory : public IExecutorFactory
{
public:
	shared_ptr<IExecutor> createExecutor(const string& command) override;

protected:
	using Creator = function<shared_ptr<IExecutor>()>;
    virtual const unordered_map<string, Creator>& getFactoryMap() = 0;

private:
	shared_ptr<IExecutor> getExecutorFromCache(const string& command);
	shared_ptr<IExecutor> getExecutorFromMap(const string& command);

    unordered_map<string, shared_ptr<IExecutor>> executorCache;
};

class ExecutorFactory : public CachedExecutorFactory
{
protected:
    const unordered_map<string, CachedExecutorFactory::Creator>& getFactoryMap() override
	{
		static const unordered_map<string, CachedExecutorFactory::Creator> factoryMap =
		{
			{WRITE_CMD,         []() { return std::make_shared<OuputDecoratedWriter>(); }},
			{FULL_WRITE_CMD, 	[]() { return std::make_shared<FullWriter>(); 			}},
			{READ_CMD,          []() { return std::make_shared<OuputDecoratedReader>(); }},
			{FULL_READ_CMD,     []() { return std::make_shared<FullReader>(); 			}},
			{ERASE_CMD,         []() { return std::make_shared<SizeEraser>(); 			}},
			{ERASE_RANGE_CMD,	[]() { return std::make_shared<RangeEraser>(); 			}},
			{HELP_CMD,          []() { return std::make_shared<Helper>(); 				}},
			{EXIT_CMD,          []() { return std::make_shared<Exiter>(); 				}},
			{FLUSH_CMD,         []() { return std::make_shared<Flusher>(); 				}},
		};

        return factoryMap;
    }
};

class CompositeExecutorFactory : public IExecutorFactory
{
public:
	shared_ptr<IExecutor> createExecutor(const string& command) override;

private:
	shared_ptr<IExecutor> executorCache;
};

class CompositeExecutorStrategyFactory
{
public:
    static shared_ptr<ICompositeExecutorStrategy> createStrategy(const string& name)
	{
		static const unordered_map<string, Creator> strategyMap = {
			{FIRST_SCRIPT_SHORT_NAME,	[]() { return std::make_shared<FullWriteReadCompareStrategy>(); }},
			{FIRST_SCRIPT_FULL_NAME,	[]() { return std::make_shared<FullWriteReadCompareStrategy>(); }},
			{SECOND_SCRIPT_SHORT_NAME,	[]() { return std::make_shared<PartialLBAWriteStrategy>(); }},
			{SECOND_SCRIPT_FULL_NAME,	[]() { return std::make_shared<PartialLBAWriteStrategy>(); }},
			{THIRD_SCRIPT_SHORT_NAME,	[]() { return std::make_shared<WriteReadAgingStrategy>(); }},
			{THIRD_SCRIPT_FULL_NAME,	[]() { return std::make_shared<WriteReadAgingStrategy>(); }},
			{FOURTH_SCRIPT_SHORT_NAME,	[]() { return std::make_shared<EraseWriteAgingStrategy>(); }},
			{FOURTH_SCRIPT_FULL_NAME,	[]() { return std::make_shared<EraseWriteAgingStrategy>(); }},
		};

		auto mapIterator = strategyMap.find(name);
		if (mapIterator == strategyMap.end()) return nullptr;

		return mapIterator->second();
	}

private:
    using Creator = function<shared_ptr<ICompositeExecutorStrategy>()>;
};

class StrategyCompositeExecutorFactory : public CompositeExecutorFactory
{
public:
	shared_ptr<IExecutor> createExecutor(const string& command) override;

private:
	CompositeExecutorStrategyFactory strategyFactory;
};

class DelegatedExecutorFactory : public IExecutorFactory
{
public:
	DelegatedExecutorFactory() {
		factories.push_back(std::make_shared<ExecutorFactory>());
		factories.push_back(std::make_shared<StrategyCompositeExecutorFactory>());
	}

	shared_ptr<IExecutor> createExecutor(const string& command) override;

private:
	std::vector<shared_ptr<IExecutorFactory>> factories;
};
