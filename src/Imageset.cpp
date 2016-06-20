#include "../include/Imageset.h"

Imageset::Imageset()
{
    //ctor
    InRAM=false;
}

Imageset::~Imageset()
{
    //dtor
}

int Imageset::clear()
 {
    WorkDir = "";
    MetadataDir = "";
    CurrentIndex = -1;
    MetadataSequence.clear();
    PathSequence.clear();
    ImageSequence.clear();
    return 1;
 }

std::string Imageset::getCurrentPath()
{
    if (CurrentIndex < (int)SequenceSize)
        return PathSequence[CurrentIndex];
    else
        return "";
}


int Imageset::loadSetFromDir(std::string path_dir, std::string img_format)
{
    DIR *dir;
    dirent *ent;
    std::string cur_img;

    clear();
    WorkDir = path_dir;
    InRAM = false;

    dir = opendir(path_dir.c_str());
    if (dir == NULL)
    {
        return 0;
    }

    while ((ent = readdir (dir)) != NULL)
    {
        cur_img = ent->d_name;
        if(cur_img.find(img_format)!=std::string::npos)
            PathSequence.push_back(WorkDir + "/" + cur_img);
    }
    closedir (dir);

    SequenceSize = PathSequence.size();
    return 1;
}

std::string Imageset::getCurrentFileName()
{
    if (CurrentIndex == (int)SequenceSize)
        return "";
    std::string path_full = getCurrentPath();
    if(path_full != "")
    {
        return path_full.substr(path_full.find_last_of("/") + 1);
    }
    else
        return "";

}


int Imageset::getCurrentImg(cv::Mat& img)
{
    if (CurrentIndex < (int)SequenceSize)
    {
        if(InRAM)
        {
            img = ImageSequence[CurrentIndex];
        }
        else
        {
            img = cv::imread(getCurrentPath());
        }
        return 1;
    }
    else
        return 0;
}

