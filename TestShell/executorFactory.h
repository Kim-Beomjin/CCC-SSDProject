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
	using Creator = function<shared_ptr<IExecutor>()>;

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

class CompositeExecutorFactory : public CachedExecutorFactory
{
protected:
    const unordered_map<string, CachedExecutorFactory::Creator>& getFactoryMap() override
	{
		static const unordered_map<string, Creator> factoryMap =
		{
			{FIRST_SCRIPT_SHORT_NAME,	[]() { return std::make_shared<FullWriteAndReadCompare>(std::make_shared<Writer>(), std::make_shared<Comparer>()); }},
			{FIRST_SCRIPT_FULL_NAME,	[]() { return std::make_shared<FullWriteAndReadCompare>(std::make_shared<Writer>(), std::make_shared<Comparer>()); }},
			{SECOND_SCRIPT_SHORT_NAME,	[]() { return std::make_shared<PartialLBAWrite>(std::make_shared<Writer>(), std::make_shared<Comparer>()); }},
			{SECOND_SCRIPT_FULL_NAME,	[]() { return std::make_shared<PartialLBAWrite>(std::make_shared<Writer>(), std::make_shared<Comparer>()); }},
			{THIRD_SCRIPT_SHORT_NAME,	[]() { return std::make_shared<WriteReadAging>(std::make_shared<Writer>(), std::make_shared<Comparer>()); }},
			{THIRD_SCRIPT_FULL_NAME,	[]() { return std::make_shared<WriteReadAging>(std::make_shared<Writer>(), std::make_shared<Comparer>()); }},
			{FOURTH_SCRIPT_SHORT_NAME,	[]() { return std::make_shared<EraseAndWriteAging>(std::make_shared<Writer>(), std::make_shared<Comparer>(), std::make_shared<Eraser>()); }},
			{FOURTH_SCRIPT_FULL_NAME,	[]() { return std::make_shared<EraseAndWriteAging>(std::make_shared<Writer>(), std::make_shared<Comparer>(), std::make_shared<Eraser>()); }},
		};
		
		return factoryMap;
    }
};

class DelegatedExecutorFactory : public IExecutorFactory
{
public:
	DelegatedExecutorFactory() {
		factories.push_back(std::make_shared<ExecutorFactory>());
		factories.push_back(std::make_shared<CompositeExecutorFactory>());
	}

	shared_ptr<IExecutor> createExecutor(const string& command) override;

private:
	std::vector<shared_ptr<IExecutorFactory>> factories;
};
