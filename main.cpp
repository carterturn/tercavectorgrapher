/*
  Copyright 2015 Carter Turnbaugh

  This file is part of Terca Vector Grapher.

  Terca Vector Grapher is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Terca Vector Grapher is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Terca Vector Grapher.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <unistd.h>
#include <vector>

#include <algorithm>
#include <chrono>

#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <ctvector.h>

using namespace std;

double xview, yview, zview, view;

vector<ctvector> vectors;
vector<ctvector> axis;

void checkkeys(GLFWwindow * window, int key, int scancode, int action, int mods){
	if(key == GLFW_KEY_KP_0 || key == GLFW_KEY_Z){
		view = view*0.9;
	}
	else if(key == GLFW_KEY_KP_DECIMAL || key == GLFW_KEY_X){
		view = view*1.1;
	}
	else{
		double xrot, yrot, zrot;
		if(key == GLFW_KEY_KP_9 || key == GLFW_KEY_Q){
			xrot = 0;
			yrot = 0.1;
			zrot = 0;
		}
		else if(key == GLFW_KEY_KP_7 || key == GLFW_KEY_E){
			xrot = 0;
			yrot = -0.1;
			zrot = 0;
		}
		else if(key == GLFW_KEY_KP_8 || key == GLFW_KEY_W){
			xrot = 0.1;
			yrot = 0;
			zrot = 0;
		}
		else if(key == GLFW_KEY_KP_2 || key == GLFW_KEY_S){
			xrot = -0.1;
			yrot = 0;
			zrot = 0;
		}
		else if(key == GLFW_KEY_KP_4 || key == GLFW_KEY_A){
			xrot = 0;
			yrot = 0;
			zrot = 0.1;
		}
		else if(key == GLFW_KEY_KP_6 || key == GLFW_KEY_D){
			xrot = 0;
			yrot = 0;
			zrot = -0.1;
		}
		for(int i = 0; i < vectors.size(); i++){
			vectors[i].rotate(xrot, yrot, zrot);
		}
		for(int i = 0; i < axis.size(); i++){
			axis[i].rotate(xrot, yrot, zrot);
		}
	}
}

int main(int argc, char* argv[]){

	xview = 10.0;
	yview = 10.0;
	zview = 10.0;
	
	for(int i = 1; i < argc; i++){
		string x;
		string y;
		string z;
		string data = argv[i];
		int mode = 0;
		for(int i = 0; i < data.length(); i++){
			if(data[i] != ','){
				switch(mode){
					case 0:
						x = x + data[i];
						break;
					case 1:
						y = y + data[i];
						break;
					case 2:
						z = z + data[i];
						break;
					default:
						break;
				}
			}
			else{
				mode++;
			}
		}
		xview = max(xview, atof(x.c_str())*100.0);
		yview = max(yview, atof(y.c_str())*100.0);
		zview = max(zview, atof(z.c_str())*100.0);
		vectors.push_back(ctvector(atof(x.c_str())*100.0, atof(y.c_str())*100.0, atof(z.c_str())*100.0));
	}
	view = max(xview, max(yview, zview));
	
	glfwInit();
	GLFWwindow * graphwindow = glfwCreateWindow(1080, 1080, "Terca Vector Grapher", NULL, NULL);

	if(!graphwindow){
		cout << "Graphwindow Error\n";
		return -1;
	}

	glfwMakeContextCurrent(graphwindow);
	glfwSetKeyCallback(graphwindow, checkkeys);
	glfwSetInputMode(graphwindow, GLFW_STICKY_KEYS, 1);

	axis.push_back(ctvector(view, 0, 0));
	axis.push_back(ctvector(-1.0*view, 0, 0));
	axis.push_back(ctvector(0, view, 0));
	axis.push_back(ctvector(0, -1*view, 0));
	axis.push_back(ctvector(0, 0, view));
	axis.push_back(ctvector(0, 0, -1*view));
	
	while(true){
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-1*view, view, -1*view, view, -1*view, view);

		glClear(GL_COLOR_BUFFER_BIT);

		for(int i = 0; i < axis.size(); i++){
			glColor3f(0.5f, (i+1)%2, 0.0f);
			
			glBegin(GL_LINES);
			
			glVertex3i(0, 0, 0);
			glVertex3i(axis[i].x, axis[i].z, axis[i].y);
			
			glEnd();
		}
		
		for(int i = 0; i < vectors.size(); i++){
			glBegin(GL_LINES);

			glColor3f((((float) (i % 2))/2.0f), (((float) (i % 3))/3.0f), 1.0f);
			
			glVertex3i(0, 0, 0);
			glVertex3i(vectors[i].x, vectors[i].z, vectors[i].y);

			glEnd();
		}

		glfwSwapBuffers(graphwindow);
		glfwPollEvents();

		usleep(1000);
	}

	glfwTerminate();
}
