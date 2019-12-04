#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
#include <vector>

#include <string.h>
#include <ctime>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include <pthread.h>
#include <stdlib.h>

#include "network.hpp"

#include "rt.h"
#include "image.h"
#include "camera.h"
#include "shape.h"
#include "SDL.hpp"
#include "light.h"

char		**cstrsplit(const char *str, char delim);
char		*uitoa(unsigned long long value);

t_Main	a;
OctTree	*boundingBox;


int X1, Y1, X2, Y2 = 0;

using namespace std;


int    make_socket(char **argv)
{
    int port_number;
    int sockfd;
    struct sockaddr_in  socket_address;


    port_number = atoi(argv[1]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("socket");
        return (-1);
    }
    memset(&socket_address, '\0', sizeof(socket_address));
    socket_address.sin_family = AF_INET;
    socket_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    socket_address.sin_port = htons(port_number);

    if (connect(sockfd, (struct sockaddr *)&socket_address, sizeof(socket_address)) < 0)
    {
        perror("connect");
        return (-1);
    }
    return (sockfd);
}

char *rayTrace(int piece, int max_pieces) {
    std::cout   << "TRACING\n";

	string image_as_a_string = "";

    X1 = 0;
    X2 = g_width;
    Y1 = 0;
    Y2 = g_height;

    if (piece == 1) {
        X1 = 0;
		X2 = g_width / 2;
    }
	if (piece == 2) {
        X1 = g_width / 2;
        X2 = g_width;
    }

	Camera		*camera = a.camera;
	ShapeSet	*scene	= a.scene;
	double		r, g, b;
	
	drawHitBounds(boundingBox);
	for (int x = X1; x < X2; x++) {
		for (int y = Y1; y < Y2; y++) {
			Vector2 screenCoord((2.0f*x) / g_width - 1.0f,
								(-2.0f*y) / g_height + 1.0f);
			Ray ray = camera->makeRay(screenCoord);
			Intersection intersection(ray);

			// if (calcInterShapes(boundingBox, intersection)) {
			if (a.scene->intersect(intersection)) {
				Intersection lightIntersection = intersection;

				if (boundingBox->intersectLights(lightIntersection)) {
					intersection.lightDistance = lightIntersection.lightDistance;
				}

				r = intersection.color.r * 255.0f * intersection.lightDistance;
				g = intersection.color.g * 255.0f * intersection.lightDistance;
				b = intersection.color.b * 255.0f * intersection.lightDistance;
				
				image_as_a_string += to_string(int(r)) + "," + to_string(int(g)) + "," + to_string(int(b));
				a.sdl->putpixel(x, y, Color(r, g, b));
			} else {
				image_as_a_string += "0,0,0";
				a.sdl->putpixel(x, y, 0);
			}
			image_as_a_string += '|';
		}
		image_as_a_string += '-';
	}
	cout << "TRACED!\n";
	cout << image_as_a_string << endl;
	return (char *)image_as_a_string.c_str();
}

bool	calcInterShapes(OctTree *o, Intersection &intersection) {
	bool f = false;
	bool r = false;

	if (o->doesIntersect(intersection.ray)) {
		if (o->octas.size() == 8) {
			for (int i = 0; i < 8; i++) {
				f = calcInterShapes(o->octas[i], intersection);
				r = r ? true : f;
			}
		} else if (o->intersectShapes(intersection)) {
			Intersection lightIntersection = intersection;

			lightIntersection.ray.origin = lightIntersection.ray.calculate(lightIntersection.t);
			if (boundingBox->intersectLights(lightIntersection)) {
				intersection.lightDistance = lightIntersection.lightDistance;
			}
			return true;
		}
	}
	return r;
}

bool	calcInterLights(OctTree *o, Intersection &intersection) {
	bool f = false;
	bool r = false;

	if (o->doesIntersect(intersection.ray)) {
		if (o->octas.size() == 8) {
			for (int i = 0; i < 8; i++) {
				f = calcInterLights(o->octas[i], intersection);
				r = r ? true : f;
			}
		} else if (o->intersectLights(intersection)) {
			return true;			
		}
	}
	return r;
}

void	drawRect(Vector min, Vector max, Color c, int i = 1) {
	if (i) {
		a.minimap->drawLine(0 + min.x, 250 + min.z, 0 + max.x, 250 + min.z, c);
		a.minimap->drawLine(0 + min.x, 250 + min.z, 0 + min.x, 250 + max.z, c);
		a.minimap->drawLine(0 + max.x, 250 + max.z, 0 + max.x, 250 + min.z, c);
		a.minimap->drawLine(0 + max.x, 250 + max.z, 0 + min.x, 250 + max.z, c);
	} else {
		a.minimap->drawLine(350 + min.x, 250 + min.y, 350 + max.x, 250 + min.y, c);
		a.minimap->drawLine(350 + min.x, 250 + min.y, 350 + min.x, 250 + max.y, c);
		a.minimap->drawLine(350 + max.x, 250 + max.y, 350 + max.x, 250 + min.y, c);
		a.minimap->drawLine(350 + max.x, 250 + max.y, 350 + min.x, 250 + max.y, c);
	}

}

void	drawHitBounds(OctTree *o) {
	Color c;

	drawRect(o->min, o->max, {255, 0, 0});
	if (o->octas.size() == 8)
		for (int i = 0; i < 8; i++) {
			drawRect(o->octas[i]->min, o->octas[i]->max, {255, 0, 0}, 1);
			drawHitBounds(o->octas[i]);
		}
}

void 	draw(OctTree *o) {
	if (o->octas.size() > 0)
	for (int i = 0; i < 8; i++) {
		drawRect(o->octas[i]->min, o->octas[i]->max, {255, 0, 0}, 1);
		drawRect(o->octas[i]->min, o->octas[i]->max, {255, 0, 0}, 0);
		draw(o->octas[i]);
	}
}

void	spawnSphere(Vector v, double size, Color c) {
	Sphere *sphere = new Sphere(v, size, c);
	a.scene->addShape(sphere);
}


void	spawnLight(Vector v, double size, E_ShapeType type) {
	Light *light = new Light(v, size, type);
	a.scene->addShape(light);
}

void	spawnPlane(Vector v, Vector n, Color c) {
	Plane *floor = new Plane(v, n, c);
	a.scene->addShape(floor);
}

void	init() {
	boundingBox = new OctTree(&a);

	a.sdl = new SDL(g_width, g_height);
	a.sdl->display();
	SDL_SetWindowPosition(a.sdl->window, 0, 200);

	a.minimap = new SDL(g_width, g_height);
	SDL_SetWindowPosition(a.minimap->window, g_width, 200);
	a.minimap->display();

	a.camera = new PerspectiveCamera(
		Point(-5.0f, 1.0f, 0.0f),
		Vector(0.0f, 1.0f, 0.0f),
		Vector(), 70.0f * PI / 180.0f,
		(double)g_width / (double)g_height);
	a.scene = new ShapeSet;
}

void	createScene() {
	spawnPlane({0.0f, 0.0f, 0.0f}, Vector(), {0.5f, 1.0f, 0.5f});
	spawnSphere({5.0f, 2.0f, 5.0f}, 1, {1.0f, 1.0f, 1.0f});

	// spawnSphere({5.0f, 0.0f, 0.0f}, 3, {1.0f, 0.0f, 0.0f});
	// spawnSphere({15.0f, 0.0f, 0.0f}, 3, {1.0f, 1.0f, 0.0f});

	spawnSphere({5.0f, 4.0f, 0.0f}, 1, {1.0f, 1.0f, 1.0f});
    spawnLight({5.0f, 5.0f, 0.0f}, 25, E_SPLight);
    return ;
	
	for (int i = -20; i < 20; i += 2) {
		spawnSphere(
		   {5.0f * abs(i + i), 
			2.0f * abs(i + i), 
			5.0f * i + i},
			1, {
			1.0f * (abs(i) % 3 == 0), 
			1.0f * (abs(i) % 3 == 1), 
			1.0f * (abs(i) % 3 == 2)});
	}

	for ( double y = 0 ; y < 10 ; y++ ) {
		for ( double x = 0 ; x < 10 ; x++ ) {
		spawnSphere(
		{
			60,
			4 * y,
			4 * (x - 5)
			},
			2,
			{
			0,
			0,
			1
			});

		}
	}

	for (int i = 1; i < 50; i++) {
		spawnSphere(
		   {(100.0f * (rand() % i) - 50.0f), 
			1, 
			(100.0f * (rand() % i) - 50.0f)},
			1, {
			1.0f * (abs(i) % 3 == 0), 
			1.0f * (abs(i) % 3 == 1), 
			1.0f * (abs(i) % 3 == 2)});
	}
}

void addImage(char *text) {
	return ;
	char **image = cstrsplit(text, '-');
	int y = 0;
	while (image[y]) {
		char **colours = cstrsplit(image[y], '|');
		int x = 0;
		while (colours[x]) {
			char **color = cstrsplit(colours[x], ',');

			int r = atoi(color[0]);
			int g = atoi(color[1]);
			int b = atoi(color[2]);

			a.sdl->putpixel(x, y, Color(r, g, b));

			x++;
		}
		y++;
	}
}

int		main(int argc, char *argv[]) {
	if (argc != 3)
    {
        printf("Usage : %s\n [port] [trace part 0/1/2] - Automatically connecting to 127.0.0.1\n", argv[0]);
        return (-1);
    }
	int sockfd;
    // setup the socket
    sockfd = make_socket(argv);

	srand(1000);
	init();
	createScene();

	boundingBox->octavateWorld();

	bool running = true;
	while (running) {
		if (a.sdl->handleInput() == eKSpace) {

			time_t stime = time(NULL);
			char *image;
			image = rayTrace(atoi(argv[2]), 2);
			std::cout << time(NULL) - stime << " seconds to trace " << strlen(image) << std::endl;
			
			size_t buffer_size = sizeof(char) * strlen(image) + 1;
			

			unsigned char buffer[buffer_size];
			bzero(buffer, buffer_size);
			if (strcmp(argv[2], "1") == 0) {
				recv(sockfd, buffer, buffer_size, 0);
				ssize_t size = atoi((char *)buffer);
				ssize_t received = 0;
				while (received < size) {
					int got = recv(sockfd, &buffer[received], buffer_size, 0);
					received += got;
					printf("The server said stuff");
				}
				// addImage((char *)buffer);
			} else {
				printf("sending message\n");
				memcpy(buffer, image, buffer_size);
				

				char *size_to_send = uitoa(buffer_size);
				send(sockfd, size_to_send, strlen(size_to_send) + 1, 0);
				ssize_t sent_size;
				size_t size = 0;
				while (size < buffer_size) {
					sent_size = send(sockfd, &buffer[size], 4096, 0);
					size += sent_size;
				}



				send(sockfd, buffer, buffer_size, 0); // send your string to the other side
			}

			std::cout << "Starting display driver: " << std::endl;
			a.sdl->display();
			a.minimap->display();
			std::cout << "done" << std::endl;

		}
		a.minimap->handleInput();
		SDL_Delay(16);
	}
}
