#include "GameObject.h"

GameObject::GameObject(float x, float y) { position = {x, y}; }

Vector2 GameObject::GetPosition() const { return position; }

void GameObject::SetPosition(Vector2 pos) { position = pos; }

Vector2 GameObject::GetVelocity() const { return velocity; }

void GameObject::SetVelocity(Vector2 velo) { velocity = velo; }

float GameObject::GetRotation() const { return rotation; }

void GameObject::SetRotation(float r) { rotation = r; }
