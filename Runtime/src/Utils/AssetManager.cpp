#include "virpch.h"
#include "AssetManager.h"
#include <filesystem>

namespace Vireo {
	
	// 静态成员变量初始化
	std::map<std::string, Ref<Texture2D>> AssetManager::s_Textures;
	std::map<std::string, Ref<Model>> AssetManager::s_Models;
	Ref<Texture2D> AssetManager::s_ErrorTexture = nullptr;
	Ref<Texture2D> AssetManager::s_WhiteTexture = nullptr;
	Ref<Texture2D> AssetManager::s_BlackTexture = nullptr;
	Ref<Texture2D> AssetManager::s_FlatNormalTexture = nullptr;
	Ref<Texture2D> AssetManager::s_DefaultMRTexture = nullptr;

	void AssetManager::Init() 
	{
		uint32_t pink = 0xffff00ff;
		s_ErrorTexture = Texture2D::Create(1, 1);
		s_ErrorTexture->SetData(&pink, sizeof(uint32_t));

		uint32_t white = 0xffffffff;
		s_WhiteTexture = Texture2D::Create(1, 1);
		s_WhiteTexture->SetData(&white, sizeof(uint32_t));

		uint32_t black = 0xff000000;
		s_BlackTexture = Texture2D::Create(1, 1);
		s_BlackTexture->SetData(&black, sizeof(uint32_t));

		uint32_t flatNormal = 0xffff8080; // RGBA = (128, 128, 255, 255)
		s_FlatNormalTexture = Texture2D::Create(1, 1);
		s_FlatNormalTexture->SetData(&flatNormal, sizeof(uint32_t));

		uint32_t defaultMR = 0xff00ff00; // RGBA = (0, 255, 0, 255) => roughness=1, metallic=0
		s_DefaultMRTexture = Texture2D::Create(1, 1);
		s_DefaultMRTexture->SetData(&defaultMR, sizeof(uint32_t));

		VIR_CORE_INFO("AssetManager Initialized.");
	}

	Ref<Texture2D> AssetManager::GetTexture(const std::string& path)
	{
		if (path.empty()) return s_WhiteTexture;

		std::string fullPath = NormalizePath(path);

		// 查找缓存
		auto it = s_Textures.find(fullPath);
		if (it != s_Textures.end())
		{
			return it->second;
		}

		// 检查文件是否存在
		if (!std::filesystem::exists(fullPath))
		{
			VIR_CORE_ERROR("AssetManager: Texture file not found at {0}", fullPath);
			return s_ErrorTexture;
		}

		// 加载新纹理
		Ref<Texture2D> texture = Texture2D::Create(fullPath);
		if (texture && texture->IsLoaded())
		{
			s_Textures[fullPath] = texture;
			VIR_CORE_TRACE("AssetManager: Loaded texture {0}", fullPath);
			return texture;
		}

		return s_ErrorTexture;
	}

	Ref<Model> AssetManager::GetModel(const std::string& path)
	{
		std::string fullPath = NormalizePath(path);

		// 查找缓存
		auto it = s_Models.find(fullPath);
		if (it != s_Models.end())
		{
			return it->second;
		}

		// 加载新模型
		if (!std::filesystem::exists(fullPath))
		{
			VIR_CORE_ERROR("AssetManager: Model file not found at {0}", fullPath);
			return nullptr;
		}

		Ref<Model> model = CreateRef<Model>(fullPath);
		s_Models[fullPath] = model;
		VIR_CORE_TRACE("AssetManager: Loaded model {0}", fullPath);
		return model;
	}

	void AssetManager::Clear()
	{
		// 由于使用了 Ref (shared_ptr)，清空 Map 会自动释放不再被引用的 GPU 资源
		s_Textures.clear();
		s_Models.clear();
		VIR_CORE_INFO("AssetManager: All assets cleared.");
	}

	std::string AssetManager::NormalizePath(const std::string& path)
	{
		// 1. 将所有反斜杠 '\\' 转换为正斜杠 '/'，适配 Windows/Linux
		std::string p = path;
		std::replace(p.begin(), p.end(), '\\', '/');

		// 2. 使用 filesystem 获取绝对/标准路径，消除 "." 和 ".." 的歧义
		try {
			std::filesystem::path fsPath(p);
			// 转化为相对于工作目录的标准路径
			return std::filesystem::weakly_canonical(fsPath).string();
		}
		catch (...) {
			return p; // 如果路径非法，原样返回供后续报错
		}
	}
}
