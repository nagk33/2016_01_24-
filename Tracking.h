#pragma once
#include"struct.h"
#include"Process.h"
#include "CCL.h"
#include<windows.h>

void recognition_button(void);
bool keeplastpoint(int ,int,char);
bool keeplastpoint(int a, int b, char c);

void histogram() {

	int temp_px = P1.x; // 測試用
	int temp_py = P1.y; // 測試用

	bool isGetTrackingROI = false;
	char thumbpoint = 0;
	bool is_handmodel_pre_percent = true;
	IplImage * once_temp;

	morris = 0;

	// 目前可以讓使用者使用500個點畫線
	// 當「畫筆的點數以用盡」後，再容許50個點的緩衝時間
	// 定位失敗，否則強制重新定位
	if (g_Point.size() + 50 >= 500 || // 畫筆點數用盡
		countTheZeroCandidateFrame >= 20) { // 卡無窮迴圈
		//if (0){
		// flag = 1; //回去抓ROI
		// cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
		if (g_Point.size() + 50 >= 500)
			cout << "畫筆的點數以用盡，強制回重新定位!!" << endl;

		else if (countTheZeroCandidateFrame >= 20)
			cout << "卡無窮迴圈，強制回重新定位!!" << endl;

		countTheZeroCandidateFrame = 0; // 初始化

		flag = 1; // 強制回重定位

		g_Point.clear(); // 清除先前的點數!!

	} // if

	wait--;

	double ave_0 = 0, ave_1 = 0, ave_2 = 0, ave_3 = 0, ave_4 = 0, ave_5 = 0, ave_6 = 0, ave_7 = 0, ave_8 = 0;
	double LDNave_0 = 0, LDNave_1 = 0, LDNave_2 = 0, LDNave_3 = 0, LDNave_4 = 0, LDNave_5 = 0, LDNave_6 = 0, LDNave_7 = 0, LDNave_8 = 0, LDNave_9 = 0, LDNave_10 = 0, LDNave_11 = 0, LDNave_12 = 0, LDNave_13 = 0, LDNave_14 = 0;
	double gave_0 = 0, gave_1 = 0, gave_2 = 0, gave_3 = 0, gave_4 = 0, gave_5 = 0, gave_6 = 0, gave_7 = 0, gave_8 = 0, gave_9 = 0, gave_10 = 0, gave_11 = 0, gave_12 = 0, gave_13 = 0, gave_14 = 0, gave_15 = 0;
	double candicate = 0;  //候選者
	double candicateLDN = 0;//候選者

	// 手指間點 初始化
	topOfTheHandModelE = height_m;

	// 手指尖點 在 手型模組 的 最高點 初始化
	topOfTheHandModelROIE = height_m;

	//double candicateLDN2 = 9999;

	if (once == 0) // 特徵只須抓一次!!
	{
		ClickBufferCount = 14; // click 初始化

		// 初始畫 尖點 位置
		PreP1x = -1;
		PreP1y = -1;

		topOfTheHandModel = height_m; // 最高 ROI 點 選取 初始化

		// 手部輪廓 初始化
		for (int index_x = 0; index_x < 4; index_x++)
		for (int index_y = 0; index_y < 4; index_y++)
			handmodel_pre[index_x][index_y] = 0;

		// HOG 質方突 初始化
		for (int index = 0; index < 8; index++)
			gAvgHOGhist_pre[index] = 0;

		once_temp = cvCloneImage(OnceFrameIpl_forCanny);

		eligibility = 0; //當選者
		eligibilityLDN = 9999;

		// ----------------------- Best LDN Histogram 初始化 -----------------------
		for (int i = 0; i < g_TheBestLDNSize; i++) {

			g_TheBestLDNHistogram1[i] = 0;

		} // for 
		g_TheBestLDNThroshold1 = 9999;
		// ----------------------- Best LDN Histogram 初始化 -----------------------

		ave0 = 0, ave1 = 0, ave2 = 0, ave3 = 0, ave4 = 0, ave5 = 0, ave6 = 0, ave7 = 0, ave8 = 0;
		total = 0;
		histogram0 = 0, histogram1 = 0, histogram2 = 0, histogram3 = 0, histogram4 = 0, histogram5 = 0, histogram6 = 0, histogram7 = 0, histogram8 = 0;
		LDNhistogram0 = 0, LDNhistogram1 = 0, LDNhistogram2 = 0, LDNhistogram3 = 0, LDNhistogram4 = 0, LDNhistogram5 = 0, LDNhistogram6 = 0, LDNhistogram7 = 0, LDNhistogram8 = 0;
		skin_total = 0;

		// 初始畫 HOG 質方突
		for (int i = 0; i < 8; i++)
			gHOGhist_pre[i] = 0;

		cvRectangle(once_temp, cvPoint(P1_x, P1_y), cvPoint(P2_x, P2_y), cvScalar(0), 1, 8, 0);

		for (int i = (P1_x >= 0) ? P1_x : 0; // 避免 ROI 範圍 過小 超過 frame 的最邊界
			i < P1_x + (P2_x - P1_x) && i < width_m; // 避免 ROI 範圍 過大 超過 frame 的最邊界
			i++)
		for (int j = (P1_y >= 0) ? P1_y : 0; // 避免 ROI 範圍  過小 超過 frame 的最邊界
			j < P1_y + (P2_y - P1_y) && j < height_m; // 避免 ROI 範圍 過大 超過 frame 的最邊界
			j++) {

			if (trackingDiffFrame_Count[i][j]>0)
				//if (cvGet2D(OnceFrameIpl_forCanny, j, i).val[0] > 0)
				//if ( YCbCr_Cr [ i ] [ j ] > 133 && YCbCr_Cr [ i ] [ j ] < 174 && YCbCr_Cb [ i ] [ j ] > 77 && YCbCr_Cb [ i ] [ j ] < 127 )
			{
				skin_total++;
				total++;

				handmodel_count_pre++;

				// ================ 抓出 手 模組 的 輪廓 ================
				int duration_x = ROI_width / 4; // block 寬 的 長度
				int duration_y = ROI_height / 4; // block 高 的 長度

				int block_width = (i - P1_x >= 0 ? i - P1_x : 0) / duration_x; // 找出 block 的 x 軸座標
				int block_height = (j - P1_y >= 0 ? j - P1_y : 0) / duration_y; // 找出 block 的 y 軸座標

				handmodel_pre[block_height][block_width] ++; // 累計 特徵點 在 指定 位置 的 特徵點 數量

				if (block_width >= 4 || block_height >= 4) // 針對 邊界問題 選擇 不計算
					handmodel_count_pre--;

				// cout << "handmodel_pre[" << block_x << "][" << block_y << "]:" << handmodel_pre[block_x][block_y]<< endl;

				if (j < topOfTheHandModel) { // 存下 手部追蹤的ROI 最高點 的 x, y 座標 (handmodel_pre)
					topOfTheHandModel = j; // 更新目前最高點
					topOfTheHandModel_pre_x = block_width;
					topOfTheHandModel_pre_y = block_height;
				} // if
				// ======================================================



				cvCircle(once_temp, cvPoint(i, j), 1, cvScalar(0), 1, 8, 0); // 畫出 特徵點

				//--------------------------------HOG---------------------------------------------
				int Gx = 0, Gy = 0, Gxy = 0; //計算梯度值
				if (i != 0 && i != width_m && j != 0 && j != height_m) //i,j>0 且不要是最邊點
					//Gx = YCbCr_Y[i + 1][j] - YCbCr_Y[i - 1][j];  //左右方向灰度值相減
					Gy = YCbCr_Y[i][j + 1] - YCbCr_Y[i][j - 1];	 //上下相減
				int atan = 0;
				//atan = atan2(YCbCr_Y[i][j + 1], YCbCr_Y[i][j - 1]) * 180 / 3.14;
				//atan = atan2(Gy, Gx) * 180 / 3.14;
				//cout << "Gy=========:: " << atan << endl;

				if (-40 <= Gy && Gy < -30)     //將梯度歸納到八個方向中
					gHOGhist_pre[0] ++;
				else if (-30 <= Gy && Gy < -20)
					gHOGhist_pre[1]++;
				else if (-20 <= Gy && Gy < -10)
					gHOGhist_pre[2]++;
				else if (-10 <= Gy && Gy < 0)
					gHOGhist_pre[3]++;
				else if (0 <= Gy && Gy < 5)
					gHOGhist_pre[4]++;
				else if (5 <= Gy && Gy < 10)
					gHOGhist_pre[5]++;
				else if (10 <= Gy && Gy < 15)
					gHOGhist_pre[6]++;
				else if (15 <= Gy && Gy < 20)
					gHOGhist_pre[7]++;
				else
					total--;

				// ------------------ LDN 256 bits -----------------------
				int feature = 0, feature2 = 0, center;
				int mean = 0;	//標準差
				int root2 = 128;
				int sum = 0;
				int p[9] = { 0 };

				p[1] = YCbCr_Y[i][j - 1];
				p[2] = YCbCr_Y[i - 1][j - 1];
				p[3] = YCbCr_Y[i - 1][j];
				p[4] = YCbCr_Y[i - 1][j + 1];
				p[5] = YCbCr_Y[i][j + 1];
				p[6] = YCbCr_Y[i + 1][j + 1];
				p[7] = YCbCr_Y[i + 1][j];
				p[8] = YCbCr_Y[i + 1][j - 1];

				center = YCbCr_Y[i][j];

				//-------計算標準差-------
				// mean = ( p [ 1 ] + p [ 2 ] + p [ 3 ] + p [ 4 ] + p [ 5 ] + p [ 6 ] + p [ 7 ] + p [ 8 ] + center ) / 9;

				for (int z = 1; z <= 8; z++) {

					if (p[z] - center >= 0)
						sum += root2;

					root2 /= 2;
				}

				// LDN 直方圖累加


				g_TheBestLDNHistogram1[g_Hash_Transfor_to_TheBestLDNBin[sum]] ++;

				if (sum <= 255 && sum > 239)
					LDNhistogram0++;
				else if (sum <= 239 && sum > 223)
					LDNhistogram1++;
				else if (sum <= 223 && sum > 207)
					LDNhistogram2++;
				else if (sum <= 207 && sum > 191)
					LDNhistogram3++;
				else if (sum <= 191 && sum > 175)
					LDNhistogram4++;
				else if (sum <= 175 && sum > 159)
					LDNhistogram5++;
				else if (sum <= 159 && sum > 143)
					LDNhistogram6++;
				else if (sum <= 143 && sum > 127)
					LDNhistogram7++;
				else if (sum <= 127 && sum > 111)
					LDNhistogram8++;
				else if (sum <= 111 && sum > 95)
					LDNhistogram9++;
				else if (sum <= 95 && sum > 79)
					LDNhistogram10++;
				else if (sum <= 79 && sum > 63)
					LDNhistogram11++;
				else if (sum <= 63 && sum > 47)
					LDNhistogram12++;
				else if (sum <= 47 && sum > 31)
					LDNhistogram13++;
				else if (sum <= 31 && sum >= 0)
					LDNhistogram14++;


				// ------------------ END LDN 結束!! -----------------------

			} // if (trackingDiffFrame_Count[i][j]>0)


		} // for

		cvShowImage("once", once_temp);

		if (total == 0)
			total = 1;

		// 正規劃 HOG 質方突
		for (int index = 0; index < 8; index++)
			gAvgHOGhist_pre[index] = gHOGhist_pre[index] / total;

		ave0 = histogram0 / total;
		ave1 = histogram1 / total;
		ave2 = histogram2 / total;
		ave3 = histogram3 / total;
		ave4 = histogram4 / total;
		ave5 = histogram5 / total;
		ave6 = histogram6 / total;
		ave7 = histogram7 / total;
		ave8 = histogram8 / total;



		LDNave0 = LDNhistogram0 / total;
		LDNave1 = LDNhistogram1 / total;
		LDNave2 = LDNhistogram2 / total;
		LDNave3 = LDNhistogram3 / total;
		LDNave4 = LDNhistogram4 / total;
		LDNave5 = LDNhistogram5 / total;
		LDNave6 = LDNhistogram6 / total;
		LDNave7 = LDNhistogram7 / total;
		LDNave8 = LDNhistogram8 / total;
		LDNave9 = LDNhistogram9 / total;
		LDNave10 = LDNhistogram10 / total;
		LDNave11 = LDNhistogram11 / total;
		LDNave12 = LDNhistogram12 / total;
		LDNave13 = LDNhistogram13 / total;
		LDNave14 = LDNhistogram14 / total;



		disbetweenfingertip = P2_y - P1_y;



		for (int i = P1_x; i < P2_x; i++) //sobel累加 //邊界
		{
			for (int j = P1_y; j < P1_y + (P2_y - P1_y) / 3 && j < height_m; j++) {
				Y[i][j] = abs(YCbCr_Y[i - 1][j - 1] * (-1) + YCbCr_Y[i][j - 1] * (-2) + YCbCr_Y[i + 1][j - 1] * (-1)
					+ YCbCr_Y[i - 1][j + 1] * 1 + YCbCr_Y[i][j + 1] * 2 + YCbCr_Y[i + 1][j + 1] * 1);

				Cb[i][j] = abs(YCbCr_Cb[i - 1][j - 1] * (-1) + YCbCr_Cb[i][j - 1] * (-2) + YCbCr_Cb[i + 1][j - 1] * (-1)
					+ YCbCr_Cb[i - 1][j + 1] * 1 + YCbCr_Cb[i][j + 1] * 2 + YCbCr_Cb[i + 1][j + 1] * 1);

				Cr[i][j] = abs(YCbCr_Cr[i - 1][j - 1] * (-1) + YCbCr_Cr[i][j - 1] * (-2) + YCbCr_Cr[i + 1][j - 1] * (-1)
					+ YCbCr_Cr[i - 1][j + 1] * 1 + YCbCr_Cr[i][j + 1] * 2 + YCbCr_Cr[i + 1][j + 1] * 1);


				SobelFrame[i][j] = Y[i][j] + Cb[i][j] + Cr[i][j];
				if (SobelFrame[i][j] > 50 && YCbCr_Cr[i][j] > 133 && YCbCr_Cr[i][j] < 174 && YCbCr_Cb[i][j] > 77 && YCbCr_Cb[i][j] < 127)


					Sobelcount2++;
				//else
				//SobelFrame[i][j] = 0;
			}
		}
		cout << "Sobelcount2: " << Sobelcount2 << endl;
		once++;  // 特徵只需要抓一次，執行完這個，就不會再進這個迴圈。
	}

	//放大框架掃描追蹤
	skin_total3 = 0;

	handmodelmatchingE = -1; // 初始化 (手型特徵 門檻值)


	int thistogram0 = 0;
	int thistogram1 = 0;
	int thistogram2 = 0;
	int thistogram3 = 0;
	int thistogram4 = 0;
	int thistogram5 = 0;
	int thistogram6 = 0;
	int thistogram7 = 0;
	int	thistogram8 = 0;



	int LDNthistogram0 = 0;
	int LDNthistogram1 = 0;
	int LDNthistogram2 = 0;
	int LDNthistogram3 = 0;
	int LDNthistogram4 = 0;
	int LDNthistogram5 = 0;
	int LDNthistogram6 = 0;
	int LDNthistogram7 = 0;
	int	LDNthistogram8 = 0;
	int LDNthistogram9 = 0;
	int LDNthistogram10 = 0;
	int LDNthistogram11 = 0;
	int LDNthistogram12 = 0;
	int LDNthistogram13 = 0;
	int LDNthistogram14 = 0;


	Threshold = 10.0;

	int DiffFramecount = 0;

	int feature1 = 0; //角點特徵

	// 累計放大區塊裡，候選區塊的總數(放大區塊m*n裡面，可能會有數個候選區塊)
	int countOfFrameCandidate = 0;

	// 更改放大區塊的大小
	int big_roi_distance = (P2_x - P1_x) / 2.5;

	// 放大框架 的 候選點 初始化
	int big_roi_p1_x = P1_x - big_roi_distance; // 放大框架的 P1_x
	int big_roi_p1_y = P1_y - big_roi_distance; // 放大框架的 P1_y
	int big_roi_p2_x = P2_x + big_roi_distance; // 放大框架的 P2_x
	int big_roi_p2_y = P2_y + big_roi_distance; // 放大框架的 P2_y

	int nnn = 10;

	// 顯示 放大框架的範圍
	cvRectangle(frame_320x240,
		cvPoint(big_roi_p1_x, big_roi_p1_y), // P1 點 再向左上放大
		cvPoint(big_roi_p2_x, big_roi_p2_y), // P2 點 再向右下放大
		CV_RGB(0, 200, 200),
		1,
		CV_AA,
		0);

	for (int m = (big_roi_p1_x >= 0) ? big_roi_p1_x : 0;  // 避免放大框架超出原圖範圍
		m < big_roi_p2_x && m < width_m; // 避免 放大框架 超過 frame 的最邊界
		m += 5)
	for (int n = (big_roi_p1_y >= 0) ? big_roi_p1_y : 0;
		n < big_roi_p2_y && n < height_m; // 避免 放大框架 超過 frame 的最邊界
		n += 5) // 每跳3格，加快速度
	{ // 每跳3格，加快速度，由上到下，由左到右

		// 手部輪廓 分數 初始化
		handmodelmatching = 0;

		if (P2_x <= big_roi_p2_x && P2_y <= big_roi_p2_y) { // 限制 ROI 不要超過 放大框架

			// ----------------------- Best LDN Histogram 初始化 -----------------------
			for (int i = 0; i < g_TheBestLDNSize; i++) {

				g_TheBestLDNHistogram2[i] = 0;

			} // for 
			g_TheBestLDNThroshold2 = 0;
			int LDNscore = 0;
			// ----------------------- Best LDN Histogram 初始化 -----------------------

			// ----------------------- HOG 初始化 -----------------------
			candicate = 0;

			for (int index = 0; index < 8; index++)
				gHOGhist_new[index] = 0;
			// ----------------------- HOG 初始化 -----------------------


			skin_total2 = 0;
			// 初始 背景相減
			DiffFramecount = 0;
			feature1 = 0;

			// 手部輪廓 初始化
			for (int index_x = 0; index_x < 4; index_x++)
			for (int index_y = 0; index_y < 4; index_y++)
				handmodel_new[index_x][index_y] = 0;

			topOfTheHandModel = height_m; // 最高 ROI 點 選取 初始化

			histogram0 = 0;
			histogram1 = 0;
			histogram2 = 0;
			histogram3 = 0;
			histogram4 = 0;
			histogram5 = 0;
			histogram6 = 0;
			histogram7 = 0;
			histogram8 = 0;


			LDNhistogram0 = 0;
			LDNhistogram1 = 0;
			LDNhistogram2 = 0;
			LDNhistogram3 = 0;
			LDNhistogram4 = 0;
			LDNhistogram5 = 0;
			LDNhistogram6 = 0;
			LDNhistogram7 = 0;
			LDNhistogram8 = 0;
			LDNhistogram9 = 0;
			LDNhistogram10 = 0;
			LDNhistogram11 = 0;
			LDNhistogram12 = 0;
			LDNhistogram13 = 0;
			LDNhistogram14 = 0;


			int trackingmotioncount = 0;

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

			subcount = 0;
			handmodel_count_new = 0;

			for (int i = m;
				i < m + (P2_x - P1_x) &&
				m + (P2_x - P1_x) < big_roi_p2_x + big_roi_distance && // 避免超過 放大框架的範圍
				i < width_m; // 避免超過 frame 的範圍
			i++)
			for (int j = n;
				j < n + (P2_y - P1_y) &&
				n + (P2_y - P1_y) < big_roi_p2_y + big_roi_distance && // 避免超過 放大框架的範圍
				j < height_m; // 避免超過 frame 的範圍
			j++) {

				// ----------------------時間差相減法---------開跑囉!!-------------------
				int Diff_Value = 0;
				Diff_Value = abs(YCbCr_Y[i][j] - Pre[i][j]); // abs取整數絕對值                Pre [ i ] [ j ] = YCbCr_Y [ i ] [ j ];

				if (Diff_Value > Threshold) {
					DiffFramecount++; //超過T則做累加
				}
				// ----------------------時間差相減法---------結束!!-------------------
				if (trackingDiffFrame_Count[i][j] > 0)
					//if (cvGet2D(trankingcanny, j, i).val[0] > 0)
					//  if ( YCbCr_Cr [ i ] [ j ] > 133 && YCbCr_Cr [ i ] [ j ] < 174 && YCbCr_Cb [ i ] [ j ] > 77 && YCbCr_Cb [ i ] [ j ] < 127 ) 
				{

					trackingmotioncount++; // 計算 motion 的 點數量

					// ======================= 相減法 =========================
					int pre_x = P1_x + (i - m); // 前一張 的 掃描 x 座標
					int pre_y = P1_y + (j - n); // 前一張 的 掃描 y 座標
					int now_x = i; // 目前 的 掃描 x 座標
					int now_y = j; // 目前 的 掃描 y 座標

					if (abs(YCbCr_Y[now_x][now_y] - PreTrack[pre_x][pre_y]) <= 30) { //相減越小越像

						handmodel_count_new++; // 累計 手型特徵總點數

						// ================ 抓出 手 模組 的 輪廓 ================
						int duration_x = ROI_width / 4; // block 寬 的 長度
						int duration_y = ROI_height / 4; // block 高 的 長度

						int block_width = (i - m >= 0 ? i - m : 0) / duration_x; // 找出 block 的 x 軸座標
						int block_height = (j - n >= 0 ? j - n : 0) / duration_y; // 找出 block 的 y 軸座標

						handmodel_new[block_width][block_height] ++; // 累計 特徵點 在 指定 位置 的 特徵點 數量

						if (block_width >= 4 || block_height >= 4) // 針對 邊界問題 選擇 不計算
							handmodel_count_new--;

						// 存下 候選塊塊 的 指尖點 位置
						if (j < topOfTheHandModel) {
							topOfTheHandModel = j; // 更新 最高點位置
							topOfTheHandModel_new_x = i;
							topOfTheHandModel_new_y = j;
						} // if
						// ======================================================

						// CV_IMAGE_ELEM(SUBFrame, uchar, j, i) = 255;

						subcount++;

					} // if
					else
						; //  CV_IMAGE_ELEM(SUBFrame, uchar, j, i) = 0;
					// ========================================================

					skin_total2++;
					int Gx = 0, Gy = 0, Gxy = 0;
					if (i != 0 && i != width_m && j != 0 && j != height_m)
						//Gx = YCbCr_Y[i + 1][j] - YCbCr_Y[i - 1][j];
						Gy = YCbCr_Y[i][j + 1] - YCbCr_Y[i][j - 1];	 //上下相減

					//cout<<orientation<<endl;
					//cout<<Gx<<" "<<Gy<<endl;

					if (-40 <= Gy && Gy < -30)     //將梯度歸納到八個方向中
						gHOGhist_new[0]++;
					else if (-30 <= Gy && Gy < -20)
						gHOGhist_new[1]++;
					else if (-20 <= Gy && Gy < -10)
						gHOGhist_new[2]++;
					else if (-10 <= Gy && Gy < 0)
						gHOGhist_new[3]++;
					else if (0 <= Gy && Gy < 5)
						gHOGhist_new[4]++;
					else if (5 <= Gy && Gy < 10)
						gHOGhist_new[5]++;
					else if (10 <= Gy && Gy < 15)
						gHOGhist_new[6]++;
					else if (15 <= Gy && Gy < 20)
						gHOGhist_new[7]++;
					else
						skin_total2--;

					// ------------------ LDN 256 bits -----------------------

					int feature = 0, feature2 = 0, center;
					int mean = 0;
					int root2 = 128;
					int sum = 0;
					int p[9] = { 0 };
					//getPixel(width,height)

					p[1] = YCbCr_Y[i][j - 1];
					p[2] = YCbCr_Y[i - 1][j - 1];
					p[3] = YCbCr_Y[i - 1][j];
					p[4] = YCbCr_Y[i - 1][j + 1];
					p[5] = YCbCr_Y[i][j + 1];
					p[6] = YCbCr_Y[i + 1][j + 1];
					p[7] = YCbCr_Y[i + 1][j];
					p[8] = YCbCr_Y[i + 1][j - 1];

					//center = CV_IMAGE_ELEM(trankingcanny, uchar, i, j);
					center = YCbCr_Y[i][j];


					//-------計算標準差-------
					mean = (p[1] + p[2] + p[3] + p[4] + p[5] + p[6] + p[7] + p[8] + center) / 9;

					for (int z = 1; z <= 8; z++) {

						if (p[z] - center >= 0)
							sum += root2;

						root2 /= 2;
					}

					// LDN 直方圖累加!!

					g_TheBestLDNHistogram2[g_Hash_Transfor_to_TheBestLDNBin[sum]] ++;

					if (sum <= 255 && sum > 239)
						LDNhistogram0++;
					else if (sum <= 239 && sum > 223)
						LDNhistogram1++;
					else if (sum <= 223 && sum > 207)
						LDNhistogram2++;
					else if (sum <= 207 && sum > 191)
						LDNhistogram3++;
					else if (sum <= 191 && sum > 175)
						LDNhistogram4++;
					else if (sum <= 175 && sum > 159)
						LDNhistogram5++;
					else if (sum <= 159 && sum > 143)
						LDNhistogram6++;
					else if (sum <= 143 && sum > 127)
						LDNhistogram7++;
					else if (sum <= 127 && sum > 111)
						LDNhistogram8++;
					else if (sum <= 111 && sum > 95)
						LDNhistogram9++;
					else if (sum <= 95 && sum > 79)
						LDNhistogram10++;
					else if (sum <= 79 && sum > 63)
						LDNhistogram11++;
					else if (sum <= 63 && sum > 47)
						LDNhistogram12++;
					else if (sum <= 47 && sum > 31)
						LDNhistogram13++;
					else if (sum <= 31 && sum >= 0)
						LDNhistogram14++;


				} // if (cvGet2D(trankingcanny, j, i).val[0] > 0)


			} // for 

			// 對 HOG 正規化!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			if (skin_total2 == 0) skin_total2 = 1;//防止分母為0

			for (int index = 0; index < 8; index++)
				gAvgHOGhist_new[index] = gHOGhist_new[index] / skin_total2;

			ave_0 = histogram0 / skin_total2;
			ave_1 = histogram1 / skin_total2;
			ave_2 = histogram2 / skin_total2;
			ave_3 = histogram3 / skin_total2;
			ave_4 = histogram4 / skin_total2;
			ave_5 = histogram5 / skin_total2;
			ave_6 = histogram6 / skin_total2;
			ave_7 = histogram7 / skin_total2;
			ave_8 = histogram8 / skin_total2;

			LDNave_0 = LDNhistogram0 / skin_total2;
			LDNave_1 = LDNhistogram1 / skin_total2;
			LDNave_2 = LDNhistogram2 / skin_total2;
			LDNave_3 = LDNhistogram3 / skin_total2;
			LDNave_4 = LDNhistogram4 / skin_total2;
			LDNave_5 = LDNhistogram5 / skin_total2;
			LDNave_6 = LDNhistogram6 / skin_total2;
			LDNave_7 = LDNhistogram7 / skin_total2;

			LDNave_8 = LDNhistogram8 / skin_total2;
			LDNave_9 = LDNhistogram9 / skin_total2;
			LDNave_10 = LDNhistogram10 / skin_total2;
			LDNave_11 = LDNhistogram11 / skin_total2;
			LDNave_12 = LDNhistogram12 / skin_total2;
			LDNave_13 = LDNhistogram13 / skin_total2;
			LDNave_14 = LDNhistogram14 / skin_total2;

			// END 正規化結束!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!




			//disbetweenfingertip2 = P2_y - P1_y;
			//候選者 //越小越趨近於當選者
			//ROI_shape = (P2_y - P1_y) / (P2_x - P1_x);




			// LDN + HOG
			/*candicate = abs(ave0 - ave_0) + abs(ave1 - ave_1) + abs(ave2 - ave_2) + abs(ave3 - ave_3) + abs(ave4 - ave_4) + abs(ave5 - ave_5) + abs(ave6 - ave_6) + abs(ave7 - ave_7) + abs(ave8 - ave_8) +
			abs(hist[1] - hist2[1]) + abs(hist[2] - hist2[2]) + abs(hist[3] - hist2[3]) + abs(hist[4] - hist2[4]) + abs(hist[5] - hist2[5]) + abs(hist[6] - hist2[6]) + abs(hist[7] - hist2[7]) + abs(hist[8] - hist2[8]);
			*/

			// LDN
			//candicateLDN = abs(hist[1] - hist2[1]) + abs(hist[2] - hist2[2]) + abs(hist[3] - hist2[3]) + abs(hist[4] - hist2[4]) + abs(hist[5] - hist2[5]) + abs(hist[6] - hist2[6]) + abs(hist[7] - hist2[7]) + abs(hist[8] - hist2[8]);




			candicateLDN = 0;

			g_TheBestLDNThroshold2 = 0;

			if (skin_total2 == 0){

				skin_total2 = 1;
			}

			for (int i = 0; i < g_TheBestLDNSize; i++) {

				if (g_TheBestLDNHistogram2[i] - g_TheBestLDNHistogram1[i] >= 0){
					g_TheBestLDNThroshold2 += g_TheBestLDNHistogram2[i] - g_TheBestLDNHistogram1[i];
					//cout << "我們來out一下:" << g_TheBestLDNHistogram2[i] << "("<<i<<")"<<endl;
				}
				else
					g_TheBestLDNThroshold2 += g_TheBestLDNHistogram1[i] - g_TheBestLDNHistogram2[i];

			} // for

			for (int i = 1; i < g_TheBestLDNSize; i++) {

				if (abs(g_TheBestLDNHistogram2[i] / skin_total2 - g_TheBestLDNHistogram1[i] / total) < 0.05){

					LDNscore++;
				}

			} // for


			g_TheBestLDNThroshold2 /= skin_total2;



			candicateLDN = abs(LDNave0 - LDNave_0) + abs(LDNave1 - LDNave_1) + abs(LDNave2 - LDNave_2) + abs(LDNave3 - LDNave_3) + abs(LDNave4 - LDNave_4) + abs(LDNave5 - LDNave_5) + abs(LDNave6 - LDNave_6) + abs(LDNave7 - LDNave_7) + abs(LDNave8 - LDNave_8) + abs(LDNave9 - LDNave_9) + abs(LDNave10 - LDNave_10) + abs(LDNave11 - LDNave_11) + abs(LDNave12 - LDNave_12) + abs(LDNave13 - LDNave_13) + abs(LDNave14 - LDNave_14);

			//candicate = abs(ave0 - ave_0) + abs(ave1 - ave_1) + abs(ave2 - ave_2) + abs(ave3 - ave_3) + abs(ave4 - ave_4) + abs(ave5 - ave_5) + abs(ave6 - ave_6) + abs(ave7 - ave_7) + abs(ave8 - ave_8);


			// HOG
			// candicate = abs(ave0 - ave_0) + abs(ave1 - ave_1) + abs(ave2 - ave_2) + abs(ave3 - ave_3) + abs(ave4 - ave_4) + abs(ave5 - ave_5) + abs(ave6 - ave_6) + abs(ave7 - ave_7) + abs(ave8 - ave_8);

			// 針對 HOG 進行 比對記分
			for (int index = 0; index < 8; index++) {

				//candicate += abs(gAvgHOGhist_pre[index] - gAvgHOGhist_new[index]) < 0.2 ? 1 : 0; // 0.2 平均最佳門檻
				candicate += abs(gAvgHOGhist_pre[index] - gAvgHOGhist_new[index]) < 0.15 ? 1 : 0; // 0.08 平均最佳門檻

				//cout << "index" << index << ":" << abs(gAvgHOGhist_pre[index] - gAvgHOGhist_new[index]) << endl;
				all_HOG_count++;
				all_HOG += abs(gAvgHOGhist_pre[index] - gAvgHOGhist_new[index]);
			} // for index


			int duration_x = ROI_width / 4; // block 寬 的 長度
			int duration_y = ROI_height / 4; // block 高 的 長度

			/*cout << "handmodel_count_pre:" << handmodel_count_pre << endl;
			cout << "handmodel_count_new:" << handmodel_count_new << endl;*/

			// 避免 手型辨識 正規化 發生 除以0
			if (handmodel_count_pre <= 0) handmodel_count_pre = 1;
			if (handmodel_count_new <= 0) handmodel_count_new = 1;

			// 針對 手部輪廓 進行 比對分數
			for (int index_y = 0; index_y < 4; index_y++) {
				for (int index_x = 0; index_x < 4; index_x++) {

					int handmodel_pre_percent = (int)100 * ((float)handmodel_pre[index_x][index_y] / handmodel_count_pre);
					int handmodel_new_percent = (int)100 * ((float)handmodel_new[index_x][index_y] / handmodel_count_new);
					if (handmodel_pre_percent > 1 && handmodel_new_percent > 1) // 小於 1 % 為雜訊
						handmodelmatching++;
					else
						/*handmodelmatching--*/;

					int draw_handmodel_width = m + index_x * duration_x + (duration_x / 2);
					int draw_handmodel_height = n + index_y * duration_y + (duration_y / 2);

				} // for index_y
			} // for index_x

			bool matching_candicate = false; // 判斷 候選區塊是否 符合門檻值

			// **************************************************** 比對後選區塊 **************************************************************************************************************************
			if (((handmodelmatching > 6) && (handmodelmatching >= handmodelmatchingE))  // 辨識 手型
				&& (topOfTheHandModel - 3 <= topOfTheHandModelE)// 辨識 最高點 
				// && LDNscore >= 15
				)
				//if (handmodelmatching >= handmodelmatchingE && // 辨識 手型
				//	topOfTheHandModel - 3 <= topOfTheHandModelE && // 辨識 最高點
				//	/*candicate >= 8 &&
				//	candicateLDN <= 0.5 &&
				//	g_TheBestLDNThroshold2 <= 2 &&
				//	g_TheBestLDNHistogram2[0] <= 200 &&*/
				//	LDNscore >= 0
				//	)
			{
				//if (candicate * weight >= 8 && /*handmodelmatching >= handmodelmatchingE && */
				//	topOfTheHandModel - 3 < topOfTheHandModelE&& trackingmotioncount >= 20 &&
				//	candicateLDN * weight <= 0.5 && // 
				//	g_TheBestLDNThroshold2 <= 2 && // 實驗顯示，手部特徵，Best LDN Histogram 的差距會在 500 之內
				//	g_TheBestLDNHistogram2[0] <= 200 && LDNscore >= 15)

				matching_candicate = true;

				morris = (float)(subcount / (trackingmotioncount));

			} // if

			if (matching_candicate) { // 候選區塊 符合門檻

				// ================================== 各種 重要資訊更新 ===========================================

				skin_total3 = skin_total2;
				eligibility = candicate;
				eligibilityLDN = candicateLDN;

				Px = m;
				Py = n;

				//isGetTrackingROI = true;

				int temp_P2x = Px + ROI_width < width_m ? Px + ROI_width : width_m;
				int temp_P2y = Py + ROI_height < height_m ? Py + ROI_height : height_m;

				g_TheBestLDNThroshold1 = g_TheBestLDNThroshold2; // LDN test

				thistogram0 = histogram0;
				thistogram1 = histogram1;
				thistogram2 = histogram2;
				thistogram3 = histogram3;
				thistogram4 = histogram4;
				thistogram5 = histogram5;
				thistogram6 = histogram6;
				thistogram7 = histogram7;
				thistogram8 = histogram8;


				LDNthistogram0 = LDNhistogram0;
				LDNthistogram1 = LDNhistogram1;
				LDNthistogram2 = LDNhistogram2;
				LDNthistogram3 = LDNhistogram3;
				LDNthistogram4 = LDNhistogram4;
				LDNthistogram5 = LDNhistogram5;
				LDNthistogram6 = LDNhistogram6;
				LDNthistogram7 = LDNhistogram7;
				LDNthistogram8 = LDNhistogram8;
				LDNthistogram9 = LDNhistogram9;
				LDNthistogram10 = LDNhistogram10;
				LDNthistogram11 = LDNhistogram11;
				LDNthistogram12 = LDNhistogram12;
				LDNthistogram13 = LDNhistogram13;
				LDNthistogram14 = LDNhistogram14;

				// 針對 手部輪廓 進行 濃縮(決定最佳的)
				handmodelmatchingE = handmodelmatching;

				// 針對 候選 ROI 盡量選取 最高點
				topOfTheHandModelE = topOfTheHandModel;

				// 找到第 n 個，符合ROI特徵值的候選區塊，候選區塊的數量又多累計一個。
				countOfFrameCandidate++;

				// 暫存下 目前 最新最佳 的 手模特徵
				for (int index_update_x = 0; index_update_x < 4; index_update_x++)
				for (int index_update_y = 0; index_update_y < 4; index_update_y++)
					handmodel_update[index_update_x][index_update_y] = handmodel_new[index_update_x][index_update_y];

				// 暫存下 目前 最新最佳 的 最高尖點
				temp_topOfTheHandModel_update_x = topOfTheHandModel_new_x;
				temp_topOfTheHandModel_update_y = topOfTheHandModel_new_y;

				thumbpoint = 0;
				keeplastpoints(topOfTheHandModel_new_x, topOfTheHandModel_new_y, thumbpoint);

				// ======================================================================================================

				//// ================================== 顯示 追蹤後選者 ====================================================
				//string ss = "now: " + to_string(temp_candi_count++);
				//IplImage * temp_candi = cvCloneImage(Tracking_Test);
				//cout << ss << ":" << endl; // 顯示
				////cvRectangle(temp_candi, cvPoint(Px, Py), cvPoint(temp_P2x, temp_P2y), cvScalar(0, 255, 0), 2, 8, 0 );
				////cvShowImage(ss.c_str(), temp_candi);
				//cout << "  handmodelmatching:" << handmodelmatching << endl;

				//// 針對 手部輪廓 進行 比對分數

				//if (is_handmodel_pre_percent) {
				//	cout << "  handmodel_pre_percent:" << handmodel_count_pre << endl;
				//	for (int index_x = 0; index_x < 4; index_x++) {
				//		cout << "  ";
				//		for (int index_y = 0; index_y < 4; index_y++) {

				//			int handmodel_pre_percent = (int)100 * ((float)handmodel_pre[index_x][index_y] / handmodel_count_pre);

				//			//cout << left << setw(3) << handmodel_pre_percent;
				//			cout << handmodel_pre_percent << " ";

				//		} // for index_y
				//		cout << endl;
				//	} // for index_x
				//	is_handmodel_pre_percent = false;
				//} // if

				//cout << "  handmodel_new_percent:" << endl;
				//for (int index_y = 0; index_y < 4; index_y++) {
				//	cout << "  ";
				//	for (int index_x = 0; index_x < 4; index_x++) {

				//		int handmodel_new_percent = (int)100 * ((float)handmodel_new[index_x][index_y] / handmodel_count_new);

				//		cout << left << setw(3) << handmodel_new_percent;

				//	} // for index_y
				//	cout << endl;
				//} // for index_x
				//// ==================================================================================================================

				//// ---------------------------- 將 Best LDN 結果 寫成 BestLDN.txt 的文件 ----------------------------
				//for (int iii = 0; iii < g_TheBestLDNSize; iii++) {

				//	if (iii % 5 == 0 && iii != 0)
				//		infile << endl;

				//	if (iii == 0) {
				//		infile << "Bin[" << setw(3) << 0 << "](" << setw(3) << -1 << "): ";
				//		infile << setw(3) << g_TheBestLDNHistogram1[0] << " ";
				//	} // if

				//	else {
				//		infile << "Bin[" << setw(3) << iii << "](" << setw(3) << g_TheBestLDNBins[iii - 1] << "): ";
				//		infile << setw(3) << g_TheBestLDNHistogram1[iii] << " ";
				//	} // else

				//} // for
				//infile << endl;
				//infile << "--------------========------------------------" << endl;
				//infile << "m:" << m << "    n:" << n << endl;
				//infile << "g_TheBestLDNThroshold1:" << g_TheBestLDNThroshold1 << endl;
				//infile << "g_TheBestLDNThroshold2:" << g_TheBestLDNThroshold2 << endl;
				//infile << "countOfFrameCandidate:" << countOfFrameCandidate << endl;
				//infile << "candicate:" << candicate << endl;
				//infile << "DiffFramecount:" << DiffFramecount << endl;
				//infile << "candicateLDN:" << candicateLDN << endl;
				//infile << "trackingmotioncount:" << trackingmotioncount << endl;
				//infile << "LDNscore:" << LDNscore << endl;
				//infile << "handmodelmatching:" << handmodelmatching << endl;
				//infile << "topOfTheHandModel:" << topOfTheHandModel << endl;
				//infile << "--------------========------------------------" << endl;
				//// ---------------------------- 將 Best LDN 結果 寫成 BestLDN.txt 的文件 ----------------------------

			} // if (matching_candicate) {

		} // if

	} // for m*n

	// cout << "all_HOG:" << all_HOG / all_HOG_count << endl;

	// cvWaitKey(0); /// =================== 顯示 追蹤後選者 ====================

	// =================== 2016/01/19 最新最強設定ROI 點座標 ===================
	if (isGetTrackingROI) { // 每當 ROI 有抓取到，才進行 ROI 更新

		int di_ROIs = sqrt((P1.x - Px)*(P1.x - Px) + (P1.y - Py)*(P1.y - Py));

		cout << "dis:" << di_ROIs << endl;

		// ================== 追蹤有 快追蹤 與 慢追蹤 ========================
		if (di_ROIs <= 10) { // 我是慢追蹤 (ROI 位移量不大的情況)
			
			Px = (Px + P1.x) / 2;
			Py = (Py + P1.y) / 2;
			/*P2.x = Px + (P2_x - P1_x);
			P2.y = Py + (P2_y - P1_y);*/

		} // if

		else { // 我是快追蹤 (ROI 位移量大的情況)

			/*Px = Px;
			Py = Py;
			P2.x = Px + (P2_x - P1_x);
			P2.y = Py + (P2_y - P1_y);*/

		} // else

		P1.x = Px;
		P1.y = Py;
		P2.x = Px + ROI_width < width_m ? Px + ROI_width : width_m;
		P2.y = Py + ROI_height < height_m ? Py + ROI_height : height_m;

		P1_x = P1.x;
		P1_y = P1.y;
		P2_x = P2.x;
		P2_y = P2.y;
		// ==================================================================

		// ================= 我終於找到最佳 ROI，現在要更新!!!爽!! =================
		// HOG 前一個 取代 後一個
		for (int index = 0; index < 8; index++)
			gAvgHOGhist_pre[index] = gAvgHOGhist_new[index];
		// =========================================================================

		// ================= 我終於找到最佳 手部輪廓 ，現在要更新!!!爽!! =================
		for (int index_x = 0; index_x < 4; index_x++) {
			for (int index_y = 0; index_y < 4; index_y++) {
				handmodel_pre[index_x][index_y] = handmodel_update[index_x][index_y];
			} // for index_y
		} // for index_x

		handmodel_count_pre = handmodel_count_new; // 總點數量
		// ================================================================================

		// ====================== 手部輪廓 最高點 的 x, y 位置(4x4) =======================
		topOfTheHandModel_pre_x = topOfTheHandModel_update_x; // 更新 手部追蹤的ROI 最高點 的 x 座標 (handmodel_pre)
		topOfTheHandModel_pre_y = topOfTheHandModel_update_y; // 更新 手部追蹤的ROI 最高點 的 y 座標 (handmodel_pre)
		// ================================================================================

		cvRectangle(frame_320x240, P1, P2, cvScalar(0, 0, 0), 3, 8, 0);
		// ========================== 舊版設定ROI 點座標 ===========================

		// ================== 筆畫有 快筆畫 與 慢筆畫 ==================
		int dis_draw_point = sqrt((PreP1x - temp_topOfTheHandModel_update_x)*(PreP1x - temp_topOfTheHandModel_update_x) + (PreP1y - temp_topOfTheHandModel_update_y)*(PreP1y - temp_topOfTheHandModel_update_y));

		if (!(PreP1x == -1 && PreP1y == -1) && // 不是第一次 追蹤
			dis_draw_point <= 10) { // 前後 筆畫距離 小於 10 為 慢筆畫
			temp_topOfTheHandModel_update_x = (temp_topOfTheHandModel_update_x + PreP1x) / 2;
			temp_topOfTheHandModel_update_y = (temp_topOfTheHandModel_update_y + PreP1y) / 2;
		} // if

		PreP1x = temp_topOfTheHandModel_update_x;
		PreP1y = temp_topOfTheHandModel_update_y;

		// =============================================================

	} // if (isGetTrackingROI)
	// ==================================================================================

	//if ( P2_y - P1_y > P2_x - P1_x ) {

	//    P2_x = Px + ( P2_x - P1_x );
	//    P2_y = Py + ( P2_y - P1_y );
	//    P1_x = Px;
	//    P1_y = Py;

	//} // if
	// =========================================================================

	// ---------------------------- 將 Best LDN 結果 寫成 BestLDN.txt 的文件 ----------------------------
	// cout << countTheZeroCandidateFrame << "***************候選區塊數量*********(" << countOfFrameCandidate << endl;
	infile << countTheZeroCandidateFrame << "***************候選區塊數量*********(" << countOfFrameCandidate << endl;
	// ---------------------------- 將 Best LDN 結果 寫成 BestLDN.txt 的文件 ----------------------------

	// 不斷的候選區塊為 0 (找不到候選區塊)，將被判定為「卡無窮迴圈」
	if (countOfFrameCandidate == 0) {

		countTheZeroCandidateFrame++; // 預計不能連續超過 20 次的「找不到候選區塊」

	} // if

	else {

		countTheZeroCandidateFrame = 0;

	} // else





	////cout<<"click ";
	////防止握拳ROI下移

	// ========================== 舊版設定ROI 點座標 ===========================
	/*P2_x = PreP1x + ( P2_x - P1_x );
	P2_y = PreP1y + ( P2_y - P1_y );
	P1_x = PreP1x;
	P1_y = PreP1y;*/
	// =========================================================================

	//// ==================== 掃描 是否出現大拇哥 ==========================
	//bool isFindThunb = false; // 是否發現大拇哥
	//int connect_area = 8; // 設定 連續區塊 必須連續 5 個相連的 pixel 才被視為拇指出現
	//int thumb_location_x = P1_x - (ROI_width / 3); // 大拇指的位置 預計在 左邊 三分之一ROI寬 的 位置
	//int max_tttt = 0;
	//for (int i = P1_y; i < P2_y - connect_area; i++) {

	//	int thumb_pixel_count = 0;

	//	for (int j = i; j < i + connect_area; j++)
	//	if (trackingDiffFrame_Count[thumb_location_x][j] > 0)
	//		thumb_pixel_count++;

	//	if (max_tttt < thumb_pixel_count)
	//		max_tttt = thumb_pixel_count;

	//	if (thumb_pixel_count == connect_area) {
	//		isFindThunb = true; // 發現 大拇哥
	//		i = P2_y - connect_area; // 強制離開迴圈
	//	} // if

	//} // for i 

	//if (isFindThunb) {
	//	
	//	int thumb_hand[4][4] =
	//	{
	//		{ 1, 1, 0, 0 },
	//		{ 1, 1, 0, 0 },
	//		{ 1, 1, 1, 1 },
	//		{ 1, 1, 1, 1 }
	//	};

	//	int threshold = 0;

	//	for (int index_y = 0; index_y < 4; index_y++) {
	//		cout << "  ";
	//		for (int index_x = 0; index_x < 4; index_x++) {

	//			cout << handmodel_pre[index_x][index_y] << " ";

	//			if (handmodel_pre[index_x][index_y] && thumb_hand[index_x][index_y])
	//				threshold++;

	//		} // for index_x
	//		cout << endl;
	//	} // for index_y

	//	if (threshold < 12)
	//		isFindThunb = false; 

	//} // if

	//// cout << "            max count:" << max_tttt << endl;

	//if (isFindThunb) { // 出現大拇指，做滑鼠 click
	//	/*mouse_event(2, 0, 0, 0, 0);
	//	mouse_event(4, 0, 0, 0, 0);*/

	//	P1.x = temp_px;
	//	P1.y = temp_py;
	//	/*P2.x = Px + (P2_x - P1_x);
	//	P2.y = Py + (P2_y - P1_y);*/
	//	P2.x = Px + ROI_width < width_m ? Px + ROI_width : width_m;
	//	P2.y = Py + ROI_height < height_m ? Py + ROI_height : height_m;

	//	P1_x = P1.x;
	//	P1_y = P1.y;
	//	P2_x = P2.x;
	//	P2_y = P2.y;
	//} // if
	//// ===================================================================

	// ==================== 掃描 是否出現 大拇哥 ==========================
	bool isFindClick = false; // 是否發現 大拇哥

	if (isGetTrackingROI) { /// 出現新的候選ROI 才進行 虛實筆 辨識
		
		int threshold = 0;

		int scan_line = temp_topOfTheHandModel_update_x + (ROI_width * 3) / 5; // 選擇 ROI 三分之一 的位置 作為掃描手指的切線

		scan_line = scan_line >= 0 ? scan_line : 0;
		scan_line = scan_line < width_m ? scan_line : width_m - 1;

		for (int row = P1_y; row < P1_y + ROI_height - 3 && row < height_m && row >= 0; row++) {
			if (trackingDiffFrame_Count[scan_line][row] > 0 &&
				trackingDiffFrame_Count[scan_line][row - 1] > 0 &&
				trackingDiffFrame_Count[scan_line][row - 2] > 0 &&
				trackingDiffFrame_Count[scan_line][row - 3] == 0 ) {
				threshold++;
			} // if
			CV_IMAGE_ELEM(temp_test_tracking, byte, row - 3, scan_line) = 255; // 顯示切的位置
		} // for

		cout << "--------------------threshold:" << threshold << endl;

		if (threshold == 2 && ( 1 ))
			isFindClick = true;

	} // if

	if (ClickBufferCount <= 14) // 設定 虛實筆 緩衝時間, 每 3 個刷新
		ClickBufferCount++;

	if (isFindClick && ClickBufferCount >= 14 
		&& abs(P1_x - temp_topOfTheHandModel_update_x) > 5
		&& abs(P2_x - temp_topOfTheHandModel_update_x) > 5) { // 出現大拇指，做 虛實筆
		ClickBufferCount = 0;

		cout << "click!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;

		mouse_event(2, 0, 0, 0, 0);
		mouse_event(4, 0, 0, 0, 0);
	} // if
	// ===================================================================

	Sobelcount = 0;
	for (int i = P1_x; i < P2_x; i++) {
		for (int j = P1_y; j < P1_y + (P2_y - P1_y) / 3 && j < height_m; j++) {
			Y[i][j] = abs(YCbCr_Y[i - 1][j - 1] * (-1) + YCbCr_Y[i][j - 1] * (-2) + YCbCr_Y[i + 1][j - 1] * (-1)
				+ YCbCr_Y[i - 1][j + 1] * 1 + YCbCr_Y[i][j + 1] * 2 + YCbCr_Y[i + 1][j + 1] * 1);

			Cb[i][j] = abs(YCbCr_Cb[i - 1][j - 1] * (-1) + YCbCr_Cb[i][j - 1] * (-2) + YCbCr_Cb[i + 1][j - 1] * (-1)
				+ YCbCr_Cb[i - 1][j + 1] * 1 + YCbCr_Cb[i][j + 1] * 2 + YCbCr_Cb[i + 1][j + 1] * 1);

			Cr[i][j] = abs(YCbCr_Cr[i - 1][j - 1] * (-1) + YCbCr_Cr[i][j - 1] * (-2) + YCbCr_Cr[i + 1][j - 1] * (-1)
				+ YCbCr_Cr[i - 1][j + 1] * 1 + YCbCr_Cr[i][j + 1] * 2 + YCbCr_Cr[i + 1][j + 1] * 1);

			SobelFrame[i][j] = Y[i][j] + Cb[i][j] + Cr[i][j];
			if (SobelFrame[i][j] > 30 && YCbCr_Cr[i][j] > 133 && YCbCr_Cr[i][j] < 174 && YCbCr_Cb[i][j] > 77 && YCbCr_Cb[i][j] < 127)

				Sobelcount++;

			//else
			//SobelFrame[i][j] = 0;



		}

	} //for ( int i = P1_x; i < P2_x; i++ )
	
	if (abs(temp_topOfTheHandModel_update_x - PreP1x) < 3 && abs(temp_topOfTheHandModel_update_y - PreP1y) < 3 && Sobelcount - Sobelcount2>25) {
		
		click_count++;

	} // if

	/*cout << "clickcount:" << click_count << endl;

	cout << "sobelcount:" << Sobelcount << endl;*/

	// 進入辨識階段
	if (click_count > 10) {

		Sobelcount2 = 0;
		click_count = 0;

		
		
		//flag = 2;
		//flag = 3;
		//click_flag = 1;

		for ( int i = 0; i<P2_x - P1_x + 8; i++ )        //儲存ROI內的像素值->fistYCbCr[][]
		    for ( int j = 0; j<P2_y - P1_y; j++ ) {
		        fistY [ P1_x + i ] [ P1_y + j ] = YCbCr_Y [ P1_x + i ] [ P1_y + j ];
		        fistCb [ P1_x + i ] [ P1_y + j ] = YCbCr_Cb [ P1_x + i ] [ P1_y + j ];
		        fistCr [ P1_x + i ] [ P1_y + j ] = YCbCr_Cr [ P1_x + i ] [ P1_y + j ];
		        fist_x = P1_x;
		        fist_y = P1_y;
		    }
		//wait = 10;
		
		recognition_button();
	}

	if (wait<0)
		wait = 0;
	//if (trackrange<8000 && trackrange>3000)
	//新舊P1相差距離
	// ========================== 舊版設定ROI 點座標 ===========================
	/*if ( abs( Px - PreP1x ) > 0 || abs( Py - PreP1y ) > 0 ) {

	P1 = cvPoint( P1_x, P1_y );
	P2 = cvPoint( P2_x, P2_y );
	}*/
	// =========================================================================

}

void recognition_button() {

	for (int i = 0; i<P2_x - P1_x + 8; i++)      	//fist_x = P1_x;
		//fist_y = P1_y;
	{
		for (int j = 0; j<P2_y - P1_y; j++)      //ROI內像素值  後減前  fistYCbCr - YCbCr = fist2YCbCr
		{
			fist2Y[fist_x + i][fist_y + j] = fistY[fist_x + i][fist_y + j] - PreTrack[fist_x + i][fist_y + j];
			if (fist2Y[fist_x + i][fist_y + j]>1 || fist2Cb[fist_x + i][fist_y + j]>1 || fist2Cr[fist_x + i][fist_y + j]>1)  //二值化
				fist2Y[fist_x + i][fist_y + j] = 1;
			else
				fist2Y[fist_x + i][fist_y + j] = 0;
		}
	}

	for (int i = 0; i<frame_320x240_size.width; i++)                           //侵蝕
	for (int j = 0; j<frame_320x240_size.height; j++)
		fist2Y_temp[i][j] = Erosion(fist2Y, i, j);

	int thumb = 0;
	int a[60] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	for (int j = 0; j<(P2_y - P1_y) / 3; j++)      //只掃上半三分之一
	{
		for (int i = 0; i<P2_x - P1_x; i++) {
			if (fist2Y_temp[fist_x + i][fist_y + j] == 1 && fist2Y_temp[fist_x + i + 1][fist_y + j] == 1 && fist2Y_temp[fist_x + i - 1][fist_y + j] == 0)  // 1-1-0
			{
				a[i] = 1;
				a[i + 1] = 1;
			}
		}
	}

	int finger_count = 0;
	for (int l = 0; l<60; l++) {
		//cout<<a[l];
		if (a[l] == 1 && a[l + 1] == 0)   // 1-0
			finger_count++;
	}

	if (finger_count == 0) {
		cout << "0 - stone" << endl;
		/*	wait = 10;
		flag = 3;
		click_flag = 1;*/
	}
	else if (finger_count == 2){
		cout << "two!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << 2 << endl;

		mouse_event(2, 0, 0, 0, 0);
		mouse_event(4, 0, 0, 0, 0);
	}

	else if (0)
		cout << "you got nothing" << endl;

}
void keeplastpoints(void){
	int point = 10;
	int pointtemp = 0;
	int startpoint = 2;
	int countfeature = 0;
	bool thumb = false;
	for (int i = startpoint; i < g_Point.size(); i++)
	{
		int p1x = 0;
		int p1y = 0;
		int p2x = 0;
		int p2y = 0;


		if (i < point){
			pointtemp = i - (point - 1);
		}
		else{
			pointtemp = point;
		}

		for (int j = 0; j < pointtemp - startpoint; j++)
		{
			if (abs(g_Point[i - ((pointtemp - startpoint) - j)].x - p1x) < 2 && abs(g_Point[i - ((pointtemp - startpoint) - j)].y - p1y) < 2)
				countfeature++;
			else
				countfeature--;

			
		}

		if (countfeature > 8)
			thumb = true;


	}//for (int i = startpoint; i < g_Point.size(); i++)
}