#include<string>

#include "fichier.hpp"

using namespace std;

Fichier::Fichier()
    : m_Deleted(false)
{ }
Fichier::~Fichier() { }

string Fichier::toString()
{
    return 
        (m_Deleted ? "<deleted> " : "          ") +
        m_LongName + " (" + m_ShortName + "." + m_Extension + ")";
}

