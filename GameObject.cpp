#include "GameObject.h"

GameObject::GameObject() {
	m_x = 0;
	m_y = 0;
	m_xVel = 0;
	m_yVel = 0;
}
// Deconstructors
GameObject::~GameObject() {
}

void GameObject::spawn() {
	m_x = 0;
	m_y = 0;
	m_xVel = 0;
	m_yVel = 0;
}
void GameObject::spawn(int x, int y) {
	m_x = x;
	m_y = y;
	m_xVel = 0;
	m_yVel = 0;
}
void GameObject::spawn(int x, int y, int vx) {
	m_x = x;
	m_y = y;
	m_xVel = vx;
	m_yVel = 0;
}

void GameObject::movement() {
	m_x += m_xVel;
}

// Getter and Setter methods
int GameObject::getX() {
	return m_x;
}

int GameObject::getY() {
	return m_y;
}

int GameObject::getVelX() {
	return m_xVel;
}

int GameObject::getVelY() {
	return m_yVel;
}
bool GameObject::getAlive() {
	return m_Alive;
}

void GameObject::setX(int x) {
	m_x = x;
}

void GameObject::setY(int y) {
	m_y = y;
}

void GameObject::setVelX(int x) {
	m_xVel = x;
}
void GameObject::setVelY(int y) {
	m_yVel = y;
}
void GameObject::setAlive(bool alive) {
	m_Alive = alive;
}
