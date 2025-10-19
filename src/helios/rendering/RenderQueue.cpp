module;

#include <cassert>
#include <iostream>
#include <memory>
#include <vector>

module helios.rendering.RenderQueue;

import helios.rendering.RenderCommand;

namespace helios::rendering {

     void RenderQueue::add(std::unique_ptr<const helios::rendering::RenderCommand> renderCommand) {
        assert(renderCommand && "Received nullptr RenderCommand.");
        renderCommands_.emplace_back(std::move(renderCommand));
    }


    const std::vector<std::unique_ptr<const helios::rendering::RenderCommand>>& RenderQueue::renderCommands() const noexcept {
        return renderCommands_;
    }


    void RenderQueue::clear() {
        renderCommands_.clear();
        /**
         * @todo strategy to decide whether shrink_to_fit should only be applied
         * if expected numbers RenderCommands for the subsequent render passes is
         * less than current size?
         */
        //renderCommands_.shrink_to_fit();
    }


    size_t RenderQueue::count() const noexcept {
        return renderCommands_.size();
    }
}