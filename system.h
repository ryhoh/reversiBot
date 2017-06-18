// Copyright (c) 2017 Tetsuya Hori
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

//  Created by 堀 哲也 on 2017/06/02.
//  Copyright © 2017年 堀 哲也. All rights reserved.

// リバーシBOTのゲーム進行用のシステムを記述

#define BOARDSIZE 8		// 1辺のマスの数

#define BLANC 0
#define BLACK 1
#define WHITE 2

// ゲームシステムの関数
void initialize(int board[][BOARDSIZE]);
void finalize(int board[][BOARDSIZE]);
int playerAction(int board[][BOARDSIZE], int color);
int update(int array[][BOARDSIZE], int grid[2], int color);
void printBoard(int board[][BOARDSIZE]);

// システムの内部動作を定義
int check(int array[][BOARDSIZE], int grid[2], int color, int mode);
