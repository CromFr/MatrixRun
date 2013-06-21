#ifndef CONFIGFILE_HPP_INCLUDED
#define CONFIGFILE_HPP_INCLUDED


/**
ConfigFile handler

Original code by Thibaut CHARLES (crom29 @ hotmail.fr)


This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.





Configuration file example:
________________________________
#Custom comment

#the syntax is :
#	[var name][space][value(s)];
#
#[var name] : Name of the var, without any whitespaces into
#[space] : must be a ' ' or a '\t'
#[value(s)] : the value(s) field
#The ';' is very important !


#Examples:
MyVar 10;
MySentense Hello world !;
MyArray 10 56 38 52 46;

#A value can contain any character excepting ';'
# It can even contain '\n' !

MyMultilineValue Dear sir,
This a message for you

Yours faithfully,
Thibaut CHARLES;

________________________________

**/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>




class ConfigFile
{
    public:
	/**
	@brief Errors casted by the ConfigFile
	@see string GetErrorString(enum Error e)
	**/
	enum Error
	{
		no_error,
		file_not_found,
		file_not_opened
	};

    /**
    @brief Get a human readable string for a given error
    @param e The error
    **/
    static std::string GetErrorString(enum Error e)
    {
    	if(e==no_error)return "Success";
    	else if(e==file_not_found)return "File not found";
    	else if(e==file_not_opened)return "The file needs to be opened";
    	return "Unknown error :(";
	}


//================================================================================
    /**
    @brief Default constructor
    @warning You must load the configfile manually before using it
    **/
    ConfigFile();
//================================================================================
	/**
	@brief Load the config file
    @param cPath Path to the config file
    @param bCreateIfNotExists If true, an empty file will be created if it doesn't exist. If false and the config file doesn't exist, will return FILE_NOT_FOUND.
	**/
	enum Error Load(const char* cPath, bool bCreateIfNotExists=false);
//================================================================================
    /**
    @brief Save the current config to the config file
    @arg sNewFile The path to the config file to be written. If void, will save to the config file that has been used in the last Load()
    **/
    enum Error WriteToFile(const char* cNewFile=0);
//================================================================================
//================================================================================
    /**
    @brief Return the string that reprensents the value of the config line. It can be
    @param cName The name of the variable in the config file
    @return examples: "10" or "Thibaut CHARLES" or "10.5 18.0 80.6" for a vector. Returns "" if the row does not exist
    **/
    std::string GetStringValue(const char* cName)const;
//================================================================================
	template<typename T>
	/**
	@brief Get a parsed value from the config file
	@param cName The name of the variable in the config file
	@param n The position of the value in a multivalue field.
	For "MyArray 10 56 38 52 46;",  GetValue<int>("MyArray", 3) will return 52. (the first value is at n=0)
	**/
    T GetValue(const char* cName, int n=0)const
	{
		std::string sValue(GetStringValue(cName)+" ");
		std::string sCurrValue;

		int nPos=0;
		for(unsigned int i=0 ; i<sValue.size() ; i++)
		{
			if(sValue[i]!=' ' && sValue[i]!='\t')
			{
				sCurrValue+=sValue[i];
			}
			else
			{
				if(nPos==n)
				{
					std::stringstream ss(sCurrValue);
					T a;
					ss>>a;
					return a;
				}
				else
				{
					sCurrValue="";
					nPos++;
				}
			}
		}
		return 0;
	}
//================================================================================
	template<typename T>
	/**
	@brief Get an array of parsed values from the config file
	@param cName The name of the variable in the config file
	**/
    std::vector<T> GetArrayValue(const char* cName)const
	{
		std::vector<T> Return;
		std::string sValue(GetStringValue(cName)+" ");

		std::string sCurrValue;
		for(unsigned int i=0 ; i<sValue.size() ; i++)
		{
			char c = sValue[i];
			if(c!=' ')
			{
				sCurrValue+=c;
			}
			else
			{
				std::stringstream ss(sCurrValue);
				T a;
				ss>>a;
				Return.push_back(a);

				sCurrValue="";
			}
		}
		return Return;
	}
//================================================================================
    /**
    @brief Set a value in the loaded config. If the name of the var doesn't exist, it will add it.
    @param cName Name of the variable to set
    @param cValue string value of the variable to set.
    **/
    void SetStringValue(const char* cName, const char* cValue);
//================================================================================
	template<typename T>
	void SetValue(const char* cName, T Value)
	{
		std::stringstream ss;
		ss<<Value;
		SetStringValue(cName, ss.str().c_str());
	}
//================================================================================
//================================================================================
    /**
    @brief Prints the config to cout. Useful for debugging ;)
    **/
    void Print()const
{
    for(unsigned int i=0 ; i<m_Config.size() ; i++)
    {
        std::cout<<m_Config[i].name<<"\t"<<m_Config[i].value<<";"<<std::endl;
    }
}





    private:

    struct ConfigLine
    {
        std::string name;
        std::string value;
    };

    std::string m_sPath;
    std::vector<struct ConfigLine> m_Config;


};

#endif // CONFIGFILE_HPP_INCLUDED
