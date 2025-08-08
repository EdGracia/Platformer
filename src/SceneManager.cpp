#include "SceneManager.h"

void SceneManager::ChangeScene(std::unique_ptr<Scene> next) {
    if (current)
        current->OnExit();
    current = std::move(next);
    if (current)
        current->OnEnter();
}

void SceneManager::Update(float dt) {
    if (!current)
        return;
    current->HandleInput();
    current->Update(dt);
}

void SceneManager::Draw() const {
    if (!current)
        return;
    current->Draw();
}

void SceneManager::Clear() {
    if (current)
        current->OnExit();
    current.reset();
}
