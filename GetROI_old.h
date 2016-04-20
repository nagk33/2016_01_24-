#pragma once //防止被多次include  保證只被編譯一次
#include "struct.h"
#include "Tracking.h"
#include <fstream>

void GetROI() {
    ROI ROI_Info [ 30 ];
    for ( int k = 0; k < 30; k++ ) {
        ROI_Info [ k ].Max_Xx = 0;
        ROI_Info [ k ].Max_Xy = 0;
        ROI_Info [ k ].Min_Yx = 9999;
        ROI_Info [ k ].Min_Yy = 9999;
        ROI_Info [ k ].Center_X = 0;
        ROI_Info [ k ].Center_Y = 0;
        ROI_Info [ k ].Area = 0;
        ROI_Info [ k ].feature_Amount1 = 0;
        ROI_Info [ k ].feature_Amount2 = 0;

    }

    int Label_index = 0;


    for ( int j = 0; j < height_m; j++ )
        for ( int i = 0; i < width_m; i++ ) {
            if ( LabelMap [ i ] [ j ] != 0 && LabelMap [ i ] [ j ] < 30 ) // <30
            {
                Label_index = LabelMap [ i ] [ j ];
                ROI_Info [ Label_index ].Area++;  //將同個labelindex值一樣的加起來

                if ( ROI_Info [ LabelMap [ i ] [ j ] ].Max_Xx < i )  //找到最大X(x,y) 最小Y(x,y)
                {
                    ROI_Info [ LabelMap [ i ] [ j ] ].Max_Xx = i;
                    ROI_Info [ LabelMap [ i ] [ j ] ].Max_Xy = j;
                }
                if ( ROI_Info [ LabelMap [ i ] [ j ] ].Min_Yy > j ) {
                    ROI_Info [ LabelMap [ i ] [ j ] ].Min_Yx = i;
                    ROI_Info [ LabelMap [ i ] [ j ] ].Min_Yy = j;
                }



            }
        }


    for ( short k = 0; k < 30; k++ ) {
        if ( ROI_Info [ k ].Area>150 )   //第一個門檻要先去除太小殘破不堪的區塊
        {


            ROI_Info [ k ].Center_X = ROI_Info [ k ].Min_Yx - 10; //中心點(最小Y位置的X座標,改成最小X位置的y座標)  //offset30
            ROI_Info [ k ].Center_Y = ROI_Info [ k ].Max_Xy;

            int scan_x = ROI_Info [ k ].Max_Xx - ( ROI_Info [ k ].Max_Xx - ROI_Info [ k ].Center_X );



            //system("pause");

            bool corner_flag = true;


            for ( int l = 0; l < 320; l++ )          //先做角點
            {
                sobel_array [ l ] = 0;
            }
            //cout << scan_x << " " << ROI_Info[k].Max_Xx<<" "<< ROI_Info[k].Center_X <<" "<< ROI_Info[k].Center_Y << endl;


            if ( scan_x > 0 ) {

                for ( int i = scan_x; i < scan_x + ( ROI_Info [ k ].Max_Xx - ROI_Info [ k ].Center_X ) * 5 / 2; i++ ) {


                    for ( int j = ROI_Info [ k ].Min_Yy - 1; j < ROI_Info [ k ].Min_Yy + ( ROI_Info [ k ].Center_Y - ROI_Info [ k ].Min_Yy ) / 2; j++ ) {
                        //sobel累加

                        //cout << "i = " << i << ", j = " << j << "\n";


                        if ( LabelMap [ i ] [ j ] == 1 && i > 2 && j > 2 )  //累加X軸上的邊點
                        {
                            sobel_array [ i - scan_x ]++;
                        }

                        if ( LabelMap [ i ] [ j ] == 1 && i > 2 && j > 2 ) {

                            int corner_count = 0;                  //scan labelmap=1 的點之周圍，如果值是自己本身的區塊編號就累加
                            if ( LabelMap [ i ] [ j - 2 ] == k )
                                corner_count++;
                            if ( LabelMap [ i + 1 ] [ j - 1 ] == k )
                                corner_count++;
                            if ( LabelMap [ i + 2 ] [ j ] == k )
                                corner_count++;
                            if ( LabelMap [ i + 1 ] [ j + 1 ] == k )
                                corner_count++;
                            if ( LabelMap [ i ] [ j + 2 ] == k )
                                corner_count++;
                            if ( LabelMap [ i - 1 ] [ j + 1 ] == k )
                                corner_count++;
                            if ( LabelMap [ i - 2 ] [ j ] == k )
                                corner_count++;
                            if ( LabelMap [ i - 1 ] [ j - 1 ] == k )
                                corner_count++;
                            if ( corner_count < 2 ) {
                                LabelMap [ i ] [ j ] = 31;
                                ROI_Info [ k ].feature_Amount1++;
                                //cout << corner_count << endl;
                                //cout <<"點點="<< ROI_Info[k].feature_Amount1 << endl;
                                for ( int m = i; m >= i - 3; m-- )     //過濾掉自己附近比自己低的角點
                                    for ( int n = j + 3; n >= j - 4; n-- ) {

                                        //cout << "k = " << k << "m = " << m << ", n = " << n << "\n";

                                        if ( LabelMap [ m ] [ n ] == 31 ) {

                                            if ( LabelMap [ i - 2 ] [ j ] == 0 && LabelMap [ m - 1 ] [ n ] != 0 )   // 防止前面的角點取代到
                                            {

                                                //cout << "1111111111111111111111111111111111111我進來了!!!!!" << endl;

                                            }
                                            else//高角點取代低角點   
                                            {
                                                //cout << "2我進來了!!!!!" << endl;

                                                if ( m == i && n == j ) {
                                                }
                                                if ( n < j )  //低於自己的角點
                                                {
                                                    LabelMap [ i ] [ j ] = 1;
                                                    ROI_Info [ k ].feature_Amount1--;
                                                    m = 0, n = 0;
                                                }
                                                if ( n>j )    //
                                                {
                                                    LabelMap [ m ] [ n ] = 1;
                                                    ROI_Info [ k ].feature_Amount1--;
                                                    m = 0, n = 0;

                                                }
                                                if ( n == j && m != i ) {//一樣高折衷
                                                    LabelMap [ m ] [ n ] = 1;
                                                    LabelMap [ i ] [ j ] = 1;
                                                    LabelMap [ i - 1 ] [ j ] = 31;
                                                    ROI_Info [ k ].feature_Amount1--;
                                                    m = 0, n = 0;


                                                }
                                            }
                                        }
                                    }



                            }

                        }
                    }

                }
            }
            /*for (int j = ROI_Info[k].Min_Yy + (ROI_Info[k].Center_Y - ROI_Info[k].Min_Yy) / 2; ROI_Info[k].Center_Y; j++)   //谷點
            {
            if (LabelMap[i][j] == 1 && i > 2 && j > 2)
            {
            int corner_count = 0;
            int corner_count1 = 0;
            if (LabelMap[i][j - 2] != k)// 上部分
            corner_count++;
            if (LabelMap[i + 2][j] == k)
            corner_count1++;
            if (LabelMap[i][j + 2] == k)
            corner_count1++;
            if (LabelMap[i - 2][j] == k)
            corner_count1++;
            if (corner_count == 1 && corner_count1 == 3)   //出現三個以上相鄰 則取中間的候選點代表之
            {
            LabelMap[i][j] = 32; //標記為谷點
            ROI_Info[k].feature_Amount2++;
            corner_flag_count--;
            for (int m = i; m >= i - 4; m--)
            for (int n = j + 4; m >= j - 4; n--)
            {
            if (LabelMap[m][n] == 32)
            {
            if (m == i&& n == j)
            {
            }
            else
            {
            LabelMap[i][j] = 1;
            ROI_Info[k].feature_Amount2--;
            corner_flag_count++;
            m = 0, n = 0;
            }
            }

            }
            if (corner_flag_count == 0)

            }
            }
            }

            }*/
            P1_x = abs( ROI_Info [ k ].Max_Xx - ( ROI_Info [ k ].Max_Xx - ROI_Info [ k ].Center_X ) * 3 / 2 );     // 長寬比例條件
            P1_y = ROI_Info [ k ].Min_Yy;
            P2_x = ROI_Info [ k ].Max_Xx;
            P2_y = ROI_Info [ k ].Max_Xy + ( ROI_Info [ k ].Center_Y - ROI_Info [ k ].Min_Yy );

            int counter_up = 0, counter_down = 0;     //上下含pixels的比例
            for ( int s = P1_x; s < P2_x; s++ ) {
                for ( int t = P1_y; t<( P2_y - P1_y ) / 3 + P1_y; t++ ) {
                    if ( LabelMap [ s ] [ t ]>1 )
                        counter_up++;
                }
            }
            for ( int s = P1_x; s < P2_x; s++ ) {
                for ( int t = ( P2_y - P1_y ) / 3 + P1_y; t<P2_y; t++ ) {
                    if ( LabelMap [ s ] [ t ]>1 )
                        counter_down++;
                }
            }
            int range = ( P2_x - P1_x )*( P2_y - P1_y );
            cout << "---------STARRTTTTT--------------" << endl;
            cout << P1_x << " " << P1_y << " " << P2_x << " " << P2_y << "center=" << ROI_Info [ k ].Center_Y << endl;

            cout << "feature = " << ROI_Info [ k ].feature_Amount1 << "!!!!!!!!!!!!!" << endl;
            cout << "range= " << range << endl;
            cout << P2_x - P1_x << "," << P2_y - P1_y << endl;
            cout << "---------ENDDDDDDDD---------------" << endl;




            if ( counter_down * 2 > counter_up * 3 )
                if ( P2_x - P1_x <= 100 && P2_y - P1_y <= 200 )
                    if ( P2_x - P1_x  < ( P2_y - P1_y ) * 2 && ( P2_x - P1_x ) * 2  > P2_y - P1_y &&range<8000 && range>3000 )
                        if ( ROI_Info [ k ].feature_Amount1 == 1 || ROI_Info [ k ].feature_Amount1 == 2 )
                            if ( P2_y - P1_y > P2_x - P1_x ) {
                                int sobel_count = 0;
                                for ( int z = 1; z<60; z++ )
                                    if ( sobel_array [ z - 1 ] <= sobel_array [ z ] && sobel_array [ z ] >sobel_array [ z + 1 ] )  //z在尖點上
                                    {
                                        sobel_count++;
                                    }

                                cout << "sobel_count=" << sobel_count << endl;
                                if ( sobel_count == 2 || sobel_count == 3 || sobel_count == 4 )
                                    //if (((P2_y * 3 / 2) - (P1_y * 2)) > (P2_x * 2) - (P1_x * 2) * 3 / 2)

                                    P1 = cvPoint( P1_x * 2, P1_y * 2 );
                                P2 = cvPoint( P2_x * 2, P2_y * 3 / 2 );

                                cout << "我終於進來了" << endl;
                                ROI_shape = ( P2_y - P1_y ) / ( P2_x - P1_x );

                                cout << "ROI長寬比 = " << ROI_shape << endl;
                                //disbetweenfingertip = (P2_y - P1_y) / 2;

                                flag = 2;
                                once = 0;




                            }//if (ROI_Info[k].feature_Amount1 == 1 || ROI_Info[k].feature_Amount1 == 2)

        }
    }

}