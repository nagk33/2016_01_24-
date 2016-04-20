#pragma once

#include "cv.h"
#include "highgui.h"
#include "Process.h"
#include "struct.h"
#include "CCL.h"
#include "GetROI.h"
#include "Recognition.h"

# include <vector>

#include <windows.h>



/*

void RGBtoYCbCr(IplImage *);
//uchar YCbCr_Y[669][1000];
//uchar YCbCr_Cb[669][1000];
//uchar YCbCr_Cr[669][1000];
uchar Gray[1000][1000];


*/

//void drawpoint() {
//
//	for (int i = 2; i < g_Point.size(); i++) {
//
//		//cvCircle(frame_320x240, g_Point[i], 2, CV_RGB(255, 0, 0), 3, CV_AA, 1);
//		cvLine(frame_320x240, g_Point[i - 1], g_Point[i], CV_RGB(255, 0, 0), 3, CV_AA, 0);
//
//	} // for
//
//} // drawpoint()

// 新增平滑法
void drawpoint() {

	// 平滑程度，越大越平順!!
	int smooth = 5;
	int smooth_temp = 0;

	// 由於 有位知名的 bug 導致 第一和第二個點 必須先排除，因此畫線起始點從 第 3 個 point 開始 ( index = 2 開始)。
	int start_point = 2; // 畫筆 的 起始點

	for (int i = start_point; i < g_Point.size(); i++) {

		int p1x = 0;
		int p1y = 0;
		int p2x = 0;
		int p2y = 0;

		// 考慮 點數 少於 平滑程度
		if (i < smooth) {

			smooth_temp = i - (smooth - 1);

		} // if

		else {

			smooth_temp = smooth;

		} // else

		// 累加 p1 和 p2 自己與前n個點
		for (int j = 0; j < smooth_temp - start_point; j++) {

			p1x += g_Point[(i - 1) - ((smooth_temp - start_point) - j)].x;
			p1y += g_Point[(i - 1) - ((smooth_temp - start_point) - j)].y;

			p2x += g_Point[i - ((smooth_temp - start_point) - j)].x;
			p2y += g_Point[i - ((smooth_temp - start_point) - j)].y;

		} // for


		// 平均 p1 和 p2 ，完成平滑法
		p1x /= (smooth_temp - start_point);
		p1y /= (smooth_temp - start_point);
		p2x /= (smooth_temp - start_point);
		p2y /= (smooth_temp - start_point);


		// 寫入平滑後的點
		//cvCircle(frame_320x240, g_Point[i], 2, CV_RGB(255, 0, 0), 3, CV_AA, 1);
		cvLine(frame_320x240, cvPoint(p1x, p1y), cvPoint(p2x, p2y), CV_RGB(255, 0, 0), 3, CV_AA, 0);

		/*mouse_event(MOUSEEVENTF_MOVE,p2x-p1x,p2y-p1y,0,0);*/
		//mouse_event(MOUSEEVENTF_XDOWN, (p2x - p1x) / 1.5, (p2y - p1y) / 1.5, 0, 0);
		// mouse_event(MOUSE_MOVE_ABSOLUTE, 5, 5, 0, 0);
		// mouse_event(MOUSE_MOVED, (p2x - p1x) / 1.5, (p2y - p1y) / 1.5, 0, 0);
		// mouse_event(MOUSE_MOVE_RELATIVE, (p2x - p1x) / 1.5, (p2y - p1y) / 1.5, 0, 0);

		//cout << (p2x - p1x) / 1.5 << ", " << (p2y - p1y) / 1.5 << endl;
		int x = 6;
		int y = 3.375;

		mouse_event(MOUSE_MOVED, ((p2x - p1x) / (6)), (p2y - p1y) / (3.375), 0, 0);


		//cvLine(line, cvPoint(p1x, p1y), cvPoint(p2x, p2y), CV_RGB(255, 0, 0), 3, CV_AA, 1);
	} // for

} // drawpoint()

// 產生一個 Best LDN 的 Table
// 第 1 個，是 非完美的 bin
// 其他，完美的 bin，照順序排列
void CreateTheBestLDNHashTable() {

	for (int i = 0; i < g_TheBestLDNSize - 1; i++) {

		g_Hash_Transfor_to_TheBestLDNBin[g_TheBestLDNBins[i]] = i + 1;

	} // for

} // CreateTheBestLDNHashTable()

IplImage *rotateImage2(IplImage *src, float angleDegrees)
{
	// Create a map_matrix, where the left 2x2 matrix
	// is the transform and the right 2x1 is the dimensions.
	float m[6];
	CvMat M = cvMat(2, 3, CV_32F, m);
	int w = src->width;
	int h = src->height;
	float angleRadians = angleDegrees * ((float)CV_PI / 180.0f);
	m[0] = (float)(cos(angleRadians));
	m[1] = (float)(sin(angleRadians));
	m[3] = -m[1];
	m[4] = m[0];
	m[2] = w*0.5f;
	m[5] = h*0.5f;

	// Make a spare image for the result
	CvSize sizeRotated;
	sizeRotated.width = cvRound(h);
	sizeRotated.height = cvRound(w);

	// Rotate
	IplImage *imageRotated = cvCreateImage(sizeRotated, src->depth, src->nChannels);

	// Transform the image
	cvGetQuadrangleSubPix(src, imageRotated, &M);

	return imageRotated;
}

int main() {


	int extend_ROI_p1x = 0; // P1.x 放大框架
	int extend_ROI_p1y = 0; // P1.y 放大框架
	int extend_ROI_p2x = 0; // P2.x 放大框架
	int extend_ROI_p2y = 0; // P2.y 放大框架

	IplImage * temp = cvCreateImage(cvSize(width_m, height_m), 8, 1);

	// 建立 Best LDN 對照表
	CreateTheBestLDNHashTable();

	int Frame_Count = 0; //方便計算PFS
	capture = cvCaptureFromCAM(0);//0是camera編號

	// 產生 Once Frame 給追蹤用
	OnceFrameIpl_forCanny = cvCreateImage(cvSize(width_m, height_m), 8, 1);
	temp_test_tracking = cvCreateImage(cvSize(width_m, height_m), 8, 1);

	while (1) {


		Frame_Count++;
		frame = cvQueryFrame(capture); //Query frame from capture cam

		// cvFlip(frame, frame, 1); // 將原影像垂直反轉，注意這個反轉後的影像不會寫回原影像
		//IplImage * frame_320x240_temp = rotateImage2(frame, 90); // 旋轉影像 270 度

		// ==================== 沒有旋轉影像 ========================================
		// Resize 640*480 ->320*240
		frame_320x240_size.height = frame->height / scale_m;    //scale_m = 2 各除二

		frame_320x240_size.width = frame->width / scale_m;
		frame_320x240 = cvCreateImage(frame_320x240_size, frame->depth, frame->nChannels);
		cvResize(frame, frame_320x240, CV_INTER_LINEAR);  // frame -> frame_320x240

		//resize後轉Ycbcr
		YCbCr_frame = cvCreateImage(frame_320x240_size, frame->depth, frame->nChannels);
		cvCvtColor(frame_320x240, YCbCr_frame, CV_BGR2YCrCb);

		// cvReleaseImage(&frame_320x240_temp); /// 釋放 旋轉影像
		// ==========================================================================

		//// ==================== 旋轉影像 ============================================
		//// Resize 640*480 ->240*320 (因為要旋轉 寬高要對調)
		//frame_320x240_size.height = frame->width / scale_m;    //scale_m = 2 各除二

		//frame_320x240_size.width =  frame->height / scale_m;

		////cout << "h:" << frame_320x240_size.height << ", w:" << frame_320x240_size.width << endl;

		//IplImage * frame_320x240_temp = rotateImage2(frame, 270); // 旋轉影像 270 度

		//frame_320x240 = cvCreateImage(frame_320x240_size, frame->depth, frame->nChannels); // 縮小 1/2

		//cvResize(frame_320x240_temp, frame_320x240, CV_INTER_LINEAR);  // frame -> frame_320x240

		//cvReleaseImage(&frame_320x240_temp);

		////resize後轉Ycbcr
		//YCbCr_frame = cvCreateImage(frame_320x240_size, frame->depth, frame->nChannels);
		//cvCvtColor(frame_320x240, YCbCr_frame, CV_BGR2YCrCb);
		//// ==========================================================================

		/////////////
		CvPoint pt0;
		CvMemStorage* storage = cvCreateMemStorage(0);
		CvSeq* ptseq = cvCreateSeq(CV_SEQ_KIND_GENERIC | CV_32SC2, sizeof(CvContour), sizeof(CvPoint), storage);
		CvSeq* hull;
		int hullcontour;

		//讀影像
		for (int i = 0; i < frame_320x240->height; i++)
		for (int j = 0; j < frame_320x240->widthStep; j = j + 3) {
			read = YCbCr_frame->imageData[i*YCbCr_frame->widthStep + j];
			YCbCr_Y[(int)j / 3][i] = (int)read;
			read = YCbCr_frame->imageData[i*YCbCr_frame->widthStep + j + 2];
			YCbCr_Cb[(int)j / 3][i] = (int)read;
			read = YCbCr_frame->imageData[i*YCbCr_frame->widthStep + j + 1];
			YCbCr_Cr[(int)j / 3][i] = (int)read;
		}
		/*  存影像
		for (int i = 0; i < frame_320x240->height; i++)
		{
		for (int j = 0; j<frame_320x240->widthStep; j++)
		{
		YCbCr_frame->imageData[i*pImageYCbCr1->widthStep + j] = YCbCr_Y[(int)j / 3][i];
		YCbCr_frame->imageData[i*pImageYCbCr1->widthStep + j + 1] = YCbCr_Cr[(int)j / 3][i];
		YCbCr_frame->imageData[i*pImageYCbCr1->widthStep + j + 2] = YCbCr_Cb[(int)j / 3][i];
		}
		}
		*/

		switch (flag) {

		case 1:
			
			Temporal_Difference();
			if (Frame_Count % 6 == 0) // fps = 8  4 ->3
			{

				// 針對膚色做二值化!!!!!!!!!!!!!!!!!!!!!!
				for (int i = 0; i < width_m; i++)
				for (int j = 0; j < height_m; j++) {//DiffFrame_Count[i][j]>3 相差過門檻值累加超過三次 才會視為前景改變
					if (DiffFrame_Count[i][j] > 2 /*&& YCbCr_Cr [ i ] [ j ] > 134 && YCbCr_Cr [ i ] [ j ] < 176 && YCbCr_Cb [ i ] [ j ] > 97 && YCbCr_Cb [ i ] [ j ] < 142*/) // && YCbCr_Cr[i][j] > 133 && YCbCr_Cr[i][j] < 174 && YCbCr_Cb[i][j] > 77 && YCbCr_Cb[i][j] < 127
					{
						Mix_ST[i][j] = 255;
						//cout << DiffFrame_Count[i][j] ;
					}
					else // <T
					{
						Mix_ST[i][j] = 0;
					}

					DiffFrame_Count[i][j] = 0;
				}

				//擴張後侵蝕

				// ==================== CCL_Delete 用 ========================
				short **temp_min_st;
				temp_min_st = new short*[frame_320x240_size.height];
				for (int i = 0; i < frame_320x240_size.height; i++)
					temp_min_st[i] = new short[frame_320x240_size.width];
				// ===========================================================

				for (int i = 0; i < frame_320x240_size.width; i++) {
					for (int j = 0; j < frame_320x240_size.height; j++)	Mix_ST2[i][j] = Dilation(Mix_ST, i, j);
				}

				for (int i = 0; i < frame_320x240_size.width; i++) {
					for (int j = 0; j < frame_320x240_size.height; j++) {
						Mix_ST[i][j] = Erosion(Mix_ST2, i, j);
						temp_min_st[j][i] = Mix_ST[i][j]; // CCL_Delete 用
					} // for j 
				}

			    CCL_Delete(temp_min_st, frame_320x240_size.height, frame_320x240_size.width, 10);

				//for (int i = 0; i < frame_320x240_size.width; i++) {
				//	for (int j = 0; j < frame_320x240_size.height; j++) {
				//		Mix_ST[i][j] = temp_min_st[j][i]; // CCL_Delete 用
				//	} // for j 
				//}

				//CCL(Mix_ST);

				difference_size.height = height_m;    //scale_m = 2 各除二
				difference_size.width = width_m;
				difference = cvCreateImage(difference_size, frame->depth, frame->nChannels);

				// 將二值化的圖，存到「difference」理面，做為顯示用!!
				for (int i = 0; i < difference->height; i++) {
					for (int j = 0; j < difference->widthStep; j = j + 1) {
						//difference->imageData[i*difference->widthStep + j] = Mix_ST[(int)j / 3][i];
						difference->imageData[i*difference->widthStep + j] = temp_min_st[i][(int)j / 3];
						difference->imageData[i*difference->widthStep + j + 1] = 0;
						difference->imageData[i*difference->widthStep + j + 2] = 0;

						// cout << Mix_ST[i][j];

					}

				}


				// ====================== 資源釋放 =====================
				for (int i = 0; i < height_m; i++)
					delete temp_min_st[i];
				delete[]  temp_min_st;
				// =====================================================

				//temp_diff = cvCreateImage(cvSize(320, 240), 8, 1 );

				//for (int i = 0; i < temp_diff->height; i++) {
				//	for (int j = 0; j < temp_diff->width; j = j + 1) {

				//		CV_IMAGE_ELEM(temp_diff, byte, i, j) = Mix_ST[(int)j][i];

				//	} // for j
				//} // for i



				//difference = cvCloneImage(temp_diff);

				//cvReleaseImage(&temp_diff);

				//CCL(difference);

				GetROI();

				for (int i = 0; i < width_m; i++) {
					for (int j = 0; j < height_m; j++) {
						LabelMap[i][j] = 0;
						DiffFrame_Count[i][j] = 0;
						if (Frame_Count>100)
							Frame_Count = 0;
					}
				}

			}

			//cvShowImage("virtical image", frame_320x240);

			cvFlip(frame_320x240, NULL, 1); // 將原影像垂直反轉，注意這個反轉後的影像不會寫回原影像

			cvShowImage("Webcam", frame_320x240);
			cvShowImage("temporal_Difference", difference);
			cvReleaseImage(&difference);
			cvReleaseImage(&frame_320x240);
			cvReleaseImage(&YCbCr_frame);


			break;
		case 2:
			temp_candi_count = 0;
			//cop_harris = cvCreateImage(frame_320x240_size, IPL_DEPTH_8U, 1);
			//ori_virtical_flip = cvCreateImage(frame_320x240_size, frame_320x240->depth, frame_320x240->nChannels); // 秀出 反轉後的原影像
			//cop_harris = cvCloneImage(canny);
			// 做 canny
			// cvCanny(frame_320x240, canny, 50, 150, 3);
			if (Frame_Count % 1 == 0)
			{

				for (int x = 0; x < height_m; x++)
				for (int y = 0; y < width_m; y++)
					trackingDiffFrame_Count[y][x] = 0;
				Temporal_Difference_tracking();

				/*for (int i = 0; i < 320; i++) {
					for (int j = 0; j < 240; j++)
						trackingDiffFrame_Count1[i][j] = Erosion(trackingDiffFrame_Count, i, j);
				}

				for (int i = 0; i < 320; i++) {
					for (int j = 0; j < 240; j++)
						trackingDiffFrame_Count[i][j] = Erosion(trackingDiffFrame_Count1, i, j);
				}*/

				//short ** temp_tracking; // 用完需做資源釋放
				//temp_tracking = new short*[height_m];
				//for (int i = 0; i < height_m; i++)
				//	temp_tracking[i] = new short[width_m];

				///// ===================== 測試 ======================
				//for (int i = 0; i < width_m; i++)
				//for (int j = 0; j < height_m; j++) {
				//	CV_IMAGE_ELEM(temp, byte, j, i) = (byte)trackingDiffFrame_Count1[i][j] > 0 ? 255 : 0; //  if >0  =255 ,  else  = 0
				//	temp_tracking[j][i] = (byte)trackingDiffFrame_Count1[i][j] > 0 ? 255 : 0; //  if >0  =255 ,  else  = 0
				//} // for
				///// ==================================================

				///// ===================== 測試 ======================
				//// CCL( temp );
				//CCL_Delete(temp_tracking, height_m, width_m, 30);
				/////// ===================== 測試 ======================

				//for (int i = 0; i < width_m; i++)
				//for (int j = 0; j < height_m; j++) {
				//	//trackingDiffFrame_Count[i][j] = (byte)CV_IMAGE_ELEM(temp, byte, j, i);
				//	CV_IMAGE_ELEM(temp, byte, j, i) = temp_tracking[j][i];
				//	//trackingDiffFrame_Count[i][j] = temp_tracking[j][i];
				//} // for
				/////// ==================================================

				//// ====================== 資源釋放 =====================
				//for (int i = 0; i < height_m; i++)
				//	delete temp_tracking[i];
				//delete[]  temp_tracking;
				//// =====================================================

				//cvShowImage("CCCCL_After", temp);
				//cvZero(temp);
			}


			for (int i = 0; i < temp_test_tracking->height; i++)
			for (int j = 0; j < temp_test_tracking->width; j++)
			if (trackingDiffFrame_Count[j][i]>0)
				CV_IMAGE_ELEM(temp_test_tracking, byte, i, j) = 255;
			else
				CV_IMAGE_ELEM(temp_test_tracking, byte, i, j) = 0;

			cvShowImage("!!!!!!!!!!!!!!!", temp_test_tracking);

			if (Frame_Count % 1 == 0) // 最好是每1個 frame ，否則難以與畫筆同步同步 
			{
				histogram();
				g_Point.push_back(cvPoint(temp_topOfTheHandModel_update_x, temp_topOfTheHandModel_update_y)); // 存下每次要被連成線的點
			}
			//cvRectangle(frame_32 0x240, cvPoint(P1_x - (P2_x - P1_x), P1_y - (P2_y - P1_y) / 4), cvPoint(P2_x + (P2_x - P1_x) / 4 - (P2_x - P1_x), P2_y + (P2_y - P1_y) / 4 - (P2_y - P1_y)), CV_RGB(0, 255, 0), 1, CV_AA, 0.1);

			// ----------------針對原圖做二值化----------------


			//------------------ ENDDD 結束抓角點----------------------------------

			cvCircle(frame_320x240, P1, 3, CV_RGB(0, 255, 0), 1, 8, 0);
			cvCircle(frame_320x240, P2, 3, CV_RGB(0, 255, 0), 1, 8, 0);
			cvRectangle(frame_320x240, P1, P2, CV_RGB(0, 255, 0), 1, CV_AA, 0); // 畫當選者的ROI

			// ---------------- 對原影像畫線 -------------------
			//line = cvCreateImage(difference_size, frame->depth, frame->nChannels);

			// 開始畫線，繪畫剛剛存下的點
			 drawpoint();
			// ------------- END 結束畫線 -----------------------

			// 測試用，將影像反轉
			cvFlip(frame_320x240, NULL, 1); // 將原影像垂直反轉
			//cvShowImage("virtical image", frame_320x240);

			cvShowImage("Webcam", frame_320x240);
			cvReleaseImage(&frame_320x240);
			cvReleaseImage(&YCbCr_frame);
			cvReleaseImage(&cop_harris);
			cvReleaseImage(&trankingcanny);

			break;

			case 3:
			if ( Frame_Count % 2 == 0 ) {/*
			if ( click_flag == 1 ) {
			wait--;
			flag = 3;
			click_flag = 1;
			cvRectangle( frame_320x240, cvPoint( P1_x, P1_y ), cvPoint( P2_x, P2_y ), CV_RGB( 255, 0, 0 ), 2, 8, 0 );
			if ( wait == 0 ) {
			wait = 10;
			flag = 2;
			click_flag = 0;

			recognition_button();

			}

			}*/
			}

			cvShowImage( "Webcam", frame_320x240 );
			cvReleaseImage( &frame_320x240 );
			cvReleaseImage( &YCbCr_frame );
			break;

		} // switch

		cvWaitKey(1);

		// 資原釋放
		cvClearMemStorage(storage);
		cvClearMemStorage(ptseq->storage);

		//ptseq
	} // while( 1 )

	cvReleaseCapture(&capture);
	cvDestroyAllWindows();

	return 0;
}





