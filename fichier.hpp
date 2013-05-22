#ifndef HPP_FICHIER
#define HPP_FICHIER

class Fichier
{
    public :
        Fichier();
        ~Fichier();

    // Getters & Setters
        std::string getLongName() { return m_LongName; }
        std::string getShortName() { return m_ShortName; }
        std::string getExtension() { return m_Extension; }
        bool getIsDeleted() { return m_Deleted; }

        void setLongName(std::string value) { m_LongName = value; }
        void setShortName(std::string value) { m_ShortName = value; }
        void setExtension(std::string value) { m_Extension = value; }
        void setDeleted() { m_Deleted = true; }

    private :
        std::string m_LongName;
        std::string m_ShortName;
        std::string m_Extension;
        bool m_Deleted;
};

#endif
