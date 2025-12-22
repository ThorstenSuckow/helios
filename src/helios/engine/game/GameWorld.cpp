module;

#include <format>
#include <memory>
#include <unordered_map>

module helios.engine.game.GameWorld;

import helios.engine.game.GameObject;
import helios.util.Guid;
import helios.util.log.Logger;
import helios.util.log.LogManager;
import helios.engine.game.UpdateContext;
import helios.scene.Scene;

namespace helios::engine::game {

    const helios::scene::Scene* GameWorld::scene() const noexcept {
        return scene_;
    }

    void GameWorld::update(helios::engine::game::UpdateContext& updateContext) const noexcept {
        for (auto& [guid, gameObject] : gameObjects_) {
            gameObject->update(updateContext);
        }
        for (auto& system : systems_) {
            system->update(updateContext);
        }
    }

    helios::engine::game::GameObject* GameWorld::addGameObject(std::unique_ptr<GameObject> gameObject) {
        if (!gameObject) {
            logger_.warn("Attempted to add null GameObject to GameWorld");
            return nullptr;
        }
        if (gameObjects_.contains(gameObject->guid())) {
            logger_.warn(std::format("GameObject with Guid {} already exists in GameWorld",
                                     gameObject->guid().value()));
            return nullptr;
        }

        auto* ptr = gameObject.get();
        gameObjects_.emplace(gameObject->guid(), std::move(gameObject));

        return ptr;
    }

    helios::engine::game::GameObject* GameWorld::find(const helios::util::Guid& guid) {
        if (auto it = gameObjects_.find(guid); it != gameObjects_.end()) {
            return it->second.get();
        }
        return nullptr;
    }

    const helios::engine::game::GameObject* GameWorld::find(const helios::util::Guid& guid) const {
        if (auto it = gameObjects_.find(guid); it != gameObjects_.end()) {
            return it->second.get();
        }
        return nullptr;
    }

    std::unique_ptr<helios::engine::game::GameObject> GameWorld::removeGameObject(const GameObject& gameObject) {
        auto node = gameObjects_.extract(gameObject.guid());

        if (node.empty()) {
            logger_.warn(std::format("Attempted to remove non-existent GameObject with Guid {} from GameWorld",
                                     gameObject.guid().value()));
            return nullptr;
        }

        return std::move(node.mapped());
    }

} // namespace helios::engine::game

