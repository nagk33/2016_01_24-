#pragma once
#include "struct.h"



void Contour_Trace( short ImgArray [ width_m ] [ height_m ], int Center_X, int Center_Y, int Trace_Direction, int Label_Index ) {
    // Console Display
    // Console.Write(""); 
	int Search_Direction[8][2] = { { 0, 1 }, { 1, 1 }, { 1, 0 }, { 1, -1 }, { 0, -1 }, { -1, -1 }, { -1, 0 }, { -1, 1 } };
	//int Search_Direction[8][2] = { { -1, 0 }, { -1, 1 }, { 0, 1 }, { 1, 1 }, { 1, 0 }, { 1, -1 }, { 0, -1 }, { -1, -1 } }; // 90
    int x, y;

    bool Trace_Stop_Flag = false;
    bool Search_Again_Flag = true;
    int x_1 = Center_X;
    int y_1 = Center_Y;

    for ( int i = 0; i < 8; i++ ) {
        x = Center_X + Search_Direction [ Trace_Direction ] [ 1 ];
        y = Center_Y + Search_Direction [ Trace_Direction ] [ 0 ];

        if ( ImgArray [ x ] [ y ] == 0 ) {
            LabelMap [ x ] [ y ] = 1;
            Trace_Direction = ( Trace_Direction + 1 ) % 8;
        }
        else {
            Center_X = x;
            Center_Y = y;
            break;
        }
    }

    if ( Center_X != x_1 || Center_Y != y_1 ) {
        int x_2 = Center_X;
        int y_2 = Center_Y;

        while ( Search_Again_Flag ) {
            Trace_Direction = ( Trace_Direction + 6 ) % 8;
            LabelMap [ Center_X ] [ Center_Y ] = Label_Index;

            for ( int i = 0; i < 8; i++ ) {
                x = Center_X + Search_Direction [ Trace_Direction ] [ 1 ];
                y = Center_Y + Search_Direction [ Trace_Direction ] [ 0 ];

                if ( ImgArray [ x ] [ y ] == 0 ) {
                    LabelMap [ x ] [ y ] = 1;
                    Trace_Direction = ( Trace_Direction + 1 ) % 8;
                }
                else {
                    Center_X = x;
                    Center_Y = y;
                    break;
                }
            }

            if ( Center_X == x_1 && Center_Y == y_1 )
                Trace_Stop_Flag = true;
            else if ( Trace_Stop_Flag ) {
                if ( Center_X == x_2 && Center_Y == y_2 )
                    Search_Again_Flag = false;
                else
                    Trace_Stop_Flag = false;
            }
        }
    }
}

void CCL( short ImgArray [ width_m ] [ height_m ] ) {
    int Connected_Component_Count = 1;
    int Label_Index = 0;
    int Trace_Direction;

    for ( int j = 0; j < height_m; j++ ) {
        for ( int i = 0; i < width_m; i++ ) {
            if ( i == 0 || i == width_m - 1 || j == 0 || j == height_m - 1 )
                ImgArray [ i ] [ j ] = 0;
        }
    }

    for ( int y = 1; y < height_m - 1; y++ ) {
        Label_Index = 0;
        for ( int x = 1; x < width_m - 1; x++ ) {
            if ( ImgArray [ x ] [ y ] == 255 ) {
                if ( Label_Index != 0 )
                    LabelMap [ x ] [ y ] = Label_Index;
                else {
                    Label_Index = LabelMap [ x ] [ y ];

                    if ( Label_Index == 0 ) {
                        Label_Index = ++Connected_Component_Count;
                        Trace_Direction = 0;
                        Contour_Trace( ImgArray, x, y, Trace_Direction, Label_Index );
                        LabelMap [ x ] [ y ] = Label_Index;    //將相同區域標是相同數字
                    }
                }
            }
            else if ( Label_Index != 0 ) {
                if ( LabelMap [ x ] [ y ] == 0 ) {
                    Trace_Direction = 1;
                    Contour_Trace( ImgArray, x - 1, y, Trace_Direction, Label_Index );
                }
                Label_Index = 0;
            }
        }
    }
    Label_Index_Total = Connected_Component_Count;

    // Show Label_Map Info. by Txt.
    //Txt_Displayer(Label_Map);
	fout_CCL.open(("CCL.txt"), ios::out);
	for (int j = 0; j<height_m; j++) {
	for (int i = 0; i<width_m; i++) {
            fout_CCL << LabelMap [ i ] [ j ];

        }
        fout_CCL << endl;
    }
    fout_CCL.close();

    /*
    //CCL 歸0! 不然跑一下下就爆掉了!
    for (int i = 0; i < width_m; i++)
    {
    for (int j = 0; j < height_m; j++)
    {
    LabelMap[i][j] = 0;
    DiffFrame_Count[i][j] = 0;
    }
    }*/
}
