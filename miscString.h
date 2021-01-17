/* Miscellaneous string definitions and functions
 *
 * Adrian Ireland <procras2@gmail.com>
 *
 */

#ifndef MISCSTRING_H
#define MISCSTRING_H

#define DUMMYSTRINGSIZE 10
char dummyString[DUMMYSTRINGSIZE];

#define BUFFERSTRINGSIZE 88 // Must include space for terminal null character
char bufferString[BUFFERSTRINGSIZE]; // Output buffer for BME string

char miscS1[] = "init";
char miscS2[] = "true";
char miscS3[] = "false";

#endif // ifndef MISCSTRING_H
