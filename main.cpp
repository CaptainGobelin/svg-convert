#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <cmath>
#include <string.h>
#include <cstdio>

bool compColors(sf::Color c1, sf::Color c2) {
	if (c1.r != c2.r)
		return false;
	if (c1.g != c2.g)
		return false;
	if (c1.b != c2.b)
		return false;
	return true;
}

int main(int argc, char *argv[]) {
	sf::Image blueprint;
	blueprint.loadFromFile((std::string)argv[1]+"."+(std::string)argv[2]);
	char *t = argv[1];
	strcat(t,".svg");
	std::ofstream file(t, std::ios::out | std::ios::trunc);
	if (!file)
		return 0;
	file << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
	sf::Vector2u size = blueprint.getSize();
	file << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" ";
	file << "width=\"" << size.x*10 << "\" height=\"" << size.y*10 << "\">\n";
	bool **pixels;
	pixels = new bool *[size.x];
	for (unsigned int i=0;i<size.x;i++)
		pixels[i] = new bool[size.y];
	for (unsigned int j=0;j<size.y;j++)
		for (unsigned int i=0;i<size.x;i++)
			pixels[i][j] = false;
	for (unsigned int j=0;j<size.y;j++) {
		for (unsigned int i=0;i<size.x;i++) {
			if (pixels[i][j])
				continue;
			sf::Color c = blueprint.getPixel(i, j);
			if (c.a == 0)
				continue;
			file << "<path fill=\"rgb(";
			file << (int)c.r << "," << (int)c.g << "," << (int)c.b << ")\" ";
			/*float opacity = (float)c.a/255.0;
			file << "fill-opacity=\"" << opacity << "\" ";*/
			file << "d=\"M" << i*10 << " " << j*10;
			unsigned int ii = i;
			unsigned int jj = j;
			bool left = true;
			bool top = true;
			std::string s = "";
			do {
				file << s;
				s = "";
				pixels[ii][jj] = true;
				if (left) {
					if (top) {
						if ((jj>0) && compColors(c, blueprint.getPixel(ii,jj-1))) {
							s = " H" + std::to_string(ii*10);
							top = false;
							jj--;
						}
						else if ((ii<(size.x-1)) && compColors(c, blueprint.getPixel(ii+1,jj))) {
							ii++;
						}
						else {
							s = " H" + std::to_string(ii*10+10);
							left = false;
						}
					}
					else {
						if ((ii>0) && compColors(c, blueprint.getPixel(ii-1,jj))) {
							s = " V" + std::to_string(jj*10+10);
							left = false;
							ii--;
						}
						else if ((jj>0) && compColors(c, blueprint.getPixel(ii,jj-1))) {
							jj--;
						}
						else {
							s = " V" + std::to_string(jj*10);
							top = true;
						}
					}
				}
				else {
					if (top) {
						if ((ii<(size.x-1)) && compColors(c, blueprint.getPixel(ii+1,jj))) {
							s = " V" + std::to_string(jj*10);
							left = true;
							ii++;
						}
						else if ((jj<(size.y-1)) && compColors(c, blueprint.getPixel(ii,jj+1))) {
							jj++;
						}
						else {
							s = " V" + std::to_string(jj*10+10);
							top = false;
						}
					}
					else {
						if ((jj<(size.y-1)) && compColors(c, blueprint.getPixel(ii,jj+1))) {
							s = " H" + std::to_string(ii*10+10);
							top = true;
							jj++;
						}
						else if ((ii>0) && compColors(c, blueprint.getPixel(ii-1,jj))) {
							ii--;
						}
						else {
							s = " H" + std::to_string(ii*10);
							left = true;
						}
					}
				} 
			} while ((ii != i) || (jj != j) || !left || !top);
			file << " Z\"/>\n";
		}
	}
	file << "</svg>";
	return 0;
}