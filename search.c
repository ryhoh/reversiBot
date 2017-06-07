// Copyright (c) 2017 Tetsuya Hori
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

//  Created by 堀 哲也 on 2017/06/02.
//  Copyright © 2017年 堀 哲也. All rights reserved.

// リバーシBOTの、石を置く場所を選ぶ動作を記述

#include <stdio.h>
#include "system.h"
#include "search.h"

// -------------------------------------------先読み（探索）アルゴリズム
// 先読みに限度がある可能性から、幅優先探索で先読みを行う
// 選んだ座標をgridに入れる
/*int wideSearch(int board[][BOARDSIZE], int grid[2], int color){
	// in progress...
}*/

// 深さに限度を設けて、深さ優先探索で先読みを行う
// 選んだ座標をgridに入れる
// どこにも置けない時0を返す
/*int deepSearch(int board[][BOARDSIZE], int grid[2], int color){
	METHOD stack[SIZE];		// 調べる座標を入れるスタック
	METHOD *pStack = NULL;			// スタックポインタ　スタックが空の場合はNULLを指す
	int max = 0;
	
	// 現在の盤面から打てる手をプッシュする
	for(i = 0; i < BOARDSIZE; i++){
		for(j = 0; j < BOARDSIZE; j++){
			grid[0] = i;
			grid[1] = j;
			if(check(board, grid, color, 0) != 0)
				// (i, j)は実行可能
				push(board, pStack, grid);
		}
	}
	
	// ポップして探索する
	while(1){
		// ポップできなくなったら終了
		if(pStack == NULL)
			break;
		
		// ポップ
		grid[0] = pStack->grid[0];
		grid[1] = pStack->grid[1];
		// in progress..
	}
	
	return max;
}*/

// -------------------------------------------指し手評価関数
// 自らに有利な位置を高く評価するアルゴリズム
// 選んだ座標をgridに入れ、評価値を返す
// 評価値は、置ける場所が1つもない場合は0
// 良くも悪くもないマスは取れる石の数+1を、
// 置きたくないマスは1を、
// 自らに有利なマスは取れる石の数より高い値
int evaluate(int board[][BOARDSIZE], int grid[2], int color){
	int max = 0, tmpGrid[2];
	int i, j, tmp;
	
	// マス1つ1つをチェック
	for(i = 0; i < BOARDSIZE; i++){
		for(j = 0; j < BOARDSIZE; j++){
			tmpGrid[0] = i;
			tmpGrid[1] = j;
			/* 評価値を計算する */
			tmp = check(board, tmpGrid, color, 0);
			if(tmp == 0)
				continue;	// 置けないので無視
			
			// 四隅を最重要視する
			if((i == 0 && j == 0) || (i == BOARDSIZE-1 && j == BOARDSIZE-1))
				tmp += 100;
			// 四隅の周囲は置くと危ない
			else if(((i + j) == 1) || (i == 1 && j == 1) || ((i + j) == 13) || ((i + j) == 13) || ((i - j) == 6) || (i == 6 && j == 1) || ((j - i) == 6) || (i == 1 && j == 6))
				tmp = 1;
			// それ以外の外周はそれなりに重要視する
			else if(i == 0 || j == 0 || i == BOARDSIZE-1 || j == BOARDSIZE-1)
				tmp += 10;
			// どれにも該当しないならば取れる石の数+1にする
			else					// これは、置きたくないマスの評価値と
				tmp += 1;		// 明確に区別するためである
			
			if(max < tmp){
				// もっと評価値が高い場所を見つけた
				max = tmp;
				// 座標を設定
				grid[0] = i;
				grid[1] = j;
			}
		}
	}
	
	return max;
}

// -------------------------------------------ひっくり返す石の数で判断するアルゴリズム
// 選んだ座標をgridに入れ、その場合に取れる石の数を返す
// 置ける場所が1つもない場合は0を返す

// ひっくり返す石の数が最大になるように選ぶ
int selectMax(int board[][BOARDSIZE], int grid[2], int color){
	int max = 0, tmpGrid[2];
	int i, j, tmp;
	
	// マス1つ1つをチェック
	for(i = 0; i < BOARDSIZE; i++){
		for(j = 0; j < BOARDSIZE; j++){
			tmpGrid[0] = i;
			tmpGrid[1] = j;
			if(max < (tmp = check(board, tmpGrid, color, 0))){
				// もっと多く取れる場所を見つけた
				max = tmp;
				// 座標を設定
				grid[0] = i;
				grid[1] = j;
			}
		}
	}
	
	return max;
}

// ひっくり返す石の数が最小になるように選ぶ
int selectMin(int board[][BOARDSIZE], int grid[2], int color){
	int min = 255, tmpGrid[2];
	int i, j, tmp;
	
	// マス1つ1つをチェック
	for(i = 0; i < BOARDSIZE; i++){
		for(j = 0; j < BOARDSIZE; j++){
			tmpGrid[0] = i;
			tmpGrid[1] = j;
			if((tmp = check(board, tmpGrid, color, 0)) == 0)
				continue;	// 置けない場所なので無視
			if(min > tmp){
				// もっと少なく取れる場所を見つけた
				min = tmp;
				// 座標を設定
				grid[0] = i;
				grid[1] = j;
			}
		}
	}
	
	if(min == 255)
		return 0;	// 置ける場所が1つもない
	return min;
}

// -------------------------------------------アルゴリズム構成用の内部システム
// ここの関数を直接呼ばないこと

// スタックへのプッシュ
// 失敗したら1を返す
int push(METHOD stack[], METHOD *p, int var[2]){
	if(p == stack + SIZE - 1)
		return 1;	// これ以上プッシュできない
	
	if(p == NULL)	// 空のスタックに1つ目を追加する場合
		p = stack;
	
	p++;		// 次に移動
	p->grid[0] = var[0];
	p->grid[1] = var[1];
	
	return 0;
}

// 盤面の優勢、劣勢を判定する関数
// 優勢なら1, 劣勢なら0を返す
int situation(int board[][BOARDSIZE], int color){
	int i, j;
	int num[3] = {0};	// 石の数
	// num[0]は空白のマスの数、num[1]はBLACKの数、num[2]はWHITEの数
	
	// 色反転操作用の配列
	// flip[BLACK]はWHITEを、flip[WHITE]はBLACKを導くので、
	// 実際の色に関係なくflip[color]として必ず色を反転できる
	// flip[0]は使用しない
	const int flip[3] = {0, WHITE, BLACK};
	
	// 数える
	for(i = 0; i < BOARDSIZE; i++){
		for(j = 0; j < BOARDSIZE; j++){
			num[ board[i][j] ]++;
		}
	}
	
	return (num[color] > num[flip[color]]) ? 1 : 0;
}
