#ifndef CAR_DETECTOR_H
#define CAR_DETECTOR_H
#include   "GeneralLibs.h"

class car_detector
{
    public:
        inline car_detector();
        inline virtual ~car_detector();

        //PUBLIC METHODS

        //Getters
        inline float GetaXCoorCentre(int Index);
        inline float GetaYCoorCentre(int Index);
        inline float GetaY2CoorCentre(int Index);
        inline float GetWidth(int Index);
        inline cv::Mat GetMatImage();

        //Setters
        inline void SetOriginalImage(cv::Mat New_Matrix);

        //Other methods
        inline int BottomsCentersCoordinatesFiles(std::string Path_to_Coordinates);
        inline void DrawMarkupRectangles(std::string LabelImage);
        inline bool DrawMarkupRectanglesFinalTesting(std::string LabelImage);
        inline bool HaarCascadeDetector(cv::Mat &img, std::vector<cv::Rect> &hnd, std::string path_to_classifier);
        inline void ErrorMatrix();
        inline void CoeficientsMasks();
        inline int FalsePositivesDetector(cv::Rect Rectangle);
        inline std::vector<cv::Point> OpticalFlow(cv::Mat MatCurrent, cv::Mat MatFuture);
        inline cv::Rect CalculateEstimatedRect(int Xcoor, int Ycoor);
        inline std::vector<cv::Rect> DoubleRectanglesCleaner(std::vector<cv::Rect> InitialCollec);
        inline std::vector<cv::Rect> DoubleRectanglesCleaner2(std::vector<cv::Rect> InitialCollec);
        inline int CalculateErrorEstimation(int AuxXCoor, int AuxYCoor, int Width, int Height);
        inline std::vector<cv::Vec4i>BorderLines(cv::Mat CurrentFrame);
        inline bool InsideBorderLines(cv::Rect RectangleToCheck, cv::Vec4i Border, bool LeftRight);


        //PUBLIC VARIABLES
        float ArrayXCoorCentre[5];
        float ArrayYCoorCentre[5];
        float ArrayY2CoorCentre[5];
        float ArrayWidth[5];

        std::vector<cv::Rect> VectorRectanglesHaar;

        int  AverageError = 0;

        cv::Mat MatImage;
        //cv::Mat MatError;

        cv::Mat MatErrorCenters;
        cv::Mat MatErrorBottoms;
        cv::Mat MaskC;
        cv::Mat MaskB;



    protected:

        //Coeficients for Bottom Approach
        float c0 = 250.1359;
        float c1 = -0.2364;
        float c2 = -2.6377;
        float c3 = 0.0003;
        float c4 = 0.0086;
        float c5 = 0.0006;

        //Coeficients for Center Approach
        float c0B = 674.94;
        float c1B = -0.1647;
        float c2B = -8.3746;
        float c3B = 0.0003495;
        float c4B = 0.02797;
        float c5B = 0.0001031;

        float FalsePositiveThreeshold = 0.75; //Average error using the coefficientes sets

    private:


};

#endif // CAR_DETECTOR_H
