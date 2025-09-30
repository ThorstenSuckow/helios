module;

export module helios.rendering.model:Shader;

export namespace helios::rendering::model {

    class Shader {

    public:

        virtual ~Shader() = default;

        virtual bool load() noexcept = 0;

        virtual bool compile() noexcept = 0;

        virtual void use() const noexcept = 0;

    };

}