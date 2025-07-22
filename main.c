#include <graphx.h>
#include <keypadc.h>
#include <math.h>
#include <sys/timers.h>
#include <ti/screen.h>

// VIEWERS NOTE: I kinda have trouble with not spamming conditionals everywhere :P

int bottle_levels[2][4][4] = {{{0,0,0,0},
                            {1,1,2,3},
                            {1,2,3,2},
                            {3,3,2,1}},

                            {{0,0,1,1},
                            {0,0,2,3},
                            {3,3,2,3},
                            {1,2,1,2}}}; // Bottle levels here!

// Don't change these.
int current_level = 0;
int highlighted_bottle = 0;
int selected_bottle = -1;

int current_bottles[4][4] = {{0,0,0,0},
                            {1,1,2,3},
                            {1,2,3,2},
                            {3,3,2,1}}; // The current bottle levels that can be changed at any time.

int bottle_colors[4] = {0x94, 0xE0, 0x07, 0x18}; // Colour of each level.

int trunc_mod(int a, int b) {
    return ((a % b) + b) % b; // This does modulos on negative nums.
}

void draw() {
    gfx_FillScreen(0xFF);

    gfx_SetTextTransparentColor(0);
    gfx_SetTextBGColor(255);
    gfx_SetTextFGColor(127);
    gfx_SetTextScale(3,3);
    gfx_SetTextXY(42,20);
    gfx_PrintString("Liquid Sort!");

    for (int i = 0; i<4; i++) {
        gfx_SetColor(0x94); // Grayish colour.
        gfx_FillRectangle((i+1)*64-16, 64, 34, 120);
        gfx_FillRectangle((i+1)*64-22, 64, 46, 4);
        gfx_SetColor(0xF8);
        if (selected_bottle==i) {
            gfx_Rectangle((i+1)*64-26, 59, 54, 130);
        }
        gfx_SetColor(0x00);
        if (i==highlighted_bottle) {
            gfx_Rectangle((i+1)*64-26, 59, 54, 130);
        }
        for (int j = 0; j<4; j++) {
            gfx_SetColor(bottle_colors[current_bottles[i][j]]);
            gfx_FillRectangle((i+1)*64-12, 68+(j*28), 26, 28);
        }
    }
    gfx_SwapDraw();
}

int main(void) {
    gfx_Begin();
	gfx_SetDrawBuffer();

    int tick = 0;

    while (true) {
        kb_Scan();
        tick++;

        if (tick%60==0) { //Slow down process so we don't SPAMMM keys.
            draw(); 

            if (kb_Data[7] & kb_Left) {
                highlighted_bottle = trunc_mod(highlighted_bottle-1, 4);
            } else if (kb_Data[7] & kb_Right) {
                highlighted_bottle = (highlighted_bottle+1)%4;
            } else if (kb_Data[1] & kb_2nd) {
                if (selected_bottle == -1) {
                    selected_bottle = highlighted_bottle;
                    highlighted_bottle = (highlighted_bottle+1)%4;
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
            }
        }

        if (kb_Data[6] & kb_Clear) {
            break;
        }
    }

    gfx_End();

    return 0;
}