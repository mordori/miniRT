#include "utils.h"

static inline void	print_editing(void);
static inline void	print_general(void);

void	printf_init(t_context *ctx)
{
	printf("\033[?25l\nRENDER SETTINGS\n");
	printf("Samples:\t%d\n", ctx->renderer.render_samples);
	printf("Bounces:\t%d\n\n", ctx->renderer.render_bounces);
	printf("CONTROLS\n");
	printf("                 RENDER MODE                        \n");
	printf("|---------------------|----------------------------|\n");
	printf("|         Key         |         Navigation         |\n");
	printf("|---------------------|----------------------------|\n");
	printf("| LMB                 | Look                       |\n");
	printf("| RMB                 | Turn                       |\n");
	printf("| W A S D             | Move                       |\n");
	printf("| SHIFT / SPACE       | Descend / Ascend           |\n");
	printf("| R                   | Reset Camera               |\n");
	printf("|---------------------|----------------------------|\n");
	printf("\n");
	print_editing();
	print_general();
}

static inline void	print_editing(void)
{
	printf("                  EDIT MODE                         \n");
	printf("|---------------------|----------------------------|\n");
	printf("|         Key         |           Action           |\n");
	printf("|---------------------|----------------------------|\n");
	printf("| TAB                 | Toggle Edit Mode           |\n");
	printf("|---------------------|----------------------------|\n");
	printf("|         Key         |         Navigation         |\n");
	printf("|---------------------|----------------------------|\n");
	printf("| F                   | Frame Selected             |\n");
	printf("| ALT + LMB           | Orbit                      |\n");
	printf("| ALT + RMB           | Zoom                       |\n");
	printf("| ALT + MMB           | Pan                        |\n");
	printf("|---------------------|----------------------------|\n");
	printf("|         Key         |          Editing           |\n");
	printf("|---------------------|----------------------------|\n");
	printf("| LMB                 | Select Object / Apply Edit |\n");
	printf("| RMB                 | Cancel Edit                |\n");
	printf("| G                   | Move                       |\n");
	printf("| R                   | Rotate                     |\n");
	printf("| S                   | Scale                      |\n");
	printf("| X / Y / Z           | Axis Constraint            |\n");
	printf("| SHIFT + X / Y / Z   | Plane Constraint           |\n");
	printf("|---------------------|----------------------------|\n");
	printf("\n");
}

static inline void	print_general(void)
{
	printf("                   GENERAL                          \n");
	printf("|---------------------|----------------------------|\n");
	printf("|         Key         |           Action           |\n");
	printf("|---------------------|----------------------------|\n");
	printf("| , / .               | Rotate Skydome             |\n");
	printf("| ↓ / ↑               | Focus Distance             |\n");
	printf("| ← / →               | Aperture                   |\n");
	printf("| K / L               | Focal Length               |\n");
	printf("| O / P               | Samples                    |\n");
	printf("| U / I               | Bounces                    |\n");
	printf("| H                   | Toggle UI                  |\n");
	printf("| Y                   | Save render                |\n");
	printf("| ESC                 | Quit                       |\n");
	printf("|---------------------|----------------------------|\n");
	printf("\n");
}
