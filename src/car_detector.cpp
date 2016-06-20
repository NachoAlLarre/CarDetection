#include "../include/car_detector.h"


//Constructor
car_detector::car_detector()
{
    MatErrorCenters = cv::Mat::zeros(270, 480, CV_32F);
    MatErrorBottoms = cv::Mat::zeros(270, 480, CV_32F);

    MaskC = cv::Mat::zeros(270, 480, CV_32F);
    MaskB = cv::Mat::zeros(270, 480, CV_32F);

}

car_detector::~car_detector()
{
    //dtor
}

//GETTERS
float car_detector::GetaXCoorCentre(int Index)
{
    return  ArrayXCoorCentre[Index];
}

float car_detector::GetaYCoorCentre(int Index)
{
    return  ArrayYCoorCentre[Index];
}

float car_detector::GetaY2CoorCentre(int Index)
{
    return  ArrayY2CoorCentre[Index];
}

float car_detector::GetWidth(int Index)
{
    return  ArrayWidth[Index];
}

cv::Mat car_detector::GetMatImage()
{
    return MatImage;
}


//SETTERS
void car_detector::SetOriginalImage(cv::Mat New_Matrix)
{
    MatImage = New_Matrix;
}

//OTHER METHODS

//Method to obtain the coordinates of the center and the bottom of the rectangles
//defined in the files of the markup folder
int car_detector::BottomsCentersCoordinatesFiles(std::string Path_to_Coordinates)
{
    std::string Line;
    std::ifstream Myfile (Path_to_Coordinates.c_str());

    std::string StrAux1, StrAux2;
    std::size_t PositionWS;

    int NumSquares = 0;

    std::string mTRUE = "True";
    std::string mFALSE = "False";

    int Width;


    int YcoorAux, XcoorAux, WidthAux;

      if (Myfile.is_open())
      {
        while ( getline (Myfile,Line) )
        {
            if ((Line.compare(mTRUE) != 0)&& (Line.compare(mFALSE) != 0))
            {

              NumSquares++;

              //With these lines we read the markup files and converto to ints the values retrieved
              PositionWS = Line.find(" ");
              StrAux1 = Line.substr (0,PositionWS);
              XcoorAux = atoi(StrAux1.c_str());
              StrAux2 = Line.substr (PositionWS+1);

              PositionWS = StrAux2.find(" ");
              StrAux1 = StrAux2.substr (0,PositionWS);
              YcoorAux = atoi(StrAux1.c_str());
              StrAux2 = StrAux2.substr (PositionWS+1);
              Width =  atoi(StrAux2.c_str());

              ArrayXCoorCentre[NumSquares] = XcoorAux + (Width/2);
              ArrayYCoorCentre[NumSquares] = YcoorAux + Width;
              ArrayY2CoorCentre[NumSquares] = YcoorAux + (Width/2);
              ArrayWidth[NumSquares] = Width;

            }
        }

      }else std::cout << "Unable to open file: " << Path_to_Coordinates;

      Myfile.close();

      return NumSquares;

}

//Method to draw the rectangles defined in the files inside the markup folder
void car_detector::DrawMarkupRectangles(std::string LabelImage)
{
    std::string Line;
    std::ifstream LabelFile (LabelImage.c_str());

    std::string StrAux1, StrAux2;
    std::size_t PositionWS;

    std::string mTRUE = "True";
    std::string mFALSE = "False";

    cv::Mat CoorBot = MatImage.clone();
    cv::Mat CoorCen = MatImage.clone();

    std::string text = "HI";
    int fontFace = CV_FONT_HERSHEY_SIMPLEX;
    double fontScale = 0.5;
    int thickness = 2;
    //cv::Point textOrg(20, 20);
    std::string Number;
    std::stringstream ss;
    if (LabelFile.is_open())
    {
        while ( getline (LabelFile,Line) )
        {

            if ((Line.compare(mTRUE) != 0)&& (Line.compare(mFALSE) != 0))
            {

                  cv::Rect DetectionRect;

                  PositionWS = Line.find(" ");
                  StrAux1 = Line.substr (0,PositionWS);
                  DetectionRect.x = atoi(StrAux1.c_str());
                  StrAux2 = Line.substr (PositionWS+1);

                  PositionWS = StrAux2.find(" ");
                  StrAux1 = StrAux2.substr (0,PositionWS);
                  DetectionRect.y = atoi(StrAux1.c_str());
                  StrAux2 = StrAux2.substr (PositionWS+1);


                  PositionWS = StrAux2.find(" ");
                  StrAux1 = StrAux2.substr (0,PositionWS);
                  DetectionRect.width = atoi(StrAux1.c_str());
                  StrAux2 = StrAux2.substr (PositionWS+1);
                  DetectionRect.height = atoi(StrAux2.c_str());

                  cv::rectangle(MatImage, DetectionRect.tl(), DetectionRect.br(), cv::Scalar(0, 0, 0), 2, CV_AA);

                    Number="(";
                  ss.str(std::string());
                  ss << DetectionRect.x+(DetectionRect.width/2);
                    Number += ss.str()+",";
                    ss.str(std::string());
                  ss << DetectionRect.y+(DetectionRect.width);
                    Number += ss.str()+")";



                  cv::rectangle(CoorBot, DetectionRect.tl(), DetectionRect.br(), cv::Scalar(0, 0, 255), 2, CV_AA);
                  cv::circle(CoorBot, cv::Point(DetectionRect.x+(DetectionRect.width/2),DetectionRect.y+(DetectionRect.width)), 3, cv::Scalar(255,0,0), -1);
                  cv::putText(CoorBot, Number, cv::Point(DetectionRect.x+(DetectionRect.width/2)+5,DetectionRect.y+(DetectionRect.width)+15), fontFace, fontScale, cv::Scalar(255,0,0), thickness,8);

                 Number="(";
                  ss.str(std::string());
                  ss << DetectionRect.x+(DetectionRect.width/2);
                    Number += ss.str()+",";
                    ss.str(std::string());
                  ss << DetectionRect.y+(DetectionRect.width/2);
                    Number += ss.str()+")";

                 cv::rectangle(CoorCen, DetectionRect.tl(), DetectionRect.br(), cv::Scalar(0, 0, 255), 2, CV_AA);
                 cv::circle(CoorCen, cv::Point(DetectionRect.x+(DetectionRect.width/2),DetectionRect.y+(DetectionRect.width/2)), 3, cv::Scalar(255,0,0), -1);
                 cv::putText(CoorCen, Number, cv::Point(DetectionRect.x+(DetectionRect.width/2)+5,DetectionRect.y+(DetectionRect.width/2)+5), fontFace, fontScale, cv::Scalar(255,0,0), thickness,8);


            }else{
                std::cout << Line << '\n';
            }


        }
        std::cout << std::endl;
        LabelFile.close();
    }else std::cout << "Unable to open file: " << LabelImage;

    cv::imshow("Bottom Approach", CoorBot);
    cv::imshow("Center Approach", CoorCen);
    cv::waitKey();

}


bool car_detector::DrawMarkupRectanglesFinalTesting(std::string LabelImage)
{

    std::ifstream f(LabelImage.c_str());
    if(f.good())
    {
        std::string Line;
        std::ifstream LabelFile (LabelImage.c_str());

        std::string StrAux1, StrAux2;
        std::size_t PositionWS;

        std::string mTRUE = "True";
        std::string mFALSE = "False";

        if (LabelFile.is_open())
        {
            while ( getline (LabelFile,Line) )
            {

                if ((Line.compare(mTRUE) != 0)&& (Line.compare(mFALSE) != 0))
                {

                        //std::cout << "Will try this" << std::endl;

                      cv::Rect DetectionRect;

                      PositionWS = Line.find(" ");
                      StrAux1 = Line.substr (0,PositionWS);
                      DetectionRect.x = atoi(StrAux1.c_str());
                      StrAux2 = Line.substr (PositionWS+1);

                      PositionWS = StrAux2.find(" ");
                      StrAux1 = StrAux2.substr (0,PositionWS);
                      DetectionRect.y = atoi(StrAux1.c_str());
                      StrAux2 = StrAux2.substr (PositionWS+1);


                      PositionWS = StrAux2.find(" ");
                      StrAux1 = StrAux2.substr (0,PositionWS);
                      DetectionRect.width = atoi(StrAux1.c_str());
                      StrAux2 = StrAux2.substr (PositionWS+1);

                      PositionWS = StrAux2.find(" ");
                      StrAux1 = StrAux2.substr (0,PositionWS);
                      DetectionRect.height = atoi(StrAux1.c_str());

                     cv::rectangle(MatImage, DetectionRect.tl(), DetectionRect.br(), cv::Scalar(0, 255, 0), 2, CV_AA);

                }else{
                    std::cout << Line << '\n';
                }


            }
            std::cout << std::endl;
            LabelFile.close();

        }else {

            std::cout << "Unable to open file: " << LabelImage;
            return false;
        }

        return true;

    }else
        return false;


}





bool car_detector::HaarCascadeDetector(cv::Mat &img, /*cv::Rect*/std::vector<cv::Rect> &hnd, std::string path_to_classifier)
{
    cv::Mat blr, dsp, aux_dsp, mask, gray, canny;
    cv::CascadeClassifier cascade;
    std::string path_classifier = path_to_classifier;
    double scaleFactor=1.1;
    int minNeighbours, flags;

    flags = 0 //|CV_HAAR_FIND_BIGGEST_OBJECT
    //|CV_HAAR_DO_ROUGH_SEARCH
    |CV_HAAR_SCALE_IMAGE
    | CV_HAAR_DO_CANNY_PRUNING
    ;

    minNeighbours=1;

    if(!cascade.load(path_classifier.c_str()))
    {
        std::cout << "Can't load classifier ";
    }

    //VectorRectanglesHaar.clear();

    //std::vector<cv::Rect> hands;
    cv::cvtColor(img, gray, CV_BGR2GRAY );
    cv::equalizeHist(gray, gray);
    cv::Size min_size, max_size;
    min_size.width=24;
    min_size.height=24;
    max_size.width=std::max(img.cols/2, img.rows/1);
    max_size.height=std::max(img.cols/2, img.rows/1);


    cascade.detectMultiScale(gray, VectorRectanglesHaar, scaleFactor, minNeighbours, flags, min_size, max_size);

    if (VectorRectanglesHaar.empty()) return false;
        hnd=VectorRectanglesHaar;//.at(0);
    return true;

}

// Method to define two mask that will determine the estimated side of a rectangle
// depending on its position
void car_detector::CoeficientsMasks()
{
    std::string Line;
    std::ifstream MyFileBottoms ("BottomsPoints.txt");
    std::ifstream MyFileCenters ("CentersPoints.txt");

    std::string StrAux1, StrAux2;
    std::size_t PositionWS;



    float EstimatedSize,  EstimatedError;

    int PrintingYCoor;

    int XcoorAux, YcoorAux, AuxWidth;

    if (MyFileBottoms.is_open())
    {
        while ( getline (MyFileBottoms,Line) )
        {

              PositionWS = Line.find(" ");
              StrAux1 = Line.substr (0,PositionWS);
              XcoorAux = atoi(StrAux1.c_str());
              StrAux2 = Line.substr (PositionWS+1);

              PositionWS = StrAux2.find(" ");
              StrAux1 = StrAux2.substr (0,PositionWS);
              YcoorAux = atoi(StrAux1.c_str());
              StrAux1 = StrAux2.substr (PositionWS+1);
              AuxWidth = atoi(StrAux1.c_str());

              PrintingYCoor = YcoorAux - (AuxWidth/2);

              EstimatedSize = c0 + (c1 * XcoorAux) + (c2 * YcoorAux)+ (c3 * XcoorAux * XcoorAux)+  (c4 * YcoorAux * YcoorAux) + (c5 * XcoorAux*YcoorAux);

              EstimatedError = abs(AuxWidth-EstimatedSize);

            if (MatErrorBottoms.at<float>(PrintingYCoor,XcoorAux)<EstimatedError/10.f)
                MatErrorBottoms.at<float>(PrintingYCoor,XcoorAux) = EstimatedError/10.f;


        }
    }
    else std::cout << "Unable to open Bottoms file";

    if (MyFileCenters.is_open())
    {
        while ( getline (MyFileCenters,Line) )
        {

              PositionWS = Line.find(" ");
              StrAux1 = Line.substr (0,PositionWS);
              XcoorAux = atoi(StrAux1.c_str());
              StrAux2 = Line.substr (PositionWS+1);

              PositionWS = StrAux2.find(" ");
              StrAux1 = StrAux2.substr (0,PositionWS);
              YcoorAux = atoi(StrAux1.c_str());
              StrAux1 = StrAux2.substr (PositionWS+1);
              AuxWidth = atoi(StrAux1.c_str());

              EstimatedSize = c0B + (c1B * XcoorAux) + (c2B * YcoorAux)+ (c3B * XcoorAux * XcoorAux)+  (c4B * YcoorAux * YcoorAux) + (c5B * XcoorAux*YcoorAux);

              EstimatedError = abs(AuxWidth-EstimatedSize);

              if (MatErrorCenters.at<float>(YcoorAux,XcoorAux)<EstimatedError/10.f)
                MatErrorCenters.at<float>(YcoorAux,XcoorAux) = EstimatedError/10.f;


        }
    }
    else std::cout << "Unable to open Centers file";


    MyFileBottoms.close();
    MyFileCenters.close();

    //APPLY 2D FILTER TO THE COULD OF CENTERS

    cv::Mat k1(8,8,CV_32F);
    k1=cv::Scalar(1);


    //Auxiliar matrixs to apply the 2D filters
    cv::Mat v_mb;
    cv::Mat v_mc;

    cv::filter2D(MatErrorBottoms, v_mb, CV_32F, k1);
    cv::filter2D(v_mb, v_mb, CV_32F, k1);
    cv::filter2D(v_mb, v_mb, CV_32F, k1);

    cv::filter2D(MatErrorCenters, v_mc, CV_32F, k1);
    cv::filter2D(v_mc, v_mc, CV_32F, k1);
    cv::filter2D(v_mc, v_mc, CV_32F, k1);

    //Final Mask


    MaskC = v_mb > v_mc;
    MaskB = v_mb < v_mc;

    //270, 480

    /*
    for(int r=0 ; r<MaskC.rows ; r++)
    {
        for(int c=0 ; c<MaskC.cols ; c++)
        {
             if((v_mb.at<float>(r,c)>0) && (v_mc.at<float>(r,c)>0))
             {
                 if(v_mc.at<float>(r,c) > v_mb.at<float>(r,c))
                    MaskC.at<float>(r,c) = 1;
                 if(v_mc.at<float>(r,c) < v_mb.at<float>(r,c))
                    MaskB.at<float>(r,c) = 1;
             }
        }

    }*/
    // MaskC.at<float>(AuxYCoor,AuxXCoor)



}




//This method is used to filter false positives

int car_detector::FalsePositivesDetector(cv::Rect PositiveRectangle)
{
    int IsItValidPositive = 0;

    float EstimatedSize,  EstimatedErrorWidth, EstimatedErrorHeight;

    int AuxXCoor, AuxYCoor, BiggestError = 0;

    float ConfidenceAux;


    //We calculate the center of the rectangle
    AuxXCoor = PositiveRectangle.x + ((PositiveRectangle.width)/2);
    AuxYCoor = PositiveRectangle.y + ((PositiveRectangle.height)/2);

    //std::cout << "MaskC.at<float>(AuxYCoor,AuxXCoor)" << MaskC.at<uchar>(AuxYCoor,AuxXCoor) << " XCoor "<< AuxXCoor << " YCoor "<< AuxYCoor<<std::endl;
    //std::cout << "MaskB.at<float>(AuxYCoor,AuxXCoor)" << MaskB.at<uchar>(AuxYCoor,AuxXCoor) << " XCoor "<< AuxXCoor << " YCoor "<< AuxYCoor<<std::endl;



    if(MaskC.at<uchar>(AuxYCoor,AuxXCoor)>0)//The center of the square is into the CentersApproach Mask
    {
        //The second set of coefficients is used to estimate the size of the rectangle
        EstimatedSize = c0B + (c1B * AuxXCoor) + (c2B * AuxYCoor)+ (c3B * AuxXCoor * AuxXCoor)+  (c4B * AuxYCoor * AuxYCoor) + (c5B * AuxXCoor*AuxYCoor);
       // std::cout << "EstimatedSize" << EstimatedSize << std::endl;
        EstimatedErrorWidth = abs(PositiveRectangle.width-EstimatedSize);
        EstimatedErrorHeight = abs(PositiveRectangle.height-EstimatedSize);

        //We calculate what side has the BiggestError
        if(EstimatedErrorWidth > EstimatedErrorHeight)
        {
            BiggestError = EstimatedErrorWidth;
        }else
        {
            BiggestError = EstimatedErrorHeight;
        }

        //If so, it is a valid rectangle
        if(BiggestError < (EstimatedSize*FalsePositiveThreeshold))
        {
            //std::cout << "EstimatedSize: " << EstimatedSize <<  " PositiveRectangle.width: " << PositiveRectangle.width << " PositiveRectangle.height: " << PositiveRectangle.height <<std::endl;
            //std::cout << "(EstimatedSize*FalsePositiveThreeshold): " << (EstimatedSize*FalsePositiveThreeshold) <<  " EstimatedErrorWidth: " << EstimatedErrorWidth << " EstimatedErrorHeight: " << EstimatedErrorHeight <<std::endl <<std::endl;

            ConfidenceAux = (float)(1 - ((BiggestError+1)/(EstimatedSize*FalsePositiveThreeshold)));
            IsItValidPositive = 255 * ConfidenceAux;
           // std::cout << "Confidence" << IsItValidPositive << std::endl;
        }
          // IsItValidPositive = 255 * (1/((BiggestError+1)/((EstimatedSize*FalsePositiveThreeshold)))); //To calculate the confidence

        //std::cout << "Biggest Error" << BiggestError << std::endl;


    }else if(MaskB.at<uchar>(AuxYCoor,AuxXCoor)>0) //The center of the square is into the BottomsApproach Mask
    {

        AuxYCoor += ((PositiveRectangle.height)/2);

        //The first set of coefficients is used to estimate the size of the rectangle
        EstimatedSize = c0 + (c1 * AuxXCoor) + (c2 * AuxYCoor)+ (c3 * AuxXCoor * AuxXCoor)+  (c4 * AuxYCoor * AuxYCoor) + (c5 * AuxXCoor*AuxYCoor);
       // std::cout << "EstimatedSize" << EstimatedSize << std::endl;
        EstimatedErrorWidth = abs(PositiveRectangle.width-EstimatedSize);
        EstimatedErrorHeight = abs(PositiveRectangle.height-EstimatedSize);

        //We calculate what side has the BiggestError
        if(EstimatedErrorWidth > EstimatedErrorHeight)
        {
            BiggestError = EstimatedErrorWidth;
        }else
        {
            BiggestError = EstimatedErrorHeight;
        }

        //If so, it is a valid rectangle
        if(BiggestError < (EstimatedSize*FalsePositiveThreeshold))
        {
           //std::cout << "EstimatedSize: " << EstimatedSize <<  " PositiveRectangle.width: " << PositiveRectangle.width << " PositiveRectangle.height: " << PositiveRectangle.height <<std::endl;
           // std::cout << "(EstimatedSize*FalsePositiveThreeshold): " << (EstimatedSize*FalsePositiveThreeshold) <<  " EstimatedErrorWidth: " << EstimatedErrorWidth << " EstimatedErrorHeight: " << EstimatedErrorHeight <<std::endl <<std::endl;

            ConfidenceAux = (float)(1 - ((BiggestError+1)/(EstimatedSize*FalsePositiveThreeshold)));
            //std::cout << "ConfidenceAux" << (float)ConfidenceAux << std::endl;
            IsItValidPositive = 255 * ConfidenceAux;
           // std::cout << "Confidence" << IsItValidPositive << std::endl;
        }

        //std::cout << "Biggest Error" << BiggestError << std::endl;

    }else
    {
       // std::cout << "Center Outside of the masks" << std::endl;
    }



    return IsItValidPositive;
}

//For the optical flow

std::vector<cv::Point> car_detector::OpticalFlow(cv::Mat MatCurrent, cv::Mat MatFuture)
{
    std::vector<cv::Point> FeaturesNow, FeaturesNext;

    std::vector<char> FeaturesFound;
    std::vector<int> Errors;

    cv::goodFeaturesToTrack(MatCurrent,FeaturesNow, 15, 0.01, 10);

    cv::calcOpticalFlowPyrLK(MatCurrent, MatFuture, FeaturesNow, FeaturesNext, FeaturesFound, Errors);

    return FeaturesNext;
}


cv::Rect car_detector::CalculateEstimatedRect(int AuxXCoor, int AuxYCoor)
{
    cv::Rect ReturnRect;
    int  EstimatedSize;
    int AuxYCoor2;

    //if(MaskC.at<uchar>(AuxYCoor,AuxXCoor)>0)//The center of the square is into the CentersApproach Mask
   // {
        //The second set of coefficients is used to estimate the size of the rectangle
        EstimatedSize = c0B + (c1B * AuxXCoor) + (c2B * AuxYCoor)+ (c3B * AuxXCoor * AuxXCoor)+  (c4B * AuxYCoor * AuxYCoor) + (c5B * AuxXCoor*AuxYCoor);
       // std::cout << "EstimatedSize" << EstimatedSize << std::endl;
       ReturnRect.x= AuxXCoor - (EstimatedSize/2);
       ReturnRect.y= AuxYCoor - (EstimatedSize/2);
       ReturnRect.width= EstimatedSize;
       ReturnRect.height= EstimatedSize;


    return ReturnRect;
}

int car_detector::CalculateErrorEstimation(int AuxXCoor, int AuxYCoor, int Width, int Height)
{
    int  EstimatedSize;
    int Error;
    int AuxYCoor2;
    int BiggestError;
    int EstimatedErrorWidth, EstimatedErrorHeight;

    if(MaskC.at<uchar>(AuxYCoor,AuxXCoor)>0)//The center of the square is into the CentersApproach Mask
   {
        //The second set of coefficients is used to estimate the size of the rectangle
        EstimatedSize = c0B + (c1B * AuxXCoor) + (c2B * AuxYCoor)+ (c3B * AuxXCoor * AuxXCoor)+  (c4B * AuxYCoor * AuxYCoor) + (c5B * AuxXCoor*AuxYCoor);
        EstimatedErrorWidth = abs(Width-EstimatedSize);
        EstimatedErrorHeight = abs(Height-EstimatedSize);

        if(EstimatedErrorWidth > EstimatedErrorHeight)
        {
            return EstimatedErrorWidth;
        }else
        {
            return EstimatedErrorHeight;
        }

    }else if(MaskB.at<uchar>(AuxYCoor,AuxXCoor)>0) //The center of the square is into the BottomsApproach Mask
    {
        AuxYCoor2 = AuxYCoor + (Height/2);
        EstimatedSize = c0 + (c1 * AuxXCoor) + (c2 * AuxYCoor)+ (c3 * AuxXCoor * AuxXCoor)+  (c4 * AuxYCoor * AuxYCoor) + (c5 * AuxXCoor*AuxYCoor);
        EstimatedErrorWidth = abs(Width-EstimatedSize);
        EstimatedErrorHeight = abs(Height-EstimatedSize);

        if(EstimatedErrorWidth > EstimatedErrorHeight)
        {
            return EstimatedErrorWidth;
        }else
        {
            return EstimatedErrorHeight;
        }

    }

}

std::vector<cv::Rect> car_detector::DoubleRectanglesCleaner(std::vector<cv::Rect> InitialCollec)
{
   std::vector<cv::Rect> ReturnRect;
   cv::Rect AuxRect;
   cv::Rect AuxRect2;
   cv::Rect SumOfRects;

   int H, W;
   int Size1, Size2, SizeBoth;

   std::vector<int> DiscartedRects;


   //std::cout <<  std::endl << "Checking for collision in this rectangles: " << InitialCollec.size()<<std::endl;


   bool NotDiscarted, NotRemoved;
   int NumCollisions = 0;

   for(int ind = 0; ind < InitialCollec.size(); ind++)
   {
        NotDiscarted = true;
        NotRemoved = true;



        for(int k=0; k < DiscartedRects.size(); k++)
        {
            if(DiscartedRects.at(k)==ind)
                NotRemoved = false;

        }
        if(NotRemoved)
        {

            AuxRect = InitialCollec.at(ind);

            for(int Subind = (ind+1); Subind < InitialCollec.size(); Subind++)
            {
                AuxRect2  = InitialCollec.at(Subind);

               // cv::Mat PrintingMatAux2 = MatImage.clone();
              //  cv::Mat RectanglesPhase = MatImage.clone();

                //Check what rectangle is more in the left side

                //PrintingMatAux2
               // cv::rectangle(PrintingMatAux2, AuxRect.tl(),AuxRect.br(), cv::Scalar(255, 0, 0), 2, CV_AA);
               // cv::rectangle(PrintingMatAux2, AuxRect2.tl(), AuxRect2.br(), cv::Scalar(255, 0, 0), 2, CV_AA);

                //cv::rectangle(RectanglesPhase, AuxRect.tl(),AuxRect.br(), cv::Scalar(255, 0, 0), 2, CV_AA);
               // cv::rectangle(RectanglesPhase, AuxRect2.tl(), AuxRect2.br(), cv::Scalar(255, 0, 0), 2, CV_AA);

                //std::cout << AuxRect.tl().x << " " <<  AuxRect.tl().y << std::endl;
                //std::cout << AuxRect.x << " " <<  AuxRect.y << std::endl;


                //TL corner X COOR
                if((AuxRect.tl().x <AuxRect2.tl().x))
                    SumOfRects.x = AuxRect.x;
                else
                    SumOfRects.x = AuxRect2.x;

                //TL corner Y COOR
                if((AuxRect.tl().y <AuxRect2.tl().y))
                    SumOfRects.y = AuxRect.y;
                else
                    SumOfRects.y = AuxRect2.y;

                //BR corner X
                if((AuxRect.br().x >AuxRect2.br().x))
                    SumOfRects.width = ((AuxRect.x + AuxRect.width)-SumOfRects.x);
                else
                    SumOfRects.width = ((AuxRect2.x + AuxRect2.width)-SumOfRects.x);

                //BR corner Y COOR
                if((AuxRect.br().y >AuxRect2.br().y))
                    SumOfRects.height = ((AuxRect.y + AuxRect.height)-SumOfRects.y);
                else
                    SumOfRects.height = ((AuxRect2.y + AuxRect2.height)-SumOfRects.y);


               // cv::rectangle(PrintingMatAux2,  SumOfRects.tl(),  SumOfRects.br(), cv::Scalar(0, 0, 255), 2, CV_AA);

                W = AuxRect.width+AuxRect2.width;
                H = AuxRect.height+AuxRect2.height;


                if((W>SumOfRects.width)&&(H>SumOfRects.height))
                {

                    //std::cout << "There is collision" << std::endl;
                    NumCollisions++;
                    //There is a Collision

                    //We check if this collision should be considered
                    Size1 = AuxRect.width * AuxRect.height;
                    Size2 = AuxRect2.width * AuxRect2.height;

                    if((((W-SumOfRects.width)*(H-SumOfRects.height)) >  Size1*0.25) || (((W-SumOfRects.width)*(H-SumOfRects.height)) >  Size2*0.25) )
                    {
                        //The collision can't be ignored
                        if((CalculateErrorEstimation(AuxRect.x, AuxRect.y,AuxRect.width, AuxRect.height))>(CalculateErrorEstimation(AuxRect2.x,AuxRect2.y,AuxRect2.width, AuxRect2.height)))
                         {
                             //The first rectangle is discarted

                            // cv::rectangle(PrintingMatAux2, AuxRect.tl(),AuxRect.br(), cv::Scalar(0, 100, 100), 2, CV_AA);
                             NotDiscarted = false;

                         }
                         else
                         {
                             //The second rectangle is ignored
                            // cv::rectangle(PrintingMatAux2, AuxRect2.tl(),AuxRect2.br(), cv::Scalar(0, 100, 100), 2, CV_AA);
                             DiscartedRects.push_back(Subind);

                         }

                    }

                }else{
                    //std::cout << "No collision between " <<ind << " " << Subind<< std::endl;
                }

               // cv::imshow("Collision", PrintingMatAux2);
               // cv::imshow("Rectangles phase", RectanglesPhase);
               // cv::waitKey();


            }

            if(NotDiscarted)
                ReturnRect.push_back(AuxRect);

        }

   }

  // std::cout << "Number of collisions: " << NumCollisions << std::endl;

   return ReturnRect;

}

std::vector<cv::Vec4i> car_detector::BorderLines(cv::Mat CurrentFrame)
{

     cv::Mat dst, cdst = cv::Mat::zeros(270, 480, CV_32F);

     cv::Canny(CurrentFrame, dst, 50, 200, 3);

     cvtColor(dst, cdst, CV_GRAY2BGR);

     std::vector<cv::Vec4i> lines;
     std::vector<cv::Vec4i> UsefulLinesLeft;
     std::vector<cv::Vec4i> UsefulLinesRight;
     std::vector<cv::Vec4i> ReturnLines;

     cv::Vec4i AuxiliarLine;

     int MinYL = 270;
     int MinYR = 270;

    cv::HoughLinesP(dst, lines, 1, CV_PI/180, 50, 50, 10 );

    for( size_t ki = 0; ki < lines.size(); ki++ )
    {
          cv::Vec4i l = lines[ki];

          float d, Aux0, Aux1, Aux2, Aux3 , l2, l1;
          int Y1, X2;


          if((l[1]>110) && (l[3]>110))//To filter the upper lines, normally belonging to trees and landscape
          {

            cv::line( cdst, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(200,200,200), 3, CV_AA);

            if(abs(l[1]-l[3])>20) // To filter almost  horizontal lines
            {
                 cv::line( cdst, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0,255,0), 3, CV_AA);

                //FOR THE LOWER PART
                Aux0 = (float)  l[0]; Aux2= (float)  l[2]; Aux3= (float)  l[3]; Aux1 = (float)  l[1];
                d = ((float)(abs(  Aux3-  Aux1)))/abs(  Aux0-  Aux2);


                 //FOR THE LEFT SIDE BORDER
                 if((l[0] < 270)&&(l[2] < 270))
                 {

                     if(l[0]>l[2])
                        l1 = (float)l[2];
                     else
                        l1 = (float)l[0];

                     l2 = float (d*l1);


                     if(l[0]>l[2])
                     {

                         Y1 = l[3] + l2;

                         //cv::line( cdst, cv::Point(0, Y1), cv::Point(l[2], l[3]), cv::Scalar(0,255,255), 3, CV_AA);
                     }
                     else
                     {
                         Y1 = l[1] + l2;


                         //cv::line( cdst, cv::Point(0, Y1), cv::Point(l[0], l[1]), cv::Scalar(0,255,255), 3, CV_AA);
                     }

                    //FOR THE UPPER

                    if(l[0]>l[2])
                        l2 = (float)(l[1]-120);
                     else
                        l2 = (float)(l[3]-120);

                     l1 = float (l2/d);



                     if(l[0]>l[2])
                     {

                          X2 = l[0] + l1;


                        // cv::line( cdst, cv::Point(l[0], l[1]), cv::Point( X2 , 120), cv::Scalar(0,255,255), 3, CV_AA);
                     }
                     else
                     {
                          X2 = l[2] + l1;


                         //cv::line( cdst, cv::Point(l[2], l[3]), cv::Point( X2 , 120), cv::Scalar(0,255,255), 3, CV_AA);
                     }

                     //Here is where the line is stored in the useful

                    AuxiliarLine[0] = 0;
                    AuxiliarLine[1] = Y1;
                    AuxiliarLine[2] = X2;
                    AuxiliarLine[3] = 120;

                    if(Y1<MinYL)
                        MinYL = Y1;

                     UsefulLinesLeft.push_back(AuxiliarLine);

                 }


                 //FOR THE RIGHT SIDE BORDER
                 if((l[0] > 220)&&(l[2] > 220))
                 {

                     //FOR THE LOWER PART

                     if(l[0]>l[2])
                        l1 = (float)(480-l[0]);
                     else
                        l1 = (float)(480 -l[2]);

                     l2 = float (d*l1);


                     if(l[0]>l[2])
                     {

                         Y1 = l[1] + l2;


                         //cv::line( cdst, cv::Point(480, Y1), cv::Point(l[0], l[1]), cv::Scalar(0,255,255), 3, CV_AA);
                     }
                     else
                     {
                         Y1 = l[3] + l2;


                         //cv::line( cdst, cv::Point(480, Y1), cv::Point(l[2], l[3]), cv::Scalar(0,255,255), 3, CV_AA);
                     }


                    //FOR THE UPPER PART

                    if(l[0]>l[2])
                        l2 = (float)(l[3]-120);
                     else
                        l2 = (float)(l[1]-120);

                     l1 = float (l2/d);



                     if(l[0]>l[2])
                     {

                          X2 = l[2] - l1;


                         //cv::line( cdst, cv::Point(l[2], l[3]), cv::Point( X2 , 120), cv::Scalar(0,255,255), 3, CV_AA);
                     }
                     else
                     {
                          X2 = l[0] - l1;


                         //cv::line( cdst, cv::Point(l[0], l[1]), cv::Point( X2 , 120), cv::Scalar(0,255,255), 3, CV_AA);
                     }


                    AuxiliarLine[0] = X2;
                    AuxiliarLine[1] = 120;
                    AuxiliarLine[2] = 480;
                    AuxiliarLine[3] = Y1;

                    if(Y1<MinYR)
                        MinYR = Y1;

                    UsefulLinesRight.push_back(AuxiliarLine);

                 }


            }

          }



    }

     int AverageXLeft = 0;
     int AverageYLeft = 0;
     int AverageXRight = 0;
     int AverageYRight = 0;

     int ContUL = 0;
     int ContUR = 0;


    //Here time from printing
    for(int loopUL = 0; loopUL< UsefulLinesRight.size(); loopUL++ )
    {
        AuxiliarLine = UsefulLinesRight.at(loopUL);

        if(abs(AuxiliarLine[3]-MinYR)<15)
        {
            AverageYRight += AuxiliarLine[3];
            AverageXRight += AuxiliarLine[0];
            ContUR++;
        }

    }

     if(ContUR>0)
    {
        AuxiliarLine[0] = AverageXRight/ContUR;
        AuxiliarLine[1] = 120;
        AuxiliarLine[2] = 480;
        AuxiliarLine[3] = AverageYRight/ContUR;


    }else{

        AuxiliarLine[0] = 0;
        AuxiliarLine[1] = 0;
        AuxiliarLine[2] = 0;
        AuxiliarLine[3] = 0;

    }

    //Fist one -> Right
    ReturnLines.push_back( AuxiliarLine);

    for(int loopUL = 0; loopUL< UsefulLinesLeft.size(); loopUL++ )
    {
        AuxiliarLine = UsefulLinesLeft.at(loopUL);

        if(abs(AuxiliarLine[1]-MinYL)<15)
        {
            AverageYLeft += AuxiliarLine[1];
            AverageXLeft += AuxiliarLine[2];
            ContUL++;
        }

    }

    if(ContUL>0)
    {
        AuxiliarLine[0] = 0;
        AuxiliarLine[1] = AverageYLeft/ContUL;
        AuxiliarLine[2] = AverageXLeft/ContUL;
        AuxiliarLine[3] = 120;


    }else{

        AuxiliarLine[0] = 0;
        AuxiliarLine[1] = 0;
        AuxiliarLine[2] = 0;
        AuxiliarLine[3] = 0;

    }

    //Second One Left
    ReturnLines.push_back(AuxiliarLine);


    return ReturnLines;

}

bool car_detector::InsideBorderLines(cv::Rect RectangleToCheck, cv::Vec4i Border, bool LeftRight)
{
   int x0, y0;
   float d;

   int Y1;

   cv::Mat Pic = MatImage.clone();

   if(LeftRight) //Left side
   {
       //std::cout << "We are in Left" << std::endl;
       x0 = RectangleToCheck.x +  RectangleToCheck.width;
       y0 = RectangleToCheck.y +  RectangleToCheck.height;


       d = ((float)(Border[1]-Border[3]))/(Border[2]-Border[0]);
       Y1 = (d* x0) + y0;

       if(Y1>Border[1])
            return true;


   }else{ //Right side

       // std::cout << "We are in Right" << std::endl;

        x0 = RectangleToCheck.x;
        y0 = RectangleToCheck.y + RectangleToCheck.height;

        d = ((float)(Border[3]-Border[1]))/(Border[2]-Border[0]);
        Y1 = (d* (480-x0) )+ y0;

       if(Y1>Border[3])
            return true;


   }

    return false;
}




///////REMOVE LATER, JUST FOR COPY AND PASTE STUFF

/*
std::vector<cv::Rect> car_detector::DoubleRectanglesCleaner2(std::vector<cv::Rect> InitialCollec)
{
   std::vector<cv::Rect> ReturnRect;
   cv::Rect AuxRect;
   cv::Rect AuxRect2;
   cv::Rect SumOfRects

   std::vector<int> DiscartedRects, ValidRects, AlreadyValidRects;

   std::cout <<  std::endl << "**Checking for collision in this rectangles: " << InitialCollec.size()<<std::endl;



   bool NotDiscarted, NotValidRectsYet, NoCollision;

   for(int ind = 0; ind < InitialCollec.size(); ind++)
   {
       std::cout << ind <<std::endl;
       NotDiscarted = true;
       NoCollision = true;

       //Check if this rectangle has been discarted
       for(int kk=0; kk < DiscartedRects.size() ; kk++)
       {
           if(DiscartedRects.at(kk)==ind)
                NotDiscarted= false;
       }

       if(NotDiscarted)
       {

           AuxRect =  InitialCollec.at(ind);

           for(int Subind = 0; Subind < InitialCollec.size(); Subind++)
           {

                //Check if this rectangle has been discarted
               for(int kkk=0; kkk < DiscartedRects.size() ; kkk++)
               {
                   if(DiscartedRects.at(kkk)==Subind)
                        NotDiscarted= false;
               }

                //To don't compare the rectangle with itself
                if((ind!=Subind)&&(NotDiscarted))
                {
                    SubAuxRect = InitialCollec.at(Subind);

                    if((AuxRect.x < SubAuxRect.x) && ((AuxRect.x+AuxRect.width)> (SubAuxRect.x)))//If true, there is a horizontal match
                    {
                            if((AuxRect.y < SubAuxRect.y) && ((AuxRect.y+AuxRect.height)> (SubAuxRect.y))) //If true, there is also vertical match
                            {
                                 //At this point we can be sure both rectangles are overlapping. It is necessary to know which one is better
                                 //We check which one of them gives a better result in the estimated rectangle

                                 std::cout << "There was a collision between: "<< ind <<" and "<< Subind<< std::endl;

                                 NoCollision=false;

                                 if((CalculateErrorEstimation(AuxRect.x, AuxRect.y,AuxRect.width, AuxRect.height))>(CalculateErrorEstimation(SubAuxRect.x,SubAuxRect.y,SubAuxRect.width, SubAuxRect.height)))
                                 {
                                     DiscartedRects.push_back(ind);
                                     ValidRects.push_back(Subind);
                                 }
                                 else
                                 {
                                     DiscartedRects.push_back(Subind);
                                     ValidRects.push_back(ind);
                                 }
                            }
                    }else if((AuxRect.x < (SubAuxRect.x+SubAuxRect.width)) && ((AuxRect.x+AuxRect.width)> (SubAuxRect.x+SubAuxRect.width)))//If true, there is a horizontal match
                    {
                            if((AuxRect.y < SubAuxRect.y) && ((AuxRect.y+AuxRect.height)> (SubAuxRect.y))) //If true, there is also vertical match
                            {
                                 //At this point we can be sure both rectangles are overlapping. It is necessary to know which one is better
                                 //We check which one of them gives a better result in the estimated rectangle

                                 std::cout << "There was a collision between: "<< ind <<" and "<< Subind<< std::endl;

                                 NoCollision=false;

                                 if((CalculateErrorEstimation(AuxRect.x, AuxRect.y,AuxRect.width, AuxRect.height))>(CalculateErrorEstimation(SubAuxRect.x,SubAuxRect.y,SubAuxRect.width, SubAuxRect.height)))
                                 {
                                     DiscartedRects.push_back(ind);
                                     ValidRects.push_back(Subind);
                                 }
                                 else
                                 {
                                     DiscartedRects.push_back(Subind);
                                     ValidRects.push_back(ind);
                                 }
                            }
                    }else if((AuxRect.x < SubAuxRect.x) && ((AuxRect.x+AuxRect.width)> (SubAuxRect.x)))//If true, there is a horizontal match
                    {
                            if((AuxRect.y < (SubAuxRect.y+SubAuxRect.height)) && ((AuxRect.y+AuxRect.height)> (SubAuxRect.y+SubAuxRect.height))) //If true, there is also vertical match
                            {
                                 //At this point we can be sure both rectangles are overlapping. It is necessary to know which one is better
                                 //We check which one of them gives a better result in the estimated rectangle

                                 std::cout << "There was a collision between: "<< ind <<" and "<< Subind<< std::endl;

                                 NoCollision=false;

                                 if((CalculateErrorEstimation(AuxRect.x, AuxRect.y,AuxRect.width, AuxRect.height))>(CalculateErrorEstimation(SubAuxRect.x,SubAuxRect.y,SubAuxRect.width, SubAuxRect.height)))
                                 {
                                     DiscartedRects.push_back(ind);
                                     ValidRects.push_back(Subind);
                                 }
                                 else
                                 {
                                     DiscartedRects.push_back(Subind);
                                     ValidRects.push_back(ind);
                                 }
                            }
                    }else if((AuxRect.x < (SubAuxRect.x+SubAuxRect.width)) && ((AuxRect.x+AuxRect.width)> ((SubAuxRect.x+SubAuxRect.width))))//If true, there is a horizontal match
                    {
                            if((AuxRect.y < (SubAuxRect.y+SubAuxRect.height)) && ((AuxRect.y+AuxRect.height)> ((SubAuxRect.y+SubAuxRect.height)))) //If true, there is also vertical match
                            {
                                 //At this point we can be sure both rectangles are overlapping. It is necessary to know which one is better
                                 //We check which one of them gives a better result in the estimated rectangle

                                 std::cout << "There was a collision between: "<< ind <<" and "<< Subind<< std::endl;

                                 NoCollision=false;

                                 if((CalculateErrorEstimation(AuxRect.x, AuxRect.y,AuxRect.width, AuxRect.height))>(CalculateErrorEstimation(SubAuxRect.x,SubAuxRect.y,SubAuxRect.width, SubAuxRect.height)))
                                 {
                                     DiscartedRects.push_back(ind);
                                     ValidRects.push_back(Subind);
                                 }
                                 else
                                 {
                                     DiscartedRects.push_back(Subind);
                                     ValidRects.push_back(ind);
                                 }
                            }
                    }




                }


           }
           if(NoCollision)
            {//There is no overlapping
                   std::cout << "There was NO collision for this one: " << ind << std::endl;
                   ValidRects.push_back(ind);
            }

       }
   }

    std::cout << "Prepearing the last step with this rectangles: " << ValidRects.size() <<std::endl;

   for(int FInd = 0; FInd < ValidRects.size() ; FInd++)
   {

       NotDiscarted = true;
       NotValidRectsYet = true;
        //If the rectangle has been discarted in any step, we discart it
        for(int kkk=0; kkk < DiscartedRects.size() ; kkk++)
        {
            if(DiscartedRects.at(kkk)==ValidRects.at(FInd))
                    NotDiscarted= false;
        }

        for(int kk2=0; kk2 < DiscartedRects.size() ; kk2++)
        {
            if(AlreadyValidRects.at(kk2)==ValidRects.at(FInd))
                    NotValidRectsYet= false;
        }

        if((NotValidRectsYet)&&(NotDiscarted))
        {
            AlreadyValidRects.push_back(ValidRects.at(FInd));
            ReturnRect.push_back(InitialCollec.at(ValidRects.at(FInd)));
        }

   }

    std::cout << "Valid rectangles: " << ReturnRect.size() <<std::endl;

    return ReturnRect;

}
*/
