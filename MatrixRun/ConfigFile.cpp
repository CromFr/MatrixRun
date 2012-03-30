
#include "ConfigFile.hpp"

using namespace std;
/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
ConfigFile::ConfigFile()
{

}

/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
ConfigFile::ConfigFile(string sPath)
{
    Load(sPath);
}

/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
void ConfigFile::Load(string sPath)
{
    m_sPath = sPath;

    //Open the file
    ifstream strifConfigFile(sPath.c_str());

    if(!strifConfigFile)
    {
        //Create the file
        ofstream strofConfigFile(sPath.c_str());
        strofConfigFile.close();
    }

    string sName;
    char cChar;
    do
    {
        //Ignore \n, spaces or tabulations
        do
        {
            strifConfigFile.get(cChar);
        }while( (cChar==' ' || cChar=='\t' || cChar=='\n' ) && !strifConfigFile.eof());

        //The cursor is after the first interessant caracter

        if(cChar=='#')//Comments
        {
            //cout<<"It's a comment line"<<endl<<"     ";
            string sLine;
            getline(strifConfigFile, sLine);
            //cout<<sLine<<endl;
        }
        else
        {
            //cout<<"Humm... let's read that !"<<endl;
            struct ConfigLine CurrConfigLine;

            //Getting line name
            string sWord;
            strifConfigFile>>sWord;

            CurrConfigLine.name = cChar+sWord;


            //Ignore spaces and tabulations between name and values
            do
            {
                strifConfigFile.get(cChar);
            }while(cChar==' ' || cChar=='\t');


            while(cChar!=';')
            {
                CurrConfigLine.value+=cChar;
                strifConfigFile.get(cChar);
            }

            m_Config.push_back(CurrConfigLine);
            //cout<<"Adding a line : "<<endl<<"     Name="<<CurrConfigLine.name<<endl<<"     Value(s)="<<CurrConfigLine.value<<endl;

        }

    //cout<<"-------------------------\n";
    }while(!strifConfigFile.eof());

    cout<<"Config file loaded : "<<sPath<<endl;

}



/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
std::string ConfigFile::GetValue(std::string sName)
{
    for(unsigned int i=0 ; i<m_Config.size() ; i++)
    {
        if(m_Config[i].name == sName)
            return m_Config[i].value;
    }
    throw 0;
}



/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
void ConfigFile::SetValue(std::string sName, std::string sValue)
{
    //Find occurences in the config
    bool bIsRegistered = false;
    unsigned int n;
    for(n=0 ; n<m_Config.size() ; n++)
    {
        if(m_Config[n].name == sName)
        {
            bIsRegistered=true;
            break;
        }
    }


    if(bIsRegistered)
    {
        m_Config[n].value = sValue;
    }
    else
    {
        //Prepare the struct
        struct ConfigLine ConfigLine;
        ConfigLine.name = sName;
        ConfigLine.value = sValue;

        m_Config.push_back(ConfigLine);
    }
}



/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
void ConfigFile::SaveToFile(string sNewFile)
{
    //Open the file
    ifstream strRead(m_sPath.c_str());
    if(!strRead)
    {
        cout<<endl<<"Cannot open config file '"<<m_sPath<<"'"<<endl;
        return;
    }

    string sFile="";

    //Creation and initialisation of the check table
    bool bConfigLinesWrited[m_Config.size()];
    for(unsigned int i=0 ; i<m_Config.size() ; i++)
        bConfigLinesWrited[i]=false;



    string sName;
    char cChar;
    do
    {
        //Ignore \n, spaces or tabulations
        do
        {
            strRead.get(cChar);
            sFile+=cChar;
        }while( (cChar==' ' || cChar=='\t' || cChar=='\n' ) && !strRead.eof());

        //The cursor is after the first interessant caracter

        if(cChar=='#')//Comments
        {
            string sLine;
            getline(strRead, sLine);
            sFile+=sLine+'\n';
        }
        else
        {//Interesting line
            //Getting line name
            string sWord;

            strRead>>sWord;
            sName = cChar+sWord;
            sFile+=sWord;

            //record spaces & tabs
            do
            {
                strRead.get(cChar);
                if(cChar==' ' || cChar=='\t')
                    sFile+=cChar;

            }while(cChar==' ' || cChar=='\t');

            //Find occurence in m_Config

            bool bRewrite = false;
            unsigned int n;
            for(n=0 ; n<m_Config.size() ; n++)
            {
                if(m_Config[n].name==sName)
                {
                    bRewrite = true;
                    break;
                }
            }

            if(bRewrite)
            {
                //Apply the change to new file
                sFile += m_Config[n].value+';';

                //Check the corresponding box :
                bConfigLinesWrited[n]=true;

                // and pass threw the old value on the old file
                while(cChar!=';')
                {
                    strRead.get(cChar);
                }
            }
        }

    }while(!strRead.eof());


    bool bAddLines = false;
    for(unsigned int n=0 ; n<m_Config.size() && bAddLines==false ; n++)
    {
        if(!bConfigLinesWrited[n])
            bAddLines = true;
    }


    //Add the addlist !
    if(bAddLines)
    {
        sFile+="\n#=========>Automatically added :";
        for(unsigned int n=0 ; n<m_Config.size() ; n++)
        {
            if(!bConfigLinesWrited[n])
                sFile+="\n"+m_Config[n].name+'\t'+m_Config[n].value+';';
        }
        sFile+="\n#==============================O";
    }



    if(sNewFile=="")
    {
        //Rewriting the entire file
        ofstream strWrite(m_sPath.c_str());
        strWrite<<sFile;
    }
    else
    {
        ofstream strWrite(sNewFile.c_str());
        strWrite<<sFile;
    }



}




/*====================================================================================================================
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
====================================================================================================================*/
void ConfigFile::Print()
{
    cout<<endl<<"=============================================================> CURRENT CONFIG"<<endl;
    for(unsigned int i=0 ; i<m_Config.size() ; i++)
    {
        cout<<m_Config[i].name<<"   \t\t"<<m_Config[i].value<<"   \t;"<<endl;
    }
    cout<<"============================================================================<"<<endl;



}









