/*
 * Author:	Lucas Costa Campos
 * Email: 	Rmk236@gmail.com
 * Version:	0.92
 * License:	GNU General Public License
 * 		Copyright: 2013 Lucas Costa Campos
 * Website: 	https://github.com/LucasCampos/libepswriter
 */

/*
 *     This file is part of libepswriter.
 *
 *     libepswriter is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 3 of the License, or
 *     (at your option) any later version.
 * 
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 * 
 *     You should have received a copy of the GNU General Public License
 *     along with this program; if not, write to the Free Software
 *     Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * 
 * */
#include "epswriter.hpp"

epswriter::epswriter(std::string fileName, double minX, double minY, double maxX, double maxY): name(fileName), currRed(0), currGreen(0), currBlue(0){

	eps=std::shared_ptr<std::ofstream>(new std::ofstream, WriterDeleter());

	eps->open(fileName);
	lineWidth = std::min(maxX-minX, maxY-minY)/20;
	fontSize = std::min(maxX-minX, maxY-minY)/10;

	//Writes *eps header
	*eps << " %!PS-Adobe-3.0 *epsF-3.0 " << std::endl;
	*eps << "%%Pages: 1" << std::endl;
	*eps << "%%BoundingBox: " << minX << " " << minY << " " << maxX << " " << maxY << std::endl;
	*eps << "%%LanguageLevel: 1" << std::endl;
	*eps << "%%EndComments" << std::endl << std::endl;

	*eps << "0 0 0 setrgbcolor" << std::endl << std::endl;
	*eps << lineWidth << " setlinewidth " << std::endl;
	*eps << "/Helvetica " << fontSize << " selectfont" << std::endl;

}

epswriter::~epswriter() {
}

bool epswriter::equalCurrColor(int r, int g, int b) {
	bool t = true;
	t&= (r==currRed);
	t&= (g==currGreen);
	t&= (b==currBlue);

	return t;
}

void epswriter::changeColor(int r, int g, int b) {
	if (!equalCurrColor(r,g,b)) {
		*eps << r/65535.0 << " " << g/65535.0 << " " << b/65535.0 << " setrgbcolor" << std::endl;
		currRed=r; currGreen=g; currBlue=b;
	}
}

double myAbs(double a){
	if (a < 0.0) return -a;
	return a;
}

void epswriter::line(double xfrom, double yfrom, double xto, double yto,  double width, int red, int green, int blue){
	changeColor(red,green,blue);
	if (myAbs(width-lineWidth) > 1e-4){
		lineWidth = width;
		*eps << width << " setlinewidth" << std::endl;
	}
	*eps << xfrom << " " << yfrom << " moveto " << std::endl;
	*eps << xto << " " << yto << " lineto " << std::endl;
	*eps << "stroke " << std::endl;

}

void epswriter::multiline (const std::vector<double>& x, const std::vector<double>& y, double width, int red, int green, int blue) {
	changeColor(red,green,blue);
	if (myAbs(width-lineWidth) > 1e-4){
		lineWidth = width;
		*eps << width << " setlinewidth" << std::endl;
	}
	if ((x.size() > 0) && (y.size() > 0))
		*eps << x[0] << " " << y[0] << " moveto" << std::endl;
	for (int i=1; i<std::min(x.size(),y.size()); i++)
		*eps << x[i] << " " << y[i] << " lineto" << std::endl;
	*eps << "stroke" << std::endl;
}

void epswriter::drawBasicTriangle(double x1, double y1, double x2, double y2, double x3, double y3, int red, int green, int blue) {

	changeColor(red,green,blue);

	*eps << x1 << " " << y1 << " moveto" << std::endl;
	*eps << x2 << " " << y2 << " lineto" << std::endl;
	*eps << x3 << " " << y3 << " lineto" << std::endl;
	*eps << "closepath " << std::endl;
}

void epswriter::triangle(double x1, double y1, double x2, double y2, double x3, double y3, int red, int green, int blue) {
	drawBasicTriangle(x1,y1,x2,y2,x3,y3, red, green, blue);
	*eps << "stroke" << std::endl;
}

void epswriter::filledTriangle(double x1, double y1, double x2, double y2, double x3, double y3, int red, int green, int blue) {
	drawBasicTriangle(x1,y1,x2,y2,x3,y3, red, green, blue);
	*eps << "fill" << std::endl;
}

void epswriter::drawBasicSquare(double xfrom, double yfrom, double xto, double yto, int red, int green, int blue) {
	
	changeColor(red,green,blue);
	*eps << xfrom << " " << yfrom << " moveto" << std::endl;
	*eps << xto << " " << yfrom << " lineto" << std::endl;
	*eps << xto << " " << yto << " lineto" << std::endl;
	*eps << xfrom << " " << yto << " lineto" << std::endl;
	*eps << "closepath" << std::endl;
}

void epswriter::square(double xfrom, double yfrom, double xto, double yto, int red, int green, int blue) {
	drawBasicSquare(xfrom, yfrom, xto, yto, red,green, blue);
	*eps << "stroke" << std::endl;
}

void epswriter::filledSquare(double xfrom, double yfrom, double xto, double yto, int red, int green, int blue) {
	drawBasicSquare(xfrom, yfrom, xto, yto, red,green, blue);
	*eps << "fill" << std::endl;
}


void epswriter::drawBasicCircle(double xcentre, double ycentre, double radius, int red, int green, int blue) {

	changeColor(red,green,blue);
	*eps << xcentre << " " << ycentre << " moveto" << std::endl;
	*eps << xcentre << " " << ycentre << " " << radius << " 0 360 arc" << std::endl;
	*eps << "closepath" << std::endl;
}

void epswriter::circle(double xcentre, double ycentre, double radius, int red, int green, int blue) 
{
	drawBasicCircle(xcentre, ycentre, radius, red, green, blue);
	*eps << "stroke" << std::endl;
}

void epswriter::filledCircle(double xcentre, double ycentre, double radius, int red, int green, int blue) 
{
	drawBasicCircle(xcentre, ycentre, radius, red, green, blue);
	*eps << "fill" << std::endl;
}


void epswriter::writeText(double x, double y, std::string text, double font, int red, int green, int blue)
{

	changeColor(red,green,blue);
	
	if (myAbs(font-fontSize) > 1e-4){
		fontSize=font;
		*eps << "/Helvetica " << fontSize << " selectfont" << std::endl;
	}

	*eps << x << " " << y << " moveto" << std::endl;
	*eps << "(" << text.c_str() <<") show" << std::endl;
}
