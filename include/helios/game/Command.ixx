module;

#include <memory>

export module helios.game.Command;

import helios.game.GameObject;
import helios.game.GameWorld;
import helios.util.Guid;


export namespace helios::game {

    class Command {



    public:

        virtual ~Command() = default;

        Command() = default;


        virtual void execute(helios::game::GameObject& gameObject) = 0;
    };


}