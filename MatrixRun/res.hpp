#ifndef RES_HPP_INCLUDED
#define RES_HPP_INCLUDED

#include <irrlicht.h>
using namespace irr;

#include <iostream>
#include <map>
using namespace std;

#include <dirent.h>

namespace res
{
	class material
	{
	public:
		static void LoadFile(video::IVideoDriver* oDriver, const char* cName, const char* cPath);
		static void LoadDir(video::IVideoDriver* oDriver, const char* cPath);
		static video::ITexture* Get(string name);

	private:
		material();

		static map<string, video::ITexture*> m_materials;
	};


	class model
	{
	public:
		static void LoadFile(scene::ISceneManager* oSM, const char* cName, const char* cPath);
		static void LoadDir(scene::ISceneManager* oSM, const char* cPath);
		static scene::IAnimatedMesh* Get(string name);

	private:
		model();

		static map<string, scene::IAnimatedMesh*> m_models;
	};





}


#endif // RES_HPP_INCLUDED
