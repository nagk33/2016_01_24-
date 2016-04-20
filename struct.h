#pragma once
#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <math.h>
#include <fstream>

# include <vector>

# include <iomanip>

//// ��l���e
#define width_m 320
#define height_m 240

//// ����᪺���e
//#define width_m 240
//#define height_m 320

using namespace std;
int SobelFrame[width_m][height_m];
int Sobelcount = 0;
int Sobelcount2 = 0;
int Y[width_m][height_m];
int Cb[width_m][height_m];
int Cr[width_m][height_m];

ofstream infile("BestLDN.txt");

CvCapture *capture;
IplImage *frame;
IplImage *Process;
IplImage * canny; // �Ȧs�U canny ����
IplImage * harris;
IplImage * beforeCanny = NULL; // �w���ϰ��G�ȤơA�b��canny ��
IplImage * cop_harris;
IplImage * trankingcanny;

bool trytrytry = true; 

bool sinpho = false;

// ------- �e�ϡB�e���� ---------
int drawpoint_x = 0, drawpoint_y = 0; // �]�w���I�� x, y ��m

// -------- ���եΡA�i�H�N�R�� ----------
IplImage * ori_virtical_flip = NULL; // �N��v����������

int countTheZeroCandidateFrame = 0; // �Ψ� debug �ΡA�קK�d�u���W���L�a�j��v

const short scale_m = 2;

struct FeaturePoint {
    int x, y;
};

//Ū�v��
uchar read;

//Resize
IplImage *frame_320x240;
CvSize frame_320x240_size;

//��ܮɶ��t
IplImage *difference;
CvSize difference_size;
IplImage * temp_test_tracking;

//Process YCbCr
IplImage *YCbCr_frame;
short YCbCr_Y [ width_m ] [ height_m ];
short YCbCr_Cb [ width_m ] [ height_m ];
short YCbCr_Cr [ width_m ] [ height_m ];

//switch case
int flag = 1; //��frame�|���g�L�e�B�z
int testcount = 0;

//Process Temporal Difference
double Threshold = 0.0; //ARM = 4 , PC = 10;
short Pre [ width_m ] [ height_m ];
short PreTrack [width_m][height_m];
short DiffFrame_Count [ width_m ] [ height_m ];
short trackingDiffFrame_Count[width_m][height_m];
short trackingDiffFrame_Count1 [ width_m ] [ height_m ];

//�I���۴���G�Ȥƫ�
short Mix_ST [ width_m ] [ height_m ];
short Mix_ST2 [ width_m ] [ height_m ];

//CCL �s�q�K��V

short LabelMap [ width_m ] [ height_m ];
int Label_Index_Total;
fstream fout_CCL;
fstream fout_ROI;

typedef uchar byte;
int temp_candi_count = 0;

struct DeletePoint {
	int x, y;
	int label;
};

//get ROI
struct  ROI {
    int Max_Xx;  //�̤jX��(x,y)
    int Max_Xy;
    int Min_Yx;  //�̤pY��(x,y)
    int Min_Yy;
    int Center_X;  //�����I(x,y)
    int Center_Y;
    int Area;
    int feature_Amount1;  //�p�⨤�I�S�x�q
    int feature_Amount2;  //���I? 
};

int P1_x;
int P1_y;
int P2_x;
int P2_y;

CvPoint P1;
CvPoint P2;
CvPoint P3;
CvPoint P4;

int ROI_width = 0;
int ROI_height = 0;

int ClickBufferCount = 14; // �w�� click �� �I���t��

//���y�줤�߶Z��

int disbetweenfingertip = 0;
int disbetweenfingertip2 = 0;
//ROI���e���
double ROI_shape = 0;

short sobel_array[width_m] = { 0 };
short ROIrange_array [ 150 ] = { 0 };




// For Once
short OnceFrame[width_m][height_m];
IplImage * OnceFrameIpl_forCanny;

//��Ҥ��e�B�z
float subcount;

float morris = 0;


int pppppp = 0;

//Tracking_HOG

double all_HOG = 0;
int all_HOG_count = 0;

float gHOGhist_pre[9]; // HOG �����
float gHOGhist_new[9]; // HOG �����

float gAvgHOGhist_pre[9];
float gAvgHOGhist_new[9];
float gAvgHOGhist_update[9];

int num_of_HOG_count_pre = 0;
int num_of_HOG_count_new = 0;

int skin_total = 0;
int skin_total2 = 0;
int skin_total3 = 0;
double histogram0 = 0, histogram1 = 0, histogram2 = 0, histogram3 = 0, histogram4 = 0, histogram5 = 0, histogram6 = 0, histogram7 = 0, histogram8 = 0;
float total = 0;
double ave0 = 0, ave1 = 0, ave2 = 0, ave3 = 0, ave4 = 0, ave5 = 0, ave6 = 0, ave7 = 0, ave8 = 0;
bool once = 0;
int Px = P1_x, Py = P1_y;
int PreP1x = 0, PreP1y = 0;
int staticCount = 0;
//double ghistogram0 = 0, ghistogram1 = 0, ghistogram2 = 0, ghistogram3 = 0, ghistogram4 = 0, ghistogram5 = 0, ghistogram6 = 0, ghistogram7 = 0, ghistogram8, ghistogram9 = 0, ghistogram10 = 0, ghistogram11 = 0, ghistogram12 = 0, ghistogram13 = 0, ghistogram14 = 0, ghistogram15 = 0;
//double gave0 = 0, gave1 = 0, gave2 = 0, gave3 = 0, gave4 = 0, gave5 = 0, gave6 = 0, gave7 = 0, gave8, gave9 = 0, gave10 = 0, gave11 = 0, gave12 = 0, gave13 = 0, gave14 = 0, gave15 = 0;
int click_count = 0;
//int voice_count = 0;
bool click_flag = 0;
bool voice_flag = 0;
int wait = 0;
//-----------------LDN

double LDNhistogram0 = 0, LDNhistogram1 = 0, LDNhistogram2 = 0, LDNhistogram3 = 0, LDNhistogram4 = 0, LDNhistogram5 = 0, LDNhistogram6 = 0, LDNhistogram7 = 0, LDNhistogram8 = 0, LDNhistogram9 = 0, LDNhistogram10 = 0, LDNhistogram11 = 0, LDNhistogram12 = 0, LDNhistogram13 = 0, LDNhistogram14 = 0;
double LDNave0 = 0, LDNave1 = 0, LDNave2 = 0, LDNave3 = 0, LDNave4 = 0, LDNave5 = 0, LDNave6 = 0, LDNave7 = 0, LDNave8, LDNave9, LDNave10 = 0, LDNave11 = 0, LDNave12 = 0, LDNave13 = 0, LDNave14 = 0;

int g_TheBestLDNBins[] = {
	 4,  7,  15, 44, 56,  60, 112, 120,  135,  143, 159, 191, 
	193, 195, 199,
	225,  227,  231, 240, 241, 248, 249
};


IplImage * temp_diff;

// --------------------------------------------- The Best LDN ---------------------------------------------
// �]�w 34 �� �̴I�� �ⳡ�S�x �� LDN Bin
//int g_TheBestLDNBins [] = { 7, 14, 15, 28, 30, 31, 48, 56, 60, 62,
//                            63, 96, 112, 120, 124, 126, 131, 135, 143, 159,
//                            192, 193, 195, 199, 207, 224, 225, 227, 231, 239,
//                            240, 241, 247, 248 } ;

// ���` LDN �� 256 �� ( ���ݭn0�A�]�p���Y�A�w�g�]�t 0)
//int g_TheBestLDNBins [] = {
//    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
//    33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64,
//    65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96,
//    97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128,
//    129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160,
//    161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192,
//    193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224,
//    225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255
//} ;

//int g_TheBestLDNBins[] = {
//	2, 3, 5, 7, 8, 9, 10, 11, 12, 15, 17, 18, 19, 20, 24, 25, 26, 27, 28, 32,
//	33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 44, 48, 49, 50, 51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64,
//	65, 66, 67, 68, 69, 70, 71, 72, 74, 80, 96, 97, 98, 100, 102, 104, 106, 108, 112, 134, 135, 136, 137, 138, 140, 143, 144, 145, 149, 150, 152, 153, 160,
//	161, 164, 166, 168, 169, 172, 175, 176, 177, 180, 184, 192,
//	194, 196, 198, 200, 204, 208, 209, 214, 217, 232, 248
//};
// �H�� g_TheBestLDNBins �����e�A�ʺA�M�w g_TheBestLDNBins �� size
// �o�̥i�઺���G�|�O 35 ( 1 �� �D������ �[�W 34 �� ������ ) 
const int g_TheBestLDNSize = 1 + ( sizeof( g_TheBestLDNBins ) / sizeof( g_TheBestLDNBins[ 0 ] ) ); 

// hash function
// �N ������ LDN Bin �ഫ�� ������ �@�� LDN Bin
int  g_Hash_Transfor_to_TheBestLDNBin [ 256 ] = { 0 };

// Bset LDN �����
float g_TheBestLDNHistogram1 [ g_TheBestLDNSize ] = { 0 };
float g_TheBestLDNHistogram2[g_TheBestLDNSize] = { 0 };

// Best LDN ���e��
double g_TheBestLDNThroshold1 = 9999;
double g_TheBestLDNThroshold2 = 0;
double eligibility ; //����
double eligibilityLDN ;

IplImage * Tracking_Test;

// --------------------------------------------- The Best LDN ---------------------------------------------

vector<CvPoint> g_Point;

int feature = 0, feature2 = 0, center;
//short lefteye_pixel[320][240];
//short LDN3x3lefteye[320][240];


//double lefthist0 = 0, lefthist1 = 0, lefthist2 = 0, lefthist3 = 0, lefthist4 = 0, lefthist5 = 0, lefthist6 = 0, lefthist7 = 0, lefthist8 = 0, lefthist9 = 0, lefthist10 = 0, lefthist11 = 0, lefthist12 = 0, lefthist13 = 0, lefthist14 = 0, lefthist15 = 0;
//double righthist0 = 0, righthist1 = 0, righthist2 = 0, righthist3 = 0, righthist4 = 0, righthist5 = 0, righthist6 = 0, righthist7 = 0, righthist8 = 0, righthist9 = 0, righthist10 = 0, righthist11 = 0, righthist12 = 0, righthist13 = 0, righthist14 = 0, righthist15 = 0;

// �ⳡ������
float handmodel_pre[5][5];
float handmodel_new[5][5];
float handmodel_update[5][5];

int handmodel_count_pre = 0;
int handmodel_count_new = 0;

int handmodelmatching = 0;
int handmodelmatchingE = 0; // �@�Y �S�x�I �ⳡ����

int topOfTheHandModel_pre_x = 0; // �s�U �ⳡ�l�ܪ�ROI �̰��I �� x �y�� (handmodel_pre)
int topOfTheHandModel_pre_y = 0; // �s�U �ⳡ�l�ܪ�ROI �̰��I �� y �y�� (handmodel_pre)

int topOfTheHandModel_new_x = 0; // �s�U �ⳡ�l�ܪ�ROI �̰��I �� x �y�� (handmodel_new)
int topOfTheHandModel_new_y = 0; // �s�U �ⳡ�l�ܪ�ROI �̰��I �� y �y�� (handmodel_new)

int topOfTheHandModel_update_x = 0; // �s�U �ⳡ�l�ܪ�ROI �̰��I �� x �y�� (handmodel_new)
int topOfTheHandModel_update_y = 0; // �s�U �ⳡ�l�ܪ�ROI �̰��I �� y �y�� (handmodel_new)

int temp_topOfTheHandModel_update_x = 0; // �s�U �ⳡ�l�ܪ�ROI �̰��I �� x �y�� (handmodel_new)
int temp_topOfTheHandModel_update_y = 0; // �s�U �ⳡ�l�ܪ�ROI �̰��I �� y �y�� (handmodel_new)

int topOfTheHandModel = height_m; // �ɶq �� �ⳡ�l�ܪ�ROI �ɶq ���y�I ����
int topOfTheHandModelE = height_m; // �ɶq �� �ⳡ�l�ܪ�ROI �ɶq ���y�I ����

int topOfTheHandModelROI = 4; // �ɶq �� �ⳡ�l�ܪ�ROI �ɶq ���y�I ����
int topOfTheHandModelROIE = 4; // �ɶq �� �ⳡ�l�ܪ�ROI �ɶq ���y�I ����

//    ------click
short fistY[width_m][height_m];
short fistCb[width_m][height_m];
short fistCr[width_m][height_m];
int fist_x = 0, fist_y = 0;
short fist2Y[width_m][height_m];
short fist2Cb[width_m][height_m];
short fist2Cr[width_m][height_m];

short fist2Y_temp[width_m][height_m];


//---------------rotation
int rotationPx = 0, rotationPy = 0;
int rotationPrePx = 0, rotationPrePy = 0;
int rotation_orientation = 0;
int rotation_count = 0;
int rotation_disX = 0;
int rotation_disY = 0;
