#ifndef _SD_CARD_H
#define _SD_CARD_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MOUNT_PATH "/mnt/card/"
#define SD_CARD_PATH "/dev/mmcblk0p1"

void mountSDCard();

void copyFileToSDCard(const char *source_path);

void unmountSDCard();

bool checkMntSuccess();

#endif
