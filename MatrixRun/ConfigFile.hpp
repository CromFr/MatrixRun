#ifndef CONFIGFILE_HPP_INCLUDED
#define CONFIGFILE_HPP_INCLUDED


/* Exemple de config file

#Commentaire personnalisé...
#Par exemple cette valeur m'intéresse :
MaValeur 10;

#il faut mettre un ';' pour terminer un champ de valeur
#ceci permet de faire ça :
LeNomDeMonApp L'application des héros !;

#Ou encore ceci pour récupérer un vecteur :
MonVecteur 10.5 18.0 80.6;

#Tabulations and spaces are ignored


*/


#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>


class ConfigFile
{
    public:

    ///Default constructor
    ConfigFile();

    ///Create and load the config file
    ConfigFile(std::string sPath);


    ///Load the config file
    void Load(std::string sPath);

    ///Return the string that reprensents the value of the config line
    /// it can be "10" or "Thibaut CHARLES" or even "10.5 18.0 80.6" for a vector
    /// throws int 0 if the row does not exist
    std::string GetValue(std::string sName);

    /// Set a value without saving it to the config file
    void SetValue(std::string sName, std::string sValue);

    ///Save the current config to the config file
    /// sNewFile : if void, will save to the current config file, otherwise save to the specified one
    void SaveToFile(std::string sNewFile="");

    ///Prints the config to cout
    void Print();


    private:

    struct ConfigLine
    {
        int curpos;
        std::string name;
        std::string value;
    };

    std::string m_sPath;
    std::vector<struct ConfigLine> m_Config;

};

#endif // CONFIGFILE_HPP_INCLUDED
