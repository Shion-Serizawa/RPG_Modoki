#include "DxLib.h"

typedef struct {
	int hp;
	int mp;
	int atk;//攻撃力
	int agi;//素早さ
	char name[13];
	char flag;
}mainca,*p_mainca;

typedef struct {
	int hp;
	int atk;//攻撃力
	int agi;//素早さ
	int def;//防御
	char name[42];
	char flag;
	int atkturn;
	int damage;
	
}enemy,*p_enemy;

//攻撃順ソート
int* agisort(mainca* main, enemy* ene, int* agi, int* num, int totalsize);
int dieagisort(int*, int*, int*, mainca* main, enemy* ene);

//技関係
int numfacter(p_mainca mai, p_enemy ene, int num, int select);
int herotec(p_mainca mai, p_enemy ene, int num, int select, int font);
int montec(p_mainca mai, p_enemy ene, int num, int select, int font);
int wittec(p_mainca mai, p_enemy ene, int num, int select, int font);
int lantec(p_mainca mai, p_enemy ene, int num, int select, int font);

int ene1atk(p_mainca mai, p_enemy ene, int*, int*, int*, int*);


// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	//Log.txtの表示(TRUE), 非表示(FALSE)
	SetOutApplicationLogValidFlag(FALSE);

	//Windowモード
	ChangeWindowMode(FALSE);

	//Windowタイトル
	SetMainWindowText("RPG風ゲーム");

	//画面モードの設定
	SetGraphMode(1920, 1080, 16);

	//ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1) {
		// エラーが起きたら直ちに終了
		return -1;
	}

	// マウスを表示状態にする
	SetMouseDispFlag(TRUE);

	//描画先を裏画面にする
	SetDrawScreen(DX_SCREEN_BACK);

	//画像のロード
	int bgImg = LoadGraph("img/pipo002.png");
	int enemyImg[2];
	enemyImg[0] = LoadGraph("img/enemy3.png");
	enemyImg[1] = LoadGraph("img/enemy3.png");

	int playerImg[4];
	playerImg[0] = LoadGraph("img/hero.png");
	playerImg[1] = LoadGraph("img/monk.png");
	playerImg[2] = LoadGraph("img/Witch.png");
	playerImg[3] = LoadGraph("img/Lan.png");

	int iconImg[12];
	iconImg[0] = LoadGraph("img/hero1.png");
	iconImg[1] = LoadGraph("img/hero2.png");
	iconImg[2] = LoadGraph("img/hero3.png");

	iconImg[3] = LoadGraph("img/monk1.png");
	iconImg[4] = LoadGraph("img/monk2.png");
	iconImg[5] = LoadGraph("img/monk3.png");

	iconImg[6] = LoadGraph("img/witch1.png");
	iconImg[7] = LoadGraph("img/witch2.png");
	iconImg[8] = LoadGraph("img/witch3.png");

	iconImg[9] = LoadGraph("img/Lan1.png");
	iconImg[10] = LoadGraph("img/Lan2.png");
	iconImg[11] = LoadGraph("img/Lan3.png");

	int ed1 = LoadGraph("img/die.png");
	int ed2 = LoadGraph("img/clear.png");

	//キャラ初期値
	mainca maincara[4];
	//p_mainca p_maincara = maincara;
	maincara[0] = { 50,50,50,54,"ゆうしゃ",1 };
	maincara[1] = { 50,50,50,51,"そうりょ",2 };
	maincara[2] = { 58,50,50,51,"うぃっち",3 };
	maincara[3] = { 50,50,50,52,"ランサー",4 };

	//キャラ生死フラグ
	int dieflag = 0;

	//敵キャラ初期値
	enemy enemyda[3];
	enemyda[0] = { 500,60,53,53,"討伐予定のかんぶ",11,0,0 };
	enemyda[1] = { 200,40,60,40,"討伐予定のかんぶ",12,0,0 };
	enemyda[2] = { 200,40,60,40,"討伐予定のかんぶ",13,0,0 };


	///敵のHP(仮)/////////////////////////////////////////////////////////////
	int enehp = (enemyda)->hp;
	int eneatk = enemyda->atk;
	/////////////////////////////////////////////////////////////////////////



	//戦闘システムフラグ
	int sysflag = 0;

	//素早さソート用
	int agility[6];
	int aginum[6];
	int counter = 0;
	int agiflag = 0;
	int totalmen = 0;
	int carasize = 0;


	//フォント設定
	int FontHandle = LoadFontDataToHandle("font.dft", 2);

	//プレイヤーステータスの設定
	int px = 8;
	int py = 650;
	int pw = 470;
	int ph = 280;

	//文字列の横幅を測る変数
	int DrawWidth = 0;

	//マウスの座標を格納する変数
	int mouseX = 0, mouseY = 0;

 	//コマンドセレクトのフラグ
	int selectFlag = 0;
	int mouseFlag = 0;
	int comandFlag = 0;
	int coment = 0;

	//炎上効果　一人の攻撃ごとに
	int turnstore = 0;

	//バッファ
	char buf[70];
	int hpmp;

	//無限ループ
	while (dieflag == 0) {
		
		//画面に描かれているものを一回全部消す
		ClearDrawScreen();

		//背景を表示
		DrawGraph(0, 0, bgImg, FALSE);

		//地名を表示する
		DrawWidth = GetDrawStringWidthToHandle("旅立ちの草原", strlen("旅立ちの草原"), FontHandle, FALSE);
		DrawStringToHandle((520 - DrawWidth) / 2, 20, "旅立ちの草原", GetColor(255, 255, 255), FontHandle, GetColor(0, 0, 0));

		//モンスターを表示
		DrawGraph((1920 - 300) / 2, 150, enemyImg[0], TRUE);//Falseにすれば調整しやすいね

		//コメントゲージの表示
		DrawBox(5, 935, 1915,1075, GetColor(0, 0, 0), TRUE);
		DrawBox(7, 937, 1913, 1073, GetColor(255, 255, 0), TRUE);
		DrawBox(11, 941, 1909, 1069, GetColor(0, 0, 0), TRUE);
		DrawBox(13, 943, 1907, 1067, GetColor(102, 205, 170), TRUE);

		//プレイヤーステータス表示
		for (int i = 0; i < 4; i++) {
			//char* buf;
			//半透明の黒ウインドウ
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
			DrawBox(px + (pw + 8) * i, py, px + (pw + 8) * i + pw, py + ph, GetColor(0, 0, 0), TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//全体の設定を戻す

			//プレイヤーを表示
			DrawGraph(px + 8 + (pw + 8) * i, py + 20, playerImg[i], TRUE);

			//プレイヤー名前を表示
			DrawWidth = GetDrawStringWidthToHandle((maincara + i)->name, strlen((maincara + i)->name), FontHandle, FALSE);
			DrawStringToHandle((pw + 8) * i + (px + 240 + pw - DrawWidth) / 2, 660, (maincara + i)->name, GetColor(255, 255, 255), FontHandle, GetColor(0, 0, 0));
		}
		if (agiflag == 0) {
			counter = 0;
			totalmen = 0;

			int state = 0;
			int store;
			int die = 0;

			for (int i = 0; i < 6; i++) {
				agility[i] = 0;
				aginum[i] = 0;
			}

			for (int i = 0; i < 6; i++) {
				if (state == 0) {
					if ((maincara + i)->hp > 0) {
						agility[i - die] = (maincara + i)->agi;
						aginum[i - die] = (maincara + i)->flag;
						//carasize++;
						totalmen++;
						
					}
					else if (i >= 4) {
						state = 1;
						store = i;

					}
					else {
						die++;
					}

				}
				
				if (state == 1) {
					

					if ((enemyda  )->hp > 0) {
						agility[i - die] = enemyda->agi;
						aginum[i - die] = enemyda->flag;
						totalmen++;
						
					}
					else {
						agility[i] = 5;
					}
					state = 2;

				}else if (state == 2) 
				{
					agility[i] = 0;
					aginum[i] = 0;
					
				}
			}
			agisort(maincara, enemyda, agility, aginum, totalmen);
			agiflag = 1;

			
		}

		if (aginum[counter] < 10) {


			if (coment != 3) {
				coment = 1;
			}

			//マウスの左ボタンが押されているか////////////////////////////////////////////////////////関数無理でちゅ
			if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0 && mouseFlag == 0 && agiflag == 1) {		// 押されている
				// マウスの位置を取得
				GetMousePoint(&mouseX, &mouseY);
				if (px <= mouseX && mouseX <= px + pw && py <= mouseY && mouseY <= py + ph && aginum[counter] == 1) {
					selectFlag = 1;
				}
				if ((px + (pw + 8) * 1) <= mouseX && mouseX <= (px + (pw + 8) * 1) + pw && py <= mouseY && mouseY <= py + ph && aginum[counter] == 2) {
					selectFlag = 2;
				}
				if ((px + (pw + 8) * 2) <= mouseX && mouseX <= (px + (pw + 8) * 2) + pw && py <= mouseY && mouseY <= py + ph && aginum[counter] == 3) {
					selectFlag = 3;
				}
				if ((px + (pw + 8) * 3) <= mouseX && mouseX <= (px + (pw + 8) * 3) + pw && py <= mouseY && mouseY <= py + ph && aginum[counter] == 4) {
					selectFlag = 4;
				}

				if (selectFlag != 0)
					mouseFlag = 1;
			}


			/////攻撃用ボタン(仮)////////////////////////////////////////////////////////////////////////////////
			if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0 && mouseFlag == 1 && agiflag == 1) {	//ここ壊している	// 押されている
			// マウスの位置を取得
				GetMousePoint(&mouseX, &mouseY);


				if ((px + (pw + 8) * (selectFlag - 1)) <= mouseX && mouseX <= (px + (pw + 8) * (selectFlag - 1)) + 2 + 152 && 504 <= mouseY && mouseY < 650) {
					
					comandFlag = 1;

					if (!(numfacter(maincara, enemyda, selectFlag, comandFlag))) {
						mouseFlag = 0;
						counter++;
						sprintf_s(buf, 40, "%d",counter);
						DrawStringToHandle(33, 984, buf, GetColor(255, 255, 255), FontHandle, GetColor(0, 0, 0));
						//一時的に
						//ene1atk(maincara, enemyda);
						selectFlag = 0;
						comandFlag = 0;

					}
				}
				else if ((px + (pw + 8) * (selectFlag - 1)) + 4 + 152 <= mouseX && mouseX <= (px + (pw + 8) * (selectFlag - 1)) - 2 + (4 + 152) * 2 && 504 <= mouseY && mouseY < 650) {
				
					comandFlag = 2;

					if (!(numfacter(maincara, enemyda, selectFlag, comandFlag))) {
						mouseFlag = 0;
						counter++;
						selectFlag = 0;
						comandFlag = 0;
					}
				}
				else if ((px + (pw + 8) * (selectFlag - 1)) + 2 + (4 + 152) * 2 <= mouseX && mouseX <= (px + (pw + 8) * (selectFlag - 1)) - 2 + (4 + 152) * 3 && 504 <= mouseY && mouseY < 650) {

					comandFlag = 3;
					if (selectFlag == 3 || selectFlag == 4) {
						coment = 3;
					}

					if (!(numfacter(maincara, enemyda, selectFlag, comandFlag))) {

						mouseFlag = 0;
						counter++;

						selectFlag = 0;
						comandFlag = 0;
						coment = 0;

					}
				}
				////////////////////////////////////////////////////////まてよ
				if (enemyda->hp <= 0) {
					dieflag = 2;
				}
				else if (mouseFlag == 0 && counter == totalmen) {
					agiflag = 0;
					counter = 0;
				}

				///////////////////////////////////////////////////////

			}
		}
		else if (aginum[counter] > 10) {
			coment = 2;
			int maindie = ene1atk(maincara, enemyda,agility,aginum,&totalmen, &counter);

			

			if (maindie) {
				dieflag = 1;
			}
			if (counter == totalmen) {
				agiflag = 0;
				counter = 0;
			}
			else {
				counter++;
			}
		}
		

		for (int i = 0; i < 4; i++) {

			if ((maincara + i)->hp >= 0) {
				hpmp = (maincara + i)->hp;
			}
			else {
				hpmp = 0;
			}

			//HPを表示/////////////////////////////////////////////////
			sprintf_s(buf, 7, "%d", hpmp);
			DrawStringToHandle(px + (pw + 8) * i + 260, py + 70, "HP:", GetColor(255, 255, 255), FontHandle, GetColor(0, 0, 0));
			DrawWidth = GetDrawStringWidthToHandle(buf, strlen(buf), FontHandle, FALSE);
			DrawStringToHandle(px + (pw + 8) * i + 450 - DrawWidth, py + 70, buf, GetColor(255, 255, 255), FontHandle, GetColor(0, 0, 0));

			if ((maincara + i)->mp >= 0) {
				hpmp = (maincara + i)->mp;
			}
			else {
				hpmp = 0;
			}


			//char* buf;
			//MPを表示//////////////////////////////////////////////
			sprintf_s(buf, 7, "%d", (maincara + i)->mp);
			DrawStringToHandle(px + (pw + 8) * i + 255, py + 170, "MP:", GetColor(255, 255, 255), FontHandle, GetColor(0, 0, 0));
			DrawWidth = GetDrawStringWidthToHandle(buf, strlen(buf), FontHandle, FALSE);
			DrawStringToHandle(px + (pw + 8) * i + 450 - DrawWidth, py + 170, buf, GetColor(255, 255, 255), FontHandle, GetColor(0, 0, 0));
		}

		//turnダメージ(仮)///////////////////////////////////////////////////////////
		if ((enemyda->atkturn > 0) && (turnstore != selectFlag) && (selectFlag != 0)) {
			turnstore = selectFlag;
			enemyda->atkturn--;
			enemyda->hp -= enemyda->damage;


			if (enemyda->atkturn == 0) {
				enemyda->damage = 0;
			}
		}
		/////////////////////////////////////////////////////////////////////////////

		//モンスターゲージを表示
		DrawBox(((1920 - 300) / 2) + 20 - 2, 460 - 2, ((1920 - 300)) / 2 + 280 + 2, 480 + 2, GetColor(0, 0, 0), TRUE);
		if (enemyda->hp > 0) {
			DrawBox(((1920 - 300) / 2) + 20, 460, ((1920 - 300) / 2) + 20 + (260 * ((float)enemyda->hp / enehp)), 480, GetColor(255, 255, 0), TRUE);
		}


		//アイコンの表示//////////////////////////////////////////////////////////////////////////関数独立

		if (selectFlag != 0) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
			DrawBox((px + (pw + 8) * (selectFlag - 1)), 504, (px + (pw + 8) * (selectFlag - 1)) + pw, 650, GetColor(0, 0, 0), TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			DrawGraph((px + (pw + 8) * (selectFlag - 1)) + 2, 504, iconImg[(selectFlag - 1) * 3], TRUE);
			DrawGraph((px + (pw + 8) * (selectFlag - 1)) + 2 + 4 + 152, 504, iconImg[(selectFlag - 1) * 3 + 1], TRUE);
			DrawGraph((px + (pw + 8) * (selectFlag - 1)) + 2 + (4 + 152) * 2, 504, iconImg[(selectFlag - 1) * 3 + 2], TRUE);
		}
		//文字の入力部//////////////////////////////////////////////////////////////////////////(考えましょう。)
		if(sysflag == 0){
			DrawStringToHandle(33, 984, "討伐予定のかんぶが現れた。", GetColor(255, 255, 255), FontHandle, GetColor(0, 0, 0));
			ScreenFlip();
			WaitTimer(2000);
			sysflag = 1;
		}
		/*if (mouseFlag == 1)*/ {	//ここ壊している	// 押されている
		// マウスの位置を取得
			if(mouseFlag == 1)
			GetMousePoint(&mouseX, &mouseY);


			if ((px + (pw + 8) * (selectFlag - 1)) <= mouseX && mouseX <= (px + (pw + 8) * (selectFlag - 1)) + 2 + 152 && 504 <= mouseY && mouseY < 650 && coment == 1 && mouseFlag == 1) {
				if (selectFlag == 1) {
					sprintf_s(buf, 70, "HP:  0 MP:  0 剣で単体攻撃");
					DrawStringToHandle(33, 984, buf, GetColor(255, 255, 255), FontHandle, GetColor(0, 0, 0));
				}

				if (selectFlag == 2) {
					sprintf_s(buf, 70, "HP:  0 MP:-35 仏の力で少し弱めの攻撃");
					DrawStringToHandle(33, 984, buf, GetColor(255, 255, 255), FontHandle, GetColor(0, 0, 0));
				}

				if (selectFlag == 3) {
					sprintf_s(buf, 70, "HP:  0 MP:  0 炎で単体攻撃　たまに炎上効果");
					DrawStringToHandle(33, 984, buf, GetColor(255, 255, 255), FontHandle, GetColor(0, 0, 0));
				}

				if (selectFlag == 4) {
					sprintf_s(buf, 70, "HP:  0 MP:  0 槍で単体攻撃　たまに大ダメージ");
					DrawStringToHandle(33, 984, buf, GetColor(255, 255, 255), FontHandle, GetColor(0, 0, 0));
				}

				//numfacter(maincara, enemyda, selectFlag, comandFlag);
				

			}
			else if ((px + (pw + 8) * (selectFlag - 1)) + 4 + 152 <= mouseX && mouseX <= (px + (pw + 8) * (selectFlag - 1)) - 2 + (4 + 152) * 2 && 504 <= mouseY && mouseY < 650 && coment == 1 && mouseFlag == 1) {
				if (selectFlag == 1) {
					sprintf_s(buf, 70, "HP:  0 MP:-40 敵の攻撃力を減らす");
					DrawStringToHandle(33, 984, buf, GetColor(255, 255, 255), FontHandle, GetColor(0, 0, 0));
				}

				if (selectFlag == 2) {
					sprintf_s(buf, 70, "HP:  0 MP:-35 低確率の即死チャレンジ");
					DrawStringToHandle(33, 984, buf, GetColor(255, 255, 255), FontHandle, GetColor(0, 0, 0));
				}

				if (selectFlag == 3) {
					sprintf_s(buf, 70, "HP:  0 MP:-65 敵に超攻撃");
					DrawStringToHandle(33, 984, buf, GetColor(255, 255, 255), FontHandle, GetColor(0, 0, 0));
				}

				if (selectFlag == 4) {
					sprintf_s(buf, 70, "HP:  0 MP:-30 自身の攻撃、素早さ増加");
					DrawStringToHandle(33, 984, buf, GetColor(255, 255, 255), FontHandle, GetColor(0, 0, 0));
				}

			}
			else if ((px + (pw + 8) * (selectFlag - 1)) + 2 + (4 + 152) * 2 <= mouseX && mouseX <= (px + (pw + 8) * (selectFlag - 1)) - 2 + (4 + 152) * 3 && 504 <= mouseY && mouseY < 650 && coment == 1 && mouseFlag == 1) {
				if (selectFlag == 1) {
					sprintf_s(buf, 70, "HP:  0 MP:-30 自身の素早さ増加");
					DrawStringToHandle(33, 984, buf, GetColor(255, 255, 255), FontHandle, GetColor(0, 0, 0));
				}

				if (selectFlag == 2) {
					sprintf_s(buf, 70, "HP:  0 MP:  0 何故か戦車な通常攻撃");
					DrawStringToHandle(33, 984, buf, GetColor(255, 255, 255), FontHandle, GetColor(0, 0, 0));
				}

				if (selectFlag == 3) {
					sprintf_s(buf, 70, "HP: 0? MP:-35 味方1体のHP回復 (対象をおしてね)");
					DrawStringToHandle(33, 984, buf, GetColor(255, 255, 255), FontHandle, GetColor(0, 0, 0));
				}

				if (selectFlag == 4) {
					sprintf_s(buf, 70, "HP:-20 MP: 0? 味方のMPを15回復(対象をおしてね)");
					DrawStringToHandle(33, 984, buf, GetColor(255, 255, 255), FontHandle, GetColor(0, 0, 0));
				}

			}
			else if (coment == 1) {
				sprintf_s(buf, 70, "%sの攻撃",(maincara + aginum[counter] - 1)->name);
				DrawStringToHandle(33, 984, buf, GetColor(255, 255, 255), FontHandle, GetColor(0, 0, 0));
			}
			else if (coment == 2) {
				sprintf_s(buf, 40, "%sの攻撃！", (enemyda)->name);
				DrawStringToHandle(33, 984, buf, GetColor(255, 255, 255), FontHandle, GetColor(0, 0, 0));

				// 裏画面の内容を表画面に反映させる
				ScreenFlip();


				// FPS 500/1000秒待つ
				WaitTimer(2000);//33ぐらいで60fpsくらい
			}
			else if (coment == 3) {
				sprintf_s(buf, 40, "使用する相手を選択してください。");
				DrawStringToHandle(33, 984, buf, GetColor(255, 255, 255), FontHandle, GetColor(0, 0, 0));
			}

		}

		if(dieflag == 1 ){
			//画面に描かれているものを一回全部消す
			ClearDrawScreen();

			//背景を表示
			DrawGraph(0, 0, ed1, FALSE);
			ScreenFlip();
			WaitKey();
			maincara[0] = { 50,50,50,54,"ゆうしゃ",1 };
			maincara[1] = { 50,50,50,51,"そうりょ",2 };
			maincara[2] = { 58,50,50,51,"うぃっち",3 };
			maincara[3] = { 50,50,50,52,"ランサー",4 };

			//キャラ生死フラグ
			dieflag = 0;

			//敵キャラ初期値
			enemyda[0] = { 500,60,53,53,"討伐予定のかんぶ",11,0,0 };
			enemyda[1] = { 200,40,60,40,"かたきうちの花",12,0,0 };
			enemyda[2] = { 200,40,60,40,"討伐予定のかんぶ",13,0,0 };

			//p_enemy penemyda = enemyda;



			//戦闘システムフラグ
			sysflag = 0;

			//素早さソート用
			counter = 0;
			agiflag = 0;
			totalmen = 0;
			carasize = 0;

			selectFlag = 0;
			mouseFlag = 0;
			comandFlag = 0;
			coment = 0;

			dieflag = 0;
		}

		if (dieflag == 2) {
			//画面に描かれているものを一回全部消す
			ClearDrawScreen();

			//背景を表示 勝利おめでとう
			DrawGraph(0, 0, ed2, FALSE);
			ScreenFlip();
			WaitKey();
			break;
		}
		////////////////////////////////////////////////////////////////////////////////////////

		// 裏画面の内容を表画面に反映させる
		ScreenFlip();


		// FPS 500/1000秒待つ
		WaitTimer(66);//33ぐらいで60fpsくらい
		// Windows システムからくる情報を処理する
		if (ProcessMessage() == -1) break;
		// ESCキーが押されたら無限ループから抜ける
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) break;

	}

	// キー入力待ち
	WaitKey();
	// ＤＸライブラリ使用の終了処理
	DxLib_End();
	// ソフトウェアの終了
	return 0;
}

//攻撃順ソート
int* agisort(mainca *main, enemy *ene, int *agi, int *num, int totalsize) {
	int i, j , buf;


	for (i = 0; i < totalsize; i++) {
		for (j = totalsize - 1; j > i; j--) {
			if (agi[j] > agi[j - 1]) {

				buf = *(agi + j - 1);
				*(agi + j - 1) = *(agi + j);
				*(agi + j) = buf;
				buf = *(num + j - 1);
				*(num + j - 1) = *(num + j);
				*(num + j) = buf;

			}
		}
	}
	return 0;
}

int dieagisort(int agility[6], int aginum[6], int *totalmen, mainca* main, enemy* ene) {
	*(totalmen )= 0;
	//carasize = 0;

	int state = 0;
	int store;
	int die = 0;
	int total = 0;

	for (int i = 0; i < 6; i++) {
		agility[i] = 0;
		aginum[i] = 0;
	}

	for (int i = 0; i < 6; i++) {
		if (state == 0) {
			if ((main + i)->hp > 0) {
				agility[i - die] = (main + i)->agi;
				aginum[i - die] = (main + i)->flag;
				//carasize++;
				total++;

			}
			else if (i >= 4) {
				state = 1;
				store = i;

			}
			else {
				die++;
			}

		}

		if (state == 1) {


			if ((ene /*+ i - store*/)->hp > 0) {
				agility[i - die] = ene->agi;
				aginum[i - die] = ene->flag;
				total++;

			}
			else {
				agility[i] = 5;
			}
			state = 2;

		}
		else if (state == 2)
		{
			agility[i] = 0;
			aginum[i] = 0;

		}
	}
	agisort(main, ene, agility, aginum, total);
	*(totalmen) = total;
	return 0;
}

//技関数を取捨選択
int numfacter(p_mainca mai, p_enemy ene, int num, int select) {
	int FontHandle = LoadFontDataToHandle("font.dft", 2);
	int use = 0;

	if (num == 1) {
		use = herotec(mai, ene, num, select, FontHandle);
	}
	if (num == 2) {
		use = montec(mai, ene, num, select, FontHandle);
	}
	if (num == 3) {
		use = wittec(mai, ene, num, select, FontHandle);
	}
	if (num == 4) {
		use = lantec(mai, ene, num, select, FontHandle);
	}

	return use;
}

int herotec(p_mainca mai, p_enemy ene, int num, int select, int font) {

	if(select == 1){
		ene->hp -= (mai + num - 1)->atk;

	}
	if (select == 2) {

	}
	if (select == 3) {
		if ((mai + num - 1)->mp < 30) {
			return 1;
		}
		(mai + num - 1)->mp -= 30;

		(mai + num - 1)->agi += 30;
	}
	return 0;
}
int montec(p_mainca mai, p_enemy ene, int num, int select, int font) {

	if (select == 1) {
		if ((mai + num - 1)->mp < 20) {
			return 1;
		}
		(mai + num - 1)->mp -= 20;
		ene->hp -= (mai + num - 1)->atk * 0.6;
	}
	if (select == 2) {
		if ((mai + num - 1)->mp < 35) {
			return 1;
		}
		(mai + num - 1)->mp -= 35;//元データを使いたい
		if (GetRand(99) == 1) {
			ene->hp -= 5000;
		}
	}
	if (select == 3) {
		ene->hp -= (mai + num - 1)->atk;
	}
	return 0;
}
int wittec(p_mainca mai, p_enemy ene, int num, int select, int font) {
	int px = 8;
	int py = 650;
	int pw = 470;
	int ph = 280;
	int mouseX, mouseY;
	int selectFlag = 0;

	if (select == 1) {

		ene->hp -= (mai + num - 1)->atk * 0.8;
		if (GetRand(9) == 1) {
			//ターンダメージ
			ene->atkturn = 3;
			ene->damage = 5;
		}

	}
	if (select == 2) {
		if ((mai + num - 1)->mp < 65) {
			return 1;
		}
		(mai + num - 1)->mp -= 65;
		ene->hp -= 500;
	}
	if (select == 3) {
		if ((mai + num - 1)->mp < 30) {
			return 1;
		}
		(mai + num - 1)->mp -= 30;
		while (1) {
			if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0 ) {		// 押されている
			// マウスの位置を取得
				GetMousePoint(&mouseX, &mouseY);
				if (px <= mouseX && mouseX <= px + pw && py <= mouseY && mouseY <= py + ph) {
					selectFlag = 1;
				}
				if ((px + (pw + 8) * 1) <= mouseX && mouseX <= (px + (pw + 8) * 1) + pw && py <= mouseY && mouseY <= py + ph) {
					selectFlag = 2;
				}
				if ((px + (pw + 8) * 2) <= mouseX && mouseX <= (px + (pw + 8) * 2) + pw && py <= mouseY && mouseY <= py + ph) {
					selectFlag = 3;
				}
				if ((px + (pw + 8) * 3) <= mouseX && mouseX <= (px + (pw + 8) * 3) + pw && py <= mouseY && mouseY <= py + ph) {
					selectFlag = 4;
				}
				
				if (selectFlag != 0) {
					break;
				}
				
			}
		}
		WaitTimer(200);
		(mai + selectFlag - 1)->hp += 15;

	}
	return 0;
}
int lantec(p_mainca mai, p_enemy ene, int num, int select, int font) {
	if (select == 1) {
		if (GetRand(1000) == 1) {
			ene->hp -= 300;
		}
		else {
			ene->hp -= (mai + num - 1)->atk * 0.8;
		}
	}
	if (select == 2) {
		if ((mai + num - 1)->mp < 30) {
			return 1;
		}

		(mai + num - 1)->mp -= 30;
		(mai + num - 1)->atk += 15;
		(mai + num - 1)->agi += 15;
	}
	if (select == 3) {
		int px = 8;
		int py = 650;
		int pw = 470;
		int ph = 280;
		int mouseX, mouseY;
		int selectFlag = 0;

		if ((mai + num - 1)->hp < 20) {
			return 1;
		}
		(mai + num - 1)->hp -= 20;

		while (1) {
			if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {		// 押されている
			// マウスの位置を取得
				GetMousePoint(&mouseX, &mouseY);
				if (px <= mouseX && mouseX <= px + pw && py <= mouseY && mouseY <= py + ph) {
					selectFlag = 1;
				}
				if ((px + (pw + 8) * 1) <= mouseX && mouseX <= (px + (pw + 8) * 1) + pw && py <= mouseY && mouseY <= py + ph) {
					selectFlag = 2;
				}
				if ((px + (pw + 8) * 2) <= mouseX && mouseX <= (px + (pw + 8) * 2) + pw && py <= mouseY && mouseY <= py + ph) {
					selectFlag = 3;
				}
				if ((px + (pw + 8) * 3) <= mouseX && mouseX <= (px + (pw + 8) * 3) + pw && py <= mouseY && mouseY <= py + ph) {
					selectFlag = 4;
				}

				if (selectFlag != 0) {
					break;
				}

			}
		}
		WaitTimer(200);
		(mai + selectFlag - 1)->mp += 15;
	}

	return 0;
}

int ene1atk(p_mainca mai, p_enemy ene, int agility[6], int aginum[6], int* totalmen, int *counter) {
	int target = GetRand(4);

		while (target == 0 || (mai + target - 1)->hp <= 0) {
			target = GetRand(4);
		}
	(mai + target -1)->hp -= ene->atk;

	if ((mai + target - 1)->hp <= 0) {
		dieagisort(agility, aginum, totalmen, mai, ene);
		if (*totalmen == 1) {
			return 1;
		}
		else if ((mai + target - 1)->agi > ene->agi) {
			*(counter) = *(counter)-1;
		}
	}
	


	return 0;
}