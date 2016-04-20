#pragma once 
#include "struct.h"
#include "Process.h"
#include <fstream>

//void recognition_button() {
//    for ( int j = 0; j<P2_y - P1_y; j++ )      //ROI內像素值  後減前  fistYCbCr - YCbCr = fist2YCbCr
//    {
//        for ( int i = 0; i<P2_x - P1_x + 8; i++ )      	//fist_x = P1_x;
//            //fist_y = P1_y;
//        {
//            fist2Y [ fist_x + i ] [ fist_y + j ] = fistY [ fist_x + i ] [ fist_y + j ] - YCbCr_Y [ fist_x + i ] [ fist_y + j ];
//            fist2Cb [ fist_x + i ] [ fist_y + j ] = fistCb [ fist_x + i ] [ fist_y + j ] - YCbCr_Cb [ fist_x + i ] [ fist_y + j ];
//            fist2Cr [ fist_x + i ] [ fist_y + j ] = fistCr [ fist_x + i ] [ fist_y + j ] - YCbCr_Cr [ fist_x + i ] [ fist_y + j ];
//            if ( fist2Y [ fist_x + i ] [ fist_y + j ]>1 || fist2Cb [ fist_x + i ] [ fist_y + j ]>1 || fist2Cr [ fist_x + i ] [ fist_y + j ]>1 )  //二值化
//                fist2Y [ fist_x + i ] [ fist_y + j ] = 1;
//            else
//                fist2Y [ fist_x + i ] [ fist_y + j ] = 0;
//        }
//    }
//
//    for ( int i = 0; i<320; i++ )                           //侵蝕
//        for ( int j = 0; j<240; j++ )
//            fist2Y_temp [ i ] [ j ] = Erosion( fist2Y, i, j );
//
//    int thumb = 0;
//    int a [ 60 ] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
//    for ( int j = 0; j<( P2_y - P1_y ) / 3; j++ )      //只掃上半三分之一
//    {
//        for ( int i = 0; i<P2_x - P1_x; i++ ) {
//            if ( fist2Y_temp [ fist_x + i ] [ fist_y + j ] == 1 && fist2Y_temp [ fist_x + i + 1 ] [ fist_y + j ] == 1 && fist2Y_temp [ fist_x + i - 1 ] [ fist_y + j ] == 0 )  // 1-1-0
//            {
//                a [ i ] = 1;
//                a [ i + 1 ] = 1;
//            }
//        }
//    }
//
//    int finger_count = 0;
//    for ( int l = 0; l<60; l++ ) {
//        //cout<<a[l];
//        if ( a [ l ] == 1 && a [ l + 1 ] == 0 )   // 1-0
//            finger_count++;
//    }
//
//    if ( finger_count == 0 ) {/*
//                              voice_count=0;
//                              for(int s=P1_x;s<P2_x;s++)
//                              for(int t=P1_y+(P2_y-P1_y)/2;t<P2_y;t++)
//                              if(fist2Y_temp[s][t]==1 && fist2Y_temp[s-1][t]!=1 && fist2Y_temp[s+1][t]!=1 && fist2Y_temp[s][t-1]!=1 && fist2Y_temp[s][t+1]!=1)
//                              voice_count++;
//                              if(voice_count>4)
//                              cout<<"zero"<<thumb<<endl;
//                              else
//                              {
//                              if(voice_flag==0)
//                              {
//                              voice_flag=1;
//                              cout<<"condition change to 1"<<endl;
//                              }
//                              else
//                              {
//                              voice_flag=0;
//                              cout<<"condition change to 0"<<endl;
//                              }
//                              }*/
//        wait = 10;
//        flag = 3;
//        click_flag = 1;
//    }
//    else if ( finger_count == 2 )
//        cout << "two" << 2 << endl;
//
//
//
//    else if ( 0 )
//        cout << "you got nothing" << endl;
//
//}

//void Rotation() {
//
//    rotationPx = P1_x + ( P2_x - P1_x ) / 2;
//    rotationPy = P1_y + ( P2_y - P1_y ) / 2;
//    int rotationX = rotationPrePx - rotationPx;
//    int rotationY = rotationPrePy - rotationPy;
//
//    //4方向
//    if ( rotationX >= 0 && rotationY<0 )
//        rotation_orientation = 1;
//    else if ( rotationX<0 && rotationY<0 )
//        rotation_orientation = 2;
//    else if ( rotationX<0 && rotationY >= 0 )
//        rotation_orientation = 3;
//    else if ( rotationX >= 0 && rotationY >= 0 )
//        rotation_orientation = 4;
//
//
//    switch ( rotation_orientation )  //旋轉方向
//    {
//        case1:
//        if ( rotation_count == 0 ) {
//            rotation_count++;
//            rotation_disX = abs( rotationX );
//            rotation_disY = abs( rotationY );
//        }
//
//        case2:
//        case3 :
//            case4 : ;
//
//    }
//
//}
//
//void MoveLookController::OnMouseMoved(
//	_In_ Windows::Devices::Input::MouseDevice^ mouseDevice,
//	_In_ Windows::Devices::Input::MouseEventArgs^ args
//	)
//{
//	float2 pointerDelta;
//	pointerDelta.x = static_cast<float>(args->MouseDelta.X);
//	pointerDelta.y = static_cast<float>(args->MouseDelta.Y);
//
//	float2 rotationDelta;
//	rotationDelta = pointerDelta * ROTATION_GAIN;	// scale for control sensitivity
//
//	// update our orientation based on the command
//	m_pitch -= rotationDelta.y;						// mouse y increases down, but pitch increases up
//	m_yaw -= rotationDelta.x;						// yaw defined as CCW around y-axis
//
//	// limit pitch to straight up or straight down
//	float limit = (float)(M_PI / 2) - 0.01f;
//	m_pitch = (float)__max(-limit, m_pitch);
//	m_pitch = (float)__min(+limit, m_pitch);
//
//	// keep longitude in useful range by wrapping
//	if (m_yaw >  M_PI)
//		m_yaw -= (float)M_PI * 2;
//	else if (m_yaw < -M_PI)
//		m_yaw += (float)M_PI * 2;
//}
