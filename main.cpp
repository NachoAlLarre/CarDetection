#include "src/car_detector.cpp"


int PointDistance(int Ax, int Ay, int Bx, int By)
{
    int distance;

    distance = sqrt(((Ax-Bx)*(Ax-Bx))+((Ay-By)*(Ay-By)));

    return distance;
}

bool ContourInclination(cv::Point AIn, cv::Point AEnd, cv::Point BIn, cv::Point BEnd)
{
    //std::cout << AIn.x << " " << AIn.y << " "<<AEnd.x << " "<< AEnd.y << " " <<std::endl;
    //std::cout << BIn.x << " " << BIn.y << " "<<BEnd.x << " "<< BEnd.y << " " <<std::endl;

    float d1;
    float d2;
    float den1;
    float num1;

    den1 = (float)(AEnd.y-AIn.y);
    num1 = (float)(AEnd.x-AIn.x);

    //std::cout << den1 << " " << num1 <<std::endl;

    d1= ((float)abs(AEnd.y-AIn.y))/abs(AEnd.x-AIn.x);
    d2 = (float)abs(BEnd.y-BIn.y)/abs(BEnd.x-BIn.x);

    float diff;
    if(d2>d1)
        diff = d2-d1;
     else
        diff = d1-d2;

    //std::cout <<"d1: " << d1   <<" d2: " << d2 << " diff: " << diff <<std::endl;
    if(diff<0.05)
        return true;
    else
        return false;

}

cv::Rect RectangleSum(cv::Rect A, cv::Rect B)
{
    cv::Rect Sum;

    if(A.tl().x < B.tl().x)
        Sum.x = A.x;
    else
        Sum.x = B.x;

    if(A.tl().y < B.tl().y)
        Sum.y = A.y;
    else
        Sum.y = B.y;


    if(A.br().x < B.br().x)
        Sum.width = ((B.x+B.width)-Sum.x);
    else
        Sum.width = ((A.x+A.width)-Sum.x);

    if(A.br().y < B.br().y)
        Sum.height = ((B.y+B.height)-Sum.y);
    else
        Sum.height = ((A.y+A.height)-Sum.y);

    return Sum;

}

bool RectangleSemiCollision(cv::Rect A, cv::Rect B)
{

    cv::Rect Sum;
    int H, W;
    int ResH, ResW;

    if(A.tl().x < B.tl().x)
        Sum.x = A.x;
    else
        Sum.x = B.x;

    if(A.tl().y < B.tl().y)
        Sum.y = A.y;
    else
        Sum.y = B.y;


    if(A.br().x < B.br().x)
        Sum.width = ((B.x+B.width)-Sum.x);
    else
        Sum.width = ((A.x+A.width)-Sum.x);

    if(A.br().y < B.br().y)
        Sum.height = ((B.y+B.height)-Sum.y);
    else
        Sum.height = ((A.y+A.height)-Sum.y);

    W = A.width+B.width;
    H = A.height+B.height;

    ResW = abs(A.x-B.x);
    ResH = abs(A.y-B.y);

    //std::cout << W << " W "<< Sum.width<< " Sum.width "<< H<< " H "<< Sum.height<< " Sum.height"<< std::endl;

    //Here is the difference
    if(((W>Sum.width)&&((H+10)>(Sum.height)))||((W+10)>(Sum.width)&&(H>Sum.height)))
        return true;
    else
        return false;


}

bool BigRectangleCollision(cv::Rect A, cv::Rect B)
{

    cv::Rect Sum;
    int H, W;

    if(A.tl().x < B.tl().x)
        Sum.x = A.x;
    else
        Sum.x = B.x;

    if(A.tl().y < B.tl().y)
        Sum.y = A.y;
    else
        Sum.y = B.y;


    if(A.br().x < B.br().x)
        Sum.width = ((B.x+B.width)-Sum.x);
    else
        Sum.width = ((A.x+A.width)-Sum.x);

    if(A.br().y < B.br().y)
        Sum.height = ((B.y+B.height)-Sum.y);
    else
        Sum.height = ((A.y+A.height)-Sum.y);

    W = A.width+B.width;
    H = A.height+B.height;


    if((W>Sum.width)&&(H>Sum.height))
        return true;
    else
        return false;


}

bool RectangleCollision(cv::Rect A, cv::Rect B)
{
    //If there is a big mismatch in sizes, we don't consider the collision
    if(((A.width*A.height)>(2*B.width*B.height))||((B.width*B.height)>(2*A.width*A.height)))
        return false;

    cv::Rect Sum;
    int H, W;

    if(A.tl().x < B.tl().x)
        Sum.x = A.x;
    else
        Sum.x = B.x;

    if(A.tl().y < B.tl().y)
        Sum.y = A.y;
    else
        Sum.y = B.y;


    if(A.br().x < B.br().x)
        Sum.width = ((B.x+B.width)-Sum.x);
    else
        Sum.width = ((A.x+A.width)-Sum.x);

    if(A.br().y < B.br().y)
        Sum.height = ((B.y+B.height)-Sum.y);
    else
        Sum.height = ((A.y+A.height)-Sum.y);

    W = A.width+B.width;
    H = A.height+B.height;


    if((W>Sum.width)&&(H>Sum.height))
        return true;
    else
        return false;


}

int XCoorFromLine(cv::Vec4i Line, int y)
{
    int X;
    int l1;
    int XUse, YUse;
    bool dUp = true;

    //The d is always calculated in the same way
    int den = abs(Line[0]-Line[2]);

    if(den==0)
        den=1;

    float d = ((float)abs(Line[1]-Line[3]))/den;

    //Since we always calculate using the lower point of the line
    if(Line[1]>Line[3])
     {
        XUse = Line[0];
        YUse = Line[1];
        if(Line[0]<Line[2])
        {
            dUp = true;
        }else{
            dUp = false;
        }
     }
    else
    {
        XUse = Line[2];
        YUse = Line[3];

        if(Line[2]<Line[0])
        {
            dUp = true;
        }else{
            dUp = false;
        }
    }


    l1 = YUse -y;

    float num = (float)abs(l1)/d;

    if(dUp)
    {
        if(l1<0) //Point under line
            X = num-XUse;
        else     //Point over line
            X = num+XUse;
    }else{
        if(l1<0) //Point under line
            X = num+XUse;
        else     //Point over line
            X = XUse-num;
    }

    return X;

}

std::vector<cv::Rect> IfCollisionMerge(std::vector<cv::Rect> InitialCollec)
{

    cv::Rect MeltingRect;
    std::vector<int> DiscartedRects;
    std::vector<cv::Rect> ReturnCollect;
    bool NotRemoved;

    for(int Col=0; Col <InitialCollec.size() ; Col++)
    {
        NotRemoved = true;

        for(int k=0; k < DiscartedRects.size(); k++)
        {
            if(DiscartedRects.at(k)==Col)
                NotRemoved = false;

        }
        if(NotRemoved)
        {

            MeltingRect = InitialCollec.at(Col);

            for(int SubCol=0; SubCol <InitialCollec.size() ; SubCol++)
            {

                if(BigRectangleCollision(MeltingRect, InitialCollec.at(SubCol)))
                {

                    MeltingRect = RectangleSum(MeltingRect, InitialCollec.at(SubCol));

                    DiscartedRects.push_back(SubCol);
                }

            }

            ReturnCollect.push_back(MeltingRect);


        }



    }

    return ReturnCollect;

}


std::vector<cv::Rect> MergingRectangles (std::vector<cv::Rect> OriginalRectangles)
{
    std::vector<cv::Rect> MergedRectanglesAux = OriginalRectangles;
    std::vector<cv::Rect> MergedRectangles;
    bool ContinueMerging = true;

    cv::Mat drawing5 = cv::Mat::zeros(270, 480,  CV_8UC3);

    //std::cout << "Size Melting: "<<MergedRectanglesAux.size() << std::endl;



    for(int indexMerging = 0; indexMerging<MergedRectanglesAux.size() ; indexMerging++ )
    {
        for(int indexMergingSub = indexMerging+1 ; indexMergingSub<MergedRectanglesAux.size() ; indexMergingSub++ )
        {
            if(RectangleSemiCollision(MergedRectanglesAux.at(indexMerging), MergedRectanglesAux.at(indexMergingSub)))
              {

                  MergedRectangles.push_back(RectangleSum(MergedRectanglesAux.at(indexMerging), MergedRectanglesAux.at(indexMergingSub)));
                  //ContinueMerging = true;
              }

        }

    }

    //Here I have the useful rectangles
    for( int indxj = 0; indxj< MergedRectanglesAux.size(); indxj++ )
    {
             cv::rectangle(drawing5, MergedRectanglesAux.at(indxj).tl(), MergedRectanglesAux.at(indxj).br(), cv::Scalar(0, 0, 255), 2, CV_AA);
    }

     for( int indxj = 0; indxj< MergedRectangles.size(); indxj++ )
    {
        cv::rectangle(drawing5, MergedRectangles.at(indxj).tl(), MergedRectangles.at(indxj).br(), cv::Scalar(255, 0, 0), 2, CV_AA);
    }

        MergedRectangles= IfCollisionMerge(MergedRectangles);

         for( int indxj = 0; indxj< MergedRectangles.size(); indxj++ )
        {
            cv::rectangle(drawing5, MergedRectangles.at(indxj).tl(), MergedRectangles.at(indxj).br(), cv::Scalar(0, 255, 0), 2, CV_AA);
        }

        //cv::imshow("Merged (In Green) ",drawing5);

        return MergedRectangles;


}

std::vector<cv::Rect> ContourDetection(cv::Mat ImageContour)
{

            //For finding contours
            std::vector<cv::Rect> VectorRectanglesContoursPrev;

            int thresh2 = 100;
            int max_thresh = 255;
            cv::RNG rng(12345);

            cv::Mat src_gray;

            /// Convert image to gray and blur it
              cv::cvtColor( ImageContour, src_gray, CV_BGR2GRAY );
              cv::blur( src_gray, src_gray, cv::Size(3,3) );


              cv::Mat canny_output;
              std::vector<std::vector<cv::Point> > contours;
              std::vector<cv::Vec4i> hierarchy;

              /// Detect edges using canny
              cv::Canny( src_gray, canny_output, thresh2, thresh2*2, 3 );
              /// Find contours
              cv::findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

              /// Draw contours
              cv::Mat drawing = cv::Mat::zeros( canny_output.size(), CV_8UC3 );
              cv::Mat drawing3 = cv::Mat::zeros( canny_output.size(), CV_8UC3 );
              cv::Mat drawing2 = cv::Mat::zeros( canny_output.size(), CV_8UC3 );
              cv::Mat drawing4 = cv::Mat::zeros( canny_output.size(), CV_8UC3 );

              //two lines are new
              std::vector<std::vector<cv::Point> > contours_poly( contours.size() );
              std::vector<std::vector<cv::Point> > contours_polyRoad( contours.size() );
              std::vector<std::vector<cv::Point> > ValidatedContours;
              std::vector<cv::Rect> boundRect;
              std::vector<cv::Rect> boundRectRoad( contours.size() );

             std::vector<cv::Point> PointsContour;
             cv::Point AuxPointContour;

             bool ValidContour;

             int DescartContourCount;

             int AuxPoAintContourX, AuxPointContourY;
             cv::Rect AuxRect;


              //To filter the lines fo the road
                std::vector<cv::Vec4i> lines;
                cv::Vec4i line;
                cv::Rect RectAuxRoad;
                cv::Mat dst3,cdst3= cv::Mat::zeros(270, 480, CV_32F);

                int indxK = 0;

                cv::Canny(ImageContour, dst3, 50, 200, 3);

                cvtColor(dst3, cdst3, CV_GRAY2BGR);

                cv::HoughLinesP(dst3, lines, 1, CV_PI/180, 50, 50, 10 );

                for( int indxj = 0; indxj< lines.size(); indxj++ )
                {
                    line = lines.at(indxj);
                    cv::line( drawing2, cv::Point(line[0], line[1]), cv::Point(line[2], line[3]), cv::Scalar(0,255,0), 3, CV_AA);
                }

                boundRectRoad.clear();

                for( int indxj = 0; indxj< lines.size(); indxj++ )
                {
                    line = lines.at(indxj);

                    if(line[0]>line[2])
                        RectAuxRoad.x = line[2];
                    else
                        RectAuxRoad.x= line[0];

                    if(line[1]>line[3])
                        RectAuxRoad.y = line[3];
                    else
                        RectAuxRoad.y = line[1];


                     RectAuxRoad.width = abs(line[0]-line[2]);

                     RectAuxRoad.height = abs(line[1]-line[3]);

                     boundRectRoad.push_back(RectAuxRoad);

                }

             //std::cout << "Size of boths" << lines.size() <<" " << boundRectRoad.size() <<std::endl;

             indxK = 0;
             int XAux;

             //std::cout << "Size contours: " << contours.size() << std::endl;

             ValidatedContours.clear();

             for( int indxi = 0; indxi< contours.size(); indxi++ )
             {
                 ValidContour = true;
                 DescartContourCount = 0;

                 drawing3 = cv::Mat::zeros( canny_output.size(), CV_8UC3 );

                 drawing4 = cv::Mat::zeros( canny_output.size(), CV_8UC3 );

                 //Filter sky/trees contours
                 PointsContour = contours[indxi];

                 for(int indxp = 0; indxp< PointsContour.size(); indxp++)
                 {
                     AuxPointContour = PointsContour.at(indxp);
                     if((AuxPointContour.y < 100 )|| (AuxPointContour.y > 265 ))
                     {
                         ValidContour= false;
                         break;
                     }

                 }

                 cv::Scalar color = cv::Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
                 cv::drawContours( drawing4, contours, indxi, color, 2, 8, hierarchy, 0, cv::Point() );


                //cv::Scalar color = cv::Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
                cv::drawContours( drawing2, contours, indxi, color, 2, 8, hierarchy, 0, cv::Point() );

                 //Filter lines contours
                 cv::approxPolyDP( cv::Mat(PointsContour), contours_poly[indxi], 3, true );
                 AuxRect =  cv::boundingRect( cv::Mat(contours_poly[indxi]) );

                 if((AuxRect.width*AuxRect.height)>50)
                 {
                     cv::rectangle(drawing4, AuxRect.tl(),AuxRect.br(), cv::Scalar(255, 0, 0), 2, CV_AA);

                     //Check if the elements may be close to each other

                     for(int IPoint = 0; IPoint < boundRectRoad.size(); IPoint++)
                     {

                        if(RectangleCollision(AuxRect,boundRectRoad.at(IPoint)))
                        {
                            //cv::rectangle(drawing3, AuxRect.tl(), AuxRect.br(), cv::Scalar(255, 0, 0), 2, CV_AA);
                            //cv::rectangle(drawing3, boundRectRoad.at(IPoint).tl(), boundRectRoad.at(IPoint).br(), cv::Scalar(0, 0, 255), 2, CV_AA);

                            line = lines.at(IPoint);

                            for(int IndexContour = 0; IndexContour<PointsContour.size(); IndexContour++)
                            {
                                if(IndexContour%2 ==0)
                                {

                                    XAux = XCoorFromLine(line,PointsContour.at(IndexContour).y);


                                    if(8>PointDistance(PointsContour.at(IndexContour).x, PointsContour.at(IndexContour).y, XAux ,PointsContour.at(IndexContour).y))
                                    {
                                        //std::cout << "Close!!" << std::endl;
                                        DescartContourCount++;

                                    }

                                    if(DescartContourCount>6)
                                    {
                                        //std::cout << "Matching!!" << std::endl;
                                        IndexContour += PointsContour.size();
                                        ValidContour= false;
                                        break;
                                    }

                                }

                            }




                        }

                     }


                 }else{

                    ValidContour = false;
                 }

                 if(ValidContour)
                    ValidatedContours.push_back(PointsContour);



             }


             for(int indxj = 0; indxj < ValidatedContours.size(); indxj++)
             {

                cv::Scalar color = cv::Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
                cv::drawContours( drawing, ValidatedContours, indxj, color, 2, 8, hierarchy, 0, cv::Point() );
                cv::approxPolyDP( cv::Mat(ValidatedContours[indxj]), contours_poly[indxj], 3, true );
                boundRect.push_back(cv::boundingRect( cv::Mat(contours_poly[indxj])));

             }

             //Here I have the useful rectangles
             for( int indxj = 0; indxj< boundRect.size(); indxj++ )
             {
                 cv::rectangle(drawing, boundRect.at(indxj).tl(), boundRect.at(indxj).br(), cv::Scalar(0, 0, 255), 2, CV_AA);
             }

             VectorRectanglesContoursPrev = MergingRectangles(boundRect);

            std::vector<cv::Rect> AuxiliarRectangles = VectorRectanglesContoursPrev;
            cv::Rect RectSqu;
            int SumRectSQ;
            int ErrorWidth;

            bool ValidatedSquare;

            VectorRectanglesContoursPrev.clear();

             for(int Conv = 0; Conv < AuxiliarRectangles.size() ; Conv++)
             {
                 ValidatedSquare = true;
                 RectSqu = AuxiliarRectangles.at(Conv);

                 if((RectSqu.width > (2.5*RectSqu.height))|| ((RectSqu.width*2.5) < RectSqu.height))
                    ValidatedSquare = false;

                 if(ValidatedSquare)
                 {
                     SumRectSQ = RectSqu.width + RectSqu.height;
                     SumRectSQ /=2;

                     if((SumRectSQ+RectSqu.x)>480)
                     {
                         ErrorWidth = 480-RectSqu.x;
                         RectSqu.width = ErrorWidth;
                         RectSqu.height = ErrorWidth;
                     }
                     else
                     {
                         RectSqu.width = (SumRectSQ);
                     }

                     if((SumRectSQ+RectSqu.y)>270)
                     {
                         ErrorWidth = 270-RectSqu.y;
                         RectSqu.width = ErrorWidth;
                         RectSqu.height = ErrorWidth;

                     }else{
                        RectSqu.height = (SumRectSQ);
                     }

                        VectorRectanglesContoursPrev.push_back(RectSqu);
                 }


             }

            return VectorRectanglesContoursPrev;


}


int main()
{

    //Custom methods to handle faster the elements in the given folders (images and markups documments)
    mvf::Imageset imsetImages;
    mvf::Imageset imsetImagesNext;
    mvf::Imageset imsetImagesNextLoop;
    mvf::Imageset imsetLabels;

    std::string ImageName;
    std::string NameFile;
    std::string FilePath;
    std::stringstream AuxiliarStringStream;



    //0- Ask the User if he/she want to use the training images or the contest images
    std::string UserInput = "";
    std::string mYES = "Y";
    std::string myes = "y";
    std::string mNo = "N";

    std::cout << "Do you want to already use the Contest Image Folder? Type 'Y' for yes, 'N' for no\n>";
    getline(std::cin, UserInput);

    if((UserInput.compare(mYES) != 0)&& (UserInput.compare(myes) != 0)) //In case it is just True, the image is empty and we can use it as background
    {

        int startingPos= 0;//4800; //To start in the highway

        imsetImages.loadSetFromDir("Data/Contest/Contest/images/", "jpg");
        imsetImages.CurrentIndex=startingPos-1;

        imsetLabels.loadSetFromDir("Data/Contest/Contest/markup/", "txt");
        imsetLabels.CurrentIndex=startingPos-1;

        imsetImagesNext.loadSetFromDir("Data/Contest/Contest/images/", "jpg");
        imsetImagesNext.CurrentIndex=startingPos;



        ////////////////////////////////////////////////////////////////
        //////   1-    PREPARATION OF THE RESOURCES                ////
        ////////////////////////////////////////////////////////////////


        //THE FIRST STEP WAS TO ORGANIZE THE FOLDERS TO APPLY TRAINCASCADE
        //FOLLOWING THE INSTRUCTIONS OF: http://docs.opencv.org/trunk/doc/user_guide/ug_traincascade.html


        //1- Ask the User if it is necessary to perform this step
        std::string UserInput = "";
        std::string mYES = "Y";
        std::string mNo = "N";

        std::cout << "Is it necessary to create a training file? Type 'Y' for yes, 'N' for no\n>";
        getline(std::cin, UserInput);

        if(UserInput.compare(mYES) == 0) //In case it is just True, the image is empty and we can use it as background
        {
            std::string line;
            std::string OutputFileName;
            std::string FinalLine;
            std::string mTRUE = "True";
            std::string mFALSE = "False";

            bool PosibleBG;
            bool Skip;

            int TotalRectangles = 0;
            int FrameRectangles = 0;

            int CounterFramesEmpty = 0;
            int CounterFramesCar = 0;

            //Variable to copy images
            FILE *fp1,*fp2;
            char ch;

            //Files to store the information
            std::ofstream Bgfile;
            Bgfile.open ("bg.txt");
            std::ofstream Infofile;
            Infofile.open ("info.txt");

            cv::Mat OriginalImg;
            car_detector mNew_car;

            if ( Bgfile.is_open() && Infofile.is_open())
            {

                for (int i=startingPos; i< imsetImages.SequenceSize; i++)
                {

                    //1- Analize every txt file from the markup folder
                    imsetLabels.CurrentIndex++;
                    NameFile = imsetLabels.getCurrentFileName();

                    AuxiliarStringStream.str(std::string());
                    AuxiliarStringStream << "Data/Contest/Contest/markup/" << NameFile;
                    FilePath = AuxiliarStringStream.str();

                    std::cout << NameFile << std::endl;

                    imsetImages.CurrentIndex++;
                    imsetImages.getCurrentImg(OriginalImg);
                    //NameImage = imsetImages.getCurrentFileName();

                    ////////////Added for images
                    cv::imshow("Original Image", OriginalImg);

                    //Instance an object of the class car_detector
                    mNew_car.SetOriginalImage(OriginalImg);

                    //Analize the markup file to print in the image the rectangles defined in the markup file
                    mNew_car.DrawMarkupRectangles(FilePath);
                    cv::Mat aux__=mNew_car.GetMatImage();
                    cv::imshow("Marked up Image", aux__);
                    cv::waitKey();

                    /////////////////////////////////////////////////////////////////////////////////////7


                    //2- Retrieve the name of the picture that matches with the analized label
                    /*
                    imsetImages.CurrentIndex++;
                    ImageName = imsetImages.getCurrentFileName();

                    std::ostringstream oss;
                    std::ifstream myfile (FilePath.c_str());

                    PosibleBG = false;
                    Skip = false;
                    FrameRectangles = 0;

                    FinalLine = "";
                    OutputFileName = "";

                    if (myfile.is_open())
                    {
                        while ( getline (myfile,line) )
                        {

                            if(line.compare(mTRUE) == 0) //In case it is just True, the image is empty and we can use it as background
                            {
                                PosibleBG = true;
                            }

                            if(line.compare(mFALSE) == 0) //In case there is just False, the image is worthless
                            {
                                Skip = true;
                            }

                            if ((line.compare(mTRUE) != 0)&& (line.compare(mFALSE) != 0))
                            {
                                  //Since there are some rectangles it is not a background
                                  PosibleBG = false;
                                  Skip = false;

                                  //Increase the number of rectanlges
                                  TotalRectangles++;
                                  FrameRectangles++;

                                  //We concatenate lines, in case there were more than one rectable in the markup file
                                  FinalLine += line;


                            }else{
                                std::cout << line << '\n';
                            }


                        }

                        //Once the file is fully read

                        if(!Skip)
                        {
                            if(PosibleBG) //It is a background
                            {

                                AuxiliarStringStream.str(std::string());
                                AuxiliarStringStream << "Data/Contest/Contest/images/" << ImageName;
                                FilePath = AuxiliarStringStream.str();



                                if((fp1=fopen(FilePath.c_str(), "rb"))==NULL)
                                {
                                    std::cout<<"Error in images";
                                    exit(-1);
                                }

                                oss << "bg\\empty" << CounterFramesEmpty << ".jpg";

                                OutputFileName = oss.str();

                                if((fp2=fopen(OutputFileName.c_str(), "wb"))==NULL)
                                {
                                    std::cout<<"Error in empty "<<i;
                                    exit(-1);
                                }

                                //The picture is copied in the bg folder with a new name: emptyX.jpg
                                while(!feof(fp1))
                                {
                                    fread(&ch, 1, 1, fp1);
                                    fwrite(&ch, 1, 1, fp2);
                                }

                                //A new line is added to the bg.txt

                                Bgfile << "bg/empty" <<CounterFramesEmpty << ".jpg" <<"\n";


                                fclose(fp2);
                                fclose(fp1);


                                CounterFramesEmpty++;

                            }else{ //It is positive frame


                                AuxiliarStringStream.str(std::string());
                                AuxiliarStringStream << "Data/Contest/Contest/images/" << ImageName;
                                FilePath = AuxiliarStringStream.str();


                                if((fp1=fopen(FilePath.c_str(), "rb"))==NULL)
                                {
                                    std::cout<<"Error en images";
                                    exit(-1);
                                }

                                oss << "positive\\car" << CounterFramesCar << ".jpg";

                                OutputFileName = oss.str();

                                if((fp2=fopen(OutputFileName.c_str(), "wb"))==NULL)
                                {
                                    std::cout<<"Error en car "<<i;
                                    exit(-1);
                                }

                                //The image is copied in the positive folder with a new name: carX.jpg
                                while(!feof(fp1))
                                {
                                    fread(&ch, 1, 1, fp1);
                                    fwrite(&ch, 1, 1, fp2);
                                }

                                //A new line is added to the info.txt

                                Infofile << "positive/car"<< CounterFramesCar << ".jpg " << FrameRectangles << " " << FinalLine <<"\n";


                                fclose(fp2);
                                fclose(fp1);

                                CounterFramesCar++;

                            }

                        }


                    }else{

                        std::cout << line << '\n';

                    }

                    std::cout << "Total Number of Rectangles (posible samples): " << TotalRectangles << std::endl;
                     */
                }

            }std::cout << "There was a problem opening the files  Bgfile.txt and Infofile.txt" << std::endl;

        }

        //WITH THE INTENTION OF FILERING FALSE POSITIVES IT WAS DECEIDED TO CREATE A SIZE ESTIMATION OF VALID OBJECT (VEHICLE)
        //BASED IN ITS POSITION (THAT IS IN ITS COORDINATES)

        //TO DO SO, IT WAS USED THE POLINOMIAL APROXIMATION: LEAST SQUARED ERROR MINIMIZATION, WHICH OPERATES WITH THE FOLLOWING FORMULA
        //                  S = c0 + c1*x + c2*y + c3*x^2 + c4*y^2 + c5*x*y
        //WHERE x AND y ARE COORDINATES. IT WAS DECIEDED TO USE TO DIFFERENT SET OF COEFFICIENTS, BASED ON TWO DIFFERENT SETS OF CORRDINATES
        // 1- WHERE x AND y DEFINE THE CENTERS OF THE SQUARE THAT SURREND EACH VEHICLE
        // 2- WHERE x AND y DEFINE THE CENTERS OF THE BOTTOM EDGE OF THE SQUARE THAT SURREND EACH VEHICLE


        //1- Ask the User if it is necessary to perform this step

        std::cout << "Is it necessary to create CentersPoints.txt and BottomsPoints.txt files? Type 'Y' for yes, 'N' for no\n>";
        getline(std::cin, UserInput);

        if(UserInput.compare(mYES) == 0) //In case it is just True, the image is empty and we can use it as background
        {
            //Preparation of the files to calculate the coeficients using the markup files

            std::ofstream PrintingCenters;
            PrintingCenters.open("CentersPoints.txt");

            std::ofstream PrintingBottoms;
            PrintingBottoms.open("BottomsPoints.txt");

            int NumSquaresFrame = 0;

            //Instance an object of the class car_detector
            car_detector mNew_car;

            for (int i=startingPos; i< imsetImages.SequenceSize; i++)
            {

                imsetLabels.CurrentIndex++;
                NameFile = imsetLabels.getCurrentFileName();

                AuxiliarStringStream.str(std::string());
                AuxiliarStringStream << "Data/Contest/Contest/markup/" << NameFile;
                FilePath = AuxiliarStringStream.str();


                //LOOP TO RETRIEVE THE COORDINATES OF THE BOTTOM AND CENTER COORDINATES
                NumSquaresFrame = mNew_car.BottomsCentersCoordinatesFiles(FilePath);

                if(NumSquaresFrame>0)
                {
                    for(int i=1; i<(NumSquaresFrame+1) ; i++)
                    {

                        PrintingBottoms << mNew_car.GetaXCoorCentre(i) << " " <<  mNew_car.GetaYCoorCentre(i) << " " <<  mNew_car.GetWidth(i) <<"\n";
                        PrintingCenters << mNew_car.GetaXCoorCentre(i) << " " <<  mNew_car.GetaY2CoorCentre(i) << " " <<  mNew_car.GetWidth(i) <<"\n";

                    }

                }


            }std::cout << "All elements of the folder have been read"<<std::endl;


           PrintingBottoms.close();
           PrintingCenters.close();

        }


        //**IT WAS USED A MATLAB SCRIP TO CALCULATE THE COEFFICIENTS, THAT LATER WERE INCLUDED AS PROTECTED CONSTANT IN THE car_detector CLASS


        ////////////////////////////////////////////////////////////////
        //////   2-  VEHICLES DETECTION                             ////
        ////////////////////////////////////////////////////////////////

        std::cout << "The scrip starts detecting vehicles"<<std::endl;


        //1- It is necessary to creat a mask to filter FALSE POSITIVES

        //Instance an object of the class car_detector
        car_detector mNew_car;

        //MaskC and MaskB are generated
        mNew_car.CoeficientsMasks();


        //ONCE THE CASCADE.XML FILE IS CREATED IT IS POSSIBLE TO USE IT

        std::string Pathxml = "cascade.xml";
        std::vector<cv::Rect> VectorRectanglesHaar;

        //Matrixes to content the images, the Original one and the Modified
        cv::Mat AuxiliarImg;
        cv::Mat OriginalImg;
        cv::Mat NextImg;

        cv::Mat CropImage;



        for (int i=startingPos; i < imsetImages.SequenceSize; i++)
        {
            std::vector<cv::Rect> ValidateRectangle;
            std::vector<cv::Rect> TotalValidatedRectangle;
            std::vector<cv::Rect> FinalRectanglesCollection;



            imsetImages.CurrentIndex++;
            imsetImages.getCurrentImg(OriginalImg);

            imsetLabels.CurrentIndex++;
            NameFile = imsetLabels.getCurrentFileName();

            //In Next image I have the next frame
            imsetImagesNext.CurrentIndex++;
            imsetImagesNext.getCurrentImg(NextImg);


            AuxiliarStringStream.str(std::string());
            AuxiliarStringStream << "Data/Contest/Contest/markup/" << NameFile;
            FilePath = AuxiliarStringStream.str();

            VectorRectanglesHaar.clear();

            //Firstly display the original image, without rectanles
            cv::imshow("Original Image", OriginalImg);

            //Instance an object of the class car_detector
            mNew_car.SetOriginalImage(OriginalImg);

            //Analize the markup file to print in the image the rectangles defined in the markup file
            mNew_car.DrawMarkupRectangles(FilePath);
            cv::Mat aux__=mNew_car.GetMatImage();
            AuxiliarImg = aux__.clone();

            //Optical flow

            //OpticalFlow(OriginalImg,NextImg);

            //Use the cascade to find cars in the next image
            if(mNew_car.HaarCascadeDetector(NextImg, VectorRectanglesHaar, Pathxml))
            {
                for(int k=0; k<VectorRectanglesHaar.size() ; k++)
                {

                    //Filtering of the false positives using MaskC and MaskB

                    if(mNew_car.FalsePositivesDetector(VectorRectanglesHaar.at(k)))
                    {
                         //cv::rectangle(AuxiliarImg, VectorRectanglesHaar.at(k).tl(), VectorRectanglesHaar.at(k).br(), cv::Scalar(0, 0, 255), 2, CV_AA);

                         ValidateRectangle.push_back(VectorRectanglesHaar.at(k));
                    }
                    else
                    {
                        //Without Filtering
                        //cv::rectangle(AuxiliarImg, VectorRectanglesHaar.at(k).tl(), VectorRectanglesHaar.at(k).br(), cv::Scalar(255, 0, 0), 2, CV_AA);

                    }


                }

            }



            //Use the cascade to find cars in the original image
            if(mNew_car.HaarCascadeDetector(OriginalImg, VectorRectanglesHaar, Pathxml))
            {
                for(int k=0; k<VectorRectanglesHaar.size() ; k++)
                {

                    //Filtering of the false positives using MaskC and MaskB

                    if(mNew_car.FalsePositivesDetector(VectorRectanglesHaar.at(k)))
                    {
                         //cv::rectangle(AuxiliarImg, VectorRectanglesHaar.at(k).tl(), VectorRectanglesHaar.at(k).br(), cv::Scalar(0, 0, 255), 2, CV_AA);
                        CropImage = OriginalImg(VectorRectanglesHaar.at(k));
                        cv::cvtColor(CropImage, CropImage, CV_BGR2GRAY);
                        std::vector<cv::Point2f> prev_p;
                        std::vector<cv::Point2f> next_p;
                        cv::goodFeaturesToTrack(CropImage, prev_p, 25, 0.01, 7);
                        if (prev_p.size()>0)
                        {
                            for (size_t i_=0; i_<prev_p.size(); i_++)
                            {
                                prev_p.at(i_).x+=VectorRectanglesHaar.at(k).x;
                                prev_p.at(i_).y+=VectorRectanglesHaar.at(k).y;
                            }
                            cv::Mat Orgn_gs, Next_gs;
                            cv::cvtColor(OriginalImg, Orgn_gs, CV_BGR2GRAY);
                            cv::cvtColor(NextImg, Next_gs, CV_BGR2GRAY);
                            std::vector<uchar> status_;
                            std::vector<float> error;

                            cv::calcOpticalFlowPyrLK(Orgn_gs, Next_gs, prev_p, next_p, status_, error, cv::Size(VectorRectanglesHaar.at(k).width, VectorRectanglesHaar.at(k).height), 5);

                            cv::Mat Pic=NextImg.clone();

                            int MasPointX, MasPointY;
                            int AveX = 0;
                            int AveY = 0;
                            size_t j_ = 0;
                            for (; j_<next_p.size(); j_++)
                            {
                                cv::circle(Pic, next_p.at(j_), 3, cv::Scalar(0,200,200), -1);
                                AveX += next_p.at(j_).x;
                                AveY += next_p.at(j_).y;

                            }
                            //j_++;

                            MasPointX = AveX/j_;
                            MasPointY = AveY/j_;

                            cv::circle(Pic, cv::Point(MasPointX,MasPointY), 5, cv::Scalar(100,200,200), -1);
                            cv::Rect ReturnRect;

                            ReturnRect = mNew_car.CalculateEstimatedRect(MasPointX, MasPointY);

                            bool IsValid = false;

                            int Distance;
                            int DiffX, DiffY;

                            for(int vr = 0; vr < ValidateRectangle.size(); vr++ )
                            {
                                   DiffX = abs(ValidateRectangle.at(vr).x + ValidateRectangle.at(vr).width/2 - ReturnRect.x - ReturnRect.width/2);
                                   DiffY = abs(ValidateRectangle.at(vr).y + ValidateRectangle.at(vr).height/2 - ReturnRect.y - ReturnRect.height/2);
                                   Distance =  sqrt((DiffX*DiffX)+(DiffY*DiffY));

                                    cv::rectangle(Pic, ValidateRectangle.at(vr), cv::Scalar(0, 0, 255), 2, CV_AA);

                                   if(Distance<(0.7*ReturnRect.width))
                                   {
                                        //Inatead of directly printing the rectangle, we store it
                                        TotalValidatedRectangle.push_back(VectorRectanglesHaar.at(k));
                                        cv::rectangle(AuxiliarImg, VectorRectanglesHaar.at(k).tl(), VectorRectanglesHaar.at(k).br(), cv::Scalar(255, 0, 0), 2, CV_AA);
                                        IsValid=true;
                                        break;

                                   }


                            }


                            if(IsValid)
                            {

                                cv::rectangle(Pic, ReturnRect.tl(), ReturnRect.br(), cv::Scalar(255, 0, 255), 2, CV_AA);
                            }
                            //ValidateRectangle.push_back(VectorRectanglesHaar.at(k));


                            //cv::imshow("aux_", AuxiliarImg);
                            //cv::imshow("n_fr", Pic);
                            //cv::waitKey();
                        }

                    }
                    else
                    {
                        //Without Filtering
                        //cv::rectangle(AuxiliarImg, VectorRectanglesHaar.at(k).tl(), VectorRectanglesHaar.at(k).br(), cv::Scalar(255, 0, 0), 2, CV_AA);

                    }


                }

                //At this point we have in TotalValidatedRectangle all the rectangles that have passed the filtering
                //Now we filter overlaped rectangles

                FinalRectanglesCollection = mNew_car.DoubleRectanglesCleaner(TotalValidatedRectangle);

                for(int PrintInd = 0; PrintInd < FinalRectanglesCollection.size() ; PrintInd++)
                    cv::rectangle(AuxiliarImg, FinalRectanglesCollection.at(PrintInd).tl(), FinalRectanglesCollection.at(PrintInd).br(), cv::Scalar(0, 0, 255), 2, CV_AA);




            }


            cv::imshow("Original Image + Rectangles. Green-> Given rectanlges, Blue-> Estimated rectangles, Red -> After Filtering", AuxiliarImg);

            cv::waitKey();

        }




    }else{


            std::string mTest = "T";
            std::string mtest = "t";

            std::cout << "Do you want to Test or Generate labels?\n>";
            getline(std::cin, UserInput);


        if((UserInput.compare(mTest) == 0)|| (UserInput.compare(mtest) == 0))
        {


            int startingPos= 0;//4800; //To start in the highway

            imsetImages.loadSetFromDir("Data/images/", "jpg");
            imsetImages.CurrentIndex=startingPos-1;

            imsetLabels.loadSetFromDir("Results/", "txt");
            imsetLabels.CurrentIndex=startingPos-1;

            cv::Mat OriginalImg;

            for (int i=startingPos; i < imsetImages.SequenceSize; i++)
            {

                imsetImages.CurrentIndex++;
                imsetImages.getCurrentImg(OriginalImg);

                imsetLabels.CurrentIndex++;
                NameFile = imsetLabels.getCurrentFileName();


                AuxiliarStringStream.str(std::string());
                AuxiliarStringStream << "Results/" << NameFile;
                FilePath = AuxiliarStringStream.str();

                car_detector mNew_car;

                //Firstly display the original image, without rectanles
                cv::imshow("Original Image", OriginalImg);

                //Instance an object of the class car_detector
                mNew_car.SetOriginalImage(OriginalImg);

                //Analize the markup file to print in the image the rectangles defined in the markup file
                mNew_car.DrawMarkupRectangles(FilePath);
                cv::Mat aux__=mNew_car.GetMatImage();
                cv::imshow("res", aux__);
                cv::waitKey();



            }

        }else{






            int startingPos= 0;//4800; //To start in the highway 1052;//

            imsetImages.loadSetFromDir("Data/images/", "jpg");
            imsetImages.CurrentIndex=startingPos-1;

            imsetImagesNext.loadSetFromDir("Data/images/", "jpg");
            imsetImagesNext.CurrentIndex=startingPos;

            imsetImagesNextLoop.loadSetFromDir("Data/images/", "jpg");
            imsetImagesNextLoop.CurrentIndex=startingPos;


            //IT WAS USED A MATLAB SCRIP TO CALCULATE THE COEFFICIENTS, THAT LATER WERE INCLUDED AS PROTECTED CONSTANT IN THE car_detector CLASS


            ////////////////////////////////////////////////////////////////
            //////   2-  VEHICLES DETECTION                             ////
            ////////////////////////////////////////////////////////////////

            std::cout << "The scrip starts detecting vehicles"<<std::endl;


            //1- It is necessary to creat a mask to filter FALSE POSITIVES

            //Instance an object of the class car_detector
            car_detector mNew_car;

            mNew_car.CoeficientsMasks();

            std::string NameImage, FilePath ;


            //ONCE THE CASCADE.XML FILE IS CREATED IT IS POSSIBLE TO USE IT
            std::string Pathxml1 = "cascade.xml";
            std::string Pathxml2 = "cascade3.xml"; //From https://github.com/abhi-kumar/CAR-DETECTION/blob/master/cas1.xml

            std::string PathxmlLeft = "cascadeLeft.xml";

            std::vector<cv::Rect> VectorRectanglesHaar;
            std::vector<cv::Rect> VectorRectanglesHaar2;
            std::vector<cv::Rect> VectorRectanglesContours;
            std::vector<cv::Rect> VectorRectanglesTotalMerged;
            std::vector<cv::Rect> VectorRectanglesHaarLeft;
            std::vector<cv::Rect> VectorRectanglesHaarLoop;
            std::vector<cv::Rect> VectorRectanglesHaarMerged;

            bool FileAlreadyCreated, NeedToBeClosed;

            //Matrixes to content the images, the Original one and the Modified
            cv::Mat AuxiliarImg;
            cv::Mat FinalImg;
            cv::Mat OriginalImg;
            cv::Mat NextImg;
            cv::Mat NextNextImg;

            cv::Mat CropImage;

            int CounterMarkedUp =0;
            int EmptyFiles = 0;

            int AuxCenterX, AuxCenterY;
            int OFWindowX = 35;
            int OFWindowX2 = 320;
            int OFWindowY = 100;

            cv::Vec4i BackUpRightBorder, BackUpLeftBorder;

            //Here we initialize the borders

            BackUpLeftBorder[0] = 0;
            BackUpLeftBorder[1] = 210;
            BackUpLeftBorder[2] = 170;
            BackUpLeftBorder[3] = 120;

            BackUpRightBorder[0] = 340;
            BackUpRightBorder[1] = 120;
            BackUpRightBorder[2] = 480;
            BackUpRightBorder[3] = 210;

            bool TrafficJam = false;


            int CountForward;

            int PositionLine;

            std::string Place;
            std::string HighWay = "110001";

            for (int i=startingPos; i < imsetImages.SequenceSize; i++)
            {

                TrafficJam = false;

                std::vector<cv::Rect> ValidateRectangle;
                std::vector<cv::Rect> ValidateRectangleLoop;
                std::vector<cv::Rect> TotalValidatedRectangle;
                std::vector<cv::Rect> FinalRectanglesCollection;

                imsetImages.CurrentIndex++;
                imsetImages.getCurrentImg(OriginalImg);
                NameImage = imsetImages.getCurrentFileName();

                //mNew_car.SetOriginalImage(OriginalImg.clone());
                //MaskC and MaskB are generated


                PositionLine = NameImage.find("-");
                Place = NameImage.substr (0,PositionLine);

                //std::cout << "Place " << Place <<std::endl;

                if(Place.compare(HighWay)==0)
                      TrafficJam = true;



                CountForward = 0;

                //imsetLabels.CurrentIndex++;
                //NameFile = imsetLabels.getCurrentFileName();


                imsetImagesNext.CurrentIndex++;
                imsetImagesNext.getCurrentImg(NextImg);



                imsetImagesNextLoop.CurrentIndex++;
                imsetImagesNextLoop.getCurrentImg(NextNextImg);

                AuxiliarStringStream.str(std::string());
                AuxiliarStringStream << "Results/" << NameImage << ".txt";
                FilePath = AuxiliarStringStream.str();

                std::ofstream ResultsFile;
                //ResultsFile.open(FilePath.c_str());

                /*if(!ResultsFile.isopen())
                    std::cout << "Error Opening Results File" << std::endl;*/

                 cv::Mat Pic = NextImg.clone();
                 cv::Mat FutureMat = NextImg.clone();


                VectorRectanglesContours.clear();
                VectorRectanglesTotalMerged.clear();
                VectorRectanglesHaarLoop.clear();

                FileAlreadyCreated = NeedToBeClosed = false;

                //Firstly display the original image, without rectanles
                //cv::imshow("Original Image", OriginalImg);

                //Instance an object of the class car_detector
                mNew_car.SetOriginalImage(OriginalImg.clone());

                //Analize the markup file to print in the image the rectangles defined in the markup file
                //mNew_car.DrawMarkupRectangles(FilePath);
                cv::Mat aux__=mNew_car.GetMatImage();
                AuxiliarImg = aux__.clone();
                FinalImg = aux__.clone();


                //Here we determine the Borders
                std::vector<cv::Vec4i> Borders = mNew_car.BorderLines(OriginalImg.clone());
                if(Borders.at(0)[0]>0)
                    BackUpRightBorder = Borders.at(0);

                if((Borders.at(1)[1]>0)&&(Borders.at(1)[2]>120))
                    BackUpLeftBorder = Borders.at(1);

               // cv::line( AuxiliarImg, cv::Point(BackUpRightBorder[0], BackUpRightBorder[1]), cv::Point(BackUpRightBorder[2], BackUpRightBorder[3]), cv::Scalar(0,200,200), 3, CV_AA);
                //cv::line( AuxiliarImg, cv::Point(BackUpLeftBorder[0], BackUpLeftBorder[1]), cv::Point(BackUpLeftBorder[2], BackUpLeftBorder[3]), cv::Scalar(0,200,200), 3, CV_AA);
                //cv::line( AuxiliarImg, cv::Point(0, 243), cv::Point(480, 243), cv::Scalar(0,200,200), 3, CV_AA);


                VectorRectanglesHaar.clear();
                VectorRectanglesHaar2.clear();

                VectorRectanglesContours = ContourDetection(NextImg);

                mNew_car.HaarCascadeDetector(NextImg, VectorRectanglesHaar, Pathxml1);
                if(TrafficJam)
                    mNew_car.HaarCascadeDetector(NextImg, VectorRectanglesHaar2, Pathxml2);
                VectorRectanglesContours.size();

                //std::cout << VectorRectanglesHaar.size() << " " <<VectorRectanglesHaar2.size()<< " "<< VectorRectanglesContours.size() << " " << TrafficJam << std::endl;

                //Merging 3 detector process
                VectorRectanglesTotalMerged.reserve(VectorRectanglesHaar.size() + VectorRectanglesHaar2.size() + VectorRectanglesContours.size());
                VectorRectanglesTotalMerged.insert(VectorRectanglesTotalMerged.end(), VectorRectanglesHaar.begin(),VectorRectanglesHaar.end() );
                VectorRectanglesTotalMerged.insert(VectorRectanglesTotalMerged.end(), VectorRectanglesHaar2.begin(),VectorRectanglesHaar2.end() );
                VectorRectanglesTotalMerged.insert(VectorRectanglesTotalMerged.end(), VectorRectanglesContours.begin(),VectorRectanglesContours.end() );

                //std::cout <<  VectorRectanglesTotalMerged.size() << std::endl;

                if(VectorRectanglesHaar.size()>0)//if((mNew_car.HaarCascadeDetector(OriginalImg, VectorRectanglesHaar, Pathxml1)) || (mNew_car.HaarCascadeDetector(OriginalImg, VectorRectanglesHaar2, Pathxml2)))
                {
                    for(int k=0; k<VectorRectanglesHaar.size() ; k++)
                    {
                        //Filtering of the false positives using MaskC and MaskB
                        if(mNew_car.FalsePositivesDetector(VectorRectanglesHaar.at(k)))
                        {
                             cv::rectangle(FutureMat, VectorRectanglesHaar.at(k).tl(), VectorRectanglesHaar.at(k).br(), cv::Scalar(255, 0, 0), 2, CV_AA);
                             ValidateRectangle.push_back(VectorRectanglesHaar.at(k));
                        }
                    }
                }


                VectorRectanglesHaar.clear();
                VectorRectanglesHaar2.clear();

                VectorRectanglesContours = ContourDetection(OriginalImg);

                //cv::Mat CheckCascade1 = AuxiliarImg.clone();
                //cv::Mat CheckCascade2 = AuxiliarImg.clone();
                cv::Mat CheckCascadeBoth = OriginalImg.clone();




                //Use the cascade to find cars in the original image
                //Conver rectangles into squares



                mNew_car.HaarCascadeDetector(OriginalImg, VectorRectanglesHaar, Pathxml1);
                if(TrafficJam)
                    mNew_car.HaarCascadeDetector(OriginalImg, VectorRectanglesHaar2, Pathxml2);

                VectorRectanglesTotalMerged.clear();

                //Merging 3 detector process
                VectorRectanglesTotalMerged.reserve(VectorRectanglesHaar.size() + VectorRectanglesHaar2.size() + VectorRectanglesContours.size());
                VectorRectanglesTotalMerged.insert(VectorRectanglesTotalMerged.end(), VectorRectanglesHaar.begin(),VectorRectanglesHaar.end() );
                VectorRectanglesTotalMerged.insert(VectorRectanglesTotalMerged.end(), VectorRectanglesHaar2.begin(),VectorRectanglesHaar2.end() );
                VectorRectanglesTotalMerged.insert(VectorRectanglesTotalMerged.end(), VectorRectanglesContours.begin(),VectorRectanglesContours.end() );

                //std::cout <<  VectorRectanglesTotalMerged.size() << std::endl;
                /*

                cv::imshow("Rectangles Detected by HaarCascade ", AuxiliarImg);
                cv::imshow("Red Color rectangles filtered by Size Estimation", CheckCascadeBoth);

                */

                if(VectorRectanglesHaar.size()>0)//if((mNew_car.HaarCascadeDetector(OriginalImg, VectorRectanglesHaar, Pathxml1)) || (mNew_car.HaarCascadeDetector(OriginalImg, VectorRectanglesHaar2, Pathxml2)))
                {


                    for(int k=0; k<VectorRectanglesHaar.size() ; k++)
                    {


                    }

                    for(int k=0; k<VectorRectanglesHaar2.size() ; k++)
                    {

                         //cv::rectangle(CheckCascadeBoth, VectorRectanglesHaar2.at(k).tl(), VectorRectanglesHaar2.at(k).br(), cv::Scalar(0, 0, 255), 2, CV_AA);

                    }


                    for(int k=0; k<VectorRectanglesContours.size() ; k++)
                    {

                         //cv::rectangle(CheckCascadeBoth, VectorRectanglesContours.at(k).tl(), VectorRectanglesContours.at(k).br(), cv::Scalar(0, 255, 0), 2, CV_AA);

                    }


                    //cv::imshow("Full Detection",CheckCascadeBoth);


                    ///////Here is the Good part
                    VectorRectanglesTotalMerged = VectorRectanglesHaar;

                    for(int k=0; k< VectorRectanglesTotalMerged.size() ; k++)
                    {
                        //In Red color -> RECTANGLES FILTERED BY FALSE POSITIVE
                        //cv::rectangle(AuxiliarImg,  VectorRectanglesTotalMerged.at(k).tl(),  VectorRectanglesTotalMerged.at(k).br(), cv::Scalar(0, 0, 255), 2, CV_AA);

                        //To filter rectangles that can't be a car, otherwise there would be an accident



                        if(( VectorRectanglesTotalMerged.at(k).y+ VectorRectanglesTotalMerged.at(k).height)<243)
                        {

                            //Filtering of the false positives using MaskC and MaskB
                            if(mNew_car.FalsePositivesDetector( VectorRectanglesTotalMerged.at(k)))
                            {


                                //In Blue color -> RECTANGLES FILTERED BY OPTIVAL FLOW
                                //cv::rectangle(AuxiliarImg,  VectorRectanglesTotalMerged.at(k).tl(),  VectorRectanglesTotalMerged.at(k).br(), cv::Scalar(255, 0, 0), 2, CV_AA);

                                //Here we stablish an Optical Flow FREE Window
                                AuxCenterX =  VectorRectanglesTotalMerged.at(k).x + ( VectorRectanglesTotalMerged.at(k).width/2);
                                AuxCenterY =  VectorRectanglesTotalMerged.at(k).y + ( VectorRectanglesTotalMerged.at(k).width/2);

                                if(((AuxCenterX > OFWindowX)&&(AuxCenterX < (OFWindowX+190))||((AuxCenterX > (OFWindowX2))&&(AuxCenterX < (480))))&&((AuxCenterY > OFWindowY)&&(AuxCenterY < (OFWindowY+150))))
                                {

                                    bool IsInsideBorder = false;

                                    if(AuxCenterX<240) //Left side
                                        IsInsideBorder = mNew_car.InsideBorderLines( VectorRectanglesTotalMerged.at(k), BackUpLeftBorder, true);
                                    else //Rigth side
                                        IsInsideBorder = mNew_car.InsideBorderLines( VectorRectanglesTotalMerged.at(k),  BackUpRightBorder, false);

                                    if(IsInsideBorder)
                                    {
                                        TotalValidatedRectangle.push_back( VectorRectanglesTotalMerged.at(k));

                                        //In Yellow color -> RECTANGLES FILTERED BY DOUBLE RECTANGLE
                                        //cv::rectangle(AuxiliarImg,  VectorRectanglesTotalMerged.at(k).tl(),  VectorRectanglesTotalMerged.at(k).br(), cv::Scalar(0, 200, 200), 2, CV_AA);

                                    }else{

                                         //cv::rectangle(AuxiliarImg,  VectorRectanglesTotalMerged.at(k).tl(),  VectorRectanglesTotalMerged.at(k).br(), cv::Scalar(200, 200, 200), 2, CV_AA);


                                    }


                                }else{


                                    CropImage = OriginalImg( VectorRectanglesTotalMerged.at(k));
                                    cv::cvtColor(CropImage, CropImage, CV_BGR2GRAY);
                                    std::vector<cv::Point2f> prev_p;
                                    std::vector<cv::Point2f> next_p;
                                    cv::goodFeaturesToTrack(CropImage, prev_p, 25, 0.01, 7);

                                    if (prev_p.size()>0)
                                    {
                                        for (size_t i_=0; i_<prev_p.size(); i_++)
                                        {
                                            prev_p.at(i_).x+= VectorRectanglesTotalMerged.at(k).x;
                                            prev_p.at(i_).y+= VectorRectanglesTotalMerged.at(k).y;
                                        }
                                        cv::Mat Orgn_gs, Next_gs;
                                        cv::cvtColor(OriginalImg, Orgn_gs, CV_BGR2GRAY);
                                        cv::cvtColor(NextImg, Next_gs, CV_BGR2GRAY);
                                        std::vector<uchar> status_;
                                        std::vector<float> error;

                                        cv::rectangle(CheckCascadeBoth,  VectorRectanglesTotalMerged.at(k).tl(),  VectorRectanglesTotalMerged.at(k).br(), cv::Scalar(0, 200, 200), 2, CV_AA);


                                        for (int j_ = 0; j_<prev_p.size(); j_++)
                                        {
                                            cv::circle(CheckCascadeBoth, prev_p.at(j_), 3, cv::Scalar(0,200,200), -1);

                                        }

                                        cv::calcOpticalFlowPyrLK(Orgn_gs, Next_gs, prev_p, next_p, status_, error, cv::Size( VectorRectanglesTotalMerged.at(k).width,  VectorRectanglesTotalMerged.at(k).height), 5);

                                        int MasPointX, MasPointY;
                                        int AveX = 0;
                                        int AveY = 0;
                                        size_t j_ = 0;
                                        for (; j_<next_p.size(); j_++)
                                        {
                                            cv::circle(Pic, next_p.at(j_), 3, cv::Scalar(0,200,200), -1);
                                            AveX += next_p.at(j_).x;
                                            AveY += next_p.at(j_).y;

                                        }


                                        MasPointX = AveX/j_;
                                        MasPointY = AveY/j_;

                                        cv::circle(Pic, cv::Point(MasPointX,MasPointY), 5, cv::Scalar(0,0,255), -1);
                                        cv::Rect ReturnRect;

                                        ReturnRect = mNew_car.CalculateEstimatedRect(MasPointX, MasPointY);

                                        cv::rectangle(Pic, ReturnRect, cv::Scalar(0, 255, 0), 3, CV_AA);
                                        cv::rectangle(FutureMat, ReturnRect, cv::Scalar(0, 255, 0), 2, CV_AA);


                                        bool IsValid = false;

                                        int Distance;
                                        int DiffX, DiffY;

                                        for(int vr = 0; vr < ValidateRectangle.size(); vr++ )
                                        {

                                               DiffX = abs(ValidateRectangle.at(vr).x + ValidateRectangle.at(vr).width/2 - ReturnRect.x - ReturnRect.width/2);
                                               DiffY = abs(ValidateRectangle.at(vr).y + ValidateRectangle.at(vr).height/2 - ReturnRect.y - ReturnRect.height/2);
                                               Distance =  sqrt((DiffX*DiffX)+(DiffY*DiffY));

                                                //cv::rectangle(Pic, ValidateRectangle.at(vr), cv::Scalar(0, 0, 255), 2, CV_AA);

                                               if(Distance<(0.6*ReturnRect.width))
                                               {
                                                    //Inatead of directly printing the rectangle, we store it
                                                    TotalValidatedRectangle.push_back( VectorRectanglesTotalMerged.at(k));

                                                     //In Yellow color -> RECTANGLES FILTERED BY DOUBLE RECTANGLE
                                                    //cv::rectangle(AuxiliarImg,  VectorRectanglesTotalMerged.at(k).tl(),  VectorRectanglesTotalMerged.at(k).br(), cv::Scalar(0, 200, 200), 2, CV_AA);
                                                    IsValid=true;
                                                    break;

                                               }


                                        }



                                    }

                            }

                        }

                    }

                }

                    //At this point we have in TotalValidatedRectangle all the rectangles that have passed the filtering
                    //Now we filter overlaped rectangles

                    for(int m1=0; m1< TotalValidatedRectangle.size(); m1++)
                    {
                        //cv::rectangle(AuxiliarImg, TotalValidatedRectangle.at( m1).tl(), TotalValidatedRectangle.at( m1).br(), cv::Scalar(255, 0, 0), 2, CV_AA);
                        //cv::rectangle(CheckCascadeBoth, TotalValidatedRectangle.at( m1).tl(), TotalValidatedRectangle.at( m1).br(), cv::Scalar(0, 0, 255), 2, CV_AA);

                    }


                    FinalRectanglesCollection = mNew_car.DoubleRectanglesCleaner(TotalValidatedRectangle);

                    for(int m1=0; m1< FinalRectanglesCollection.size(); m1++)
                    {
                        //cv::rectangle(CheckCascadeBoth, FinalRectanglesCollection.at(m1).tl(), FinalRectanglesCollection.at(m1).br(), cv::Scalar(255, 0, 0), 2, CV_AA);
                    }

                    for(int PrintInd = 0; PrintInd < FinalRectanglesCollection.size() ; PrintInd++)
                    {


                        //cv::rectangle(AuxiliarImg, FinalRectanglesCollection.at(PrintInd).tl(), FinalRectanglesCollection.at(PrintInd).br(), cv::Scalar(0, 255, 0), 2, CV_AA);
                        //cv::rectangle(FinalImg, FinalRectanglesCollection.at(PrintInd).tl(), FinalRectanglesCollection.at(PrintInd).br(), cv::Scalar(0, 255, 0), 2, CV_AA);

                             //Here we write the file with the result
                             //ResultsFile << FinalRectanglesCollection.at(PrintInd).x << " " <<  FinalRectanglesCollection.at(PrintInd).y << " " <<  FinalRectanglesCollection.at(PrintInd).width << " " << FinalRectanglesCollection.at(PrintInd).height << " 255" <<"\n";
                             NeedToBeClosed = true;

                    }

                    if(NeedToBeClosed)
                    {
                        CounterMarkedUp++;

                    }else{
                            EmptyFiles++;
                    }


                }

                //ResultsFile.close();
                //To see how many have been printed

                //std::cout << "Curren image name: " << NameImage << " Current index: " << i << std::endl; //" Number of ouw makesup: " << CounterMarkedUp << " Empty Files: " << EmptyFiles<<" Total amount: "<< (CounterMarkedUp+EmptyFiles) << std::endl;


                cv::imshow("Predicted features next frame", Pic);
                cv::imshow("Features current frame", CheckCascadeBoth);
                cv::imshow("HaarCascade detection future frame", FutureMat);
                cv::waitKey();

            }
        }
    }

    return 0;
}


 /* This will be for something else

                                    if((!IsValid)&&(((AuxCenterX > 160)&&(AuxCenterX < 340))&&((AuxCenterY > 70)&&(AuxCenterY < 180))))
                                    {
                                        for(int l=0 ; l<5; l++)
                                        {

                                            imsetImagesNextLoop.getCurrentImg(NextNextImg);
                                            imsetImagesNextLoop.CurrentIndex++;
                                            CountForward++;

                                            if(mNew_car.HaarCascadeDetector(NextNextImg, VectorRectanglesHaarLoop, Pathxml))
                                            {

                                                for(int idxLoop=0; idxLoop<VectorRectanglesHaarLoop.size() ; idxLoop++)
                                                {

                                                        AuxCenterX = VectorRectanglesHaarLoop.at(idxLoop).x + (VectorRectanglesHaarLoop.at(idxLoop).width/2);
                                                        AuxCenterY = VectorRectanglesHaarLoop.at(idxLoop).y + (VectorRectanglesHaarLoop.at(idxLoop).width/2);

                                                        if((((AuxCenterX > 160)&&(AuxCenterX < 340))&&((AuxCenterY > 70)&&(AuxCenterY < 180))))
                                                            ValidateRectangleLoop.push_back(VectorRectanglesHaarLoop.at(idxLoop));


                                                }


                                            }


                                            for(int vr = 0; vr < ValidateRectangleLoop.size(); vr++ )
                                            {

                                               DiffX = abs(ValidateRectangleLoop.at(vr).x + ValidateRectangleLoop.at(vr).width/2 - ReturnRect.x - ReturnRect.width/2);
                                               DiffY = abs(ValidateRectangleLoop.at(vr).y + ValidateRectangleLoop.at(vr).height/2 - ReturnRect.y - ReturnRect.height/2);
                                               Distance =  sqrt((DiffX*DiffX)+(DiffY*DiffY));

                                               if(Distance<(0.3*ReturnRect.width)) //More accuracy
                                               {
                                                    //Inatead of directly printing the rectangle, we store it
                                                    TotalValidatedRectangle.push_back(VectorRectanglesHaar.at(k));

                                                     //In Yellow color -> RECTANGLES FILTERED BY DOUBLE RECTANGLE
                                                    cv::rectangle(AuxiliarImg, VectorRectanglesHaar.at(k).tl(), VectorRectanglesHaar.at(k).br(), cv::Scalar(0, 200, 200), 2, CV_AA);
                                                    l=5;
                                                    break;

                                               }

                                            }

                                        }


                                    }

                                    imsetImagesNextLoop.CurrentIndex -=  CountForward;
                                    */
