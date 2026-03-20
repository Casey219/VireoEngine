#pragma once
#include "Model/Model.h"


namespace Vireo {

	class AssetManager {
	public:
		// 初始化，创建粉色错误贴图和默认白图
		static void Init();

		// 核心接口：纹理
		static Ref<Texture2D> GetTexture(const std::string& path);
		static Ref<Texture2D> GetWhiteTexture() { return s_WhiteTexture; }

		// 核心接口：模型
		static Ref<Model> GetModel(const std::string& path);

		// 清理缓存（例如切换场景时）
		static void Clear();

	private:
		// 辅助函数：标准化路径，防止 assets/a.png 和 ./assets/a.png 重复加载
		static std::string NormalizePath(const std::string& path);

	private:
		static std::map<std::string, Ref<Texture2D>> s_Textures;
		static std::map<std::string, Ref<Model>> s_Models;

		static Ref<Texture2D> s_ErrorTexture;
		static Ref<Texture2D> s_WhiteTexture;
	};
}
