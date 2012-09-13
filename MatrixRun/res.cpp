#include "res.hpp"

using namespace std;
using namespace irr;


namespace res
{


/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
	map<string, video::ITexture*> material::m_materials;
	void material::LoadDir(video::IVideoDriver* oDriver, const char* cPath)
	{
		DIR* dir = opendir(cPath);
		if(dir!=NULL)
		{
			struct dirent *entity;
			while ((entity = readdir(dir)) != NULL)
			{
				string sEntityPath(cPath);
				sEntityPath+="/";
				sEntityPath+=entity->d_name;
				DIR* isdir = opendir(sEntityPath.c_str());
				if(isdir && string(entity->d_name)!="." && string(entity->d_name)!="..")//it is a directory
				{
					LoadDir(oDriver, sEntityPath.c_str());
				}
				else
				{
					const char* cAllowedFiles[] = {".bmp", ".jpg", ".gif", ".png", ".tga", 0};
					for(int i=0 ; cAllowedFiles[i]!=0 ; i++)
					{
						if(string(entity->d_name).find(cAllowedFiles[i])!=string::npos)
						{
							//Load file
							if(string(entity->d_name).find(string("_n")+cAllowedFiles[i])!=string::npos)
							{
								//Normal map
								video::ITexture* text= oDriver->getTexture(sEntityPath.c_str());
								oDriver->makeNormalMapTexture(text, 9.0f);
								m_materials.insert(	pair<string, video::ITexture*>(string(entity->d_name),		text)	);
							}
							else
							{
								//common texture
								m_materials.insert(	pair<string, video::ITexture*>(string(entity->d_name),		oDriver->getTexture(sEntityPath.c_str()))	);
							}
							break;
						}
					}
				}
			}
		}
		else
			cerr<<"X  ERROR\tCant find the directory (path='"<<cPath<<"')"<<endl;
	}


/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
	video::ITexture* material::Get(string name)
	{
		std::map<string,video::ITexture*>::iterator it = m_materials.find(name);
 		return it->second;
	}





	map<string, scene::IAnimatedMesh*> model::m_models;
/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
	void model::LoadFile(scene::ISceneManager* oSM, const char* cName, const char* cPath)
	{
		m_models.insert(pair<string, scene::IAnimatedMesh*>(string(cName), oSM->getMesh(cPath)));
	}


/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
	void model::LoadDir(scene::ISceneManager* oSM, const char* cPath)
	{
		DIR* dir = opendir(cPath);
		if(dir!=NULL)
		{
			struct dirent *entity;
			while ((entity = readdir(dir)) != NULL)
			{
				string sEntityPath(cPath);
				sEntityPath+="/";
				sEntityPath+=entity->d_name;
				DIR* isdir = opendir(sEntityPath.c_str());
				if(isdir && string(entity->d_name)!="." && string(entity->d_name)!="..")//it is a directory
				{
					LoadDir(oSM, sEntityPath.c_str());
				}
				else
				{
					const char* cAllowedFiles[] = {".3ds", 0};
					for(int i=0 ; cAllowedFiles[i]!=0 ; i++)
					{
						if(string(entity->d_name).find(cAllowedFiles[i])!=string::npos)
						{
							LoadFile(oSM, entity->d_name, sEntityPath.c_str());
							break;
						}
					}
				}
			}
		}
		else
			cerr<<"X  ERROR\tCant find the directory (path='"<<cPath<<"')"<<endl;
	}


/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
	scene::IAnimatedMesh* model::Get(string name)
	{
		std::map<string,scene::IAnimatedMesh*>::iterator it = m_models.find(name);
 		return it->second;
	}








}
