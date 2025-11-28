module;


export module helios.ext.imgui.ImGuiWidget;


export namespace helios::ext::imgui {


    class ImGuiWidget {


    public:
        ImGuiWidget() = default;
        virtual ~ImGuiWidget() = default;
        virtual void draw() = 0;

    };


}