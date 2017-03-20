#include "camera.h"

#include <stdio.h>

void Camera::setEye(Point e) {
	eye = e;
}

void Camera::setCenter(Point c) {
	center = c;
}

void Camera::setUp(Vector u) {
	up = u;
}

void Camera::setViewWidth(double w) {
	view_width = w;
}

void Camera::setViewHeight(double h) {
	view_height = h;
}

Point Camera::getEye() {
	return eye;
}

Point Camera::getCenter() {
	return center;
}

Vector Camera::getUp() {
	return up;
}

double Camera::getViewWidth() {
	return view_width;
}

double Camera::getViewHeight() {
	return view_height;
}
