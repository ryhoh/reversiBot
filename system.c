// Copyright (c) 2017 Tetsuya Hori
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

//  Created by 堀 哲也 on 2017/06/02.
//  Copyright © 2017年 堀 哲也. All rights reserved.

// リバーシBOTのゲーム進行用のシステムを記述

#include <stdio.h>
#include "system.h"
#include "search.h"

// 色反転操作用の配列
// flip[BLACK]はWHITEを、flip[WHITE]はBLACKを導くので、
// 実際の色に関係なくflip[color]として必ず色を反転できる
// flip[0]は使用しない
const int flip[3] = {0, WHITE, BLACK};

/* ----------------------------API : これらの関数を呼び出してゲームを進行する----------------------------- */

// 盤面の初期化
void initialize(int board[][BOARDSIZE]){
	int i, j;
	
	for(i = 0; i < BOARDSIZE; i++){
		for(j = 0; j < BOARDSIZE; j++){
			board[i][j] = 0;
		}
	}
	
	board[3][3] = board[4][4] = BLACK;
	board[3][4] = board[4][3] = WHITE;
}

// 結果を表示して終了する
void finalize(int board[][BOARDSIZE]){
	int i, j;
	int num[3] = {0};	// 石の数
	// num[0]は空白のマスの数、num[1]はBLACKの数、num[2]はWHITEの数
	
	// 数える
	for(i = 0; i < BOARDSIZE; i++){
		for(j = 0; j < BOARDSIZE; j++){
			num[ board[i][j] ]++;
		}
	}
	
	// 表示する
	printf("黒%d:白%dで%sの勝ちです\n", num[1], num[2], (num[1]>num[2]) ? "黒" : "白");
}

// プレイヤーに入力させ、石を置く
// ひっくり返した石の数を返す（置けなければ0）
int playerAction(int board[][BOARDSIZE], int color){
	int grid[2], num;
	int i, j, ableToPut = 0;
	
	// 盤面を表示する
	printBoard(board);
	
	// どこにも置けない場合（かどうかを判別する）の処理
	for(i = 0; i < BOARDSIZE; i++){
		for(j = 0; j < BOARDSIZE; j++){
			grid[0] = i;
			grid[1] = j;
			if(check(board, grid, color, 0) != 0)
				ableToPut = 1;
		}
	}
	
	if(!ableToPut){	// どこにも置けない場合
		printf("あなたの番ですが、どこにも置くことができません\n");
		return 0;
	}
	
	// 入力を促す
	while(1){
		printf("あなたの手を、半角スペース区切りで、x座標(1-8) y座標(1-8)の形で入力してください\n");
		scanf("%d%d", &grid[0], &grid[1]);
		
		grid[0]--;	// 内部での座標0-7に変換
		grid[1]--;
		
		num = update(board, grid, color);	// ここで石を置く（ことを試みる）
		if(num > 0)	// 1つ以上ひっくり返すことができたということは
			break;		// 石を置くことに成功したことを意味する
		
		printf("そこには置けません\n");
	}	// 成功するまで繰り返す
	
	return num;
}

// コンピュータが石を置く操作
// ひっくり返した石の数を返す（置けなければ0）
int computerAction(int board[][BOARDSIZE], int color){
	int grid[2], num;
	
	// 盤面を表示する
	printBoard(board);
	
	/* ここにマス選択アルゴリズムを書く */
	// ひっくり返す石の数を必ずnumに入れること
	num = selectMax(board, grid, color);
	
	// どこにも置けない場合
	if(num == 0){
		printf("コンピュータの番ですが、どこにも置くことができません\n");
		return 0;
	}
	
	// ひっくり返す
	update(board, grid, color);

	return num;
}
// 異なるアルゴリズムを持つコンピュータ同士を勝負させる時用
// コンピュータが石を置く操作
// ひっくり返した石の数を返す（置けなければ0）
int computerActionAlter(int board[][BOARDSIZE], int color){
	int grid[2], num;
	
	// 盤面を表示する
	printBoard(board);
	
	/* ここに別のマス選択アルゴリズムを書く */
	// ひっくり返す石の数を必ずnumに入れること
	num = evaluate(board, grid, color);
	
	// どこにも置けない場合
	if(num == 0){
		printf("コンピュータの番ですが、どこにも置くことができません\n");
		return 0;
	}
	
	// ひっくり返す
	update(board, grid, color);

	return num;
}

// マスに石を置いて、ひっくり返す
// grid[2]の場所にcolor色の石を置く
// 正常に置いたらひっくり返した数、置けなかったら0を返す
int update(int array[][BOARDSIZE], int grid[2], int color){
	int x = grid[0], y = grid[1];
	int num;
	
	// すでに石が置いてある場所ではないことを確認
	if(array[x][y] != BLANC)
		return 0;	// 置けないので1
	
	// 実際に置いてみる
	if((num = check(array, grid, color, 1)) == 0)
		return 0;	// 置けなかった
	
	// 問題なかったので、置いた報告をしてから0を返して終了
	//printf("%s stone has put on (%d, %d)\n", (color == 1) ? "BLACK" : "WHITE", grid[0]+1, grid[1]+1);
	//printf("%d stones has been fliped\n\n", num);
	return num;
}

// 表示
void printBoard(int board[][BOARDSIZE]){
	int i, j;
	// 表示される向きを調整するために、添字を[j][i]とする
	
	printf("\n");	// 読みやすくするため
	
	// 列番号
	printf(" ");
	for(i = 0; i < BOARDSIZE; i++)
		printf("%d", i+1);
	printf("\n");
	
	// 盤面の表示
	for(i = 0; i < BOARDSIZE; i++){
		printf("%d", i+1);	// 行番号
		for(j = 0; j < BOARDSIZE; j++){
			if(!board[j][i]){
				printf(" ");
			} else if(board[j][i] == BLACK){
				printf("■");
			} else {
				printf("□");
			}
		}
		printf("\n");
	}
	printf("\n");
}


/* ----------------------------System : ここの関数はmainから直接呼び出さないこと----------------------------- */

// 置けるかどうか、置いた場合の取れる石の数を計算
// 引数：盤面データ, 置きたい場所（座標）, 石を置くプレイヤーの色、モード
// モードが0の時は数えるだけ、1の時は実際にひっくり返す
// 戻り値：置けない場合は0, 置ける場合は取れる石の数
int check(int array[][BOARDSIZE], int grid[2], int color, int mode){
	int x = grid[0], y = grid[1];
	int xi, yi, k;
	int count, sum;		// 取れる石のカウント、最大数
	
	// すでに石が置いてある場所ではないことを確認
	if(array[x][y] != BLANC)
		return 0;	// 置けないので0
	
	sum = 0;
	
	// 上に2つ以上マスがある
	if(y >= 2){
		// 1つ上をチェック
		yi = y-1;
		if(array[x][yi] == flip[color]){
			// 石があり、プレイヤーと違う色だったので、この方向にひっくり返せるかも
			count = 1;
			for(yi = yi - 1; yi > -1; yi--){
				// 順に見ていく
				if(array[x][yi] == BLANC){
					// もう石がない
					break;
				}
				if(array[x][yi] == color){
					// 自分の石があったので、count個ひっくり返せる
					sum += count;
					
					/* 実際にひっくり返す場合だけの処理 */
					if(mode){
						for(k = y; k > yi; k--){
							array[x][k] = color;
						}
					}
					/* ------------------------------------------------ */
					
					break;
				}
				// 相手の石が続く
				count++;
			}
		}
	}
	
	// 下に2つ以上マスがある
	if(y <= 5){
		// 1つ下をチェック
		yi = y+1;
		if(array[x][yi] == flip[color]){
			// 石があり、プレイヤーと違う色だったので、この方向にひっくり返せるかも
			count = 1;
			for(yi = yi + 1; yi < 8; yi++){
				// 順に見ていく
				if(array[x][yi] == BLANC){
					// もう石がない
					break;
				}
				if(array[x][yi] == color){
					// 自分の石があったので、count個ひっくり返せる
					sum += count;
					
					/* 実際にひっくり返す場合だけの処理 */
					if(mode){
						for(k = y; k < yi; k++){
							array[x][k] = color;
						}
					}
					/* ------------------------------------------------ */
					
					break;
				}
				// 相手の石が続く
				count++;
			}
		}
	}
	
	// 左に2つ以上マスがある
	if(x >= 2){
		// 1つ左をチェック
		xi = x-1;
		if(array[xi][y] == flip[color]){
			// 石があり、プレイヤーと違う色だったので、この方向にひっくり返せるかも
			count = 1;
			for(xi = xi - 1; xi > -1; xi--){
				// 順に見ていく
				if(array[xi][y] == BLANC){
					// もう石がない
					break;
				}
				if(array[xi][y] == color){
					// 自分の石があったので、count個ひっくり返せる
					sum += count;
					
					/* 実際にひっくり返す場合だけの処理 */
					if(mode){
						for(k = x; k > xi; k--){
							array[k][y] = color;
						}
					}
					/* ------------------------------------------------ */
					
					break;
				}
				// 相手の石が続く
				count++;
			}
		}
	}
	
	// 右に2つ以上マスがある
	if(x <= 5){
		// 1つ右をチェック
		xi = x+1;
		if(array[xi][y] == flip[color]){
			// 石があり、プレイヤーと違う色だったので、この方向にひっくり返せるかも
			count = 1;
			for(xi = xi + 1; xi < 8; xi++){
				// 順に見ていく
				if(array[xi][y] == BLANC){
					// もう石がない
					break;
				}
				if(array[xi][y] == color){
					// 自分の石があったので、count個ひっくり返せる
					sum += count;
					
					/* 実際にひっくり返す場合だけの処理 */
					if(mode){
						for(k = x; k < xi; k++){
							array[k][y] = color;
						}
					}
					/* ------------------------------------------------ */
					
					break;
				}
				// 相手の石が続く
				count++;
			}
		}
	}
	
	// 左上の斜め方向に2つ以上マスがある
	if(x >= 2 && y >= 2){
		// 1つ左上をチェック
		xi = x - 1;
		yi = y - 1;
		if(array[xi][yi] == flip[color]){
			// 石があり、プレイヤーと違う色だったので、この方向にひっくり返せるかも
			count = 1;
			xi--;
			yi--;
			while(xi > -1 && yi > -1){
				// 順に見ていく
				if(array[xi][yi] == BLANC){
					// もう石がない
					break;
				}
				if(array[xi][yi] == color){
					// 自分の石があったので、count個ひっくり返せる
					sum += count;
					
					/* 実際にひっくり返す場合だけの処理 */
					if(mode){
						for(k = 0; k < x - xi; k++){	// xの移動量とyの移動量は同じなので
							array[x-k][y-k] = color;		// xyの移動量でループさせる
						}
					}
					/* ------------------------------------------------ */
					
					break;
				}
				// 相手の石が続く
				count++;
				xi--;
				yi--;
			}
		}	
	}
	
	// 左下の斜め方向に2つ以上マスがある
	if(x >= 2 && y <= 5){
		// 1つ左下をチェック
		xi = x - 1;
		yi = y + 1;
		if(array[xi][yi] == flip[color]){
			// 石があり、プレイヤーと違う色だったので、この方向にひっくり返せるかも
			count = 1;
			xi--;
			yi++;
			while(xi > -1 && yi < 8){
				// 順に見ていく
				if(array[xi][yi] == BLANC){
					// もう石がない
					break;
				}
				if(array[xi][yi] == color){
					// 自分の石があったので、count個ひっくり返せる
					sum += count;
					
					/* 実際にひっくり返す場合だけの処理 */
					if(mode){
						for(k = 0; k < x - xi; k++){	// xの移動量とyの移動量は同じなので
							array[x-k][y+k] = color;		// xyの移動量でループさせる
						}
					}
					/* ------------------------------------------------ */
					
					break;
				}
				// 相手の石が続く
				count++;
				xi--;
				yi++;
			}
		}	
	}
	
	// 右下の斜め方向に2つ以上マスがある
	if(x <= 5 && y <= 5){
		// 1つ右下をチェック
		xi = x + 1;
		yi = y + 1;
		if(array[xi][yi] == flip[color]){
			// 石があり、プレイヤーと違う色だったので、この方向にひっくり返せるかも
			count = 1;
			xi++;
			yi++;
			while(xi < 8 && yi < 8){
				// 順に見ていく
				if(array[xi][yi] == BLANC){
					// もう石がない
					break;
				}
				if(array[xi][yi] == color){
					// 自分の石があったので、count個ひっくり返せる
					sum += count;
					
					/* 実際にひっくり返す場合だけの処理 */
					if(mode){
						for(k = 0; k < xi - x; k++){		// xの移動量とyの移動量は同じなので
							array[x+k][y+k] = color;		// xyの移動量でループさせる
						}
					}
					/* ------------------------------------------------ */
					
					break;
				}
				// 相手の石が続く
				count++;
				xi++;
				yi++;
			}
		}	
	}
	
	// 右上の斜め方向に2つ以上マスがある
	if(x <= 5 && y >= 2){
		// 1つ右上をチェック
		xi = x + 1;
		yi = y - 1;
		if(array[xi][yi] == flip[color]){
			// 石があり、プレイヤーと違う色だったので、この方向にひっくり返せるかも
			count = 1;
			xi++;
			yi--;
			while(xi < 8 && yi > -1){
				// 順に見ていく
				if(array[xi][yi] == BLANC){
					// もう石がない
					break;
				}
				if(array[xi][yi] == color){
					// 自分の石があったので、count個ひっくり返せる
					sum += count;
					
					/* 実際にひっくり返す場合だけの処理 */
					if(mode){
						for(k = 0; k < xi - x; k++){		// xの移動量とyの移動量は同じなので
							array[x+k][y-k] = color;		// xyの移動量でループさせる
						}
					}
					/* ------------------------------------------------ */
					
					break;
				}
				// 相手の石が続く
				count++;
				xi++;
				yi--;
			}
		}	
	}
	
	return sum;
}