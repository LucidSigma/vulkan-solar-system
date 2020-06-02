#include "AssetManager.h"

template <typename T>
void AssetManager<T>::Remove(const std::string& name)
{
	m_assets.erase(name);
}