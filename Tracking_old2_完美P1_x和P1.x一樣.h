#pragma once
#include"struct.h"
#include"Process.h"
#include "CCL.h"


void histogram() {

    // 目前可以讓使用者使用500個點畫線
    // 當「畫筆的點數以用盡」後，再容許50個點的緩衝時間
    // 定位失敗，否則強制重新定位
    if ( g_Point.size() + 50 >= 500 || // 畫筆點數用盡
         countTheZeroCandidateFrame >= 20 ) { // 卡無窮迴圈
        //if (0){
        // flag = 1; //回去抓ROI
        // cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
        if ( g_Point.size() + 50 >= 500 )
            cout << "畫筆的點數以用盡，強制回重新定位!!" << endl;

        else if ( countTheZeroCandidateFrame >= 20 )
            cout << "卡無窮迴圈，強制回重新定位!!" << endl;

        countTheZeroCandidateFrame = 0; // 初始化

        flag = 1; // 強制回重定位

        g_Point.clear(); // 清除先前的點數!!

    } // if


    if ( skin_total3 == 0 ) //防止分母等於0的情況
    {
        skin_total3 = 1;
    }

    if ( skin_total != 0 )
        if ( abs( skin_total / skin_total3 ) > 10 ) {
            flag = 1; //回去抓ROI
            P1_x = 0, P1_y = 0;
            P2_x = 0, P2_y = 0;
            skin_total = 0, skin_total2 = 0, skin_total3 = 0;
            staticCount = 0;
            click_count = 0;
            click_flag = 0;
            voice_flag = 0;
            wait = 0;

            g_Point.clear();
        }
    wait--;

    PreP1x = P1_x, PreP1y = P1_y;
    double ave_0 = 0, ave_1 = 0, ave_2 = 0, ave_3 = 0, ave_4 = 0, ave_5 = 0, ave_6 = 0, ave_7 = 0, ave_8 = 0;
    double LBPave_0 = 0, LBPave_1 = 0, LBPave_2 = 0, LBPave_3 = 0, LBPave_4 = 0, LBPave_5 = 0, LBPave_6 = 0, LBPave_7 = 0, LBPave_8 = 0, LBPave_9 = 0, LBPave_10 = 0, LBPave_11 = 0, LBPave_12 = 0, LBPave_13 = 0, LBPave_14 = 0;
    double gave_0 = 0, gave_1 = 0, gave_2 = 0, gave_3 = 0, gave_4 = 0, gave_5 = 0, gave_6 = 0, gave_7 = 0, gave_8 = 0, gave_9 = 0, gave_10 = 0, gave_11 = 0, gave_12 = 0, gave_13 = 0, gave_14 = 0, gave_15 = 0;
    double candicate = 0;  //候選者
    double candicateLBP = 0;//候選者
    double eligibility = 9999; //當選者
    double eligibilityLBP = 9999;
    //double candicateLBP2 = 9999;


    if ( once == 0 ) // 特徵只須抓一次!!
    {


        ave0 = 0, ave1 = 0, ave2 = 0, ave3 = 0, ave4 = 0, ave5 = 0, ave6 = 0, ave7 = 0, ave8 = 0;
        total = 0;
        histogram0 = 0, histogram1 = 0, histogram2 = 0, histogram3 = 0, histogram4 = 0, histogram5 = 0, histogram6 = 0, histogram7 = 0, histogram8 = 0;
        LBPhistogram0 = 0, LBPhistogram1 = 0, LBPhistogram2 = 0, LBPhistogram3 = 0, LBPhistogram4 = 0, LBPhistogram5 = 0, LBPhistogram6 = 0, LBPhistogram7 = 0, LBPhistogram8 = 0;
        skin_total = 0;

        for ( int i = ( P1_x >= 0 ) ? P1_x : 0; // 避免 ROI 範圍 過小 超過 frame 的最邊界
              i < P1_x + ( P2_x - P1_x ) && 
              i < width_m ; // 避免 ROI 範圍 過大 超過 frame 的最邊界
              i ++ )
              for ( int j = ( P1_y >= 0 ) ? P1_y : 0; // 避免 ROI 範圍 過小 超過 frame 的最邊界
                    j < P1_y + ( P2_y - P1_y ) && 
                    j < height_m ; // 避免 ROI 範圍 過大 超過 frame 的最邊界
                    j ++ ) {
        //for ( int i = P1_x + ( P2_x - P1_x ) / 5; i<P1_x + ( P2_x - P1_x ) - ( P2_x - P1_x ) / 3 && i < width_m; i++ )
        //    //for (int i = m + (P2_x - P1_x) ; i<m + (P2_x - P1_x) ; i++)
        //    for ( int j = P1_y + ( P2_y - P1_y ) / 2; j<P1_y + ( P2_y - P1_y ) && j < height_m; j++ ) {
                //for (int i = P1_x + (P2_x - P1_x) / 3; i<P2_x - (P2_x - P1_x) / 6 && i < width_m; i++)  // 抓ROI內的特徵(抓ROI方框中下半部分?) //內縮 
                //	//for (int i = P1_x ; i<P2_x ; i++)  // 抓ROI內的特徵(抓ROI方框中下半部分?) //內縮 
                //for (int j = P1_y + (P2_y - P1_y) /2; j < P2_y && j < height_m ; j++)//下半部
                //{




                if ( YCbCr_Cr [ i ] [ j ] > 133 && YCbCr_Cr [ i ] [ j ] < 174 && YCbCr_Cb [ i ] [ j ] > 77 && YCbCr_Cb [ i ] [ j ] < 127 ) {
                    skin_total++;
                    total++;

                    //--------------------------------HOG---------------------------------------------
                    int Gx = 0, Gy = 0, Gxy = 0; //計算梯度值
                    if ( i != 0 && i != width_m && j != 0 && j != height_m ) //i,j>0 且不要是最邊點
                        //Gx = YCbCr_Y[i + 1][j] - YCbCr_Y[i - 1][j];  //左右方向灰度值相減
                        Gy = YCbCr_Y [ i ] [ j + 1 ] - YCbCr_Y [ i ] [ j - 1 ];	 //上下相減
                    int atan = 0;
                    atan = atan2( YCbCr_Y [ i ] [ j + 1 ], YCbCr_Y [ i ] [ j - 1 ] ) * 180 / 3.14;
                    //cout << "Gy=========:: " << atan << endl;



                    if ( -40 <= Gy && Gy < -30 )     //將梯度歸納到八個方向中
                        histogram0++;
                    else if ( -30 <= Gy && Gy < -20 )
                        histogram1++;
                    else if ( -20 <= Gy && Gy < -10 )
                        histogram2++;
                    else if ( -10 <= Gy && Gy < 0 )
                        histogram3++;
                    else if ( 0 <= Gy && Gy < 10 )
                        histogram4++;
                    else if ( 10 <= Gy && Gy < 20 )
                        histogram5++;
                    else if ( 20 <= Gy && Gy < 30 )
                        histogram6++;
                    else if ( 30 <= Gy && Gy < 40 )
                        histogram7++;
                    else
                        total--;





                    //system("pause");
                    // ------------------ LBP 256 bits -----------------------
                    int feature = 0, feature2 = 0, center;
                    int mean = 0;	//標準差
                    int root2 = 128;
                    int sum = 0;
                    int p [ 9 ] = { 0 };
                    //getPixel(width,height)

                    p [ 1 ] = YCbCr_Y [ i ] [ j - 1 ];
                    p [ 2 ] = YCbCr_Y [ i - 1 ] [ j - 1 ];
                    p [ 3 ] = YCbCr_Y [ i - 1 ] [ j ];
                    p [ 4 ] = YCbCr_Y [ i - 1 ] [ j + 1 ];
                    p [ 5 ] = YCbCr_Y [ i ] [ j + 1 ];
                    p [ 6 ] = YCbCr_Y [ i + 1 ] [ j + 1 ];
                    p [ 7 ] = YCbCr_Y [ i + 1 ] [ j ];
                    p [ 8 ] = YCbCr_Y [ i + 1 ] [ j - 1 ];

                    center = YCbCr_Y [ i ] [ j ];


                    //-------計算標準差-------
                    mean = ( p [ 1 ] + p [ 2 ] + p [ 3 ] + p [ 4 ] + p [ 5 ] + p [ 6 ] + p [ 7 ] + p [ 8 ] + center ) / 9;

                    for ( int z = 1; z <= 8; z++ ) {


                        if ( p [ z ] - mean >= 0 )
                            sum += root2;

                        root2 /= 2;
                    }

                    // LBP 直方圖累加



                    /*	         LBPbetter ={   7,  14, 15, 28, 30, 31,
                    48, 56, 60, 62, 63, 96, 112, 120, 124, 126,  131, 135, 143,
                    159, 192, 193, 195, 199, 207, 224, 225, 227, 231, 239, 240, 241, 247,
                    　        248 }     45       */


                    /*	if (sum == 7 || sum == 14 || sum == 15 || sum == 28 || sum == 30 || sum == 31 || sum == 48 || sum == 56 || sum == 60 || sum == 62 || sum == 63 || sum == 96 || sum == 112 || sum == 120 || sum == 124 || sum == 126 ||  sum == 131 || sum == 135 || sum == 143 || sum == 159 ||  sum == 192 || sum == 193 || sum == 195 || sum == 199 || sum == 207 ||  sum == 224 || sum == 225 || sum == 227 || sum == 231 || sum == 239 || sum == 240 || sum == 241 || sum == 247 || sum == 248)
                    hist[sum]++;
                    else
                    hist[255]++;
                    */




                    if ( sum <= 255 && sum > 239 )
                        LBPhistogram0++;
                    else if ( sum <= 239 && sum > 223 )
                        LBPhistogram1++;
                    else if ( sum <= 223 && sum > 207 )
                        LBPhistogram2++;
                    else if ( sum <= 207 && sum > 191 )
                        LBPhistogram3++;
                    else if ( sum <= 191 && sum > 175 )
                        LBPhistogram4++;
                    else if ( sum <= 175 && sum > 159 )
                        LBPhistogram5++;
                    else if ( sum <= 159 && sum > 143 )
                        LBPhistogram6++;
                    else if ( sum <= 143 && sum > 127 )
                        LBPhistogram7++;
                    else if ( sum <= 127 && sum > 111 )
                        LBPhistogram8++;
                    else if ( sum <= 111 && sum > 95 )
                        LBPhistogram9++;
                    else if ( sum <= 95 && sum > 79 )
                        LBPhistogram10++;
                    else if ( sum <= 79 && sum > 63 )
                        LBPhistogram11++;
                    else if ( sum <= 63 && sum > 47 )
                        LBPhistogram12++;
                    else if ( sum <= 47 && sum > 31 )
                        LBPhistogram13++;
                    else if ( sum <= 31 && sum >= 0 )
                        LBPhistogram14++;


                    // ------------------ END LBP 結束!! -----------------------

                }









            }




        //int temptemp = 0;
        //for (int i = P1_x + (P2_x - P1_x) / 3; i<P2_x - (P2_x - P1_x) / 6; i++)       //特徵取ROI內縮 可以改善太容易被臉追掉問題 嗎
        //for (int j = P1_y + (P2_y - P1_y) / 2; j<P2_y ; j++)
        //{



        if ( total == 0 )
            total = 1;



        ave0 = histogram0 / total;
        ave1 = histogram1 / total;
        ave2 = histogram2 / total;
        ave3 = histogram3 / total;
        ave4 = histogram4 / total;
        ave5 = histogram5 / total;
        ave6 = histogram6 / total;
        ave7 = histogram7 / total;
        ave8 = histogram8 / total;



        LBPave0 = LBPhistogram0 / total;
        LBPave1 = LBPhistogram1 / total;
        LBPave2 = LBPhistogram2 / total;
        LBPave3 = LBPhistogram3 / total;
        LBPave4 = LBPhistogram4 / total;
        LBPave5 = LBPhistogram5 / total;
        LBPave6 = LBPhistogram6 / total;
        LBPave7 = LBPhistogram7 / total;
        LBPave8 = LBPhistogram8 / total;
        LBPave9 = LBPhistogram9 / total;
        LBPave10 = LBPhistogram10 / total;
        LBPave11 = LBPhistogram11 / total;
        LBPave12 = LBPhistogram12 / total;
        LBPave13 = LBPhistogram13 / total;
        LBPave14 = LBPhistogram14 / total;



        disbetweenfingertip = P2_y - P1_y;
        Sobelcount2 = 0;
        for ( int i = P1_x; i < P2_x; i++ ) //sobel累加 //邊界
        {
            for ( int j = P1_y; j < P1_y + ( P2_y - P1_y ) / 3 && j < height_m; j++ ) {
                Y [ i ] [ j ] = abs( YCbCr_Y [ i - 1 ] [ j - 1 ] * ( -1 ) + YCbCr_Y [ i ] [ j - 1 ] * ( -2 ) + YCbCr_Y [ i + 1 ] [ j - 1 ] * ( -1 )
                                     + YCbCr_Y [ i - 1 ] [ j + 1 ] * 1 + YCbCr_Y [ i ] [ j + 1 ] * 2 + YCbCr_Y [ i + 1 ] [ j + 1 ] * 1 );

                Cb [ i ] [ j ] = abs( YCbCr_Cb [ i - 1 ] [ j - 1 ] * ( -1 ) + YCbCr_Cb [ i ] [ j - 1 ] * ( -2 ) + YCbCr_Cb [ i + 1 ] [ j - 1 ] * ( -1 )
                                      + YCbCr_Cb [ i - 1 ] [ j + 1 ] * 1 + YCbCr_Cb [ i ] [ j + 1 ] * 2 + YCbCr_Cb [ i + 1 ] [ j + 1 ] * 1 );

                Cr [ i ] [ j ] = abs( YCbCr_Cr [ i - 1 ] [ j - 1 ] * ( -1 ) + YCbCr_Cr [ i ] [ j - 1 ] * ( -2 ) + YCbCr_Cr [ i + 1 ] [ j - 1 ] * ( -1 )
                                      + YCbCr_Cr [ i - 1 ] [ j + 1 ] * 1 + YCbCr_Cr [ i ] [ j + 1 ] * 2 + YCbCr_Cr [ i + 1 ] [ j + 1 ] * 1 );


                SobelFrame [ i ] [ j ] = Y [ i ] [ j ] + Cb [ i ] [ j ] + Cr [ i ] [ j ];
                if ( SobelFrame [ i ] [ j ] > 50 && YCbCr_Cr [ i ] [ j ] > 133 && YCbCr_Cr [ i ] [ j ] < 174 && YCbCr_Cb [ i ] [ j ] > 77 && YCbCr_Cb [ i ] [ j ] < 127 )
                    Sobelcount2++;
                //else
                //SobelFrame[i][j] = 0;
            }
        }

        once ++;  // 特徵只需要抓一次，執行完這個，就不會再進這個迴圈。
    }

    // cout << "-------------------------**------------****----------------" << endl;

    //放大框架掃描追蹤
    skin_total3 = 0;


    int thistogram0 = 0;
    int thistogram1 = 0;
    int thistogram2 = 0;
    int thistogram3 = 0;
    int thistogram4 = 0;
    int thistogram5 = 0;
    int thistogram6 = 0;
    int thistogram7 = 0;
    int	thistogram8 = 0;



    int LBPthistogram0 = 0;
    int LBPthistogram1 = 0;
    int LBPthistogram2 = 0;
    int LBPthistogram3 = 0;
    int LBPthistogram4 = 0;
    int LBPthistogram5 = 0;
    int LBPthistogram6 = 0;
    int LBPthistogram7 = 0;
    int	LBPthistogram8 = 0;
    int LBPthistogram9 = 0;
    int LBPthistogram10 = 0;
    int LBPthistogram11 = 0;
    int LBPthistogram12 = 0;
    int LBPthistogram13 = 0;
    int LBPthistogram14 = 0;


    Threshold = 10.0;

    int DiffFramecount = 0;

    //cvRectangle(frame_320x240, cvPoint((P1_x - (P2_x - P1_x) / 2) * 2, (P1_y - (P2_y - P1_y) / 2) * 2), cvPoint((P2_x + (P2_x - P1_x) / 4) * 2, (P2_y + (P2_y - P1_y) / 4) * 2), CV_RGB(0, 255, 0), 1, CV_AA, 1);


    int feature1 = 0; //角點特徵

    // 累計放大區塊裡，候選區塊的總數(放大區塊m*n裡面，可能會有數個候選區塊)
    int countOfFrameCandidate = 0;

    // 更改放大區塊的大小
    int big_roi_distance = ( P2.x - P1.x ) / 2.5;

    // 放大框架 的 候選點 初始化
    int big_roi_p1_x = P1.x - big_roi_distance; // 放大框架的 p1.x
    int big_roi_p1_y = P1.y - big_roi_distance; // 放大框架的 p1.y
    int big_roi_p2_x = P2.x + big_roi_distance; // 放大框架的 p2.x
    int big_roi_p2_y = P2.y + big_roi_distance; // 放大框架的 p2.y

    int nnn = 10 ;

    // 顯示 放大框架的範圍
    cvRectangle( frame_320x240,
                 cvPoint( big_roi_p1_x, big_roi_p1_y ), // P1 點 再向左上放大
                 cvPoint( big_roi_p2_x, big_roi_p2_y ), // P2 點 再向右下放大
                 CV_RGB( 0, 200, 200 ),
                 1,
                 CV_AA,
                 0 );

    // 由上到下，由左到右                 
    for ( int m = ( big_roi_p1_x >= 0 ) ? big_roi_p1_x : 0;  // 避免放大框架超出原圖範圍
          m < big_roi_p2_x && 
          m < width_m; // 避免 放大框架 超過 frame 的最邊界
          m += 5 ) // 每跳3格，加快速度

          for ( int n = ( big_roi_p1_y >= 0 ) ? big_roi_p1_y : 0;
                n < big_roi_p2_y && 
                n < height_m; // 避免 放大框架 超過 frame 的最邊界
                n += 5 ) // 每跳3格，加快速度
    {

        //// 由上到下，有左到右                 
        //for (int m = (P1_x - (P2_x - P1_x) >= 0) ? P1_x - (P2_x - P1_x) : 0;  // 避免放大框架超出原圖範圍
        //	m < P2_x + (P2_x - P1_x) / 3 - (P2_x - P1_x) && m < width_m;
        //		 m+=3) // 每跳3格，加快速度

        //		 for (int n = (P1_y - (P2_y - P1_y) / 4 >= 0) ? P1_y - (P2_y - P1_y) / 4 : 0 ;
        //			 n < P2_y + (P2_y - P1_y) / 3 - (P2_y - P1_y) && n <  height_m ;
        //				  n+=3) // 每跳3格，加快速度
        //{


        // 初始 背景相減
        DiffFramecount = 0;
        feature1 = 0;

        histogram0 = 0;
        histogram1 = 0;
        histogram2 = 0;
        histogram3 = 0;
        histogram4 = 0;
        histogram5 = 0;
        histogram6 = 0;
        histogram7 = 0;
        histogram8 = 0;


        LBPhistogram0 = 0;
        LBPhistogram1 = 0;
        LBPhistogram2 = 0;
        LBPhistogram3 = 0;
        LBPhistogram4 = 0;
        LBPhistogram5 = 0;
        LBPhistogram6 = 0;
        LBPhistogram7 = 0;
        LBPhistogram8 = 0;
        LBPhistogram9 = 0;
        LBPhistogram10 = 0;
        LBPhistogram11 = 0;
        LBPhistogram12 = 0;
        LBPhistogram13 = 0;
        LBPhistogram14 = 0;

        skin_total2 = 0;

        //// 顯示 將被掃描 的 所有區塊
        //if ( m + ( P2_x - P1_x ) < big_roi_p2_x + big_roi_distance &&
        //     n + ( P2_y - P1_y ) < big_roi_p2_y + big_roi_distance ) {
        //    cvRectangle( frame_320x240,
        //                 cvPoint( m, n ), // P1 點 再向左上放大
        //                 cvPoint( m + ( P2_x - P1_x ), n + ( P2_y - P1_y ) ), // P2 點 再向右下放大
        //                 CV_RGB( nnn, nnn, nnn ),
        //                 1,
        //                 CV_AA,
        //                 0 );
        //} // if
        
        for ( int i = m;
              i < m + ( P2_x - P1_x ) &&
              m + ( P2_x - P1_x ) < big_roi_p2_x + big_roi_distance && // 避免超過 放大框架的範圍
              i < width_m; // 避免超過 frame 的範圍
              i ++ )
            //for (int i = m + (P2_x - P1_x) ; i<m + (P2_x - P1_x) ; i++)
            for ( int j = n ; 
                  j < n + ( P2_y - P1_y ) &&
                  n + ( P2_y - P1_y ) < big_roi_p2_y + big_roi_distance && // 避免超過 放大框架的範圍
                  j < height_m ; // 避免超過 frame 的範圍
                  j ++ ) {

        //for ( int i = m + ( P2_x - P1_x ) / 5; i<m + ( P2_x - P1_x ) - ( P2_x - P1_x ) / 3 && i < width_m; i++ )
        //    //for (int i = m + (P2_x - P1_x) ; i<m + (P2_x - P1_x) ; i++)
        //    for ( int j = n + ( P2_y - P1_y ) / 2; j<n + ( P2_y - P1_y ) && j < height_m; j++ ) {
                // ----------------------時間差相減法---------開跑囉!!-------------------
                int Diff_Value = 0;
                Diff_Value = abs( YCbCr_Y [ i ] [ j ] - Pre [ i ] [ j ] ); // abs取整數絕對值                Pre [ i ] [ j ] = YCbCr_Y [ i ] [ j ];

                if ( Diff_Value > Threshold ) {
                    DiffFramecount ++; //超過T則做累加
                }

                // ----------------------時間差相減法---------結束!!-------------------


                if ( YCbCr_Cr [ i ] [ j ] > 133 && YCbCr_Cr [ i ] [ j ] < 174 && YCbCr_Cb [ i ] [ j ] > 77 && YCbCr_Cb [ i ] [ j ] < 127 ) {
                    skin_total2 ++;
                    int Gx = 0, Gy = 0, Gxy = 0;
                    if ( i != 0 && i != width_m && j != 0 && j != height_m )
                        Gx = YCbCr_Y [ i + 1 ] [ j ] - YCbCr_Y [ i - 1 ] [ j ];
                    Gy = YCbCr_Y [ i ] [ j + 1 ] - YCbCr_Y [ i ] [ j - 1 ];	 //上下相減

                    //cout<<orientation<<endl;
                    //cout<<Gx<<" "<<Gy<<endl;

                    if ( -40 <= Gy && Gy < -30 )     //將梯度歸納到八個方向中
                        histogram0++;
                    else if ( -30 <= Gy && Gy < -20 )
                        histogram1++;
                    else if ( -20 <= Gy && Gy < -10 )
                        histogram2++;
                    else if ( -10 <= Gy && Gy < 0 )
                        histogram3++;
                    else if ( 0 <= Gy && Gy < 10 )
                        histogram4++;
                    else if ( 10 <= Gy && Gy < 20 )
                        histogram5++;
                    else if ( 20 <= Gy && Gy < 30 )
                        histogram6++;
                    else if ( 30 <= Gy && Gy < 40 )
                        histogram7++;
                    else
                        ;

                    // ------------------ LBP 256 bits -----------------------

                    int feature = 0, feature2 = 0, center;
                    int mean = 0;
                    int root2 = 128;
                    int sum = 0;
                    int p [ 9 ] = { 0 };
                    //getPixel(width,height)

                    p [ 1 ] = YCbCr_Y [ i ] [ j - 1 ];
                    p [ 2 ] = YCbCr_Y [ i - 1 ] [ j - 1 ];
                    p [ 3 ] = YCbCr_Y [ i - 1 ] [ j ];
                    p [ 4 ] = YCbCr_Y [ i - 1 ] [ j + 1 ];
                    p [ 5 ] = YCbCr_Y [ i ] [ j + 1 ];
                    p [ 6 ] = YCbCr_Y [ i + 1 ] [ j + 1 ];
                    p [ 7 ] = YCbCr_Y [ i + 1 ] [ j ];
                    p [ 8 ] = YCbCr_Y [ i + 1 ] [ j - 1 ];

                    center = YCbCr_Y [ i ] [ j ];


                    //-------計算標準差-------
                    mean = ( p [ 1 ] + p [ 2 ] + p [ 3 ] + p [ 4 ] + p [ 5 ] + p [ 6 ] + p [ 7 ] + p [ 8 ] + center ) / 9;

                    for ( int z = 1; z <= 8; z++ ) {

                        if ( p [ z ] - mean >= 0 )
                            sum += root2;

                        root2 /= 2;
                    }

                    // LBP 直方圖累加!!

                    /*if (sum == 7 || sum == 14 || sum == 15 || sum == 28 || sum == 30 || sum == 31 || sum == 48 || sum == 56 || sum == 60 || sum == 62 || sum == 63 || sum == 96 || sum == 112 || sum == 120 || sum == 124 || sum == 126 || sum == 131 || sum == 135 || sum == 143 || sum == 159 || sum == 192 || sum == 193 || sum == 195 || sum == 199 || sum == 207 || sum == 224 || sum == 225 || sum == 227 || sum == 231 || sum == 239 || sum == 240 || sum == 241 || sum == 247 || sum == 248)
                    hist2[sum]++;
                    else
                    hist2[255]++;
                    */

                    if ( sum <= 255 && sum > 239 )
                        LBPhistogram0++;
                    else if ( sum <= 239 && sum > 223 )
                        LBPhistogram1++;
                    else if ( sum <= 223 && sum > 207 )
                        LBPhistogram2++;
                    else if ( sum <= 207 && sum > 191 )
                        LBPhistogram3++;
                    else if ( sum <= 191 && sum > 175 )
                        LBPhistogram4++;
                    else if ( sum <= 175 && sum > 159 )
                        LBPhistogram5++;
                    else if ( sum <= 159 && sum > 143 )
                        LBPhistogram6++;
                    else if ( sum <= 143 && sum > 127 )
                        LBPhistogram7++;
                    else if ( sum <= 127 && sum > 111 )
                        LBPhistogram8++;
                    else if ( sum <= 111 && sum > 95 )
                        LBPhistogram9++;
                    else if ( sum <= 95 && sum > 79 )
                        LBPhistogram10++;
                    else if ( sum <= 79 && sum > 63 )
                        LBPhistogram11++;
                    else if ( sum <= 63 && sum > 47 )
                        LBPhistogram12++;
                    else if ( sum <= 47 && sum > 31 )
                        LBPhistogram13++;
                    else if ( sum <= 31 && sum >= 0 )
                        LBPhistogram14++;

                }

            }

        // 對 HOG 正規化!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        if ( total == 0 ) total = 1;//防止分母為0
        ave_0 = histogram0 / total;
        ave_1 = histogram1 / total;
        ave_2 = histogram2 / total;
        ave_3 = histogram3 / total;
        ave_4 = histogram4 / total;
        ave_5 = histogram5 / total;
        ave_6 = histogram6 / total;
        ave_7 = histogram7 / total;
        ave_8 = histogram8 / total;


        LBPave_0 = LBPhistogram0 / total;
        LBPave_1 = LBPhistogram1 / total;
        LBPave_2 = LBPhistogram2 / total;
        LBPave_3 = LBPhistogram3 / total;
        LBPave_4 = LBPhistogram4 / total;
        LBPave_5 = LBPhistogram5 / total;
        LBPave_6 = LBPhistogram6 / total;
        LBPave_7 = LBPhistogram7 / total;
        LBPave_8 = LBPhistogram8 / total;
        LBPave_9 = LBPhistogram9 / total;
        LBPave_10 = LBPhistogram10 / total;
        LBPave_11 = LBPhistogram11 / total;
        LBPave_12 = LBPhistogram12 / total;
        LBPave_13 = LBPhistogram13 / total;
        LBPave_14 = LBPhistogram14 / total;

        // END 正規化結束!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!




        //disbetweenfingertip2 = P2_y - P1_y;
        //候選者 //越小越趨近於當選者
        //ROI_shape = (P2_y - P1_y) / (P2_x - P1_x);




        // LBP + HOG
        /*candicate = abs(ave0 - ave_0) + abs(ave1 - ave_1) + abs(ave2 - ave_2) + abs(ave3 - ave_3) + abs(ave4 - ave_4) + abs(ave5 - ave_5) + abs(ave6 - ave_6) + abs(ave7 - ave_7) + abs(ave8 - ave_8) +
        abs(hist[1] - hist2[1]) + abs(hist[2] - hist2[2]) + abs(hist[3] - hist2[3]) + abs(hist[4] - hist2[4]) + abs(hist[5] - hist2[5]) + abs(hist[6] - hist2[6]) + abs(hist[7] - hist2[7]) + abs(hist[8] - hist2[8]);
        */

        // LBP
        //candicateLBP = abs(hist[1] - hist2[1]) + abs(hist[2] - hist2[2]) + abs(hist[3] - hist2[3]) + abs(hist[4] - hist2[4]) + abs(hist[5] - hist2[5]) + abs(hist[6] - hist2[6]) + abs(hist[7] - hist2[7]) + abs(hist[8] - hist2[8]);

        candicateLBP = 0;


        candicateLBP = abs( LBPave0 - LBPave_0 ) + abs( LBPave1 - LBPave_1 ) + abs( LBPave2 - LBPave_2 ) + abs( LBPave3 - LBPave_3 ) + abs( LBPave4 - LBPave_4 ) + abs( LBPave5 - LBPave_5 ) + abs( LBPave6 - LBPave_6 ) + abs( LBPave7 - LBPave_7 ) + abs( LBPave8 - LBPave_8 ) + abs( LBPave9 - LBPave_9 ) + abs( LBPave10 - LBPave_10 ) + abs( LBPave11 - LBPave_11 ) + abs( LBPave12 - LBPave_12 ) + abs( LBPave13 - LBPave_13 ) + abs( LBPave14 - LBPave_14 );

        //candicate = abs(ave0 - ave_0) + abs(ave1 - ave_1) + abs(ave2 - ave_2) + abs(ave3 - ave_3) + abs(ave4 - ave_4) + abs(ave5 - ave_5) + abs(ave6 - ave_6) + abs(ave7 - ave_7) + abs(ave8 - ave_8);
        //candicateLBP = abs(hist[1] - hist11[1]) + abs(hist[2] - hist11[2]) + abs(hist[3] - hist11[3]) + abs(hist[4] - hist11[4]) + abs(hist[5] - hist11[5]) + abs(hist[6] - hist11[6]) + abs(hist[7] - hist11[7]) + abs(hist[8] - hist11[8]) + abs(hist[9] - hist11[9]) + abs(hist[10] - hist11[10]) + abs(hist[11] - hist11[11]) + abs(hist[12] - hist11[12]) + abs(hist[13] - hist11[13]) + abs(hist[14] - hist11[14]) + abs(hist[15] - hist11[15]) + abs(hist2[1] - hist22[1]) + abs(hist2[2] - hist22[2]) + abs(hist2[3] - hist22[3]) + abs(hist2[4] - hist22[4]) + abs(hist2[5] - hist22[5]) + abs(hist2[6] - hist22[6]) + abs(hist2[7] - hist22[7]) + abs(hist2[8] - hist22[8]) + abs(hist2[9] - hist22[9]) + abs(hist2[10] - hist22[10]) + abs(hist2[11] - hist22[11]) + abs(hist2[12] - hist22[12]) + abs(hist2[13] - hist22[13]) + abs(hist2[14] - hist22[14]) + abs(hist2[15] - hist22[15]); //+ abs(gave0 - gave_0) + abs(gave1 - gave_1) + abs(gave2 - gave_2) + abs(gave3 - gave_3) + abs(gave4 - gave_4) + abs(gave5 - gave_5) + abs(gave6 - gave_6) + abs(gave7 - gave_7) + abs(gave8 - gave_8) + abs(gave9 - gave_9) + abs(gave10 - gave_10) + abs(gave11 - gave_11) + abs(gave12 - gave_12) + abs(gave13 - gave_13) + abs(gave14 - gave_14) + abs(gave15 - gave_15)


        // HOG
        candicate = abs( ave0 - ave_0 ) + abs( ave1 - ave_1 ) + abs( ave2 - ave_2 ) + abs( ave3 - ave_3 ) + abs( ave4 - ave_4 ) + abs( ave5 - ave_5 ) + abs( ave6 - ave_6 ) + abs( ave7 - ave_7 ) + abs( ave8 - ave_8 );



        /*cout << "!!!!! HOG( " << candicate << " )!!!!!!!!!" << endl ;
        cout << "!!!!! LBP( " << abs(hist[1] - hist2[1]) + abs(hist[2] - hist2[2]) + abs(hist[3] - hist2[3]) + abs(hist[4] - hist2[4]) + abs(hist[5] - hist2[5]) + abs(hist[6] - hist2[6]) + abs(hist[7] - hist2[7]) + abs(hist[8] - hist2[8]) << " )!!!!!!!!!" << endl;

        system("pause");*/

        /*	cout << "------------------------------------------------------------------------------------------" << endl;
        cout << "------------------------------------------------------------------------------------------" << endl;
        cout << "------------------------------------------------------------------------------------------" << endl;
        cout << "xxxxxxxxxxx!!!!" << endl;
        cout << "!!!!! hog( " << candicate << " )!!!!!!!!!" << endl;
        cout << "!!!!! lbp( " << candicateLBP << " )!!!!!!!!!" << endl;
        cout << "------------------------------------------------------------------------------------------" << endl;
        cout << "------------------------------------------------------------------------------------------" << endl;
        cout << "------------------------------------------------------------------------------------------" << endl;
        system("pause");*/


        //for (int j = n + (P2_y - P1_y) / 2; j<n + (P2_y - P1_y) && j < height_m; j++)



        int skin_det1 = 0, skin_det2 = 0, skin_det3 = 0;

        for ( int i = m + ( P2_x - P1_x ) / 5; i<m + ( P2_x - P1_x ) - ( P2_x - P1_x ) / 3 && i < width_m; i++ )
            if ( YCbCr_Cr [ i ] [ ( int ) ( n + ( P2_y - P1_y )*0.25 ) ] > 133 && // 三個區段之手部
                 YCbCr_Cr [ i ] [ ( int ) ( n + ( P2_y - P1_y )*0.25 ) ] < 174 &&
                 YCbCr_Cb [ i ] [ ( int ) ( n + ( P2_y - P1_y )*0.25 ) ] > 77 &&
                 YCbCr_Cb [ i ] [ ( int ) ( n + ( P2_y - P1_y )*0.25 ) ] < 127 )
                 skin_det1++;

        for ( int i = m + ( P2_x - P1_x ) / 5; i<m + ( P2_x - P1_x ) - ( P2_x - P1_x ) / 3 && i < width_m; i++ )
            if ( YCbCr_Cr [ i ] [ ( int ) ( n + ( P2_y - P1_y )*0.5 ) ] > 133 &&
                 YCbCr_Cr [ i ] [ ( int ) ( n + ( P2_y - P1_y )*0.5 ) ] < 174 &&
                 YCbCr_Cb [ i ] [ ( int ) ( n + ( P2_y - P1_y )*0.5 ) ] > 77 &&
                 YCbCr_Cb [ i ] [ ( int ) ( n + ( P2_y - P1_y )*0.5 ) ] < 127 )
                 skin_det2++;

        for ( int i = m + ( P2_x - P1_x ) / 5; i<m + ( P2_x - P1_x ) - ( P2_x - P1_x ) / 3 && i < width_m; i++ )
            if ( YCbCr_Cr [ i ] [ ( int ) ( n + ( P2_y - P1_y )*0.75 ) ] > 133 &&
                 YCbCr_Cr [ i ] [ ( int ) ( n + ( P2_y - P1_y )*0.75 ) ] < 174 &&
                 YCbCr_Cb [ i ] [ ( int ) ( n + ( P2_y - P1_y )*0.75 ) ] > 77 &&
                 YCbCr_Cb [ i ] [ ( int ) ( n + ( P2_y - P1_y )*0.75 ) ] < 127 )
                 skin_det3++;



        // ------------------------------------- ROI 掃描結束 -----------------------------------------

        //cout << "skin_det======" << skin_det1 << "----------"<< skin_det2 <<"------------"<<skin_det3<< endl;

        if ( candicate <= eligibility && DiffFramecount >= 60 && candicateLBP < eligibilityLBP *1.1 && skin_det1 >= 0 && skin_det2 >= 0 && skin_det3 >= 5 ) {
            //cout << "eligibility:" << eligibility << endl;

            if ( DiffFramecount <= 200 ) {
                
                cout << "--------------========------------------------" << endl;
                cout << "candicate:" << candicate << endl;
                cout << "DiffFramecount:" << DiffFramecount << endl;
                cout << "candicateLBP:" << candicateLBP << endl;
                cout << "skin_det:" << skin_det1 + skin_det2 + skin_det3 << endl;
                cout << "--------------========------------------------" << endl;

            } // if
            

            //cout << "--------------------------------" << skin_det << endl;

            /*cout << "------------------------------------------------------------------------------------------" << endl;
            cout << "------------------------------------------------------------------------------------------" << endl;
            cout << "------------------------------------------------------------------------------------------" << endl;
            cout << "success!!!!" << endl;
            cout << "!!!!! hog( " << candicate << " )!!!!!!!!!" << endl;
            cout << "!!!!! lbp( " << candicateLBP << " )!!!!!!!!!" << endl;
            cout << "------------------------------------------------------------------------------------------" << endl;
            cout << "------------------------------------------------------------------------------------------" << endl;
            cout << "------------------------------------------------------------------------------------------" << endl;
            system("pause");*/

            skin_total3 = skin_total2;
            eligibility = candicate;
            eligibilityLBP = candicateLBP;


            Px = m;
            Py = n;
            

            thistogram0 = histogram0;
            thistogram1 = histogram1;
            thistogram2 = histogram2;
            thistogram3 = histogram3;
            thistogram4 = histogram4;
            thistogram5 = histogram5;
            thistogram6 = histogram6;
            thistogram7 = histogram7;
            thistogram8 = histogram8;


            LBPthistogram0 = LBPhistogram0;
            LBPthistogram1 = LBPhistogram1;
            LBPthistogram2 = LBPhistogram2;
            LBPthistogram3 = LBPhistogram3;
            LBPthistogram4 = LBPhistogram4;
            LBPthistogram5 = LBPhistogram5;
            LBPthistogram6 = LBPhistogram6;
            LBPthistogram7 = LBPhistogram7;
            LBPthistogram8 = LBPhistogram8;
            LBPthistogram9 = LBPhistogram9;
            LBPthistogram10 = LBPhistogram10;
            LBPthistogram11 = LBPhistogram11;
            LBPthistogram12 = LBPhistogram12;
            LBPthistogram13 = LBPhistogram13;
            LBPthistogram14 = LBPhistogram14;

            // 找到第 n 個，符合ROI特徵值的候選區塊，候選區塊的數量又多累計一個。
            countOfFrameCandidate++;

        }

    } // for m*n

    if ( P2_y - P1_y > P2_x - P1_x ) {
        
        P2_x = Px + ( P2_x - P1_x );
        P2_y = Py + ( P2_y - P1_y );
        P1_x = Px;
        P1_y = Py;

    } // if

    //cout << countTheZeroCandidateFrame << "***************候選區塊數量*********(" << countOfFrameCandidate << endl;

    // 不斷的候選區塊為 0 (找不到候選區塊)，將被判定為「卡無窮迴圈」
    if ( countOfFrameCandidate == 0 ) {

        countTheZeroCandidateFrame++; // 預計不能連續超過 20 次的「找不到候選區塊」

    } // if

    else {

        countTheZeroCandidateFrame = 0;

    } // else

    Sobelcount = 0;
    //cout<<P2_x<<" "<<P2_y<<endl;
    if ( eligibility > 0.25 )   //追丟回原位
    {


        ////cout<<"click ";
        ////防止握拳ROI下移
        P2_x = PreP1x + ( P2_x - P1_x );
        P2_y = PreP1y + ( P2_y - P1_y );
        P1_x = PreP1x;
        P1_y = PreP1y;

        for ( int i = P1_x; i < P2_x; i++ ) {
            for ( int j = P1_y; j < P1_y + ( P2_y - P1_y ) / 2 && j < height_m; j++ ) {
                Y [ i ] [ j ] = abs( YCbCr_Y [ i - 1 ] [ j - 1 ] * ( -1 ) + YCbCr_Y [ i ] [ j - 1 ] * ( -2 ) + YCbCr_Y [ i + 1 ] [ j - 1 ] * ( -1 )
                                     + YCbCr_Y [ i - 1 ] [ j + 1 ] * 1 + YCbCr_Y [ i ] [ j + 1 ] * 2 + YCbCr_Y [ i + 1 ] [ j + 1 ] * 1 );

                Cb [ i ] [ j ] = abs( YCbCr_Cb [ i - 1 ] [ j - 1 ] * ( -1 ) + YCbCr_Cb [ i ] [ j - 1 ] * ( -2 ) + YCbCr_Cb [ i + 1 ] [ j - 1 ] * ( -1 )
                                      + YCbCr_Cb [ i - 1 ] [ j + 1 ] * 1 + YCbCr_Cb [ i ] [ j + 1 ] * 2 + YCbCr_Cb [ i + 1 ] [ j + 1 ] * 1 );

                Cr [ i ] [ j ] = abs( YCbCr_Cr [ i - 1 ] [ j - 1 ] * ( -1 ) + YCbCr_Cr [ i ] [ j - 1 ] * ( -2 ) + YCbCr_Cr [ i + 1 ] [ j - 1 ] * ( -1 )
                                      + YCbCr_Cr [ i - 1 ] [ j + 1 ] * 1 + YCbCr_Cr [ i ] [ j + 1 ] * 2 + YCbCr_Cr [ i + 1 ] [ j + 1 ] * 1 );

                SobelFrame [ i ] [ j ] = Y [ i ] [ j ] + Cb [ i ] [ j ] + Cr [ i ] [ j ];
                if ( SobelFrame [ i ] [ j ] > 50 && YCbCr_Cr [ i ] [ j ] > 133 && YCbCr_Cr [ i ] [ j ] < 174 && YCbCr_Cb [ i ] [ j ] > 77 && YCbCr_Cb [ i ] [ j ] < 127 )
                    Sobelcount++;
                //else
                //SobelFrame[i][j] = 0;

            }
        }

        if ( abs( P1_x - PreP1x ) < 3 && abs( P1_y - PreP1y ) < 3 && wait<0 && Sobelcount - Sobelcount2>60 ) {

            //click_count++;

            //cout << "+++++++++++++++++++++++++++++++++++++++" << endl;
        } // if

    }

    // 進入辨識階段
    if ( click_count == 5 ) {
        cout << "Sobelcount: " << Sobelcount << endl;
        cout << "Sobelcount2: " << Sobelcount2 << endl;
        click_count = 0;
        cout << "click" << endl;
        flag = 3;
        click_flag = 1;

        for ( int i = 0; i<P2_x - P1_x + 8; i++ )        //儲存ROI內的像素值->fistYCbCr[][]
            for ( int j = 0; j<P2_y - P1_y; j++ ) {
                fistY [ P1_x + i ] [ P1_y + j ] = YCbCr_Y [ P1_x + i ] [ P1_y + j ];
                fistCb [ P1_x + i ] [ P1_y + j ] = YCbCr_Cb [ P1_x + i ] [ P1_y + j ];
                fistCr [ P1_x + i ] [ P1_y + j ] = YCbCr_Cr [ P1_x + i ] [ P1_y + j ];
                fist_x = P1_x;
                fist_y = P1_y;
            }
        wait = 10;
    }

    if ( wait<0 )
        wait = 0;
    //if (trackrange<8000 && trackrange>3000)
    //新舊P1相差距離
    if ( abs( Px - PreP1x ) > 0 || abs( Py - PreP1y ) > 0 ) {

        P1 = cvPoint( P1_x , P1_y );
        P2 = cvPoint( P2_x , P2_y );
    }

}
