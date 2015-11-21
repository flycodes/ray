/*!
	@file
	@author		George Evmenov
	@date		07/2009
*/

#include <ray/gui_manager.h>
#include <ray/gui_assert.h>
#include <ray/gui_types.h>
#include <ray/ioserver.h>

#include "MyGUI_DataFileStream.h"

_NAME_BEGIN

using namespace MyGUI;

GuiResManager::GuiResManager() 
	: mIsInitialise(false)
{
}

void GuiResManager::initialise()
{
	assert(!mIsInitialise);
	mIsInitialise = true;
}

void GuiResManager::shutdown()
{
	assert(mIsInitialise);
	mIsInitialise = false;
}

IDataStream* GuiResManager::getData(const std::string& _name)
{
	std::string filepath = getDataPath(_name);
	if (filepath.empty())
		return nullptr;

	std::ifstream* stream = new std::ifstream();
	stream->open(filepath.c_str(), std::ios_base::binary);

	if (!stream->is_open())
	{
		delete stream;
		return nullptr;
	}

	DataFileStream* data = new DataFileStream(stream);

	return data;
}

void GuiResManager::freeData(IDataStream* _data)
{
	delete _data;
}

bool GuiResManager::isDataExist(const std::string& _name)
{
	const VectorString& files = getDataListNames(_name);
	return !files.empty();
}

const VectorString& GuiResManager::getDataListNames(const std::string& _pattern)
{
	static VectorString result;
	common::VectorWString wresult;
	result.clear();

	for (VectorArhivInfo::const_iterator item = mPaths.begin(); item != mPaths.end(); ++item)
	{
		common::scanFolder(wresult, (*item).name, (*item).recursive, MyGUI::UString(_pattern).asWStr(), false);
	}

	for (common::VectorWString::const_iterator item = wresult.begin(); item != wresult.end(); ++item)
	{
		result.push_back(MyGUI::UString(*item).asUTF8());
	}

	return result;
}

const std::string& GuiResManager::getDataPath(const std::string& _name)
{
	static std::string path;
	VectorString result;
	common::VectorWString wresult;

	for (VectorArhivInfo::const_iterator item = mPaths.begin(); item != mPaths.end(); ++item)
	{
		common::scanFolder(wresult, (*item).name, (*item).recursive, MyGUI::UString(_name).asWStr(), true);
	}

	for (common::VectorWString::const_iterator item = wresult.begin(); item != wresult.end(); ++item)
	{
		result.push_back(MyGUI::UString(*item).asUTF8());
	}

	if (!result.empty())
	{
		path = result[0];
		if (result.size() > 1)
		{
			MYGUI_PLATFORM_LOG(Warning, "There are several files with name '" << _name << "'. '" << path << "' was used.");
			MYGUI_PLATFORM_LOG(Warning, "Other candidater are:");
			for (size_t index = 1; index < result.size(); index ++)
				MYGUI_PLATFORM_LOG(Warning, " - '" << result[index] << "'");
		}
	}

	return path;
}

void GuiResManager::addResourceLocation(const std::string& _name, bool _recursive)
{
	ArhivInfo info;
	info.name = MyGUI::UString(_name).asWStr();
	info.recursive = _recursive;
	mPaths.push_back(info);
}

_NAME_END