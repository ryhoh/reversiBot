// Copyright (c) 2017 Tetsuya Hori
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

//  Created by 堀 哲也 on 2017/06/02.
//  Copyright © 2017年 堀 哲也. All rights reserved.

// リバーシBOTの、BOTの振る舞いを記述

#define SIZE 1000

// 打つ手の座標、何手先かを記録する構造体
typedef struct _method{
	int grid[2];		// 座標
	int depth;		// 何手先か
} METHOD;


// コンピュータに操作させる場合の関数
int computerAction(int board[][BOARDSIZE], int color);
int computerActionAlter(int board[][BOARDSIZE], int color);

// マス選択アルゴリズム


// 指し手評価関数
int evaluate(int board[][BOARDSIZE], int grid[2], int color);
// 取れる石の数で判断
int selectMax(int array[][BOARDSIZE], int grid[2], int color);
int selectMin(int array[][BOARDSIZE], int grid[2], int color);

// 内部用関数
int push(METHOD stack[], METHOD *p, int var[2]);
int situation(int board[][BOARDSIZE], int color);
