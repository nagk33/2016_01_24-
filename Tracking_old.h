#pragma once
#include"struct.h"
#include"Process.h"
#include "CCL.h"

void histogram()
{

	// 目前可以讓使用者使用500個點畫線
	// 當「畫筆的點數以用盡」後，再容許50個點的緩衝時間
	// 定位失敗，否則強制重新定位
	if (g_Point.size() + 50 >= 500 || // 畫筆點數用盡
		countTheZeroCandidateFrame >= 20) { // 卡無窮迴圈
	//if (0){
		// flag = 1; //回去抓ROI
		// cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
		if (g_Point.size() + 50 >= 500 )
			cout << "畫筆的點數以用盡，強制回重新定位!!" << endl;

		else if (countTheZeroCandidateFrame >= 20)
			cout << "卡無窮迴圈，強制回重新定位!!" << endl;
		
		countTheZeroCandidateFrame = 0; // 初始化

        g_Point.clear();
		flag = 1; // 強制回重定位

	} // if


	if (skin_total3 == 0) //防止分母等於0的情況
	{
		skin_total3 = 1;
	}

	if (skin_total != 0)
	if (abs(skin_total / skin_total3) > 10)
	{
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
	double ave_0 = 0, ave_1 = 0, ave_2 = 0, ave_3 = 0, ave_4 = 0, ave_5 = 0, ave_6 = 0, ave_7 = 0, ave_8 = 0 ;
	double LBPave_0 = 0, LBPave_1 = 0, LBPave_2 = 0, LBPave_3 = 0, LBPave_4 = 0, LBPave_5 = 0, LBPave_6 = 0, LBPave_7 = 0, LBPave_8 = 0, LBPave_9 = 0, LBPave_10 = 0, LBPave_11 = 0, LBPave_12 = 0, LBPave_13 = 0, LBPave_14 = 0;
	double gave_0 = 0, gave_1 = 0, gave_2 = 0, gave_3 = 0, gave_4 = 0, gave_5 = 0, gave_6 = 0, gave_7 = 0, gave_8 = 0, gave_9 = 0, gave_10 = 0, gave_11 = 0, gave_12 = 0, gave_13 = 0, gave_14 = 0, gave_15 = 0;
	double candicate = 0;  //候選者
	double candicateLBP = 0;//候選者
	double eligibility = 9999; //當選者
	double eligibilityLBP = 9999;
	//double candicateLBP2 = 9999;



	// //初始化
	//for (int n = 0; n < 256; n++)
	//{
	//	hist[n] = 0;
	//}
	

	//////////////////////////test
	//double candicateLBP = 0;  //候選者





	if (once == 0) // 特徵只須抓一次!!
	{
		
		//// LBP 初始化
		//for (int i = 0; i < 256; i ++) {
		//	
		//	hist[i] = 0;
		//	//hist2[i] = 0;

		//} // for
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        //for ( int numm = 0; numm < ( sizeof( g_LBPbetter ) / sizeof( g_LBPbetter [ 0 ] ) ) + 1; numm ++ ) {

        //    g_LBPbetterAve1 [ numm ] = 0;
        //    g_LBPbetterHist1 [ numm ] = 0 ;

        //} // for
        for ( int numm = 0; numm < 256; numm ++ ) {

            g_LBPbetterAve1 [ numm ] = 0;
            g_LBPbetterHist1 [ numm ] = 0;

        } // for

		ave0 = 0, ave1 = 0, ave2 = 0, ave3 = 0, ave4 = 0, ave5 = 0, ave6 = 0, ave7 = 0, ave8 = 0;
		total = 0;
		histogram0 = 0, histogram1 = 0, histogram2 = 0, histogram3 = 0, histogram4 = 0, histogram5 = 0, histogram6 = 0, histogram7 = 0, histogram8 = 0;
		LBPhistogram0 = 0, LBPhistogram1 = 0, LBPhistogram2 = 0, LBPhistogram3 = 0, LBPhistogram4 = 0, LBPhistogram5 = 0, LBPhistogram6 = 0, LBPhistogram7 = 0, LBPhistogram8 = 0;
		skin_total = 0;

        for ( int i = P1_x + ( P2_x - P1_x ) / 5; i<P1_x + ( P2_x - P1_x ) - ( P2_x - P1_x ) / 3 && i < width_m; i++ )
            //for (int i = m + (P2_x - P1_x) ; i<m + (P2_x - P1_x) ; i++)
            for ( int j = P1_y + ( P2_y - P1_y ) / 2; j<P1_y + ( P2_y - P1_y ) && j < height_m; j++ ) {

		//for (int i = P1_x + (P2_x - P1_x) / 3; i<P2_x - (P2_x - P1_x) / 6 && i < width_m; i++)  // 抓ROI內的特徵(抓ROI方框中下半部分?) //內縮 
		//	//for (int i = P1_x ; i<P2_x ; i++)  // 抓ROI內的特徵(抓ROI方框中下半部分?) //內縮 
		//for (int j = P1_y + (P2_y - P1_y) * 1 / 2; j < P2_y && j < height_m ; j++)//下半部
		//{


            //-------計算標準差-------**********************************************************************************************************
            //mean = ( p [ 1 ] + p [ 2 ] + p [ 3 ] + p [ 4 ] + p [ 5 ] + p [ 6 ] + p [ 7 ] + p [ 8 ] + center ) / 9;
            int center;
            int root2 = 128;
            int sum = 0;
            int p [ 9 ] = { 0 };


            p [ 1 ] = YCbCr_Y [ i ] [ j - 1 ];
            p [ 2 ] = YCbCr_Y [ i - 1 ] [ j - 1 ];
            p [ 3 ] = YCbCr_Y [ i - 1 ] [ j ];
            p [ 4 ] = YCbCr_Y [ i - 1 ] [ j + 1 ];
            p [ 5 ] = YCbCr_Y [ i ] [ j + 1 ];
            p [ 6 ] = YCbCr_Y [ i + 1 ] [ j + 1 ];
            p [ 7 ] = YCbCr_Y [ i + 1 ] [ j ];
            p [ 8 ] = YCbCr_Y [ i + 1 ] [ j - 1 ];

            center = YCbCr_Y [ i ] [ j ];

            for ( int z = 1; z <= 8; z++ ) {

                // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                //if (p[z] - mean >= 0)
                if ( p [ z ] - center >= 0 )
                    sum += root2;

                root2 /= 2;
            }

            // LBP 直方圖累加

            g_LBPbetterHist1 [ g_hashtransLBPtoHist [ sum ] ] ++;

            //-------計算標準差-------**********************************************************************************************************

			if (YCbCr_Cr[i][j] > 133 && YCbCr_Cr[i][j] < 174 && YCbCr_Cb[i][j] > 77 && YCbCr_Cb[i][j] < 127) 
			{
				skin_total++;
				total++;

				//--------------------------------HOG---------------------------------------------
				int Gx = 0, Gy = 0, Gxy = 0; //計算梯度值
				if (i != 0 && i != width_m && j != 0 && j != height_m) //i,j>0 且不要是最邊點
					//Gx = YCbCr_Y[i + 1][j] - YCbCr_Y[i - 1][j];  //左右方向灰度值相減
				Gy = YCbCr_Y[i][j + 1] - YCbCr_Y[i][j - 1];	 //上下相減



				if (-15 <= Gy && Gy < -9)     //將梯度歸納到八個方向中
					histogram0++;
				else if (-9 <= Gy && Gy < -6)
					histogram1++;
				else if (-6 <= Gy && Gy < -3)
					histogram2++;
				else if (-3 <= Gy && Gy < 0)
					histogram3++;
				else if (0 <= Gy && Gy < 3)
					histogram4++;
				else if (3 <= Gy && Gy < 6)
					histogram5++;
				else if (6 <= Gy && Gy < 9)
					histogram6++;
				else if (9 <= Gy && Gy < 15)
					histogram7++;
				else
					total--;

				

				

					//system("pause");
				// ------------------ LBP 256 bits -----------------------
				int feature = 0, feature2 = 0, center;
				int mean = 0;	//標準差
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

				center = YCbCr_Y[i][j];


				//-------計算標準差-------
				mean = (p[1] + p[2] + p[3] + p[4] + p[5] + p[6] + p[7] + p[8] + center) / 9;

				for (int z = 1; z <= 8; z++)
				{

                    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
					//if (p[z] - mean >= 0)
                    if ( p [ z ] - center >= 0 )
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



				/*
				for (int z = 0; z < 248; z++)
				{

				if (hist[z]>0)
				cout << "histogrammmmmm:" << z << " = " << hist[z] << endl;

				}
				*/

                // LBPbetter 直方圖累加
                // g_LBPbetterHist1 [ g_hashtransLBPtoHist [ sum ] ] ++;
                // g_LBPbetterHist1 [ sum ] ++; //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

				if (sum <= 255 && sum > 239)
					LBPhistogram0++;
				else if (sum <= 239 && sum > 223)
					LBPhistogram1++;
				else if (sum <= 223 && sum > 207)
					LBPhistogram2++;
				else if (sum <= 207 && sum > 191)
					LBPhistogram3++;
				else if (sum <= 191 && sum > 175)
					LBPhistogram4++;
				else if (sum <= 175 && sum > 159)
					LBPhistogram5++;
				else if (sum <= 159 && sum > 143)
					LBPhistogram6++;
				else if (sum <= 143 && sum > 127)
					LBPhistogram7++;
				else if (sum <= 127 && sum > 111)
					LBPhistogram8++;
				else if (sum <= 111 && sum > 95)
					LBPhistogram9++;
				else if (sum <= 95 && sum > 79)
					LBPhistogram10++;
				else if (sum <= 79 && sum > 63)
					LBPhistogram11++;
				else if (sum <= 63 && sum > 47)
					LBPhistogram12++;
				else if (sum <= 47 && sum > 31)
					LBPhistogram13++;
				else if (sum <= 31 && sum >= 0)
					LBPhistogram14++;


				// ------------------ END LBP 結束!! -----------------------

			}


			
			


			//cout << "feature:" << LBPhistogram0 << "," << LBPhistogram1 << "," << LBPhistogram2 << "," << LBPhistogram3 << "," << LBPhistogram4 << "," << LBPhistogram5 << "," << LBPhistogram6 << "," << LBPhistogram7 <<","<< LBPhistogram8 << "," << LBPhistogram9 << "," << LBPhistogram10 << "," << LBPhistogram11 << "," << LBPhistogram12 << "," << LBPhistogram13 << "," << LBPhistogram14  << endl;



			//// ------------------ LBP -----------------------
			//int feature = 0, feature2 = 0, center;
			//int mean = 0;	//標準差
			//int p[9] = { 0 };
			////getPixel(width,height)

			//p[1] = YCbCr_Y[i][j - 1];
			//p[2] = YCbCr_Y[i - 1][j - 1];
			//p[3] = YCbCr_Y[i - 1][j];
			//p[4] = YCbCr_Y[i - 1][j + 1];
			//p[5] = YCbCr_Y[i][j + 1];
			//p[6] = YCbCr_Y[i + 1][j + 1];
			//p[7] = YCbCr_Y[i + 1][j];
			//p[8] = YCbCr_Y[i + 1][j - 1];

			//center = YCbCr_Y[i][j];


			////-------計算標準差-------
			//mean = (p[1] + p[2] + p[3] + p[4] + p[5] + p[6] + p[7] + p[8] + center) / 9;

			//for (int z = 1; z <= 8; z++)
			//{


			//	if (p[z] - mean>0)

			//		hist[z]++;

			//}
			//// ------------------ END LBP 結束!! -----------------------

			


		}


		//cout << "feature:" << histogram0 << "," << histogram1 << "," << histogram2 << "," << histogram3 << "," << histogram4 << "," << histogram5 << "," << histogram6 << "," << histogram7 << endl;

		//cout << "feature:" << hist[1] << "," << hist[2] << "," << hist[3] << "," << hist[4] << "," << hist[5] << "," << hist[6] << "," << hist[7] << "," << hist[8] << endl;

		cout << " 1111111111111111111111111111111111111111111" << endl;
		//----------------------------------------------------------LBP


		

		//for (int i = P1_x + 1; i < P2_x - 1; i++)  // 抓ROI內的特徵(抓ROI方框中下半部分?) //內縮
		//{

		//	for (int j = P1_y + 1 + (P2_y - P1_y) * 1 / 2; j < P2_y - 1; j++)
		//	{
		//		
		//		int feature = 0, feature2 = 0, center;
		//		int StdDeviation = 0, mean = 0, mean2 = 0, StdDeviation2 = 0;	//標準差
		//		int p[5] = { 0 }, q[5] = {0};
		//		//getPixel(width,height)

		//		p[1] = YCbCr_Y[i][j - 1];
		//		p[2] = YCbCr_Y[i - 1][j - 1];
		//		p[3] = YCbCr_Y[i - 1][j];
		//		p[4] = YCbCr_Y[i - 1][j + 1];


		//		q[1] = YCbCr_Y[i][j + 1];
		//		q[2] = YCbCr_Y[i + 1][j + 1];
		//		q[3] = YCbCr_Y[i + 1][j];
		//		q[4] = YCbCr_Y[i + 1][j - 1];

		//		center = YCbCr_Y[i][j];


		//		//-------計算標準差-------
		//		mean = (p[1] + p[2] + p[3] + p[4] + center) / 5;
		//		mean2 = (q[1] + q[2] + q[3] + q[4] + center) / 5;
		//		StdDeviation += pow(center - mean, 2);
		//		StdDeviation2 += pow(center - mean2, 2);
		//		for (int i = 1; i <= 4; i++) //平方相加
		//		{
		//			StdDeviation += pow(p[i] - mean, 2);
		//			StdDeviation2 += pow(q[i] - mean2, 2);
		//		}
		//		StdDeviation = (int)(sqrt((StdDeviation / 5)) * 0.3);//平均開跟號	
		//		StdDeviation2 = (int)(sqrt((StdDeviation2 / 5)) * 0.3);


		//		for (int z = 1; z <= 4; z++)
		//		{
		//			int pow = 4 - z;
		//			feature += getFeature(p[z], center, pow, mean);
		//			feature2 += getFeature(q[z], center, pow, mean2);

		//		}
		//		//setPixel(width,height,pixel)


		//		//cout << "feature:" << feature << endl;
		//		hist[feature]++;
		//		hist2[feature2]++;


		//		//system("pause");






		//	
		//	}

		//}
		//
		//-----------------------------------------------------------LBP


		//cout << "LBPPPPPPPPfeature1-->  " << hist[1] << "，" << hist[2] << "，" << hist[3] << "，" << hist[4] << "，" << hist[5] << "，" << hist[6] << "，" << hist[7] << "，" << hist[8] << "，" << hist[9] << "，" << hist[10] << "，" << hist[11] << "，" << hist[12] << "，" << hist[13] << "，" << hist[14] << "，" << hist[15] << endl;
		//cout << "LBPPPPPPPPfeature2-->  " << hist2[1] << "，" << hist2[2] << "，" << hist2[3] << "，" << hist2[4] << "，" << hist2[5] << "，" << hist2[6] << "，" << hist2[7] << "，" << hist2[8] << "，" << hist[9] << "，" << hist2[10] << "，" << hist2[11] << "，" << hist2[12] << "，" << hist2[13] << "，" << hist2[14] << "，" << hist2[15] << endl;

		//int temptemp = 0;
		//for (int i = P1_x + (P2_x - P1_x) / 3; i<P2_x - (P2_x - P1_x) / 6; i++)       //特徵取ROI內縮 可以改善太容易被臉追掉問題 嗎
		//for (int j = P1_y + (P2_y - P1_y) / 2; j<P2_y ; j++)
		//{


		//	
		//	temptemp++;
		//	if (temptemp == total) //跳出迴圈
		//		i = 640, j = 480;
		//	if (0 <= YCbCr_Y[i][j] && YCbCr_Y[i][j]<16)
		//		ghistogram0++;
		//	else if (16 <= YCbCr_Y[i][j] && YCbCr_Y[i][j]<32)
		//		ghistogram1++;
		//	else if (32 <= YCbCr_Y[i][j] && YCbCr_Y[i][j]<48)
		//		ghistogram2++;
		//	else if (48 <= YCbCr_Y[i][j] && YCbCr_Y[i][j]<64)
		//		ghistogram3++;
		//	else if (64 <= YCbCr_Y[i][j] && YCbCr_Y[i][j]<80)
		//		ghistogram4++;
		//	else if (80 <= YCbCr_Y[i][j] && YCbCr_Y[i][j]<96)
		//		ghistogram5++;
		//	else if (96 <= YCbCr_Y[i][j] && YCbCr_Y[i][j]<112)
		//		ghistogram6++;
		//	else if (112 <= YCbCr_Y[i][j] && YCbCr_Y[i][j]<128)
		//		ghistogram7++;
		//	else if (128 <= YCbCr_Y[i][j] && YCbCr_Y[i][j]<144)
		//		ghistogram8++;
		//	else if (144 <= YCbCr_Y[i][j] && YCbCr_Y[i][j]<160)
		//		ghistogram9++;
		//	else if (160 <= YCbCr_Y[i][j] && YCbCr_Y[i][j]<176)
		//		ghistogram10++;
		//	else if (176 <= YCbCr_Y[i][j] && YCbCr_Y[i][j]<192)
		//		ghistogram11++;
		//	else if (192 <= YCbCr_Y[i][j] && YCbCr_Y[i][j]<208)
		//		ghistogram12++;
		//	else if (208 <= YCbCr_Y[i][j] && YCbCr_Y[i][j]<224)
		//		ghistogram13++;
		//	else if (224 <= YCbCr_Y[i][j] && YCbCr_Y[i][j]<240)
		//		ghistogram14++;
		//	else if (240 <= YCbCr_Y[i][j] && YCbCr_Y[i][j]<256)
		//		ghistogram15++;
		//}

		if (total == 0)
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


        // 對 LBPBetter 做正規化
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        for ( int i = 0; i < ( sizeof( g_LBPbetter ) / sizeof( g_LBPbetter [ 0 ] ) ) + 1; i ++ ) {

            g_LBPbetterAve1 [ i ] = g_LBPbetterHist1 [ i ] ;// / total;

        } // for

        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        //for ( int i = 0; i < 256; i ++ ) {

        //    g_LBPbetterAve1 [ i ] = g_LBPbetterHist1 [ i ] / total;

        //} // for

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
		LBPave11 = LBPhistogram11/ total;
		LBPave12 = LBPhistogram12 / total;
		LBPave13 = LBPhistogram13 / total;
		LBPave14 = LBPhistogram14 / total;

		

		/*gave0 = ghistogram0 / total;
		gave1 = ghistogram1 / total;
		gave2 = ghistogram2 / total;
		gave3 = ghistogram3 / total;
		gave4 = ghistogram4 / total;
		gave5 = ghistogram5 / total;
		gave6 = ghistogram6 / total;
		gave7 = ghistogram7 / total;
		gave8 = ghistogram8 / total;
		gave9 = ghistogram9 / total;
		gave10 = ghistogram10 / total;
		gave11 = ghistogram11 / total;
		gave12 = ghistogram12 / total;
		gave13 = ghistogram13 / total;
		gave14 = ghistogram14 / total;
		gave15 = ghistogram15 / total;*/

		//cout << "******************meanshift*******************" << endl;
		//cout << gave0 << " ," << gave1 << "," << gave2 << "," << gave3 << "," << gave4 << "," << gave5 << "," << gave6 << "," << gave7 << "," << gave8 << "," << gave9 << "," << gave10 << "," << gave11 << "," << gave12 << "," << gave13 << "," << gave14 << "," << gave15 << "," << endl;

		disbetweenfingertip = P2_y - P1_y;
		Sobelcount2 = 0;
		for (int i = P1_x; i < P2_x; i++) //sobel累加 //邊界
		{
			for (int j = P1_y + (P2_y - P1_y) / 2; j < P1_y + (P2_y - P1_y) * 3 / 4 && j < height_m; j++)
			{
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

		once++;  // 特徵只需要抓一次，執行完這個，就不會再進這個迴圈。
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

	/*int gthistogram0 = 0;
	int gthistogram1 = 0;
	int gthistogram2 = 0;
	int gthistogram3 = 0;
	int gthistogram4 = 0;
	int gthistogram5 = 0;
	int gthistogram6 = 0;
	int gthistogram7 = 0;
	int	gthistogram8 = 0;
	int gthistogram9 = 0;
	int gthistogram10 = 0;
	int gthistogram11 = 0;
	int gthistogram12 = 0;
	int gthistogram13 = 0;
	int gthistogram14 = 0;
	int gthistogram15 = 0;*/

	Threshold = 10.0;

	int DiffFramecount = 0;

	//cvRectangle(frame_320x240, cvPoint((P1_x - (P2_x - P1_x) / 2) * 2, (P1_y - (P2_y - P1_y) / 2) * 2), cvPoint((P2_x + (P2_x - P1_x) / 4) * 2, (P2_y + (P2_y - P1_y) / 4) * 2), CV_RGB(0, 255, 0), 1, CV_AA, 1);


	int feature1 = 0; //角點特徵
	
	// 累計放大區塊裡，候選區塊的總數(放大區塊m*n裡面，可能會有數個候選區塊)
	int countOfFrameCandidate = 0;



    //cvRectangle( frame_320x240, 
    //             cvPoint( P1_x , P1_y ),
    //             cvPoint( P2_x , P2_y ),
    //             CV_RGB( 0, 125, 125 ), 
    //             1, 
    //             CV_AA, 
    //             0 ); // 畫當選者的ROI

	// 由上到下，有左到右                 
	for (int m = (P1_x - (P2_x - P1_x) >= 0) ? P1_x - (P2_x - P1_x) : 0;  // 避免放大框架超出原圖範圍
		m < P2_x + (P2_x - P1_x) / 3 - (P2_x - P1_x) && m < width_m;
			 m +=5) // 可調整速度，+越多越快，ex: m += 3

			 for (int n = (P1_y - (P2_y - P1_y) / 4 >= 0) ? P1_y - (P2_y - P1_y) / 4 : 0 ;
				 n < P2_y + (P2_y - P1_y) / 3 - (P2_y - P1_y) && n <  height_m ;
					  n +=5) // 可調整速度，+越多越快，ex: n += 3
	{

        // 初始化 histogram2
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        for ( int numm = 0; numm < ( sizeof( g_LBPbetter ) / sizeof( g_LBPbetter [ 0 ] ) ) + 1; numm ++ ) {

            g_LBPbetterAve2 [ numm ] = 0 ;
            g_LBPbetterHist2 [ numm ] = 0 ;

        } // for

        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        //for ( int numm = 0; numm < 256; numm ++ ) {

        //    g_LBPbetterAve2 [ numm ] = 0;
        //    // g_LBPbetterHist2 [ numm ] = 0;

        //} // for

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
		

		/*ghistogram0 = 0;
		ghistogram1 = 0;
		ghistogram2 = 0;
		ghistogram3 = 0;
		ghistogram4 = 0;
		ghistogram5 = 0;
		ghistogram6 = 0;
		ghistogram7 = 0;
		ghistogram8 = 0;
		ghistogram9 = 0;
		ghistogram10 = 0;
		ghistogram11 = 0;
		ghistogram12 = 0;
		ghistogram13 = 0;
		ghistogram14 = 0;
		ghistogram15 = 0;
		*/
		skin_total2 = 0;



		 //LBP 初始化
		//for (int i = 0; i < 256; i++) {
		//	hist2[i] = 0;
		//} // for

        //if ( m == 0 && n == 0 ) {
        if (1) {
            cvRectangle( frame_320x240,
                         cvPoint( m + ( P2_x - P1_x ) / 5, n + ( P2_y - P1_y ) / 2 ),
                         cvPoint( m + ( P2_x - P1_x ) - ( P2_x - P1_x ) / 3, n + ( P2_y - P1_y ) ),
                         CV_RGB( 50, 50, 50 ),
                         1,
                         CV_AA,
                         0 ); // 畫當選者的ROI

        } // if

		for (int i = m + (P2_x - P1_x) / 5; i<m + (P2_x - P1_x) - (P2_x - P1_x) / 3 && i < width_m; i++)
			//for (int i = m + (P2_x - P1_x) ; i<m + (P2_x - P1_x) ; i++)
		for (int j = n + (P2_y - P1_y) / 2; j<n + (P2_y - P1_y) && j < height_m ; j++)
		{


			// ----------------------時間差相減法---------開跑囉!!-------------------
			int Diff_Value = 0;
			Diff_Value = abs(YCbCr_Y[i][j] - Pre[i][j]); // abs取整數絕對值
			Pre[i][j] = YCbCr_Y[i][j];

			if (Diff_Value > Threshold)
			{
				DiffFramecount++; //超過T則做累加
			}

			// ----------------------時間差相減法---------結束!!-------------------
            //-------計算標準差-------**********************************************************************************************************
            //mean = ( p [ 1 ] + p [ 2 ] + p [ 3 ] + p [ 4 ] + p [ 5 ] + p [ 6 ] + p [ 7 ] + p [ 8 ] + center ) / 9;
            int feature = 0, center;
            int root2 = 128;
            int sum = 0;
            int p [ 9 ] = { 0 };


            p [ 1 ] = YCbCr_Y [ i ] [ j - 1 ];
            p [ 2 ] = YCbCr_Y [ i - 1 ] [ j - 1 ];
            p [ 3 ] = YCbCr_Y [ i - 1 ] [ j ];
            p [ 4 ] = YCbCr_Y [ i - 1 ] [ j + 1 ];
            p [ 5 ] = YCbCr_Y [ i ] [ j + 1 ];
            p [ 6 ] = YCbCr_Y [ i + 1 ] [ j + 1 ];
            p [ 7 ] = YCbCr_Y [ i + 1 ] [ j ];
            p [ 8 ] = YCbCr_Y [ i + 1 ] [ j - 1 ];

            center = YCbCr_Y [ i ] [ j ];

            for ( int z = 1; z <= 8; z++ ) {

                // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                //if (p[z] - mean >= 0)
                if ( p [ z ] - center >= 0 )
                    sum += root2;

                root2 /= 2;
            }

            // LBP 直方圖累加

            g_LBPbetterHist2 [ g_hashtransLBPtoHist [ sum ] ] ++;

            //-------計算標準差-------**********************************************************************************************************

			if (YCbCr_Cr[i][j] > 133 && YCbCr_Cr[i][j] < 174 && YCbCr_Cb[i][j] > 77 && YCbCr_Cb[i][j] < 127)
			{
				skin_total2++;
				int Gx = 0, Gy = 0, Gxy = 0;
				if (i != 0 && i != width_m && j != 0 && j != height_m)
					Gx = YCbCr_Y[i + 1][j] - YCbCr_Y[i - 1][j];
				Gy = YCbCr_Y[i][j + 1] - YCbCr_Y[i][j - 1];	 //上下相減

				//cout<<orientation<<endl;
				//cout<<Gx<<" "<<Gy<<endl;
			
				if (-15 <= Gy && Gy < -9)
					histogram0++;
				else if (-9 <= Gy && Gy < -6)
					histogram1++;
				else if (-6 <= Gy && Gy < -3)
					histogram2++;
				else if (-3 <= Gy && Gy < 0)
					histogram3++;
				else if (0 <= Gy && Gy < 3)
					histogram4++;
				else if (3 <= Gy && Gy < 6)
					histogram5++;
				else if (6 <= Gy && Gy < 9)
					histogram6++;
				else if (9 <= Gy && Gy < 15)
					histogram7++;
				else
					;

				// ------------------ LBP 256 bits -----------------------

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

				center = YCbCr_Y[i][j];


				//-------計算標準差-------
				mean = (p[1] + p[2] + p[3] + p[4] + p[5] + p[6] + p[7] + p[8] + center) / 9;

				for (int z = 1; z <= 8; z++)
				{

                    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
					// if (p[z] - mean >= 0)
                    if ( p [ z ] - center >= 0 )
						sum += root2;

					root2 /= 2;
				}






				// LBP 直方圖累加!!



				/*if (sum == 7 || sum == 14 || sum == 15 || sum == 28 || sum == 30 || sum == 31 || sum == 48 || sum == 56 || sum == 60 || sum == 62 || sum == 63 || sum == 96 || sum == 112 || sum == 120 || sum == 124 || sum == 126 || sum == 131 || sum == 135 || sum == 143 || sum == 159 || sum == 192 || sum == 193 || sum == 195 || sum == 199 || sum == 207 || sum == 224 || sum == 225 || sum == 227 || sum == 231 || sum == 239 || sum == 240 || sum == 241 || sum == 247 || sum == 248)
				hist2[sum]++;
				else
				hist2[255]++;
				*/

                // LBPbetter 直方圖累加
                // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                //g_LBPbetterHist2 [ g_hashtransLBPtoHist [ sum ] ] ++;
                // g_LBPbetterHist2 [ sum ] ++;

				if (sum <= 255 && sum > 239)
					LBPhistogram0++;
				else if (sum <= 239 && sum > 223)
					LBPhistogram1++;
				else if (sum <= 223 && sum > 207)
					LBPhistogram2++;
				else if (sum <= 207 && sum > 191)
					LBPhistogram3++;
				else if (sum <= 191 && sum > 175)
					LBPhistogram4++;
				else if (sum <= 175 && sum > 159)
					LBPhistogram5++;
				else if (sum <= 159 && sum > 143)
					LBPhistogram6++;
				else if (sum <= 143 && sum > 127)
					LBPhistogram7++;
				else if (sum <= 127 && sum > 111)
					LBPhistogram8++;
				else if (sum <= 111 && sum > 95)
					LBPhistogram9++;
				else if (sum <= 95 && sum > 79)
					LBPhistogram10++;
				else if (sum <= 79 && sum > 63)
					LBPhistogram11++;
				else if (sum <= 63 && sum > 47)
					LBPhistogram12++;
				else if (sum <= 47 && sum > 31)
					LBPhistogram13++;
				else if (sum <= 31 && sum >= 0)
					LBPhistogram14++;



				// ------------------ END LBP 結束!! -----------------------



				/*if (sum <= 255 && sum > 223)
					LBPhistogram0++;
				else if (sum <= 223 && sum > 191)
					LBPhistogram1++;
				else if (sum <= 191 && sum > 159)
					LBPhistogram2++;
				else if (sum <= 159 && sum > 127)
					LBPhistogram3++;
				else if (sum <= 127 && sum > 95)
					LBPhistogram4++;
				else if (sum <= 95 && sum > 63)
					LBPhistogram5++;
				else if (sum <= 63 && sum > 31)
					LBPhistogram6++;
				else if (sum <= 31 && sum >= 0)
					LBPhistogram7++;*/

				
				
				//// ------------------ LBP -----------------------

				//int feature = 0, feature2 = 0, center;
				//int mean = 0;
				//int p[9] = { 0 };
				////getPixel(width,height)

				//p[1] = YCbCr_Y[i][j - 1];
				//p[2] = YCbCr_Y[i - 1][j - 1];
				//p[3] = YCbCr_Y[i - 1][j];
				//p[4] = YCbCr_Y[i - 1][j + 1];
				//p[5] = YCbCr_Y[i][j + 1];
				//p[6] = YCbCr_Y[i + 1][j + 1];
				//p[7] = YCbCr_Y[i + 1][j];
				//p[8] = YCbCr_Y[i + 1][j - 1];

				//center = YCbCr_Y[i][j];


				////-------計算標準差-------
				//mean = (p[1] + p[2] + p[3] + p[4] + p[5] + p[6] + p[7] + p[8] + center) / 9;

				//for (int z = 1; z <= 8; z++)
				//{


				//	if (p[z] - mean>0)

				//		hist2[z]++;

				//}
				//// ------------------ END LBP 結束!! -----------------------


				

				



				//if (LabelMap[i][j] == 1 && i>2 && j >2)
				//{
				//	int corner_count = 0;                  //scan labelmap=1 的點之周圍，如果值是自己本身的區塊編號就累加
				//	if (LabelMap[i][j - 2] == k)
				//		corner_count++;
				//	if (LabelMap[i + 1][j - 1] == k)
				//		corner_count++;
				//	if (LabelMap[i + 2][j] == k)
				//		corner_count++;
				//	if (LabelMap[i + 1][j + 1] == k)
				//		corner_count++;
				//	if (LabelMap[i][j + 2] == k)
				//		corner_count++;
				//	if (LabelMap[i - 1][j + 1] == k)
				//		corner_count++;
				//	if (LabelMap[i - 2][j] == k)
				//		corner_count++;
				//	if (LabelMap[i - 1][j - 1] == k)
				//		corner_count++;
				//	if (corner_count < 2)
				//	{
				//		LabelMap[i][j] = 31;
				//		feature1++;
				//		//cout << corner_count << endl;
				//		//cout <<"點點="<< ROI_Info[k].feature_Amount1 << endl;
				//		for (int mm = i; mm >= i - 3; mm--)     //過濾掉自己附近比自己低的角點
				//		for (int nn = j + 3; nn >= j - 4; nn--)
				//		{

				//			if (LabelMap[mm][nn] == 31)
				//			{

				//				if (LabelMap[i - 2][j] == 0 && LabelMap[mm - 1][nn] != 0)   // 防止前面的角點取代到
				//				{

				//					//cout << "1111111111111111111111111111111111111我進來了!!!!!" << endl;

				//				}
				//				else//高角點取代低角點   
				//				{
				//					//cout << "2我進來了!!!!!" << endl;

				//					if (mm == i && nn == j)
				//					{
				//					}
				//					if (nn < j)  //低於自己的角點
				//					{
				//						LabelMap[i][j] = 1;
				//						feature1--;
				//						mm = 0, nn = 0;
				//					}
				//					if (nn>j)    //
				//					{
				//						LabelMap[mm][nn] = 1;
				//						feature1--;
				//						mm = 0, nn = 0;

				//					}
				//					if (nn == j && mm != i)
				//					{//一樣高折衷
				//						LabelMap[mm][nn] = 1;
				//						LabelMap[i][j] = 1;
				//						LabelMap[i - 1][j] = 31;
				//						feature1--;
				//						mm = 0, nn = 0;


				//					}
				//				}
				//			}
				//		}



				//	}

				//}  // if (LabelMap[i][j] == 1 && i>2 && j >2)

			



			}
			//histogram8++;
			
			
			//int feature = 0, feature2 = 0, center;
			//int StdDeviation = 0, mean = 0, mean2 = 0, StdDeviation2 = 0;	//標準差
			//int p[5], q[5];
			////getPixel(width,height)

			//p[1] = YCbCr_Y[i][j - 1];
			//p[2] = YCbCr_Y[i - 1][j - 1];
			//p[3] = YCbCr_Y[i - 1][j];
			//p[4] = YCbCr_Y[i - 1][j + 1];


			//q[1] = YCbCr_Y[i][j + 1];
			//q[2] = YCbCr_Y[i + 1][j + 1];
			//q[3] = YCbCr_Y[i + 1][j];
			//q[4] = YCbCr_Y[i + 1][j - 1];

			//center = YCbCr_Y[i][j];


			////-------計算標準差-------
			//mean = (p[1] + p[2] + p[3] + p[4] + center) / 5;
			//mean2 = (q[1] + q[2] + q[3] + q[4] + center) / 5;
			///*StdDeviation += pow(center - mean, 2);
			//StdDeviation2 += pow(center - mean2, 2);*/
			//StdDeviation += (center - mean)*(center - mean);
			//StdDeviation2 += (center - mean2)*(center - mean2);
			//for (int i = 1; i <= 4; i++) //平方相加
			//{
			//	//StdDeviation += pow(p[i] - mean, 2);
			//	//StdDeviation2 += pow(q[i] - mean2, 2);
			//	StdDeviation += (p[i] - mean) * (p[i] - mean);
			//	StdDeviation2 += (q[i] - mean2) * (q[i] - mean2);
			//}
			////StdDeviation = (int)(sqrt((StdDeviation / 5)) * 0.3);//平均開跟號	
			////StdDeviation2 = (int)(sqrt((StdDeviation2 / 5)) * 0.3);

			//int testpow = 8;

			//for (int z = 1; z <= 4; z++)
			//{
			//	
			//	testpow /= 2;

			//	if (p[z] > mean)
			//	{
			//		feature += testpow;
			//	}
			//	else {
			//		feature += 0;
			//	} // else

			//	if (q[z] > mean2)
			//	{
			//		feature2 += testpow;
			//	}
			//	else {
			//		feature2 += 0;
			//	} // else

			//	//feature += getFeature(, center, pow, mean);
			//	//feature2 += getFeature(q[z], center, pow, mean2);

			//}
			////setPixel(width,height,pixel)

			//

			//hist11[feature]++;
			//hist22[feature2]++;

		}


		////----------------------------------------------------------LBP
		//for (int i = m+P1_x + 1; i <m+ P2_x - 1; i++)  // 抓ROI內的特徵(抓ROI方框中下半部分?) //內縮
		//{

		//	for (int j =n+ P1_y + 1 + (P2_y - P1_y) * 1 / 2; j <n+ P2_y - 1; j++)
		//	{
		//		
		//		
		//		int feature = 0, feature2 = 0, center;
		//		int StdDeviation = 0, mean = 0, mean2 = 0, StdDeviation2 = 0;	//標準差
		//		int p[5], q[5];
		//		//getPixel(width,height)

		//		p[1] = YCbCr_Y[i][j - 1];
		//		p[2] = YCbCr_Y[i - 1][j - 1];
		//		p[3] = YCbCr_Y[i - 1][j];
		//		p[4] = YCbCr_Y[i - 1][j + 1];


		//		q[1] = YCbCr_Y[i][j + 1];
		//		q[2] = YCbCr_Y[i + 1][j + 1];
		//		q[3] = YCbCr_Y[i + 1][j];
		//		q[4] = YCbCr_Y[i + 1][j - 1];

		//		center = YCbCr_Y[i][j];

		//		
		//		//-------計算標準差-------
		//		mean = (p[1] + p[2] + p[3] + p[4] + center) / 5;
		//		mean2 = (q[1] + q[2] + q[3] + q[4] + center) / 5;
		//		StdDeviation += pow(center - mean, 2);
		//		StdDeviation2 += pow(center - mean2, 2);
		//		for (int i = 1; i <= 4; i++) //平方相加
		//		{
		//			StdDeviation += pow(p[i] - mean, 2);
		//			StdDeviation2 += pow(q[i] - mean2, 2);
		//		}
		//		StdDeviation = (int)(sqrt((StdDeviation / 5)) * 0.3);//平均開跟號	
		//		StdDeviation2 = (int)(sqrt((StdDeviation2 / 5)) * 0.3);


		//		for (int z = 1; z <= 4; z++)
		//		{
		//			int pow = 4 - z;
		//			feature += getFeature(p[z], center, pow, mean);
		//			feature2 += getFeature(q[z], center, pow, mean2);
		//		
		//		}
		//		//setPixel(width,height,pixel)


		//		cout << "i:" << i << " j:" << j << endl;
		//		cout << " 22222222222222222222222222222222222222222222222222222" << endl;
		//		
		//		//system("pause");

		//		hist11[feature]++;
		//		hist22[feature2]++;








		//	
		//	}

		//}

		//cout << "LBPPPPPPPPfeature11-->  " << hist11[1] << "，" << hist11[2] << "，" << hist11[3] << "，" << hist11[4] << "，" << hist11[5] << "，" << hist11[6] << "，" << hist11[7] << "，" << hist11[8] << "，" << hist11[9] << "，" << hist11[10] << "，" << hist11[11] << "，" << hist11[12] << "，" << hist11[13] << "，" << hist11[14] << "，" << hist11[15] << endl;
		//cout << "LBPPPPPPPPfeature22-->  " << hist22[1] << "，" << hist22[2] << "，" << hist22[3] << "，" << hist22[4] << "，" << hist22[5] << "，" << hist22[6] << "，" << hist22[7] << "，" << hist22[8] << "，" << hist22[9] << "，" << hist22[10] << "，" << hist22[11] << "，" << hist22[12] << "，" << hist22[13] << "，" << hist22[14] << "，" << hist22[15] << endl;

		//-----------------------------------------------------------LBP

		/*int temptemp = 0;
		for (int i = m + (P2_x - P1_x) / 5; i<m + (P2_x - P1_x) - (P2_x - P1_x) / 3; i++)
		for (int j = n + (P2_y - P1_y) / 2; j<n + (P2_y - P1_y); j++)
		{
		temptemp++;
		if (temptemp == total)
		i = 640, j = 480;
		if (0 <= YCbCr_Y[i][j] && YCbCr_Y[i][j]<16)
		ghistogram0++;
		else if (16 <= YCbCr_Y[i][j] && YCbCr_Y[i][j]<32)
		ghistogram1++;
		else if (32 <= YCbCr_Y[i][j] && YCbCr_Y[i][j]<48)
		ghistogram2++;
		else if (48 <= YCbCr_Y[i][j] && YCbCr_Y[i][j]<64)
		ghistogram3++;
		else if (64 <= YCbCr_Y[i][j] && YCbCr_Y[i][j]<80)
		ghistogram4++;
		else if (80 <= YCbCr_Y[i][j] && YCbCr_Y[i][j]<96)
		ghistogram5++;
		else if (96 <= YCbCr_Y[i][j] && YCbCr_Y[i][j]<112)
		ghistogram6++;
		else if (112 <= YCbCr_Y[i][j] && YCbCr_Y[i][j]<128)
		ghistogram7++;
		else if (128 <= YCbCr_Y[i][j] && YCbCr_Y[i][j]<144)
		ghistogram8++;
		else if (144 <= YCbCr_Y[i][j] && YCbCr_Y[i][j]<160)
		ghistogram9++;
		else if (160 <= YCbCr_Y[i][j] && YCbCr_Y[i][j]<176)
		ghistogram10++;
		else if (176 <= YCbCr_Y[i][j] && YCbCr_Y[i][j]<192)
		ghistogram11++;
		else if (192 <= YCbCr_Y[i][j] && YCbCr_Y[i][j]<208)
		ghistogram12++;
		else if (208 <= YCbCr_Y[i][j] && YCbCr_Y[i][j]<224)
		ghistogram13++;
		else if (224 <= YCbCr_Y[i][j] && YCbCr_Y[i][j]<240)
		ghistogram14++;
		else if (240 <= YCbCr_Y[i][j] && YCbCr_Y[i][j]<256)
		ghistogram15++;






		}*/
	
		// 對 HOG 正規化!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		if (total == 0) total = 1;//防止分母為0
		ave_0 = histogram0 / total;
		ave_1 = histogram1 / total;
		ave_2 = histogram2 / total;
		ave_3 = histogram3 / total;
		ave_4 = histogram4 / total;
		ave_5 = histogram5 / total;
		ave_6 = histogram6 / total;
		ave_7 = histogram7 / total;
		ave_8 = histogram8 / total;


        // 對 LBPBetter 做正規化
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        for ( int i = 0; i < ( sizeof( g_LBPbetter ) / sizeof( g_LBPbetter [ 0 ] ) ) + 1 ; i ++ ) {
            
            g_LBPbetterAve2 [ i ] = g_LBPbetterHist2 [ i ] ;// / total ;

        } // for

        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        //for ( int i = 0; i < 256; i ++ ) {

        //    g_LBPbetterAve2 [ i ] = g_LBPbetterHist2 [ i ] / total;

        //} // for

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




		/*gave_0 = ghistogram0 / total;
		gave_1 = ghistogram1 / total;
		gave_2 = ghistogram2 / total;
		gave_3 = ghistogram3 / total;
		gave_4 = ghistogram4 / total;
		gave_5 = ghistogram5 / total;
		gave_6 = ghistogram6 / total;
		gave_7 = ghistogram7 / total;
		gave_8 = ghistogram8 / total;
		gave_9 = ghistogram9 / total;
		gave_10 = ghistogram10 / total;
		gave_11 = ghistogram11 / total;
		gave_12 = ghistogram12 / total;
		gave_13 = ghistogram13 / total;
		gave_14 = ghistogram14 / total;
		gave_15 = ghistogram15 / total;*/



		//disbetweenfingertip2 = P2_y - P1_y;
		//候選者 //越小越趨近於當選者
		//ROI_shape = (P2_y - P1_y) / (P2_x - P1_x);



		//cout << "DiffFrame_Count[m][m]=" << DiffFrame_Count[m][n] << endl;

		// LBP + HOG
		/*candicate = abs(ave0 - ave_0) + abs(ave1 - ave_1) + abs(ave2 - ave_2) + abs(ave3 - ave_3) + abs(ave4 - ave_4) + abs(ave5 - ave_5) + abs(ave6 - ave_6) + abs(ave7 - ave_7) + abs(ave8 - ave_8) +
			abs(hist[1] - hist2[1]) + abs(hist[2] - hist2[2]) + abs(hist[3] - hist2[3]) + abs(hist[4] - hist2[4]) + abs(hist[5] - hist2[5]) + abs(hist[6] - hist2[6]) + abs(hist[7] - hist2[7]) + abs(hist[8] - hist2[8]);
		*/

		// LBP
		//candicateLBP = abs(hist[1] - hist2[1]) + abs(hist[2] - hist2[2]) + abs(hist[3] - hist2[3]) + abs(hist[4] - hist2[4]) + abs(hist[5] - hist2[5]) + abs(hist[6] - hist2[6]) + abs(hist[7] - hist2[7]) + abs(hist[8] - hist2[8]);

		candicateLBP = 0;

		//for (int i = 0; i < 248; i ++) {
		//	
		//	candicateLBP += abs(hist[i] / total - hist2[i] / total);

        // 計算 LBP 門檻值
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        candicateLBPBetter = 0 ;
        for ( int numm = 0; numm < ( sizeof( g_LBPbetter ) / sizeof( g_LBPbetter [ 0 ] ) ) + 1 ; numm ++ ) {
            
            candicateLBPBetter += abs( g_LBPbetterAve1 [ numm ] - g_LBPbetterAve2 [ numm ] );

        } // for

        // 以下測試用LBP !!!!!!!!!!!!!
        //for ( int numm = 1; numm < 256; numm ++ ) {

        //    candicateLBPBetter += abs( g_LBPbetterAve1 [ numm ] - g_LBPbetterAve2 [ numm ] );

        //} // for

        

		//} // for
		candicateLBP = abs(LBPave0 - LBPave_0) + abs(LBPave1 - LBPave_1) + abs(LBPave2 - LBPave_2) + abs(LBPave3 - LBPave_3) + abs(LBPave4 - LBPave_4) + abs(LBPave5 - LBPave_5) + abs(LBPave6 - LBPave_6) + abs(LBPave7 - LBPave_7)+abs(LBPave8 - LBPave_8) + abs(LBPave9 - LBPave_9) + abs(LBPave10 - LBPave_10) + abs(LBPave11 - LBPave_11) + abs(LBPave12 - LBPave_12) + abs(LBPave13 - LBPave_13) + abs(LBPave14 - LBPave_14) ;

		//candicate = abs(ave0 - ave_0) + abs(ave1 - ave_1) + abs(ave2 - ave_2) + abs(ave3 - ave_3) + abs(ave4 - ave_4) + abs(ave5 - ave_5) + abs(ave6 - ave_6) + abs(ave7 - ave_7) + abs(ave8 - ave_8);
		//candicateLBP = abs(hist[1] - hist11[1]) + abs(hist[2] - hist11[2]) + abs(hist[3] - hist11[3]) + abs(hist[4] - hist11[4]) + abs(hist[5] - hist11[5]) + abs(hist[6] - hist11[6]) + abs(hist[7] - hist11[7]) + abs(hist[8] - hist11[8]) + abs(hist[9] - hist11[9]) + abs(hist[10] - hist11[10]) + abs(hist[11] - hist11[11]) + abs(hist[12] - hist11[12]) + abs(hist[13] - hist11[13]) + abs(hist[14] - hist11[14]) + abs(hist[15] - hist11[15]) + abs(hist2[1] - hist22[1]) + abs(hist2[2] - hist22[2]) + abs(hist2[3] - hist22[3]) + abs(hist2[4] - hist22[4]) + abs(hist2[5] - hist22[5]) + abs(hist2[6] - hist22[6]) + abs(hist2[7] - hist22[7]) + abs(hist2[8] - hist22[8]) + abs(hist2[9] - hist22[9]) + abs(hist2[10] - hist22[10]) + abs(hist2[11] - hist22[11]) + abs(hist2[12] - hist22[12]) + abs(hist2[13] - hist22[13]) + abs(hist2[14] - hist22[14]) + abs(hist2[15] - hist22[15]); //+ abs(gave0 - gave_0) + abs(gave1 - gave_1) + abs(gave2 - gave_2) + abs(gave3 - gave_3) + abs(gave4 - gave_4) + abs(gave5 - gave_5) + abs(gave6 - gave_6) + abs(gave7 - gave_7) + abs(gave8 - gave_8) + abs(gave9 - gave_9) + abs(gave10 - gave_10) + abs(gave11 - gave_11) + abs(gave12 - gave_12) + abs(gave13 - gave_13) + abs(gave14 - gave_14) + abs(gave15 - gave_15)

		
		// HOG
		candicate = abs(ave0 - ave_0) + abs(ave1 - ave_1) + abs(ave2 - ave_2) + abs(ave3 - ave_3) + abs(ave4 - ave_4) + abs(ave5 - ave_5) + abs(ave6 - ave_6) + abs(ave7 - ave_7) + abs(ave8 - ave_8);



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


		// ------------------------------ 針對 ROI 進行 三個區段的膚色掃描 ------------------------------

		int skin_det = 0; // 掃描膚色門檻值

		for (int i = m + (P2_x - P1_x) / 5; i<m + (P2_x - P1_x) - (P2_x - P1_x) / 3 && i < width_m; i++)
            if ( YCbCr_Cr [ i ] [ ( int ) ( n + ( P2_y - P1_y )*0.25 ) ] > 133 &&
             YCbCr_Cr [ i ] [ ( int ) ( n + ( P2_y - P1_y )*0.25 ) ] < 174 &&
             YCbCr_Cb [ i ] [ ( int ) ( n + ( P2_y - P1_y )*0.25 ) ] > 77 &&
             YCbCr_Cb [ i ] [ ( int ) ( n + ( P2_y - P1_y )*0.25 ) ] < 127 )
			skin_det++;

		for (int i = m + (P2_x - P1_x) / 5; i<m + (P2_x - P1_x) - (P2_x - P1_x) / 3 && i < width_m; i++)
            if ( YCbCr_Cr [ i ] [ ( int ) ( n + ( P2_y - P1_y )*0.5 ) ] > 133 &&
                 YCbCr_Cr [ i ] [ ( int ) ( n + ( P2_y - P1_y )*0.5 ) ] < 174 &&
                 YCbCr_Cb [ i ] [ ( int ) ( n + ( P2_y - P1_y )*0.5 ) ] > 77 &&
                 YCbCr_Cb [ i ] [ ( int ) ( n + ( P2_y - P1_y )*0.5 ) ] < 127 )
			skin_det++;

		for (int i = m + (P2_x - P1_x) / 5; i<m + (P2_x - P1_x) - (P2_x - P1_x) / 3 && i < width_m; i++)
            if ( YCbCr_Cr [ i ] [ ( int ) ( n + ( P2_y - P1_y )*0.75 ) ] > 133 &&
                 YCbCr_Cr [ i ] [ ( int ) ( n + ( P2_y - P1_y )*0.75 ) ] < 174 &&
                 YCbCr_Cb [ i ] [ ( int ) ( n + ( P2_y - P1_y )*0.75 ) ] > 77 &&
                 YCbCr_Cb [ i ] [ ( int ) ( n + ( P2_y - P1_y )*0.75 ) ] < 127 )
			skin_det++;

		// ------------------------------------- ROI 掃描結束 -----------------------------------------

        // if ( candicate < eligibility && DiffFramecount >= 1 && candicateLBP < eligibilityLBP * 1.2 && skin_det >= 0 )
        if ( candicate < eligibility && DiffFramecount >= 1 && skin_det >= 1 && candicateLBPBetter <= eligibilityLBP * 2.8 )
		{
			
            // canny 
            //canny = cvCreateImage( frame_320x240_size, IPL_DEPTH_8U, 1 ); // 做 canny 測邊
            //cvCanny( canny, canny, 50, 150, 3 );
            //cvShowImage( "canny_Tra", canny );

            // canny end!!!!!!!!

            cooooount++;
            candicateLBPBetter1 += candicateLBPBetter;


            inFile << "--------------------------------------------平均*" << candicateLBPBetter1 / cooooount 
                << "(" << candicateLBPBetter1 << " / " << cooooount << ")" << endl ;

            inFile << "-------------------------------------------------" << candicateLBPBetter << endl;
            inFile << "----------------------------------------------------------" << endl;
            for ( int numm = 0; numm < ( sizeof( g_LBPbetter ) / sizeof( g_LBPbetter [ 0 ] ) ) + 1; numm ++ ) {

                if ( numm != ( sizeof( g_LBPbetter ) / sizeof( g_LBPbetter [ 0 ] ) ) )
                    inFile << "[" << g_LBPbetter [ numm ] << "]:" << g_LBPbetterHist2 [ g_hashtransLBPtoHist [ g_LBPbetter [ numm ] ] ]
                    << " ";

                else
                    inFile << "[" << 0 << "]:" << g_LBPbetterHist2 [ 0 ] << endl;

                if ( numm != 0 && numm % 5 == 0 ) inFile << endl;

            } // for
            inFile << "----------------------------------------------------------" << endl;



            // 以下測試LBP用 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            //for ( int numm1 = 0; numm1 < 256; numm1 ++ ) 
            //    g_LBPbetterHisttemp [ numm1 ] = g_LBPbetterHist2 [ numm1 ] ;
            //inFile << "----------------------------------------------------------" << endl;
            //for ( int numm1 = 0; numm1 < 256; numm1 ++ ) {

            //    int max = numm1;

            //    for ( int numm2 = 1; numm2 < 256; numm2 ++ ) {
            //        
            //        if ( g_LBPbetterHisttemp [ max ] < g_LBPbetterHisttemp [ numm2 ] ) {
            //            
            //            max = numm2 ;

            //        } // 

            //    } // for


            //    inFile << "[" << max << "]:" << g_LBPbetterHisttemp [ max ] << endl;
            //    g_LBPbetterHisttemp [ max ] = -1 ;

            //} // for
            //inFile << "----------------------------------------------------------" << endl;

            //for ( int nnnn = 0 ; nnnn < 256 ; nnnn ++ ) {
            //    
            //    if ( nnnn % 5  == 0 ) cout << endl ;
            //    cout << g_LBPbetterHist2 [nnnn] << " ";

            //} // for

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
			// eligibilityLBP = candicateLBP; 

             eligibilityLBP = candicateLBPBetter;// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

			disbetweenfingertip = disbetweenfingertip2;

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
			
			/*gthistogram0 = ghistogram0;
			gthistogram1 = ghistogram1;
			gthistogram2 = ghistogram2;
			gthistogram3 = ghistogram3;
			gthistogram4 = ghistogram4;
			gthistogram5 = ghistogram5;
			gthistogram6 = ghistogram6;
			gthistogram7 = ghistogram7;
			gthistogram8 = ghistogram8;
			gthistogram9 = ghistogram9;
			gthistogram10 = ghistogram10;
			gthistogram11 = ghistogram11;
			gthistogram12 = ghistogram12;
			gthistogram13 = ghistogram13;
			gthistogram14 = ghistogram14;
			gthistogram15 = ghistogram15;*/

			//hist[1] = hist11[1];
			//hist[2] = hist11[2];
			//hist[3] = hist11[3];
			//hist[4] = hist11[4];
			//hist[5] = hist11[5];
			//hist[6] = hist11[6];
			//hist[7] = hist11[7];
			//hist[8] = hist11[8];
			//hist[9] = hist11[9];
			//hist[10] = hist11[10];
			//hist[11] = hist11[11];
			//hist[12] = hist11[12];
			//hist[13] = hist11[13];
			//hist[14] = hist11[14];
			//hist[15] = hist11[15];
			//

		/*	hist[1] = hist2[1];
			hist[2] = hist2[2];
			hist[3] = hist2[3];
			hist[4] = hist2[4];
			hist[5] = hist2[5];
			hist[6] = hist2[6];
			hist[7] = hist2[7];
			hist[8] = hist2[8];*/
			
            for ( int numm = 0 ; numm < ( sizeof( g_LBPbetter ) / sizeof( g_LBPbetter [ 0 ] ) ) + 1 ; numm ++ ) {

                g_LBPbetterAve1 [ numm ] = g_LBPbetterAve2 [ numm ] ;

            } // for

			//for (int i = 0; i < 256; i++) {

			//	hist[i] = hist2[i];

			//} // for


			// 找到第 n 個，符合ROI特徵值的候選區塊，候選區塊的數量又多累計一個。
			countOfFrameCandidate++;

		}


	} // for m*n

	P2_x = Px + (P2_x - P1_x);
	P2_y = Py + (P2_y - P1_y);
	P1_x = Px;
	P1_y = Py;

	cout << countTheZeroCandidateFrame << "***************候選區塊數量*********(" << countOfFrameCandidate << endl;

	// 不斷的候選區塊為 0 (找不到候選區塊)，將被判定為「卡無窮迴圈」
	if (countOfFrameCandidate == 0) {
		
		countTheZeroCandidateFrame++; // 預計不能連續超過 20 次的「找不到候選區塊」

	} // if

	else {
		
		countTheZeroCandidateFrame = 0;

	} // else

	Sobelcount = 0;
	//cout<<P2_x<<" "<<P2_y<<endl;
	if (eligibility > 0.25 )   //追丟回原位
	{


		////cout<<"click ";
		////防止握拳ROI下移
		P2_x = PreP1x + (P2_x - P1_x);
		P2_y = PreP1y + (P2_y - P1_y);
		P1_x = PreP1x;
		P1_y = PreP1y;

		for (int i = P1_x; i < P2_x; i++)
		{
			for (int j = P1_y + (P2_y - P1_y) / 2; j < P1_y + (P2_y - P1_y) * 3 / 4 && j < height_m ; j++)
			{
				Y[i][j] = abs(YCbCr_Y[i - 1][j - 1] * (-1) + YCbCr_Y[i][j - 1] * (-2) + YCbCr_Y[i + 1][j - 1] * (-1)
					+ YCbCr_Y[i - 1][j + 1] * 1 + YCbCr_Y[i][j + 1] * 2 + YCbCr_Y[i + 1][j + 1] * 1);

				Cb[i][j] = abs(YCbCr_Cb[i - 1][j - 1] * (-1) + YCbCr_Cb[i][j - 1] * (-2) + YCbCr_Cb[i + 1][j - 1] * (-1)
					+ YCbCr_Cb[i - 1][j + 1] * 1 + YCbCr_Cb[i][j + 1] * 2 + YCbCr_Cb[i + 1][j + 1] * 1);

				Cr[i][j] = abs(YCbCr_Cr[i - 1][j - 1] * (-1) + YCbCr_Cr[i][j - 1] * (-2) + YCbCr_Cr[i + 1][j - 1] * (-1)
					+ YCbCr_Cr[i - 1][j + 1] * 1 + YCbCr_Cr[i][j + 1] * 2 + YCbCr_Cr[i + 1][j + 1] * 1);


				SobelFrame[i][j] = Y[i][j] + Cb[i][j] + Cr[i][j];
				if (SobelFrame[i][j] > 50 && YCbCr_Cr[i][j] > 133 && YCbCr_Cr[i][j] < 174 && YCbCr_Cb[i][j] > 77 && YCbCr_Cb[i][j] < 127)
					Sobelcount++;
				//else
				//SobelFrame[i][j] = 0;


			}
		}

		if (abs(P1_x - PreP1x) < 4 && abs(P1_y - PreP1y) < 4 && wait<0 && Sobelcount - Sobelcount2>30) {

			click_count++;

			//cout << "+++++++++++++++++++++++++++++++++++++++" << endl;
		} // if



	}


	// 進入辨識階段
	//if (click_count == 5)
	//{//cout<<"Sobelcount: "<<Sobelcount<<endl;
	//	cout<<"Sobelcount2: "<<Sobelcount2<<endl;
	//	click_count = 0;
	//	cout<<"click"<<endl;
	//	flag = 3;
	//	click_flag = 1;

	//	for (int i = 0; i<P2_x - P1_x + 8; i++)
	//	for (int j = 0; j<P2_y - P1_y; j++)
	//	{
	//		fistY[P1_x + i][P1_y + j] = YCbCr_Y[P1_x + i][P1_y + j];
	//		fistCb[P1_x + i][P1_y + j] = YCbCr_Cb[P1_x + i][P1_y + j];
	//		fistCr[P1_x + i][P1_y + j] = YCbCr_Cr[P1_x + i][P1_y + j];
	//		fist_x = P1_x;
	//		fist_y = P1_y;
	//	}
	//	wait = 20;
	//}


	if (wait<0)
		wait = 0;
	//if (trackrange<8000 && trackrange>3000)
	//新舊P1相差距離
	if (abs(Px - PreP1x)>0 || abs(Py - PreP1y)>0)
	{

		P1 = cvPoint(P1_x * 2, P1_y * 2);
		P2 = cvPoint(P2_x * 2, P2_y * 2);
	}

}