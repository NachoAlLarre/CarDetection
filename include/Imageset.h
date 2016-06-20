#ifndef IMAGESET_H
#define IMAGESET_H
#include <sys/types.h>
#include <dirent.h>
#include   "GeneralLibs.h"

class Imageset
{
    public:
        //Constructor/Destructor
        inline Imageset();
        inline virtual ~Imageset();

        //Methods
        inline std::string getCurrentFileName();
        inline int getCurrentImg(cv::Mat& img);
        inline int loadSetFromDir(std::string path_dir, std::string img_format);
        inline std::string getCurrentPath();
        inline int clear();

        //Atributes
        int CurrentIndex;
        int SequenceSize;
        bool InRAM;

        std::string WorkDir;
        std::string MetadataDir;
        std::vector<mvf::Settings> MetadataSequence;
        std::vector<std::string> PathSequence;
        std::vector<cv::Mat> ImageSequence;

};

#endif // IMAGESET_H
