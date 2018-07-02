#ifndef BRAIN_H
#define BRAIN_H

#include <QString>

class brain
{
public:
    brain();

    void clear();

    inline void set_user(QString name)
    {
        user = name;
    }
    inline QString get_user()
    {
        return user;
    }

    inline void set_rootdir(QString dirpath)
    {
        rootdir = dirpath;
    }
    inline QString get_rootdir()
    {
        return rootdir;
    }


private:
    QString user;
    QString rootdir;

};

#endif // BRAIN_H
