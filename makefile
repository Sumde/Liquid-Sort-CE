# ----------------------------
# Makefile Options
# ----------------------------

NAME = LIQSORT
ICON = icon.png
DESCRIPTION = "A game about sorting liquid."
COMPRESSED = NO
ARCHIVED = YES

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

# ----------------------------

include $(shell cedev-config --makefile)
