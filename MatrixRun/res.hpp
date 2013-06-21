#ifndef RES_HPP_INCLUDED
#define RES_HPP_INCLUDED

#include <irrlicht.h>

#include <string>
#include <unordered_map>

#include <dirent.h>

namespace res
{
	class material
	{
	public:
		static void LoadFile(irr::video::IVideoDriver* oDriver, const char* cName, const char* cPath);
		static void LoadDir(irr::video::IVideoDriver* oDriver, const char* cPath);
		static irr::video::ITexture* Get(std::string name);

	private:
		material();

		static std::unordered_map<std::string, irr::video::ITexture*> m_materials;
	};


	class model
	{
	public:
		static void LoadFile(irr::scene::ISceneManager* oSM, const char* cName, const char* cPath);
		static void LoadDir(irr::scene::ISceneManager* oSM, const char* cPath);
		static irr::scene::IAnimatedMesh* Get(std::string name);

	private:
		model();

		static std::unordered_map<std::string, irr::scene::IAnimatedMesh*> m_models;
	};





}


#endif // RES_HPP_INCLUDED
