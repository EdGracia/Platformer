#include "GameObject.h"

GameObject::GameObject(float x, float y) { position = {x, y}; }

Vector2 GameObject::GetPosition() const { return position; }

void GameObject::SetPosition(Vector2 pos) { position = pos; }
