#pragma once
#include "Model/Model.h"


namespace Vireo {

	class AssetManager {
	public:
		static void Init();

		static Ref<Texture2D> GetTexture(const std::string& path);
		static Ref<Texture2D> GetWhiteTexture() { return s_WhiteTexture; }
		static Ref<Texture2D> GetBlackTexture() { return s_BlackTexture; }
		static Ref<Texture2D> GetFlatNormalTexture() { return s_FlatNormalTexture; }
		static Ref<Texture2D> GetDefaultMRTexture() { return s_DefaultMRTexture; }

		static Ref<Model> GetModel(const std::string& path);

		static void Clear();

	private:
		static std::string NormalizePath(const std::string& path);

	private:
		static std::map<std::string, Ref<Texture2D>> s_Textures;
		static std::map<std::string, Ref<Model>> s_Models;

		static Ref<Texture2D> s_ErrorTexture;
		static Ref<Texture2D> s_WhiteTexture;
		static Ref<Texture2D> s_BlackTexture;
		static Ref<Texture2D> s_FlatNormalTexture;
		static Ref<Texture2D> s_DefaultMRTexture;
	};
}
