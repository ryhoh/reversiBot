// Copyright (c) 2017 Tetsuya Hori
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

//  Created by 堀 哲也 on 2017/06/02.
//  Copyright © 2017年 堀 哲也. All rights reserved.

// リバーシには必勝法がある（はずだ）が、人間の相手をするBOTを作ることが目的なので、必勝法のプログラム化はしない
// 学習機能(AI)は持たず、一定の手順に従って操作するだけ
#include <stdio.h>
#include <unistd.h>
#include "system.h"
#include "search.h"

#define DELAYTIME 1

// それらしい手順に従って、できるだけたくさんの石を取れる手を探す
// 先読みなしで、直ちに取れる石の数が最も多いものを選ぶ

int main(void){
	int board[BOARDSIZE][BOARDSIZE] = {0};
	int grid[2];
	int previousGetStones[2] = {1, 1};	// 先手, 後手の形で、最後に取った石の数を記録する
															// previousGetStones[0] == previousGetStones[1] == 0 の時、どちらも石を置けず、ゲーム終了
	int i, j, tmp;
	
	// ゲームの準備
	initialize(board);
	
	//board[5][5] = board[6][6] =  WHITE;
	/*for(i = 0; i < 5; i++){
		board[i][0] = board[i][4] = BLACK;
	}
	for(j = 0; j < 5; j++){
		board[0][j] = board[4][j] = BLACK;
	}
	
	for(i = 1; i < 4; i++){
		board[i][1] = board[i][3] = WHITE;
	}
	for(j = 1; j < 4; j++){
		board[1][j] = board[3][j] = WHITE;
	}*/
	
	// ゲーム開始
	// (まずはコンピュータ同士でやらせる)
	printf("ゲーム開始です\n");
	//printf("先手はあなたです\n");
	
	// お互いに置けなくなるまで置き続ける
	while(previousGetStones[0] != 0 || previousGetStones[1] != 0){
		previousGetStones[0] = playerAction(board, BLACK);
		sleep(DELAYTIME);
		
		if(previousGetStones[0] == 0 && previousGetStones[1] == 0)
			break;
		
		previousGetStones[1] = computerActionAlter(board, WHITE);
		sleep(DELAYTIME);
	}
	
	printf("ゲーム終了です\n結果は以下の通りです\n");
	printBoard(board);
	finalize(board);
	printf("終了するには何かキーを押してください\n");
	getchar();
	
	return 0;
}
