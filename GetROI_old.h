#pragma once //����Q�h��include  �O�ҥu�Q�sĶ�@��
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
                ROI_Info [ Label_index ].Area++;  //�N�P��labelindex�Ȥ@�˪��[�_��

                if ( ROI_Info [ LabelMap [ i ] [ j ] ].Max_Xx < i )  //���̤jX(x,y) �̤pY(x,y)
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
        if ( ROI_Info [ k ].Area>150 )   //�Ĥ@�Ӫ��e�n���h���Ӥp�ݯ}�������϶�
        {


            ROI_Info [ k ].Center_X = ROI_Info [ k ].Min_Yx - 10; //�����I(�̤pY��m��X�y��,�令�̤pX��m��y�y��)  //offset30
            ROI_Info [ k ].Center_Y = ROI_Info [ k ].Max_Xy;

            int scan_x = ROI_Info [ k ].Max_Xx - ( ROI_Info [ k ].Max_Xx - ROI_Info [ k ].Center_X );



            //system("pause");

            bool corner_flag = true;


            for ( int l = 0; l < 320; l++ )          //�������I
            {
                sobel_array [ l ] = 0;
            }
            //cout << scan_x << " " << ROI_Info[k].Max_Xx<<" "<< ROI_Info[k].Center_X <<" "<< ROI_Info[k].Center_Y << endl;


            if ( scan_x > 0 ) {

                for ( int i = scan_x; i < scan_x + ( ROI_Info [ k ].Max_Xx - ROI_Info [ k ].Center_X ) * 5 / 2; i++ ) {


                    for ( int j = ROI_Info [ k ].Min_Yy - 1; j < ROI_Info [ k ].Min_Yy + ( ROI_Info [ k ].Center_Y - ROI_Info [ k ].Min_Yy ) / 2; j++ ) {
                        //sobel�֥[

                        //cout << "i = " << i << ", j = " << j << "\n";


                        if ( LabelMap [ i ] [ j ] == 1 && i > 2 && j > 2 )  //�֥[X�b�W�����I
                        {
                            sobel_array [ i - scan_x ]++;
                        }

                        if ( LabelMap [ i ] [ j ] == 1 && i > 2 && j > 2 ) {

                            int corner_count = 0;                  //scan labelmap=1 ���I���P��A�p�G�ȬO�ۤv�������϶��s���N�֥[
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
                                //cout <<"�I�I="<< ROI_Info[k].feature_Amount1 << endl;
                                for ( int m = i; m >= i - 3; m-- )     //�L�o���ۤv�����ۤv�C�����I
                                    for ( int n = j + 3; n >= j - 4; n-- ) {

                                        //cout << "k = " << k << "m = " << m << ", n = " << n << "\n";

                                        if ( LabelMap [ m ] [ n ] == 31 ) {

                                            if ( LabelMap [ i - 2 ] [ j ] == 0 && LabelMap [ m - 1 ] [ n ] != 0 )   // ����e�������I���N��
                                            {

                                                //cout << "1111111111111111111111111111111111111�ڶi�ӤF!!!!!" << endl;

                                            }
                                            else//�����I���N�C���I   
                                            {
                                                //cout << "2�ڶi�ӤF!!!!!" << endl;

                                                if ( m == i && n == j ) {
                                                }
                                                if ( n < j )  //�C��ۤv�����I
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
                                                if ( n == j && m != i ) {//�@�˰���J
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
            /*for (int j = ROI_Info[k].Min_Yy + (ROI_Info[k].Center_Y - ROI_Info[k].Min_Yy) / 2; ROI_Info[k].Center_Y; j++)   //���I
            {
            if (LabelMap[i][j] == 1 && i > 2 && j > 2)
            {
            int corner_count = 0;
            int corner_count1 = 0;
            if (LabelMap[i][j - 2] != k)// �W����
            corner_count++;
            if (LabelMap[i + 2][j] == k)
            corner_count1++;
            if (LabelMap[i][j + 2] == k)
            corner_count1++;
            if (LabelMap[i - 2][j] == k)
            corner_count1++;
            if (corner_count == 1 && corner_count1 == 3)   //�X�{�T�ӥH�W�۾F �h���������Կ��I�N��
            {
            LabelMap[i][j] = 32; //�аO�����I
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
            P1_x = abs( ROI_Info [ k ].Max_Xx - ( ROI_Info [ k ].Max_Xx - ROI_Info [ k ].Center_X ) * 3 / 2 );     // ���e��ұ���
            P1_y = ROI_Info [ k ].Min_Yy;
            P2_x = ROI_Info [ k ].Max_Xx;
            P2_y = ROI_Info [ k ].Max_Xy + ( ROI_Info [ k ].Center_Y - ROI_Info [ k ].Min_Yy );

            int counter_up = 0, counter_down = 0;     //�W�U�tpixels�����
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
                                    if ( sobel_array [ z - 1 ] <= sobel_array [ z ] && sobel_array [ z ] >sobel_array [ z + 1 ] )  //z�b�y�I�W
                                    {
                                        sobel_count++;
                                    }

                                cout << "sobel_count=" << sobel_count << endl;
                                if ( sobel_count == 2 || sobel_count == 3 || sobel_count == 4 )
                                    //if (((P2_y * 3 / 2) - (P1_y * 2)) > (P2_x * 2) - (P1_x * 2) * 3 / 2)

                                    P1 = cvPoint( P1_x * 2, P1_y * 2 );
                                P2 = cvPoint( P2_x * 2, P2_y * 3 / 2 );

                                cout << "�ڲש�i�ӤF" << endl;
                                ROI_shape = ( P2_y - P1_y ) / ( P2_x - P1_x );

                                cout << "ROI���e�� = " << ROI_shape << endl;
                                //disbetweenfingertip = (P2_y - P1_y) / 2;

                                flag = 2;
                                once = 0;




                            }//if (ROI_Info[k].feature_Amount1 == 1 || ROI_Info[k].feature_Amount1 == 2)

        }
    }

}