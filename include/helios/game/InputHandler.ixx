module;

export module helios.game.InputHandler;

import helios.game.Command;
import helios.game.CommandBuffer;
import helios.game.InputSnapshot;
import helios.game.GameObject;
import helios.util.Guid;

export namespace helios::game {

    class InputHandler {


    public:
        virtual ~InputHandler() = default;

        virtual void handleInput(
            const helios::game::InputSnapshot& inputSnapshot,
            const GameObject& targetGameObject,
            helios::game::CommandBuffer& commandBuffer,
            float deltaTime
        ) = 0;

        virtual void handleInput(
            const helios::game::InputSnapshot& inputSnapshot,
            const helios::util::Guid& guid,
            helios::game::CommandBuffer& commandBuffer,
            float deltaTime
        ) = 0;


    };


}