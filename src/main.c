#include <graphx.h>
#include <keypadc.h>
#include <math.h>
#include <sys/timers.h>
#include <ti/screen.h>
#include <ti/getkey.h>
#include "levels.h"

// VIEWERS NOTE: I kinda have trouble with not spamming conditionals everywhere :P

// Don't change these.
int current_level = 0;
int highlighted_bottle = 0;
int selected_bottle = -1;
int num_bottles = 4;

int current_bottles[8][4] = {{0,0,1,1},
                            {0,0,1,1},
                            {-1,-1,-1,-1},
                            {-1,-1,-1,-1},
                            {-1,-1,-1,-1},
                            {-1,-1,-1,-1},
                            {-1,-1,-1,-1},
                            {-1,-1,-1,-1}}; // The current bottle levels that can be changed at any time.

int bottle_colors[7] = {0x94, 0xE0, 0x07, 0x18, 0xE3, 0xF8, 0x00}; // Colour of each level.

int trunc_mod(int a, int b) {
    return ((a % b) + b) % b; // This does modulos on negative nums.
}

void draw() {
    gfx_FillScreen(0xFF);

    gfx_SetTextTransparentColor(0);
    gfx_SetTextBGColor(0xFF);
    gfx_SetTextFGColor(0x7F);
    gfx_SetTextScale(3,3);
    gfx_SetTextXY(42,20);
    gfx_PrintString("Liquid Sort!");

    gfx_SetTextFGColor(0x98);
    gfx_SetTextScale(2,2);
    gfx_SetTextXY(32,200);
	gfx_PrintString("Current Level: ");
    gfx_SetTextXY(16*sizeof("Current Level: "),200);
	gfx_PrintInt(current_level, 1);

    int win_count = 0;

    num_bottles = 0;

    for (int i = 0; i<8; i++) {
        if (bottle_levels[current_level][i][0]!=-1) {
            num_bottles++;
        }
    }

    for (int i = 0; i<num_bottles; i++) {
        if (num_bottles<=4) {
            gfx_SetColor(0x94); // Grayish colour.
            gfx_FillRectangle((i+1)*64-16+(128-num_bottles*32), 64, 34, 120);
            gfx_FillRectangle((i+1)*64-22+(128-num_bottles*32), 64, 46, 4);
            gfx_SetColor(0xF8);
            if (selected_bottle==i) {
                gfx_Rectangle((i+1)*64-26+(128-num_bottles*32), 59, 54, 130);
                gfx_Rectangle((i+1)*64-26+(128-num_bottles*32)-1, 58, 56, 132);
            }
            gfx_SetColor(0x00);
            if (i==highlighted_bottle) {
                gfx_Rectangle((i+1)*64-26+(128-num_bottles*32), 59, 54, 130);
                gfx_Rectangle((i+1)*64-26+(128-num_bottles*32)-1, 58, 56, 132);
            }
            for (int j = 0; j<4; j++) {
                gfx_SetColor(bottle_colors[current_bottles[i][j]]);
                gfx_FillRectangle((i+1)*64-12+(128-num_bottles*32), 68+(j*28), 26, 28);
                if (bottle_colors[current_bottles[i][j]]==bottle_colors[current_bottles[i][0]]) {
                    win_count++;
                }
            }
        } else {
            gfx_SetColor(0x94);
            if (i<4) {
                gfx_FillRectangle((i+1)*64-16, 64, 17, 60);
                gfx_FillRectangle((i+1)*64-19, 64, 23, 2);
            } else {
                gfx_FillRectangle((i-3)*64-16, 138, 17, 60);
                gfx_FillRectangle((i-3)*64-19, 138, 23, 2);
            }
            gfx_SetColor(0xF8);
            if (selected_bottle==i) {
                if (i<4) {
                    gfx_Rectangle((i+1)*64-21, 61, 27, 65);
                    gfx_Rectangle((i+1)*64-21-1, 60, 29, 67);
                } else {
                    gfx_Rectangle((i-3)*64-21, 135, 27, 65);
                    gfx_Rectangle((i-3)*64-21-1, 134, 29, 67);
                }
            }
            gfx_SetColor(0x00);
            if (i==highlighted_bottle) {
                if (i<4) {
                    gfx_Rectangle((i+1)*64-21, 61, 27, 65);
                    gfx_Rectangle((i+1)*64-21-1, 60, 29, 67);
                } else {
                    gfx_Rectangle((i-3)*64-21, 135, 27, 65);
                    gfx_Rectangle((i-3)*64-21-1, 134, 29, 67);
                }
            }
            for (int j = 0; j<4; j++) {
                gfx_SetColor(bottle_colors[current_bottles[i][j]]);
                if (i<4) {
                    gfx_FillRectangle((i+1)*64-14, 66+(j*14), 13, 14);
                } else {
                    gfx_FillRectangle((i-3)*64-14, 140+(j*14), 13, 14);
                }
                if (bottle_colors[current_bottles[i][j]]==bottle_colors[current_bottles[i][0]]) {
                    win_count++;
                }
            }
        }
    }

    if (win_count==(4*num_bottles)) {
        gfx_SetTextFGColor(0xE0);
        gfx_SetTextScale(4,4);
        gfx_SetTextXY(52,104);
        gfx_PrintString("You won!");
    }
    gfx_SwapDraw();
    if (win_count==(4*num_bottles)) {
        delay(2000);
        current_level++;
        for (int i = 0; i<8; i++) {
            for (int j = 0; j<4; j++) {
                current_bottles[i][j] = bottle_levels[current_level][i][j];
            }
        }
    }
}

int main(void) {
    gfx_Begin();
	gfx_SetDrawBuffer();

    int tick = 0;
    int is_holding_key = 0;

    while (current_level<num_bottle_levels) {
        kb_Scan();
        tick++;

        if (tick%3==0) { //Slow down process so we don't crash da calc.
            draw(); 

            if ((!(kb_Data[7] & (kb_Left | kb_Right)))) {
                if ((!(kb_Data[1] & kb_2nd))) {
                    is_holding_key = 0;
                }
            }

            if (kb_Data[7] & kb_Left && is_holding_key==0) {
                highlighted_bottle = trunc_mod(highlighted_bottle-1, num_bottles);
                is_holding_key = 1;
            } else if (kb_Data[7] & kb_Right && is_holding_key==0) {
                highlighted_bottle = (highlighted_bottle+1)%num_bottles;
                is_holding_key = 1;
            } else if (kb_Data[1] & kb_2nd && is_holding_key==0) {
                if (selected_bottle == -1) {
                    selected_bottle = highlighted_bottle;
                    highlighted_bottle = (highlighted_bottle+1)%num_bottles;
                } else if (selected_bottle == highlighted_bottle) {
                    selected_bottle = -1;
                } else {
                    int current_iteration;
                    // Pouring logic:
                    for (int i = 0; i<4; i++) {
                        if (current_iteration!=current_bottles[selected_bottle][i] && i!=0 && current_iteration!=0) {
                            break;
                        }
                        current_iteration = current_bottles[selected_bottle][i];
                        if (current_iteration==0) {
                            continue;
                        } else { //Brute force, yay!!
                            for (int j = 0; j<3; j++) {
                                if (current_bottles[highlighted_bottle][j]==0 && current_bottles[highlighted_bottle][j+1]==current_iteration) {
                                    current_bottles[highlighted_bottle][j] = current_iteration;
                                    current_bottles[selected_bottle][i] = 0;
                                    break;
                                } else if (current_bottles[highlighted_bottle][3]==0) {
                                    current_bottles[highlighted_bottle][3] = current_iteration;
                                    current_bottles[selected_bottle][i] = 0;
                                    break;
                                }
                            }
                        }
                    }
                    selected_bottle = -1;
                }
                is_holding_key = 1;
            }
        }

        if (kb_Data[6] & kb_Clear) {
            break;
        }
    }

    gfx_End();

    return 0;
}