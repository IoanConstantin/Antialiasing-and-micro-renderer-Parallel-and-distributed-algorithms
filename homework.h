#ifndef HOMEWORK_H
#define HOMEWORK_H

typedef struct imagine{
	unsigned char *sir1;
	int latime;
	int inaltime;
	int max_value;
	unsigned char *sir2;
}image;

void readInput(const char * fileName, image *img);

void writeData(const char * fileName, image *img);

void resize(image *in, image * out);

#endif /* HOMEWORK_H */
