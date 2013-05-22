#ifndef HPP_FICHIER
#define HPP_FICHIER

#include<queue> 

class Fichier
{
    public :
        Fichier();
        ~Fichier();
        std::string toString();

    // Getters & Setters
        std::string getLongName() { return m_LongName; }
        std::string getShortName() { return m_ShortName; }
        std::string getExtension() { return m_Extension; }
        bool getIsDeleted() { return m_Deleted; }
        long getNextCluster() 
            { long value = m_Clusters.front(); m_Clusters.pop(); 
              return value; }

        void setLongName(std::string value) { m_LongName = value; }
        void setShortName(std::string value) { m_ShortName = value; }
        void setExtension(std::string value) { m_Extension = value; }
        void setDeleted() { m_Deleted = true; }
        void addCluster(long cluster) { m_Clusters.push(cluster); }

    private :
        std::string m_LongName;
        std::string m_ShortName;
        std::string m_Extension;
        bool m_Deleted;
        std::queue<long> m_Clusters;
};

#endif

