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

// �s�W���ƪk
void drawpoint() {

	// ���Ƶ{�סA�V�j�V����!!
	int smooth = 5;
	int smooth_temp = 0;

	// �ѩ� ���쪾�W�� bug �ɭP �Ĥ@�M�ĤG���I �������ư��A�]���e�u�_�l�I�q �� 3 �� point �}�l ( index = 2 �}�l)�C
	int start_point = 2; // �e�� �� �_�l�I

	for (int i = start_point; i < g_Point.size(); i++) {

		int p1x = 0;
		int p1y = 0;
		int p2x = 0;
		int p2y = 0;

		// �Ҽ{ �I�� �֩� ���Ƶ{��
		if (i < smooth) {

			smooth_temp = i - (smooth - 1);

		} // if

		else {

			smooth_temp = smooth;

		} // else

		// �֥[ p1 �M p2 �ۤv�P�en���I
		for (int j = 0; j < smooth_temp - start_point; j++) {

			p1x += g_Point[(i - 1) - ((smooth_temp - start_point) - j)].x;
			p1y += g_Point[(i - 1) - ((smooth_temp - start_point) - j)].y;

			p2x += g_Point[i - ((smooth_temp - start_point) - j)].x;
			p2y += g_Point[i - ((smooth_temp - start_point) - j)].y;

		} // for


		// ���� p1 �M p2 �A�������ƪk
		p1x /= (smooth_temp - start_point);
		p1y /= (smooth_temp - start_point);
		p2x /= (smooth_temp - start_point);
		p2y /= (smooth_temp - start_point);


		// �g�J���ƫ᪺�I
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

// ���ͤ@�� Best LDN �� Table
// �� 1 �ӡA�O �D������ bin
// ��L�A������ bin�A�Ӷ��ǱƦC
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


	int extend_ROI_p1x = 0; // P1.x ��j�ج[
	int extend_ROI_p1y = 0; // P1.y ��j�ج[
	int extend_ROI_p2x = 0; // P2.x ��j�ج[
	int extend_ROI_p2y = 0; // P2.y ��j�ج[

	IplImage * temp = cvCreateImage(cvSize(width_m, height_m), 8, 1);

	// �إ� Best LDN ��Ӫ�
	CreateTheBestLDNHashTable();

	int Frame_Count = 0; //��K�p��PFS
	capture = cvCaptureFromCAM(0);//0�Ocamera�s��

	// ���� Once Frame ���l�ܥ�
	OnceFrameIpl_forCanny = cvCreateImage(cvSize(width_m, height_m), 8, 1);
	temp_test_tracking = cvCreateImage(cvSize(width_m, height_m), 8, 1);

	while (1) {


		Frame_Count++;
		frame = cvQueryFrame(capture); //Query frame from capture cam

		// cvFlip(frame, frame, 1); // �N��v����������A�`�N�o�Ӥ���᪺�v�����|�g�^��v��
		//IplImage * frame_320x240_temp = rotateImage2(frame, 90); // ����v�� 270 ��

		// ==================== �S������v�� ========================================
		// Resize 640*480 ->320*240
		frame_320x240_size.height = frame->height / scale_m;    //scale_m = 2 �U���G

		frame_320x240_size.width = frame->width / scale_m;
		frame_320x240 = cvCreateImage(frame_320x240_size, frame->depth, frame->nChannels);
		cvResize(frame, frame_320x240, CV_INTER_LINEAR);  // frame -> frame_320x240

		//resize����Ycbcr
		YCbCr_frame = cvCreateImage(frame_320x240_size, frame->depth, frame->nChannels);
		cvCvtColor(frame_320x240, YCbCr_frame, CV_BGR2YCrCb);

		// cvReleaseImage(&frame_320x240_temp); /// ���� ����v��
		// ==========================================================================

		//// ==================== ����v�� ============================================
		//// Resize 640*480 ->240*320 (�]���n���� �e���n���)
		//frame_320x240_size.height = frame->width / scale_m;    //scale_m = 2 �U���G

		//frame_320x240_size.width =  frame->height / scale_m;

		////cout << "h:" << frame_320x240_size.height << ", w:" << frame_320x240_size.width << endl;

		//IplImage * frame_320x240_temp = rotateImage2(frame, 270); // ����v�� 270 ��

		//frame_320x240 = cvCreateImage(frame_320x240_size, frame->depth, frame->nChannels); // �Y�p 1/2

		//cvResize(frame_320x240_temp, frame_320x240, CV_INTER_LINEAR);  // frame -> frame_320x240

		//cvReleaseImage(&frame_320x240_temp);

		////resize����Ycbcr
		//YCbCr_frame = cvCreateImage(frame_320x240_size, frame->depth, frame->nChannels);
		//cvCvtColor(frame_320x240, YCbCr_frame, CV_BGR2YCrCb);
		//// ==========================================================================

		/////////////
		CvPoint pt0;
		CvMemStorage* storage = cvCreateMemStorage(0);
		CvSeq* ptseq = cvCreateSeq(CV_SEQ_KIND_GENERIC | CV_32SC2, sizeof(CvContour), sizeof(CvPoint), storage);
		CvSeq* hull;
		int hullcontour;

		//Ū�v��
		for (int i = 0; i < frame_320x240->height; i++)
		for (int j = 0; j < frame_320x240->widthStep; j = j + 3) {
			read = YCbCr_frame->imageData[i*YCbCr_frame->widthStep + j];
			YCbCr_Y[(int)j / 3][i] = (int)read;
			read = YCbCr_frame->imageData[i*YCbCr_frame->widthStep + j + 2];
			YCbCr_Cb[(int)j / 3][i] = (int)read;
			read = YCbCr_frame->imageData[i*YCbCr_frame->widthStep + j + 1];
			YCbCr_Cr[(int)j / 3][i] = (int)read;
		}
		/*  �s�v��
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

				// �w�ｧ�ⰵ�G�Ȥ�!!!!!!!!!!!!!!!!!!!!!!
				for (int i = 0; i < width_m; i++)
				for (int j = 0; j < height_m; j++) {//DiffFrame_Count[i][j]>3 �ۮt�L���e�Ȳ֥[�W�L�T�� �~�|�����e������
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

				//�X�i��I�k

				// ==================== CCL_Delete �� ========================
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
						temp_min_st[j][i] = Mix_ST[i][j]; // CCL_Delete ��
					} // for j 
				}

			    CCL_Delete(temp_min_st, frame_320x240_size.height, frame_320x240_size.width, 10);

				//for (int i = 0; i < frame_320x240_size.width; i++) {
				//	for (int j = 0; j < frame_320x240_size.height; j++) {
				//		Mix_ST[i][j] = temp_min_st[j][i]; // CCL_Delete ��
				//	} // for j 
				//}

				//CCL(Mix_ST);

				difference_size.height = height_m;    //scale_m = 2 �U���G
				difference_size.width = width_m;
				difference = cvCreateImage(difference_size, frame->depth, frame->nChannels);

				// �N�G�Ȥƪ��ϡA�s��udifference�v�z���A������ܥ�!!
				for (int i = 0; i < difference->height; i++) {
					for (int j = 0; j < difference->widthStep; j = j + 1) {
						//difference->imageData[i*difference->widthStep + j] = Mix_ST[(int)j / 3][i];
						difference->imageData[i*difference->widthStep + j] = temp_min_st[i][(int)j / 3];
						difference->imageData[i*difference->widthStep + j + 1] = 0;
						difference->imageData[i*difference->widthStep + j + 2] = 0;

						// cout << Mix_ST[i][j];

					}

				}


				// ====================== �귽���� =====================
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

			cvFlip(frame_320x240, NULL, 1); // �N��v����������A�`�N�o�Ӥ���᪺�v�����|�g�^��v��

			cvShowImage("Webcam", frame_320x240);
			cvShowImage("temporal_Difference", difference);
			cvReleaseImage(&difference);
			cvReleaseImage(&frame_320x240);
			cvReleaseImage(&YCbCr_frame);


			break;
		case 2:
			temp_candi_count = 0;
			//cop_harris = cvCreateImage(frame_320x240_size, IPL_DEPTH_8U, 1);
			//ori_virtical_flip = cvCreateImage(frame_320x240_size, frame_320x240->depth, frame_320x240->nChannels); // �q�X ����᪺��v��
			//cop_harris = cvCloneImage(canny);
			// �� canny
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

				//short ** temp_tracking; // �Χ��ݰ��귽����
				//temp_tracking = new short*[height_m];
				//for (int i = 0; i < height_m; i++)
				//	temp_tracking[i] = new short[width_m];

				///// ===================== ���� ======================
				//for (int i = 0; i < width_m; i++)
				//for (int j = 0; j < height_m; j++) {
				//	CV_IMAGE_ELEM(temp, byte, j, i) = (byte)trackingDiffFrame_Count1[i][j] > 0 ? 255 : 0; //  if >0  =255 ,  else  = 0
				//	temp_tracking[j][i] = (byte)trackingDiffFrame_Count1[i][j] > 0 ? 255 : 0; //  if >0  =255 ,  else  = 0
				//} // for
				///// ==================================================

				///// ===================== ���� ======================
				//// CCL( temp );
				//CCL_Delete(temp_tracking, height_m, width_m, 30);
				/////// ===================== ���� ======================

				//for (int i = 0; i < width_m; i++)
				//for (int j = 0; j < height_m; j++) {
				//	//trackingDiffFrame_Count[i][j] = (byte)CV_IMAGE_ELEM(temp, byte, j, i);
				//	CV_IMAGE_ELEM(temp, byte, j, i) = temp_tracking[j][i];
				//	//trackingDiffFrame_Count[i][j] = temp_tracking[j][i];
				//} // for
				/////// ==================================================

				//// ====================== �귽���� =====================
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

			if (Frame_Count % 1 == 0) // �̦n�O�C1�� frame �A�_�h���H�P�e���P�B�P�B 
			{
				histogram();
				g_Point.push_back(cvPoint(temp_topOfTheHandModel_update_x, temp_topOfTheHandModel_update_y)); // �s�U�C���n�Q�s���u���I
			}
			//cvRectangle(frame_32 0x240, cvPoint(P1_x - (P2_x - P1_x), P1_y - (P2_y - P1_y) / 4), cvPoint(P2_x + (P2_x - P1_x) / 4 - (P2_x - P1_x), P2_y + (P2_y - P1_y) / 4 - (P2_y - P1_y)), CV_RGB(0, 255, 0), 1, CV_AA, 0.1);

			// ----------------�w���ϰ��G�Ȥ�----------------


			//------------------ ENDDD �����쨤�I----------------------------------

			cvCircle(frame_320x240, P1, 3, CV_RGB(0, 255, 0), 1, 8, 0);
			cvCircle(frame_320x240, P2, 3, CV_RGB(0, 255, 0), 1, 8, 0);
			cvRectangle(frame_320x240, P1, P2, CV_RGB(0, 255, 0), 1, CV_AA, 0); // �e���̪�ROI

			// ---------------- ���v���e�u -------------------
			//line = cvCreateImage(difference_size, frame->depth, frame->nChannels);

			// �}�l�e�u�Aø�e���s�U���I
			 drawpoint();
			// ------------- END �����e�u -----------------------

			// ���եΡA�N�v������
			cvFlip(frame_320x240, NULL, 1); // �N��v����������
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

		// �������
		cvClearMemStorage(storage);
		cvClearMemStorage(ptseq->storage);

		//ptseq
	} // while( 1 )

	cvReleaseCapture(&capture);
	cvDestroyAllWindows();

	return 0;
}





