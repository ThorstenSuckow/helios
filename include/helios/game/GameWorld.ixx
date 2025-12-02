module;

#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

export module helios.game.GameWorld;

import helios.game.GameObject;
import helios.util.Guid;


export namespace helios::game {

    class GameWorld {

        std::unordered_map<helios::util::Guid, std::unique_ptr<GameObject>> gameObjects_;

    public:
        helios::game::GameObject* addGameObject(std::unique_ptr<GameObject> gameObject) {

            if (!gameObject) {
                // @todo: log
                return nullptr;
            }
            if (gameObjects_.contains(gameObject->guid())) {
                // @todo: log
                return nullptr;
            }

            auto* ptr = gameObject.get();
            gameObjects_.emplace(gameObject->guid(), std::move(gameObject));

            return ptr;
        }

        [[nodiscard]] helios::game::GameObject* find(const helios::util::Guid& guid) {

            if (auto it = gameObjects_.find(guid); it != gameObjects_.end()) {
                return it->second.get();
            }

            return nullptr;
        }

        [[nodiscard]] const helios::game::GameObject* find(const helios::util::Guid& guid) const {

            if (auto it = gameObjects_.find(guid); it != gameObjects_.end()) {
                return it->second.get();
            }

            return nullptr;
        }

        std::unique_ptr<helios::game::GameObject> removeGameObject(const GameObject& gameObject) {

            auto node = gameObjects_.extract(gameObject.guid());

            if (node.empty()) {
                // @todo log
                return nullptr;
            }

            return std::move(node.mapped());

        }



    };

}