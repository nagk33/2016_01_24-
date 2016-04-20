#pragma once
#include "struct.h"

//Temporal Difference Image
/*
void Temporal_Difference()
{
Threshold = 0.0;
for (int i = 0; i < width_m; i++)
{
for (int j = 0; j < height_m; j++)
{
Threshold += YCbCr_Y[i][j];
}
}
Threshold = Threshold / (width_m * height_m);
Threshold = Threshold * 0.08;  //Michael original : Threshold = Threshold * 0.08;

for (int i = 0; i < width_m; i++)
{
for (int j = 0; j < height_m; j++)
{
int Diff_Value = 0;
Diff_Value = abs(YCbCr_Y[i][j] - Pre[i][j]);
Pre[i][j] = YCbCr_Y[i][j];

if (Diff_Value > Threshold)
{
DiffFrame_Count[i][j]++;
}
}
}
}*/

void Temporal_Difference() {

    Threshold = 10.0;
    for ( int i = 0; i < width_m; i++ ) {
        for ( int j = 0; j < height_m; j++ ) {
            int Diff_Value = 0;
            Diff_Value = abs( YCbCr_Y [ i ] [ j ] - Pre [ i ] [ j ] ); // abs取整數絕對值
            Pre [ i ] [ j ] = YCbCr_Y [ i ] [ j ];

            if ( Diff_Value > Threshold ) {
				DiffFrame_Count [ i ] [ j ]++; //超過T則做累加
            }
        }
    }
}

void Temporal_Difference_tracking() {
	Threshold = 10.0;
	for (int i = 0; i < width_m; i++) {
		for (int j = 0; j < height_m; j++) {
			int Diff_Value = 0;
			Diff_Value = abs(YCbCr_Y[i][j] - PreTrack[i][j]); // abs取整數絕對值
			PreTrack[i][j] = YCbCr_Y[i][j];

			if (Diff_Value > Threshold) {
				trackingDiffFrame_Count[i][j]++; //超過T則做累加
			}
		}
	}
}

int Dilation( short pre [ width_m ] [ height_m ], int i, int j ) // OR 有1就是1
{
    int post = 0;
    if ( i != 0 && i != width_m && j != 0 && j != height_m ) {
        post = ( pre [ i - 1 ] [ j - 1 ] | pre [ i ] [ j - 1 ] | pre [ i + 1 ] [ j - 1 ] |
                 pre [ i - 1 ] [ j ] | pre [ i + 1 ] [ j ] | pre [ i - 1 ] [ j + 1 ] |
                 pre [ i ] [ j + 1 ] | pre [ i + 1 ] [ j + 1 ] );
    }
    return post;
}

int Erosion( short pre [ width_m ] [ height_m ], int i, int j ) // AND 都是1才是1
{
    int post = 0;
    if ( i != 0 && i != width_m && j != 0 && j != height_m ) {
        post = ( pre [ i - 1 ] [ j - 1 ] & pre [ i ] [ j - 1 ] & pre [ i + 1 ] [ j - 1 ] &
                 pre [ i - 1 ] [ j ] & pre [ i + 1 ] [ j ] &
                 pre [ i - 1 ] [ j + 1 ] & pre [ i ] [ j + 1 ] & pre [ i + 1 ] [ j + 1 ] );
    }
    return post;
}



int getFeature( int pixel, int center, int num, int mean )  // ||_ _ _ _  || _ _ _ _|| 兩個四位的二進制
{
    int std = pixel - mean;
    if ( std > 0 ) {
        return ( int ) pow( 2, num );
    }
    else {
        return 0;
    }

}
// ====================== 修改版 CCL  ======================

// 算 連通 面積，會回傳面積大小
//int CCL_Tracing( IplImage * src, int row, int col, int label, int & area, vector<FeaturePoint> & deletePoint, int background ) {
//
//    if ( !( row >= 0 && col >= 0 && row < src->height && col < src->width ) ) {
//
//        return 0;
//
//    } // if
//
//    else if ( CV_IMAGE_ELEM( src, byte, row, col ) == ( byte ) background ) {
//
//        return 0;
//
//    } // if
//
//    else {
//
//        CV_IMAGE_ELEM( src, byte, row, col ) = ( byte ) background;
//        area++;
//
//        FeaturePoint p;
//
//        p.x = row;
//        p.y = col;
//
//        deletePoint.push_back( p ); // 將該 特徵點的座標位置 存下，將來考慮做刪除
//
//        CCL_Tracing( src, row - 1, col, label, area, deletePoint, background ); // 上
//        CCL_Tracing( src, row - 1, col + 1, label, area, deletePoint, background ); // 右上
//        CCL_Tracing( src, row, col + 1, label, area, deletePoint, background ); // 右
//        CCL_Tracing( src, row + 1, col + 1, label, area, deletePoint, background ); // 右下
//        CCL_Tracing( src, row + 1, col, label, area, deletePoint, background ); // 下
//        CCL_Tracing( src, row + 1, col - 1, label, area, deletePoint, background ); // 左下
//        CCL_Tracing( src, row, col - 1, label, area, deletePoint, background ); // 左
//        CCL_Tracing( src, row - 1, col - 1, label, area, deletePoint, background ); // 左上
//
//        return area; // 回傳面積!!
//
//    } // else
//
//} // CCL_Tracing()

// 丟入一個 二值化後的影像(0 or 255)，顯示 CLL 的結果
void CCL( IplImage * src ) {

    IplImage * map = cvCloneImage( src ); // 複製一影像
    // IplImage * dst = cvCreateImage( cvSize( src->width, src->height ), IPL_DEPTH_8U, 1 ); // 顯示連通之後的結果

    // cvSetZero( dst ); // 初始化 CLL 結果圖!!

    int background = 0; // 決定 背景 的 pixel 値

    int label = 0;
    int area = 0;
    int delete_area_Threshold = 50; // 決定 珊除的面積

    for ( int i = 0; i < map->height; i++ ) {

        for ( int j = 0; j < map->width; j++ ) {

            area = 0;

            if ( CV_IMAGE_ELEM( map, byte, i, j ) != ( byte ) background ) { // (byte) 0 為 黑色的點

                vector<FeaturePoint> deletePoint;

                // cout << "label " << label++ - 1 << ": " << CCL_Tracing( map, dst, i, j, 255, area, deletePoint ) << endl;

                //CCL_Tracing( map, i, j, 255, area, deletePoint, background );

                if ( area < delete_area_Threshold ) { // 如果面積小於某個大小，將該 區塊視為雜訊 !! 清除 !!

                    for ( int k = 0; k < deletePoint.size(); k++ ) {

                        // CV_IMAGE_ELEM( dst, byte, deletePoint [ k ].x, deletePoint [ k ].y ) = ( byte ) 0;
                        CV_IMAGE_ELEM( src, byte, deletePoint [ k ].x, deletePoint [ k ].y ) = ( byte ) background;

                    } // for

                } // if

				vector<FeaturePoint>().swap(deletePoint);

            } // if

        } // for

    } // for i

    cvReleaseImage( &map );

    //cvShowImage( "CCL", dst );
    //cvWaitKey(0) ;

} // CCL()
// =========================================================
void contour_trace_test(short ** imgarray, int ** labelmap, int center_x, int center_y, int trace_direction, int label_index) {
	// console display
	// console.write(""); 
	int search_direction[8][2] = { { 0, 1 }, { 1, 1 }, { 1, 0 }, { 1, -1 }, { 0, -1 }, { -1, -1 }, { -1, 0 }, { -1, 1 } };
	int x, y;

	bool trace_stop_flag = false;
	bool search_again_flag = true;
	int x_1 = center_x;
	int y_1 = center_y;

	for (int i = 0; i < 8; i++) {
		x = center_x + search_direction[trace_direction][1];
		y = center_y + search_direction[trace_direction][0];

		if (imgarray[x][y] == 0) {
			labelmap[x][y] = 1;
			trace_direction = (trace_direction + 1) % 8;
		}
		else {
			center_x = x;
			center_y = y;
			break;
		}
	}

	if (center_x != x_1 || center_y != y_1) {
		int x_2 = center_x;
		int y_2 = center_y;

		while (search_again_flag) {
			trace_direction = (trace_direction + 6) % 8;
			labelmap[center_x][center_y] = label_index;

			for (int i = 0; i < 8; i++) {
				x = center_x + search_direction[trace_direction][1];
				y = center_y + search_direction[trace_direction][0];

				if (imgarray[x][y] == 0) {
					labelmap[x][y] = 1;
					trace_direction = (trace_direction + 1) % 8;
				}
				else {
					center_x = x;
					center_y = y;
					break;
				}
			}

			if (center_x == x_1 && center_y == y_1)
				trace_stop_flag = true;
			else if (trace_stop_flag) {
				if (center_x == x_2 && center_y == y_2)
					search_again_flag = false;
				else
					trace_stop_flag = false;
			}
		}
	}
}


int CCL_test(short **imgarray, int ** labelmap, int height, int width) {
	int connected_component_count = 1;
	int label_index = 0;
	int trace_direction;

	int temp = height;
	height = width;
	width = temp;

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			labelmap[i][j] = 0;
			if (i == 0 || i == width - 1 || j == 0 || j == height - 1)
				imgarray[i][j] = 0;
		}
	}

	for (int y = 1; y < height - 1; y++) {
		label_index = 0;
		for (int x = 1; x < width - 1; x++) {
			if (imgarray[x][y] > 0) {
				if (label_index != 0)
					labelmap[x][y] = label_index;
				else {
					label_index = labelmap[x][y];

					if (label_index == 0) {
						label_index = ++connected_component_count;
						trace_direction = 0;
						contour_trace_test(imgarray, labelmap, x, y, trace_direction, label_index);
						labelmap[x][y] = label_index;    //將相同區域標是相同數字
					}
				}
			}
			else if (label_index != 0) {
				if (labelmap[x][y] == 0) {
					trace_direction = 1;
					contour_trace_test(imgarray, labelmap, x - 1, y, trace_direction, label_index);
				}
				label_index = 0;
			}
		}
	}

	return connected_component_count;
}

void CCL_Delete(short **imgarray, const int height, const int width, int area) {
	
	// 宣告 用來存 連通標籤圖 的 2為矩陣
	int ** LabelMapPtr;
	LabelMapPtr = new int *[height];
	for (int i = 0; i < height; i++)
		LabelMapPtr[i] = new int[width];

	// 存下 各個區塊的 面積
	int ccl_next_index = CCL_test(imgarray, LabelMapPtr, height, width) + 1;
	int * Label_Region_Area = new int[ccl_next_index];
	for (int i = 0; i < ccl_next_index; i++)
		Label_Region_Area[i] = 0;

	// 刪除前
	//cout << "Delete Before" << endl;
	//for (int i = 0; i < height; i++) {
	//	for (int j = 0; j < width; j++) {
	//		cout << LabelMapPtr[i][j] << " ";
	//	} // for j
	//	cout << endl;
	//} // for i

	vector<DeletePoint> DeleteRegion;

	// 根據 連通標籤圖(LabelMapPtr) 計算各區塊 之 面積
	for (int i = 0; i < height; i++) {

		for (int j = 0; j < width; j++) {

			if (LabelMapPtr[i][j] == 1) // 我是邊緣，請刪除我
				LabelMapPtr[i][j] = 1;

			else if (LabelMapPtr[i][j] >= 2) { // 我是連通圖 計算面積

				DeletePoint p;

				p.x = i;
				p.y = j;
				p.label = LabelMapPtr[i][j];

				DeleteRegion.push_back(p); // 記下該點 未來可能將之刪除

				Label_Region_Area[LabelMapPtr[i][j]] ++; // 製作 對照表 對應 Label 之 Area 

			} // else if

		} // for j

	} // for i

	//// 顯示 各區塊面積
	/*for (int i = 0; i < ccl_next_index; i++)
		cout << "area[" << i + 1 << "]:" << Label_Region_Area[i] << endl;*/

	for (int i = 0; i < DeleteRegion.size(); i++) {

		if (DeleteRegion[i].label >= 2) { // 2 以上 表示 該為連通圖

			if (Label_Region_Area[DeleteRegion[i].label] < area) { // 該 連通圖 面積 小於 門檻值 則砍掉

				LabelMapPtr[DeleteRegion[i].x][DeleteRegion[i].y] = 0; // 設為背景
				imgarray[DeleteRegion[i].x][DeleteRegion[i].y] = 0; // 設為背景


			} // if

		} // if

	} // for i

	// 刪除後
	//cout << "Delete After" << endl;
	//for (int i = 0; i < height; i++) {
	//	for (int j = 0; j < width; j++) {
	//		cout << LabelMapPtr[i][j] << " ";
	//	} // for j
	//	cout << endl;
	//} // for i
	for (int i = 0; i < height; i++)
	for (int j = 0; j < width; j++)
		LabelMap[j][i] = LabelMapPtr[i][j];
	fout_CCL.open(("CCL.txt"), ios::out);
	for (int j = 0; j<height_m; j++) {
		for (int i = 0; i<width_m; i++) {
			fout_CCL << LabelMap[i][j];

		}
		fout_CCL << endl;
	}
	fout_CCL.close();

	// --------------------- 資源釋放 ---------------------
	for (int i = 0; i < height; i++)
		delete LabelMapPtr[i];
	delete[] LabelMapPtr;

	delete Label_Region_Area;
	// ----------------------------------------------------

} // CCL_Delete()