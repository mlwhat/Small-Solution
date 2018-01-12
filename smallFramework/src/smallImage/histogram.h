#pragma once

#include "oneImage.h"

class Histogram
{
public:
	Histogram();
	~Histogram();

	Histogram(oneImage * img);


	void createHistogram(oneImage * img);


	void scretchHistogram(oneImage * img, int value);
	void stretchHistogramRGB(oneImage * img, int value);

	oneImage * getHistoRGB();
	oneImage * getHistoRed();
	oneImage * getHistoGreen();
	oneImage * getHistoBlue();

protected:
	int leftValue(int arr[]);
	int rightValue(int arr[]);

	int histoRGB[256], histoRed[256], histoBlue[256], histoGreen[256];
	
};